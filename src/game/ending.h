#pragma once

const char* credits01[] = {"1GAME DIRECTOR", "SHIGERU MIYAMOTO"};
const char* credits02[] = {"2ASSISTANT DIRECTORS", "YOSHIAKI KOIZUMI", "TAKASHI TEZUKA"};
const char* credits03[] = {"2SYSTEM PROGRAMMERS", "YASUNARI NISHIDA", "YOSHINORI TANIMOTO"};
const char* credits04[] = {"3PROGRAMMERS", "HAJIME YAJIMA", "DAIKI IWAMOTO", "TOSHIO IWAWAKI"};
const char* credits05[] = {"4CAMERA PROGRAMMER", "MARIO FACE PROGRAMMER", "TAKUMI KAWAGOE", "GILES GODDARD"}; // US combines camera programmer and mario face programmer
const char* credits06[] = {"2COURSE DIRECTORS", "YOICHI YAMADA", "YASUHISA YAMAMURA"};
const char* credits07[] = {"2COURSE DESIGNERS", "KENTA USUI", "NAOKI MORI"};
const char* credits08[] = {"3COURSE DESIGNERS", "YOSHIKI HARUHANA", "MAKOTO MIYANAGA", "KATSUHIKO KANNO"};
const char* credits09[] = {"1SOUND COMPOSER", "KOJI KONDO"};
const char* credits10[] = {"4SOUND EFFECTS", "SOUND PROGRAMMER", "YOJI INAGAKI", "HIDEAKI SHIMIZU"}; // as well as sound effects and sound programmer
const char* credits11[] = {"23-D ANIMATORS", "YOSHIAKI KOIZUMI", "SATORU TAKIZAWA"};
const char* credits12[] = {"1ADDITIONAL GRAPHICS", "MASANAO ARIMOTO"};
const char* credits13[] = {"3TECHNICAL SUPPORT", "TAKAO SAWANO", "HIROHITO YOSHIMOTO", "HIROTO YADA"};
const char* credits14[] = {"1TECHNICAL SUPPORT", "SGI N64 PROJECT STAFF"};
const char* credits15[] = {"2PROGRESS MANAGEMENT", "KIMIYOSHI FUKUI", "KEIZO KATO"};
const char* credits16[] = {"5SCREEN TEXT WRITER", "TRANSLATION", "LESLIE SWAN", "MINA AKINO", "HIRO YAMADA"}; // ...in order to make room for these 2 new lines
const char* credits17[] = {"4MARIO VOICE", "PEACH VOICE", "CHARLES MARTINET", "LESLIE SWAN"};
const char* credits18[] = {"3SPECIAL THANKS TO", "EAD STAFF", "ALL NINTENDO PERSONNEL", "MARIO CLUB STAFF"};
const char* credits19[] = {"1PRODUCER", "SHIGERU MIYAMOTO"};
const char* credits20[] = {"1EXECUTIVE PRODUCER", "HIROSHI YAMAUCHI"};

struct CreditsEntry sCreditsSequence[] = {
    {LEVEL_CASTLE_GROUNDS, 1, 1, -128, {0, 8000, 0}, NULL},
    {LEVEL_BOB, 1, 1, 117, {713, 3918, -3889}, credits01},
    {LEVEL_WF, 1, 50, 46, {347, 5376, 326}, credits02},
    {LEVEL_JRB, 1, 18, 22, {3800, -4840, 2727}, credits03},
    {LEVEL_CCM, 2, 34, 25, {-5464, 6656, -6575}, credits04},
    {LEVEL_BBH, 1, 1, 60, {257, 1922, 2580}, credits05},
    {LEVEL_HMC, 1, -15, 123, {-6469, 1616, -6054}, credits06},
    {LEVEL_THI, 3, 17, -32, {508, 1024, 1942}, credits07},
    {LEVEL_LLL, 2, 33, 124, {-73, 82, -1467}, credits08},
    {LEVEL_SSL, 1, 65, 98, {-5906, 1024, -2576}, credits09},
    {LEVEL_DDD, 1, 50, 47, {-4884, -4607, -272}, credits10},
    {LEVEL_SL, 1, 17, -34, {1925, 3328, 563}, credits11},
    {LEVEL_WDW, 1, 33, 105, {-537, 1850, 1818}, credits12},
    {LEVEL_TTM, 1, 2, -33, {2613, 313, 1074}, credits13},
    {LEVEL_THI, 1, 51, 54, {-2609, 512, 856}, credits14},
    {LEVEL_TTC, 1, 17, -72, {-1304, -71, -967}, credits15},
    {LEVEL_RR, 1, 33, 64, {1565, 1024, -148}, credits16},
    {LEVEL_SA, 1, 1, 24, {-1050, -1330, -1559}, credits17},
    {LEVEL_COTMC, 1, 49, -16, {-254, 415, -6045}, credits18},
    {LEVEL_DDD, 2, -111, -64, {3948, 1185, -104}, credits19},
    {LEVEL_CCM, 1, 33, 31, {3169, -4607, 5240}, credits20},
    {LEVEL_CASTLE_GROUNDS, 1, 1, -128, {0, 906, -1200}, NULL},
    {0, 0, 1, 0, {0, 0, 0}, NULL},
};
