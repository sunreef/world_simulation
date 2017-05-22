#include <iostream>

#include "world.h"

#include"utils/polygon_map.h"

int main(int argc, char** argv) {
	PolygonMap m(400, 400);
	m.drawMap();

	World w;

	for (int i = 0; i < 1000; i++) {
		w.update();
		w.showWorldState();
	}
	return 0;
}