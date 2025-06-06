void scroll_CHAO_GARDEN_dl_a_water_visual_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 30;
	int width = 64 * 0x20;
	int height = 64 * 0x20;

	static int currentX = 0;
	int deltaX;
	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(CHAO_GARDEN_dl_a_water_visual_mesh_layer_5_vtx_0);

	deltaX = (int)(0.05000000074505806 * 0x20) % width;
	deltaY = (int)(0.05000000074505806 * 0x20) % height;

	if (absi(currentX) > width) {
		deltaX -= (int)(absi(currentX) / width) * width * signum_positive(deltaX);
	}
	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[0] += deltaX;
		vertices[i].n.tc[1] += deltaY;
	}
	currentX += deltaX;	currentY += deltaY;
}

void scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_7() {
	int i = 0;
	int count = 91;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_7);

	deltaY = (int)(-10.0 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_5_vtx_0() {
	int i = 0;
	int count = 53;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(CHAO_GARDEN_dl_Chao_garden_mesh_layer_5_vtx_0);

	deltaY = (int)(-0.10000000149011612 * 0x20) % height;

	if (absi(currentY) > height) {
		deltaY -= (int)(absi(currentY) / height) * height * signum_positive(deltaY);
	}

	for (i = 0; i < count; i++) {
		vertices[i].n.tc[1] += deltaY;
	}
	currentY += deltaY;
}

void scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_5_vtx_1() {
	int i = 0;
	int count = 47;
	int height = 64 * 0x20;

	static int currentY = 0;
	int deltaY;
	Vtx *vertices = segmented_to_virtual(CHAO_GARDEN_dl_Chao_garden_mesh_layer_5_vtx_1);

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
	scroll_CHAO_GARDEN_dl_a_water_visual_mesh_layer_5_vtx_0();
	scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_1_vtx_7();
	scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_5_vtx_0();
	scroll_CHAO_GARDEN_dl_Chao_garden_mesh_layer_5_vtx_1();
};
