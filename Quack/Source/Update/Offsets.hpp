#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

// Updated For version-d599f7fc52a8404c
// Last Updated At 12/03/2026
// By Xdblaze

namespace Executor {
	inline std::string Version = "v1.0.0";
	inline std::string RobloxVersion = "version-d599f7fc52a8404c";
	inline bool DebugMode = true;
}

namespace Offsets {
	namespace Enums {
		enum ModuleType :uintptr_t {
			None = 0,
			Module = 1,
			Core = 2
		};

		enum RunContext :int {
			Legacy = 0,
			Client = 1,
			Server = 2,
			Plugin = 3
		};
	}

	namespace DataModel {
		inline uintptr_t FakeDataModel = 0x81c2c38;
		inline uintptr_t RealDataModel = 0x1C0;
		inline uintptr_t ServerIP = 0x5E8;
		inline uintptr_t ScriptContext = 0x3F0;
		inline uintptr_t CreatorId = 0x188;
		inline uintptr_t GameId = 0x190;
		inline uintptr_t GameLoaded = 0x600;
		inline uintptr_t JobId = 0x138;
		inline uintptr_t PlaceId = 0x198;
		inline uintptr_t PlaceVersion = 0x1B4;
		inline uintptr_t PrimitiveCount = 0x440;
		inline uintptr_t ToRenderView1 = 0x1D0;
		inline uintptr_t ToRenderView2 = 0x08;
		inline uintptr_t ToRenderView3 = 0x28;
		inline uintptr_t Workspace = 0x178;
		inline uintptr_t Ping = 0x5E8;
	}

	namespace TaskScheduler {
		inline uintptr_t Pointer = 0x801D408;
		inline uintptr_t JobStart = 0x1D0;
		inline uintptr_t JobEnd = 0x1D8;
		inline uintptr_t JobName = 0x18;
		inline uintptr_t JobType = 0x138;
		inline uintptr_t ScriptContext = 0x1F8;
		inline uintptr_t RequireBypass = 0x878;
		inline uintptr_t TargetFPS = 0x1B0;
		inline uintptr_t FakeDataModel = 0x38;
		inline uintptr_t DataModel = 0x1B0;
	}

	namespace Workspace {
		inline uintptr_t DistributedGameTime = 0x430;
		inline uintptr_t ReadOnlyGravity = 0x978;
		inline uintptr_t Gravity = 0x1D0;
		inline uintptr_t GravityContainer = 0x3D8;
		inline uintptr_t PrimitivesPointer1 = 0x3D8;
		inline uintptr_t PrimitivesPointer2 = 0x240;
	}

	namespace Instance {
		inline uintptr_t AttributeContainer = 0x48;
		inline uintptr_t AttributeList = 0x18;
		inline uintptr_t AttributeToNext = 0x58;
		inline uintptr_t AttributeToValue = 0x18;

		inline uintptr_t This = 0x8;
		inline uintptr_t ClassDescriptorBase = 0x5A8;
		inline uintptr_t ClassDescriptor = 0x18;
		inline uintptr_t ClassName = 0x8;

		inline uintptr_t Name = 0xB0;
		inline uintptr_t Parent = 0x68;

		inline uintptr_t ChildrenStart = 0x70;
		inline uintptr_t ChildrenEnd = 0x8;

		inline uintptr_t Deleter = 0x10;
		inline uintptr_t DeleterBack = 0x18;
	}

	namespace Misc {
		inline uintptr_t Adornee = 0xD0;
		inline uintptr_t StringLength = 0x10;
		inline uintptr_t Value = 0xD0;
	}

	namespace LocalPlayer {
		inline uintptr_t Pointer = 0x130;
		inline uintptr_t UserId = 0x2C8;
		inline uintptr_t DisplayName = 0x130;
		inline uintptr_t PlayerMouse = 0xF68;
		inline uintptr_t ModelInstance = 0x380;
		inline uintptr_t CharacterAppearanceId = 0x2B8;
		inline uintptr_t ForceNewAFKDurationPointer = 0x1B8;
	}

	namespace RunService {
		inline uintptr_t HeartbeatFPS = 0xB8;
		inline uintptr_t HeartbeatTask = 0xF0;
	}

	namespace Bytecode {
		inline uintptr_t OpcodeLookupTable = 0x58E5380;
		inline uintptr_t Pointer = 0x10;
		inline uintptr_t Size = 0x20;
	}

	namespace BaseScript {
		inline uintptr_t RunContext = 0x148;
	}

	namespace LocalScript {
		inline uintptr_t ByteCode = 0x1A8;
		inline uintptr_t Hash = 0x1B8;
	}

	namespace ModuleScript {
		inline uintptr_t IsCoreScript = 0x180;
		inline uintptr_t ModuleFlags = IsCoreScript - 0x4;
		inline uintptr_t ByteCode = 0x150;
		inline uintptr_t Hash = 0x160;
	}

	namespace MouseService {
		inline uintptr_t InputObject = 0x100;
		inline uintptr_t MousePosition = 0xEC;
		inline uintptr_t SensitivityPointer = 0x7FEA7D0;
	}

	namespace Model {
		inline uintptr_t PrimaryPart = 0x260;
		inline uintptr_t Scale = 0x16C;
	}

	namespace FFLAGS {
		inline uintptr_t WebSocketServiceEnableClientCreation = 0x6750EE8;
		inline uintptr_t EnableLoadModule = 0x65317B0;
		inline uintptr_t TaskSchedulerTargetFps = 0x787D9EC;
		inline uintptr_t LockViolationInstanceCrash = 0x6256710;

		// For Desync Method
		inline uintptr_t DataSenderMaxBandwidthBps = 0x6B18E1C;
		inline uintptr_t DataSenderMaxJoinBandwidthBps = 0x6B18E20;
		inline uintptr_t PhysicsSenderMaxBandwidthBps = 0x6B18DF8;
		inline uintptr_t PhysicsSenderMaxBandwidthBpsScaling = 0x6B18DF8;
		inline uintptr_t LargeDataSenderMaxBandwidthBps = 0x6B18DF8;
		inline uintptr_t LargeDataSenderMinBandwidthBps = 0x6B18DF8;
		inline uintptr_t WorldStepMax = 0x6B1CB70;
		inline uintptr_t WorldStepsOffsetAdjustRate = 0x6B1CB64;
		inline uintptr_t HangDetectionReport = 0x7B471B0;
		inline uintptr_t HangDetectionForceCrash = 0x7B47198;
		inline uintptr_t DebugDisableTimeoutDisconnect = 0x7917128;
	}

	namespace BindableEvent {
		inline constexpr uintptr_t Signals = 0xc8;
		inline constexpr uintptr_t Function = 0x8;
		inline constexpr uintptr_t FunctionImpl = 0x30;
		inline constexpr uintptr_t ThreadRef = 0x60;
		inline constexpr uintptr_t LiveThread = 0x20;
		inline constexpr uintptr_t LuaState = 0x8;
	}

	namespace LuaState {
		inline constexpr uintptr_t Userdata = 0x40;
		inline constexpr uintptr_t GlobalState = 0x8;
	}

	namespace GlobalState {
		inline constexpr uintptr_t MainThread = 0x8;
	}

	namespace ExtraSpace {
		inline constexpr uintptr_t Identity = 0x48;
		inline constexpr uintptr_t Caps = 0x98;
	}
}

inline uintptr_t identity_to_caps(uintptr_t i) {
	uintptr_t ret = 0;
	switch (i) {
	case 4: ret = 0x2000000000000003LL; break;
	case 3: ret = 0x200000000000000BLL; break;
	case 5: ret = 0x2000000000000001LL; break;
	case 6: ret = 0x600000000000000BLL; break;
	case 8: ret = 0x200000000000003FLL; break;
	case 9: ret = 12LL; break;
	case 10: ret = 0x6000000000000003LL; break;
	case 11: ret = 0x2000000000000000LL; break;
	default: ret = 0LL; break;
	}
	return ret | 0x3FFFFFF00LL;
}

namespace Offsets {
	bool UpdateFromDefinition(const std::string& definition);
	bool RefreshFromRemote();
}

//namespace RBX {
//	using TPrint = void(__cdecl*)(int, const char*, ...);
//	inline TPrint Print = reinterpret_cast<TPrint>(Offsets::Print);
//}
