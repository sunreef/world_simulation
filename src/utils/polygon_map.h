#pragma once

#include <vector>
#include <random>
#include <map>
#include <iostream>
#include <queue>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/shape.hpp>

typedef std::pair<double, double> Coord;

struct Face {
	int v[3];
	int f[3];

	Face(int vertices[3], int faces[3]) {
		for (int i = 0; i < 3; i++) {
			v[i] = vertices[i];
			f[i] = faces[i];
		}
	}
};

class PolygonMap {
private:
	std::mt19937 RNG;
	std::uniform_int_distribution<int> m_distX, m_distY;


	std::vector<Coord> m_vertices;
	std::vector<Face> m_faces;
	std::vector<std::vector<int> > m_splitGraph;
	std::vector<int> m_graphParent;

	bool isPointInside(const Coord& c, const Face& f);
	bool isPointInCircle(const Coord& d, const Face& f);
	void splitFace(int f, int v);
	bool flipEdge(int f1, int f2);
	void insertVertex(Coord& c);


	void createVertices(int number_of_vertices);

	void computeDelaunay();

	cv::Mat m_map;

public:
	PolygonMap(int width, int height);
	~PolygonMap();

	void drawMap();

};
