#include <gtest/gtest.h>
#include <core/face.hpp>
#include <core/vertex.hpp>
#include <core/halfedge.hpp>
#include <models/mesh.hpp>

// /**
//  * @note The following test case is based on the following mesh:
//  *
//  * # vertices
//  * v -1 -1 0
//  * v 1 -1 0
//  * v 1 1 0
//  * # face
//  * f 1 2 3
//  *
//  * Vertex	Coordinate	Incident edge
//  * v1	    (-1, -1, 0)	e0
//  * v2	    (1, -1, 0)  e1
//  * v3	    (1, 1, 0) 	e2
//  *
//  * Face	Half-edge
//  * f0	  e0
//  *
//  * Half-edge	Origin	Twin	Incident face	Next	Prev
//  * e0	        v1	    e3	  f0            e1    e2
//  * e1	        v2	    e4	  f0            e2    e0
//  * e2	        v3	    e5	  f0            e0    e1
//  * e3	        v2	    e0	  ∅	            e5	  e4
//  * e4	        v3	    e1	  ∅	            e3	  e5
//  * e5	        v1	    e2	  ∅	            e4	  e3

class MeshTest : public ::testing::Test
{
protected:
  // Creating half edges
  core::HalfEdge *e0 = new core::HalfEdge();
  core::HalfEdge *e1 = new core::HalfEdge();
  core::HalfEdge *e2 = new core::HalfEdge();
  core::HalfEdge *e3 = new core::HalfEdge();
  core::HalfEdge *e4 = new core::HalfEdge();
  core::HalfEdge *e5 = new core::HalfEdge();

  // Creating vertices
  core::Vertex *v1 = new core::Vertex(-1.0, -1.0, -1.0, 1.0, nullptr);
  core::Vertex *v2 = new core::Vertex(1.0, -1.0, -1.0, 1.0, nullptr);
  core::Vertex *v3 = new core::Vertex(1.0, 1.0, -1.0, 1.0, nullptr);

  // Creating a list that contains how the edges are connected
  std::vector<std::vector<int>> edges = {{0, 1, 2}};

  // Creating a list that contains all edges of the face
  // std::vector<core::HalfEdge *> edges = {e0, e1, e2, e3, e4, e5};

  // // set the incident edge of each vertex
  // v1->setHalfEdge(e3);
  // v2->setHalfEdge(e4);
  // v3->setHalfEdge(e5);

  // // set the origin of each edge
  // e0->setOrigin(v1);
  // e1->setOrigin(v2);
  // e2->setOrigin(v3);
  // e3->setOrigin(v2);
  // e4->setOrigin(v3);
  // e5->setOrigin(v1);

  // // set the next edge of each edge
  // e0->setNext(e1);
  // e1->setNext(e2);
  // e2->setNext(e0);
  // e3->setNext(e5);
  // e4->setNext(e3);
  // e5->setNext(e4);

  // // set the previous edge of each edge
  // e0->setPrev(e2);
  // e1->setPrev(e0);
  // e2->setPrev(e1);
  // e3->setPrev(e4);
  // e4->setPrev(e5);
  // e5->setPrev(e3);

  // // set the twin edge of each edge
  // e0->setTwin(e3);
  // e1->setTwin(e4);
  // e2->setTwin(e5);
  // e3->setTwin(e0);
  // e4->setTwin(e1);
  // e5->setTwin(e2);

  // // Creating a face
  // core::Face *f0 = new Face(edges, e0, true);

  // // set the incident face of each edge
  // e0->setFace(f0);
  // e1->setFace(f0);
  // e2->setFace(f0);
  // e3->setFace(nullptr);
  // e4->setFace(nullptr);
  // e5->setFace(nullptr);

  void
  SetUp() override
  {
  }
};

/**
 * @brief Test the constructor of the mesh
 *
 */
TEST_F(MeshTest, default_mesh_constructor)
{
  models::Mesh *mesh = new models::Mesh();
  EXPECT_EQ(mesh->getNumFaces(), 0);
  EXPECT_EQ(mesh->getVertices().size(), 0);
  EXPECT_EQ(mesh->getHalfEdges().size(), 0);
  EXPECT_EQ(mesh->getFaces().size(), 0);
}

/**
 * @brief Test the constructor of the mesh
 *
 */
// TEST_F(MeshTest, mesh_constructor)
// {
//   core::Mesh *mesh = new core::Mesh({v1, v2, v3}, edges);

//   core::HalfEdge *he = mesh->getHalfEdges()[0];
// }

/**
 * @brief Test case for a cube mesh
 *
 */
TEST_F(MeshTest, cube_mesh_constructor)
{
  // Back Face
  // Left Face
  // Bottom Face
  // Right Face
  // Top Face

  // Bottom-left-back
  core::Vertex *v0 = new core::Vertex(-1.0, -1.0, -1.0, 1.0, nullptr, "v1");
  // Bottom-right-back
  core::Vertex *v1 = new core::Vertex(1.0, -1.0, -1.0, 1.0, nullptr, "v2");
  // Bottom-right-front
  core::Vertex *v2 = new core::Vertex(1.0, -1.0, 1.0, 1.0, nullptr, "v3");
  // Bottom-left-front
  core::Vertex *v3 = new core::Vertex(-1.0, -1.0, 1.0, 1.0, nullptr, "v4");
  // Top-left-back
  core::Vertex *v4 = new core::Vertex(-1.0, 1.0, -1.0, 1.0, nullptr, "v5");
  // Top-right-back
  core::Vertex *v5 = new core::Vertex(1.0, 1.0, -1.0, 1.0, nullptr, "v6");
  // Top-right-front
  core::Vertex *v6 = new core::Vertex(1.0, 1.0, 1.0, 1.0, nullptr, "v7");
  // Top-left-front
  core::Vertex *v7 = new core::Vertex(-1.0, 1.0, 1.0, 1.0, nullptr, "v8");

  std::vector<std::vector<int>> edges = {
      // Back Face
      {2, 6, 7, 3},
      // Left Face
      {1, 0, 4, 5},
      // Bottom Face
      {3, 7, 4, 0},
      // Right Face
      {2, 3, 0, 1},
      // Top Face
      {6, 2, 1, 5},
      // Front Face
      {6, 5, 4, 7}};

  models::Mesh *mesh = new models::Mesh({v0, v1, v2, v3, v4, v5, v6, v7}, edges, "cube");

  std::cout << "Face\tHalf-edge" << std::endl;
  for (auto face : mesh->getFaces())
  {
    std::cout << face->getId() << "\t" << face->getHalfEdge()->getId() << std::endl;
  }

  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "Vertex\tCoordinate\tIncident edge" << std::endl;
  for (auto vertex : mesh->getVertices())
  {
    std::cout << vertex->getId() << "\t(" << vertex->getX() << ", " << vertex->getY() << ", " << vertex->getZ() << ")\t" << vertex->getHalfEdge()->getId() << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Half-edge\tOrigin\tTwin\tIncident face\tNext\tPrev" << std::endl;
  for (auto edge : mesh->getHalfEdges())
  {
    std::cout << edge->getId() << "\t\t" << edge->getOrigin()->getId() << "\t" << edge->getTwin()->getId() << "\t" << edge->getFace()->getId() << "\t\t" << edge->getNext()->getId() << "\t" << edge->getPrev()->getId() << std::endl;
  }
}
