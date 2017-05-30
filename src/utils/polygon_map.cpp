#include "polygon_map.h"


void PolygonMap::insertVertex(Vertex v)
{
	Facet* f;
	bool allocated = false;
	for (auto it = P->facet_begin(); it != P->facet_end(); it++) {
		if ((*it)->isInside(v)) {
			allocated = true;
			f = *it;
			break;
		}
	}
	if (!allocated) {
		std::cout << "Not inside a face: " << v.toString() << std::endl;
	}
	P->splitFacet(v, f);

	std::queue<HalfEdge*> toDo;

	Facet* currentFacet = f;
	HalfEdge* currentEdge = f->edge;
	do {
		toDo.push(currentEdge);
		currentEdge = currentEdge->next->opposite->next;
	} while (currentEdge != f->edge);

	while (!toDo.empty()) {
		HalfEdge* edge = toDo.front();
		toDo.pop();
		if (edge->opposite == nullptr) {
			continue;
		}
		Circle c = edge->face->getCircumcircle();

		if (c.isInside(edge->opposite->next->target)) {
			//std::cout << edge->next->target->toString() << " " << c.toString() << std::endl;
			toDo.push(edge->opposite->next);
			toDo.push(edge->opposite->next->next);

			P->flipEdge(edge);
		}
	}

	//for (auto it = P->facet_begin(); it != P->facet_end(); it++) {
	//	Circle c = (*it)->getCircumcircle();
	//	try {

	//		if ((*it)->edge->opposite != nullptr) {

	//			Vertex* v1 = (*it)->edge->opposite->next->target;
	//			if (c.isInside(v1)) std::cout << "bad triangulation" << std::endl;
	//		}
	//		Vertex* v2 = (*it)->edge->next->opposite->next->target;
	//		Vertex* v3 = (*it)->edge->next->next->opposite->next->target;
	//	}
	//	catch (std::exception e) {

	//	}
	//}
}

void PolygonMap::createVertices(int number_of_vertices)
{
	for (int i = 0; i < number_of_vertices; i++) {
		Vertex v;
		v.z = 0;
		v.x = m_distX(RNG);
		v.y = m_distY(RNG);

		insertVertex(v);
	}
}

void PolygonMap::computeDelaunay()
{

}

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

	P = Polyhedron::createTriangle(v1, v2, v3);

	createVertices(1000);
}

PolygonMap::~PolygonMap()
{
}

void PolygonMap::drawMap()
{
	m_map = cv::Mat(m_map.rows, m_map.cols, CV_64FC3);
	for (auto it = P->facet_begin(); it != P->facet_end(); it++) {
		HalfEdge* faceEdge = (*it)->edge;
		HalfEdge* currentEdge = faceEdge;
		Vertex* currentVertex = currentEdge->target;

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
