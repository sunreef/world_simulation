#include "polygon_map.h"

PolygonMap::PolygonMap(int width, int height)
{
	RNG = std::mt19937(std::random_device()());

	m_distX = std::uniform_int_distribution<int>(0, width);
	m_distY = std::uniform_int_distribution<int>(0, height);

	m_map = cv::Mat(height, width, CV_64FC3);

	Vertex v1;
	v1.x = -1;
	v1.y = -1;
	v1.z = 0;

	Vertex v2;
	v2.x = -1;
	v2.y = 2 * height + 1;
	v2.z = 0;

	Vertex v3;
	v3.x = 2 * width + 1;
	v3.y = -1;
	v3.z = 0;

    std::vector<Vertex> vertices;
    for(int i = 0; i < 1; i++) {
        Vertex v;
        v.x = m_distX(RNG) / 2;
        v.y = m_distY(RNG)/2;
        v.z = 0;

        vertices.push_back(v);
    }

    P = Delaunay(vertices);
}

PolygonMap::~PolygonMap()
{
}

void PolygonMap::drawMap()
{
	m_map = cv::Mat(m_map.rows, m_map.cols, CV_64FC3);
	for (auto it = P.facet_begin(); it != P.facet_end(); it++) {
		HalfEdge* faceEdge = (*it)->edge;
		HalfEdge* currentEdge = faceEdge;
        std::cout << currentEdge << std::endl;
		Vertex* currentVertex = currentEdge->target;
        std::cout << "kfjshk" << std::endl;
		Circle c = (*it)->getCircumcircle();
		//cv::circle(m_map, cv::Point(c.c.x, c.c.y), c.radius, cv::Scalar(0, 0, 1));

		do {
			currentEdge = currentEdge->next;
			Vertex* newTarget = currentEdge->target;

			cv::line(m_map, cv::Point(currentVertex->x, currentVertex->y), cv::Point(newTarget->x, newTarget->y), cv::Scalar(1, 0, 0));

			currentVertex = newTarget;
		} while (currentEdge != faceEdge);
	}
	cv::imshow("Polygon Map", m_map);
	cv::waitKey();
}
