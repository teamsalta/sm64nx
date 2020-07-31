// 0x0C000224
const GeoLayout boo_geo[] = {
   GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x96, 70),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 26214),
      GEO_OPEN_NODE(),
         GEO_ASM(0, AlphaControl),
         GEO_SWITCH_CASE(2, ControlShapeAnime),
         GEO_OPEN_NODE(),
            GEO_DISPLAY_LIST(LAYER_OPAQUE, boo_seg5_dl_0500C1B0),
            GEO_DISPLAY_LIST(LAYER_TRANSPARENT, boo_seg5_dl_0500C1B0),
         GEO_CLOSE_NODE(),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
GEO_CLOSE_NODE(), //! more close than open nodes
GEO_END(),
};
