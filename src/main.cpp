#include <iostream>

#include "world.h"

#include"utils/polygon_map.h"
#include "utils/polyhedron.h"

int main(int argc, char** argv) {

	PolygonMap m(800, 600);
	m.drawMap();

	World w;

	for (int i = 0; i < 1000; i++) {
		w.update();
		w.showWorldState();
	}
	return 0;
}