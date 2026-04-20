// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#include "Luau/Ast/Confusables.h"

#include <algorithm>
#include <array>

namespace Luau
{

struct Confusable
{
    unsigned codepoint : 24;
    char text[5];
};

// Derived from http://www.unicode.org/Public/security/10.0.0/confusables.txt; sorted by codepoint
// clang-format off
static const Confusable kConfusables[] =
{
    {34, "\""},      // MA#* ( " → '' ) QUOTATION MARK → APOSTROPHE, APOSTROPHE# # Converted to a quote.
    {48, "O"},       // MA# ( 0 → O ) DIGIT ZERO → LATIN CAPITAL LETTER O#
    {49, "l"},       // MA# ( 1 → l ) DIGIT ONE → LATIN SMALL LETTER L#
    {73, "l"},       // MA# ( I → l ) LATIN CAPITAL LETTER I → LATIN SMALL LETTER L#
    {96, "'"},       // MA#* ( ` → ' ) GRAVE ACCENT → APOSTROPHE# →ˋ→→｀→→‘→
    {109, "rn"},     // MA# ( m → rn ) LATIN SMALL LETTER M → LATIN SMALL LETTER R, LATIN SMALL LETTER N#
    {124, "l"},      // MA#* ( | → l ) VERTICAL LINE → LATIN SMALL LETTER L#
    {160, " "},      // MA#* (   →   ) NO-BREAK SPACE → SPACE#
    {180, "'"},      // MA#* ( ´ → ' ) ACUTE ACCENT → APOSTROPHE# →΄→→ʹ→
    {184, ","},      // MA#* ( ¸ → , ) CEDILLA → COMMA#
    {198, "AE"},     // MA# ( Æ → AE ) LATIN CAPITAL LETTER AE → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER E#
    {215, "x"},      // MA#* ( × → x ) MULTIPLICATION SIGN → LATIN SMALL LETTER X#
    {230, "ae"},     // MA# ( æ → ae ) LATIN SMALL LETTER AE → LATIN SMALL LETTER A, LATIN SMALL LETTER E#
    {305, "i"},      // MA# ( ı → i ) LATIN SMALL LETTER DOTLESS I → LATIN SMALL LETTER I#
    {306, "lJ"},     // MA# ( Ĳ → lJ ) LATIN CAPITAL LIGATURE IJ → LATIN SMALL LETTER L, LATIN CAPITAL LETTER J# →IJ→
    {307, "ij"},     // MA# ( ĳ → ij ) LATIN SMALL LIGATURE IJ → LATIN SMALL LETTER I, LATIN SMALL LETTER J#
    {329, "'n"},     // MA# ( ŉ → 'n ) LATIN SMALL LETTER N PRECEDED BY APOSTROPHE → APOSTROPHE, LATIN SMALL LETTER N# →ʼn→
    {338, "OE"},     // MA# ( Œ → OE ) LATIN CAPITAL LIGATURE OE → LATIN CAPITAL LETTER O, LATIN CAPITAL LETTER E#
    {339, "oe"},     // MA# ( œ → oe ) LATIN SMALL LIGATURE OE → LATIN SMALL LETTER O, LATIN SMALL LETTER E#
    {383, "f"},      // MA# ( ſ → f ) LATIN SMALL LETTER LONG S → LATIN SMALL LETTER F#
    {385, "'B"},     // MA# ( Ɓ → 'B ) LATIN CAPITAL LETTER B WITH HOOK → APOSTROPHE, LATIN CAPITAL LETTER B# →ʽB→
    {388, "b"},      // MA# ( Ƅ → b ) LATIN CAPITAL LETTER TONE SIX → LATIN SMALL LETTER B#
    {391, "C'"},     // MA# ( Ƈ → C' ) LATIN CAPITAL LETTER C WITH HOOK → LATIN CAPITAL LETTER C, APOSTROPHE# →Cʽ→
    {394, "'D"},     // MA# ( Ɗ → 'D ) LATIN CAPITAL LETTER D WITH HOOK → APOSTROPHE, LATIN CAPITAL LETTER D# →ʽD→
    {397, "g"},      // MA# ( ƍ → g ) LATIN SMALL LETTER TURNED DELTA → LATIN SMALL LETTER G#
    {403, "G'"},     // MA# ( Ɠ → G' ) LATIN CAPITAL LETTER G WITH HOOK → LATIN CAPITAL LETTER G, APOSTROPHE# →Gʽ→
    {406, "l"},      // MA# ( Ɩ → l ) LATIN CAPITAL LETTER IOTA → LATIN SMALL LETTER L#
    {408, "K'"},     // MA# ( Ƙ → K' ) LATIN CAPITAL LETTER K WITH HOOK → LATIN CAPITAL LETTER K, APOSTROPHE# →Kʽ→
    {416, "O'"},     // MA# ( Ơ → O' ) LATIN CAPITAL LETTER O WITH HORN → LATIN CAPITAL LETTER O, APOSTROPHE# →Oʼ→
    {417, "o'"},     // MA# ( ơ → o' ) LATIN SMALL LETTER O WITH HORN → LATIN SMALL LETTER O, APOSTROPHE# →oʼ→
    {420, "'P"},     // MA# ( Ƥ → 'P ) LATIN CAPITAL LETTER P WITH HOOK → APOSTROPHE, LATIN CAPITAL LETTER P# →ʽP→
    {422, "R"},      // MA# ( Ʀ → R ) LATIN LETTER YR → LATIN CAPITAL LETTER R#
    {423, "2"},      // MA# ( Ƨ → 2 ) LATIN CAPITAL LETTER TONE TWO → DIGIT TWO#
    {428, "'T"},     // MA# ( Ƭ → 'T ) LATIN CAPITAL LETTER T WITH HOOK → APOSTROPHE, LATIN CAPITAL LETTER T# →ʽT→
    {435, "'Y"},     // MA# ( Ƴ → 'Y ) LATIN CAPITAL LETTER Y WITH HOOK → APOSTROPHE, LATIN CAPITAL LETTER Y# →ʽY→
    {439, "3"},      // MA# ( Ʒ → 3 ) LATIN CAPITAL LETTER EZH → DIGIT THREE#
    {444, "5"},      // MA# ( Ƽ → 5 ) LATIN CAPITAL LETTER TONE FIVE → DIGIT FIVE#
    {445, "s"},      // MA# ( ƽ → s ) LATIN SMALL LETTER TONE FIVE → LATIN SMALL LETTER S#
    {448, "l"},      // MA# ( ǀ → l ) LATIN LETTER DENTAL CLICK → LATIN SMALL LETTER L#
    {449, "ll"},     // MA# ( ǁ → ll ) LATIN LETTER LATERAL CLICK → LATIN SMALL LETTER L, LATIN SMALL LETTER L# →‖→→∥→→||→
    {451, "!"},      // MA# ( ǃ → ! ) LATIN LETTER RETROFLEX CLICK → EXCLAMATION MARK#
    {455, "LJ"},     // MA# ( Ǉ → LJ ) LATIN CAPITAL LETTER LJ → LATIN CAPITAL LETTER L, LATIN CAPITAL LETTER J#
    {456, "Lj"},     // MA# ( ǈ → Lj ) LATIN CAPITAL LETTER L WITH SMALL LETTER J → LATIN CAPITAL LETTER L, LATIN SMALL LETTER J#
    {457, "lj"},     // MA# ( ǉ → lj ) LATIN SMALL LETTER LJ → LATIN SMALL LETTER L, LATIN SMALL LETTER J#
    {458, "NJ"},     // MA# ( Ǌ → NJ ) LATIN CAPITAL LETTER NJ → LATIN CAPITAL LETTER N, LATIN CAPITAL LETTER J#
    {459, "Nj"},     // MA# ( ǋ → Nj ) LATIN CAPITAL LETTER N WITH SMALL LETTER J → LATIN CAPITAL LETTER N, LATIN SMALL LETTER J#
    {460, "nj"},     // MA# ( ǌ → nj ) LATIN SMALL LETTER NJ → LATIN SMALL LETTER N, LATIN SMALL LETTER J#
    {497, "DZ"},     // MA# ( Ǳ → DZ ) LATIN CAPITAL LETTER DZ → LATIN CAPITAL LETTER D, LATIN CAPITAL LETTER Z#
    {498, "Dz"},     // MA# ( ǲ → Dz ) LATIN CAPITAL LETTER D WITH SMALL LETTER Z → LATIN CAPITAL LETTER D, LATIN SMALL LETTER Z#
    {499, "dz"},     // MA# ( ǳ → dz ) LATIN SMALL LETTER DZ → LATIN SMALL LETTER D, LATIN SMALL LETTER Z#
    {540, "3"},      // MA# ( Ȝ → 3 ) LATIN CAPITAL LETTER YOGH → DIGIT THREE# →Ʒ→
    {546, "8"},      // MA# ( Ȣ → 8 ) LATIN CAPITAL LETTER OU → DIGIT EIGHT#
    {547, "8"},      // MA# ( ȣ → 8 ) LATIN SMALL LETTER OU → DIGIT EIGHT#
    {577, "?"},      // MA# ( Ɂ → ? ) LATIN CAPITAL LETTER GLOTTAL STOP → QUESTION MARK# →ʔ→
    {593, "a"},      // MA# ( ɑ → a ) LATIN SMALL LETTER ALPHA → LATIN SMALL LETTER A#
    {609, "g"},      // MA# ( ɡ → g ) LATIN SMALL LETTER SCRIPT G → LATIN SMALL LETTER G#
    {611, "y"},      // MA# ( ɣ → y ) LATIN SMALL LETTER GAMMA → LATIN SMALL LETTER Y# →γ→
    {617, "i"},      // MA# ( ɩ → i ) LATIN SMALL LETTER IOTA → LATIN SMALL LETTER I#
    {618, "i"},      // MA# ( ɪ → i ) LATIN LETTER SMALL CAPITAL I → LATIN SMALL LETTER I# →ı→
    {623, "w"},      // MA# ( ɯ → w ) LATIN SMALL LETTER TURNED M → LATIN SMALL LETTER W#
    {651, "u"},      // MA# ( ʋ → u ) LATIN SMALL LETTER V WITH HOOK → LATIN SMALL LETTER U#
    {655, "y"},      // MA# ( ʏ → y ) LATIN LETTER SMALL CAPITAL Y → LATIN SMALL LETTER Y# →ү→→γ→
    {660, "?"},      // MA# ( ʔ → ? ) LATIN LETTER GLOTTAL STOP → QUESTION MARK#
    {675, "dz"},     // MA# ( ʣ → dz ) LATIN SMALL LETTER DZ DIGRAPH → LATIN SMALL LETTER D, LATIN SMALL LETTER Z#
    {678, "ts"},     // MA# ( ʦ → ts ) LATIN SMALL LETTER TS DIGRAPH → LATIN SMALL LETTER T, LATIN SMALL LETTER S#
    {682, "ls"},     // MA# ( ʪ → ls ) LATIN SMALL LETTER LS DIGRAPH → LATIN SMALL LETTER L, LATIN SMALL LETTER S#
    {683, "lz"},     // MA# ( ʫ → lz ) LATIN SMALL LETTER LZ DIGRAPH → LATIN SMALL LETTER L, LATIN SMALL LETTER Z#
    {697, "'"},      // MA# ( ʹ → ' ) MODIFIER LETTER PRIME → APOSTROPHE#
    {698, "\""},     // MA# ( ʺ → '' ) MODIFIER LETTER DOUBLE PRIME → APOSTROPHE, APOSTROPHE# →"→# Converted to a quote.
    {699, "'"},      // MA# ( ʻ → ' ) MODIFIER LETTER TURNED COMMA → APOSTROPHE# →‘→
    {700, "'"},      // MA# ( ʼ → ' ) MODIFIER LETTER APOSTROPHE → APOSTROPHE# →′→
    {701, "'"},      // MA# ( ʽ → ' ) MODIFIER LETTER REVERSED COMMA → APOSTROPHE# →‘→
    {702, "'"},      // MA# ( ʾ → ' ) MODIFIER LETTER RIGHT HALF RING → APOSTROPHE# →ʼ→→′→
    {706, "<"},      // MA#* ( ˂ → < ) MODIFIER LETTER LEFT ARROWHEAD → LESS-THAN SIGN#
    {707, ">"},      // MA#* ( ˃ → > ) MODIFIER LETTER RIGHT ARROWHEAD → GREATER-THAN SIGN#
    {708, "^"},      // MA#* ( ˄ → ^ ) MODIFIER LETTER UP ARROWHEAD → CIRCUMFLEX ACCENT#
    {710, "^"},      // MA# ( ˆ → ^ ) MODIFIER LETTER CIRCUMFLEX ACCENT → CIRCUMFLEX ACCENT#
    {712, "'"},      // MA# ( ˈ → ' ) MODIFIER LETTER VERTICAL LINE → APOSTROPHE#
    {714, "'"},      // MA# ( ˊ → ' ) MODIFIER LETTER ACUTE ACCENT → APOSTROPHE# →ʹ→→′→
    {715, "'"},      // MA# ( ˋ → ' ) MODIFIER LETTER GRAVE ACCENT → APOSTROPHE# →｀→→‘→
    {720, ":"},      // MA# ( ː → : ) MODIFIER LETTER TRIANGULAR COLON → COLON#
    {727, "-"},      // MA#* ( ˗ → - ) MODIFIER LETTER MINUS SIGN → HYPHEN-MINUS#
    {731, "i"},      // MA#* ( ˛ → i ) OGONEK → LATIN SMALL LETTER I# →ͺ→→ι→→ι→
    {732, "~"},      // MA#* ( ˜ → ~ ) SMALL TILDE → TILDE#
    {733, "\""},     // MA#* ( ˝ → '' ) DOUBLE ACUTE ACCENT → APOSTROPHE, APOSTROPHE# →"→# Converted to a quote.
    {750, "\""},     // MA# ( ˮ → '' ) MODIFIER LETTER DOUBLE APOSTROPHE → APOSTROPHE, APOSTROPHE# →″→→"→# Converted to a quote.
    {756, "'"},      // MA#* ( ˴ → ' ) MODIFIER LETTER MIDDLE GRAVE ACCENT → APOSTROPHE# →ˋ→→｀→→‘→
    {758, "\""},     // MA#* ( ˶ → '' ) MODIFIER LETTER MIDDLE DOUBLE ACUTE ACCENT → APOSTROPHE, APOSTROPHE# →˝→→"→# Converted to a quote.
    {760, ":"},      // MA#* ( ˸ → : ) MODIFIER LETTER RAISED COLON → COLON#
    {884, "'"},      // MA# ( ʹ → ' ) GREEK NUMERAL SIGN → APOSTROPHE# →′→
    {890, "i"},      // MA#* ( ͺ → i ) GREEK YPOGEGRAMMENI → LATIN SMALL LETTER I# →ι→→ι→
    {894, ";"},      // MA#* ( ; → ; ) GREEK QUESTION MARK → SEMICOLON#
    {895, "J"},      // MA# ( Ϳ → J ) GREEK CAPITAL LETTER YOT → LATIN CAPITAL LETTER J#
    {900, "'"},      // MA#* ( ΄ → ' ) GREEK TONOS → APOSTROPHE# →ʹ→
    {913, "A"},      // MA# ( Α → A ) GREEK CAPITAL LETTER ALPHA → LATIN CAPITAL LETTER A#
    {914, "B"},      // MA# ( Β → B ) GREEK CAPITAL LETTER BETA → LATIN CAPITAL LETTER B#
    {917, "E"},      // MA# ( Ε → E ) GREEK CAPITAL LETTER EPSILON → LATIN CAPITAL LETTER E#
    {918, "Z"},      // MA# ( Ζ → Z ) GREEK CAPITAL LETTER ZETA → LATIN CAPITAL LETTER Z#
    {919, "H"},      // MA# ( Η → H ) GREEK CAPITAL LETTER ETA → LATIN CAPITAL LETTER H#
    {921, "l"},      // MA# ( Ι → l ) GREEK CAPITAL LETTER IOTA → LATIN SMALL LETTER L#
    {922, "K"},      // MA# ( Κ → K ) GREEK CAPITAL LETTER KAPPA → LATIN CAPITAL LETTER K#
    {924, "M"},      // MA# ( Μ → M ) GREEK CAPITAL LETTER MU → LATIN CAPITAL LETTER M#
    {925, "N"},      // MA# ( Ν → N ) GREEK CAPITAL LETTER NU → LATIN CAPITAL LETTER N#
    {927, "O"},      // MA# ( Ο → O ) GREEK CAPITAL LETTER OMICRON → LATIN CAPITAL LETTER O#
    {929, "P"},      // MA# ( Ρ → P ) GREEK CAPITAL LETTER RHO → LATIN CAPITAL LETTER P#
    {932, "T"},      // MA# ( Τ → T ) GREEK CAPITAL LETTER TAU → LATIN CAPITAL LETTER T#
    {933, "Y"},      // MA# ( Υ → Y ) GREEK CAPITAL LETTER UPSILON → LATIN CAPITAL LETTER Y#
    {935, "X"},      // MA# ( Χ → X ) GREEK CAPITAL LETTER CHI → LATIN CAPITAL LETTER X#
    {945, "a"},      // MA# ( α → a ) GREEK SMALL LETTER ALPHA → LATIN SMALL LETTER A#
    {947, "y"},      // MA# ( γ → y ) GREEK SMALL LETTER GAMMA → LATIN SMALL LETTER Y#
    {953, "i"},      // MA# ( ι → i ) GREEK SMALL LETTER IOTA → LATIN SMALL LETTER I#
    {957, "v"},      // MA# ( ν → v ) GREEK SMALL LETTER NU → LATIN SMALL LETTER V#
    {959, "o"},      // MA# ( ο → o ) GREEK SMALL LETTER OMICRON → LATIN SMALL LETTER O#
    {961, "p"},      // MA# ( ρ → p ) GREEK SMALL LETTER RHO → LATIN SMALL LETTER P#
    {963, "o"},      // MA# ( σ → o ) GREEK SMALL LETTER SIGMA → LATIN SMALL LETTER O#
    {965, "u"},      // MA# ( υ → u ) GREEK SMALL LETTER UPSILON → LATIN SMALL LETTER U# →ʋ→
    {978, "Y"},      // MA# ( ϒ → Y ) GREEK UPSILON WITH HOOK SYMBOL → LATIN CAPITAL LETTER Y#
    {988, "F"},      // MA# ( Ϝ → F ) GREEK LETTER DIGAMMA → LATIN CAPITAL LETTER F#
    {1000, "2"},     // MA# ( Ϩ → 2 ) COPTIC CAPITAL LETTER HORI → DIGIT TWO# →Ƨ→
    {1009, "p"},     // MA# ( ϱ → p ) GREEK RHO SYMBOL → LATIN SMALL LETTER P# →ρ→
    {1010, "c"},     // MA# ( ϲ → c ) GREEK LUNATE SIGMA SYMBOL → LATIN SMALL LETTER C#
    {1011, "j"},     // MA# ( ϳ → j ) GREEK LETTER YOT → LATIN SMALL LETTER J#
    {1017, "C"},     // MA# ( Ϲ → C ) GREEK CAPITAL LUNATE SIGMA SYMBOL → LATIN CAPITAL LETTER C#
    {1018, "M"},     // MA# ( Ϻ → M ) GREEK CAPITAL LETTER SAN → LATIN CAPITAL LETTER M#
    {1029, "S"},     // MA# ( Ѕ → S ) CYRILLIC CAPITAL LETTER DZE → LATIN CAPITAL LETTER S#
    {1030, "l"},     // MA# ( І → l ) CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I → LATIN SMALL LETTER L#
    {1032, "J"},     // MA# ( Ј → J ) CYRILLIC CAPITAL LETTER JE → LATIN CAPITAL LETTER J#
    {1040, "A"},     // MA# ( А → A ) CYRILLIC CAPITAL LETTER A → LATIN CAPITAL LETTER A#
    {1042, "B"},     // MA# ( В → B ) CYRILLIC CAPITAL LETTER VE → LATIN CAPITAL LETTER B#
    {1045, "E"},     // MA# ( Е → E ) CYRILLIC CAPITAL LETTER IE → LATIN CAPITAL LETTER E#
    {1047, "3"},     // MA# ( З → 3 ) CYRILLIC CAPITAL LETTER ZE → DIGIT THREE#
    {1050, "K"},     // MA# ( К → K ) CYRILLIC CAPITAL LETTER KA → LATIN CAPITAL LETTER K#
    {1052, "M"},     // MA# ( М → M ) CYRILLIC CAPITAL LETTER EM → LATIN CAPITAL LETTER M#
    {1053, "H"},     // MA# ( Н → H ) CYRILLIC CAPITAL LETTER EN → LATIN CAPITAL LETTER H#
    {1054, "O"},     // MA# ( О → O ) CYRILLIC CAPITAL LETTER O → LATIN CAPITAL LETTER O#
    {1056, "P"},     // MA# ( Р → P ) CYRILLIC CAPITAL LETTER ER → LATIN CAPITAL LETTER P#
    {1057, "C"},     // MA# ( С → C ) CYRILLIC CAPITAL LETTER ES → LATIN CAPITAL LETTER C#
    {1058, "T"},     // MA# ( Т → T ) CYRILLIC CAPITAL LETTER TE → LATIN CAPITAL LETTER T#
    {1059, "Y"},     // MA# ( У → Y ) CYRILLIC CAPITAL LETTER U → LATIN CAPITAL LETTER Y#
    {1061, "X"},     // MA# ( Х → X ) CYRILLIC CAPITAL LETTER HA → LATIN CAPITAL LETTER X#
    {1067, "bl"},    // MA# ( Ы → bl ) CYRILLIC CAPITAL LETTER YERU → LATIN SMALL LETTER B, LATIN SMALL LETTER L# →ЬІ→→Ь1→
    {1068, "b"},     // MA# ( Ь → b ) CYRILLIC CAPITAL LETTER SOFT SIGN → LATIN SMALL LETTER B# →Ƅ→
    {1070, "lO"},    // MA# ( Ю → lO ) CYRILLIC CAPITAL LETTER YU → LATIN SMALL LETTER L, LATIN CAPITAL LETTER O# →IO→
    {1072, "a"},     // MA# ( а → a ) CYRILLIC SMALL LETTER A → LATIN SMALL LETTER A#
    {1073, "6"},     // MA# ( б → 6 ) CYRILLIC SMALL LETTER BE → DIGIT SIX#
    {1075, "r"},     // MA# ( г → r ) CYRILLIC SMALL LETTER GHE → LATIN SMALL LETTER R#
    {1077, "e"},     // MA# ( е → e ) CYRILLIC SMALL LETTER IE → LATIN SMALL LETTER E#
    {1086, "o"},     // MA# ( о → o ) CYRILLIC SMALL LETTER O → LATIN SMALL LETTER O#
    {1088, "p"},     // MA# ( р → p ) CYRILLIC SMALL LETTER ER → LATIN SMALL LETTER P#
    {1089, "c"},     // MA# ( с → c ) CYRILLIC SMALL LETTER ES → LATIN SMALL LETTER C#
    {1091, "y"},     // MA# ( у → y ) CYRILLIC SMALL LETTER U → LATIN SMALL LETTER Y#
    {1093, "x"},     // MA# ( х → x ) CYRILLIC SMALL LETTER HA → LATIN SMALL LETTER X#
    {1109, "s"},     // MA# ( ѕ → s ) CYRILLIC SMALL LETTER DZE → LATIN SMALL LETTER S#
    {1110, "i"},     // MA# ( і → i ) CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I → LATIN SMALL LETTER I#
    {1112, "j"},     // MA# ( ј → j ) CYRILLIC SMALL LETTER JE → LATIN SMALL LETTER J#
    {1121, "w"},     // MA# ( ѡ → w ) CYRILLIC SMALL LETTER OMEGA → LATIN SMALL LETTER W#
    {1140, "V"},     // MA# ( Ѵ → V ) CYRILLIC CAPITAL LETTER IZHITSA → LATIN CAPITAL LETTER V#
    {1141, "v"},     // MA# ( ѵ → v ) CYRILLIC SMALL LETTER IZHITSA → LATIN SMALL LETTER V#
    {1169, "r'"},    // MA# ( ґ → r' ) CYRILLIC SMALL LETTER GHE WITH UPTURN → LATIN SMALL LETTER R, APOSTROPHE# →гˈ→
    {1198, "Y"},     // MA# ( Ү → Y ) CYRILLIC CAPITAL LETTER STRAIGHT U → LATIN CAPITAL LETTER Y#
    {1199, "y"},     // MA# ( ү → y ) CYRILLIC SMALL LETTER STRAIGHT U → LATIN SMALL LETTER Y# →γ→
    {1211, "h"},     // MA# ( һ → h ) CYRILLIC SMALL LETTER SHHA → LATIN SMALL LETTER H#
    {1213, "e"},     // MA# ( ҽ → e ) CYRILLIC SMALL LETTER ABKHASIAN CHE → LATIN SMALL LETTER E#
    {1216, "l"},     // MA# ( Ӏ → l ) CYRILLIC LETTER PALOCHKA → LATIN SMALL LETTER L#
    {1231, "i"},     // MA# ( ӏ → i ) CYRILLIC SMALL LETTER PALOCHKA → LATIN SMALL LETTER I# →ı→
    {1236, "AE"},    // MA# ( Ӕ → AE ) CYRILLIC CAPITAL LIGATURE A IE → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER E# →Æ→
    {1237, "ae"},    // MA# ( ӕ → ae ) CYRILLIC SMALL LIGATURE A IE → LATIN SMALL LETTER A, LATIN SMALL LETTER E# →ае→
    {1248, "3"},     // MA# ( Ӡ → 3 ) CYRILLIC CAPITAL LETTER ABKHASIAN DZE → DIGIT THREE# →Ʒ→
    {1281, "d"},     // MA# ( ԁ → d ) CYRILLIC SMALL LETTER KOMI DE → LATIN SMALL LETTER D#
    {1292, "G"},     // MA# ( Ԍ → G ) CYRILLIC CAPITAL LETTER KOMI SJE → LATIN CAPITAL LETTER G#
    {1307, "q"},     // MA# ( ԛ → q ) CYRILLIC SMALL LETTER QA → LATIN SMALL LETTER Q#
    {1308, "W"},     // MA# ( Ԝ → W ) CYRILLIC CAPITAL LETTER WE → LATIN CAPITAL LETTER W#
    {1309, "w"},     // MA# ( ԝ → w ) CYRILLIC SMALL LETTER WE → LATIN SMALL LETTER W#
    {1357, "U"},     // MA# ( Ս → U ) ARMENIAN CAPITAL LETTER SEH → LATIN CAPITAL LETTER U#
    {1359, "S"},     // MA# ( Տ → S ) ARMENIAN CAPITAL LETTER TIWN → LATIN CAPITAL LETTER S#
    {1365, "O"},     // MA# ( Օ → O ) ARMENIAN CAPITAL LETTER OH → LATIN CAPITAL LETTER O#
    {1370, "'"},     // MA#* ( ՚ → ' ) ARMENIAN APOSTROPHE → APOSTROPHE# →’→
    {1373, "'"},     // MA#* ( ՝ → ' ) ARMENIAN COMMA → APOSTROPHE# →ˋ→→｀→→‘→
    {1377, "w"},     // MA# ( ա → w ) ARMENIAN SMALL LETTER AYB → LATIN SMALL LETTER W# →ɯ→
    {1379, "q"},     // MA# ( գ → q ) ARMENIAN SMALL LETTER GIM → LATIN SMALL LETTER Q#
    {1382, "q"},     // MA# ( զ → q ) ARMENIAN SMALL LETTER ZA → LATIN SMALL LETTER Q#
    {1392, "h"},     // MA# ( հ → h ) ARMENIAN SMALL LETTER HO → LATIN SMALL LETTER H#
    {1400, "n"},     // MA# ( ո → n ) ARMENIAN SMALL LETTER VO → LATIN SMALL LETTER N#
    {1404, "n"},     // MA# ( ռ → n ) ARMENIAN SMALL LETTER RA → LATIN SMALL LETTER N#
    {1405, "u"},     // MA# ( ս → u ) ARMENIAN SMALL LETTER SEH → LATIN SMALL LETTER U#
    {1409, "g"},     // MA# ( ց → g ) ARMENIAN SMALL LETTER CO → LATIN SMALL LETTER G#
    {1412, "f"},     // MA# ( ք → f ) ARMENIAN SMALL LETTER KEH → LATIN SMALL LETTER F#
    {1413, "o"},     // MA# ( օ → o ) ARMENIAN SMALL LETTER OH → LATIN SMALL LETTER O#
    {1417, ":"},     // MA#* ( ։ → : ) ARMENIAN FULL STOP → COLON#
    {1472, "l"},     // MA#* ( ‎׀‎ → l ) HEBREW PUNCTUATION PASEQ → LATIN SMALL LETTER L# →|→
    {1475, ":"},     // MA#* ( ‎׃‎ → : ) HEBREW PUNCTUATION SOF PASUQ → COLON#
    {1493, "l"},     // MA# ( ‎ו‎ → l ) HEBREW LETTER VAV → LATIN SMALL LETTER L#
    {1496, "v"},     // MA# ( ‎ט‎ → v ) HEBREW LETTER TET → LATIN SMALL LETTER V#
    {1497, "'"},     // MA# ( ‎י‎ → ' ) HEBREW LETTER YOD → APOSTROPHE#
    {1503, "l"},     // MA# ( ‎ן‎ → l ) HEBREW LETTER FINAL NUN → LATIN SMALL LETTER L#
    {1505, "o"},     // MA# ( ‎ס‎ → o ) HEBREW LETTER SAMEKH → LATIN SMALL LETTER O#
    {1520, "ll"},    // MA# ( ‎װ‎ → ll ) HEBREW LIGATURE YIDDISH DOUBLE VAV → LATIN SMALL LETTER L, LATIN SMALL LETTER L# →‎וו‎→
    {1521, "l'"},    // MA# ( ‎ױ‎ → l' ) HEBREW LIGATURE YIDDISH VAV YOD → LATIN SMALL LETTER L, APOSTROPHE# →‎וי‎→
    {1522, "\""},    // MA# ( ‎ײ‎ → '' ) HEBREW LIGATURE YIDDISH DOUBLE YOD → APOSTROPHE, APOSTROPHE# →‎יי‎→# Converted to a quote.
    {1523, "'"},     // MA#* ( ‎׳‎ → ' ) HEBREW PUNCTUATION GERESH → APOSTROPHE#
    {1524, "\""},    // MA#* ( ‎״‎ → '' ) HEBREW PUNCTUATION GERSHAYIM → APOSTROPHE, APOSTROPHE# →"→# Converted to a quote.
    {1549, ","},     // MA#* ( ‎؍‎ → , ) ARABIC DATE SEPARATOR → COMMA# →‎٫‎→
    {1575, "l"},     // MA# ( ‎ا‎ → l ) ARABIC LETTER ALEF → LATIN SMALL LETTER L# →1→
    {1607, "o"},     // MA# ( ‎ه‎ → o ) ARABIC LETTER HEH → LATIN SMALL LETTER O#
    {1632, "."},     // MA# ( ‎٠‎ → . ) ARABIC-INDIC DIGIT ZERO → FULL STOP#
    {1633, "l"},     // MA# ( ‎١‎ → l ) ARABIC-INDIC DIGIT ONE → LATIN SMALL LETTER L# →1→
    {1637, "o"},     // MA# ( ‎٥‎ → o ) ARABIC-INDIC DIGIT FIVE → LATIN SMALL LETTER O#
    {1639, "V"},     // MA# ( ‎٧‎ → V ) ARABIC-INDIC DIGIT SEVEN → LATIN CAPITAL LETTER V#
    {1643, ","},     // MA#* ( ‎٫‎ → , ) ARABIC DECIMAL SEPARATOR → COMMA#
    {1645, "*"},     // MA#* ( ‎٭‎ → * ) ARABIC FIVE POINTED STAR → ASTERISK#
    {1726, "o"},     // MA# ( ‎ھ‎ → o ) ARABIC LETTER HEH DOACHASHMEE → LATIN SMALL LETTER O# →‎ه‎→
    {1729, "o"},     // MA# ( ‎ہ‎ → o ) ARABIC LETTER HEH GOAL → LATIN SMALL LETTER O# →‎ه‎→
    {1748, "-"},     // MA#* ( ‎۔‎ → - ) ARABIC FULL STOP → HYPHEN-MINUS# →‐→
    {1749, "o"},     // MA# ( ‎ە‎ → o ) ARABIC LETTER AE → LATIN SMALL LETTER O# →‎ه‎→
    {1776, "."},     // MA# ( ۰ → . ) EXTENDED ARABIC-INDIC DIGIT ZERO → FULL STOP# →‎٠‎→
    {1777, "l"},     // MA# ( ۱ → l ) EXTENDED ARABIC-INDIC DIGIT ONE → LATIN SMALL LETTER L# →1→
    {1781, "o"},     // MA# ( ۵ → o ) EXTENDED ARABIC-INDIC DIGIT FIVE → LATIN SMALL LETTER O# →‎٥‎→
    {1783, "V"},     // MA# ( ۷ → V ) EXTENDED ARABIC-INDIC DIGIT SEVEN → LATIN CAPITAL LETTER V# →‎٧‎→
    {1793, "."},     // MA#* ( ‎܁‎ → . ) SYRIAC SUPRALINEAR FULL STOP → FULL STOP#
    {1794, "."},     // MA#* ( ‎܂‎ → . ) SYRIAC SUBLINEAR FULL STOP → FULL STOP#
    {1795, ":"},     // MA#* ( ‎܃‎ → : ) SYRIAC SUPRALINEAR COLON → COLON#
    {1796, ":"},     // MA#* ( ‎܄‎ → : ) SYRIAC SUBLINEAR COLON → COLON#
    {1984, "O"},     // MA# ( ‎߀‎ → O ) NKO DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {1994, "l"},     // MA# ( ‎ߊ‎ → l ) NKO LETTER A → LATIN SMALL LETTER L# →∣→→ǀ→
    {2036, "'"},     // MA# ( ‎ߴ‎ → ' ) NKO HIGH TONE APOSTROPHE → APOSTROPHE# →’→
    {2037, "'"},     // MA# ( ‎ߵ‎ → ' ) NKO LOW TONE APOSTROPHE → APOSTROPHE# →‘→
    {2042, "_"},     // MA# ( ‎ߺ‎ → _ ) NKO LAJANYALAN → LOW LINE#
    {2307, ":"},     // MA# ( ः → : ) DEVANAGARI SIGN VISARGA → COLON#
    {2406, "o"},     // MA# ( ० → o ) DEVANAGARI DIGIT ZERO → LATIN SMALL LETTER O#
    {2429, "?"},     // MA# ( ॽ → ? ) DEVANAGARI LETTER GLOTTAL STOP → QUESTION MARK#
    {2534, "O"},     // MA# ( ০ → O ) BENGALI DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {2538, "8"},     // MA# ( ৪ → 8 ) BENGALI DIGIT FOUR → DIGIT EIGHT#
    {2541, "9"},     // MA# ( ৭ → 9 ) BENGALI DIGIT SEVEN → DIGIT NINE#
    {2662, "o"},     // MA# ( ੦ → o ) GURMUKHI DIGIT ZERO → LATIN SMALL LETTER O#
    {2663, "9"},     // MA# ( ੧ → 9 ) GURMUKHI DIGIT ONE → DIGIT NINE#
    {2666, "8"},     // MA# ( ੪ → 8 ) GURMUKHI DIGIT FOUR → DIGIT EIGHT#
    {2691, ":"},     // MA# ( ઃ → : ) GUJARATI SIGN VISARGA → COLON#
    {2790, "o"},     // MA# ( ૦ → o ) GUJARATI DIGIT ZERO → LATIN SMALL LETTER O#
    {2819, "8"},     // MA# ( ଃ → 8 ) ORIYA SIGN VISARGA → DIGIT EIGHT#
    {2848, "O"},     // MA# ( ଠ → O ) ORIYA LETTER TTHA → LATIN CAPITAL LETTER O# →୦→→0→
    {2918, "O"},     // MA# ( ୦ → O ) ORIYA DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {2920, "9"},     // MA# ( ୨ → 9 ) ORIYA DIGIT TWO → DIGIT NINE#
    {3046, "o"},     // MA# ( ௦ → o ) TAMIL DIGIT ZERO → LATIN SMALL LETTER O#
    {3074, "o"},     // MA# ( ం → o ) TELUGU SIGN ANUSVARA → LATIN SMALL LETTER O#
    {3174, "o"},     // MA# ( ౦ → o ) TELUGU DIGIT ZERO → LATIN SMALL LETTER O#
    {3202, "o"},     // MA# ( ಂ → o ) KANNADA SIGN ANUSVARA → LATIN SMALL LETTER O#
    {3302, "o"},     // MA# ( ೦ → o ) KANNADA DIGIT ZERO → LATIN SMALL LETTER O# →౦→
    {3330, "o"},     // MA# ( ം → o ) MALAYALAM SIGN ANUSVARA → LATIN SMALL LETTER O#
    {3360, "o"},     // MA# ( ഠ → o ) MALAYALAM LETTER TTHA → LATIN SMALL LETTER O#
    {3430, "o"},     // MA# ( ൦ → o ) MALAYALAM DIGIT ZERO → LATIN SMALL LETTER O#
    {3437, "9"},     // MA# ( ൭ → 9 ) MALAYALAM DIGIT SEVEN → DIGIT NINE#
    {3458, "o"},     // MA# ( ං → o ) SINHALA SIGN ANUSVARAYA → LATIN SMALL LETTER O#
    {3664, "o"},     // MA# ( ๐ → o ) THAI DIGIT ZERO → LATIN SMALL LETTER O#
    {3792, "o"},     // MA# ( ໐ → o ) LAO DIGIT ZERO → LATIN SMALL LETTER O#
    {4125, "o"},     // MA# ( ဝ → o ) MYANMAR LETTER WA → LATIN SMALL LETTER O#
    {4160, "o"},     // MA# ( ၀ → o ) MYANMAR DIGIT ZERO → LATIN SMALL LETTER O#
    {4327, "y"},     // MA# ( ყ → y ) GEORGIAN LETTER QAR → LATIN SMALL LETTER Y#
    {4351, "o"},     // MA# ( ჿ → o ) GEORGIAN LETTER LABIAL SIGN → LATIN SMALL LETTER O#
    {4608, "U"},     // MA# ( ሀ → U ) ETHIOPIC SYLLABLE HA → LATIN CAPITAL LETTER U# →Ս→
    {4816, "O"},     // MA# ( ዐ → O ) ETHIOPIC SYLLABLE PHARYNGEAL A → LATIN CAPITAL LETTER O# →Օ→
    {5024, "D"},     // MA# ( Ꭰ → D ) CHEROKEE LETTER A → LATIN CAPITAL LETTER D#
    {5025, "R"},     // MA# ( Ꭱ → R ) CHEROKEE LETTER E → LATIN CAPITAL LETTER R#
    {5026, "T"},     // MA# ( Ꭲ → T ) CHEROKEE LETTER I → LATIN CAPITAL LETTER T#
    {5028, "O'"},    // MA# ( Ꭴ → O' ) CHEROKEE LETTER U → LATIN CAPITAL LETTER O, APOSTROPHE# →Ơ→→Oʼ→
    {5029, "i"},     // MA# ( Ꭵ → i ) CHEROKEE LETTER V → LATIN SMALL LETTER I#
    {5033, "Y"},     // MA# ( Ꭹ → Y ) CHEROKEE LETTER GI → LATIN CAPITAL LETTER Y#
    {5034, "A"},     // MA# ( Ꭺ → A ) CHEROKEE LETTER GO → LATIN CAPITAL LETTER A#
    {5035, "J"},     // MA# ( Ꭻ → J ) CHEROKEE LETTER GU → LATIN CAPITAL LETTER J#
    {5036, "E"},     // MA# ( Ꭼ → E ) CHEROKEE LETTER GV → LATIN CAPITAL LETTER E#
    {5038, "?"},     // MA# ( Ꭾ → ? ) CHEROKEE LETTER HE → QUESTION MARK# →Ɂ→→ʔ→
    {5043, "W"},     // MA# ( Ꮃ → W ) CHEROKEE LETTER LA → LATIN CAPITAL LETTER W#
    {5047, "M"},     // MA# ( Ꮇ → M ) CHEROKEE LETTER LU → LATIN CAPITAL LETTER M#
    {5051, "H"},     // MA# ( Ꮋ → H ) CHEROKEE LETTER MI → LATIN CAPITAL LETTER H#
    {5053, "Y"},     // MA# ( Ꮍ → Y ) CHEROKEE LETTER MU → LATIN CAPITAL LETTER Y# →Ꭹ→
    {5056, "G"},     // MA# ( Ꮐ → G ) CHEROKEE LETTER NAH → LATIN CAPITAL LETTER G#
    {5058, "h"},     // MA# ( Ꮒ → h ) CHEROKEE LETTER NI → LATIN SMALL LETTER H#
    {5059, "Z"},     // MA# ( Ꮓ → Z ) CHEROKEE LETTER NO → LATIN CAPITAL LETTER Z#
    {5070, "4"},     // MA# ( Ꮞ → 4 ) CHEROKEE LETTER SE → DIGIT FOUR#
    {5071, "b"},     // MA# ( Ꮟ → b ) CHEROKEE LETTER SI → LATIN SMALL LETTER B#
    {5074, "R"},     // MA# ( Ꮢ → R ) CHEROKEE LETTER SV → LATIN CAPITAL LETTER R#
    {5076, "W"},     // MA# ( Ꮤ → W ) CHEROKEE LETTER TA → LATIN CAPITAL LETTER W#
    {5077, "S"},     // MA# ( Ꮥ → S ) CHEROKEE LETTER DE → LATIN CAPITAL LETTER S#
    {5081, "V"},     // MA# ( Ꮩ → V ) CHEROKEE LETTER DO → LATIN CAPITAL LETTER V#
    {5082, "S"},     // MA# ( Ꮪ → S ) CHEROKEE LETTER DU → LATIN CAPITAL LETTER S#
    {5086, "L"},     // MA# ( Ꮮ → L ) CHEROKEE LETTER TLE → LATIN CAPITAL LETTER L#
    {5087, "C"},     // MA# ( Ꮯ → C ) CHEROKEE LETTER TLI → LATIN CAPITAL LETTER C#
    {5090, "P"},     // MA# ( Ꮲ → P ) CHEROKEE LETTER TLV → LATIN CAPITAL LETTER P#
    {5094, "K"},     // MA# ( Ꮶ → K ) CHEROKEE LETTER TSO → LATIN CAPITAL LETTER K#
    {5095, "d"},     // MA# ( Ꮷ → d ) CHEROKEE LETTER TSU → LATIN SMALL LETTER D#
    {5102, "6"},     // MA# ( Ꮾ → 6 ) CHEROKEE LETTER WV → DIGIT SIX#
    {5107, "G"},     // MA# ( Ᏻ → G ) CHEROKEE LETTER YU → LATIN CAPITAL LETTER G#
    {5108, "B"},     // MA# ( Ᏼ → B ) CHEROKEE LETTER YV → LATIN CAPITAL LETTER B#
    {5120, "="},     // MA#* ( ᐀ → = ) CANADIAN SYLLABICS HYPHEN → EQUALS SIGN#
    {5167, "V"},     // MA# ( ᐯ → V ) CANADIAN SYLLABICS PE → LATIN CAPITAL LETTER V#
    {5171, ">"},     // MA# ( ᐳ → > ) CANADIAN SYLLABICS PO → GREATER-THAN SIGN#
    {5176, "<"},     // MA# ( ᐸ → < ) CANADIAN SYLLABICS PA → LESS-THAN SIGN#
    {5194, "'"},     // MA# ( ᑊ → ' ) CANADIAN SYLLABICS WEST-CREE P → APOSTROPHE# →ˈ→
    {5196, "U"},     // MA# ( ᑌ → U ) CANADIAN SYLLABICS TE → LATIN CAPITAL LETTER U#
    {5223, "U'"},    // MA# ( ᑧ → U' ) CANADIAN SYLLABICS TTE → LATIN CAPITAL LETTER U, APOSTROPHE# →ᑌᑊ→→ᑌ'→
    {5229, "P"},     // MA# ( ᑭ → P ) CANADIAN SYLLABICS KI → LATIN CAPITAL LETTER P#
    {5231, "d"},     // MA# ( ᑯ → d ) CANADIAN SYLLABICS KO → LATIN SMALL LETTER D#
    {5254, "P'"},    // MA# ( ᒆ → P' ) CANADIAN SYLLABICS SOUTH-SLAVEY KIH → LATIN CAPITAL LETTER P, APOSTROPHE# →ᑭᑊ→
    {5255, "d'"},    // MA# ( ᒇ → d' ) CANADIAN SYLLABICS SOUTH-SLAVEY KOH → LATIN SMALL LETTER D, APOSTROPHE# →ᑯᑊ→
    {5261, "J"},     // MA# ( ᒍ → J ) CANADIAN SYLLABICS CO → LATIN CAPITAL LETTER J#
    {5290, "L"},     // MA# ( ᒪ → L ) CANADIAN SYLLABICS MA → LATIN CAPITAL LETTER L#
    {5311, "2"},     // MA# ( ᒿ → 2 ) CANADIAN SYLLABICS SAYISI M → DIGIT TWO#
    {5441, "x"},     // MA# ( ᕁ → x ) CANADIAN SYLLABICS SAYISI YI → LATIN SMALL LETTER X# →᙮→
    {5500, "H"},     // MA# ( ᕼ → H ) CANADIAN SYLLABICS NUNAVUT H → LATIN CAPITAL LETTER H#
    {5501, "x"},     // MA# ( ᕽ → x ) CANADIAN SYLLABICS HK → LATIN SMALL LETTER X# →ᕁ→→᙮→
    {5511, "R"},     // MA# ( ᖇ → R ) CANADIAN SYLLABICS TLHI → LATIN CAPITAL LETTER R#
    {5551, "b"},     // MA# ( ᖯ → b ) CANADIAN SYLLABICS AIVILIK B → LATIN SMALL LETTER B#
    {5556, "F"},     // MA# ( ᖴ → F ) CANADIAN SYLLABICS BLACKFOOT WE → LATIN CAPITAL LETTER F#
    {5573, "A"},     // MA# ( ᗅ → A ) CANADIAN SYLLABICS CARRIER GHO → LATIN CAPITAL LETTER A#
    {5598, "D"},     // MA# ( ᗞ → D ) CANADIAN SYLLABICS CARRIER THE → LATIN CAPITAL LETTER D#
    {5610, "D"},     // MA# ( ᗪ → D ) CANADIAN SYLLABICS CARRIER PE → LATIN CAPITAL LETTER D# →ᗞ→
    {5616, "M"},     // MA# ( ᗰ → M ) CANADIAN SYLLABICS CARRIER GO → LATIN CAPITAL LETTER M#
    {5623, "B"},     // MA# ( ᗷ → B ) CANADIAN SYLLABICS CARRIER KHE → LATIN CAPITAL LETTER B#
    {5741, "X"},     // MA#* ( ᙭ → X ) CANADIAN SYLLABICS CHI SIGN → LATIN CAPITAL LETTER X#
    {5742, "x"},     // MA#* ( ᙮ → x ) CANADIAN SYLLABICS FULL STOP → LATIN SMALL LETTER X#
    {5760, " "},     // MA#* (   →   ) OGHAM SPACE MARK → SPACE#
    {5810, "<"},     // MA# ( ᚲ → < ) RUNIC LETTER KAUNA → LESS-THAN SIGN#
    {5815, "X"},     // MA# ( ᚷ → X ) RUNIC LETTER GEBO GYFU G → LATIN CAPITAL LETTER X#
    {5825, "l"},     // MA# ( ᛁ → l ) RUNIC LETTER ISAZ IS ISS I → LATIN SMALL LETTER L# →I→
    {5836, "'"},     // MA# ( ᛌ → ' ) RUNIC LETTER SHORT-TWIG-SOL S → APOSTROPHE#
    {5845, "K"},     // MA# ( ᛕ → K ) RUNIC LETTER OPEN-P → LATIN CAPITAL LETTER K#
    {5846, "M"},     // MA# ( ᛖ → M ) RUNIC LETTER EHWAZ EH E → LATIN CAPITAL LETTER M#
    {5868, ":"},     // MA#* ( ᛬ → : ) RUNIC MULTIPLE PUNCTUATION → COLON#
    {5869, "+"},     // MA#* ( ᛭ → + ) RUNIC CROSS PUNCTUATION → PLUS SIGN#
    {5941, "/"},     // MA#* ( ᜵ → / ) PHILIPPINE SINGLE PUNCTUATION → SOLIDUS#
    {6147, ":"},     // MA#* ( ᠃ → : ) MONGOLIAN FULL STOP → COLON#
    {6153, ":"},     // MA#* ( ᠉ → : ) MONGOLIAN MANCHU FULL STOP → COLON#
    {7379, "\""},    // MA#* ( ᳓ → '' ) VEDIC SIGN NIHSHVASA → APOSTROPHE, APOSTROPHE# →″→→"→# Converted to a quote.
    {7428, "c"},     // MA# ( ᴄ → c ) LATIN LETTER SMALL CAPITAL C → LATIN SMALL LETTER C#
    {7439, "o"},     // MA# ( ᴏ → o ) LATIN LETTER SMALL CAPITAL O → LATIN SMALL LETTER O#
    {7441, "o"},     // MA# ( ᴑ → o ) LATIN SMALL LETTER SIDEWAYS O → LATIN SMALL LETTER O#
    {7452, "u"},     // MA# ( ᴜ → u ) LATIN LETTER SMALL CAPITAL U → LATIN SMALL LETTER U#
    {7456, "v"},     // MA# ( ᴠ → v ) LATIN LETTER SMALL CAPITAL V → LATIN SMALL LETTER V#
    {7457, "w"},     // MA# ( ᴡ → w ) LATIN LETTER SMALL CAPITAL W → LATIN SMALL LETTER W#
    {7458, "z"},     // MA# ( ᴢ → z ) LATIN LETTER SMALL CAPITAL Z → LATIN SMALL LETTER Z#
    {7462, "r"},     // MA# ( ᴦ → r ) GREEK LETTER SMALL CAPITAL GAMMA → LATIN SMALL LETTER R# →г→
    {7531, "ue"},    // MA# ( ᵫ → ue ) LATIN SMALL LETTER UE → LATIN SMALL LETTER U, LATIN SMALL LETTER E#
    {7555, "g"},     // MA# ( ᶃ → g ) LATIN SMALL LETTER G WITH PALATAL HOOK → LATIN SMALL LETTER G#
    {7564, "y"},     // MA# ( ᶌ → y ) LATIN SMALL LETTER V WITH PALATAL HOOK → LATIN SMALL LETTER Y#
    {7837, "f"},     // MA# ( ẝ → f ) LATIN SMALL LETTER LONG S WITH HIGH STROKE → LATIN SMALL LETTER F#
    {7935, "y"},     // MA# ( ỿ → y ) LATIN SMALL LETTER Y WITH LOOP → LATIN SMALL LETTER Y#
    {8125, "'"},     // MA#* ( ᾽ → ' ) GREEK KORONIS → APOSTROPHE# →’→
    {8126, "i"},     // MA# ( ι → i ) GREEK PROSGEGRAMMENI → LATIN SMALL LETTER I# →ι→
    {8127, "'"},     // MA#* ( ᾿ → ' ) GREEK PSILI → APOSTROPHE# →’→
    {8128, "~"},     // MA#* ( ῀ → ~ ) GREEK PERISPOMENI → TILDE# →˜→
    {8175, "'"},     // MA#* ( ` → ' ) GREEK VARIA → APOSTROPHE# →ˋ→→｀→→‘→
    {8189, "'"},     // MA#* ( ´ → ' ) GREEK OXIA → APOSTROPHE# →´→→΄→→ʹ→
    {8190, "'"},     // MA#* ( ῾ → ' ) GREEK DASIA → APOSTROPHE# →‛→→′→
    {8192, " "},     // MA#* (   →   ) EN QUAD → SPACE#
    {8193, " "},     // MA#* (   →   ) EM QUAD → SPACE#
    {8194, " "},     // MA#* (   →   ) EN SPACE → SPACE#
    {8195, " "},     // MA#* (   →   ) EM SPACE → SPACE#
    {8196, " "},     // MA#* (   →   ) THREE-PER-EM SPACE → SPACE#
    {8197, " "},     // MA#* (   →   ) FOUR-PER-EM SPACE → SPACE#
    {8198, " "},     // MA#* (   →   ) SIX-PER-EM SPACE → SPACE#
    {8199, " "},     // MA#* (   →   ) FIGURE SPACE → SPACE#
    {8200, " "},     // MA#* (   →   ) PUNCTUATION SPACE → SPACE#
    {8201, " "},     // MA#* (   →   ) THIN SPACE → SPACE#
    {8202, " "},     // MA#* (   →   ) HAIR SPACE → SPACE#
    {8208, "-"},     // MA#* ( ‐ → - ) HYPHEN → HYPHEN-MINUS#
    {8209, "-"},     // MA#* ( ‑ → - ) NON-BREAKING HYPHEN → HYPHEN-MINUS#
    {8210, "-"},     // MA#* ( ‒ → - ) FIGURE DASH → HYPHEN-MINUS#
    {8211, "-"},     // MA#* ( – → - ) EN DASH → HYPHEN-MINUS#
    {8214, "ll"},    // MA#* ( ‖ → ll ) DOUBLE VERTICAL LINE → LATIN SMALL LETTER L, LATIN SMALL LETTER L# →∥→→||→
    {8216, "'"},     // MA#* ( ‘ → ' ) LEFT SINGLE QUOTATION MARK → APOSTROPHE#
    {8217, "'"},     // MA#* ( ’ → ' ) RIGHT SINGLE QUOTATION MARK → APOSTROPHE#
    {8218, ","},     // MA#* ( ‚ → , ) SINGLE LOW-9 QUOTATION MARK → COMMA#
    {8219, "'"},     // MA#* ( ‛ → ' ) SINGLE HIGH-REVERSED-9 QUOTATION MARK → APOSTROPHE# →′→
    {8220, "\""},    // MA#* ( “ → '' ) LEFT DOUBLE QUOTATION MARK → APOSTROPHE, APOSTROPHE# →"→# Converted to a quote.
    {8221, "\""},    // MA#* ( ” → '' ) RIGHT DOUBLE QUOTATION MARK → APOSTROPHE, APOSTROPHE# →"→# Converted to a quote.
    {8223, "\""},    // MA#* ( ‟ → '' ) DOUBLE HIGH-REVERSED-9 QUOTATION MARK → APOSTROPHE, APOSTROPHE# →“→→"→# Converted to a quote.
    {8228, "."},     // MA#* ( ․ → . ) ONE DOT LEADER → FULL STOP#
    {8229, ".."},    // MA#* ( ‥ → .. ) TWO DOT LEADER → FULL STOP, FULL STOP#
    {8230, "..."},   // MA#* ( … → ... ) HORIZONTAL ELLIPSIS → FULL STOP, FULL STOP, FULL STOP#
    {8232, " "},     // MA#* (  →   ) LINE SEPARATOR → SPACE#
    {8233, " "},     // MA#* (  →   ) PARAGRAPH SEPARATOR → SPACE#
    {8239, " "},     // MA#* (   →   ) NARROW NO-BREAK SPACE → SPACE#
    {8242, "'"},     // MA#* ( ′ → ' ) PRIME → APOSTROPHE#
    {8243, "\""},    // MA#* ( ″ → '' ) DOUBLE PRIME → APOSTROPHE, APOSTROPHE# →"→# Converted to a quote.
    {8244, "'''"},   // MA#* ( ‴ → ''' ) TRIPLE PRIME → APOSTROPHE, APOSTROPHE, APOSTROPHE# →′′′→
    {8245, "'"},     // MA#* ( ‵ → ' ) REVERSED PRIME → APOSTROPHE# →ʽ→→‘→
    {8246, "\""},    // MA#* ( ‶ → '' ) REVERSED DOUBLE PRIME → APOSTROPHE, APOSTROPHE# →‵‵→# Converted to a quote.
    {8247, "'''"},   // MA#* ( ‷ → ''' ) REVERSED TRIPLE PRIME → APOSTROPHE, APOSTROPHE, APOSTROPHE# →‵‵‵→
    {8249, "<"},     // MA#* ( ‹ → < ) SINGLE LEFT-POINTING ANGLE QUOTATION MARK → LESS-THAN SIGN#
    {8250, ">"},     // MA#* ( › → > ) SINGLE RIGHT-POINTING ANGLE QUOTATION MARK → GREATER-THAN SIGN#
    {8252, "!!"},    // MA#* ( ‼ → !! ) DOUBLE EXCLAMATION MARK → EXCLAMATION MARK, EXCLAMATION MARK#
    {8257, "/"},     // MA#* ( ⁁ → / ) CARET INSERTION POINT → SOLIDUS#
    {8259, "-"},     // MA#* ( ⁃ → - ) HYPHEN BULLET → HYPHEN-MINUS# →‐→
    {8260, "/"},     // MA#* ( ⁄ → / ) FRACTION SLASH → SOLIDUS#
    {8263, "??"},    // MA#* ( ⁇ → ?? ) DOUBLE QUESTION MARK → QUESTION MARK, QUESTION MARK#
    {8264, "?!"},    // MA#* ( ⁈ → ?! ) QUESTION EXCLAMATION MARK → QUESTION MARK, EXCLAMATION MARK#
    {8265, "!?"},    // MA#* ( ⁉ → !? ) EXCLAMATION QUESTION MARK → EXCLAMATION MARK, QUESTION MARK#
    {8270, "*"},     // MA#* ( ⁎ → * ) LOW ASTERISK → ASTERISK#
    {8275, "~"},     // MA#* ( ⁓ → ~ ) SWUNG DASH → TILDE#
    {8279, "''''"},  // MA#* ( ⁗ → '''' ) QUADRUPLE PRIME → APOSTROPHE, APOSTROPHE, APOSTROPHE, APOSTROPHE# →′′′′→
    {8282, ":"},     // MA#* ( ⁚ → : ) TWO DOT PUNCTUATION → COLON#
    {8287, " "},     // MA#* (   →   ) MEDIUM MATHEMATICAL SPACE → SPACE#
    {8360, "Rs"},    // MA#* ( ₨ → Rs ) RUPEE SIGN → LATIN CAPITAL LETTER R, LATIN SMALL LETTER S#
    {8374, "lt"},    // MA#* ( ₶ → lt ) LIVRE TOURNOIS SIGN → LATIN SMALL LETTER L, LATIN SMALL LETTER T#
    {8448, "a/c"},   // MA#* ( ℀ → a/c ) ACCOUNT OF → LATIN SMALL LETTER A, SOLIDUS, LATIN SMALL LETTER C#
    {8449, "a/s"},   // MA#* ( ℁ → a/s ) ADDRESSED TO THE SUBJECT → LATIN SMALL LETTER A, SOLIDUS, LATIN SMALL LETTER S#
    {8450, "C"},     // MA# ( ℂ → C ) DOUBLE-STRUCK CAPITAL C → LATIN CAPITAL LETTER C#
    {8453, "c/o"},   // MA#* ( ℅ → c/o ) CARE OF → LATIN SMALL LETTER C, SOLIDUS, LATIN SMALL LETTER O#
    {8454, "c/u"},   // MA#* ( ℆ → c/u ) CADA UNA → LATIN SMALL LETTER C, SOLIDUS, LATIN SMALL LETTER U#
    {8458, "g"},     // MA# ( ℊ → g ) SCRIPT SMALL G → LATIN SMALL LETTER G#
    {8459, "H"},     // MA# ( ℋ → H ) SCRIPT CAPITAL H → LATIN CAPITAL LETTER H#
    {8460, "H"},     // MA# ( ℌ → H ) BLACK-LETTER CAPITAL H → LATIN CAPITAL LETTER H#
    {8461, "H"},     // MA# ( ℍ → H ) DOUBLE-STRUCK CAPITAL H → LATIN CAPITAL LETTER H#
    {8462, "h"},     // MA# ( ℎ → h ) PLANCK CONSTANT → LATIN SMALL LETTER H#
    {8464, "l"},     // MA# ( ℐ → l ) SCRIPT CAPITAL I → LATIN SMALL LETTER L# →I→
    {8465, "l"},     // MA# ( ℑ → l ) BLACK-LETTER CAPITAL I → LATIN SMALL LETTER L# →I→
    {8466, "L"},     // MA# ( ℒ → L ) SCRIPT CAPITAL L → LATIN CAPITAL LETTER L#
    {8467, "l"},     // MA# ( ℓ → l ) SCRIPT SMALL L → LATIN SMALL LETTER L#
    {8469, "N"},     // MA# ( ℕ → N ) DOUBLE-STRUCK CAPITAL N → LATIN CAPITAL LETTER N#
    {8470, "No"},    // MA#* ( № → No ) NUMERO SIGN → LATIN CAPITAL LETTER N, LATIN SMALL LETTER O#
    {8473, "P"},     // MA# ( ℙ → P ) DOUBLE-STRUCK CAPITAL P → LATIN CAPITAL LETTER P#
    {8474, "Q"},     // MA# ( ℚ → Q ) DOUBLE-STRUCK CAPITAL Q → LATIN CAPITAL LETTER Q#
    {8475, "R"},     // MA# ( ℛ → R ) SCRIPT CAPITAL R → LATIN CAPITAL LETTER R#
    {8476, "R"},     // MA# ( ℜ → R ) BLACK-LETTER CAPITAL R → LATIN CAPITAL LETTER R#
    {8477, "R"},     // MA# ( ℝ → R ) DOUBLE-STRUCK CAPITAL R → LATIN CAPITAL LETTER R#
    {8481, "TEL"},   // MA#* ( ℡ → TEL ) TELEPHONE SIGN → LATIN CAPITAL LETTER T, LATIN CAPITAL LETTER E, LATIN CAPITAL LETTER L#
    {8484, "Z"},     // MA# ( ℤ → Z ) DOUBLE-STRUCK CAPITAL Z → LATIN CAPITAL LETTER Z#
    {8488, "Z"},     // MA# ( ℨ → Z ) BLACK-LETTER CAPITAL Z → LATIN CAPITAL LETTER Z#
    {8490, "K"},     // MA# ( K → K ) KELVIN SIGN → LATIN CAPITAL LETTER K#
    {8492, "B"},     // MA# ( ℬ → B ) SCRIPT CAPITAL B → LATIN CAPITAL LETTER B#
    {8493, "C"},     // MA# ( ℭ → C ) BLACK-LETTER CAPITAL C → LATIN CAPITAL LETTER C#
    {8494, "e"},     // MA# ( ℮ → e ) ESTIMATED SYMBOL → LATIN SMALL LETTER E#
    {8495, "e"},     // MA# ( ℯ → e ) SCRIPT SMALL E → LATIN SMALL LETTER E#
    {8496, "E"},     // MA# ( ℰ → E ) SCRIPT CAPITAL E → LATIN CAPITAL LETTER E#
    {8497, "F"},     // MA# ( ℱ → F ) SCRIPT CAPITAL F → LATIN CAPITAL LETTER F#
    {8499, "M"},     // MA# ( ℳ → M ) SCRIPT CAPITAL M → LATIN CAPITAL LETTER M#
    {8500, "o"},     // MA# ( ℴ → o ) SCRIPT SMALL O → LATIN SMALL LETTER O#
    {8505, "i"},     // MA# ( ℹ → i ) INFORMATION SOURCE → LATIN SMALL LETTER I#
    {8507, "FAX"},   // MA#* ( ℻ → FAX ) FACSIMILE SIGN → LATIN CAPITAL LETTER F, LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER X#
    {8509, "y"},     // MA# ( ℽ → y ) DOUBLE-STRUCK SMALL GAMMA → LATIN SMALL LETTER Y# →γ→
    {8517, "D"},     // MA# ( ⅅ → D ) DOUBLE-STRUCK ITALIC CAPITAL D → LATIN CAPITAL LETTER D#
    {8518, "d"},     // MA# ( ⅆ → d ) DOUBLE-STRUCK ITALIC SMALL D → LATIN SMALL LETTER D#
    {8519, "e"},     // MA# ( ⅇ → e ) DOUBLE-STRUCK ITALIC SMALL E → LATIN SMALL LETTER E#
    {8520, "i"},     // MA# ( ⅈ → i ) DOUBLE-STRUCK ITALIC SMALL I → LATIN SMALL LETTER I#
    {8521, "j"},     // MA# ( ⅉ → j ) DOUBLE-STRUCK ITALIC SMALL J → LATIN SMALL LETTER J#
    {8544, "l"},     // MA# ( Ⅰ → l ) ROMAN NUMERAL ONE → LATIN SMALL LETTER L# →Ӏ→
    {8545, "ll"},    // MA# ( Ⅱ → ll ) ROMAN NUMERAL TWO → LATIN SMALL LETTER L, LATIN SMALL LETTER L# →II→
    {8546, "lll"},   // MA# ( Ⅲ → lll ) ROMAN NUMERAL THREE → LATIN SMALL LETTER L, LATIN SMALL LETTER L, LATIN SMALL LETTER L# →III→
    {8547, "lV"},    // MA# ( Ⅳ → lV ) ROMAN NUMERAL FOUR → LATIN SMALL LETTER L, LATIN CAPITAL LETTER V# →IV→
    {8548, "V"},     // MA# ( Ⅴ → V ) ROMAN NUMERAL FIVE → LATIN CAPITAL LETTER V#
    {8549, "Vl"},    // MA# ( Ⅵ → Vl ) ROMAN NUMERAL SIX → LATIN CAPITAL LETTER V, LATIN SMALL LETTER L# →VI→
    {8550, "Vll"},   // MA# ( Ⅶ → Vll ) ROMAN NUMERAL SEVEN → LATIN CAPITAL LETTER V, LATIN SMALL LETTER L, LATIN SMALL LETTER L# →VII→
    {8551, "Vlll"},  // MA# ( Ⅷ → Vlll ) ROMAN NUMERAL EIGHT → LATIN CAPITAL LETTER V, LATIN SMALL LETTER L, LATIN SMALL LETTER L, LATIN SMALL LETTER L# →VIII→
    {8552, "lX"},    // MA# ( Ⅸ → lX ) ROMAN NUMERAL NINE → LATIN SMALL LETTER L, LATIN CAPITAL LETTER X# →IX→
    {8553, "X"},     // MA# ( Ⅹ → X ) ROMAN NUMERAL TEN → LATIN CAPITAL LETTER X#
    {8554, "Xl"},    // MA# ( Ⅺ → Xl ) ROMAN NUMERAL ELEVEN → LATIN CAPITAL LETTER X, LATIN SMALL LETTER L# →XI→
    {8555, "Xll"},   // MA# ( Ⅻ → Xll ) ROMAN NUMERAL TWELVE → LATIN CAPITAL LETTER X, LATIN SMALL LETTER L, LATIN SMALL LETTER L# →XII→
    {8556, "L"},     // MA# ( Ⅼ → L ) ROMAN NUMERAL FIFTY → LATIN CAPITAL LETTER L#
    {8557, "C"},     // MA# ( Ⅽ → C ) ROMAN NUMERAL ONE HUNDRED → LATIN CAPITAL LETTER C#
    {8558, "D"},     // MA# ( Ⅾ → D ) ROMAN NUMERAL FIVE HUNDRED → LATIN CAPITAL LETTER D#
    {8559, "M"},     // MA# ( Ⅿ → M ) ROMAN NUMERAL ONE THOUSAND → LATIN CAPITAL LETTER M#
    {8560, "i"},     // MA# ( ⅰ → i ) SMALL ROMAN NUMERAL ONE → LATIN SMALL LETTER I#
    {8561, "ii"},    // MA# ( ⅱ → ii ) SMALL ROMAN NUMERAL TWO → LATIN SMALL LETTER I, LATIN SMALL LETTER I#
    {8562, "iii"},   // MA# ( ⅲ → iii ) SMALL ROMAN NUMERAL THREE → LATIN SMALL LETTER I, LATIN SMALL LETTER I, LATIN SMALL LETTER I#
    {8563, "iv"},    // MA# ( ⅳ → iv ) SMALL ROMAN NUMERAL FOUR → LATIN SMALL LETTER I, LATIN SMALL LETTER V#
    {8564, "v"},     // MA# ( ⅴ → v ) SMALL ROMAN NUMERAL FIVE → LATIN SMALL LETTER V#
    {8565, "vi"},    // MA# ( ⅵ → vi ) SMALL ROMAN NUMERAL SIX → LATIN SMALL LETTER V, LATIN SMALL LETTER I#
    {8566, "vii"},   // MA# ( ⅶ → vii ) SMALL ROMAN NUMERAL SEVEN → LATIN SMALL LETTER V, LATIN SMALL LETTER I, LATIN SMALL LETTER I#
    {8567, "viii"},  // MA# ( ⅷ → viii ) SMALL ROMAN NUMERAL EIGHT → LATIN SMALL LETTER V, LATIN SMALL LETTER I, LATIN SMALL LETTER I, LATIN SMALL LETTER I#
    {8568, "ix"},    // MA# ( ⅸ → ix ) SMALL ROMAN NUMERAL NINE → LATIN SMALL LETTER I, LATIN SMALL LETTER X#
    {8569, "x"},     // MA# ( ⅹ → x ) SMALL ROMAN NUMERAL TEN → LATIN SMALL LETTER X#
    {8570, "xi"},    // MA# ( ⅺ → xi ) SMALL ROMAN NUMERAL ELEVEN → LATIN SMALL LETTER X, LATIN SMALL LETTER I#
    {8571, "xii"},   // MA# ( ⅻ → xii ) SMALL ROMAN NUMERAL TWELVE → LATIN SMALL LETTER X, LATIN SMALL LETTER I, LATIN SMALL LETTER I#
    {8572, "l"},     // MA# ( ⅼ → l ) SMALL ROMAN NUMERAL FIFTY → LATIN SMALL LETTER L#
    {8573, "c"},     // MA# ( ⅽ → c ) SMALL ROMAN NUMERAL ONE HUNDRED → LATIN SMALL LETTER C#
    {8574, "d"},     // MA# ( ⅾ → d ) SMALL ROMAN NUMERAL FIVE HUNDRED → LATIN SMALL LETTER D#
    {8575, "rn"},    // MA# ( ⅿ → rn ) SMALL ROMAN NUMERAL ONE THOUSAND → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {8722, "-"},     // MA#* ( − → - ) MINUS SIGN → HYPHEN-MINUS#
    {8725, "/"},     // MA#* ( ∕ → / ) DIVISION SLASH → SOLIDUS#
    {8726, "\\"},    // MA#* ( ∖ → \ ) SET MINUS → REVERSE SOLIDUS#
    {8727, "*"},     // MA#* ( ∗ → * ) ASTERISK OPERATOR → ASTERISK#
    {8734, "oo"},    // MA#* ( ∞ → oo ) INFINITY → LATIN SMALL LETTER O, LATIN SMALL LETTER O# →ꝏ→
    {8739, "l"},     // MA#* ( ∣ → l ) DIVIDES → LATIN SMALL LETTER L# →ǀ→
    {8741, "ll"},    // MA#* ( ∥ → ll ) PARALLEL TO → LATIN SMALL LETTER L, LATIN SMALL LETTER L# →||→
    {8744, "v"},     // MA#* ( ∨ → v ) LOGICAL OR → LATIN SMALL LETTER V#
    {8746, "U"},     // MA#* ( ∪ → U ) UNION → LATIN CAPITAL LETTER U# →ᑌ→
    {8758, ":"},     // MA#* ( ∶ → : ) RATIO → COLON#
    {8764, "~"},     // MA#* ( ∼ → ~ ) TILDE OPERATOR → TILDE#
    {8810, "<<"},    // MA#* ( ≪ → << ) MUCH LESS-THAN → LESS-THAN SIGN, LESS-THAN SIGN#
    {8811, ">>"},    // MA#* ( ≫ → >> ) MUCH GREATER-THAN → GREATER-THAN SIGN, GREATER-THAN SIGN#
    {8868, "T"},     // MA#* ( ⊤ → T ) DOWN TACK → LATIN CAPITAL LETTER T#
    {8897, "v"},     // MA#* ( ⋁ → v ) N-ARY LOGICAL OR → LATIN SMALL LETTER V# →∨→
    {8899, "U"},     // MA#* ( ⋃ → U ) N-ARY UNION → LATIN CAPITAL LETTER U# →∪→→ᑌ→
    {8920, "<<<"},   // MA#* ( ⋘ → <<< ) VERY MUCH LESS-THAN → LESS-THAN SIGN, LESS-THAN SIGN, LESS-THAN SIGN#
    {8921, ">>>"},   // MA#* ( ⋙ → >>> ) VERY MUCH GREATER-THAN → GREATER-THAN SIGN, GREATER-THAN SIGN, GREATER-THAN SIGN#
    {8959, "E"},     // MA#* ( ⋿ → E ) Z NOTATION BAG MEMBERSHIP → LATIN CAPITAL LETTER E#
    {9075, "i"},     // MA#* ( ⍳ → i ) APL FUNCTIONAL SYMBOL IOTA → LATIN SMALL LETTER I# →ι→
    {9076, "p"},     // MA#* ( ⍴ → p ) APL FUNCTIONAL SYMBOL RHO → LATIN SMALL LETTER P# →ρ→
    {9082, "a"},     // MA#* ( ⍺ → a ) APL FUNCTIONAL SYMBOL ALPHA → LATIN SMALL LETTER A# →α→
    {9213, "l"},     // MA#* ( ⏽ → l ) POWER ON SYMBOL → LATIN SMALL LETTER L# →I→
    {9290, "\\\\"},  // MA#* ( ⑊ → \\ ) OCR DOUBLE BACKSLASH → REVERSE SOLIDUS, REVERSE SOLIDUS#
    {9332, "(l)"},   // MA#* ( ⑴ → (l) ) PARENTHESIZED DIGIT ONE → LEFT PARENTHESIS, LATIN SMALL LETTER L, RIGHT PARENTHESIS# →(1)→
    {9333, "(2)"},   // MA#* ( ⑵ → (2) ) PARENTHESIZED DIGIT TWO → LEFT PARENTHESIS, DIGIT TWO, RIGHT PARENTHESIS#
    {9334, "(3)"},   // MA#* ( ⑶ → (3) ) PARENTHESIZED DIGIT THREE → LEFT PARENTHESIS, DIGIT THREE, RIGHT PARENTHESIS#
    {9335, "(4)"},   // MA#* ( ⑷ → (4) ) PARENTHESIZED DIGIT FOUR → LEFT PARENTHESIS, DIGIT FOUR, RIGHT PARENTHESIS#
    {9336, "(5)"},   // MA#* ( ⑸ → (5) ) PARENTHESIZED DIGIT FIVE → LEFT PARENTHESIS, DIGIT FIVE, RIGHT PARENTHESIS#
    {9337, "(6)"},   // MA#* ( ⑹ → (6) ) PARENTHESIZED DIGIT SIX → LEFT PARENTHESIS, DIGIT SIX, RIGHT PARENTHESIS#
    {9338, "(7)"},   // MA#* ( ⑺ → (7) ) PARENTHESIZED DIGIT SEVEN → LEFT PARENTHESIS, DIGIT SEVEN, RIGHT PARENTHESIS#
    {9339, "(8)"},   // MA#* ( ⑻ → (8) ) PARENTHESIZED DIGIT EIGHT → LEFT PARENTHESIS, DIGIT EIGHT, RIGHT PARENTHESIS#
    {9340, "(9)"},   // MA#* ( ⑼ → (9) ) PARENTHESIZED DIGIT NINE → LEFT PARENTHESIS, DIGIT NINE, RIGHT PARENTHESIS#
    {9341, "(lO)"},  // MA#* ( ⑽ → (lO) ) PARENTHESIZED NUMBER TEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, LATIN CAPITAL LETTER O, RIGHT PARENTHESIS# →(10)→
    {9342, "(ll)"},  // MA#* ( ⑾ → (ll) ) PARENTHESIZED NUMBER ELEVEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, LATIN SMALL LETTER L, RIGHT PARENTHESIS# →(11)→
    {9343, "(l2)"},  // MA#* ( ⑿ → (l2) ) PARENTHESIZED NUMBER TWELVE → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT TWO, RIGHT PARENTHESIS# →(12)→
    {9344, "(l3)"},  // MA#* ( ⒀ → (l3) ) PARENTHESIZED NUMBER THIRTEEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT THREE, RIGHT PARENTHESIS# →(13)→
    {9345, "(l4)"},  // MA#* ( ⒁ → (l4) ) PARENTHESIZED NUMBER FOURTEEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT FOUR, RIGHT PARENTHESIS# →(14)→
    {9346, "(l5)"},  // MA#* ( ⒂ → (l5) ) PARENTHESIZED NUMBER FIFTEEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT FIVE, RIGHT PARENTHESIS# →(15)→
    {9347, "(l6)"},  // MA#* ( ⒃ → (l6) ) PARENTHESIZED NUMBER SIXTEEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT SIX, RIGHT PARENTHESIS# →(16)→
    {9348, "(l7)"},  // MA#* ( ⒄ → (l7) ) PARENTHESIZED NUMBER SEVENTEEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT SEVEN, RIGHT PARENTHESIS# →(17)→
    {9349, "(l8)"},  // MA#* ( ⒅ → (l8) ) PARENTHESIZED NUMBER EIGHTEEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT EIGHT, RIGHT PARENTHESIS# →(18)→
    {9350, "(l9)"},  // MA#* ( ⒆ → (l9) ) PARENTHESIZED NUMBER NINETEEN → LEFT PARENTHESIS, LATIN SMALL LETTER L, DIGIT NINE, RIGHT PARENTHESIS# →(19)→
    {9351, "(2O)"},  // MA#* ( ⒇ → (2O) ) PARENTHESIZED NUMBER TWENTY → LEFT PARENTHESIS, DIGIT TWO, LATIN CAPITAL LETTER O, RIGHT PARENTHESIS# →(20)→
    {9352, "l."},    // MA#* ( ⒈ → l. ) DIGIT ONE FULL STOP → LATIN SMALL LETTER L, FULL STOP# →1.→
    {9353, "2."},    // MA#* ( ⒉ → 2. ) DIGIT TWO FULL STOP → DIGIT TWO, FULL STOP#
    {9354, "3."},    // MA#* ( ⒊ → 3. ) DIGIT THREE FULL STOP → DIGIT THREE, FULL STOP#
    {9355, "4."},    // MA#* ( ⒋ → 4. ) DIGIT FOUR FULL STOP → DIGIT FOUR, FULL STOP#
    {9356, "5."},    // MA#* ( ⒌ → 5. ) DIGIT FIVE FULL STOP → DIGIT FIVE, FULL STOP#
    {9357, "6."},    // MA#* ( ⒍ → 6. ) DIGIT SIX FULL STOP → DIGIT SIX, FULL STOP#
    {9358, "7."},    // MA#* ( ⒎ → 7. ) DIGIT SEVEN FULL STOP → DIGIT SEVEN, FULL STOP#
    {9359, "8."},    // MA#* ( ⒏ → 8. ) DIGIT EIGHT FULL STOP → DIGIT EIGHT, FULL STOP#
    {9360, "9."},    // MA#* ( ⒐ → 9. ) DIGIT NINE FULL STOP → DIGIT NINE, FULL STOP#
    {9361, "lO."},   // MA#* ( ⒑ → lO. ) NUMBER TEN FULL STOP → LATIN SMALL LETTER L, LATIN CAPITAL LETTER O, FULL STOP# →10.→
    {9362, "ll."},   // MA#* ( ⒒ → ll. ) NUMBER ELEVEN FULL STOP → LATIN SMALL LETTER L, LATIN SMALL LETTER L, FULL STOP# →11.→
    {9363, "l2."},   // MA#* ( ⒓ → l2. ) NUMBER TWELVE FULL STOP → LATIN SMALL LETTER L, DIGIT TWO, FULL STOP# →12.→
    {9364, "l3."},   // MA#* ( ⒔ → l3. ) NUMBER THIRTEEN FULL STOP → LATIN SMALL LETTER L, DIGIT THREE, FULL STOP# →13.→
    {9365, "l4."},   // MA#* ( ⒕ → l4. ) NUMBER FOURTEEN FULL STOP → LATIN SMALL LETTER L, DIGIT FOUR, FULL STOP# →14.→
    {9366, "l5."},   // MA#* ( ⒖ → l5. ) NUMBER FIFTEEN FULL STOP → LATIN SMALL LETTER L, DIGIT FIVE, FULL STOP# →15.→
    {9367, "l6."},   // MA#* ( ⒗ → l6. ) NUMBER SIXTEEN FULL STOP → LATIN SMALL LETTER L, DIGIT SIX, FULL STOP# →16.→
    {9368, "l7."},   // MA#* ( ⒘ → l7. ) NUMBER SEVENTEEN FULL STOP → LATIN SMALL LETTER L, DIGIT SEVEN, FULL STOP# →17.→
    {9369, "l8."},   // MA#* ( ⒙ → l8. ) NUMBER EIGHTEEN FULL STOP → LATIN SMALL LETTER L, DIGIT EIGHT, FULL STOP# →18.→
    {9370, "l9."},   // MA#* ( ⒚ → l9. ) NUMBER NINETEEN FULL STOP → LATIN SMALL LETTER L, DIGIT NINE, FULL STOP# →19.→
    {9371, "2O."},   // MA#* ( ⒛ → 2O. ) NUMBER TWENTY FULL STOP → DIGIT TWO, LATIN CAPITAL LETTER O, FULL STOP# →20.→
    {9372, "(a)"},   // MA#* ( ⒜ → (a) ) PARENTHESIZED LATIN SMALL LETTER A → LEFT PARENTHESIS, LATIN SMALL LETTER A, RIGHT PARENTHESIS#
    {9373, "(b)"},   // MA#* ( ⒝ → (b) ) PARENTHESIZED LATIN SMALL LETTER B → LEFT PARENTHESIS, LATIN SMALL LETTER B, RIGHT PARENTHESIS#
    {9374, "(c)"},   // MA#* ( ⒞ → (c) ) PARENTHESIZED LATIN SMALL LETTER C → LEFT PARENTHESIS, LATIN SMALL LETTER C, RIGHT PARENTHESIS#
    {9375, "(d)"},   // MA#* ( ⒟ → (d) ) PARENTHESIZED LATIN SMALL LETTER D → LEFT PARENTHESIS, LATIN SMALL LETTER D, RIGHT PARENTHESIS#
    {9376, "(e)"},   // MA#* ( ⒠ → (e) ) PARENTHESIZED LATIN SMALL LETTER E → LEFT PARENTHESIS, LATIN SMALL LETTER E, RIGHT PARENTHESIS#
    {9377, "(f)"},   // MA#* ( ⒡ → (f) ) PARENTHESIZED LATIN SMALL LETTER F → LEFT PARENTHESIS, LATIN SMALL LETTER F, RIGHT PARENTHESIS#
    {9378, "(g)"},   // MA#* ( ⒢ → (g) ) PARENTHESIZED LATIN SMALL LETTER G → LEFT PARENTHESIS, LATIN SMALL LETTER G, RIGHT PARENTHESIS#
    {9379, "(h)"},   // MA#* ( ⒣ → (h) ) PARENTHESIZED LATIN SMALL LETTER H → LEFT PARENTHESIS, LATIN SMALL LETTER H, RIGHT PARENTHESIS#
    {9380, "(i)"},   // MA#* ( ⒤ → (i) ) PARENTHESIZED LATIN SMALL LETTER I → LEFT PARENTHESIS, LATIN SMALL LETTER I, RIGHT PARENTHESIS#
    {9381, "(j)"},   // MA#* ( ⒥ → (j) ) PARENTHESIZED LATIN SMALL LETTER J → LEFT PARENTHESIS, LATIN SMALL LETTER J, RIGHT PARENTHESIS#
    {9382, "(k)"},   // MA#* ( ⒦ → (k) ) PARENTHESIZED LATIN SMALL LETTER K → LEFT PARENTHESIS, LATIN SMALL LETTER K, RIGHT PARENTHESIS#
    {9383, "(l)"},   // MA#* ( ⒧ → (l) ) PARENTHESIZED LATIN SMALL LETTER L → LEFT PARENTHESIS, LATIN SMALL LETTER L, RIGHT PARENTHESIS#
    {9384, "(rn)"},  // MA#* ( ⒨ → (rn) ) PARENTHESIZED LATIN SMALL LETTER M → LEFT PARENTHESIS, LATIN SMALL LETTER R, LATIN SMALL LETTER N, RIGHT PARENTHESIS# →(m)→
    {9385, "(n)"},   // MA#* ( ⒩ → (n) ) PARENTHESIZED LATIN SMALL LETTER N → LEFT PARENTHESIS, LATIN SMALL LETTER N, RIGHT PARENTHESIS#
    {9386, "(o)"},   // MA#* ( ⒪ → (o) ) PARENTHESIZED LATIN SMALL LETTER O → LEFT PARENTHESIS, LATIN SMALL LETTER O, RIGHT PARENTHESIS#
    {9387, "(p)"},   // MA#* ( ⒫ → (p) ) PARENTHESIZED LATIN SMALL LETTER P → LEFT PARENTHESIS, LATIN SMALL LETTER P, RIGHT PARENTHESIS#
    {9388, "(q)"},   // MA#* ( ⒬ → (q) ) PARENTHESIZED LATIN SMALL LETTER Q → LEFT PARENTHESIS, LATIN SMALL LETTER Q, RIGHT PARENTHESIS#
    {9389, "(r)"},   // MA#* ( ⒭ → (r) ) PARENTHESIZED LATIN SMALL LETTER R → LEFT PARENTHESIS, LATIN SMALL LETTER R, RIGHT PARENTHESIS#
    {9390, "(s)"},   // MA#* ( ⒮ → (s) ) PARENTHESIZED LATIN SMALL LETTER S → LEFT PARENTHESIS, LATIN SMALL LETTER S, RIGHT PARENTHESIS#
    {9391, "(t)"},   // MA#* ( ⒯ → (t) ) PARENTHESIZED LATIN SMALL LETTER T → LEFT PARENTHESIS, LATIN SMALL LETTER T, RIGHT PARENTHESIS#
    {9392, "(u)"},   // MA#* ( ⒰ → (u) ) PARENTHESIZED LATIN SMALL LETTER U → LEFT PARENTHESIS, LATIN SMALL LETTER U, RIGHT PARENTHESIS#
    {9393, "(v)"},   // MA#* ( ⒱ → (v) ) PARENTHESIZED LATIN SMALL LETTER V → LEFT PARENTHESIS, LATIN SMALL LETTER V, RIGHT PARENTHESIS#
    {9394, "(w)"},   // MA#* ( ⒲ → (w) ) PARENTHESIZED LATIN SMALL LETTER W → LEFT PARENTHESIS, LATIN SMALL LETTER W, RIGHT PARENTHESIS#
    {9395, "(x)"},   // MA#* ( ⒳ → (x) ) PARENTHESIZED LATIN SMALL LETTER X → LEFT PARENTHESIS, LATIN SMALL LETTER X, RIGHT PARENTHESIS#
    {9396, "(y)"},   // MA#* ( ⒴ → (y) ) PARENTHESIZED LATIN SMALL LETTER Y → LEFT PARENTHESIS, LATIN SMALL LETTER Y, RIGHT PARENTHESIS#
    {9397, "(z)"},   // MA#* ( ⒵ → (z) ) PARENTHESIZED LATIN SMALL LETTER Z → LEFT PARENTHESIS, LATIN SMALL LETTER Z, RIGHT PARENTHESIS#
    {9585, "/"},     // MA#* ( ╱ → / ) BOX DRAWINGS LIGHT DIAGONAL UPPER RIGHT TO LOWER LEFT → SOLIDUS#
    {9587, "X"},     // MA#* ( ╳ → X ) BOX DRAWINGS LIGHT DIAGONAL CROSS → LATIN CAPITAL LETTER X#
    {10088, "("},    // MA#* ( ❨ → ( ) MEDIUM LEFT PARENTHESIS ORNAMENT → LEFT PARENTHESIS#
    {10089, ")"},    // MA#* ( ❩ → ) ) MEDIUM RIGHT PARENTHESIS ORNAMENT → RIGHT PARENTHESIS#
    {10094, "<"},    // MA#* ( ❮ → < ) HEAVY LEFT-POINTING ANGLE QUOTATION MARK ORNAMENT → LESS-THAN SIGN# →‹→
    {10095, ">"},    // MA#* ( ❯ → > ) HEAVY RIGHT-POINTING ANGLE QUOTATION MARK ORNAMENT → GREATER-THAN SIGN# →›→
    {10098, "("},    // MA#* ( ❲ → ( ) LIGHT LEFT TORTOISE SHELL BRACKET ORNAMENT → LEFT PARENTHESIS# →〔→
    {10099, ")"},    // MA#* ( ❳ → ) ) LIGHT RIGHT TORTOISE SHELL BRACKET ORNAMENT → RIGHT PARENTHESIS# →〕→
    {10100, "{"},    // MA#* ( ❴ → { ) MEDIUM LEFT CURLY BRACKET ORNAMENT → LEFT CURLY BRACKET#
    {10101, "}"},    // MA#* ( ❵ → } ) MEDIUM RIGHT CURLY BRACKET ORNAMENT → RIGHT CURLY BRACKET#
    {10133, "+"},    // MA#* ( ➕ → + ) HEAVY PLUS SIGN → PLUS SIGN#
    {10134, "-"},    // MA#* ( ➖ → - ) HEAVY MINUS SIGN → HYPHEN-MINUS# →−→
    {10187, "/"},    // MA#* ( ⟋ → / ) MATHEMATICAL RISING DIAGONAL → SOLIDUS#
    {10189, "\\"},   // MA#* ( ⟍ → \ ) MATHEMATICAL FALLING DIAGONAL → REVERSE SOLIDUS#
    {10201, "T"},    // MA#* ( ⟙ → T ) LARGE DOWN TACK → LATIN CAPITAL LETTER T#
    {10539, "x"},    // MA#* ( ⤫ → x ) RISING DIAGONAL CROSSING FALLING DIAGONAL → LATIN SMALL LETTER X#
    {10540, "x"},    // MA#* ( ⤬ → x ) FALLING DIAGONAL CROSSING RISING DIAGONAL → LATIN SMALL LETTER X#
    {10741, "\\"},   // MA#* ( ⧵ → \ ) REVERSE SOLIDUS OPERATOR → REVERSE SOLIDUS#
    {10744, "/"},    // MA#* ( ⧸ → / ) BIG SOLIDUS → SOLIDUS#
    {10745, "\\"},   // MA#* ( ⧹ → \ ) BIG REVERSE SOLIDUS → REVERSE SOLIDUS#
    {10784, ">>"},   // MA#* ( ⨠ → >> ) Z NOTATION SCHEMA PIPING → GREATER-THAN SIGN, GREATER-THAN SIGN# →≫→
    {10799, "x"},    // MA#* ( ⨯ → x ) VECTOR OR CROSS PRODUCT → LATIN SMALL LETTER X# →×→
    {10868, "::="},  // MA#* ( ⩴ → ::= ) DOUBLE COLON EQUAL → COLON, COLON, EQUALS SIGN#
    {10869, "=="},   // MA#* ( ⩵ → == ) TWO CONSECUTIVE EQUALS SIGNS → EQUALS SIGN, EQUALS SIGN#
    {10870, "==="},  // MA#* ( ⩶ → === ) THREE CONSECUTIVE EQUALS SIGNS → EQUALS SIGN, EQUALS SIGN, EQUALS SIGN#
    {10917, "><"},   // MA#* ( ⪥ → >< ) GREATER-THAN BESIDE LESS-THAN → GREATER-THAN SIGN, LESS-THAN SIGN#
    {11003, "///"},  // MA#* ( ⫻ → /// ) TRIPLE SOLIDUS BINARY RELATION → SOLIDUS, SOLIDUS, SOLIDUS#
    {11005, "//"},   // MA#* ( ⫽ → // ) DOUBLE SOLIDUS OPERATOR → SOLIDUS, SOLIDUS#
    {11397, "r"},    // MA# ( ⲅ → r ) COPTIC SMALL LETTER GAMMA → LATIN SMALL LETTER R# →г→
    {11406, "H"},    // MA# ( Ⲏ → H ) COPTIC CAPITAL LETTER HATE → LATIN CAPITAL LETTER H# →Η→
    {11410, "l"},    // MA# ( Ⲓ → l ) COPTIC CAPITAL LETTER IAUDA → LATIN SMALL LETTER L# →Ӏ→
    {11412, "K"},    // MA# ( Ⲕ → K ) COPTIC CAPITAL LETTER KAPA → LATIN CAPITAL LETTER K# →Κ→
    {11416, "M"},    // MA# ( Ⲙ → M ) COPTIC CAPITAL LETTER MI → LATIN CAPITAL LETTER M#
    {11418, "N"},    // MA# ( Ⲛ → N ) COPTIC CAPITAL LETTER NI → LATIN CAPITAL LETTER N#
    {11422, "O"},    // MA# ( Ⲟ → O ) COPTIC CAPITAL LETTER O → LATIN CAPITAL LETTER O#
    {11423, "o"},    // MA# ( ⲟ → o ) COPTIC SMALL LETTER O → LATIN SMALL LETTER O#
    {11426, "P"},    // MA# ( Ⲣ → P ) COPTIC CAPITAL LETTER RO → LATIN CAPITAL LETTER P#
    {11427, "p"},    // MA# ( ⲣ → p ) COPTIC SMALL LETTER RO → LATIN SMALL LETTER P# →ρ→
    {11428, "C"},    // MA# ( Ⲥ → C ) COPTIC CAPITAL LETTER SIMA → LATIN CAPITAL LETTER C# →Ϲ→
    {11429, "c"},    // MA# ( ⲥ → c ) COPTIC SMALL LETTER SIMA → LATIN SMALL LETTER C# →ϲ→
    {11430, "T"},    // MA# ( Ⲧ → T ) COPTIC CAPITAL LETTER TAU → LATIN CAPITAL LETTER T#
    {11432, "Y"},    // MA# ( Ⲩ → Y ) COPTIC CAPITAL LETTER UA → LATIN CAPITAL LETTER Y#
    {11436, "X"},    // MA# ( Ⲭ → X ) COPTIC CAPITAL LETTER KHI → LATIN CAPITAL LETTER X# →Х→
    {11450, "-"},    // MA# ( Ⲻ → - ) COPTIC CAPITAL LETTER DIALECT-P NI → HYPHEN-MINUS# →‒→
    {11462, "/"},    // MA# ( Ⳇ → / ) COPTIC CAPITAL LETTER OLD COPTIC ESH → SOLIDUS#
    {11466, "9"},    // MA# ( Ⳋ → 9 ) COPTIC CAPITAL LETTER DIALECT-P HORI → DIGIT NINE#
    {11468, "3"},    // MA# ( Ⳍ → 3 ) COPTIC CAPITAL LETTER OLD COPTIC HORI → DIGIT THREE# →Ȝ→→Ʒ→
    {11472, "L"},    // MA# ( Ⳑ → L ) COPTIC CAPITAL LETTER L-SHAPED HA → LATIN CAPITAL LETTER L#
    {11474, "6"},    // MA# ( Ⳓ → 6 ) COPTIC CAPITAL LETTER OLD COPTIC HEI → DIGIT SIX#
    {11513, "\\\\"}, // MA#* ( ⳹ → \\ ) COPTIC OLD NUBIAN FULL STOP → REVERSE SOLIDUS, REVERSE SOLIDUS#
    {11576, "V"},    // MA# ( ⴸ → V ) TIFINAGH LETTER YADH → LATIN CAPITAL LETTER V#
    {11577, "E"},    // MA# ( ⴹ → E ) TIFINAGH LETTER YADD → LATIN CAPITAL LETTER E#
    {11599, "l"},    // MA# ( ⵏ → l ) TIFINAGH LETTER YAN → LATIN SMALL LETTER L# →Ӏ→
    {11601, "!"},    // MA# ( ⵑ → ! ) TIFINAGH LETTER TUAREG YANG → EXCLAMATION MARK#
    {11604, "O"},    // MA# ( ⵔ → O ) TIFINAGH LETTER YAR → LATIN CAPITAL LETTER O#
    {11605, "Q"},    // MA# ( ⵕ → Q ) TIFINAGH LETTER YARR → LATIN CAPITAL LETTER Q#
    {11613, "X"},    // MA# ( ⵝ → X ) TIFINAGH LETTER YATH → LATIN CAPITAL LETTER X#
    {11816, "(("},   // MA#* ( ⸨ → (( ) LEFT DOUBLE PARENTHESIS → LEFT PARENTHESIS, LEFT PARENTHESIS#
    {11817, "))"},   // MA#* ( ⸩ → )) ) RIGHT DOUBLE PARENTHESIS → RIGHT PARENTHESIS, RIGHT PARENTHESIS#
    {11840, "="},    // MA#* ( ⹀ → = ) DOUBLE HYPHEN → EQUALS SIGN#
    {12034, "\\"},   // MA#* ( ⼂ → \ ) KANGXI RADICAL DOT → REVERSE SOLIDUS#
    {12035, "/"},    // MA#* ( ⼃ → / ) KANGXI RADICAL SLASH → SOLIDUS#
    {12291, "\""},   // MA#* ( 〃 → '' ) DITTO MARK → APOSTROPHE, APOSTROPHE# →″→→"→# Converted to a quote.
    {12295, "O"},    // MA# ( 〇 → O ) IDEOGRAPHIC NUMBER ZERO → LATIN CAPITAL LETTER O#
    {12308, "("},    // MA#* ( 〔 → ( ) LEFT TORTOISE SHELL BRACKET → LEFT PARENTHESIS#
    {12309, ")"},    // MA#* ( 〕 → ) ) RIGHT TORTOISE SHELL BRACKET → RIGHT PARENTHESIS#
    {12339, "/"},    // MA# ( 〳 → / ) VERTICAL KANA REPEAT MARK UPPER HALF → SOLIDUS#
    {12448, "="},    // MA#* ( ゠ → = ) KATAKANA-HIRAGANA DOUBLE HYPHEN → EQUALS SIGN#
    {12494, "/"},    // MA# ( ノ → / ) KATAKANA LETTER NO → SOLIDUS# →⼃→
    {12755, "/"},    // MA#* ( ㇓ → / ) CJK STROKE SP → SOLIDUS# →⼃→
    {12756, "\\"},   // MA#* ( ㇔ → \ ) CJK STROKE D → REVERSE SOLIDUS# →⼂→
    {20022, "\\"},   // MA# ( 丶 → \ ) CJK UNIFIED IDEOGRAPH-4E36 → REVERSE SOLIDUS# →⼂→
    {20031, "/"},    // MA# ( 丿 → / ) CJK UNIFIED IDEOGRAPH-4E3F → SOLIDUS# →⼃→
    {42192, "B"},    // MA# ( ꓐ → B ) LISU LETTER BA → LATIN CAPITAL LETTER B#
    {42193, "P"},    // MA# ( ꓑ → P ) LISU LETTER PA → LATIN CAPITAL LETTER P#
    {42194, "d"},    // MA# ( ꓒ → d ) LISU LETTER PHA → LATIN SMALL LETTER D#
    {42195, "D"},    // MA# ( ꓓ → D ) LISU LETTER DA → LATIN CAPITAL LETTER D#
    {42196, "T"},    // MA# ( ꓔ → T ) LISU LETTER TA → LATIN CAPITAL LETTER T#
    {42198, "G"},    // MA# ( ꓖ → G ) LISU LETTER GA → LATIN CAPITAL LETTER G#
    {42199, "K"},    // MA# ( ꓗ → K ) LISU LETTER KA → LATIN CAPITAL LETTER K#
    {42201, "J"},    // MA# ( ꓙ → J ) LISU LETTER JA → LATIN CAPITAL LETTER J#
    {42202, "C"},    // MA# ( ꓚ → C ) LISU LETTER CA → LATIN CAPITAL LETTER C#
    {42204, "Z"},    // MA# ( ꓜ → Z ) LISU LETTER DZA → LATIN CAPITAL LETTER Z#
    {42205, "F"},    // MA# ( ꓝ → F ) LISU LETTER TSA → LATIN CAPITAL LETTER F#
    {42207, "M"},    // MA# ( ꓟ → M ) LISU LETTER MA → LATIN CAPITAL LETTER M#
    {42208, "N"},    // MA# ( ꓠ → N ) LISU LETTER NA → LATIN CAPITAL LETTER N#
    {42209, "L"},    // MA# ( ꓡ → L ) LISU LETTER LA → LATIN CAPITAL LETTER L#
    {42210, "S"},    // MA# ( ꓢ → S ) LISU LETTER SA → LATIN CAPITAL LETTER S#
    {42211, "R"},    // MA# ( ꓣ → R ) LISU LETTER ZHA → LATIN CAPITAL LETTER R#
    {42214, "V"},    // MA# ( ꓦ → V ) LISU LETTER HA → LATIN CAPITAL LETTER V#
    {42215, "H"},    // MA# ( ꓧ → H ) LISU LETTER XA → LATIN CAPITAL LETTER H#
    {42218, "W"},    // MA# ( ꓪ → W ) LISU LETTER WA → LATIN CAPITAL LETTER W#
    {42219, "X"},    // MA# ( ꓫ → X ) LISU LETTER SHA → LATIN CAPITAL LETTER X#
    {42220, "Y"},    // MA# ( ꓬ → Y ) LISU LETTER YA → LATIN CAPITAL LETTER Y#
    {42222, "A"},    // MA# ( ꓮ → A ) LISU LETTER A → LATIN CAPITAL LETTER A#
    {42224, "E"},    // MA# ( ꓰ → E ) LISU LETTER E → LATIN CAPITAL LETTER E#
    {42226, "l"},    // MA# ( ꓲ → l ) LISU LETTER I → LATIN SMALL LETTER L# →I→
    {42227, "O"},    // MA# ( ꓳ → O ) LISU LETTER O → LATIN CAPITAL LETTER O#
    {42228, "U"},    // MA# ( ꓴ → U ) LISU LETTER U → LATIN CAPITAL LETTER U#
    {42232, "."},    // MA# ( ꓸ → . ) LISU LETTER TONE MYA TI → FULL STOP#
    {42233, ","},    // MA# ( ꓹ → , ) LISU LETTER TONE NA PO → COMMA#
    {42234, ".."},   // MA# ( ꓺ → .. ) LISU LETTER TONE MYA CYA → FULL STOP, FULL STOP#
    {42235, ".,"},   // MA# ( ꓻ → ., ) LISU LETTER TONE MYA BO → FULL STOP, COMMA#
    {42237, ":"},    // MA# ( ꓽ → : ) LISU LETTER TONE MYA JEU → COLON#
    {42238, "-."},   // MA#* ( ꓾ → -. ) LISU PUNCTUATION COMMA → HYPHEN-MINUS, FULL STOP#
    {42239, "="},    // MA#* ( ꓿ → = ) LISU PUNCTUATION FULL STOP → EQUALS SIGN#
    {42510, "."},    // MA#* ( ꘎ → . ) VAI FULL STOP → FULL STOP#
    {42564, "2"},    // MA# ( Ꙅ → 2 ) CYRILLIC CAPITAL LETTER REVERSED DZE → DIGIT TWO# →Ƨ→
    {42567, "i"},    // MA# ( ꙇ → i ) CYRILLIC SMALL LETTER IOTA → LATIN SMALL LETTER I# →ι→
    {42648, "OO"},   // MA# ( Ꚙ → OO ) CYRILLIC CAPITAL LETTER DOUBLE O → LATIN CAPITAL LETTER O, LATIN CAPITAL LETTER O#
    {42649, "oo"},   // MA# ( ꚙ → oo ) CYRILLIC SMALL LETTER DOUBLE O → LATIN SMALL LETTER O, LATIN SMALL LETTER O#
    {42719, "V"},    // MA# ( ꛟ → V ) BAMUM LETTER KO → LATIN CAPITAL LETTER V#
    {42731, "?"},    // MA# ( ꛫ → ? ) BAMUM LETTER NTUU → QUESTION MARK# →ʔ→
    {42735, "2"},    // MA# ( ꛯ → 2 ) BAMUM LETTER KOGHOM → DIGIT TWO# →Ƨ→
    {42792, "T3"},   // MA# ( Ꜩ → T3 ) LATIN CAPITAL LETTER TZ → LATIN CAPITAL LETTER T, DIGIT THREE# →TƷ→
    {42801, "s"},    // MA# ( ꜱ → s ) LATIN LETTER SMALL CAPITAL S → LATIN SMALL LETTER S#
    {42802, "AA"},   // MA# ( Ꜳ → AA ) LATIN CAPITAL LETTER AA → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER A#
    {42803, "aa"},   // MA# ( ꜳ → aa ) LATIN SMALL LETTER AA → LATIN SMALL LETTER A, LATIN SMALL LETTER A#
    {42804, "AO"},   // MA# ( Ꜵ → AO ) LATIN CAPITAL LETTER AO → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER O#
    {42805, "ao"},   // MA# ( ꜵ → ao ) LATIN SMALL LETTER AO → LATIN SMALL LETTER A, LATIN SMALL LETTER O#
    {42806, "AU"},   // MA# ( Ꜷ → AU ) LATIN CAPITAL LETTER AU → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER U#
    {42807, "au"},   // MA# ( ꜷ → au ) LATIN SMALL LETTER AU → LATIN SMALL LETTER A, LATIN SMALL LETTER U#
    {42808, "AV"},   // MA# ( Ꜹ → AV ) LATIN CAPITAL LETTER AV → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER V#
    {42809, "av"},   // MA# ( ꜹ → av ) LATIN SMALL LETTER AV → LATIN SMALL LETTER A, LATIN SMALL LETTER V#
    {42810, "AV"},   // MA# ( Ꜻ → AV ) LATIN CAPITAL LETTER AV WITH HORIZONTAL BAR → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER V#
    {42811, "av"},   // MA# ( ꜻ → av ) LATIN SMALL LETTER AV WITH HORIZONTAL BAR → LATIN SMALL LETTER A, LATIN SMALL LETTER V#
    {42812, "AY"},   // MA# ( Ꜽ → AY ) LATIN CAPITAL LETTER AY → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER Y#
    {42813, "ay"},   // MA# ( ꜽ → ay ) LATIN SMALL LETTER AY → LATIN SMALL LETTER A, LATIN SMALL LETTER Y#
    {42830, "OO"},   // MA# ( Ꝏ → OO ) LATIN CAPITAL LETTER OO → LATIN CAPITAL LETTER O, LATIN CAPITAL LETTER O#
    {42831, "oo"},   // MA# ( ꝏ → oo ) LATIN SMALL LETTER OO → LATIN SMALL LETTER O, LATIN SMALL LETTER O#
    {42842, "2"},    // MA# ( Ꝛ → 2 ) LATIN CAPITAL LETTER R ROTUNDA → DIGIT TWO#
    {42858, "3"},    // MA# ( Ꝫ → 3 ) LATIN CAPITAL LETTER ET → DIGIT THREE#
    {42862, "9"},    // MA# ( Ꝯ → 9 ) LATIN CAPITAL LETTER CON → DIGIT NINE#
    {42871, "tf"},   // MA# ( ꝷ → tf ) LATIN SMALL LETTER TUM → LATIN SMALL LETTER T, LATIN SMALL LETTER F#
    {42872, "&"},    // MA# ( ꝸ → & ) LATIN SMALL LETTER UM → AMPERSAND#
    {42889, ":"},    // MA#* ( ꞉ → : ) MODIFIER LETTER COLON → COLON#
    {42892, "'"},    // MA# ( ꞌ → ' ) LATIN SMALL LETTER SALTILLO → APOSTROPHE#
    {42904, "F"},    // MA# ( Ꞙ → F ) LATIN CAPITAL LETTER F WITH STROKE → LATIN CAPITAL LETTER F#
    {42905, "f"},    // MA# ( ꞙ → f ) LATIN SMALL LETTER F WITH STROKE → LATIN SMALL LETTER F#
    {42911, "u"},    // MA# ( ꞟ → u ) LATIN SMALL LETTER VOLAPUK UE → LATIN SMALL LETTER U#
    {42923, "3"},    // MA# ( Ɜ → 3 ) LATIN CAPITAL LETTER REVERSED OPEN E → DIGIT THREE#
    {42930, "J"},    // MA# ( Ʝ → J ) LATIN CAPITAL LETTER J WITH CROSSED-TAIL → LATIN CAPITAL LETTER J#
    {42931, "X"},    // MA# ( Ꭓ → X ) LATIN CAPITAL LETTER CHI → LATIN CAPITAL LETTER X#
    {42932, "B"},    // MA# ( Ꞵ → B ) LATIN CAPITAL LETTER BETA → LATIN CAPITAL LETTER B#
    {43826, "e"},    // MA# ( ꬲ → e ) LATIN SMALL LETTER BLACKLETTER E → LATIN SMALL LETTER E#
    {43829, "f"},    // MA# ( ꬵ → f ) LATIN SMALL LETTER LENIS F → LATIN SMALL LETTER F#
    {43837, "o"},    // MA# ( ꬽ → o ) LATIN SMALL LETTER BLACKLETTER O → LATIN SMALL LETTER O#
    {43847, "r"},    // MA# ( ꭇ → r ) LATIN SMALL LETTER R WITHOUT HANDLE → LATIN SMALL LETTER R#
    {43848, "r"},    // MA# ( ꭈ → r ) LATIN SMALL LETTER DOUBLE R → LATIN SMALL LETTER R#
    {43854, "u"},    // MA# ( ꭎ → u ) LATIN SMALL LETTER U WITH SHORT RIGHT LEG → LATIN SMALL LETTER U#
    {43858, "u"},    // MA# ( ꭒ → u ) LATIN SMALL LETTER U WITH LEFT HOOK → LATIN SMALL LETTER U#
    {43866, "y"},    // MA# ( ꭚ → y ) LATIN SMALL LETTER Y WITH SHORT RIGHT LEG → LATIN SMALL LETTER Y#
    {43875, "uo"},   // MA# ( ꭣ → uo ) LATIN SMALL LETTER UO → LATIN SMALL LETTER U, LATIN SMALL LETTER O#
    {43893, "i"},    // MA# ( ꭵ → i ) CHEROKEE SMALL LETTER V → LATIN SMALL LETTER I#
    {43905, "r"},    // MA# ( ꮁ → r ) CHEROKEE SMALL LETTER HU → LATIN SMALL LETTER R# →ᴦ→→г→
    {43907, "w"},    // MA# ( ꮃ → w ) CHEROKEE SMALL LETTER LA → LATIN SMALL LETTER W# →ᴡ→
    {43923, "z"},    // MA# ( ꮓ → z ) CHEROKEE SMALL LETTER NO → LATIN SMALL LETTER Z# →ᴢ→
    {43945, "v"},    // MA# ( ꮩ → v ) CHEROKEE SMALL LETTER DO → LATIN SMALL LETTER V# →ᴠ→
    {43946, "s"},    // MA# ( ꮪ → s ) CHEROKEE SMALL LETTER DU → LATIN SMALL LETTER S# →ꜱ→
    {43951, "c"},    // MA# ( ꮯ → c ) CHEROKEE SMALL LETTER TLI → LATIN SMALL LETTER C# →ᴄ→
    {64256, "ff"},   // MA# ( ﬀ → ff ) LATIN SMALL LIGATURE FF → LATIN SMALL LETTER F, LATIN SMALL LETTER F#
    {64257, "fi"},   // MA# ( ﬁ → fi ) LATIN SMALL LIGATURE FI → LATIN SMALL LETTER F, LATIN SMALL LETTER I#
    {64258, "fl"},   // MA# ( ﬂ → fl ) LATIN SMALL LIGATURE FL → LATIN SMALL LETTER F, LATIN SMALL LETTER L#
    {64259, "ffi"},  // MA# ( ﬃ → ffi ) LATIN SMALL LIGATURE FFI → LATIN SMALL LETTER F, LATIN SMALL LETTER F, LATIN SMALL LETTER I#
    {64260, "ffl"},  // MA# ( ﬄ → ffl ) LATIN SMALL LIGATURE FFL → LATIN SMALL LETTER F, LATIN SMALL LETTER F, LATIN SMALL LETTER L#
    {64262, "st"},   // MA# ( ﬆ → st ) LATIN SMALL LIGATURE ST → LATIN SMALL LETTER S, LATIN SMALL LETTER T#
    {64422, "o"},    // MA# ( ‎ﮦ‎ → o ) ARABIC LETTER HEH GOAL ISOLATED FORM → LATIN SMALL LETTER O# →‎ه‎→
    {64423, "o"},    // MA# ( ‎ﮧ‎ → o ) ARABIC LETTER HEH GOAL FINAL FORM → LATIN SMALL LETTER O# →‎ہ‎→→‎ه‎→
    {64424, "o"},    // MA# ( ‎ﮨ‎ → o ) ARABIC LETTER HEH GOAL INITIAL FORM → LATIN SMALL LETTER O# →‎ہ‎→→‎ه‎→
    {64425, "o"},    // MA# ( ‎ﮩ‎ → o ) ARABIC LETTER HEH GOAL MEDIAL FORM → LATIN SMALL LETTER O# →‎ہ‎→→‎ه‎→
    {64426, "o"},    // MA# ( ‎ﮪ‎ → o ) ARABIC LETTER HEH DOACHASHMEE ISOLATED FORM → LATIN SMALL LETTER O# →‎ه‎→
    {64427, "o"},    // MA# ( ‎ﮫ‎ → o ) ARABIC LETTER HEH DOACHASHMEE FINAL FORM → LATIN SMALL LETTER O# →‎ﻪ‎→→‎ه‎→
    {64428, "o"},    // MA# ( ‎ﮬ‎ → o ) ARABIC LETTER HEH DOACHASHMEE INITIAL FORM → LATIN SMALL LETTER O# →‎ﻫ‎→→‎ه‎→
    {64429, "o"},    // MA# ( ‎ﮭ‎ → o ) ARABIC LETTER HEH DOACHASHMEE MEDIAL FORM → LATIN SMALL LETTER O# →‎ﻬ‎→→‎ه‎→
    {64830, "("},    // MA#* ( ﴾ → ( ) ORNATE LEFT PARENTHESIS → LEFT PARENTHESIS#
    {64831, ")"},    // MA#* ( ﴿ → ) ) ORNATE RIGHT PARENTHESIS → RIGHT PARENTHESIS#
    {65072, ":"},    // MA#* ( ︰ → : ) PRESENTATION FORM FOR VERTICAL TWO DOT LEADER → COLON#
    {65101, "_"},    // MA# ( ﹍ → _ ) DASHED LOW LINE → LOW LINE#
    {65102, "_"},    // MA# ( ﹎ → _ ) CENTRELINE LOW LINE → LOW LINE#
    {65103, "_"},    // MA# ( ﹏ → _ ) WAVY LOW LINE → LOW LINE#
    {65112, "-"},    // MA#* ( ﹘ → - ) SMALL EM DASH → HYPHEN-MINUS#
    {65128, "\\"},   // MA#* ( ﹨ → \ ) SMALL REVERSE SOLIDUS → REVERSE SOLIDUS# →∖→
    {65165, "l"},    // MA# ( ‎ﺍ‎ → l ) ARABIC LETTER ALEF ISOLATED FORM → LATIN SMALL LETTER L# →‎ا‎→→1→
    {65166, "l"},    // MA# ( ‎ﺎ‎ → l ) ARABIC LETTER ALEF FINAL FORM → LATIN SMALL LETTER L# →‎ا‎→→1→
    {65257, "o"},    // MA# ( ‎ﻩ‎ → o ) ARABIC LETTER HEH ISOLATED FORM → LATIN SMALL LETTER O# →‎ه‎→
    {65258, "o"},    // MA# ( ‎ﻪ‎ → o ) ARABIC LETTER HEH FINAL FORM → LATIN SMALL LETTER O# →‎ه‎→
    {65259, "o"},    // MA# ( ‎ﻫ‎ → o ) ARABIC LETTER HEH INITIAL FORM → LATIN SMALL LETTER O# →‎ه‎→
    {65260, "o"},    // MA# ( ‎ﻬ‎ → o ) ARABIC LETTER HEH MEDIAL FORM → LATIN SMALL LETTER O# →‎ه‎→
    {65281, "!"},    // MA#* ( ！ → ! ) FULLWIDTH EXCLAMATION MARK → EXCLAMATION MARK# →ǃ→
    {65282, "\""},   // MA#* ( ＂ → '' ) FULLWIDTH QUOTATION MARK → APOSTROPHE, APOSTROPHE# →”→→"→# Converted to a quote.
    {65287, "'"},    // MA#* ( ＇ → ' ) FULLWIDTH APOSTROPHE → APOSTROPHE# →’→
    {65306, ":"},    // MA#* ( ： → : ) FULLWIDTH COLON → COLON# →︰→
    {65313, "A"},    // MA# ( Ａ → A ) FULLWIDTH LATIN CAPITAL LETTER A → LATIN CAPITAL LETTER A# →А→
    {65314, "B"},    // MA# ( Ｂ → B ) FULLWIDTH LATIN CAPITAL LETTER B → LATIN CAPITAL LETTER B# →Β→
    {65315, "C"},    // MA# ( Ｃ → C ) FULLWIDTH LATIN CAPITAL LETTER C → LATIN CAPITAL LETTER C# →С→
    {65317, "E"},    // MA# ( Ｅ → E ) FULLWIDTH LATIN CAPITAL LETTER E → LATIN CAPITAL LETTER E# →Ε→
    {65320, "H"},    // MA# ( Ｈ → H ) FULLWIDTH LATIN CAPITAL LETTER H → LATIN CAPITAL LETTER H# →Η→
    {65321, "l"},    // MA# ( Ｉ → l ) FULLWIDTH LATIN CAPITAL LETTER I → LATIN SMALL LETTER L# →Ӏ→
    {65322, "J"},    // MA# ( Ｊ → J ) FULLWIDTH LATIN CAPITAL LETTER J → LATIN CAPITAL LETTER J# →Ј→
    {65323, "K"},    // MA# ( Ｋ → K ) FULLWIDTH LATIN CAPITAL LETTER K → LATIN CAPITAL LETTER K# →Κ→
    {65325, "M"},    // MA# ( Ｍ → M ) FULLWIDTH LATIN CAPITAL LETTER M → LATIN CAPITAL LETTER M# →Μ→
    {65326, "N"},    // MA# ( Ｎ → N ) FULLWIDTH LATIN CAPITAL LETTER N → LATIN CAPITAL LETTER N# →Ν→
    {65327, "O"},    // MA# ( Ｏ → O ) FULLWIDTH LATIN CAPITAL LETTER O → LATIN CAPITAL LETTER O# →О→
    {65328, "P"},    // MA# ( Ｐ → P ) FULLWIDTH LATIN CAPITAL LETTER P → LATIN CAPITAL LETTER P# →Р→
    {65331, "S"},    // MA# ( Ｓ → S ) FULLWIDTH LATIN CAPITAL LETTER S → LATIN CAPITAL LETTER S# →Ѕ→
    {65332, "T"},    // MA# ( Ｔ → T ) FULLWIDTH LATIN CAPITAL LETTER T → LATIN CAPITAL LETTER T# →Т→
    {65336, "X"},    // MA# ( Ｘ → X ) FULLWIDTH LATIN CAPITAL LETTER X → LATIN CAPITAL LETTER X# →Х→
    {65337, "Y"},    // MA# ( Ｙ → Y ) FULLWIDTH LATIN CAPITAL LETTER Y → LATIN CAPITAL LETTER Y# →Υ→
    {65338, "Z"},    // MA# ( Ｚ → Z ) FULLWIDTH LATIN CAPITAL LETTER Z → LATIN CAPITAL LETTER Z# →Ζ→
    {65339, "("},    // MA#* ( ［ → ( ) FULLWIDTH LEFT SQUARE BRACKET → LEFT PARENTHESIS# →〔→
    {65340, "\\"},   // MA#* ( ＼ → \ ) FULLWIDTH REVERSE SOLIDUS → REVERSE SOLIDUS# →∖→
    {65341, ")"},    // MA#* ( ］ → ) ) FULLWIDTH RIGHT SQUARE BRACKET → RIGHT PARENTHESIS# →〕→
    {65344, "'"},    // MA#* ( ｀ → ' ) FULLWIDTH GRAVE ACCENT → APOSTROPHE# →‘→
    {65345, "a"},    // MA# ( ａ → a ) FULLWIDTH LATIN SMALL LETTER A → LATIN SMALL LETTER A# →а→
    {65347, "c"},    // MA# ( ｃ → c ) FULLWIDTH LATIN SMALL LETTER C → LATIN SMALL LETTER C# →с→
    {65349, "e"},    // MA# ( ｅ → e ) FULLWIDTH LATIN SMALL LETTER E → LATIN SMALL LETTER E# →е→
    {65351, "g"},    // MA# ( ｇ → g ) FULLWIDTH LATIN SMALL LETTER G → LATIN SMALL LETTER G# →ɡ→
    {65352, "h"},    // MA# ( ｈ → h ) FULLWIDTH LATIN SMALL LETTER H → LATIN SMALL LETTER H# →һ→
    {65353, "i"},    // MA# ( ｉ → i ) FULLWIDTH LATIN SMALL LETTER I → LATIN SMALL LETTER I# →і→
    {65354, "j"},    // MA# ( ｊ → j ) FULLWIDTH LATIN SMALL LETTER J → LATIN SMALL LETTER J# →ϳ→
    {65356, "l"},    // MA# ( ｌ → l ) FULLWIDTH LATIN SMALL LETTER L → LATIN SMALL LETTER L# →Ⅰ→→Ӏ→
    {65359, "o"},    // MA# ( ｏ → o ) FULLWIDTH LATIN SMALL LETTER O → LATIN SMALL LETTER O# →о→
    {65360, "p"},    // MA# ( ｐ → p ) FULLWIDTH LATIN SMALL LETTER P → LATIN SMALL LETTER P# →р→
    {65363, "s"},    // MA# ( ｓ → s ) FULLWIDTH LATIN SMALL LETTER S → LATIN SMALL LETTER S# →ѕ→
    {65366, "v"},    // MA# ( ｖ → v ) FULLWIDTH LATIN SMALL LETTER V → LATIN SMALL LETTER V# →ν→
    {65368, "x"},    // MA# ( ｘ → x ) FULLWIDTH LATIN SMALL LETTER X → LATIN SMALL LETTER X# →х→
    {65369, "y"},    // MA# ( ｙ → y ) FULLWIDTH LATIN SMALL LETTER Y → LATIN SMALL LETTER Y# →у→
    {65512, "l"},    // MA#* ( ￨ → l ) HALFWIDTH FORMS LIGHT VERTICAL → LATIN SMALL LETTER L# →|→
    {66178, "B"},    // MA# ( 𐊂 → B ) LYCIAN LETTER B → LATIN CAPITAL LETTER B#
    {66182, "E"},    // MA# ( 𐊆 → E ) LYCIAN LETTER I → LATIN CAPITAL LETTER E#
    {66183, "F"},    // MA# ( 𐊇 → F ) LYCIAN LETTER W → LATIN CAPITAL LETTER F#
    {66186, "l"},    // MA# ( 𐊊 → l ) LYCIAN LETTER J → LATIN SMALL LETTER L# →I→
    {66192, "X"},    // MA# ( 𐊐 → X ) LYCIAN LETTER MM → LATIN CAPITAL LETTER X#
    {66194, "O"},    // MA# ( 𐊒 → O ) LYCIAN LETTER U → LATIN CAPITAL LETTER O#
    {66197, "P"},    // MA# ( 𐊕 → P ) LYCIAN LETTER R → LATIN CAPITAL LETTER P#
    {66198, "S"},    // MA# ( 𐊖 → S ) LYCIAN LETTER S → LATIN CAPITAL LETTER S#
    {66199, "T"},    // MA# ( 𐊗 → T ) LYCIAN LETTER T → LATIN CAPITAL LETTER T#
    {66203, "+"},    // MA# ( 𐊛 → + ) LYCIAN LETTER H → PLUS SIGN#
    {66208, "A"},    // MA# ( 𐊠 → A ) CARIAN LETTER A → LATIN CAPITAL LETTER A#
    {66209, "B"},    // MA# ( 𐊡 → B ) CARIAN LETTER P2 → LATIN CAPITAL LETTER B#
    {66210, "C"},    // MA# ( 𐊢 → C ) CARIAN LETTER D → LATIN CAPITAL LETTER C#
    {66213, "F"},    // MA# ( 𐊥 → F ) CARIAN LETTER R → LATIN CAPITAL LETTER F#
    {66219, "O"},    // MA# ( 𐊫 → O ) CARIAN LETTER O → LATIN CAPITAL LETTER O#
    {66224, "M"},    // MA# ( 𐊰 → M ) CARIAN LETTER S → LATIN CAPITAL LETTER M#
    {66225, "T"},    // MA# ( 𐊱 → T ) CARIAN LETTER C-18 → LATIN CAPITAL LETTER T#
    {66226, "Y"},    // MA# ( 𐊲 → Y ) CARIAN LETTER U → LATIN CAPITAL LETTER Y#
    {66228, "X"},    // MA# ( 𐊴 → X ) CARIAN LETTER X → LATIN CAPITAL LETTER X#
    {66255, "H"},    // MA# ( 𐋏 → H ) CARIAN LETTER E2 → LATIN CAPITAL LETTER H#
    {66293, "Z"},    // MA#* ( 𐋵 → Z ) COPTIC EPACT NUMBER THREE HUNDRED → LATIN CAPITAL LETTER Z#
    {66305, "B"},    // MA# ( 𐌁 → B ) OLD ITALIC LETTER BE → LATIN CAPITAL LETTER B#
    {66306, "C"},    // MA# ( 𐌂 → C ) OLD ITALIC LETTER KE → LATIN CAPITAL LETTER C#
    {66313, "l"},    // MA# ( 𐌉 → l ) OLD ITALIC LETTER I → LATIN SMALL LETTER L# →I→
    {66321, "M"},    // MA# ( 𐌑 → M ) OLD ITALIC LETTER SHE → LATIN CAPITAL LETTER M#
    {66325, "T"},    // MA# ( 𐌕 → T ) OLD ITALIC LETTER TE → LATIN CAPITAL LETTER T#
    {66327, "X"},    // MA# ( 𐌗 → X ) OLD ITALIC LETTER EKS → LATIN CAPITAL LETTER X#
    {66330, "8"},    // MA# ( 𐌚 → 8 ) OLD ITALIC LETTER EF → DIGIT EIGHT#
    {66335, "*"},    // MA# ( 𐌟 → * ) OLD ITALIC LETTER ESS → ASTERISK#
    {66336, "l"},    // MA#* ( 𐌠 → l ) OLD ITALIC NUMERAL ONE → LATIN SMALL LETTER L# →𐌉→→I→
    {66338, "X"},    // MA#* ( 𐌢 → X ) OLD ITALIC NUMERAL TEN → LATIN CAPITAL LETTER X# →𐌗→
    {66564, "O"},    // MA# ( 𐐄 → O ) DESERET CAPITAL LETTER LONG O → LATIN CAPITAL LETTER O#
    {66581, "C"},    // MA# ( 𐐕 → C ) DESERET CAPITAL LETTER CHEE → LATIN CAPITAL LETTER C#
    {66587, "L"},    // MA# ( 𐐛 → L ) DESERET CAPITAL LETTER ETH → LATIN CAPITAL LETTER L#
    {66592, "S"},    // MA# ( 𐐠 → S ) DESERET CAPITAL LETTER ZHEE → LATIN CAPITAL LETTER S#
    {66604, "o"},    // MA# ( 𐐬 → o ) DESERET SMALL LETTER LONG O → LATIN SMALL LETTER O#
    {66621, "c"},    // MA# ( 𐐽 → c ) DESERET SMALL LETTER CHEE → LATIN SMALL LETTER C#
    {66632, "s"},    // MA# ( 𐑈 → s ) DESERET SMALL LETTER ZHEE → LATIN SMALL LETTER S#
    {66740, "R"},    // MA# ( 𐒴 → R ) OSAGE CAPITAL LETTER BRA → LATIN CAPITAL LETTER R# →Ʀ→
    {66754, "O"},    // MA# ( 𐓂 → O ) OSAGE CAPITAL LETTER O → LATIN CAPITAL LETTER O#
    {66766, "U"},    // MA# ( 𐓎 → U ) OSAGE CAPITAL LETTER U → LATIN CAPITAL LETTER U#
    {66770, "7"},    // MA# ( 𐓒 → 7 ) OSAGE CAPITAL LETTER ZA → DIGIT SEVEN#
    {66794, "o"},    // MA# ( 𐓪 → o ) OSAGE SMALL LETTER O → LATIN SMALL LETTER O#
    {66806, "u"},    // MA# ( 𐓶 → u ) OSAGE SMALL LETTER U → LATIN SMALL LETTER U# →ᴜ→
    {66835, "N"},    // MA# ( 𐔓 → N ) ELBASAN LETTER NE → LATIN CAPITAL LETTER N#
    {66838, "O"},    // MA# ( 𐔖 → O ) ELBASAN LETTER O → LATIN CAPITAL LETTER O#
    {66840, "K"},    // MA# ( 𐔘 → K ) ELBASAN LETTER QE → LATIN CAPITAL LETTER K#
    {66844, "C"},    // MA# ( 𐔜 → C ) ELBASAN LETTER SHE → LATIN CAPITAL LETTER C#
    {66845, "V"},    // MA# ( 𐔝 → V ) ELBASAN LETTER TE → LATIN CAPITAL LETTER V#
    {66853, "F"},    // MA# ( 𐔥 → F ) ELBASAN LETTER GHE → LATIN CAPITAL LETTER F#
    {66854, "L"},    // MA# ( 𐔦 → L ) ELBASAN LETTER GHAMMA → LATIN CAPITAL LETTER L#
    {66855, "X"},    // MA# ( 𐔧 → X ) ELBASAN LETTER KHE → LATIN CAPITAL LETTER X#
    {68176, "."},    // MA#* ( ‎𐩐‎ → . ) KHAROSHTHI PUNCTUATION DOT → FULL STOP#
    {70864, "O"},    // MA# ( 𑓐 → O ) TIRHUTA DIGIT ZERO → LATIN CAPITAL LETTER O# →০→→0→
    {71424, "rn"},   // MA# ( 𑜀 → rn ) AHOM LETTER KA → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {71430, "v"},    // MA# ( 𑜆 → v ) AHOM LETTER PA → LATIN SMALL LETTER V#
    {71434, "w"},    // MA# ( 𑜊 → w ) AHOM LETTER JA → LATIN SMALL LETTER W#
    {71438, "w"},    // MA# ( 𑜎 → w ) AHOM LETTER LA → LATIN SMALL LETTER W#
    {71439, "w"},    // MA# ( 𑜏 → w ) AHOM LETTER SA → LATIN SMALL LETTER W#
    {71840, "V"},    // MA# ( 𑢠 → V ) WARANG CITI CAPITAL LETTER NGAA → LATIN CAPITAL LETTER V#
    {71842, "F"},    // MA# ( 𑢢 → F ) WARANG CITI CAPITAL LETTER WI → LATIN CAPITAL LETTER F#
    {71843, "L"},    // MA# ( 𑢣 → L ) WARANG CITI CAPITAL LETTER YU → LATIN CAPITAL LETTER L#
    {71844, "Y"},    // MA# ( 𑢤 → Y ) WARANG CITI CAPITAL LETTER YA → LATIN CAPITAL LETTER Y#
    {71846, "E"},    // MA# ( 𑢦 → E ) WARANG CITI CAPITAL LETTER II → LATIN CAPITAL LETTER E#
    {71849, "Z"},    // MA# ( 𑢩 → Z ) WARANG CITI CAPITAL LETTER O → LATIN CAPITAL LETTER Z#
    {71852, "9"},    // MA# ( 𑢬 → 9 ) WARANG CITI CAPITAL LETTER KO → DIGIT NINE#
    {71854, "E"},    // MA# ( 𑢮 → E ) WARANG CITI CAPITAL LETTER YUJ → LATIN CAPITAL LETTER E#
    {71855, "4"},    // MA# ( 𑢯 → 4 ) WARANG CITI CAPITAL LETTER UC → DIGIT FOUR#
    {71858, "L"},    // MA# ( 𑢲 → L ) WARANG CITI CAPITAL LETTER TTE → LATIN CAPITAL LETTER L#
    {71861, "O"},    // MA# ( 𑢵 → O ) WARANG CITI CAPITAL LETTER AT → LATIN CAPITAL LETTER O#
    {71864, "U"},    // MA# ( 𑢸 → U ) WARANG CITI CAPITAL LETTER PU → LATIN CAPITAL LETTER U#
    {71867, "5"},    // MA# ( 𑢻 → 5 ) WARANG CITI CAPITAL LETTER HORR → DIGIT FIVE#
    {71868, "T"},    // MA# ( 𑢼 → T ) WARANG CITI CAPITAL LETTER HAR → LATIN CAPITAL LETTER T#
    {71872, "v"},    // MA# ( 𑣀 → v ) WARANG CITI SMALL LETTER NGAA → LATIN SMALL LETTER V#
    {71873, "s"},    // MA# ( 𑣁 → s ) WARANG CITI SMALL LETTER A → LATIN SMALL LETTER S#
    {71874, "F"},    // MA# ( 𑣂 → F ) WARANG CITI SMALL LETTER WI → LATIN CAPITAL LETTER F#
    {71875, "i"},    // MA# ( 𑣃 → i ) WARANG CITI SMALL LETTER YU → LATIN SMALL LETTER I# →ι→
    {71876, "z"},    // MA# ( 𑣄 → z ) WARANG CITI SMALL LETTER YA → LATIN SMALL LETTER Z#
    {71878, "7"},    // MA# ( 𑣆 → 7 ) WARANG CITI SMALL LETTER II → DIGIT SEVEN#
    {71880, "o"},    // MA# ( 𑣈 → o ) WARANG CITI SMALL LETTER E → LATIN SMALL LETTER O#
    {71882, "3"},    // MA# ( 𑣊 → 3 ) WARANG CITI SMALL LETTER ANG → DIGIT THREE#
    {71884, "9"},    // MA# ( 𑣌 → 9 ) WARANG CITI SMALL LETTER KO → DIGIT NINE#
    {71893, "6"},    // MA# ( 𑣕 → 6 ) WARANG CITI SMALL LETTER AT → DIGIT SIX#
    {71894, "9"},    // MA# ( 𑣖 → 9 ) WARANG CITI SMALL LETTER AM → DIGIT NINE#
    {71895, "o"},    // MA# ( 𑣗 → o ) WARANG CITI SMALL LETTER BU → LATIN SMALL LETTER O#
    {71896, "u"},    // MA# ( 𑣘 → u ) WARANG CITI SMALL LETTER PU → LATIN SMALL LETTER U# →υ→→ʋ→
    {71900, "y"},    // MA# ( 𑣜 → y ) WARANG CITI SMALL LETTER HAR → LATIN SMALL LETTER Y# →ɣ→→γ→
    {71904, "O"},    // MA# ( 𑣠 → O ) WARANG CITI DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {71907, "rn"},   // MA# ( 𑣣 → rn ) WARANG CITI DIGIT THREE → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {71909, "Z"},    // MA# ( 𑣥 → Z ) WARANG CITI DIGIT FIVE → LATIN CAPITAL LETTER Z#
    {71910, "W"},    // MA# ( 𑣦 → W ) WARANG CITI DIGIT SIX → LATIN CAPITAL LETTER W#
    {71913, "C"},    // MA# ( 𑣩 → C ) WARANG CITI DIGIT NINE → LATIN CAPITAL LETTER C#
    {71916, "X"},    // MA#* ( 𑣬 → X ) WARANG CITI NUMBER THIRTY → LATIN CAPITAL LETTER X#
    {71919, "W"},    // MA#* ( 𑣯 → W ) WARANG CITI NUMBER SIXTY → LATIN CAPITAL LETTER W#
    {71922, "C"},    // MA#* ( 𑣲 → C ) WARANG CITI NUMBER NINETY → LATIN CAPITAL LETTER C#
    {93960, "V"},    // MA# ( 𖼈 → V ) MIAO LETTER VA → LATIN CAPITAL LETTER V#
    {93962, "T"},    // MA# ( 𖼊 → T ) MIAO LETTER TA → LATIN CAPITAL LETTER T#
    {93974, "L"},    // MA# ( 𖼖 → L ) MIAO LETTER LA → LATIN CAPITAL LETTER L#
    {93992, "l"},    // MA# ( 𖼨 → l ) MIAO LETTER GHA → LATIN SMALL LETTER L# →I→
    {94005, "R"},    // MA# ( 𖼵 → R ) MIAO LETTER ZHA → LATIN CAPITAL LETTER R#
    {94010, "S"},    // MA# ( 𖼺 → S ) MIAO LETTER SA → LATIN CAPITAL LETTER S#
    {94011, "3"},    // MA# ( 𖼻 → 3 ) MIAO LETTER ZA → DIGIT THREE# →Ʒ→
    {94015, ">"},    // MA# ( 𖼿 → > ) MIAO LETTER ARCHAIC ZZA → GREATER-THAN SIGN#
    {94016, "A"},    // MA# ( 𖽀 → A ) MIAO LETTER ZZYA → LATIN CAPITAL LETTER A#
    {94018, "U"},    // MA# ( 𖽂 → U ) MIAO LETTER WA → LATIN CAPITAL LETTER U#
    {94019, "Y"},    // MA# ( 𖽃 → Y ) MIAO LETTER AH → LATIN CAPITAL LETTER Y#
    {94033, "'"},    // MA# ( 𖽑 → ' ) MIAO SIGN ASPIRATION → APOSTROPHE# →ʼ→→′→
    {94034, "'"},    // MA# ( 𖽒 → ' ) MIAO SIGN REFORMED VOICING → APOSTROPHE# →ʻ→→‘→
    {119060, "{"},   // MA#* ( 𝄔 → { ) MUSICAL SYMBOL BRACE → LEFT CURLY BRACKET#
    {119149, "."},   // MA# ( 𝅭 → . ) MUSICAL SYMBOL COMBINING AUGMENTATION DOT → FULL STOP#
    {119302, "3"},   // MA#* ( 𝈆 → 3 ) GREEK VOCAL NOTATION SYMBOL-7 → DIGIT THREE#
    {119309, "V"},   // MA#* ( 𝈍 → V ) GREEK VOCAL NOTATION SYMBOL-14 → LATIN CAPITAL LETTER V#
    {119311, "\\"},  // MA#* ( 𝈏 → \ ) GREEK VOCAL NOTATION SYMBOL-16 → REVERSE SOLIDUS#
    {119314, "7"},   // MA#* ( 𝈒 → 7 ) GREEK VOCAL NOTATION SYMBOL-19 → DIGIT SEVEN#
    {119315, "F"},   // MA#* ( 𝈓 → F ) GREEK VOCAL NOTATION SYMBOL-20 → LATIN CAPITAL LETTER F# →Ϝ→
    {119318, "R"},   // MA#* ( 𝈖 → R ) GREEK VOCAL NOTATION SYMBOL-23 → LATIN CAPITAL LETTER R#
    {119338, "L"},   // MA#* ( 𝈪 → L ) GREEK INSTRUMENTAL NOTATION SYMBOL-23 → LATIN CAPITAL LETTER L#
    {119350, "<"},   // MA#* ( 𝈶 → < ) GREEK INSTRUMENTAL NOTATION SYMBOL-40 → LESS-THAN SIGN#
    {119351, ">"},   // MA#* ( 𝈷 → > ) GREEK INSTRUMENTAL NOTATION SYMBOL-42 → GREATER-THAN SIGN#
    {119354, "/"},   // MA#* ( 𝈺 → / ) GREEK INSTRUMENTAL NOTATION SYMBOL-47 → SOLIDUS#
    {119355, "\\"},  // MA#* ( 𝈻 → \ ) GREEK INSTRUMENTAL NOTATION SYMBOL-48 → REVERSE SOLIDUS# →𝈏→
    {119808, "A"},   // MA# ( 𝐀 → A ) MATHEMATICAL BOLD CAPITAL A → LATIN CAPITAL LETTER A#
    {119809, "B"},   // MA# ( 𝐁 → B ) MATHEMATICAL BOLD CAPITAL B → LATIN CAPITAL LETTER B#
    {119810, "C"},   // MA# ( 𝐂 → C ) MATHEMATICAL BOLD CAPITAL C → LATIN CAPITAL LETTER C#
    {119811, "D"},   // MA# ( 𝐃 → D ) MATHEMATICAL BOLD CAPITAL D → LATIN CAPITAL LETTER D#
    {119812, "E"},   // MA# ( 𝐄 → E ) MATHEMATICAL BOLD CAPITAL E → LATIN CAPITAL LETTER E#
    {119813, "F"},   // MA# ( 𝐅 → F ) MATHEMATICAL BOLD CAPITAL F → LATIN CAPITAL LETTER F#
    {119814, "G"},   // MA# ( 𝐆 → G ) MATHEMATICAL BOLD CAPITAL G → LATIN CAPITAL LETTER G#
    {119815, "H"},   // MA# ( 𝐇 → H ) MATHEMATICAL BOLD CAPITAL H → LATIN CAPITAL LETTER H#
    {119816, "l"},   // MA# ( 𝐈 → l ) MATHEMATICAL BOLD CAPITAL I → LATIN SMALL LETTER L# →I→
    {119817, "J"},   // MA# ( 𝐉 → J ) MATHEMATICAL BOLD CAPITAL J → LATIN CAPITAL LETTER J#
    {119818, "K"},   // MA# ( 𝐊 → K ) MATHEMATICAL BOLD CAPITAL K → LATIN CAPITAL LETTER K#
    {119819, "L"},   // MA# ( 𝐋 → L ) MATHEMATICAL BOLD CAPITAL L → LATIN CAPITAL LETTER L#
    {119820, "M"},   // MA# ( 𝐌 → M ) MATHEMATICAL BOLD CAPITAL M → LATIN CAPITAL LETTER M#
    {119821, "N"},   // MA# ( 𝐍 → N ) MATHEMATICAL BOLD CAPITAL N → LATIN CAPITAL LETTER N#
    {119822, "O"},   // MA# ( 𝐎 → O ) MATHEMATICAL BOLD CAPITAL O → LATIN CAPITAL LETTER O#
    {119823, "P"},   // MA# ( 𝐏 → P ) MATHEMATICAL BOLD CAPITAL P → LATIN CAPITAL LETTER P#
    {119824, "Q"},   // MA# ( 𝐐 → Q ) MATHEMATICAL BOLD CAPITAL Q → LATIN CAPITAL LETTER Q#
    {119825, "R"},   // MA# ( 𝐑 → R ) MATHEMATICAL BOLD CAPITAL R → LATIN CAPITAL LETTER R#
    {119826, "S"},   // MA# ( 𝐒 → S ) MATHEMATICAL BOLD CAPITAL S → LATIN CAPITAL LETTER S#
    {119827, "T"},   // MA# ( 𝐓 → T ) MATHEMATICAL BOLD CAPITAL T → LATIN CAPITAL LETTER T#
    {119828, "U"},   // MA# ( 𝐔 → U ) MATHEMATICAL BOLD CAPITAL U → LATIN CAPITAL LETTER U#
    {119829, "V"},   // MA# ( 𝐕 → V ) MATHEMATICAL BOLD CAPITAL V → LATIN CAPITAL LETTER V#
    {119830, "W"},   // MA# ( 𝐖 → W ) MATHEMATICAL BOLD CAPITAL W → LATIN CAPITAL LETTER W#
    {119831, "X"},   // MA# ( 𝐗 → X ) MATHEMATICAL BOLD CAPITAL X → LATIN CAPITAL LETTER X#
    {119832, "Y"},   // MA# ( 𝐘 → Y ) MATHEMATICAL BOLD CAPITAL Y → LATIN CAPITAL LETTER Y#
    {119833, "Z"},   // MA# ( 𝐙 → Z ) MATHEMATICAL BOLD CAPITAL Z → LATIN CAPITAL LETTER Z#
    {119834, "a"},   // MA# ( 𝐚 → a ) MATHEMATICAL BOLD SMALL A → LATIN SMALL LETTER A#
    {119835, "b"},   // MA# ( 𝐛 → b ) MATHEMATICAL BOLD SMALL B → LATIN SMALL LETTER B#
    {119836, "c"},   // MA# ( 𝐜 → c ) MATHEMATICAL BOLD SMALL C → LATIN SMALL LETTER C#
    {119837, "d"},   // MA# ( 𝐝 → d ) MATHEMATICAL BOLD SMALL D → LATIN SMALL LETTER D#
    {119838, "e"},   // MA# ( 𝐞 → e ) MATHEMATICAL BOLD SMALL E → LATIN SMALL LETTER E#
    {119839, "f"},   // MA# ( 𝐟 → f ) MATHEMATICAL BOLD SMALL F → LATIN SMALL LETTER F#
    {119840, "g"},   // MA# ( 𝐠 → g ) MATHEMATICAL BOLD SMALL G → LATIN SMALL LETTER G#
    {119841, "h"},   // MA# ( 𝐡 → h ) MATHEMATICAL BOLD SMALL H → LATIN SMALL LETTER H#
    {119842, "i"},   // MA# ( 𝐢 → i ) MATHEMATICAL BOLD SMALL I → LATIN SMALL LETTER I#
    {119843, "j"},   // MA# ( 𝐣 → j ) MATHEMATICAL BOLD SMALL J → LATIN SMALL LETTER J#
    {119844, "k"},   // MA# ( 𝐤 → k ) MATHEMATICAL BOLD SMALL K → LATIN SMALL LETTER K#
    {119845, "l"},   // MA# ( 𝐥 → l ) MATHEMATICAL BOLD SMALL L → LATIN SMALL LETTER L#
    {119846, "rn"},  // MA# ( 𝐦 → rn ) MATHEMATICAL BOLD SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {119847, "n"},   // MA# ( 𝐧 → n ) MATHEMATICAL BOLD SMALL N → LATIN SMALL LETTER N#
    {119848, "o"},   // MA# ( 𝐨 → o ) MATHEMATICAL BOLD SMALL O → LATIN SMALL LETTER O#
    {119849, "p"},   // MA# ( 𝐩 → p ) MATHEMATICAL BOLD SMALL P → LATIN SMALL LETTER P#
    {119850, "q"},   // MA# ( 𝐪 → q ) MATHEMATICAL BOLD SMALL Q → LATIN SMALL LETTER Q#
    {119851, "r"},   // MA# ( 𝐫 → r ) MATHEMATICAL BOLD SMALL R → LATIN SMALL LETTER R#
    {119852, "s"},   // MA# ( 𝐬 → s ) MATHEMATICAL BOLD SMALL S → LATIN SMALL LETTER S#
    {119853, "t"},   // MA# ( 𝐭 → t ) MATHEMATICAL BOLD SMALL T → LATIN SMALL LETTER T#
    {119854, "u"},   // MA# ( 𝐮 → u ) MATHEMATICAL BOLD SMALL U → LATIN SMALL LETTER U#
    {119855, "v"},   // MA# ( 𝐯 → v ) MATHEMATICAL BOLD SMALL V → LATIN SMALL LETTER V#
    {119856, "w"},   // MA# ( 𝐰 → w ) MATHEMATICAL BOLD SMALL W → LATIN SMALL LETTER W#
    {119857, "x"},   // MA# ( 𝐱 → x ) MATHEMATICAL BOLD SMALL X → LATIN SMALL LETTER X#
    {119858, "y"},   // MA# ( 𝐲 → y ) MATHEMATICAL BOLD SMALL Y → LATIN SMALL LETTER Y#
    {119859, "z"},   // MA# ( 𝐳 → z ) MATHEMATICAL BOLD SMALL Z → LATIN SMALL LETTER Z#
    {119860, "A"},   // MA# ( 𝐴 → A ) MATHEMATICAL ITALIC CAPITAL A → LATIN CAPITAL LETTER A#
    {119861, "B"},   // MA# ( 𝐵 → B ) MATHEMATICAL ITALIC CAPITAL B → LATIN CAPITAL LETTER B#
    {119862, "C"},   // MA# ( 𝐶 → C ) MATHEMATICAL ITALIC CAPITAL C → LATIN CAPITAL LETTER C#
    {119863, "D"},   // MA# ( 𝐷 → D ) MATHEMATICAL ITALIC CAPITAL D → LATIN CAPITAL LETTER D#
    {119864, "E"},   // MA# ( 𝐸 → E ) MATHEMATICAL ITALIC CAPITAL E → LATIN CAPITAL LETTER E#
    {119865, "F"},   // MA# ( 𝐹 → F ) MATHEMATICAL ITALIC CAPITAL F → LATIN CAPITAL LETTER F#
    {119866, "G"},   // MA# ( 𝐺 → G ) MATHEMATICAL ITALIC CAPITAL G → LATIN CAPITAL LETTER G#
    {119867, "H"},   // MA# ( 𝐻 → H ) MATHEMATICAL ITALIC CAPITAL H → LATIN CAPITAL LETTER H#
    {119868, "l"},   // MA# ( 𝐼 → l ) MATHEMATICAL ITALIC CAPITAL I → LATIN SMALL LETTER L# →I→
    {119869, "J"},   // MA# ( 𝐽 → J ) MATHEMATICAL ITALIC CAPITAL J → LATIN CAPITAL LETTER J#
    {119870, "K"},   // MA# ( 𝐾 → K ) MATHEMATICAL ITALIC CAPITAL K → LATIN CAPITAL LETTER K#
    {119871, "L"},   // MA# ( 𝐿 → L ) MATHEMATICAL ITALIC CAPITAL L → LATIN CAPITAL LETTER L#
    {119872, "M"},   // MA# ( 𝑀 → M ) MATHEMATICAL ITALIC CAPITAL M → LATIN CAPITAL LETTER M#
    {119873, "N"},   // MA# ( 𝑁 → N ) MATHEMATICAL ITALIC CAPITAL N → LATIN CAPITAL LETTER N#
    {119874, "O"},   // MA# ( 𝑂 → O ) MATHEMATICAL ITALIC CAPITAL O → LATIN CAPITAL LETTER O#
    {119875, "P"},   // MA# ( 𝑃 → P ) MATHEMATICAL ITALIC CAPITAL P → LATIN CAPITAL LETTER P#
    {119876, "Q"},   // MA# ( 𝑄 → Q ) MATHEMATICAL ITALIC CAPITAL Q → LATIN CAPITAL LETTER Q#
    {119877, "R"},   // MA# ( 𝑅 → R ) MATHEMATICAL ITALIC CAPITAL R → LATIN CAPITAL LETTER R#
    {119878, "S"},   // MA# ( 𝑆 → S ) MATHEMATICAL ITALIC CAPITAL S → LATIN CAPITAL LETTER S#
    {119879, "T"},   // MA# ( 𝑇 → T ) MATHEMATICAL ITALIC CAPITAL T → LATIN CAPITAL LETTER T#
    {119880, "U"},   // MA# ( 𝑈 → U ) MATHEMATICAL ITALIC CAPITAL U → LATIN CAPITAL LETTER U#
    {119881, "V"},   // MA# ( 𝑉 → V ) MATHEMATICAL ITALIC CAPITAL V → LATIN CAPITAL LETTER V#
    {119882, "W"},   // MA# ( 𝑊 → W ) MATHEMATICAL ITALIC CAPITAL W → LATIN CAPITAL LETTER W#
    {119883, "X"},   // MA# ( 𝑋 → X ) MATHEMATICAL ITALIC CAPITAL X → LATIN CAPITAL LETTER X#
    {119884, "Y"},   // MA# ( 𝑌 → Y ) MATHEMATICAL ITALIC CAPITAL Y → LATIN CAPITAL LETTER Y#
    {119885, "Z"},   // MA# ( 𝑍 → Z ) MATHEMATICAL ITALIC CAPITAL Z → LATIN CAPITAL LETTER Z#
    {119886, "a"},   // MA# ( 𝑎 → a ) MATHEMATICAL ITALIC SMALL A → LATIN SMALL LETTER A#
    {119887, "b"},   // MA# ( 𝑏 → b ) MATHEMATICAL ITALIC SMALL B → LATIN SMALL LETTER B#
    {119888, "c"},   // MA# ( 𝑐 → c ) MATHEMATICAL ITALIC SMALL C → LATIN SMALL LETTER C#
    {119889, "d"},   // MA# ( 𝑑 → d ) MATHEMATICAL ITALIC SMALL D → LATIN SMALL LETTER D#
    {119890, "e"},   // MA# ( 𝑒 → e ) MATHEMATICAL ITALIC SMALL E → LATIN SMALL LETTER E#
    {119891, "f"},   // MA# ( 𝑓 → f ) MATHEMATICAL ITALIC SMALL F → LATIN SMALL LETTER F#
    {119892, "g"},   // MA# ( 𝑔 → g ) MATHEMATICAL ITALIC SMALL G → LATIN SMALL LETTER G#
    {119894, "i"},   // MA# ( 𝑖 → i ) MATHEMATICAL ITALIC SMALL I → LATIN SMALL LETTER I#
    {119895, "j"},   // MA# ( 𝑗 → j ) MATHEMATICAL ITALIC SMALL J → LATIN SMALL LETTER J#
    {119896, "k"},   // MA# ( 𝑘 → k ) MATHEMATICAL ITALIC SMALL K → LATIN SMALL LETTER K#
    {119897, "l"},   // MA# ( 𝑙 → l ) MATHEMATICAL ITALIC SMALL L → LATIN SMALL LETTER L#
    {119898, "rn"},  // MA# ( 𝑚 → rn ) MATHEMATICAL ITALIC SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {119899, "n"},   // MA# ( 𝑛 → n ) MATHEMATICAL ITALIC SMALL N → LATIN SMALL LETTER N#
    {119900, "o"},   // MA# ( 𝑜 → o ) MATHEMATICAL ITALIC SMALL O → LATIN SMALL LETTER O#
    {119901, "p"},   // MA# ( 𝑝 → p ) MATHEMATICAL ITALIC SMALL P → LATIN SMALL LETTER P#
    {119902, "q"},   // MA# ( 𝑞 → q ) MATHEMATICAL ITALIC SMALL Q → LATIN SMALL LETTER Q#
    {119903, "r"},   // MA# ( 𝑟 → r ) MATHEMATICAL ITALIC SMALL R → LATIN SMALL LETTER R#
    {119904, "s"},   // MA# ( 𝑠 → s ) MATHEMATICAL ITALIC SMALL S → LATIN SMALL LETTER S#
    {119905, "t"},   // MA# ( 𝑡 → t ) MATHEMATICAL ITALIC SMALL T → LATIN SMALL LETTER T#
    {119906, "u"},   // MA# ( 𝑢 → u ) MATHEMATICAL ITALIC SMALL U → LATIN SMALL LETTER U#
    {119907, "v"},   // MA# ( 𝑣 → v ) MATHEMATICAL ITALIC SMALL V → LATIN SMALL LETTER V#
    {119908, "w"},   // MA# ( 𝑤 → w ) MATHEMATICAL ITALIC SMALL W → LATIN SMALL LETTER W#
    {119909, "x"},   // MA# ( 𝑥 → x ) MATHEMATICAL ITALIC SMALL X → LATIN SMALL LETTER X#
    {119910, "y"},   // MA# ( 𝑦 → y ) MATHEMATICAL ITALIC SMALL Y → LATIN SMALL LETTER Y#
    {119911, "z"},   // MA# ( 𝑧 → z ) MATHEMATICAL ITALIC SMALL Z → LATIN SMALL LETTER Z#
    {119912, "A"},   // MA# ( 𝑨 → A ) MATHEMATICAL BOLD ITALIC CAPITAL A → LATIN CAPITAL LETTER A#
    {119913, "B"},   // MA# ( 𝑩 → B ) MATHEMATICAL BOLD ITALIC CAPITAL B → LATIN CAPITAL LETTER B#
    {119914, "C"},   // MA# ( 𝑪 → C ) MATHEMATICAL BOLD ITALIC CAPITAL C → LATIN CAPITAL LETTER C#
    {119915, "D"},   // MA# ( 𝑫 → D ) MATHEMATICAL BOLD ITALIC CAPITAL D → LATIN CAPITAL LETTER D#
    {119916, "E"},   // MA# ( 𝑬 → E ) MATHEMATICAL BOLD ITALIC CAPITAL E → LATIN CAPITAL LETTER E#
    {119917, "F"},   // MA# ( 𝑭 → F ) MATHEMATICAL BOLD ITALIC CAPITAL F → LATIN CAPITAL LETTER F#
    {119918, "G"},   // MA# ( 𝑮 → G ) MATHEMATICAL BOLD ITALIC CAPITAL G → LATIN CAPITAL LETTER G#
    {119919, "H"},   // MA# ( 𝑯 → H ) MATHEMATICAL BOLD ITALIC CAPITAL H → LATIN CAPITAL LETTER H#
    {119920, "l"},   // MA# ( 𝑰 → l ) MATHEMATICAL BOLD ITALIC CAPITAL I → LATIN SMALL LETTER L# →I→
    {119921, "J"},   // MA# ( 𝑱 → J ) MATHEMATICAL BOLD ITALIC CAPITAL J → LATIN CAPITAL LETTER J#
    {119922, "K"},   // MA# ( 𝑲 → K ) MATHEMATICAL BOLD ITALIC CAPITAL K → LATIN CAPITAL LETTER K#
    {119923, "L"},   // MA# ( 𝑳 → L ) MATHEMATICAL BOLD ITALIC CAPITAL L → LATIN CAPITAL LETTER L#
    {119924, "M"},   // MA# ( 𝑴 → M ) MATHEMATICAL BOLD ITALIC CAPITAL M → LATIN CAPITAL LETTER M#
    {119925, "N"},   // MA# ( 𝑵 → N ) MATHEMATICAL BOLD ITALIC CAPITAL N → LATIN CAPITAL LETTER N#
    {119926, "O"},   // MA# ( 𝑶 → O ) MATHEMATICAL BOLD ITALIC CAPITAL O → LATIN CAPITAL LETTER O#
    {119927, "P"},   // MA# ( 𝑷 → P ) MATHEMATICAL BOLD ITALIC CAPITAL P → LATIN CAPITAL LETTER P#
    {119928, "Q"},   // MA# ( 𝑸 → Q ) MATHEMATICAL BOLD ITALIC CAPITAL Q → LATIN CAPITAL LETTER Q#
    {119929, "R"},   // MA# ( 𝑹 → R ) MATHEMATICAL BOLD ITALIC CAPITAL R → LATIN CAPITAL LETTER R#
    {119930, "S"},   // MA# ( 𝑺 → S ) MATHEMATICAL BOLD ITALIC CAPITAL S → LATIN CAPITAL LETTER S#
    {119931, "T"},   // MA# ( 𝑻 → T ) MATHEMATICAL BOLD ITALIC CAPITAL T → LATIN CAPITAL LETTER T#
    {119932, "U"},   // MA# ( 𝑼 → U ) MATHEMATICAL BOLD ITALIC CAPITAL U → LATIN CAPITAL LETTER U#
    {119933, "V"},   // MA# ( 𝑽 → V ) MATHEMATICAL BOLD ITALIC CAPITAL V → LATIN CAPITAL LETTER V#
    {119934, "W"},   // MA# ( 𝑾 → W ) MATHEMATICAL BOLD ITALIC CAPITAL W → LATIN CAPITAL LETTER W#
    {119935, "X"},   // MA# ( 𝑿 → X ) MATHEMATICAL BOLD ITALIC CAPITAL X → LATIN CAPITAL LETTER X#
    {119936, "Y"},   // MA# ( 𝒀 → Y ) MATHEMATICAL BOLD ITALIC CAPITAL Y → LATIN CAPITAL LETTER Y#
    {119937, "Z"},   // MA# ( 𝒁 → Z ) MATHEMATICAL BOLD ITALIC CAPITAL Z → LATIN CAPITAL LETTER Z#
    {119938, "a"},   // MA# ( 𝒂 → a ) MATHEMATICAL BOLD ITALIC SMALL A → LATIN SMALL LETTER A#
    {119939, "b"},   // MA# ( 𝒃 → b ) MATHEMATICAL BOLD ITALIC SMALL B → LATIN SMALL LETTER B#
    {119940, "c"},   // MA# ( 𝒄 → c ) MATHEMATICAL BOLD ITALIC SMALL C → LATIN SMALL LETTER C#
    {119941, "d"},   // MA# ( 𝒅 → d ) MATHEMATICAL BOLD ITALIC SMALL D → LATIN SMALL LETTER D#
    {119942, "e"},   // MA# ( 𝒆 → e ) MATHEMATICAL BOLD ITALIC SMALL E → LATIN SMALL LETTER E#
    {119943, "f"},   // MA# ( 𝒇 → f ) MATHEMATICAL BOLD ITALIC SMALL F → LATIN SMALL LETTER F#
    {119944, "g"},   // MA# ( 𝒈 → g ) MATHEMATICAL BOLD ITALIC SMALL G → LATIN SMALL LETTER G#
    {119945, "h"},   // MA# ( 𝒉 → h ) MATHEMATICAL BOLD ITALIC SMALL H → LATIN SMALL LETTER H#
    {119946, "i"},   // MA# ( 𝒊 → i ) MATHEMATICAL BOLD ITALIC SMALL I → LATIN SMALL LETTER I#
    {119947, "j"},   // MA# ( 𝒋 → j ) MATHEMATICAL BOLD ITALIC SMALL J → LATIN SMALL LETTER J#
    {119948, "k"},   // MA# ( 𝒌 → k ) MATHEMATICAL BOLD ITALIC SMALL K → LATIN SMALL LETTER K#
    {119949, "l"},   // MA# ( 𝒍 → l ) MATHEMATICAL BOLD ITALIC SMALL L → LATIN SMALL LETTER L#
    {119950, "rn"},  // MA# ( 𝒎 → rn ) MATHEMATICAL BOLD ITALIC SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {119951, "n"},   // MA# ( 𝒏 → n ) MATHEMATICAL BOLD ITALIC SMALL N → LATIN SMALL LETTER N#
    {119952, "o"},   // MA# ( 𝒐 → o ) MATHEMATICAL BOLD ITALIC SMALL O → LATIN SMALL LETTER O#
    {119953, "p"},   // MA# ( 𝒑 → p ) MATHEMATICAL BOLD ITALIC SMALL P → LATIN SMALL LETTER P#
    {119954, "q"},   // MA# ( 𝒒 → q ) MATHEMATICAL BOLD ITALIC SMALL Q → LATIN SMALL LETTER Q#
    {119955, "r"},   // MA# ( 𝒓 → r ) MATHEMATICAL BOLD ITALIC SMALL R → LATIN SMALL LETTER R#
    {119956, "s"},   // MA# ( 𝒔 → s ) MATHEMATICAL BOLD ITALIC SMALL S → LATIN SMALL LETTER S#
    {119957, "t"},   // MA# ( 𝒕 → t ) MATHEMATICAL BOLD ITALIC SMALL T → LATIN SMALL LETTER T#
    {119958, "u"},   // MA# ( 𝒖 → u ) MATHEMATICAL BOLD ITALIC SMALL U → LATIN SMALL LETTER U#
    {119959, "v"},   // MA# ( 𝒗 → v ) MATHEMATICAL BOLD ITALIC SMALL V → LATIN SMALL LETTER V#
    {119960, "w"},   // MA# ( 𝒘 → w ) MATHEMATICAL BOLD ITALIC SMALL W → LATIN SMALL LETTER W#
    {119961, "x"},   // MA# ( 𝒙 → x ) MATHEMATICAL BOLD ITALIC SMALL X → LATIN SMALL LETTER X#
    {119962, "y"},   // MA# ( 𝒚 → y ) MATHEMATICAL BOLD ITALIC SMALL Y → LATIN SMALL LETTER Y#
    {119963, "z"},   // MA# ( 𝒛 → z ) MATHEMATICAL BOLD ITALIC SMALL Z → LATIN SMALL LETTER Z#
    {119964, "A"},   // MA# ( 𝒜 → A ) MATHEMATICAL SCRIPT CAPITAL A → LATIN CAPITAL LETTER A#
    {119966, "C"},   // MA# ( 𝒞 → C ) MATHEMATICAL SCRIPT CAPITAL C → LATIN CAPITAL LETTER C#
    {119967, "D"},   // MA# ( 𝒟 → D ) MATHEMATICAL SCRIPT CAPITAL D → LATIN CAPITAL LETTER D#
    {119970, "G"},   // MA# ( 𝒢 → G ) MATHEMATICAL SCRIPT CAPITAL G → LATIN CAPITAL LETTER G#
    {119973, "J"},   // MA# ( 𝒥 → J ) MATHEMATICAL SCRIPT CAPITAL J → LATIN CAPITAL LETTER J#
    {119974, "K"},   // MA# ( 𝒦 → K ) MATHEMATICAL SCRIPT CAPITAL K → LATIN CAPITAL LETTER K#
    {119977, "N"},   // MA# ( 𝒩 → N ) MATHEMATICAL SCRIPT CAPITAL N → LATIN CAPITAL LETTER N#
    {119978, "O"},   // MA# ( 𝒪 → O ) MATHEMATICAL SCRIPT CAPITAL O → LATIN CAPITAL LETTER O#
    {119979, "P"},   // MA# ( 𝒫 → P ) MATHEMATICAL SCRIPT CAPITAL P → LATIN CAPITAL LETTER P#
    {119980, "Q"},   // MA# ( 𝒬 → Q ) MATHEMATICAL SCRIPT CAPITAL Q → LATIN CAPITAL LETTER Q#
    {119982, "S"},   // MA# ( 𝒮 → S ) MATHEMATICAL SCRIPT CAPITAL S → LATIN CAPITAL LETTER S#
    {119983, "T"},   // MA# ( 𝒯 → T ) MATHEMATICAL SCRIPT CAPITAL T → LATIN CAPITAL LETTER T#
    {119984, "U"},   // MA# ( 𝒰 → U ) MATHEMATICAL SCRIPT CAPITAL U → LATIN CAPITAL LETTER U#
    {119985, "V"},   // MA# ( 𝒱 → V ) MATHEMATICAL SCRIPT CAPITAL V → LATIN CAPITAL LETTER V#
    {119986, "W"},   // MA# ( 𝒲 → W ) MATHEMATICAL SCRIPT CAPITAL W → LATIN CAPITAL LETTER W#
    {119987, "X"},   // MA# ( 𝒳 → X ) MATHEMATICAL SCRIPT CAPITAL X → LATIN CAPITAL LETTER X#
    {119988, "Y"},   // MA# ( 𝒴 → Y ) MATHEMATICAL SCRIPT CAPITAL Y → LATIN CAPITAL LETTER Y#
    {119989, "Z"},   // MA# ( 𝒵 → Z ) MATHEMATICAL SCRIPT CAPITAL Z → LATIN CAPITAL LETTER Z#
    {119990, "a"},   // MA# ( 𝒶 → a ) MATHEMATICAL SCRIPT SMALL A → LATIN SMALL LETTER A#
    {119991, "b"},   // MA# ( 𝒷 → b ) MATHEMATICAL SCRIPT SMALL B → LATIN SMALL LETTER B#
    {119992, "c"},   // MA# ( 𝒸 → c ) MATHEMATICAL SCRIPT SMALL C → LATIN SMALL LETTER C#
    {119993, "d"},   // MA# ( 𝒹 → d ) MATHEMATICAL SCRIPT SMALL D → LATIN SMALL LETTER D#
    {119995, "f"},   // MA# ( 𝒻 → f ) MATHEMATICAL SCRIPT SMALL F → LATIN SMALL LETTER F#
    {119997, "h"},   // MA# ( 𝒽 → h ) MATHEMATICAL SCRIPT SMALL H → LATIN SMALL LETTER H#
    {119998, "i"},   // MA# ( 𝒾 → i ) MATHEMATICAL SCRIPT SMALL I → LATIN SMALL LETTER I#
    {119999, "j"},   // MA# ( 𝒿 → j ) MATHEMATICAL SCRIPT SMALL J → LATIN SMALL LETTER J#
    {120000, "k"},   // MA# ( 𝓀 → k ) MATHEMATICAL SCRIPT SMALL K → LATIN SMALL LETTER K#
    {120001, "l"},   // MA# ( 𝓁 → l ) MATHEMATICAL SCRIPT SMALL L → LATIN SMALL LETTER L#
    {120002, "rn"},  // MA# ( 𝓂 → rn ) MATHEMATICAL SCRIPT SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120003, "n"},   // MA# ( 𝓃 → n ) MATHEMATICAL SCRIPT SMALL N → LATIN SMALL LETTER N#
    {120005, "p"},   // MA# ( 𝓅 → p ) MATHEMATICAL SCRIPT SMALL P → LATIN SMALL LETTER P#
    {120006, "q"},   // MA# ( 𝓆 → q ) MATHEMATICAL SCRIPT SMALL Q → LATIN SMALL LETTER Q#
    {120007, "r"},   // MA# ( 𝓇 → r ) MATHEMATICAL SCRIPT SMALL R → LATIN SMALL LETTER R#
    {120008, "s"},   // MA# ( 𝓈 → s ) MATHEMATICAL SCRIPT SMALL S → LATIN SMALL LETTER S#
    {120009, "t"},   // MA# ( 𝓉 → t ) MATHEMATICAL SCRIPT SMALL T → LATIN SMALL LETTER T#
    {120010, "u"},   // MA# ( 𝓊 → u ) MATHEMATICAL SCRIPT SMALL U → LATIN SMALL LETTER U#
    {120011, "v"},   // MA# ( 𝓋 → v ) MATHEMATICAL SCRIPT SMALL V → LATIN SMALL LETTER V#
    {120012, "w"},   // MA# ( 𝓌 → w ) MATHEMATICAL SCRIPT SMALL W → LATIN SMALL LETTER W#
    {120013, "x"},   // MA# ( 𝓍 → x ) MATHEMATICAL SCRIPT SMALL X → LATIN SMALL LETTER X#
    {120014, "y"},   // MA# ( 𝓎 → y ) MATHEMATICAL SCRIPT SMALL Y → LATIN SMALL LETTER Y#
    {120015, "z"},   // MA# ( 𝓏 → z ) MATHEMATICAL SCRIPT SMALL Z → LATIN SMALL LETTER Z#
    {120016, "A"},   // MA# ( 𝓐 → A ) MATHEMATICAL BOLD SCRIPT CAPITAL A → LATIN CAPITAL LETTER A#
    {120017, "B"},   // MA# ( 𝓑 → B ) MATHEMATICAL BOLD SCRIPT CAPITAL B → LATIN CAPITAL LETTER B#
    {120018, "C"},   // MA# ( 𝓒 → C ) MATHEMATICAL BOLD SCRIPT CAPITAL C → LATIN CAPITAL LETTER C#
    {120019, "D"},   // MA# ( 𝓓 → D ) MATHEMATICAL BOLD SCRIPT CAPITAL D → LATIN CAPITAL LETTER D#
    {120020, "E"},   // MA# ( 𝓔 → E ) MATHEMATICAL BOLD SCRIPT CAPITAL E → LATIN CAPITAL LETTER E#
    {120021, "F"},   // MA# ( 𝓕 → F ) MATHEMATICAL BOLD SCRIPT CAPITAL F → LATIN CAPITAL LETTER F#
    {120022, "G"},   // MA# ( 𝓖 → G ) MATHEMATICAL BOLD SCRIPT CAPITAL G → LATIN CAPITAL LETTER G#
    {120023, "H"},   // MA# ( 𝓗 → H ) MATHEMATICAL BOLD SCRIPT CAPITAL H → LATIN CAPITAL LETTER H#
    {120024, "l"},   // MA# ( 𝓘 → l ) MATHEMATICAL BOLD SCRIPT CAPITAL I → LATIN SMALL LETTER L# →I→
    {120025, "J"},   // MA# ( 𝓙 → J ) MATHEMATICAL BOLD SCRIPT CAPITAL J → LATIN CAPITAL LETTER J#
    {120026, "K"},   // MA# ( 𝓚 → K ) MATHEMATICAL BOLD SCRIPT CAPITAL K → LATIN CAPITAL LETTER K#
    {120027, "L"},   // MA# ( 𝓛 → L ) MATHEMATICAL BOLD SCRIPT CAPITAL L → LATIN CAPITAL LETTER L#
    {120028, "M"},   // MA# ( 𝓜 → M ) MATHEMATICAL BOLD SCRIPT CAPITAL M → LATIN CAPITAL LETTER M#
    {120029, "N"},   // MA# ( 𝓝 → N ) MATHEMATICAL BOLD SCRIPT CAPITAL N → LATIN CAPITAL LETTER N#
    {120030, "O"},   // MA# ( 𝓞 → O ) MATHEMATICAL BOLD SCRIPT CAPITAL O → LATIN CAPITAL LETTER O#
    {120031, "P"},   // MA# ( 𝓟 → P ) MATHEMATICAL BOLD SCRIPT CAPITAL P → LATIN CAPITAL LETTER P#
    {120032, "Q"},   // MA# ( 𝓠 → Q ) MATHEMATICAL BOLD SCRIPT CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120033, "R"},   // MA# ( 𝓡 → R ) MATHEMATICAL BOLD SCRIPT CAPITAL R → LATIN CAPITAL LETTER R#
    {120034, "S"},   // MA# ( 𝓢 → S ) MATHEMATICAL BOLD SCRIPT CAPITAL S → LATIN CAPITAL LETTER S#
    {120035, "T"},   // MA# ( 𝓣 → T ) MATHEMATICAL BOLD SCRIPT CAPITAL T → LATIN CAPITAL LETTER T#
    {120036, "U"},   // MA# ( 𝓤 → U ) MATHEMATICAL BOLD SCRIPT CAPITAL U → LATIN CAPITAL LETTER U#
    {120037, "V"},   // MA# ( 𝓥 → V ) MATHEMATICAL BOLD SCRIPT CAPITAL V → LATIN CAPITAL LETTER V#
    {120038, "W"},   // MA# ( 𝓦 → W ) MATHEMATICAL BOLD SCRIPT CAPITAL W → LATIN CAPITAL LETTER W#
    {120039, "X"},   // MA# ( 𝓧 → X ) MATHEMATICAL BOLD SCRIPT CAPITAL X → LATIN CAPITAL LETTER X#
    {120040, "Y"},   // MA# ( 𝓨 → Y ) MATHEMATICAL BOLD SCRIPT CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120041, "Z"},   // MA# ( 𝓩 → Z ) MATHEMATICAL BOLD SCRIPT CAPITAL Z → LATIN CAPITAL LETTER Z#
    {120042, "a"},   // MA# ( 𝓪 → a ) MATHEMATICAL BOLD SCRIPT SMALL A → LATIN SMALL LETTER A#
    {120043, "b"},   // MA# ( 𝓫 → b ) MATHEMATICAL BOLD SCRIPT SMALL B → LATIN SMALL LETTER B#
    {120044, "c"},   // MA# ( 𝓬 → c ) MATHEMATICAL BOLD SCRIPT SMALL C → LATIN SMALL LETTER C#
    {120045, "d"},   // MA# ( 𝓭 → d ) MATHEMATICAL BOLD SCRIPT SMALL D → LATIN SMALL LETTER D#
    {120046, "e"},   // MA# ( 𝓮 → e ) MATHEMATICAL BOLD SCRIPT SMALL E → LATIN SMALL LETTER E#
    {120047, "f"},   // MA# ( 𝓯 → f ) MATHEMATICAL BOLD SCRIPT SMALL F → LATIN SMALL LETTER F#
    {120048, "g"},   // MA# ( 𝓰 → g ) MATHEMATICAL BOLD SCRIPT SMALL G → LATIN SMALL LETTER G#
    {120049, "h"},   // MA# ( 𝓱 → h ) MATHEMATICAL BOLD SCRIPT SMALL H → LATIN SMALL LETTER H#
    {120050, "i"},   // MA# ( 𝓲 → i ) MATHEMATICAL BOLD SCRIPT SMALL I → LATIN SMALL LETTER I#
    {120051, "j"},   // MA# ( 𝓳 → j ) MATHEMATICAL BOLD SCRIPT SMALL J → LATIN SMALL LETTER J#
    {120052, "k"},   // MA# ( 𝓴 → k ) MATHEMATICAL BOLD SCRIPT SMALL K → LATIN SMALL LETTER K#
    {120053, "l"},   // MA# ( 𝓵 → l ) MATHEMATICAL BOLD SCRIPT SMALL L → LATIN SMALL LETTER L#
    {120054, "rn"},  // MA# ( 𝓶 → rn ) MATHEMATICAL BOLD SCRIPT SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120055, "n"},   // MA# ( 𝓷 → n ) MATHEMATICAL BOLD SCRIPT SMALL N → LATIN SMALL LETTER N#
    {120056, "o"},   // MA# ( 𝓸 → o ) MATHEMATICAL BOLD SCRIPT SMALL O → LATIN SMALL LETTER O#
    {120057, "p"},   // MA# ( 𝓹 → p ) MATHEMATICAL BOLD SCRIPT SMALL P → LATIN SMALL LETTER P#
    {120058, "q"},   // MA# ( 𝓺 → q ) MATHEMATICAL BOLD SCRIPT SMALL Q → LATIN SMALL LETTER Q#
    {120059, "r"},   // MA# ( 𝓻 → r ) MATHEMATICAL BOLD SCRIPT SMALL R → LATIN SMALL LETTER R#
    {120060, "s"},   // MA# ( 𝓼 → s ) MATHEMATICAL BOLD SCRIPT SMALL S → LATIN SMALL LETTER S#
    {120061, "t"},   // MA# ( 𝓽 → t ) MATHEMATICAL BOLD SCRIPT SMALL T → LATIN SMALL LETTER T#
    {120062, "u"},   // MA# ( 𝓾 → u ) MATHEMATICAL BOLD SCRIPT SMALL U → LATIN SMALL LETTER U#
    {120063, "v"},   // MA# ( 𝓿 → v ) MATHEMATICAL BOLD SCRIPT SMALL V → LATIN SMALL LETTER V#
    {120064, "w"},   // MA# ( 𝔀 → w ) MATHEMATICAL BOLD SCRIPT SMALL W → LATIN SMALL LETTER W#
    {120065, "x"},   // MA# ( 𝔁 → x ) MATHEMATICAL BOLD SCRIPT SMALL X → LATIN SMALL LETTER X#
    {120066, "y"},   // MA# ( 𝔂 → y ) MATHEMATICAL BOLD SCRIPT SMALL Y → LATIN SMALL LETTER Y#
    {120067, "z"},   // MA# ( 𝔃 → z ) MATHEMATICAL BOLD SCRIPT SMALL Z → LATIN SMALL LETTER Z#
    {120068, "A"},   // MA# ( 𝔄 → A ) MATHEMATICAL FRAKTUR CAPITAL A → LATIN CAPITAL LETTER A#
    {120069, "B"},   // MA# ( 𝔅 → B ) MATHEMATICAL FRAKTUR CAPITAL B → LATIN CAPITAL LETTER B#
    {120071, "D"},   // MA# ( 𝔇 → D ) MATHEMATICAL FRAKTUR CAPITAL D → LATIN CAPITAL LETTER D#
    {120072, "E"},   // MA# ( 𝔈 → E ) MATHEMATICAL FRAKTUR CAPITAL E → LATIN CAPITAL LETTER E#
    {120073, "F"},   // MA# ( 𝔉 → F ) MATHEMATICAL FRAKTUR CAPITAL F → LATIN CAPITAL LETTER F#
    {120074, "G"},   // MA# ( 𝔊 → G ) MATHEMATICAL FRAKTUR CAPITAL G → LATIN CAPITAL LETTER G#
    {120077, "J"},   // MA# ( 𝔍 → J ) MATHEMATICAL FRAKTUR CAPITAL J → LATIN CAPITAL LETTER J#
    {120078, "K"},   // MA# ( 𝔎 → K ) MATHEMATICAL FRAKTUR CAPITAL K → LATIN CAPITAL LETTER K#
    {120079, "L"},   // MA# ( 𝔏 → L ) MATHEMATICAL FRAKTUR CAPITAL L → LATIN CAPITAL LETTER L#
    {120080, "M"},   // MA# ( 𝔐 → M ) MATHEMATICAL FRAKTUR CAPITAL M → LATIN CAPITAL LETTER M#
    {120081, "N"},   // MA# ( 𝔑 → N ) MATHEMATICAL FRAKTUR CAPITAL N → LATIN CAPITAL LETTER N#
    {120082, "O"},   // MA# ( 𝔒 → O ) MATHEMATICAL FRAKTUR CAPITAL O → LATIN CAPITAL LETTER O#
    {120083, "P"},   // MA# ( 𝔓 → P ) MATHEMATICAL FRAKTUR CAPITAL P → LATIN CAPITAL LETTER P#
    {120084, "Q"},   // MA# ( 𝔔 → Q ) MATHEMATICAL FRAKTUR CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120086, "S"},   // MA# ( 𝔖 → S ) MATHEMATICAL FRAKTUR CAPITAL S → LATIN CAPITAL LETTER S#
    {120087, "T"},   // MA# ( 𝔗 → T ) MATHEMATICAL FRAKTUR CAPITAL T → LATIN CAPITAL LETTER T#
    {120088, "U"},   // MA# ( 𝔘 → U ) MATHEMATICAL FRAKTUR CAPITAL U → LATIN CAPITAL LETTER U#
    {120089, "V"},   // MA# ( 𝔙 → V ) MATHEMATICAL FRAKTUR CAPITAL V → LATIN CAPITAL LETTER V#
    {120090, "W"},   // MA# ( 𝔚 → W ) MATHEMATICAL FRAKTUR CAPITAL W → LATIN CAPITAL LETTER W#
    {120091, "X"},   // MA# ( 𝔛 → X ) MATHEMATICAL FRAKTUR CAPITAL X → LATIN CAPITAL LETTER X#
    {120092, "Y"},   // MA# ( 𝔜 → Y ) MATHEMATICAL FRAKTUR CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120094, "a"},   // MA# ( 𝔞 → a ) MATHEMATICAL FRAKTUR SMALL A → LATIN SMALL LETTER A#
    {120095, "b"},   // MA# ( 𝔟 → b ) MATHEMATICAL FRAKTUR SMALL B → LATIN SMALL LETTER B#
    {120096, "c"},   // MA# ( 𝔠 → c ) MATHEMATICAL FRAKTUR SMALL C → LATIN SMALL LETTER C#
    {120097, "d"},   // MA# ( 𝔡 → d ) MATHEMATICAL FRAKTUR SMALL D → LATIN SMALL LETTER D#
    {120098, "e"},   // MA# ( 𝔢 → e ) MATHEMATICAL FRAKTUR SMALL E → LATIN SMALL LETTER E#
    {120099, "f"},   // MA# ( 𝔣 → f ) MATHEMATICAL FRAKTUR SMALL F → LATIN SMALL LETTER F#
    {120100, "g"},   // MA# ( 𝔤 → g ) MATHEMATICAL FRAKTUR SMALL G → LATIN SMALL LETTER G#
    {120101, "h"},   // MA# ( 𝔥 → h ) MATHEMATICAL FRAKTUR SMALL H → LATIN SMALL LETTER H#
    {120102, "i"},   // MA# ( 𝔦 → i ) MATHEMATICAL FRAKTUR SMALL I → LATIN SMALL LETTER I#
    {120103, "j"},   // MA# ( 𝔧 → j ) MATHEMATICAL FRAKTUR SMALL J → LATIN SMALL LETTER J#
    {120104, "k"},   // MA# ( 𝔨 → k ) MATHEMATICAL FRAKTUR SMALL K → LATIN SMALL LETTER K#
    {120105, "l"},   // MA# ( 𝔩 → l ) MATHEMATICAL FRAKTUR SMALL L → LATIN SMALL LETTER L#
    {120106, "rn"},  // MA# ( 𝔪 → rn ) MATHEMATICAL FRAKTUR SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120107, "n"},   // MA# ( 𝔫 → n ) MATHEMATICAL FRAKTUR SMALL N → LATIN SMALL LETTER N#
    {120108, "o"},   // MA# ( 𝔬 → o ) MATHEMATICAL FRAKTUR SMALL O → LATIN SMALL LETTER O#
    {120109, "p"},   // MA# ( 𝔭 → p ) MATHEMATICAL FRAKTUR SMALL P → LATIN SMALL LETTER P#
    {120110, "q"},   // MA# ( 𝔮 → q ) MATHEMATICAL FRAKTUR SMALL Q → LATIN SMALL LETTER Q#
    {120111, "r"},   // MA# ( 𝔯 → r ) MATHEMATICAL FRAKTUR SMALL R → LATIN SMALL LETTER R#
    {120112, "s"},   // MA# ( 𝔰 → s ) MATHEMATICAL FRAKTUR SMALL S → LATIN SMALL LETTER S#
    {120113, "t"},   // MA# ( 𝔱 → t ) MATHEMATICAL FRAKTUR SMALL T → LATIN SMALL LETTER T#
    {120114, "u"},   // MA# ( 𝔲 → u ) MATHEMATICAL FRAKTUR SMALL U → LATIN SMALL LETTER U#
    {120115, "v"},   // MA# ( 𝔳 → v ) MATHEMATICAL FRAKTUR SMALL V → LATIN SMALL LETTER V#
    {120116, "w"},   // MA# ( 𝔴 → w ) MATHEMATICAL FRAKTUR SMALL W → LATIN SMALL LETTER W#
    {120117, "x"},   // MA# ( 𝔵 → x ) MATHEMATICAL FRAKTUR SMALL X → LATIN SMALL LETTER X#
    {120118, "y"},   // MA# ( 𝔶 → y ) MATHEMATICAL FRAKTUR SMALL Y → LATIN SMALL LETTER Y#
    {120119, "z"},   // MA# ( 𝔷 → z ) MATHEMATICAL FRAKTUR SMALL Z → LATIN SMALL LETTER Z#
    {120120, "A"},   // MA# ( 𝔸 → A ) MATHEMATICAL DOUBLE-STRUCK CAPITAL A → LATIN CAPITAL LETTER A#
    {120121, "B"},   // MA# ( 𝔹 → B ) MATHEMATICAL DOUBLE-STRUCK CAPITAL B → LATIN CAPITAL LETTER B#
    {120123, "D"},   // MA# ( 𝔻 → D ) MATHEMATICAL DOUBLE-STRUCK CAPITAL D → LATIN CAPITAL LETTER D#
    {120124, "E"},   // MA# ( 𝔼 → E ) MATHEMATICAL DOUBLE-STRUCK CAPITAL E → LATIN CAPITAL LETTER E#
    {120125, "F"},   // MA# ( 𝔽 → F ) MATHEMATICAL DOUBLE-STRUCK CAPITAL F → LATIN CAPITAL LETTER F#
    {120126, "G"},   // MA# ( 𝔾 → G ) MATHEMATICAL DOUBLE-STRUCK CAPITAL G → LATIN CAPITAL LETTER G#
    {120128, "l"},   // MA# ( 𝕀 → l ) MATHEMATICAL DOUBLE-STRUCK CAPITAL I → LATIN SMALL LETTER L# →I→
    {120129, "J"},   // MA# ( 𝕁 → J ) MATHEMATICAL DOUBLE-STRUCK CAPITAL J → LATIN CAPITAL LETTER J#
    {120130, "K"},   // MA# ( 𝕂 → K ) MATHEMATICAL DOUBLE-STRUCK CAPITAL K → LATIN CAPITAL LETTER K#
    {120131, "L"},   // MA# ( 𝕃 → L ) MATHEMATICAL DOUBLE-STRUCK CAPITAL L → LATIN CAPITAL LETTER L#
    {120132, "M"},   // MA# ( 𝕄 → M ) MATHEMATICAL DOUBLE-STRUCK CAPITAL M → LATIN CAPITAL LETTER M#
    {120134, "O"},   // MA# ( 𝕆 → O ) MATHEMATICAL DOUBLE-STRUCK CAPITAL O → LATIN CAPITAL LETTER O#
    {120138, "S"},   // MA# ( 𝕊 → S ) MATHEMATICAL DOUBLE-STRUCK CAPITAL S → LATIN CAPITAL LETTER S#
    {120139, "T"},   // MA# ( 𝕋 → T ) MATHEMATICAL DOUBLE-STRUCK CAPITAL T → LATIN CAPITAL LETTER T#
    {120140, "U"},   // MA# ( 𝕌 → U ) MATHEMATICAL DOUBLE-STRUCK CAPITAL U → LATIN CAPITAL LETTER U#
    {120141, "V"},   // MA# ( 𝕍 → V ) MATHEMATICAL DOUBLE-STRUCK CAPITAL V → LATIN CAPITAL LETTER V#
    {120142, "W"},   // MA# ( 𝕎 → W ) MATHEMATICAL DOUBLE-STRUCK CAPITAL W → LATIN CAPITAL LETTER W#
    {120143, "X"},   // MA# ( 𝕏 → X ) MATHEMATICAL DOUBLE-STRUCK CAPITAL X → LATIN CAPITAL LETTER X#
    {120144, "Y"},   // MA# ( 𝕐 → Y ) MATHEMATICAL DOUBLE-STRUCK CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120146, "a"},   // MA# ( 𝕒 → a ) MATHEMATICAL DOUBLE-STRUCK SMALL A → LATIN SMALL LETTER A#
    {120147, "b"},   // MA# ( 𝕓 → b ) MATHEMATICAL DOUBLE-STRUCK SMALL B → LATIN SMALL LETTER B#
    {120148, "c"},   // MA# ( 𝕔 → c ) MATHEMATICAL DOUBLE-STRUCK SMALL C → LATIN SMALL LETTER C#
    {120149, "d"},   // MA# ( 𝕕 → d ) MATHEMATICAL DOUBLE-STRUCK SMALL D → LATIN SMALL LETTER D#
    {120150, "e"},   // MA# ( 𝕖 → e ) MATHEMATICAL DOUBLE-STRUCK SMALL E → LATIN SMALL LETTER E#
    {120151, "f"},   // MA# ( 𝕗 → f ) MATHEMATICAL DOUBLE-STRUCK SMALL F → LATIN SMALL LETTER F#
    {120152, "g"},   // MA# ( 𝕘 → g ) MATHEMATICAL DOUBLE-STRUCK SMALL G → LATIN SMALL LETTER G#
    {120153, "h"},   // MA# ( 𝕙 → h ) MATHEMATICAL DOUBLE-STRUCK SMALL H → LATIN SMALL LETTER H#
    {120154, "i"},   // MA# ( 𝕚 → i ) MATHEMATICAL DOUBLE-STRUCK SMALL I → LATIN SMALL LETTER I#
    {120155, "j"},   // MA# ( 𝕛 → j ) MATHEMATICAL DOUBLE-STRUCK SMALL J → LATIN SMALL LETTER J#
    {120156, "k"},   // MA# ( 𝕜 → k ) MATHEMATICAL DOUBLE-STRUCK SMALL K → LATIN SMALL LETTER K#
    {120157, "l"},   // MA# ( 𝕝 → l ) MATHEMATICAL DOUBLE-STRUCK SMALL L → LATIN SMALL LETTER L#
    {120158, "rn"},  // MA# ( 𝕞 → rn ) MATHEMATICAL DOUBLE-STRUCK SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120159, "n"},   // MA# ( 𝕟 → n ) MATHEMATICAL DOUBLE-STRUCK SMALL N → LATIN SMALL LETTER N#
    {120160, "o"},   // MA# ( 𝕠 → o ) MATHEMATICAL DOUBLE-STRUCK SMALL O → LATIN SMALL LETTER O#
    {120161, "p"},   // MA# ( 𝕡 → p ) MATHEMATICAL DOUBLE-STRUCK SMALL P → LATIN SMALL LETTER P#
    {120162, "q"},   // MA# ( 𝕢 → q ) MATHEMATICAL DOUBLE-STRUCK SMALL Q → LATIN SMALL LETTER Q#
    {120163, "r"},   // MA# ( 𝕣 → r ) MATHEMATICAL DOUBLE-STRUCK SMALL R → LATIN SMALL LETTER R#
    {120164, "s"},   // MA# ( 𝕤 → s ) MATHEMATICAL DOUBLE-STRUCK SMALL S → LATIN SMALL LETTER S#
    {120165, "t"},   // MA# ( 𝕥 → t ) MATHEMATICAL DOUBLE-STRUCK SMALL T → LATIN SMALL LETTER T#
    {120166, "u"},   // MA# ( 𝕦 → u ) MATHEMATICAL DOUBLE-STRUCK SMALL U → LATIN SMALL LETTER U#
    {120167, "v"},   // MA# ( 𝕧 → v ) MATHEMATICAL DOUBLE-STRUCK SMALL V → LATIN SMALL LETTER V#
    {120168, "w"},   // MA# ( 𝕨 → w ) MATHEMATICAL DOUBLE-STRUCK SMALL W → LATIN SMALL LETTER W#
    {120169, "x"},   // MA# ( 𝕩 → x ) MATHEMATICAL DOUBLE-STRUCK SMALL X → LATIN SMALL LETTER X#
    {120170, "y"},   // MA# ( 𝕪 → y ) MATHEMATICAL DOUBLE-STRUCK SMALL Y → LATIN SMALL LETTER Y#
    {120171, "z"},   // MA# ( 𝕫 → z ) MATHEMATICAL DOUBLE-STRUCK SMALL Z → LATIN SMALL LETTER Z#
    {120172, "A"},   // MA# ( 𝕬 → A ) MATHEMATICAL BOLD FRAKTUR CAPITAL A → LATIN CAPITAL LETTER A#
    {120173, "B"},   // MA# ( 𝕭 → B ) MATHEMATICAL BOLD FRAKTUR CAPITAL B → LATIN CAPITAL LETTER B#
    {120174, "C"},   // MA# ( 𝕮 → C ) MATHEMATICAL BOLD FRAKTUR CAPITAL C → LATIN CAPITAL LETTER C#
    {120175, "D"},   // MA# ( 𝕯 → D ) MATHEMATICAL BOLD FRAKTUR CAPITAL D → LATIN CAPITAL LETTER D#
    {120176, "E"},   // MA# ( 𝕰 → E ) MATHEMATICAL BOLD FRAKTUR CAPITAL E → LATIN CAPITAL LETTER E#
    {120177, "F"},   // MA# ( 𝕱 → F ) MATHEMATICAL BOLD FRAKTUR CAPITAL F → LATIN CAPITAL LETTER F#
    {120178, "G"},   // MA# ( 𝕲 → G ) MATHEMATICAL BOLD FRAKTUR CAPITAL G → LATIN CAPITAL LETTER G#
    {120179, "H"},   // MA# ( 𝕳 → H ) MATHEMATICAL BOLD FRAKTUR CAPITAL H → LATIN CAPITAL LETTER H#
    {120180, "l"},   // MA# ( 𝕴 → l ) MATHEMATICAL BOLD FRAKTUR CAPITAL I → LATIN SMALL LETTER L# →I→
    {120181, "J"},   // MA# ( 𝕵 → J ) MATHEMATICAL BOLD FRAKTUR CAPITAL J → LATIN CAPITAL LETTER J#
    {120182, "K"},   // MA# ( 𝕶 → K ) MATHEMATICAL BOLD FRAKTUR CAPITAL K → LATIN CAPITAL LETTER K#
    {120183, "L"},   // MA# ( 𝕷 → L ) MATHEMATICAL BOLD FRAKTUR CAPITAL L → LATIN CAPITAL LETTER L#
    {120184, "M"},   // MA# ( 𝕸 → M ) MATHEMATICAL BOLD FRAKTUR CAPITAL M → LATIN CAPITAL LETTER M#
    {120185, "N"},   // MA# ( 𝕹 → N ) MATHEMATICAL BOLD FRAKTUR CAPITAL N → LATIN CAPITAL LETTER N#
    {120186, "O"},   // MA# ( 𝕺 → O ) MATHEMATICAL BOLD FRAKTUR CAPITAL O → LATIN CAPITAL LETTER O#
    {120187, "P"},   // MA# ( 𝕻 → P ) MATHEMATICAL BOLD FRAKTUR CAPITAL P → LATIN CAPITAL LETTER P#
    {120188, "Q"},   // MA# ( 𝕼 → Q ) MATHEMATICAL BOLD FRAKTUR CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120189, "R"},   // MA# ( 𝕽 → R ) MATHEMATICAL BOLD FRAKTUR CAPITAL R → LATIN CAPITAL LETTER R#
    {120190, "S"},   // MA# ( 𝕾 → S ) MATHEMATICAL BOLD FRAKTUR CAPITAL S → LATIN CAPITAL LETTER S#
    {120191, "T"},   // MA# ( 𝕿 → T ) MATHEMATICAL BOLD FRAKTUR CAPITAL T → LATIN CAPITAL LETTER T#
    {120192, "U"},   // MA# ( 𝖀 → U ) MATHEMATICAL BOLD FRAKTUR CAPITAL U → LATIN CAPITAL LETTER U#
    {120193, "V"},   // MA# ( 𝖁 → V ) MATHEMATICAL BOLD FRAKTUR CAPITAL V → LATIN CAPITAL LETTER V#
    {120194, "W"},   // MA# ( 𝖂 → W ) MATHEMATICAL BOLD FRAKTUR CAPITAL W → LATIN CAPITAL LETTER W#
    {120195, "X"},   // MA# ( 𝖃 → X ) MATHEMATICAL BOLD FRAKTUR CAPITAL X → LATIN CAPITAL LETTER X#
    {120196, "Y"},   // MA# ( 𝖄 → Y ) MATHEMATICAL BOLD FRAKTUR CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120197, "Z"},   // MA# ( 𝖅 → Z ) MATHEMATICAL BOLD FRAKTUR CAPITAL Z → LATIN CAPITAL LETTER Z#
    {120198, "a"},   // MA# ( 𝖆 → a ) MATHEMATICAL BOLD FRAKTUR SMALL A → LATIN SMALL LETTER A#
    {120199, "b"},   // MA# ( 𝖇 → b ) MATHEMATICAL BOLD FRAKTUR SMALL B → LATIN SMALL LETTER B#
    {120200, "c"},   // MA# ( 𝖈 → c ) MATHEMATICAL BOLD FRAKTUR SMALL C → LATIN SMALL LETTER C#
    {120201, "d"},   // MA# ( 𝖉 → d ) MATHEMATICAL BOLD FRAKTUR SMALL D → LATIN SMALL LETTER D#
    {120202, "e"},   // MA# ( 𝖊 → e ) MATHEMATICAL BOLD FRAKTUR SMALL E → LATIN SMALL LETTER E#
    {120203, "f"},   // MA# ( 𝖋 → f ) MATHEMATICAL BOLD FRAKTUR SMALL F → LATIN SMALL LETTER F#
    {120204, "g"},   // MA# ( 𝖌 → g ) MATHEMATICAL BOLD FRAKTUR SMALL G → LATIN SMALL LETTER G#
    {120205, "h"},   // MA# ( 𝖍 → h ) MATHEMATICAL BOLD FRAKTUR SMALL H → LATIN SMALL LETTER H#
    {120206, "i"},   // MA# ( 𝖎 → i ) MATHEMATICAL BOLD FRAKTUR SMALL I → LATIN SMALL LETTER I#
    {120207, "j"},   // MA# ( 𝖏 → j ) MATHEMATICAL BOLD FRAKTUR SMALL J → LATIN SMALL LETTER J#
    {120208, "k"},   // MA# ( 𝖐 → k ) MATHEMATICAL BOLD FRAKTUR SMALL K → LATIN SMALL LETTER K#
    {120209, "l"},   // MA# ( 𝖑 → l ) MATHEMATICAL BOLD FRAKTUR SMALL L → LATIN SMALL LETTER L#
    {120210, "rn"},  // MA# ( 𝖒 → rn ) MATHEMATICAL BOLD FRAKTUR SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120211, "n"},   // MA# ( 𝖓 → n ) MATHEMATICAL BOLD FRAKTUR SMALL N → LATIN SMALL LETTER N#
    {120212, "o"},   // MA# ( 𝖔 → o ) MATHEMATICAL BOLD FRAKTUR SMALL O → LATIN SMALL LETTER O#
    {120213, "p"},   // MA# ( 𝖕 → p ) MATHEMATICAL BOLD FRAKTUR SMALL P → LATIN SMALL LETTER P#
    {120214, "q"},   // MA# ( 𝖖 → q ) MATHEMATICAL BOLD FRAKTUR SMALL Q → LATIN SMALL LETTER Q#
    {120215, "r"},   // MA# ( 𝖗 → r ) MATHEMATICAL BOLD FRAKTUR SMALL R → LATIN SMALL LETTER R#
    {120216, "s"},   // MA# ( 𝖘 → s ) MATHEMATICAL BOLD FRAKTUR SMALL S → LATIN SMALL LETTER S#
    {120217, "t"},   // MA# ( 𝖙 → t ) MATHEMATICAL BOLD FRAKTUR SMALL T → LATIN SMALL LETTER T#
    {120218, "u"},   // MA# ( 𝖚 → u ) MATHEMATICAL BOLD FRAKTUR SMALL U → LATIN SMALL LETTER U#
    {120219, "v"},   // MA# ( 𝖛 → v ) MATHEMATICAL BOLD FRAKTUR SMALL V → LATIN SMALL LETTER V#
    {120220, "w"},   // MA# ( 𝖜 → w ) MATHEMATICAL BOLD FRAKTUR SMALL W → LATIN SMALL LETTER W#
    {120221, "x"},   // MA# ( 𝖝 → x ) MATHEMATICAL BOLD FRAKTUR SMALL X → LATIN SMALL LETTER X#
    {120222, "y"},   // MA# ( 𝖞 → y ) MATHEMATICAL BOLD FRAKTUR SMALL Y → LATIN SMALL LETTER Y#
    {120223, "z"},   // MA# ( 𝖟 → z ) MATHEMATICAL BOLD FRAKTUR SMALL Z → LATIN SMALL LETTER Z#
    {120224, "A"},   // MA# ( 𝖠 → A ) MATHEMATICAL SANS-SERIF CAPITAL A → LATIN CAPITAL LETTER A#
    {120225, "B"},   // MA# ( 𝖡 → B ) MATHEMATICAL SANS-SERIF CAPITAL B → LATIN CAPITAL LETTER B#
    {120226, "C"},   // MA# ( 𝖢 → C ) MATHEMATICAL SANS-SERIF CAPITAL C → LATIN CAPITAL LETTER C#
    {120227, "D"},   // MA# ( 𝖣 → D ) MATHEMATICAL SANS-SERIF CAPITAL D → LATIN CAPITAL LETTER D#
    {120228, "E"},   // MA# ( 𝖤 → E ) MATHEMATICAL SANS-SERIF CAPITAL E → LATIN CAPITAL LETTER E#
    {120229, "F"},   // MA# ( 𝖥 → F ) MATHEMATICAL SANS-SERIF CAPITAL F → LATIN CAPITAL LETTER F#
    {120230, "G"},   // MA# ( 𝖦 → G ) MATHEMATICAL SANS-SERIF CAPITAL G → LATIN CAPITAL LETTER G#
    {120231, "H"},   // MA# ( 𝖧 → H ) MATHEMATICAL SANS-SERIF CAPITAL H → LATIN CAPITAL LETTER H#
    {120232, "l"},   // MA# ( 𝖨 → l ) MATHEMATICAL SANS-SERIF CAPITAL I → LATIN SMALL LETTER L# →I→
    {120233, "J"},   // MA# ( 𝖩 → J ) MATHEMATICAL SANS-SERIF CAPITAL J → LATIN CAPITAL LETTER J#
    {120234, "K"},   // MA# ( 𝖪 → K ) MATHEMATICAL SANS-SERIF CAPITAL K → LATIN CAPITAL LETTER K#
    {120235, "L"},   // MA# ( 𝖫 → L ) MATHEMATICAL SANS-SERIF CAPITAL L → LATIN CAPITAL LETTER L#
    {120236, "M"},   // MA# ( 𝖬 → M ) MATHEMATICAL SANS-SERIF CAPITAL M → LATIN CAPITAL LETTER M#
    {120237, "N"},   // MA# ( 𝖭 → N ) MATHEMATICAL SANS-SERIF CAPITAL N → LATIN CAPITAL LETTER N#
    {120238, "O"},   // MA# ( 𝖮 → O ) MATHEMATICAL SANS-SERIF CAPITAL O → LATIN CAPITAL LETTER O#
    {120239, "P"},   // MA# ( 𝖯 → P ) MATHEMATICAL SANS-SERIF CAPITAL P → LATIN CAPITAL LETTER P#
    {120240, "Q"},   // MA# ( 𝖰 → Q ) MATHEMATICAL SANS-SERIF CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120241, "R"},   // MA# ( 𝖱 → R ) MATHEMATICAL SANS-SERIF CAPITAL R → LATIN CAPITAL LETTER R#
    {120242, "S"},   // MA# ( 𝖲 → S ) MATHEMATICAL SANS-SERIF CAPITAL S → LATIN CAPITAL LETTER S#
    {120243, "T"},   // MA# ( 𝖳 → T ) MATHEMATICAL SANS-SERIF CAPITAL T → LATIN CAPITAL LETTER T#
    {120244, "U"},   // MA# ( 𝖴 → U ) MATHEMATICAL SANS-SERIF CAPITAL U → LATIN CAPITAL LETTER U#
    {120245, "V"},   // MA# ( 𝖵 → V ) MATHEMATICAL SANS-SERIF CAPITAL V → LATIN CAPITAL LETTER V#
    {120246, "W"},   // MA# ( 𝖶 → W ) MATHEMATICAL SANS-SERIF CAPITAL W → LATIN CAPITAL LETTER W#
    {120247, "X"},   // MA# ( 𝖷 → X ) MATHEMATICAL SANS-SERIF CAPITAL X → LATIN CAPITAL LETTER X#
    {120248, "Y"},   // MA# ( 𝖸 → Y ) MATHEMATICAL SANS-SERIF CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120249, "Z"},   // MA# ( 𝖹 → Z ) MATHEMATICAL SANS-SERIF CAPITAL Z → LATIN CAPITAL LETTER Z#
    {120250, "a"},   // MA# ( 𝖺 → a ) MATHEMATICAL SANS-SERIF SMALL A → LATIN SMALL LETTER A#
    {120251, "b"},   // MA# ( 𝖻 → b ) MATHEMATICAL SANS-SERIF SMALL B → LATIN SMALL LETTER B#
    {120252, "c"},   // MA# ( 𝖼 → c ) MATHEMATICAL SANS-SERIF SMALL C → LATIN SMALL LETTER C#
    {120253, "d"},   // MA# ( 𝖽 → d ) MATHEMATICAL SANS-SERIF SMALL D → LATIN SMALL LETTER D#
    {120254, "e"},   // MA# ( 𝖾 → e ) MATHEMATICAL SANS-SERIF SMALL E → LATIN SMALL LETTER E#
    {120255, "f"},   // MA# ( 𝖿 → f ) MATHEMATICAL SANS-SERIF SMALL F → LATIN SMALL LETTER F#
    {120256, "g"},   // MA# ( 𝗀 → g ) MATHEMATICAL SANS-SERIF SMALL G → LATIN SMALL LETTER G#
    {120257, "h"},   // MA# ( 𝗁 → h ) MATHEMATICAL SANS-SERIF SMALL H → LATIN SMALL LETTER H#
    {120258, "i"},   // MA# ( 𝗂 → i ) MATHEMATICAL SANS-SERIF SMALL I → LATIN SMALL LETTER I#
    {120259, "j"},   // MA# ( 𝗃 → j ) MATHEMATICAL SANS-SERIF SMALL J → LATIN SMALL LETTER J#
    {120260, "k"},   // MA# ( 𝗄 → k ) MATHEMATICAL SANS-SERIF SMALL K → LATIN SMALL LETTER K#
    {120261, "l"},   // MA# ( 𝗅 → l ) MATHEMATICAL SANS-SERIF SMALL L → LATIN SMALL LETTER L#
    {120262, "rn"},  // MA# ( 𝗆 → rn ) MATHEMATICAL SANS-SERIF SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120263, "n"},   // MA# ( 𝗇 → n ) MATHEMATICAL SANS-SERIF SMALL N → LATIN SMALL LETTER N#
    {120264, "o"},   // MA# ( 𝗈 → o ) MATHEMATICAL SANS-SERIF SMALL O → LATIN SMALL LETTER O#
    {120265, "p"},   // MA# ( 𝗉 → p ) MATHEMATICAL SANS-SERIF SMALL P → LATIN SMALL LETTER P#
    {120266, "q"},   // MA# ( 𝗊 → q ) MATHEMATICAL SANS-SERIF SMALL Q → LATIN SMALL LETTER Q#
    {120267, "r"},   // MA# ( 𝗋 → r ) MATHEMATICAL SANS-SERIF SMALL R → LATIN SMALL LETTER R#
    {120268, "s"},   // MA# ( 𝗌 → s ) MATHEMATICAL SANS-SERIF SMALL S → LATIN SMALL LETTER S#
    {120269, "t"},   // MA# ( 𝗍 → t ) MATHEMATICAL SANS-SERIF SMALL T → LATIN SMALL LETTER T#
    {120270, "u"},   // MA# ( 𝗎 → u ) MATHEMATICAL SANS-SERIF SMALL U → LATIN SMALL LETTER U#
    {120271, "v"},   // MA# ( 𝗏 → v ) MATHEMATICAL SANS-SERIF SMALL V → LATIN SMALL LETTER V#
    {120272, "w"},   // MA# ( 𝗐 → w ) MATHEMATICAL SANS-SERIF SMALL W → LATIN SMALL LETTER W#
    {120273, "x"},   // MA# ( 𝗑 → x ) MATHEMATICAL SANS-SERIF SMALL X → LATIN SMALL LETTER X#
    {120274, "y"},   // MA# ( 𝗒 → y ) MATHEMATICAL SANS-SERIF SMALL Y → LATIN SMALL LETTER Y#
    {120275, "z"},   // MA# ( 𝗓 → z ) MATHEMATICAL SANS-SERIF SMALL Z → LATIN SMALL LETTER Z#
    {120276, "A"},   // MA# ( 𝗔 → A ) MATHEMATICAL SANS-SERIF BOLD CAPITAL A → LATIN CAPITAL LETTER A#
    {120277, "B"},   // MA# ( 𝗕 → B ) MATHEMATICAL SANS-SERIF BOLD CAPITAL B → LATIN CAPITAL LETTER B#
    {120278, "C"},   // MA# ( 𝗖 → C ) MATHEMATICAL SANS-SERIF BOLD CAPITAL C → LATIN CAPITAL LETTER C#
    {120279, "D"},   // MA# ( 𝗗 → D ) MATHEMATICAL SANS-SERIF BOLD CAPITAL D → LATIN CAPITAL LETTER D#
    {120280, "E"},   // MA# ( 𝗘 → E ) MATHEMATICAL SANS-SERIF BOLD CAPITAL E → LATIN CAPITAL LETTER E#
    {120281, "F"},   // MA# ( 𝗙 → F ) MATHEMATICAL SANS-SERIF BOLD CAPITAL F → LATIN CAPITAL LETTER F#
    {120282, "G"},   // MA# ( 𝗚 → G ) MATHEMATICAL SANS-SERIF BOLD CAPITAL G → LATIN CAPITAL LETTER G#
    {120283, "H"},   // MA# ( 𝗛 → H ) MATHEMATICAL SANS-SERIF BOLD CAPITAL H → LATIN CAPITAL LETTER H#
    {120284, "l"},   // MA# ( 𝗜 → l ) MATHEMATICAL SANS-SERIF BOLD CAPITAL I → LATIN SMALL LETTER L# →I→
    {120285, "J"},   // MA# ( 𝗝 → J ) MATHEMATICAL SANS-SERIF BOLD CAPITAL J → LATIN CAPITAL LETTER J#
    {120286, "K"},   // MA# ( 𝗞 → K ) MATHEMATICAL SANS-SERIF BOLD CAPITAL K → LATIN CAPITAL LETTER K#
    {120287, "L"},   // MA# ( 𝗟 → L ) MATHEMATICAL SANS-SERIF BOLD CAPITAL L → LATIN CAPITAL LETTER L#
    {120288, "M"},   // MA# ( 𝗠 → M ) MATHEMATICAL SANS-SERIF BOLD CAPITAL M → LATIN CAPITAL LETTER M#
    {120289, "N"},   // MA# ( 𝗡 → N ) MATHEMATICAL SANS-SERIF BOLD CAPITAL N → LATIN CAPITAL LETTER N#
    {120290, "O"},   // MA# ( 𝗢 → O ) MATHEMATICAL SANS-SERIF BOLD CAPITAL O → LATIN CAPITAL LETTER O#
    {120291, "P"},   // MA# ( 𝗣 → P ) MATHEMATICAL SANS-SERIF BOLD CAPITAL P → LATIN CAPITAL LETTER P#
    {120292, "Q"},   // MA# ( 𝗤 → Q ) MATHEMATICAL SANS-SERIF BOLD CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120293, "R"},   // MA# ( 𝗥 → R ) MATHEMATICAL SANS-SERIF BOLD CAPITAL R → LATIN CAPITAL LETTER R#
    {120294, "S"},   // MA# ( 𝗦 → S ) MATHEMATICAL SANS-SERIF BOLD CAPITAL S → LATIN CAPITAL LETTER S#
    {120295, "T"},   // MA# ( 𝗧 → T ) MATHEMATICAL SANS-SERIF BOLD CAPITAL T → LATIN CAPITAL LETTER T#
    {120296, "U"},   // MA# ( 𝗨 → U ) MATHEMATICAL SANS-SERIF BOLD CAPITAL U → LATIN CAPITAL LETTER U#
    {120297, "V"},   // MA# ( 𝗩 → V ) MATHEMATICAL SANS-SERIF BOLD CAPITAL V → LATIN CAPITAL LETTER V#
    {120298, "W"},   // MA# ( 𝗪 → W ) MATHEMATICAL SANS-SERIF BOLD CAPITAL W → LATIN CAPITAL LETTER W#
    {120299, "X"},   // MA# ( 𝗫 → X ) MATHEMATICAL SANS-SERIF BOLD CAPITAL X → LATIN CAPITAL LETTER X#
    {120300, "Y"},   // MA# ( 𝗬 → Y ) MATHEMATICAL SANS-SERIF BOLD CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120301, "Z"},   // MA# ( 𝗭 → Z ) MATHEMATICAL SANS-SERIF BOLD CAPITAL Z → LATIN CAPITAL LETTER Z#
    {120302, "a"},   // MA# ( 𝗮 → a ) MATHEMATICAL SANS-SERIF BOLD SMALL A → LATIN SMALL LETTER A#
    {120303, "b"},   // MA# ( 𝗯 → b ) MATHEMATICAL SANS-SERIF BOLD SMALL B → LATIN SMALL LETTER B#
    {120304, "c"},   // MA# ( 𝗰 → c ) MATHEMATICAL SANS-SERIF BOLD SMALL C → LATIN SMALL LETTER C#
    {120305, "d"},   // MA# ( 𝗱 → d ) MATHEMATICAL SANS-SERIF BOLD SMALL D → LATIN SMALL LETTER D#
    {120306, "e"},   // MA# ( 𝗲 → e ) MATHEMATICAL SANS-SERIF BOLD SMALL E → LATIN SMALL LETTER E#
    {120307, "f"},   // MA# ( 𝗳 → f ) MATHEMATICAL SANS-SERIF BOLD SMALL F → LATIN SMALL LETTER F#
    {120308, "g"},   // MA# ( 𝗴 → g ) MATHEMATICAL SANS-SERIF BOLD SMALL G → LATIN SMALL LETTER G#
    {120309, "h"},   // MA# ( 𝗵 → h ) MATHEMATICAL SANS-SERIF BOLD SMALL H → LATIN SMALL LETTER H#
    {120310, "i"},   // MA# ( 𝗶 → i ) MATHEMATICAL SANS-SERIF BOLD SMALL I → LATIN SMALL LETTER I#
    {120311, "j"},   // MA# ( 𝗷 → j ) MATHEMATICAL SANS-SERIF BOLD SMALL J → LATIN SMALL LETTER J#
    {120312, "k"},   // MA# ( 𝗸 → k ) MATHEMATICAL SANS-SERIF BOLD SMALL K → LATIN SMALL LETTER K#
    {120313, "l"},   // MA# ( 𝗹 → l ) MATHEMATICAL SANS-SERIF BOLD SMALL L → LATIN SMALL LETTER L#
    {120314, "rn"},  // MA# ( 𝗺 → rn ) MATHEMATICAL SANS-SERIF BOLD SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120315, "n"},   // MA# ( 𝗻 → n ) MATHEMATICAL SANS-SERIF BOLD SMALL N → LATIN SMALL LETTER N#
    {120316, "o"},   // MA# ( 𝗼 → o ) MATHEMATICAL SANS-SERIF BOLD SMALL O → LATIN SMALL LETTER O#
    {120317, "p"},   // MA# ( 𝗽 → p ) MATHEMATICAL SANS-SERIF BOLD SMALL P → LATIN SMALL LETTER P#
    {120318, "q"},   // MA# ( 𝗾 → q ) MATHEMATICAL SANS-SERIF BOLD SMALL Q → LATIN SMALL LETTER Q#
    {120319, "r"},   // MA# ( 𝗿 → r ) MATHEMATICAL SANS-SERIF BOLD SMALL R → LATIN SMALL LETTER R#
    {120320, "s"},   // MA# ( 𝘀 → s ) MATHEMATICAL SANS-SERIF BOLD SMALL S → LATIN SMALL LETTER S#
    {120321, "t"},   // MA# ( 𝘁 → t ) MATHEMATICAL SANS-SERIF BOLD SMALL T → LATIN SMALL LETTER T#
    {120322, "u"},   // MA# ( 𝘂 → u ) MATHEMATICAL SANS-SERIF BOLD SMALL U → LATIN SMALL LETTER U#
    {120323, "v"},   // MA# ( 𝘃 → v ) MATHEMATICAL SANS-SERIF BOLD SMALL V → LATIN SMALL LETTER V#
    {120324, "w"},   // MA# ( 𝘄 → w ) MATHEMATICAL SANS-SERIF BOLD SMALL W → LATIN SMALL LETTER W#
    {120325, "x"},   // MA# ( 𝘅 → x ) MATHEMATICAL SANS-SERIF BOLD SMALL X → LATIN SMALL LETTER X#
    {120326, "y"},   // MA# ( 𝘆 → y ) MATHEMATICAL SANS-SERIF BOLD SMALL Y → LATIN SMALL LETTER Y#
    {120327, "z"},   // MA# ( 𝘇 → z ) MATHEMATICAL SANS-SERIF BOLD SMALL Z → LATIN SMALL LETTER Z#
    {120328, "A"},   // MA# ( 𝘈 → A ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL A → LATIN CAPITAL LETTER A#
    {120329, "B"},   // MA# ( 𝘉 → B ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL B → LATIN CAPITAL LETTER B#
    {120330, "C"},   // MA# ( 𝘊 → C ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL C → LATIN CAPITAL LETTER C#
    {120331, "D"},   // MA# ( 𝘋 → D ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL D → LATIN CAPITAL LETTER D#
    {120332, "E"},   // MA# ( 𝘌 → E ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL E → LATIN CAPITAL LETTER E#
    {120333, "F"},   // MA# ( 𝘍 → F ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL F → LATIN CAPITAL LETTER F#
    {120334, "G"},   // MA# ( 𝘎 → G ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL G → LATIN CAPITAL LETTER G#
    {120335, "H"},   // MA# ( 𝘏 → H ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL H → LATIN CAPITAL LETTER H#
    {120336, "l"},   // MA# ( 𝘐 → l ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL I → LATIN SMALL LETTER L# →I→
    {120337, "J"},   // MA# ( 𝘑 → J ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL J → LATIN CAPITAL LETTER J#
    {120338, "K"},   // MA# ( 𝘒 → K ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL K → LATIN CAPITAL LETTER K#
    {120339, "L"},   // MA# ( 𝘓 → L ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL L → LATIN CAPITAL LETTER L#
    {120340, "M"},   // MA# ( 𝘔 → M ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL M → LATIN CAPITAL LETTER M#
    {120341, "N"},   // MA# ( 𝘕 → N ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL N → LATIN CAPITAL LETTER N#
    {120342, "O"},   // MA# ( 𝘖 → O ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL O → LATIN CAPITAL LETTER O#
    {120343, "P"},   // MA# ( 𝘗 → P ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL P → LATIN CAPITAL LETTER P#
    {120344, "Q"},   // MA# ( 𝘘 → Q ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120345, "R"},   // MA# ( 𝘙 → R ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL R → LATIN CAPITAL LETTER R#
    {120346, "S"},   // MA# ( 𝘚 → S ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL S → LATIN CAPITAL LETTER S#
    {120347, "T"},   // MA# ( 𝘛 → T ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL T → LATIN CAPITAL LETTER T#
    {120348, "U"},   // MA# ( 𝘜 → U ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL U → LATIN CAPITAL LETTER U#
    {120349, "V"},   // MA# ( 𝘝 → V ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL V → LATIN CAPITAL LETTER V#
    {120350, "W"},   // MA# ( 𝘞 → W ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL W → LATIN CAPITAL LETTER W#
    {120351, "X"},   // MA# ( 𝘟 → X ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL X → LATIN CAPITAL LETTER X#
    {120352, "Y"},   // MA# ( 𝘠 → Y ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120353, "Z"},   // MA# ( 𝘡 → Z ) MATHEMATICAL SANS-SERIF ITALIC CAPITAL Z → LATIN CAPITAL LETTER Z#
    {120354, "a"},   // MA# ( 𝘢 → a ) MATHEMATICAL SANS-SERIF ITALIC SMALL A → LATIN SMALL LETTER A#
    {120355, "b"},   // MA# ( 𝘣 → b ) MATHEMATICAL SANS-SERIF ITALIC SMALL B → LATIN SMALL LETTER B#
    {120356, "c"},   // MA# ( 𝘤 → c ) MATHEMATICAL SANS-SERIF ITALIC SMALL C → LATIN SMALL LETTER C#
    {120357, "d"},   // MA# ( 𝘥 → d ) MATHEMATICAL SANS-SERIF ITALIC SMALL D → LATIN SMALL LETTER D#
    {120358, "e"},   // MA# ( 𝘦 → e ) MATHEMATICAL SANS-SERIF ITALIC SMALL E → LATIN SMALL LETTER E#
    {120359, "f"},   // MA# ( 𝘧 → f ) MATHEMATICAL SANS-SERIF ITALIC SMALL F → LATIN SMALL LETTER F#
    {120360, "g"},   // MA# ( 𝘨 → g ) MATHEMATICAL SANS-SERIF ITALIC SMALL G → LATIN SMALL LETTER G#
    {120361, "h"},   // MA# ( 𝘩 → h ) MATHEMATICAL SANS-SERIF ITALIC SMALL H → LATIN SMALL LETTER H#
    {120362, "i"},   // MA# ( 𝘪 → i ) MATHEMATICAL SANS-SERIF ITALIC SMALL I → LATIN SMALL LETTER I#
    {120363, "j"},   // MA# ( 𝘫 → j ) MATHEMATICAL SANS-SERIF ITALIC SMALL J → LATIN SMALL LETTER J#
    {120364, "k"},   // MA# ( 𝘬 → k ) MATHEMATICAL SANS-SERIF ITALIC SMALL K → LATIN SMALL LETTER K#
    {120365, "l"},   // MA# ( 𝘭 → l ) MATHEMATICAL SANS-SERIF ITALIC SMALL L → LATIN SMALL LETTER L#
    {120366, "rn"},  // MA# ( 𝘮 → rn ) MATHEMATICAL SANS-SERIF ITALIC SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120367, "n"},   // MA# ( 𝘯 → n ) MATHEMATICAL SANS-SERIF ITALIC SMALL N → LATIN SMALL LETTER N#
    {120368, "o"},   // MA# ( 𝘰 → o ) MATHEMATICAL SANS-SERIF ITALIC SMALL O → LATIN SMALL LETTER O#
    {120369, "p"},   // MA# ( 𝘱 → p ) MATHEMATICAL SANS-SERIF ITALIC SMALL P → LATIN SMALL LETTER P#
    {120370, "q"},   // MA# ( 𝘲 → q ) MATHEMATICAL SANS-SERIF ITALIC SMALL Q → LATIN SMALL LETTER Q#
    {120371, "r"},   // MA# ( 𝘳 → r ) MATHEMATICAL SANS-SERIF ITALIC SMALL R → LATIN SMALL LETTER R#
    {120372, "s"},   // MA# ( 𝘴 → s ) MATHEMATICAL SANS-SERIF ITALIC SMALL S → LATIN SMALL LETTER S#
    {120373, "t"},   // MA# ( 𝘵 → t ) MATHEMATICAL SANS-SERIF ITALIC SMALL T → LATIN SMALL LETTER T#
    {120374, "u"},   // MA# ( 𝘶 → u ) MATHEMATICAL SANS-SERIF ITALIC SMALL U → LATIN SMALL LETTER U#
    {120375, "v"},   // MA# ( 𝘷 → v ) MATHEMATICAL SANS-SERIF ITALIC SMALL V → LATIN SMALL LETTER V#
    {120376, "w"},   // MA# ( 𝘸 → w ) MATHEMATICAL SANS-SERIF ITALIC SMALL W → LATIN SMALL LETTER W#
    {120377, "x"},   // MA# ( 𝘹 → x ) MATHEMATICAL SANS-SERIF ITALIC SMALL X → LATIN SMALL LETTER X#
    {120378, "y"},   // MA# ( 𝘺 → y ) MATHEMATICAL SANS-SERIF ITALIC SMALL Y → LATIN SMALL LETTER Y#
    {120379, "z"},   // MA# ( 𝘻 → z ) MATHEMATICAL SANS-SERIF ITALIC SMALL Z → LATIN SMALL LETTER Z#
    {120380, "A"},   // MA# ( 𝘼 → A ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL A → LATIN CAPITAL LETTER A#
    {120381, "B"},   // MA# ( 𝘽 → B ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL B → LATIN CAPITAL LETTER B#
    {120382, "C"},   // MA# ( 𝘾 → C ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL C → LATIN CAPITAL LETTER C#
    {120383, "D"},   // MA# ( 𝘿 → D ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL D → LATIN CAPITAL LETTER D#
    {120384, "E"},   // MA# ( 𝙀 → E ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL E → LATIN CAPITAL LETTER E#
    {120385, "F"},   // MA# ( 𝙁 → F ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL F → LATIN CAPITAL LETTER F#
    {120386, "G"},   // MA# ( 𝙂 → G ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL G → LATIN CAPITAL LETTER G#
    {120387, "H"},   // MA# ( 𝙃 → H ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL H → LATIN CAPITAL LETTER H#
    {120388, "l"},   // MA# ( 𝙄 → l ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL I → LATIN SMALL LETTER L# →I→
    {120389, "J"},   // MA# ( 𝙅 → J ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL J → LATIN CAPITAL LETTER J#
    {120390, "K"},   // MA# ( 𝙆 → K ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL K → LATIN CAPITAL LETTER K#
    {120391, "L"},   // MA# ( 𝙇 → L ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL L → LATIN CAPITAL LETTER L#
    {120392, "M"},   // MA# ( 𝙈 → M ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL M → LATIN CAPITAL LETTER M#
    {120393, "N"},   // MA# ( 𝙉 → N ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL N → LATIN CAPITAL LETTER N#
    {120394, "O"},   // MA# ( 𝙊 → O ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL O → LATIN CAPITAL LETTER O#
    {120395, "P"},   // MA# ( 𝙋 → P ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL P → LATIN CAPITAL LETTER P#
    {120396, "Q"},   // MA# ( 𝙌 → Q ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120397, "R"},   // MA# ( 𝙍 → R ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL R → LATIN CAPITAL LETTER R#
    {120398, "S"},   // MA# ( 𝙎 → S ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL S → LATIN CAPITAL LETTER S#
    {120399, "T"},   // MA# ( 𝙏 → T ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL T → LATIN CAPITAL LETTER T#
    {120400, "U"},   // MA# ( 𝙐 → U ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL U → LATIN CAPITAL LETTER U#
    {120401, "V"},   // MA# ( 𝙑 → V ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL V → LATIN CAPITAL LETTER V#
    {120402, "W"},   // MA# ( 𝙒 → W ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL W → LATIN CAPITAL LETTER W#
    {120403, "X"},   // MA# ( 𝙓 → X ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL X → LATIN CAPITAL LETTER X#
    {120404, "Y"},   // MA# ( 𝙔 → Y ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120405, "Z"},   // MA# ( 𝙕 → Z ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL Z → LATIN CAPITAL LETTER Z#
    {120406, "a"},   // MA# ( 𝙖 → a ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL A → LATIN SMALL LETTER A#
    {120407, "b"},   // MA# ( 𝙗 → b ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL B → LATIN SMALL LETTER B#
    {120408, "c"},   // MA# ( 𝙘 → c ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL C → LATIN SMALL LETTER C#
    {120409, "d"},   // MA# ( 𝙙 → d ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL D → LATIN SMALL LETTER D#
    {120410, "e"},   // MA# ( 𝙚 → e ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL E → LATIN SMALL LETTER E#
    {120411, "f"},   // MA# ( 𝙛 → f ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL F → LATIN SMALL LETTER F#
    {120412, "g"},   // MA# ( 𝙜 → g ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL G → LATIN SMALL LETTER G#
    {120413, "h"},   // MA# ( 𝙝 → h ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL H → LATIN SMALL LETTER H#
    {120414, "i"},   // MA# ( 𝙞 → i ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL I → LATIN SMALL LETTER I#
    {120415, "j"},   // MA# ( 𝙟 → j ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL J → LATIN SMALL LETTER J#
    {120416, "k"},   // MA# ( 𝙠 → k ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL K → LATIN SMALL LETTER K#
    {120417, "l"},   // MA# ( 𝙡 → l ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL L → LATIN SMALL LETTER L#
    {120418, "rn"},  // MA# ( 𝙢 → rn ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120419, "n"},   // MA# ( 𝙣 → n ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL N → LATIN SMALL LETTER N#
    {120420, "o"},   // MA# ( 𝙤 → o ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL O → LATIN SMALL LETTER O#
    {120421, "p"},   // MA# ( 𝙥 → p ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL P → LATIN SMALL LETTER P#
    {120422, "q"},   // MA# ( 𝙦 → q ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL Q → LATIN SMALL LETTER Q#
    {120423, "r"},   // MA# ( 𝙧 → r ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL R → LATIN SMALL LETTER R#
    {120424, "s"},   // MA# ( 𝙨 → s ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL S → LATIN SMALL LETTER S#
    {120425, "t"},   // MA# ( 𝙩 → t ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL T → LATIN SMALL LETTER T#
    {120426, "u"},   // MA# ( 𝙪 → u ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL U → LATIN SMALL LETTER U#
    {120427, "v"},   // MA# ( 𝙫 → v ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL V → LATIN SMALL LETTER V#
    {120428, "w"},   // MA# ( 𝙬 → w ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL W → LATIN SMALL LETTER W#
    {120429, "x"},   // MA# ( 𝙭 → x ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL X → LATIN SMALL LETTER X#
    {120430, "y"},   // MA# ( 𝙮 → y ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL Y → LATIN SMALL LETTER Y#
    {120431, "z"},   // MA# ( 𝙯 → z ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL Z → LATIN SMALL LETTER Z#
    {120432, "A"},   // MA# ( 𝙰 → A ) MATHEMATICAL MONOSPACE CAPITAL A → LATIN CAPITAL LETTER A#
    {120433, "B"},   // MA# ( 𝙱 → B ) MATHEMATICAL MONOSPACE CAPITAL B → LATIN CAPITAL LETTER B#
    {120434, "C"},   // MA# ( 𝙲 → C ) MATHEMATICAL MONOSPACE CAPITAL C → LATIN CAPITAL LETTER C#
    {120435, "D"},   // MA# ( 𝙳 → D ) MATHEMATICAL MONOSPACE CAPITAL D → LATIN CAPITAL LETTER D#
    {120436, "E"},   // MA# ( 𝙴 → E ) MATHEMATICAL MONOSPACE CAPITAL E → LATIN CAPITAL LETTER E#
    {120437, "F"},   // MA# ( 𝙵 → F ) MATHEMATICAL MONOSPACE CAPITAL F → LATIN CAPITAL LETTER F#
    {120438, "G"},   // MA# ( 𝙶 → G ) MATHEMATICAL MONOSPACE CAPITAL G → LATIN CAPITAL LETTER G#
    {120439, "H"},   // MA# ( 𝙷 → H ) MATHEMATICAL MONOSPACE CAPITAL H → LATIN CAPITAL LETTER H#
    {120440, "l"},   // MA# ( 𝙸 → l ) MATHEMATICAL MONOSPACE CAPITAL I → LATIN SMALL LETTER L# →I→
    {120441, "J"},   // MA# ( 𝙹 → J ) MATHEMATICAL MONOSPACE CAPITAL J → LATIN CAPITAL LETTER J#
    {120442, "K"},   // MA# ( 𝙺 → K ) MATHEMATICAL MONOSPACE CAPITAL K → LATIN CAPITAL LETTER K#
    {120443, "L"},   // MA# ( 𝙻 → L ) MATHEMATICAL MONOSPACE CAPITAL L → LATIN CAPITAL LETTER L#
    {120444, "M"},   // MA# ( 𝙼 → M ) MATHEMATICAL MONOSPACE CAPITAL M → LATIN CAPITAL LETTER M#
    {120445, "N"},   // MA# ( 𝙽 → N ) MATHEMATICAL MONOSPACE CAPITAL N → LATIN CAPITAL LETTER N#
    {120446, "O"},   // MA# ( 𝙾 → O ) MATHEMATICAL MONOSPACE CAPITAL O → LATIN CAPITAL LETTER O#
    {120447, "P"},   // MA# ( 𝙿 → P ) MATHEMATICAL MONOSPACE CAPITAL P → LATIN CAPITAL LETTER P#
    {120448, "Q"},   // MA# ( 𝚀 → Q ) MATHEMATICAL MONOSPACE CAPITAL Q → LATIN CAPITAL LETTER Q#
    {120449, "R"},   // MA# ( 𝚁 → R ) MATHEMATICAL MONOSPACE CAPITAL R → LATIN CAPITAL LETTER R#
    {120450, "S"},   // MA# ( 𝚂 → S ) MATHEMATICAL MONOSPACE CAPITAL S → LATIN CAPITAL LETTER S#
    {120451, "T"},   // MA# ( 𝚃 → T ) MATHEMATICAL MONOSPACE CAPITAL T → LATIN CAPITAL LETTER T#
    {120452, "U"},   // MA# ( 𝚄 → U ) MATHEMATICAL MONOSPACE CAPITAL U → LATIN CAPITAL LETTER U#
    {120453, "V"},   // MA# ( 𝚅 → V ) MATHEMATICAL MONOSPACE CAPITAL V → LATIN CAPITAL LETTER V#
    {120454, "W"},   // MA# ( 𝚆 → W ) MATHEMATICAL MONOSPACE CAPITAL W → LATIN CAPITAL LETTER W#
    {120455, "X"},   // MA# ( 𝚇 → X ) MATHEMATICAL MONOSPACE CAPITAL X → LATIN CAPITAL LETTER X#
    {120456, "Y"},   // MA# ( 𝚈 → Y ) MATHEMATICAL MONOSPACE CAPITAL Y → LATIN CAPITAL LETTER Y#
    {120457, "Z"},   // MA# ( 𝚉 → Z ) MATHEMATICAL MONOSPACE CAPITAL Z → LATIN CAPITAL LETTER Z#
    {120458, "a"},   // MA# ( 𝚊 → a ) MATHEMATICAL MONOSPACE SMALL A → LATIN SMALL LETTER A#
    {120459, "b"},   // MA# ( 𝚋 → b ) MATHEMATICAL MONOSPACE SMALL B → LATIN SMALL LETTER B#
    {120460, "c"},   // MA# ( 𝚌 → c ) MATHEMATICAL MONOSPACE SMALL C → LATIN SMALL LETTER C#
    {120461, "d"},   // MA# ( 𝚍 → d ) MATHEMATICAL MONOSPACE SMALL D → LATIN SMALL LETTER D#
    {120462, "e"},   // MA# ( 𝚎 → e ) MATHEMATICAL MONOSPACE SMALL E → LATIN SMALL LETTER E#
    {120463, "f"},   // MA# ( 𝚏 → f ) MATHEMATICAL MONOSPACE SMALL F → LATIN SMALL LETTER F#
    {120464, "g"},   // MA# ( 𝚐 → g ) MATHEMATICAL MONOSPACE SMALL G → LATIN SMALL LETTER G#
    {120465, "h"},   // MA# ( 𝚑 → h ) MATHEMATICAL MONOSPACE SMALL H → LATIN SMALL LETTER H#
    {120466, "i"},   // MA# ( 𝚒 → i ) MATHEMATICAL MONOSPACE SMALL I → LATIN SMALL LETTER I#
    {120467, "j"},   // MA# ( 𝚓 → j ) MATHEMATICAL MONOSPACE SMALL J → LATIN SMALL LETTER J#
    {120468, "k"},   // MA# ( 𝚔 → k ) MATHEMATICAL MONOSPACE SMALL K → LATIN SMALL LETTER K#
    {120469, "l"},   // MA# ( 𝚕 → l ) MATHEMATICAL MONOSPACE SMALL L → LATIN SMALL LETTER L#
    {120470, "rn"},  // MA# ( 𝚖 → rn ) MATHEMATICAL MONOSPACE SMALL M → LATIN SMALL LETTER R, LATIN SMALL LETTER N# →m→
    {120471, "n"},   // MA# ( 𝚗 → n ) MATHEMATICAL MONOSPACE SMALL N → LATIN SMALL LETTER N#
    {120472, "o"},   // MA# ( 𝚘 → o ) MATHEMATICAL MONOSPACE SMALL O → LATIN SMALL LETTER O#
    {120473, "p"},   // MA# ( 𝚙 → p ) MATHEMATICAL MONOSPACE SMALL P → LATIN SMALL LETTER P#
    {120474, "q"},   // MA# ( 𝚚 → q ) MATHEMATICAL MONOSPACE SMALL Q → LATIN SMALL LETTER Q#
    {120475, "r"},   // MA# ( 𝚛 → r ) MATHEMATICAL MONOSPACE SMALL R → LATIN SMALL LETTER R#
    {120476, "s"},   // MA# ( 𝚜 → s ) MATHEMATICAL MONOSPACE SMALL S → LATIN SMALL LETTER S#
    {120477, "t"},   // MA# ( 𝚝 → t ) MATHEMATICAL MONOSPACE SMALL T → LATIN SMALL LETTER T#
    {120478, "u"},   // MA# ( 𝚞 → u ) MATHEMATICAL MONOSPACE SMALL U → LATIN SMALL LETTER U#
    {120479, "v"},   // MA# ( 𝚟 → v ) MATHEMATICAL MONOSPACE SMALL V → LATIN SMALL LETTER V#
    {120480, "w"},   // MA# ( 𝚠 → w ) MATHEMATICAL MONOSPACE SMALL W → LATIN SMALL LETTER W#
    {120481, "x"},   // MA# ( 𝚡 → x ) MATHEMATICAL MONOSPACE SMALL X → LATIN SMALL LETTER X#
    {120482, "y"},   // MA# ( 𝚢 → y ) MATHEMATICAL MONOSPACE SMALL Y → LATIN SMALL LETTER Y#
    {120483, "z"},   // MA# ( 𝚣 → z ) MATHEMATICAL MONOSPACE SMALL Z → LATIN SMALL LETTER Z#
    {120484, "i"},   // MA# ( 𝚤 → i ) MATHEMATICAL ITALIC SMALL DOTLESS I → LATIN SMALL LETTER I# →ı→
    {120488, "A"},   // MA# ( 𝚨 → A ) MATHEMATICAL BOLD CAPITAL ALPHA → LATIN CAPITAL LETTER A# →𝐀→
    {120489, "B"},   // MA# ( 𝚩 → B ) MATHEMATICAL BOLD CAPITAL BETA → LATIN CAPITAL LETTER B# →Β→
    {120492, "E"},   // MA# ( 𝚬 → E ) MATHEMATICAL BOLD CAPITAL EPSILON → LATIN CAPITAL LETTER E# →𝐄→
    {120493, "Z"},   // MA# ( 𝚭 → Z ) MATHEMATICAL BOLD CAPITAL ZETA → LATIN CAPITAL LETTER Z# →Ζ→
    {120494, "H"},   // MA# ( 𝚮 → H ) MATHEMATICAL BOLD CAPITAL ETA → LATIN CAPITAL LETTER H# →Η→
    {120496, "l"},   // MA# ( 𝚰 → l ) MATHEMATICAL BOLD CAPITAL IOTA → LATIN SMALL LETTER L# →Ι→
    {120497, "K"},   // MA# ( 𝚱 → K ) MATHEMATICAL BOLD CAPITAL KAPPA → LATIN CAPITAL LETTER K# →Κ→
    {120499, "M"},   // MA# ( 𝚳 → M ) MATHEMATICAL BOLD CAPITAL MU → LATIN CAPITAL LETTER M# →𝐌→
    {120500, "N"},   // MA# ( 𝚴 → N ) MATHEMATICAL BOLD CAPITAL NU → LATIN CAPITAL LETTER N# →𝐍→
    {120502, "O"},   // MA# ( 𝚶 → O ) MATHEMATICAL BOLD CAPITAL OMICRON → LATIN CAPITAL LETTER O# →𝐎→
    {120504, "P"},   // MA# ( 𝚸 → P ) MATHEMATICAL BOLD CAPITAL RHO → LATIN CAPITAL LETTER P# →𝐏→
    {120507, "T"},   // MA# ( 𝚻 → T ) MATHEMATICAL BOLD CAPITAL TAU → LATIN CAPITAL LETTER T# →Τ→
    {120508, "Y"},   // MA# ( 𝚼 → Y ) MATHEMATICAL BOLD CAPITAL UPSILON → LATIN CAPITAL LETTER Y# →Υ→
    {120510, "X"},   // MA# ( 𝚾 → X ) MATHEMATICAL BOLD CAPITAL CHI → LATIN CAPITAL LETTER X# →Χ→
    {120514, "a"},   // MA# ( 𝛂 → a ) MATHEMATICAL BOLD SMALL ALPHA → LATIN SMALL LETTER A# →α→
    {120516, "y"},   // MA# ( 𝛄 → y ) MATHEMATICAL BOLD SMALL GAMMA → LATIN SMALL LETTER Y# →γ→
    {120522, "i"},   // MA# ( 𝛊 → i ) MATHEMATICAL BOLD SMALL IOTA → LATIN SMALL LETTER I# →ι→
    {120526, "v"},   // MA# ( 𝛎 → v ) MATHEMATICAL BOLD SMALL NU → LATIN SMALL LETTER V# →ν→
    {120528, "o"},   // MA# ( 𝛐 → o ) MATHEMATICAL BOLD SMALL OMICRON → LATIN SMALL LETTER O# →𝐨→
    {120530, "p"},   // MA# ( 𝛒 → p ) MATHEMATICAL BOLD SMALL RHO → LATIN SMALL LETTER P# →ρ→
    {120532, "o"},   // MA# ( 𝛔 → o ) MATHEMATICAL BOLD SMALL SIGMA → LATIN SMALL LETTER O# →σ→
    {120534, "u"},   // MA# ( 𝛖 → u ) MATHEMATICAL BOLD SMALL UPSILON → LATIN SMALL LETTER U# →υ→→ʋ→
    {120544, "p"},   // MA# ( 𝛠 → p ) MATHEMATICAL BOLD RHO SYMBOL → LATIN SMALL LETTER P# →ρ→
    {120546, "A"},   // MA# ( 𝛢 → A ) MATHEMATICAL ITALIC CAPITAL ALPHA → LATIN CAPITAL LETTER A# →Α→
    {120547, "B"},   // MA# ( 𝛣 → B ) MATHEMATICAL ITALIC CAPITAL BETA → LATIN CAPITAL LETTER B# →Β→
    {120550, "E"},   // MA# ( 𝛦 → E ) MATHEMATICAL ITALIC CAPITAL EPSILON → LATIN CAPITAL LETTER E# →Ε→
    {120551, "Z"},   // MA# ( 𝛧 → Z ) MATHEMATICAL ITALIC CAPITAL ZETA → LATIN CAPITAL LETTER Z# →𝑍→
    {120552, "H"},   // MA# ( 𝛨 → H ) MATHEMATICAL ITALIC CAPITAL ETA → LATIN CAPITAL LETTER H# →Η→
    {120554, "l"},   // MA# ( 𝛪 → l ) MATHEMATICAL ITALIC CAPITAL IOTA → LATIN SMALL LETTER L# →Ι→
    {120555, "K"},   // MA# ( 𝛫 → K ) MATHEMATICAL ITALIC CAPITAL KAPPA → LATIN CAPITAL LETTER K# →𝐾→
    {120557, "M"},   // MA# ( 𝛭 → M ) MATHEMATICAL ITALIC CAPITAL MU → LATIN CAPITAL LETTER M# →𝑀→
    {120558, "N"},   // MA# ( 𝛮 → N ) MATHEMATICAL ITALIC CAPITAL NU → LATIN CAPITAL LETTER N# →𝑁→
    {120560, "O"},   // MA# ( 𝛰 → O ) MATHEMATICAL ITALIC CAPITAL OMICRON → LATIN CAPITAL LETTER O# →𝑂→
    {120562, "P"},   // MA# ( 𝛲 → P ) MATHEMATICAL ITALIC CAPITAL RHO → LATIN CAPITAL LETTER P# →Ρ→
    {120565, "T"},   // MA# ( 𝛵 → T ) MATHEMATICAL ITALIC CAPITAL TAU → LATIN CAPITAL LETTER T# →Τ→
    {120566, "Y"},   // MA# ( 𝛶 → Y ) MATHEMATICAL ITALIC CAPITAL UPSILON → LATIN CAPITAL LETTER Y# →Υ→
    {120568, "X"},   // MA# ( 𝛸 → X ) MATHEMATICAL ITALIC CAPITAL CHI → LATIN CAPITAL LETTER X# →Χ→
    {120572, "a"},   // MA# ( 𝛼 → a ) MATHEMATICAL ITALIC SMALL ALPHA → LATIN SMALL LETTER A# →α→
    {120574, "y"},   // MA# ( 𝛾 → y ) MATHEMATICAL ITALIC SMALL GAMMA → LATIN SMALL LETTER Y# →γ→
    {120580, "i"},   // MA# ( 𝜄 → i ) MATHEMATICAL ITALIC SMALL IOTA → LATIN SMALL LETTER I# →ι→
    {120584, "v"},   // MA# ( 𝜈 → v ) MATHEMATICAL ITALIC SMALL NU → LATIN SMALL LETTER V# →ν→
    {120586, "o"},   // MA# ( 𝜊 → o ) MATHEMATICAL ITALIC SMALL OMICRON → LATIN SMALL LETTER O# →𝑜→
    {120588, "p"},   // MA# ( 𝜌 → p ) MATHEMATICAL ITALIC SMALL RHO → LATIN SMALL LETTER P# →ρ→
    {120590, "o"},   // MA# ( 𝜎 → o ) MATHEMATICAL ITALIC SMALL SIGMA → LATIN SMALL LETTER O# →σ→
    {120592, "u"},   // MA# ( 𝜐 → u ) MATHEMATICAL ITALIC SMALL UPSILON → LATIN SMALL LETTER U# →υ→→ʋ→
    {120602, "p"},   // MA# ( 𝜚 → p ) MATHEMATICAL ITALIC RHO SYMBOL → LATIN SMALL LETTER P# →ρ→
    {120604, "A"},   // MA# ( 𝜜 → A ) MATHEMATICAL BOLD ITALIC CAPITAL ALPHA → LATIN CAPITAL LETTER A# →Α→
    {120605, "B"},   // MA# ( 𝜝 → B ) MATHEMATICAL BOLD ITALIC CAPITAL BETA → LATIN CAPITAL LETTER B# →Β→
    {120608, "E"},   // MA# ( 𝜠 → E ) MATHEMATICAL BOLD ITALIC CAPITAL EPSILON → LATIN CAPITAL LETTER E# →Ε→
    {120609, "Z"},   // MA# ( 𝜡 → Z ) MATHEMATICAL BOLD ITALIC CAPITAL ZETA → LATIN CAPITAL LETTER Z# →Ζ→
    {120610, "H"},   // MA# ( 𝜢 → H ) MATHEMATICAL BOLD ITALIC CAPITAL ETA → LATIN CAPITAL LETTER H# →𝑯→
    {120612, "l"},   // MA# ( 𝜤 → l ) MATHEMATICAL BOLD ITALIC CAPITAL IOTA → LATIN SMALL LETTER L# →Ι→
    {120613, "K"},   // MA# ( 𝜥 → K ) MATHEMATICAL BOLD ITALIC CAPITAL KAPPA → LATIN CAPITAL LETTER K# →𝑲→
    {120615, "M"},   // MA# ( 𝜧 → M ) MATHEMATICAL BOLD ITALIC CAPITAL MU → LATIN CAPITAL LETTER M# →𝑴→
    {120616, "N"},   // MA# ( 𝜨 → N ) MATHEMATICAL BOLD ITALIC CAPITAL NU → LATIN CAPITAL LETTER N# →𝑵→
    {120618, "O"},   // MA# ( 𝜪 → O ) MATHEMATICAL BOLD ITALIC CAPITAL OMICRON → LATIN CAPITAL LETTER O# →𝑶→
    {120620, "P"},   // MA# ( 𝜬 → P ) MATHEMATICAL BOLD ITALIC CAPITAL RHO → LATIN CAPITAL LETTER P# →Ρ→
    {120623, "T"},   // MA# ( 𝜯 → T ) MATHEMATICAL BOLD ITALIC CAPITAL TAU → LATIN CAPITAL LETTER T# →Τ→
    {120624, "Y"},   // MA# ( 𝜰 → Y ) MATHEMATICAL BOLD ITALIC CAPITAL UPSILON → LATIN CAPITAL LETTER Y# →Υ→
    {120626, "X"},   // MA# ( 𝜲 → X ) MATHEMATICAL BOLD ITALIC CAPITAL CHI → LATIN CAPITAL LETTER X# →𝑿→
    {120630, "a"},   // MA# ( 𝜶 → a ) MATHEMATICAL BOLD ITALIC SMALL ALPHA → LATIN SMALL LETTER A# →α→
    {120632, "y"},   // MA# ( 𝜸 → y ) MATHEMATICAL BOLD ITALIC SMALL GAMMA → LATIN SMALL LETTER Y# →γ→
    {120638, "i"},   // MA# ( 𝜾 → i ) MATHEMATICAL BOLD ITALIC SMALL IOTA → LATIN SMALL LETTER I# →ι→
    {120642, "v"},   // MA# ( 𝝂 → v ) MATHEMATICAL BOLD ITALIC SMALL NU → LATIN SMALL LETTER V# →ν→
    {120644, "o"},   // MA# ( 𝝄 → o ) MATHEMATICAL BOLD ITALIC SMALL OMICRON → LATIN SMALL LETTER O# →𝒐→
    {120646, "p"},   // MA# ( 𝝆 → p ) MATHEMATICAL BOLD ITALIC SMALL RHO → LATIN SMALL LETTER P# →ρ→
    {120648, "o"},   // MA# ( 𝝈 → o ) MATHEMATICAL BOLD ITALIC SMALL SIGMA → LATIN SMALL LETTER O# →σ→
    {120650, "u"},   // MA# ( 𝝊 → u ) MATHEMATICAL BOLD ITALIC SMALL UPSILON → LATIN SMALL LETTER U# →υ→→ʋ→
    {120660, "p"},   // MA# ( 𝝔 → p ) MATHEMATICAL BOLD ITALIC RHO SYMBOL → LATIN SMALL LETTER P# →ρ→
    {120662, "A"},   // MA# ( 𝝖 → A ) MATHEMATICAL SANS-SERIF BOLD CAPITAL ALPHA → LATIN CAPITAL LETTER A# →Α→
    {120663, "B"},   // MA# ( 𝝗 → B ) MATHEMATICAL SANS-SERIF BOLD CAPITAL BETA → LATIN CAPITAL LETTER B# →Β→
    {120666, "E"},   // MA# ( 𝝚 → E ) MATHEMATICAL SANS-SERIF BOLD CAPITAL EPSILON → LATIN CAPITAL LETTER E# →Ε→
    {120667, "Z"},   // MA# ( 𝝛 → Z ) MATHEMATICAL SANS-SERIF BOLD CAPITAL ZETA → LATIN CAPITAL LETTER Z# →Ζ→
    {120668, "H"},   // MA# ( 𝝜 → H ) MATHEMATICAL SANS-SERIF BOLD CAPITAL ETA → LATIN CAPITAL LETTER H# →Η→
    {120670, "l"},   // MA# ( 𝝞 → l ) MATHEMATICAL SANS-SERIF BOLD CAPITAL IOTA → LATIN SMALL LETTER L# →Ι→
    {120671, "K"},   // MA# ( 𝝟 → K ) MATHEMATICAL SANS-SERIF BOLD CAPITAL KAPPA → LATIN CAPITAL LETTER K# →Κ→
    {120673, "M"},   // MA# ( 𝝡 → M ) MATHEMATICAL SANS-SERIF BOLD CAPITAL MU → LATIN CAPITAL LETTER M# →Μ→
    {120674, "N"},   // MA# ( 𝝢 → N ) MATHEMATICAL SANS-SERIF BOLD CAPITAL NU → LATIN CAPITAL LETTER N# →Ν→
    {120676, "O"},   // MA# ( 𝝤 → O ) MATHEMATICAL SANS-SERIF BOLD CAPITAL OMICRON → LATIN CAPITAL LETTER O# →Ο→
    {120678, "P"},   // MA# ( 𝝦 → P ) MATHEMATICAL SANS-SERIF BOLD CAPITAL RHO → LATIN CAPITAL LETTER P# →Ρ→
    {120681, "T"},   // MA# ( 𝝩 → T ) MATHEMATICAL SANS-SERIF BOLD CAPITAL TAU → LATIN CAPITAL LETTER T# →Τ→
    {120682, "Y"},   // MA# ( 𝝪 → Y ) MATHEMATICAL SANS-SERIF BOLD CAPITAL UPSILON → LATIN CAPITAL LETTER Y# →Υ→
    {120684, "X"},   // MA# ( 𝝬 → X ) MATHEMATICAL SANS-SERIF BOLD CAPITAL CHI → LATIN CAPITAL LETTER X# →Χ→
    {120688, "a"},   // MA# ( 𝝰 → a ) MATHEMATICAL SANS-SERIF BOLD SMALL ALPHA → LATIN SMALL LETTER A# →α→
    {120690, "y"},   // MA# ( 𝝲 → y ) MATHEMATICAL SANS-SERIF BOLD SMALL GAMMA → LATIN SMALL LETTER Y# →γ→
    {120696, "i"},   // MA# ( 𝝸 → i ) MATHEMATICAL SANS-SERIF BOLD SMALL IOTA → LATIN SMALL LETTER I# →ι→
    {120700, "v"},   // MA# ( 𝝼 → v ) MATHEMATICAL SANS-SERIF BOLD SMALL NU → LATIN SMALL LETTER V# →ν→
    {120702, "o"},   // MA# ( 𝝾 → o ) MATHEMATICAL SANS-SERIF BOLD SMALL OMICRON → LATIN SMALL LETTER O# →ο→
    {120704, "p"},   // MA# ( 𝞀 → p ) MATHEMATICAL SANS-SERIF BOLD SMALL RHO → LATIN SMALL LETTER P# →ρ→
    {120706, "o"},   // MA# ( 𝞂 → o ) MATHEMATICAL SANS-SERIF BOLD SMALL SIGMA → LATIN SMALL LETTER O# →σ→
    {120708, "u"},   // MA# ( 𝞄 → u ) MATHEMATICAL SANS-SERIF BOLD SMALL UPSILON → LATIN SMALL LETTER U# →υ→→ʋ→
    {120718, "p"},   // MA# ( 𝞎 → p ) MATHEMATICAL SANS-SERIF BOLD RHO SYMBOL → LATIN SMALL LETTER P# →ρ→
    {120720, "A"},   // MA# ( 𝞐 → A ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL ALPHA → LATIN CAPITAL LETTER A# →Α→
    {120721, "B"},   // MA# ( 𝞑 → B ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL BETA → LATIN CAPITAL LETTER B# →Β→
    {120724, "E"},   // MA# ( 𝞔 → E ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL EPSILON → LATIN CAPITAL LETTER E# →Ε→
    {120725, "Z"},   // MA# ( 𝞕 → Z ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL ZETA → LATIN CAPITAL LETTER Z# →Ζ→
    {120726, "H"},   // MA# ( 𝞖 → H ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL ETA → LATIN CAPITAL LETTER H# →Η→
    {120728, "l"},   // MA# ( 𝞘 → l ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL IOTA → LATIN SMALL LETTER L# →Ι→
    {120729, "K"},   // MA# ( 𝞙 → K ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL KAPPA → LATIN CAPITAL LETTER K# →Κ→
    {120731, "M"},   // MA# ( 𝞛 → M ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL MU → LATIN CAPITAL LETTER M# →Μ→
    {120732, "N"},   // MA# ( 𝞜 → N ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL NU → LATIN CAPITAL LETTER N# →Ν→
    {120734, "O"},   // MA# ( 𝞞 → O ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL OMICRON → LATIN CAPITAL LETTER O# →Ο→
    {120736, "P"},   // MA# ( 𝞠 → P ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL RHO → LATIN CAPITAL LETTER P# →Ρ→
    {120739, "T"},   // MA# ( 𝞣 → T ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL TAU → LATIN CAPITAL LETTER T# →Τ→
    {120740, "Y"},   // MA# ( 𝞤 → Y ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL UPSILON → LATIN CAPITAL LETTER Y# →Υ→
    {120742, "X"},   // MA# ( 𝞦 → X ) MATHEMATICAL SANS-SERIF BOLD ITALIC CAPITAL CHI → LATIN CAPITAL LETTER X# →Χ→
    {120746, "a"},   // MA# ( 𝞪 → a ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL ALPHA → LATIN SMALL LETTER A# →α→
    {120748, "y"},   // MA# ( 𝞬 → y ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL GAMMA → LATIN SMALL LETTER Y# →γ→
    {120754, "i"},   // MA# ( 𝞲 → i ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL IOTA → LATIN SMALL LETTER I# →ι→
    {120758, "v"},   // MA# ( 𝞶 → v ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL NU → LATIN SMALL LETTER V# →ν→
    {120760, "o"},   // MA# ( 𝞸 → o ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL OMICRON → LATIN SMALL LETTER O# →ο→
    {120762, "p"},   // MA# ( 𝞺 → p ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL RHO → LATIN SMALL LETTER P# →ρ→
    {120764, "o"},   // MA# ( 𝞼 → o ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL SIGMA → LATIN SMALL LETTER O# →σ→
    {120766, "u"},   // MA# ( 𝞾 → u ) MATHEMATICAL SANS-SERIF BOLD ITALIC SMALL UPSILON → LATIN SMALL LETTER U# →υ→→ʋ→
    {120776, "p"},   // MA# ( 𝟈 → p ) MATHEMATICAL SANS-SERIF BOLD ITALIC RHO SYMBOL → LATIN SMALL LETTER P# →ρ→
    {120778, "F"},   // MA# ( 𝟊 → F ) MATHEMATICAL BOLD CAPITAL DIGAMMA → LATIN CAPITAL LETTER F# →Ϝ→
    {120782, "O"},   // MA# ( 𝟎 → O ) MATHEMATICAL BOLD DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {120783, "l"},   // MA# ( 𝟏 → l ) MATHEMATICAL BOLD DIGIT ONE → LATIN SMALL LETTER L# →1→
    {120784, "2"},   // MA# ( 𝟐 → 2 ) MATHEMATICAL BOLD DIGIT TWO → DIGIT TWO#
    {120785, "3"},   // MA# ( 𝟑 → 3 ) MATHEMATICAL BOLD DIGIT THREE → DIGIT THREE#
    {120786, "4"},   // MA# ( 𝟒 → 4 ) MATHEMATICAL BOLD DIGIT FOUR → DIGIT FOUR#
    {120787, "5"},   // MA# ( 𝟓 → 5 ) MATHEMATICAL BOLD DIGIT FIVE → DIGIT FIVE#
    {120788, "6"},   // MA# ( 𝟔 → 6 ) MATHEMATICAL BOLD DIGIT SIX → DIGIT SIX#
    {120789, "7"},   // MA# ( 𝟕 → 7 ) MATHEMATICAL BOLD DIGIT SEVEN → DIGIT SEVEN#
    {120790, "8"},   // MA# ( 𝟖 → 8 ) MATHEMATICAL BOLD DIGIT EIGHT → DIGIT EIGHT#
    {120791, "9"},   // MA# ( 𝟗 → 9 ) MATHEMATICAL BOLD DIGIT NINE → DIGIT NINE#
    {120792, "O"},   // MA# ( 𝟘 → O ) MATHEMATICAL DOUBLE-STRUCK DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {120793, "l"},   // MA# ( 𝟙 → l ) MATHEMATICAL DOUBLE-STRUCK DIGIT ONE → LATIN SMALL LETTER L# →1→
    {120794, "2"},   // MA# ( 𝟚 → 2 ) MATHEMATICAL DOUBLE-STRUCK DIGIT TWO → DIGIT TWO#
    {120795, "3"},   // MA# ( 𝟛 → 3 ) MATHEMATICAL DOUBLE-STRUCK DIGIT THREE → DIGIT THREE#
    {120796, "4"},   // MA# ( 𝟜 → 4 ) MATHEMATICAL DOUBLE-STRUCK DIGIT FOUR → DIGIT FOUR#
    {120797, "5"},   // MA# ( 𝟝 → 5 ) MATHEMATICAL DOUBLE-STRUCK DIGIT FIVE → DIGIT FIVE#
    {120798, "6"},   // MA# ( 𝟞 → 6 ) MATHEMATICAL DOUBLE-STRUCK DIGIT SIX → DIGIT SIX#
    {120799, "7"},   // MA# ( 𝟟 → 7 ) MATHEMATICAL DOUBLE-STRUCK DIGIT SEVEN → DIGIT SEVEN#
    {120800, "8"},   // MA# ( 𝟠 → 8 ) MATHEMATICAL DOUBLE-STRUCK DIGIT EIGHT → DIGIT EIGHT#
    {120801, "9"},   // MA# ( 𝟡 → 9 ) MATHEMATICAL DOUBLE-STRUCK DIGIT NINE → DIGIT NINE#
    {120802, "O"},   // MA# ( 𝟢 → O ) MATHEMATICAL SANS-SERIF DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {120803, "l"},   // MA# ( 𝟣 → l ) MATHEMATICAL SANS-SERIF DIGIT ONE → LATIN SMALL LETTER L# →1→
    {120804, "2"},   // MA# ( 𝟤 → 2 ) MATHEMATICAL SANS-SERIF DIGIT TWO → DIGIT TWO#
    {120805, "3"},   // MA# ( 𝟥 → 3 ) MATHEMATICAL SANS-SERIF DIGIT THREE → DIGIT THREE#
    {120806, "4"},   // MA# ( 𝟦 → 4 ) MATHEMATICAL SANS-SERIF DIGIT FOUR → DIGIT FOUR#
    {120807, "5"},   // MA# ( 𝟧 → 5 ) MATHEMATICAL SANS-SERIF DIGIT FIVE → DIGIT FIVE#
    {120808, "6"},   // MA# ( 𝟨 → 6 ) MATHEMATICAL SANS-SERIF DIGIT SIX → DIGIT SIX#
    {120809, "7"},   // MA# ( 𝟩 → 7 ) MATHEMATICAL SANS-SERIF DIGIT SEVEN → DIGIT SEVEN#
    {120810, "8"},   // MA# ( 𝟪 → 8 ) MATHEMATICAL SANS-SERIF DIGIT EIGHT → DIGIT EIGHT#
    {120811, "9"},   // MA# ( 𝟫 → 9 ) MATHEMATICAL SANS-SERIF DIGIT NINE → DIGIT NINE#
    {120812, "O"},   // MA# ( 𝟬 → O ) MATHEMATICAL SANS-SERIF BOLD DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {120813, "l"},   // MA# ( 𝟭 → l ) MATHEMATICAL SANS-SERIF BOLD DIGIT ONE → LATIN SMALL LETTER L# →1→
    {120814, "2"},   // MA# ( 𝟮 → 2 ) MATHEMATICAL SANS-SERIF BOLD DIGIT TWO → DIGIT TWO#
    {120815, "3"},   // MA# ( 𝟯 → 3 ) MATHEMATICAL SANS-SERIF BOLD DIGIT THREE → DIGIT THREE#
    {120816, "4"},   // MA# ( 𝟰 → 4 ) MATHEMATICAL SANS-SERIF BOLD DIGIT FOUR → DIGIT FOUR#
    {120817, "5"},   // MA# ( 𝟱 → 5 ) MATHEMATICAL SANS-SERIF BOLD DIGIT FIVE → DIGIT FIVE#
    {120818, "6"},   // MA# ( 𝟲 → 6 ) MATHEMATICAL SANS-SERIF BOLD DIGIT SIX → DIGIT SIX#
    {120819, "7"},   // MA# ( 𝟳 → 7 ) MATHEMATICAL SANS-SERIF BOLD DIGIT SEVEN → DIGIT SEVEN#
    {120820, "8"},   // MA# ( 𝟴 → 8 ) MATHEMATICAL SANS-SERIF BOLD DIGIT EIGHT → DIGIT EIGHT#
    {120821, "9"},   // MA# ( 𝟵 → 9 ) MATHEMATICAL SANS-SERIF BOLD DIGIT NINE → DIGIT NINE#
    {120822, "O"},   // MA# ( 𝟶 → O ) MATHEMATICAL MONOSPACE DIGIT ZERO → LATIN CAPITAL LETTER O# →0→
    {120823, "l"},   // MA# ( 𝟷 → l ) MATHEMATICAL MONOSPACE DIGIT ONE → LATIN SMALL LETTER L# →1→
    {120824, "2"},   // MA# ( 𝟸 → 2 ) MATHEMATICAL MONOSPACE DIGIT TWO → DIGIT TWO#
    {120825, "3"},   // MA# ( 𝟹 → 3 ) MATHEMATICAL MONOSPACE DIGIT THREE → DIGIT THREE#
    {120826, "4"},   // MA# ( 𝟺 → 4 ) MATHEMATICAL MONOSPACE DIGIT FOUR → DIGIT FOUR#
    {120827, "5"},   // MA# ( 𝟻 → 5 ) MATHEMATICAL MONOSPACE DIGIT FIVE → DIGIT FIVE#
    {120828, "6"},   // MA# ( 𝟼 → 6 ) MATHEMATICAL MONOSPACE DIGIT SIX → DIGIT SIX#
    {120829, "7"},   // MA# ( 𝟽 → 7 ) MATHEMATICAL MONOSPACE DIGIT SEVEN → DIGIT SEVEN#
    {120830, "8"},   // MA# ( 𝟾 → 8 ) MATHEMATICAL MONOSPACE DIGIT EIGHT → DIGIT EIGHT#
    {120831, "9"},   // MA# ( 𝟿 → 9 ) MATHEMATICAL MONOSPACE DIGIT NINE → DIGIT NINE#
    {125127, "l"},   // MA#* ( ‎𞣇‎ → l ) MENDE KIKAKUI DIGIT ONE → LATIN SMALL LETTER L#
    {125131, "8"},   // MA#* ( ‎𞣋‎ → 8 ) MENDE KIKAKUI DIGIT FIVE → DIGIT EIGHT#
    {126464, "l"},   // MA# ( ‎𞸀‎ → l ) ARABIC MATHEMATICAL ALEF → LATIN SMALL LETTER L# →‎ا‎→→1→
    {126500, "o"},   // MA# ( ‎𞸤‎ → o ) ARABIC MATHEMATICAL INITIAL HEH → LATIN SMALL LETTER O# →‎ه‎→
    {126564, "o"},   // MA# ( ‎𞹤‎ → o ) ARABIC MATHEMATICAL STRETCHED HEH → LATIN SMALL LETTER O# →‎ه‎→
    {126592, "l"},   // MA# ( ‎𞺀‎ → l ) ARABIC MATHEMATICAL LOOPED ALEF → LATIN SMALL LETTER L# →‎ا‎→→1→
    {126596, "o"},   // MA# ( ‎𞺄‎ → o ) ARABIC MATHEMATICAL LOOPED HEH → LATIN SMALL LETTER O# →‎ه‎→
    {127232, "O."},  // MA#* ( 🄀 → O. ) DIGIT ZERO FULL STOP → LATIN CAPITAL LETTER O, FULL STOP# →0.→
    {127233, "O,"},  // MA#* ( 🄁 → O, ) DIGIT ZERO COMMA → LATIN CAPITAL LETTER O, COMMA# →0,→
    {127234, "l,"},  // MA#* ( 🄂 → l, ) DIGIT ONE COMMA → LATIN SMALL LETTER L, COMMA# →1,→
    {127235, "2,"},  // MA#* ( 🄃 → 2, ) DIGIT TWO COMMA → DIGIT TWO, COMMA#
    {127236, "3,"},  // MA#* ( 🄄 → 3, ) DIGIT THREE COMMA → DIGIT THREE, COMMA#
    {127237, "4,"},  // MA#* ( 🄅 → 4, ) DIGIT FOUR COMMA → DIGIT FOUR, COMMA#
    {127238, "5,"},  // MA#* ( 🄆 → 5, ) DIGIT FIVE COMMA → DIGIT FIVE, COMMA#
    {127239, "6,"},  // MA#* ( 🄇 → 6, ) DIGIT SIX COMMA → DIGIT SIX, COMMA#
    {127240, "7,"},  // MA#* ( 🄈 → 7, ) DIGIT SEVEN COMMA → DIGIT SEVEN, COMMA#
    {127241, "8,"},  // MA#* ( 🄉 → 8, ) DIGIT EIGHT COMMA → DIGIT EIGHT, COMMA#
    {127242, "9,"},  // MA#* ( 🄊 → 9, ) DIGIT NINE COMMA → DIGIT NINE, COMMA#
    {127248, "(A)"}, // MA#* ( 🄐 → (A) ) PARENTHESIZED LATIN CAPITAL LETTER A → LEFT PARENTHESIS, LATIN CAPITAL LETTER A, RIGHT PARENTHESIS#
    {127249, "(B)"}, // MA#* ( 🄑 → (B) ) PARENTHESIZED LATIN CAPITAL LETTER B → LEFT PARENTHESIS, LATIN CAPITAL LETTER B, RIGHT PARENTHESIS#
    {127250, "(C)"}, // MA#* ( 🄒 → (C) ) PARENTHESIZED LATIN CAPITAL LETTER C → LEFT PARENTHESIS, LATIN CAPITAL LETTER C, RIGHT PARENTHESIS#
    {127251, "(D)"}, // MA#* ( 🄓 → (D) ) PARENTHESIZED LATIN CAPITAL LETTER D → LEFT PARENTHESIS, LATIN CAPITAL LETTER D, RIGHT PARENTHESIS#
    {127252, "(E)"}, // MA#* ( 🄔 → (E) ) PARENTHESIZED LATIN CAPITAL LETTER E → LEFT PARENTHESIS, LATIN CAPITAL LETTER E, RIGHT PARENTHESIS#
    {127253, "(F)"}, // MA#* ( 🄕 → (F) ) PARENTHESIZED LATIN CAPITAL LETTER F → LEFT PARENTHESIS, LATIN CAPITAL LETTER F, RIGHT PARENTHESIS#
    {127254, "(G)"}, // MA#* ( 🄖 → (G) ) PARENTHESIZED LATIN CAPITAL LETTER G → LEFT PARENTHESIS, LATIN CAPITAL LETTER G, RIGHT PARENTHESIS#
    {127255, "(H)"}, // MA#* ( 🄗 → (H) ) PARENTHESIZED LATIN CAPITAL LETTER H → LEFT PARENTHESIS, LATIN CAPITAL LETTER H, RIGHT PARENTHESIS#
    {127256, "(l)"}, // MA#* ( 🄘 → (l) ) PARENTHESIZED LATIN CAPITAL LETTER I → LEFT PARENTHESIS, LATIN SMALL LETTER L, RIGHT PARENTHESIS# →(I)→
    {127257, "(J)"}, // MA#* ( 🄙 → (J) ) PARENTHESIZED LATIN CAPITAL LETTER J → LEFT PARENTHESIS, LATIN CAPITAL LETTER J, RIGHT PARENTHESIS#
    {127258, "(K)"}, // MA#* ( 🄚 → (K) ) PARENTHESIZED LATIN CAPITAL LETTER K → LEFT PARENTHESIS, LATIN CAPITAL LETTER K, RIGHT PARENTHESIS#
    {127259, "(L)"}, // MA#* ( 🄛 → (L) ) PARENTHESIZED LATIN CAPITAL LETTER L → LEFT PARENTHESIS, LATIN CAPITAL LETTER L, RIGHT PARENTHESIS#
    {127260, "(M)"}, // MA#* ( 🄜 → (M) ) PARENTHESIZED LATIN CAPITAL LETTER M → LEFT PARENTHESIS, LATIN CAPITAL LETTER M, RIGHT PARENTHESIS#
    {127261, "(N)"}, // MA#* ( 🄝 → (N) ) PARENTHESIZED LATIN CAPITAL LETTER N → LEFT PARENTHESIS, LATIN CAPITAL LETTER N, RIGHT PARENTHESIS#
    {127262, "(O)"}, // MA#* ( 🄞 → (O) ) PARENTHESIZED LATIN CAPITAL LETTER O → LEFT PARENTHESIS, LATIN CAPITAL LETTER O, RIGHT PARENTHESIS#
    {127263, "(P)"}, // MA#* ( 🄟 → (P) ) PARENTHESIZED LATIN CAPITAL LETTER P → LEFT PARENTHESIS, LATIN CAPITAL LETTER P, RIGHT PARENTHESIS#
    {127264, "(Q)"}, // MA#* ( 🄠 → (Q) ) PARENTHESIZED LATIN CAPITAL LETTER Q → LEFT PARENTHESIS, LATIN CAPITAL LETTER Q, RIGHT PARENTHESIS#
    {127265, "(R)"}, // MA#* ( 🄡 → (R) ) PARENTHESIZED LATIN CAPITAL LETTER R → LEFT PARENTHESIS, LATIN CAPITAL LETTER R, RIGHT PARENTHESIS#
    {127266, "(S)"}, // MA#* ( 🄢 → (S) ) PARENTHESIZED LATIN CAPITAL LETTER S → LEFT PARENTHESIS, LATIN CAPITAL LETTER S, RIGHT PARENTHESIS#
    {127267, "(T)"}, // MA#* ( 🄣 → (T) ) PARENTHESIZED LATIN CAPITAL LETTER T → LEFT PARENTHESIS, LATIN CAPITAL LETTER T, RIGHT PARENTHESIS#
    {127268, "(U)"}, // MA#* ( 🄤 → (U) ) PARENTHESIZED LATIN CAPITAL LETTER U → LEFT PARENTHESIS, LATIN CAPITAL LETTER U, RIGHT PARENTHESIS#
    {127269, "(V)"}, // MA#* ( 🄥 → (V) ) PARENTHESIZED LATIN CAPITAL LETTER V → LEFT PARENTHESIS, LATIN CAPITAL LETTER V, RIGHT PARENTHESIS#
    {127270, "(W)"}, // MA#* ( 🄦 → (W) ) PARENTHESIZED LATIN CAPITAL LETTER W → LEFT PARENTHESIS, LATIN CAPITAL LETTER W, RIGHT PARENTHESIS#
    {127271, "(X)"}, // MA#* ( 🄧 → (X) ) PARENTHESIZED LATIN CAPITAL LETTER X → LEFT PARENTHESIS, LATIN CAPITAL LETTER X, RIGHT PARENTHESIS#
    {127272, "(Y)"}, // MA#* ( 🄨 → (Y) ) PARENTHESIZED LATIN CAPITAL LETTER Y → LEFT PARENTHESIS, LATIN CAPITAL LETTER Y, RIGHT PARENTHESIS#
    {127273, "(Z)"}, // MA#* ( 🄩 → (Z) ) PARENTHESIZED LATIN CAPITAL LETTER Z → LEFT PARENTHESIS, LATIN CAPITAL LETTER Z, RIGHT PARENTHESIS#
    {127274, "(S)"}, // MA#* ( 🄪 → (S) ) TORTOISE SHELL BRACKETED LATIN CAPITAL LETTER S → LEFT PARENTHESIS, LATIN CAPITAL LETTER S, RIGHT PARENTHESIS# →〔S〕→
    {128768, "QE"},  // MA#* ( 🜀 → QE ) ALCHEMICAL SYMBOL FOR QUINTESSENCE → LATIN CAPITAL LETTER Q, LATIN CAPITAL LETTER E#
    {128775, "AR"},  // MA#* ( 🜇 → AR ) ALCHEMICAL SYMBOL FOR AQUA REGIA-2 → LATIN CAPITAL LETTER A, LATIN CAPITAL LETTER R#
    {128844, "C"},   // MA#* ( 🝌 → C ) ALCHEMICAL SYMBOL FOR CALX → LATIN CAPITAL LETTER C#
    {128860, "sss"}, // MA#* ( 🝜 → sss ) ALCHEMICAL SYMBOL FOR STRATUM SUPER STRATUM → LATIN SMALL LETTER S, LATIN SMALL LETTER S, LATIN SMALL LETTER S#
    {128872, "T"},   // MA#* ( 🝨 → T ) ALCHEMICAL SYMBOL FOR CRUCIBLE-4 → LATIN CAPITAL LETTER T#
    {128875, "MB"},  // MA#* ( 🝫 → MB ) ALCHEMICAL SYMBOL FOR BATH OF MARY → LATIN CAPITAL LETTER M, LATIN CAPITAL LETTER B#
    {128876, "VB"},  // MA#* ( 🝬 → VB ) ALCHEMICAL SYMBOL FOR BATH OF VAPOURS → LATIN CAPITAL LETTER V, LATIN CAPITAL LETTER B#
};
// clang-format on

const char* findConfusable(uint32_t codepoint)
{
    auto it = std::lower_bound(
        std::begin(kConfusables),
        std::end(kConfusables),
        codepoint,
        [](const Confusable& lhs, uint32_t rhs)
        {
            return lhs.codepoint < rhs;
        }
    );

    return (it != std::end(kConfusables) && it->codepoint == codepoint) ? it->text : nullptr;
}

} // namespace Luau
