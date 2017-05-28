#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>

struct Vertex;
struct HalfEdge;
struct Facet;

struct Vertex {
	double x;
	double y;
	double z;

	HalfEdge* edge;

	std::string toString() {
		std::string result = "";
		result += "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
		return result;
	}
};

struct HalfEdge {
	Vertex* target;
	HalfEdge* opposite;
	HalfEdge* next;
	HalfEdge* prev;
	Facet* face;

	std::string toString() {
		return target->toString();
	}
};

struct Facet {
	HalfEdge* edge;

	std::string toString() {
		HalfEdge* faceEdge = edge;
		HalfEdge* currentEdge = faceEdge;
		std::string result = "[";
		do {
			result += currentEdge->toString() + ", ";
			currentEdge = currentEdge->next;
		} while (currentEdge != faceEdge);
		result += "]";
		return result;
	}
};

class Polyhedron
{
	std::vector<Vertex*> m_vertices;
	std::vector<HalfEdge*> m_edges;
	std::vector<Facet*> m_facets;

public:
	Polyhedron();
	~Polyhedron();

	Facet* getFacet(int index);

	static std::unique_ptr<Polyhedron> createTriangle(Vertex v1, Vertex v2, Vertex v3);
	void splitFacet(Vertex v, Facet* f);

	std::string toString();

};

