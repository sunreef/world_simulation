#include "delaunay.h"

void Delaunay::insertVertex(Vertex& v) {
    Facet* f;
    bool allocated = false;
    for (auto it = facet_begin(); it != facet_end(); it++) {
        if ((*it)->isInside(v)) {
            allocated = true;
            f = *it;
            break;
        }
    }
    if (!allocated) {
        std::cout << "Not inside a face: " << v.toString() << std::endl;
    }
    splitFacet(v, f);

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

            flipEdge(edge);
        }
    }
}

Delaunay::Delaunay()
{
}


Delaunay::~Delaunay()
{
    std::cout << "Detroyed delaunay" << std::endl;
}

Delaunay::Delaunay(std::vector<Vertex>& vertices) {
    Vertex minBound, maxBound;
    for(int i = 0; i < vertices.size(); i++) {
        minBound.x = std::min(minBound.x, vertices[i].x);
        minBound.y = std::min(minBound.y, vertices[i].y);

        maxBound.x = std::max(maxBound.x, vertices[i].x);
        maxBound.y = std::max(maxBound.y, vertices[i].y);
    }
    minBound.x -= 0.001;
    minBound.y -= 0.001;
    minBound.z = 0;

    maxBound.x += 0.001;
    maxBound.y += 0.001;
    maxBound.z = 0;

    Vertex bottomLeft;
    bottomLeft.x = minBound.x;
    bottomLeft.y = maxBound.y;
    bottomLeft.z = 0;

    Vertex topRight;
    topRight.x = maxBound.x;
    topRight.y = minBound.y;
    topRight.z = 0;

    std::vector<Vertex> boundVertices;
    boundVertices.push_back(minBound);
    boundVertices.push_back(bottomLeft);
    boundVertices.push_back(maxBound);
    boundVertices.push_back(topRight);

    addFacet(boundVertices);

    for(Vertex v : vertices) {
        insertVertex(v);
    }
}
