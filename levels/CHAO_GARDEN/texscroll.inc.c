void scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_7() {
	int i = 0;
	int count = 53;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_7);

	deltaY = (int)(-0.10000000149011612 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_8() {
	int i = 0;
	int count = 76;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_8);

	deltaY = (int)(-10.0 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_11() {
	int i = 0;
	int count = 47;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_11);

	deltaY = (int)(-0.10000000149011612 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_CHAO_GARDEN() {
	scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_7();
	scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_8();
	scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_11();
};
