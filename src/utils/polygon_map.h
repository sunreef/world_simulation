#pragma once

#include "polyhedron.h"

#include <vector>
#include <random>
#include <map>
#include <iostream>
#include <queue>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/shape.hpp>

class PolygonMap {
private:
	std::mt19937 RNG;
	std::uniform_int_distribution<int> m_distX, m_distY;

	std::unique_ptr<Polyhedron> P;

	void insertVertex(Vertex v);

	void createVertices(int number_of_vertices);
	void computeDelaunay();

	cv::Mat m_map;

public:
	PolygonMap(int width, int height);
	~PolygonMap();

	void drawMap();

};
