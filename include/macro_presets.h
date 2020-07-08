#ifndef _MACRO_PRESETS_H
#define _MACRO_PRESETS_H

#include "macro_preset_names.h"
#include "behavior_data.h"
#include "model_ids.h"

struct MacroPreset
{
    /*0x00*/ const BehaviorScript *beh;
    /*0x04*/ s16 model;
    /*0x06*/ s16 param;
};

struct MacroPreset MacroObjectPresets[] = {
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvOneCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvMovingBlueCoin(), MODEL_BLUE_COIN, 0},
    {sm64::bhv::bhvBlueCoinSliding(), MODEL_BLUE_COIN, 0}, // unused
    {sm64::bhv::bhvRedCoin(), MODEL_RED_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, 0},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, COIN_FORMATION_FLAG_RING},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, COIN_FORMATION_FLAG_ARROW},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, COIN_FORMATION_FLAG_FLYING},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, COIN_FORMATION_FLAG_FLYING | COIN_FORMATION_FLAG_VERTICAL},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, COIN_FORMATION_FLAG_FLYING | COIN_FORMATION_FLAG_RING},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, COIN_FORMATION_FLAG_FLYING | COIN_FORMATION_FLAG_RING | COIN_FORMATION_FLAG_VERTICAL},
    {sm64::bhv::bhvCoinFormation(), MODEL_NONE, COIN_FORMATION_FLAG_FLYING | COIN_FORMATION_FLAG_ARROW}, // unused
    {sm64::bhv::bhvHiddenStarTrigger(), MODEL_NONE, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvUnusedFakeStar(), MODEL_STAR, 0}, // unused
    {sm64::bhv::bhvMessagePanel(), MODEL_WOODEN_SIGNPOST, 0},
    {sm64::bhv::bhvCannonClosed(), MODEL_DL_CANNON_LID, 0},
    {sm64::bhv::bhvBobombBuddyOpensCannon(), MODEL_BOBOMB_BUDDY, 0},
    {sm64::bhv::bhvButterfly(), MODEL_BUTTERFLY, 0}, // unused
    {sm64::bhv::bhvBouncingFireball(), MODEL_NONE, 0}, // unused
    {sm64::bhv::bhvLargeFishGroup(), MODEL_NONE, 0}, // unused
    {sm64::bhv::bhvLargeFishGroup(), MODEL_NONE, 1},
    {sm64::bhv::bhvBetaFishSplashSpawner(), MODEL_NONE, 0},
    {sm64::bhv::bhvHidden1upInPoleSpawner(), MODEL_NONE, 0},
    {sm64::bhv::bhvGoomba(), MODEL_GOOMBA, 1},
    {sm64::bhv::bhvGoomba(), MODEL_GOOMBA, 2},
    {sm64::bhv::bhvGoombaTripletSpawner(), MODEL_NONE, 0},
    {sm64::bhv::bhvGoombaTripletSpawner(), MODEL_NONE, 8}, // unused
    {sm64::bhv::bhvSignOnWall(), MODEL_NONE, 0},
    {sm64::bhv::bhvChuckya(), MODEL_CHUCKYA, 0},
    {sm64::bhv::bhvCannon(), MODEL_CANNON_BASE, 0},
    {sm64::bhv::bhvGoomba(), MODEL_GOOMBA, 0},
    {sm64::bhv::bhvHomingAmp(), MODEL_AMP, 0},
    {sm64::bhv::bhvCirclingAmp(), MODEL_AMP, 0},
    {sm64::bhv::bhvCarrySomething1(), MODEL_UNKNOWN_7D, 0}, // unused
    {sm64::bhv::bhvBetaTrampolineTop(), MODEL_TRAMPOLINE, 0}, // unused
    {sm64::bhv::bhvFreeBowlingBall(), MODEL_BOWLING_BALL, 0}, // unused
    {sm64::bhv::bhvSnufit(), MODEL_SNUFIT, 0},
    {sm64::bhv::bhvRecoveryHeart(), MODEL_HEART, 0},
    {sm64::bhv::bhv1upSliding(), MODEL_1UP, 0},
    {sm64::bhv::bhv1Up(), MODEL_1UP, 0},
    {sm64::bhv::bhv1upJumpOnApproach(), MODEL_1UP, 0}, // unused
    {sm64::bhv::bhvHidden1up(), MODEL_1UP, 0},
    {sm64::bhv::bhvHidden1upTrigger(), MODEL_NONE, 0},
    {sm64::bhv::bhv1Up(), MODEL_1UP, 1},
    {sm64::bhv::bhv1Up(), MODEL_1UP, 2},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvBlueCoinSwitch(), MODEL_BLUE_COIN_SWITCH, 0},
    {sm64::bhv::bhvHiddenBlueCoin(), MODEL_BLUE_COIN, 0},
    {sm64::bhv::bhvCapSwitch(), MODEL_CAP_SWITCH, 0}, // unused
    {sm64::bhv::bhvCapSwitch(), MODEL_CAP_SWITCH, 1}, // unused
    {sm64::bhv::bhvCapSwitch(), MODEL_CAP_SWITCH, 2}, // unused
    {sm64::bhv::bhvCapSwitch(), MODEL_CAP_SWITCH, 3}, // unused
    {sm64::bhv::bhvWaterLevelDiamond(), MODEL_BREAKABLE_BOX, 0}, // unused
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 0},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 1},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 2},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 3},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 4}, // unused
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 5},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 6},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 7},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 8},
    {sm64::bhv::bhvBreakableBox(), MODEL_BREAKABLE_BOX, 0},
    {sm64::bhv::bhvBreakableBox(), MODEL_BREAKABLE_BOX, 1},
    {sm64::bhv::bhvPushableMetalBox(), MODEL_METAL_BOX, 0},
    {sm64::bhv::bhvBreakableBoxSmall(), MODEL_BREAKABLE_BOX_SMALL, 0},
    {sm64::bhv::bhvFloorSwitchHiddenObjects(), MODEL_PURPLE_SWITCH, 0},
    {sm64::bhv::bhvHiddenObject(), MODEL_BREAKABLE_BOX, 0},
    {sm64::bhv::bhvHiddenObject(), MODEL_BREAKABLE_BOX, 1}, // unused
    {sm64::bhv::bhvHiddenObject(), MODEL_BREAKABLE_BOX, 2}, // unused
    {sm64::bhv::bhvBreakableBox(), MODEL_BREAKABLE_BOX, 3},
    {sm64::bhv::bhvKoopaShellUnderwater(), MODEL_KOOPA_SHELL, 0},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 9},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvBulletBill(), MODEL_BULLET_BILL, 0}, // unused
    {sm64::bhv::bhvHeaveHo(), MODEL_HEAVE_HO, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvThwomp2(), MODEL_THWOMP, 0}, // unused
    {sm64::bhv::bhvFireSpitter(), MODEL_BOWLING_BALL, 0},
    {sm64::bhv::bhvFlyGuy(), MODEL_FLYGUY, 1},
    {sm64::bhv::bhvJumpingBox(), MODEL_BREAKABLE_BOX, 0},
    {sm64::bhv::bhvTripletButterfly(), MODEL_BUTTERFLY, 0},
    {sm64::bhv::bhvTripletButterfly(), MODEL_BUTTERFLY, 4},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvSmallBully(), MODEL_BULLY, 0},
    {sm64::bhv::bhvSmallBully(), MODEL_BULLY_BOSS, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvStub1D0C(), MODEL_UNKNOWN_58, 0}, // unused
    {sm64::bhv::bhvBouncingFireball(), MODEL_NONE, 0},
    {sm64::bhv::bhvFlamethrower(), MODEL_NONE, 4},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvWoodenPost(), MODEL_WOODEN_POST, 0},
    {sm64::bhv::bhvWaterBombSpawner(), MODEL_NONE, 0},
    {sm64::bhv::bhvEnemyLakitu(), MODEL_ENEMY_LAKITU, 0},
    {sm64::bhv::bhvKoopa(), MODEL_KOOPA_WITH_SHELL, 2}, // unused
    {sm64::bhv::bhvKoopaRaceEndpoint(), MODEL_NONE, 0}, // unused
    {sm64::bhv::bhvBobomb(), MODEL_BLACK_BOBOMB, 0},
    {sm64::bhv::bhvWaterBombCannon(), MODEL_CANNON_BASE, 0}, // unused
    {sm64::bhv::bhvBobombBuddyOpensCannon(), MODEL_BOBOMB_BUDDY, 0}, // unused
    {sm64::bhv::bhvWaterBombCannon(), MODEL_CANNON_BASE, 0},
    {sm64::bhv::bhvBobomb(), MODEL_BLACK_BOBOMB, 1},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvUnusedFakeStar(), MODEL_UNKNOWN_54, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvUnagi(), MODEL_UNAGI, 0}, // unused
    {sm64::bhv::bhvSushiShark(), MODEL_SUSHI, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvStaticObject(), MODEL_KLEPTO, 0}, // unused
    {sm64::bhv::bhvTornado(), MODEL_TORNADO, 0}, // unused
    {sm64::bhv::bhvPokey(), MODEL_NONE, 0},
    {sm64::bhv::bhvPokey(), MODEL_NONE, 0}, // unused
    {sm64::bhv::bhvToxBox(), MODEL_SSL_TOX_BOX, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvMontyMole(), MODEL_MONTY_MOLE, 0}, // unused
    {sm64::bhv::bhvMontyMole(), MODEL_MONTY_MOLE, 1},
    {sm64::bhv::bhvMontyMoleHole(), MODEL_DL_MONTY_MOLE_HOLE, 0},
    {sm64::bhv::bhvFlyGuy(), MODEL_FLYGUY, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvWigglerHead(), MODEL_WIGGLER_HEAD, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvSpindrift(), MODEL_SPINDRIFT, 0},
    {sm64::bhv::bhvMrBlizzard(), MODEL_MR_BLIZZARD_HIDDEN, 0},
    {sm64::bhv::bhvMrBlizzard(), MODEL_MR_BLIZZARD_HIDDEN, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvSmallPenguin(), MODEL_PENGUIN, 0}, // unused
    {sm64::bhv::bhvTuxiesMother(), MODEL_PENGUIN, 0}, // unused
    {sm64::bhv::bhvTuxiesMother(), MODEL_PENGUIN, 0}, // unused
    {sm64::bhv::bhvMrBlizzard(), MODEL_MR_BLIZZARD_HIDDEN, 1}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvHauntedChair(), MODEL_HAUNTED_CHAIR, 0}, // unused
    {sm64::bhv::bhvHauntedChair(), MODEL_HAUNTED_CHAIR, 0},
    {sm64::bhv::bhvHauntedChair(), MODEL_HAUNTED_CHAIR, 0}, // unused
    {sm64::bhv::bhvGhostHuntBoo(), MODEL_BOO, 0}, // unused
    {sm64::bhv::bhvGhostHuntBoo(), MODEL_BOO, 0}, // unused
    {sm64::bhv::bhvCourtyardBooTriplet(), MODEL_BOO, 0}, // unused
    {sm64::bhv::bhvBooWithCage(), MODEL_BOO, 0}, // unused
    {sm64::bhv::bhvAlphaBooKey(), MODEL_BETA_BOO_KEY, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvChirpChirp(), MODEL_NONE, 0},
    {sm64::bhv::bhvSeaweedBundle(), MODEL_NONE, 0},
    {sm64::bhv::bhvBetaChestBottom(), MODEL_TREASURE_CHEST_BASE, 0}, // unused
    {sm64::bhv::bhvBowserBomb(), MODEL_WATER_MINE, 0}, // unused
    {sm64::bhv::bhvLargeFishGroup(), MODEL_NONE, 2}, // unused
    {sm64::bhv::bhvLargeFishGroup(), MODEL_NONE, 3},
    {sm64::bhv::bhvJetStreamRingSpawner(), MODEL_WATER_RING, 0}, // unused
    {sm64::bhv::bhvJetStreamRingSpawner(), MODEL_WATER_RING, 0}, // unused
    {sm64::bhv::bhvSkeeter(), MODEL_SKEETER, 0},
    {sm64::bhv::bhvClamShell(), MODEL_CLAM_SHELL, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvMacroUkiki(), MODEL_UKIKI, 0}, // unused
    {sm64::bhv::bhvMacroUkiki(), MODEL_UKIKI, 1}, // unused
    {sm64::bhv::bhvPiranhaPlant(), MODEL_PIRANHA_PLANT, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvSmallWhomp(), MODEL_WHOMP, 0},
    {sm64::bhv::bhvChainChomp(), MODEL_CHAIN_CHOMP, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvKoopa(), MODEL_KOOPA_WITH_SHELL, 1},
    {sm64::bhv::bhvKoopa(), MODEL_KOOPA_WITHOUT_SHELL, 0}, // unused
    {sm64::bhv::bhvWoodenPost(), MODEL_WOODEN_POST, 0}, // unused
    {sm64::bhv::bhvFirePiranhaPlant(), MODEL_PIRANHA_PLANT, 0},
    {sm64::bhv::bhvFirePiranhaPlant(), MODEL_PIRANHA_PLANT, 1}, // unused
    {sm64::bhv::bhvKoopa(), MODEL_KOOPA_WITH_SHELL, 4},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvMoneybagHidden(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvSwoop(), MODEL_SWOOP, 0},
    {sm64::bhv::bhvSwoop(), MODEL_SWOOP, 1},
    {sm64::bhv::bhvMrI(), MODEL_NONE, 0},
    {sm64::bhv::bhvScuttlebugSpawn(), MODEL_NONE, 0},
    {sm64::bhv::bhvScuttlebug(), MODEL_SCUTTLEBUG, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_UNKNOWN_54, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvTTCRotatingSolid(), MODEL_TTC_ROTATING_CUBE, 0},
    {sm64::bhv::bhvTTCRotatingSolid(), MODEL_TTC_ROTATING_PRISM, 1},
    {sm64::bhv::bhvTTCPendulum(), MODEL_TTC_PENDULUM, 0},
    {sm64::bhv::bhvTTCTreadmill(), MODEL_TTC_LARGE_TREADMILL, 0},
    {sm64::bhv::bhvTTCTreadmill(), MODEL_TTC_SMALL_TREADMILL, 1},
    {sm64::bhv::bhvTTCMovingBar(), MODEL_TTC_PUSH_BLOCK, 0},
    {sm64::bhv::bhvTTCCog(), MODEL_TTC_ROTATING_HEXAGON, 0},
    {sm64::bhv::bhvTTCCog(), MODEL_TTC_ROTATING_TRIANGLE, 2},
    {sm64::bhv::bhvTTCPitBlock(), MODEL_TTC_PIT_BLOCK, 0},
    {sm64::bhv::bhvTTCPitBlock(), MODEL_TTC_PIT_BLOCK_UNUSED, 1}, // unused
    {sm64::bhv::bhvTTCElevator(), MODEL_TTC_ELEVATOR_PLATFORM, 0},
    {sm64::bhv::bhvTTC2DRotator(), MODEL_TTC_CLOCK_HAND, 0},
    {sm64::bhv::bhvTTCSpinner(), MODEL_TTC_SPINNER, 0},
    {sm64::bhv::bhvTTC2DRotator(), MODEL_TTC_SMALL_GEAR, 1},
    {sm64::bhv::bhvTTC2DRotator(), MODEL_TTC_LARGE_GEAR, 1},
    {sm64::bhv::bhvTTCTreadmill(), MODEL_TTC_LARGE_TREADMILL, 2},
    {sm64::bhv::bhvTTCTreadmill(), MODEL_TTC_SMALL_TREADMILL, 3},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 10},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 11},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 12},
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 13}, // unused
    {sm64::bhv::bhvExclamationBox(), MODEL_EXCLAMATION_BOX, 14},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvSlidingPlatform2(), MODEL_BITS_SLIDING_PLATFORM, 0}, // unused
    {sm64::bhv::bhvSlidingPlatform2(), MODEL_BITS_TWIN_SLIDING_PLATFORMS, 0}, // unused
    {sm64::bhv::bhvAnotherTiltingPlatform(), MODEL_BITDW_SLIDING_PLATFORM, 0}, // unused
    {sm64::bhv::bhvOctagonalPlatformRotating(), MODEL_BITS_OCTAGONAL_PLATFORM, 0}, // unused
    {sm64::bhv::bhvAnimatesOnFloorSwitchPress(), MODEL_BITS_STAIRCASE, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvFerrisWheelAxle(), MODEL_BITS_FERRIS_WHEEL_AXLE, 0}, // unused
    {sm64::bhv::bhvActivatedBackAndForthPlatform(), MODEL_BITS_ARROW_PLATFORM, 0}, // unused
    {sm64::bhv::bhvSeesawPlatform(), MODEL_BITS_SEESAW_PLATFORM, 0}, // unused
    {sm64::bhv::bhvSeesawPlatform(), MODEL_BITS_TILTING_W_PLATFORM, 0}, // unused
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
    {sm64::bhv::bhvYellowCoin(), MODEL_YELLOW_COIN, 0},
	{sm64::bhv::bhvShyGuy(), MODEL_SHYGUY, 0}
};

#endif // _MACRO_PRESETS_H
