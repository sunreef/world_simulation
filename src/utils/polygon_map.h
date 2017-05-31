#pragma once

#include "polyhedron.h"
#include "delaunay.h"

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

	Delaunay P;

	cv::Mat m_map;

public:
	PolygonMap(int width, int height);
	~PolygonMap();

	void drawMap();

};
