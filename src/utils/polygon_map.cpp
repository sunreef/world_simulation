#include "polygon_map.h"

bool PolygonMap::isPointInside(const Coord & c, const Face & f)
{
	double value = 0;
	for (int i = 0; i < 3; i++) {
		Coord v1 = m_vertices[f.v[i]];
		Coord v2 = m_vertices[f.v[(i + 1) % 3]];
		double crossProduct = 0;
		crossProduct += (v2.first - v1.first) * (c.second - v1.second) - (v2.second - v1.second) * (c.first - v1.first);
		if (value * crossProduct < 0) {
			return false;
		}
		else {
			value = crossProduct;
		}
	}
	return true;
}

bool PolygonMap::isPointInCircle(const Coord & d, const Face & f)
{
	Coord a = m_vertices[f.v[0]];
	Coord b = m_vertices[f.v[1]];
	Coord c = m_vertices[f.v[2]];

    std::cout << a.first << " " << a.second << std::endl;
	std::cout << d.first << " " << d.second << std::endl;
	double determinant = 0;
	for (int i = 0; i < 3; i++) {
		determinant += (m_vertices[f.v[i]].first - d.first)
			* (m_vertices[f.v[(i + 1) % 3]].second - d.second)
			* (std::pow(m_vertices[f.v[(i + 2) % 3]].first, 2) - std::pow(d.first, 2) + std::pow(m_vertices[f.v[(i + 2) % 3]].second, 2) - std::pow(d.second, 2));

		determinant -= (m_vertices[f.v[i]].first - d.first)
			* (m_vertices[f.v[(i - 1) % 3]].second - d.second)
			* (std::pow(m_vertices[f.v[(i - 2) % 3]].first, 2) - std::pow(d.first, 2) + std::pow(m_vertices[f.v[(i - 2) % 3]].second, 2) - std::pow(d.second, 2));
		std::cout << determinant << std::endl;
	}

	return determinant > 0;
}

void PolygonMap::splitFace(int f, int v)
{
	Face face = m_faces[f];
	int index = m_faces.size();
	for (int i = 0; i < 3; i++) {
		int vertices[3] = { face.v[i], face.v[(i + 1) % 3], v };
		int faces[3] = { face.f[i], index + ((i + 1) % 3), index + ((i + 2) % 3) };
		m_faces.push_back(Face(vertices, faces));
		if(face.f[i] >= 0) {
			for(int j = 0; j < 3; j++) {
				if(m_faces[face.f[i]].f[j] == f) {
					m_faces[face.f[i]].f[j] = index + i;
				}
			}

		}
		m_splitGraph[f].push_back(index + i);
		m_splitGraph.push_back(std::vector<int>());
		m_graphParent.push_back(f);
	}
}

bool PolygonMap::flipEdge(int f1, int f2)
{
	Face face1 = m_faces[f1];
	Face face2 = m_faces[f2];
	int id_f1, id_f2;

	for (int i = 0; i < 3; i++) {
		if (face1.f[i] == f2) {
			id_f1 = i;
		}
		if (face2.f[i] == f1) {
			id_f2 = i;
		}
	}

	if (!isPointInCircle(m_vertices[face2.v[(id_f2 + 2) % 3]], face1)) {
		return false;
	}

	m_faces[f1].v[(id_f1 + 1) % 3] = face2.v[(id_f2 + 2) % 3];
	m_faces[f2].v[(id_f2 + 1) % 3] = face1.v[(id_f1 + 2) % 3];

	int neighbour1 = face2.f[(id_f2 + 1) % 3];
	m_faces[f1].f[id_f1] = neighbour1;
	if(neighbour1 >= 0) {
		for(int i = 0; i < 3; i++) {
			if(m_faces[neighbour1].f[i] == f2) {
				m_faces[neighbour1].f[i] = f1;
			}
		}
	}

	int neighbour2 = face1.f[(id_f1 + 1) % 3];
	m_faces[f1].f[(id_f1 + 1) % 3] = f2;
	m_faces[f2].f[id_f2] = neighbour2;
    if(neighbour2 >= 0) {
		for(int i = 0; i < 3; i++) {
			if(m_faces[neighbour2].f[i] == f1) {
				m_faces[neighbour2].f[i] = f2;
			}
		}

	}
	m_faces[f2].f[(id_f2 + 1) % 3] = f1;

	for (int i = 0; i < 3; i++) {
		std::cout << m_faces[f1].v[i] << " " << m_faces[f2].v[i] << std::endl;
	}

	m_splitGraph[m_graphParent[f1]].push_back(f2);
	m_splitGraph[m_graphParent[f2]].push_back(f1);

	return true;
}

void PolygonMap::insertVertex(Coord & c)
{
	std::cout << "insert new vertex" << std::endl;
	m_vertices.push_back(c);
	int currentTriangle = 0;
	for (int t = 0; t < m_faces.size(); t++) {
		if (m_splitGraph[t].size() == 0 && isPointInside(c, m_faces[t])) {
			currentTriangle = t;
			break;
		}
		//for (int i = 0; i < m_splitGraph[currentTriangle].size(); i++) {
		//	if (isPointInside(c, m_faces[m_splitGraph[currentTriangle][i]])) {
		//		currentTriangle = m_splitGraph[currentTriangle][i];
		//		break;
		//	}
		//}
	}
	splitFace(currentTriangle, m_vertices.size() - 1);

	std::queue<std::pair<int, int> > toDo;
	for (int i = 0; i < 3; i++) {
		Face face1 = m_faces[m_faces[currentTriangle].f[i]];
		Face face2 = m_faces[m_faces.size() - 3 + i];
		for (int k = 0; k < 3; k++) {
			//std::cout << face2.v[k] << " " << face1.v[k] << std::endl;
		}
		toDo.push(std::make_pair(m_faces[currentTriangle].f[i], m_faces.size() - 1 - i));
	}

	while (!toDo.empty()) {
		std::pair<int, int> pair = toDo.front();
		toDo.pop();
		if (pair.first < 0 || pair.second < 0) {
			continue;
		}
		if (flipEdge(pair.first, pair.second)) {

		}
		//std::cout << toDo.size() << std:: endl;
	}
}

void PolygonMap::createVertices(int number_of_vertices)
{
	for (int i = 0; i < number_of_vertices; i++) {
		Coord new_vertex = Coord(m_distX(RNG), m_distY(RNG));
		insertVertex(new_vertex);
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

	m_map = cv::Mat(width, height, CV_64FC1);

	m_vertices.push_back(Coord(0, 0));
	m_vertices.push_back(Coord(0, 2 * height));
	m_vertices.push_back(Coord(2 * width, 0));

	int v[3] = { 0,1,2 };
	int f[3] = { -1,-1,-1 };
	m_faces.push_back(Face(v, f));

	m_splitGraph.push_back(std::vector<int>());
	m_graphParent.push_back(-1);

	createVertices(10);
}

PolygonMap::~PolygonMap()
{
}

void PolygonMap::drawMap()
{
	for (int i = 0; i < m_faces.size(); i++) {
		if (m_splitGraph[i].size() > 0) {
			continue;
		}
		for (int k = 0; k < 3; k++) {
			Coord v = m_vertices[m_faces[i].v[k]];
			Coord v2 = m_vertices[m_faces[i].v[(k + 1) % 3]];
			cv::line(m_map, cv::Point(v.first, v.second), cv::Point(v2.first, v2.second), cv::Scalar(1, 0, 0));
		}
	}
	cv::imshow("Polygon Map", m_map);
	cv::waitKey();
}
