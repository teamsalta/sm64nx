// 0x16000B10
const GeoLayout red_flame_shadow_geo[] = {
   GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x50, 20),
   GEO_OPEN_NODE(),
      GEO_BRANCH_AND_LINK(red_flame_geo),
   GEO_CLOSE_NODE(),
   GEO_END(),
};

// 0x16000B2C
const GeoLayout red_flame_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_SWITCH_CASE(8, ControlShapeAnime),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3B0),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3C8),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3E0),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B3F8),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B410),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B428),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B440),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B458),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};

// 0x16000B8C
const GeoLayout blue_flame_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_SWITCH_CASE(8, ControlShapeAnime),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B500),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B518),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B530),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B548),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B560),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B578),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B590),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, flame_seg3_dl_0301B5A8),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
