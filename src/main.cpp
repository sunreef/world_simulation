#include <iostream>

#include "world.h"

#include"utils/polygon_map.h"
#include "utils/polyhedron.h"

int main(int argc, char** argv) {

	Vertex v1; 
	v1.x = 0;
	v1.y = 0;
	v1.z = 0;

	Vertex v2;
	v2.x = 0;
	v2.y = 100;
	v2.z = 0;

	Vertex v3;
	v3.x = 100;
	v3.y = 0;
	v3.z = 0;

	Vertex v4;
	v4.x = 10;
	v4.y = 10;
	v4.z = 0;

	std::unique_ptr<Polyhedron> P = Polyhedron::createTriangle(v1, v2, v3);

	P->splitFacet(v4, P->getFacet(0));

	std::cout << P->toString() << std::endl;

	PolygonMap m(400, 400);
	m.drawMap();

	World w;

	for (int i = 0; i < 1000; i++) {
		w.update();
		w.showWorldState();
	}
	return 0;
}