void scroll_gfx_mat_bbh_dl_water_layer5() {
	Gfx *mat = segmented_to_virtual(mat_bbh_dl_water_layer5);


	shift_s_down(mat, 20, PACK_TILESIZE(0, 1));
	shift_s(mat, 22, PACK_TILESIZE(0, 1));
	shift_t(mat, 22, PACK_TILESIZE(0, 1));

};

void scroll_bbh() {
	scroll_gfx_mat_bbh_dl_water_layer5();
};
