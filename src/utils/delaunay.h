#pragma once

#include <queue>

#include "polyhedron.h"

class Delaunay : public Polyhedron
{

	void insertVertex(Vertex& v);

public:
	Delaunay();
	~Delaunay();

	Delaunay(std::vector<Vertex>& vertices);
};

