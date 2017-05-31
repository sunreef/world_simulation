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

HalfEdge * Polyhedron::getHalfEdge(int index)
{
	return m_edges[index];
}

std::vector<HalfEdge*>::iterator Polyhedron::edge_begin()
{
	return m_edges.begin();
}

std::vector<HalfEdge*>::iterator Polyhedron::edge_end()
{
	return m_edges.end();
}

std::vector<Facet*>::iterator Polyhedron::facet_begin()
{
	return m_facets.begin();
}

std::vector<Facet*>::iterator Polyhedron::facet_end()
{
	return m_facets.end();
}

std::unique_ptr<Polyhedron> Polyhedron::createTriangle(Vertex v1, Vertex  v2, Vertex  v3)
{
	std::unique_ptr<Polyhedron> P = std::unique_ptr<Polyhedron>(new Polyhedron());

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

	return std::move(P);
}

void Polyhedron::addFacet(std::vector<Vertex>& vertices) {
	Facet* f = new Facet;
	m_facets.push_back(f);

	std::vector<HalfEdge*> edges(vertices.size());
	std::vector<Vertex*> new_vertices(vertices.size());
	for(int i = 0; i < edges.size(); i++) {
		edges[i] = new HalfEdge;
		new_vertices[i] = new Vertex(vertices[i]);
	}

	for(int i = 0; i < vertices.size(); i++) {
		new_vertices[i]->edge = edges[(i+1) % vertices.size()];

		edges[i]->target = new_vertices[i];
		edges[i]->next = edges[(i+1) % vertices.size()];
		edges[i]->face = f;
		edges[i]->opposite = nullptr;
	}
    f->edge = edges[0];

	for(auto edge : edges) {
		m_edges.push_back(edge);
	}
	for(auto vertex : new_vertices) {
		m_vertices.push_back(vertex);
	}
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

void Polyhedron::flipEdge(HalfEdge * edge)
{
	HalfEdge* oppositeEdge = edge->opposite;
	if (oppositeEdge == nullptr) {
		std::cout << "ERROR: trying to flip a border edge." << std::endl;
		return;
	}

	Facet* f = edge->face;
	Facet* f2 = oppositeEdge->face;

	if (!f->isTriangle() || !f2->isTriangle()) {
		std::cout << "ERROR: trying to flip an edge that doesn't belong to two triangles." << std::endl;
		return;
	}

	edge->target->edge = edge->next;
	oppositeEdge->target->edge = oppositeEdge->next;

	edge->target = edge->next->target;
	oppositeEdge->target = oppositeEdge->next->target;

	edge->next->next->next = oppositeEdge->next;
	oppositeEdge->next->next->next = edge->next;

	HalfEdge* temp = edge->next->next;
	HalfEdge* tempOpposite = oppositeEdge->next->next;

	edge->next->next = oppositeEdge;
	edge->next->face = f2;
	oppositeEdge->next->next = edge;
	oppositeEdge->next->face = f;

	edge->next = temp;
	oppositeEdge->next = tempOpposite;

	f->edge = edge;
	f2->edge = oppositeEdge;

	if (!f->isTriangle()) {
		std::cout << "Wrong" << std::endl;
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
