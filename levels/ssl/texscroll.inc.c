void scroll_gfx_mat_ssl_dl_snowy_mist_layer5() {
	Gfx *mat = segmented_to_virtual(mat_ssl_dl_snowy_mist_layer5);


	shift_s_down(mat, 12, PACK_TILESIZE(0, 1));
	shift_s(mat, 14, PACK_TILESIZE(0, 1));
	shift_t(mat, 14, PACK_TILESIZE(0, 1));

};

void scroll_ssl() {
	scroll_gfx_mat_ssl_dl_snowy_mist_layer5();
};
