// floating_box.c.inc

void s_wd_kibako_main(void)
{
	o->oPosY = o->oHomeY + sins(o->oTimer * 0x400 / FRAME_RATE_SCALER_INV) * 10.0f;
}
