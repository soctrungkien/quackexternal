#pragma once
#include <string>
#include <vector>
#include <xxhash.h>
#include <zstd.h>
#include <blake3.h>
#include "Luau/Ast/Ast.h"
#include "Luau/Ast/Parser.h"
#include "Luau/Common/Bytecode.h"
#include "Luau/Common/BytecodeUtils.h"
#include "Luau/Compiler/Compiler.h"
#include "Luau/Compiler/BytecodeBuilder.h"
#include "nlohmann/json.hpp"

struct CompilationResult {
    bool Compiled;
    nlohmann::json Errors = nlohmann::json::array();
};

class Bytecode_Encoder : public Luau::BytecodeEncoder {
    inline void encode(uint32_t* data, size_t count) override {
        for (auto i = 0u; i < count;) {
            auto& opcode = *reinterpret_cast<uint8_t*>(data + i);
            i += Luau::getOpLength(LuauOpcode(opcode));
            opcode *= 227;
        }
    }
};

static constexpr uint8_t BYTECODE_SIGNATURE[4] = { 'R', 'S', 'B', '1' };
static constexpr uint8_t BYTECODE_HASH_MULTIPLIER = 41;
static constexpr uint32_t BYTECODE_HASH_SEED = 42u;

static constexpr uint32_t MAGIC_A = 0x4C464F52;
static constexpr uint32_t MAGIC_B = 0x946AC432;
static constexpr uint8_t  KEY_BYTES[4] = { 0x52, 0x4F, 0x46, 0x4C };

static inline uint8_t rotl8(uint8_t value, int shift) {
    shift &= 7;
    return (value << shift) | (value >> (8 - shift));
}

namespace Bytecode {
    inline std::string Compress(const std::string& BytecodeData) {
        const auto max_size = ZSTD_compressBound(BytecodeData.size());
        auto buffer = std::vector<char>(max_size + 8);

        memcpy(&buffer[0], BYTECODE_SIGNATURE, 4);

        const auto size = static_cast<uint32_t>(BytecodeData.size());
        memcpy(&buffer[4], &size, sizeof(size));

        const auto compressed_size = ZSTD_compress(&buffer[8], max_size, BytecodeData.data(), BytecodeData.size(), ZSTD_maxCLevel());
        if (ZSTD_isError(compressed_size))
            return "";

        const auto final_size = compressed_size + 8;
        buffer.resize(final_size);

        const auto hash_key = XXH32(buffer.data(), final_size, BYTECODE_HASH_SEED);
        const auto bytes = reinterpret_cast<const uint8_t*>(&hash_key);

        for (auto i = 0u; i < final_size; ++i) {
            buffer[i] ^= (bytes[i % 4] + i * BYTECODE_HASH_MULTIPLIER) & 0xFF;
        }
        return std::string(buffer.data(), final_size);
    }

    inline std::string Decompress(const std::string_view compressed) {
        const uint8_t bytecodeSignature[4] = { 'R', 'S', 'B', '1' };
        const int bytecodeHashMultiplier = 41;
        const int bytecodeHashSeed = 42;

        if (compressed.size() < 8)
            return ""; // Compressed data too short

        std::vector<uint8_t> compressedData(compressed.begin(), compressed.end());
        std::vector<uint8_t> headerBuffer(4);

        for (size_t i = 0; i < 4; ++i) {
            headerBuffer[i] = compressedData[i] ^ bytecodeSignature[i];
            headerBuffer[i] = (headerBuffer[i] - i * bytecodeHashMultiplier) % 256;
        }

        for (size_t i = 0; i < compressedData.size(); ++i) {
            compressedData[i] ^= (headerBuffer[i % 4] + i * bytecodeHashMultiplier) % 256;
        }

        uint32_t hashValue = 0;
        for (size_t i = 0; i < 4; ++i) {
            hashValue |= headerBuffer[i] << (i * 8);
        }

        uint32_t rehash = XXH32(compressedData.data(), compressedData.size(), bytecodeHashSeed);
        if (rehash != hashValue)
            return "Hash mismatch during decompression";

        uint32_t decompressedSize = 0;
        for (size_t i = 4; i < 8; ++i) {
            decompressedSize |= compressedData[i] << ((i - 4) * 8);
        }

        compressedData = std::vector<uint8_t>(compressedData.begin() + 8, compressedData.end());
        std::vector<uint8_t> decompressed(decompressedSize);

        size_t const actualDecompressedSize = ZSTD_decompress(decompressed.data(), decompressedSize, compressedData.data(), compressedData.size());
        if (ZSTD_isError(actualDecompressedSize))
            return "ZSTD decompression error: " + std::string(ZSTD_getErrorName(actualDecompressedSize));

        decompressed.resize(actualDecompressedSize);
        return std::string(decompressed.begin(), decompressed.end());
    }

    inline CompilationResult Compilable(const std::string& source) {
        Luau::ParseOptions parseOptions;
        parseOptions.noErrorLimit = true;

        Luau::Allocator allocator;
        Luau::AstNameTable names(allocator);
        Luau::ParseResult result = Luau::Parser::parse(source.c_str(), source.size(), names, allocator, parseOptions);

        if (!result.errors.empty()) {
            nlohmann::json Errors = nlohmann::json::array();

            for (const auto& err : result.errors) {
                nlohmann::json e;
                e["Line"] = std::to_string(err.getLocation().begin.line + 1);
                e["Column"] = std::to_string(err.getLocation().begin.column + 1);
                e["Error"] = std::string(err.getMessage());
                Errors.push_back(e);
            }
            return { false, Errors };
        }
        else {
            nlohmann::json Errors;
            return { true, Errors };
        }
    }

    inline std::string Compile(const std::string& source) {
        auto bytecode_encoder = Bytecode_Encoder();
        static const char* CommonGlobals[] = { "Game", "Workspace", "game", "plugin", "script", "shared", "workspace", "_G", "_ENV", nullptr };

        Luau::CompileOptions Options;
        Options.debugLevel = 1;
        Options.optimizationLevel = 1;
        Options.mutableGlobals = CommonGlobals;
        Options.vectorLib = "Vector3";
        Options.vectorCtor = "new";
        Options.vectorType = "Vector3";
        return Luau::compile(source, Options, {}, &bytecode_encoder);
    }

    inline std::string NormalCompile(const std::string& Source) {
        const std::string bytecode = Luau::compile(Source, {}, {}, nullptr);
        if (bytecode[0] == '\0') {
            std::string bytecodeP = bytecode;
            bytecodeP.erase(std::remove(bytecodeP.begin(), bytecodeP.end(), '\0'), bytecodeP.end());
            return std::string("");
        }
        return bytecode;
    }

    inline static std::string Sign(const std::string& bytecode) {
        if (bytecode.empty()) {
            return "";
        }

        constexpr uint32_t FOOTER_SIZE = 40u;
        std::vector<uint8_t> blake3_hash(32); {
            blake3_hasher hasher;
            blake3_hasher_init(&hasher);
            blake3_hasher_update(&hasher, bytecode.data(), bytecode.size());
            blake3_hasher_finalize(&hasher, blake3_hash.data(), blake3_hash.size());
        }
        std::vector<uint8_t> transformed_hash(32);

        for (int i = 0; i < 32; ++i) {
            uint8_t byte = KEY_BYTES[i & 3];
            uint8_t hash_byte = blake3_hash[i];
            uint8_t combined = byte + i;
            uint8_t result;

            switch (i & 3) {
            case 0: {
                int shift = ((combined & 3) + 1);
                result = rotl8(hash_byte ^ ~byte, shift);
                break;
            }
            case 1: {
                int shift = ((combined & 3) + 2);
                result = rotl8(byte ^ ~hash_byte, shift);
                break;
            }
            case 2: {
                int shift = ((combined & 3) + 3);
                result = rotl8(hash_byte ^ ~byte, shift);
                break;
            }
            case 3: {
                int shift = ((combined & 3) + 4);
                result = rotl8(byte ^ ~hash_byte, shift);
                break;
            }
            }
            transformed_hash[i] = result;
        }
        std::vector<uint8_t> footer(FOOTER_SIZE, 0);

        uint32_t first_hash_dword = *reinterpret_cast<uint32_t*>(transformed_hash.data());
        uint32_t footer_prefix = first_hash_dword ^ MAGIC_B;
        memcpy(&footer[0], &footer_prefix, 4);

        uint32_t xor_ed = first_hash_dword ^ MAGIC_A;
        memcpy(&footer[4], &xor_ed, 4);
        memcpy(&footer[8], transformed_hash.data(), 32);

        std::string signed_bytecode = bytecode;
        signed_bytecode.append(reinterpret_cast<const char*>(footer.data()), footer.size());
        return Compress(signed_bytecode);
    }
}
