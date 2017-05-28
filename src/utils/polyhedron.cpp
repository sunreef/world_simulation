#include "polyhedron.h"



Polyhedron::Polyhedron()
{
}


Polyhedron::~Polyhedron()
{
	for (int i = 0; i < m_facets.size(); i++) {
		delete m_facets[i];
	}
	for (int i = 0; i < m_edges.size(); i++) {
		delete m_edges[i];
	}
	for (int i = 0; i < m_vertices.size(); i++) {
		delete m_vertices[i];
	}
}

Facet * Polyhedron::getFacet(int index)
{
	return m_facets[index];
}

std::unique_ptr<Polyhedron> Polyhedron::createTriangle(Vertex v1, Vertex  v2, Vertex  v3)
{
	std::unique_ptr<Polyhedron> P = std::make_unique<Polyhedron>(Polyhedron());

	Vertex* u1 = new Vertex(v1);
	Vertex* u2 = new Vertex(v2);
	Vertex* u3 = new Vertex(v3);

	HalfEdge* e1 = new HalfEdge;
	HalfEdge* e2 = new HalfEdge;
	HalfEdge* e3 = new HalfEdge;

	Facet* f = new Facet;

	e1->target = u1;
	e1->next = e2;
	e1->opposite = nullptr;
	e1->face = f;
	u1->edge = e2;

	e2->target = u2;
	e2->next = e3;
	e2->opposite = nullptr;
	e2->face = f;
	u2->edge = e3;

	e3->target = u3;
	e3->next = e1;
	e3->opposite = nullptr;
	e3->face = f;
	u3->edge = e1;

	f->edge = e1;

	P->m_facets.push_back(f);
	P->m_edges.push_back(e1);
	P->m_edges.push_back(e2);
	P->m_edges.push_back(e3);
	P->m_vertices.push_back(u1);
	P->m_vertices.push_back(u2);
	P->m_vertices.push_back(u3);

	return P;
}

void Polyhedron::splitFacet(Vertex v, Facet* f) {

	Vertex *v2 = new Vertex(v);
	m_vertices.push_back(v2);

	HalfEdge* faceEdge = f->edge;
	HalfEdge* currentEdge = faceEdge->next;

	std::vector<Facet*> tempFaces;
	std::vector<HalfEdge*> tempEdges;
	tempFaces.push_back(f);
	tempEdges.push_back(new HalfEdge);
	tempEdges.push_back(new HalfEdge);
	while (currentEdge != faceEdge) {
		tempFaces.push_back(new Facet);
		tempFaces[tempFaces.size() - 1]->edge = currentEdge;
		tempEdges.push_back(new HalfEdge);
		tempEdges.push_back(new HalfEdge);

		currentEdge = currentEdge->next;
	}

	int i = 0;
	currentEdge = faceEdge;
	for (int i = 0; i < tempFaces.size(); i++) {
		HalfEdge* existingEdge = tempFaces[i]->edge;
		HalfEdge* newEdge = tempEdges[2 * i];
		HalfEdge* newEdgeOpposite = tempEdges[2 * i + 1];

		newEdge->target = v2;
		newEdge->opposite = newEdgeOpposite;
		int idx = 2 * i - 1;
		if (idx < 0) {
			idx += tempEdges.size();
		}
		newEdge->next = tempEdges[idx];
		newEdge->face = tempFaces[i];

		newEdgeOpposite->target = existingEdge->target;
		newEdgeOpposite->opposite = newEdge;
		newEdgeOpposite->next = existingEdge->next;
		newEdgeOpposite->face = tempFaces[(i + 1) % tempFaces.size()];

		existingEdge->next = newEdge;
		existingEdge->face = tempFaces[i];
	}
	v2->edge = tempEdges[1];

	for (int i = 1; i < tempFaces.size(); i++) {
		m_facets.push_back(tempFaces[i]);
	}
	for (HalfEdge* edge : tempEdges) {
		m_edges.push_back(edge);
	}
}

std::string Polyhedron::toString()
{
	std::string result;
	for (int i = 0; i < m_facets.size(); i++) {
		result += m_facets[i]->toString() + "\n";
	}
	return result;
}
