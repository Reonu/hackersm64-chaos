void scroll_gfx_mat_sl_dl_f3dlite_material_001_layer5() {
	Gfx *mat = segmented_to_virtual(mat_sl_dl_f3dlite_material_001_layer5);


	shift_s_down(mat, 13, PACK_TILESIZE(0, 1));
	shift_s(mat, 15, PACK_TILESIZE(0, 1));
	shift_t(mat, 15, PACK_TILESIZE(0, 1));

};

void scroll_sl() {
	scroll_gfx_mat_sl_dl_f3dlite_material_001_layer5();
};
