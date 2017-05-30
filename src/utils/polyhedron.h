#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>

struct Vertex;
struct HalfEdge;
struct Facet;
struct Circle;

struct Vertex {
	double x;
	double y;
	double z;

	HalfEdge* edge;

	double squareLength() {
		return x*x + y*y + z*z;
	}

	Vertex operator+(const Vertex& b) {
		Vertex v;
		v.x = x + b.x;
		v.y = y + b.y;
		v.z = z + b.z;
		return v;

	}

	Vertex operator-(const Vertex& b) {
		Vertex v;
		v.x = x - b.x;
		v.y = y - b.y;
		v.z = z - b.z;
		return v;
	}

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
	Facet* face;

	std::string toString() {
		return target->toString();
	}
};

struct Circle {
	Vertex c;
	double radius;

	bool isInside(Vertex* v) {
		return (c - *v).squareLength() < radius * radius;
	}

	std::string toString() {
		return "{Center: " + c.toString() + ", Radius: " + std::to_string(radius) + "}";
	}
};

struct Facet {
	HalfEdge* edge;

	bool isTriangle() {
		return (edge->next->next->next == edge);
	}

	Circle getCircumcircle() {
		Circle circle;

		if (!isTriangle()) {
			return circle;
		}

		Vertex a = *(edge->target);
		Vertex b = *(edge->next->target);
		Vertex c = *(edge->next->next->target);

		b = b - a;
		c = c - a;

		double sqB = b.squareLength();
		double sqC = c.squareLength();

		double D = 2 * (b.x * c.y - c.x * b.y);
		D = 1.0 / D;

		double ux = D * (sqB * c.y - sqC *  b.y);
		double uy = D * (sqC * b.x - sqB * c.x);

		Vertex U;
		U.x = ux;
		U.y = uy;
		U.z = 0;

		double radius = std::sqrt(U.squareLength());

		circle.c = U + a;
		circle.radius = radius;
		return circle;
	}

	bool isInside(Vertex v) {
		bool inside = false;
		HalfEdge* currentEdge = edge;
		Vertex* currentVertex = edge->target;
		do {
			currentEdge = currentEdge->next;
			Vertex* newTarget = currentEdge->target;

			if (((currentVertex->y > v.y) != (newTarget->y > v.y)) &&
				(v.x < (newTarget->x - currentVertex->x) * (v.y - currentVertex->y) / (newTarget->y - currentVertex->y) + currentVertex->x)) {
				inside = !inside;
			}
			currentVertex = newTarget;

		} while (currentEdge != edge);
		return inside;
	}

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
	HalfEdge* getHalfEdge(int index);

	std::vector<HalfEdge*>::iterator edge_begin();
	std::vector<HalfEdge*>::iterator edge_end();
	std::vector<Facet*>::iterator facet_begin();
	std::vector<Facet*>::iterator facet_end();




	static std::unique_ptr<Polyhedron> createTriangle(Vertex v1, Vertex v2, Vertex v3);

	void splitFacet(Vertex v, Facet* f);
	void flipEdge(HalfEdge* edge);

	std::string toString();

};

