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
      // // Back Face
      // {2, 6, 7, 3},
      // // Left Face
      // {1, 0, 4, 5},
      // // Bottom Face
      // {3, 7, 4, 0},
      // // Right Face
      // {2, 3, 0, 1},
      // // Top Face
      // {6, 2, 1, 5},
      // // Front Face
      // {6, 5, 4, 7}};
      // Back Face
      {2, 6, 7},
      {7, 3, 2},
      // Left Face
      {1, 0, 4},
      {4, 5, 1},
      // Bottom Face
      {3, 7, 4},
      {4, 0, 3},
      // Right Face
      {2, 3, 0},
      {0, 1, 2},
      // Top Face
      {6, 2, 1},
      {1, 5, 6},
      // Front Face
      {6, 5, 4},
      {4, 7, 6}};

  models::Mesh *mesh = new models::Mesh({v0, v1, v2, v3, v4, v5, v6, v7}, edges, "cube");

  EXPECT_EQ(mesh->getVertices().size(), 8);
  EXPECT_EQ(mesh->getNumFaces(), 12);
  EXPECT_EQ(mesh->getHalfEdges().size(), 36);

  for (int i = 0; i < mesh->getVertices().size(); i++)
    EXPECT_EQ(mesh->getVertices()[i]->getId(), "v" + std::to_string(i + 1));

  // Vertex	Coordinate	Incident edge
  // v1	    (-1, -1, 0)	e7
  // v2	    (1, -1, 0)	e6
  // v3	    (1, -1, 0)	e0
  // v4	    (-1, -1, 0)	e4
  // v5	    (-1, 1, 0)	e8
  // v6	    (1, 1, 0)	  e10
  // v7	    (1, 1, 0) 	e1
  // v8	    (-1, 1, 0)	e2

  EXPECT_EQ(mesh->getVertices()[0]->getHalfEdge()->getId(), "e7");
  EXPECT_EQ(mesh->getVertices()[1]->getHalfEdge()->getId(), "e6");
  EXPECT_EQ(mesh->getVertices()[2]->getHalfEdge()->getId(), "e0");
  EXPECT_EQ(mesh->getVertices()[3]->getHalfEdge()->getId(), "e4");
  EXPECT_EQ(mesh->getVertices()[4]->getHalfEdge()->getId(), "e8");
  EXPECT_EQ(mesh->getVertices()[5]->getHalfEdge()->getId(), "e10");
  EXPECT_EQ(mesh->getVertices()[6]->getHalfEdge()->getId(), "e1");
  EXPECT_EQ(mesh->getVertices()[7]->getHalfEdge()->getId(), "e2");

  for (int i = 0; i < mesh->getNumFaces(); i++)
    EXPECT_EQ(mesh->getFaces()[i]->getId(), "f" + std::to_string(i));

  // Face	Half-edge
  // f0	  e0
  // f1	  e3
  // f2	  e6
  // f3	  e9
  // f4	  e12
  // f5	  e15
  // f6	  e18
  // f7	  e21
  // f8	  e24
  // f9	  e27
  // f10	  e30
  // f11	  e33

  EXPECT_EQ(mesh->getFaces()[0]->getHalfEdge()->getId(), "e0");
  EXPECT_EQ(mesh->getFaces()[1]->getHalfEdge()->getId(), "e3");
  EXPECT_EQ(mesh->getFaces()[2]->getHalfEdge()->getId(), "e6");
  EXPECT_EQ(mesh->getFaces()[3]->getHalfEdge()->getId(), "e9");
  EXPECT_EQ(mesh->getFaces()[4]->getHalfEdge()->getId(), "e12");
  EXPECT_EQ(mesh->getFaces()[5]->getHalfEdge()->getId(), "e15");
  EXPECT_EQ(mesh->getFaces()[6]->getHalfEdge()->getId(), "e18");
  EXPECT_EQ(mesh->getFaces()[7]->getHalfEdge()->getId(), "e21");
  EXPECT_EQ(mesh->getFaces()[8]->getHalfEdge()->getId(), "e24");
  EXPECT_EQ(mesh->getFaces()[9]->getHalfEdge()->getId(), "e27");
  EXPECT_EQ(mesh->getFaces()[10]->getHalfEdge()->getId(), "e30");
  EXPECT_EQ(mesh->getFaces()[11]->getHalfEdge()->getId(), "e33");

  // Half-edge	Origin	Twin	Incident face	Next	Prev
  // e0	          v3	    e24 	f0           	e1	  e2
  // e1	          v7	    e34 	f0           	e2	  e0
  // e2	          v8	    e5	  f0	          e0	  e1
  // e3	          v8	    e12 	f1           	e4	  e5
  // e4	          v4	    e18 	f1           	e5	  e3
  // e5	          v3	    e2	  f1	          e3	  e4
  // e6	          v2	    e21 	f2           	e7	  e8
  // e7	          v1	    e15 	f2           	e8	  e6
  // e8	          v5	    e11 	f2           	e6	  e7
  // e9	          v5	    e31 	f3           	e10	  e11
  // e10	        v6	    e27 	f3           	e11	  e9
  // e11	        v2	    e8	  f3	          e9	  e10
  // e12	        v4	    e3	  f4	          e13	  e14
  // e13	        v8	    e33 	f4           	e14	  e12
  // e14	        v5	    e17 	f4           	e12	  e13
  // e15	        v5	    e7	  f5	          e16	  e17
  // e16	        v1	    e19 	f5           	e17	  e15
  // e17	        v4	    e14 	f5           	e15	  e16
  // e18	        v3	    e4	  f6	          e19	  e20
  // e19	        v4	    e16 	f6           	e20	  e18
  // e20	        v1	    e23 	f6           	e18	  e19
  // e21	        v1	    e6	  f7	          e22	  e23
  // e22	        v2	    e25 	f7           	e23	  e21
  // e23	        v3	    e20 	f7           	e21	  e22
  // e24	        v7	    e0	  f8	          e25	  e26
  // e25	        v3	    e22 	f8           	e26	  e24
  // e26	        v2	    e29 	f8           	e24	  e25
  // e27	        v2	    e10 	f9           	e28	  e29
  // e28	        v6	    e30 	f9           	e29	  e27
  // e29	        v7	    e26 	f9           	e27	  e28
  // e30	        v7	    e28 	f10	          e31	  e32
  // e31	        v6	    e9	  f10          	e32	  e30
  // e32	        v5	    e35 	f10	          e30	  e31
  // e33	        v5	    e13 	f11	          e34	  e35
  // e34	        v8	    e1	  f11          	e35	  e33
  // e35	        v7	    e32 	f11	          e33	  e34

  for (int i = 0; i < mesh->getHalfEdges().size(); i++)
    EXPECT_EQ(mesh->getHalfEdges()[i]->getId(), "e" + std::to_string(i));

  EXPECT_EQ(mesh->getHalfEdges()[0]->getOrigin()->getId(), "v3");
  EXPECT_EQ(mesh->getHalfEdges()[0]->getTwin()->getId(), "e24");
  EXPECT_EQ(mesh->getHalfEdges()[0]->getFace()->getId(), "f0");
  EXPECT_EQ(mesh->getHalfEdges()[0]->getNext()->getId(), "e1");
  EXPECT_EQ(mesh->getHalfEdges()[0]->getPrev()->getId(), "e2");

  EXPECT_EQ(mesh->getHalfEdges()[1]->getOrigin()->getId(), "v7");
  EXPECT_EQ(mesh->getHalfEdges()[1]->getTwin()->getId(), "e34");
  EXPECT_EQ(mesh->getHalfEdges()[1]->getFace()->getId(), "f0");
  EXPECT_EQ(mesh->getHalfEdges()[1]->getNext()->getId(), "e2");
  EXPECT_EQ(mesh->getHalfEdges()[1]->getPrev()->getId(), "e0");

  EXPECT_EQ(mesh->getHalfEdges()[2]->getOrigin()->getId(), "v8");
  EXPECT_EQ(mesh->getHalfEdges()[2]->getTwin()->getId(), "e5");
  EXPECT_EQ(mesh->getHalfEdges()[2]->getFace()->getId(), "f0");
  EXPECT_EQ(mesh->getHalfEdges()[2]->getNext()->getId(), "e0");
  EXPECT_EQ(mesh->getHalfEdges()[2]->getPrev()->getId(), "e1");

  EXPECT_EQ(mesh->getHalfEdges()[3]->getOrigin()->getId(), "v8");
  EXPECT_EQ(mesh->getHalfEdges()[3]->getTwin()->getId(), "e12");
  EXPECT_EQ(mesh->getHalfEdges()[3]->getFace()->getId(), "f1");
  EXPECT_EQ(mesh->getHalfEdges()[3]->getNext()->getId(), "e4");
  EXPECT_EQ(mesh->getHalfEdges()[3]->getPrev()->getId(), "e5");

  EXPECT_EQ(mesh->getHalfEdges()[4]->getOrigin()->getId(), "v4");
  EXPECT_EQ(mesh->getHalfEdges()[4]->getTwin()->getId(), "e18");
  EXPECT_EQ(mesh->getHalfEdges()[4]->getFace()->getId(), "f1");
  EXPECT_EQ(mesh->getHalfEdges()[4]->getNext()->getId(), "e5");
  EXPECT_EQ(mesh->getHalfEdges()[4]->getPrev()->getId(), "e3");

  EXPECT_EQ(mesh->getHalfEdges()[5]->getOrigin()->getId(), "v3");
  EXPECT_EQ(mesh->getHalfEdges()[5]->getTwin()->getId(), "e2");
  EXPECT_EQ(mesh->getHalfEdges()[5]->getFace()->getId(), "f1");
  EXPECT_EQ(mesh->getHalfEdges()[5]->getNext()->getId(), "e3");
  EXPECT_EQ(mesh->getHalfEdges()[5]->getPrev()->getId(), "e4");

  EXPECT_EQ(mesh->getHalfEdges()[6]->getOrigin()->getId(), "v2");
  EXPECT_EQ(mesh->getHalfEdges()[6]->getTwin()->getId(), "e21");
  EXPECT_EQ(mesh->getHalfEdges()[6]->getFace()->getId(), "f2");
  EXPECT_EQ(mesh->getHalfEdges()[6]->getNext()->getId(), "e7");
  EXPECT_EQ(mesh->getHalfEdges()[6]->getPrev()->getId(), "e8");

  EXPECT_EQ(mesh->getHalfEdges()[7]->getOrigin()->getId(), "v1");
  EXPECT_EQ(mesh->getHalfEdges()[7]->getTwin()->getId(), "e15");
  EXPECT_EQ(mesh->getHalfEdges()[7]->getFace()->getId(), "f2");
  EXPECT_EQ(mesh->getHalfEdges()[7]->getNext()->getId(), "e8");
  EXPECT_EQ(mesh->getHalfEdges()[7]->getPrev()->getId(), "e6");

  EXPECT_EQ(mesh->getHalfEdges()[8]->getOrigin()->getId(), "v5");
  EXPECT_EQ(mesh->getHalfEdges()[8]->getTwin()->getId(), "e11");
  EXPECT_EQ(mesh->getHalfEdges()[8]->getFace()->getId(), "f2");
  EXPECT_EQ(mesh->getHalfEdges()[8]->getNext()->getId(), "e6");
  EXPECT_EQ(mesh->getHalfEdges()[8]->getPrev()->getId(), "e7");

  EXPECT_EQ(mesh->getHalfEdges()[9]->getOrigin()->getId(), "v5");
  EXPECT_EQ(mesh->getHalfEdges()[9]->getTwin()->getId(), "e31");
  EXPECT_EQ(mesh->getHalfEdges()[9]->getFace()->getId(), "f3");
  EXPECT_EQ(mesh->getHalfEdges()[9]->getNext()->getId(), "e10");
  EXPECT_EQ(mesh->getHalfEdges()[9]->getPrev()->getId(), "e11");

  EXPECT_EQ(mesh->getHalfEdges()[10]->getOrigin()->getId(), "v6");
  EXPECT_EQ(mesh->getHalfEdges()[10]->getTwin()->getId(), "e27");
  EXPECT_EQ(mesh->getHalfEdges()[10]->getFace()->getId(), "f3");
  EXPECT_EQ(mesh->getHalfEdges()[10]->getNext()->getId(), "e11");
  EXPECT_EQ(mesh->getHalfEdges()[10]->getPrev()->getId(), "e9");

  EXPECT_EQ(mesh->getHalfEdges()[11]->getOrigin()->getId(), "v2");
  EXPECT_EQ(mesh->getHalfEdges()[11]->getTwin()->getId(), "e8");
  EXPECT_EQ(mesh->getHalfEdges()[11]->getFace()->getId(), "f3");
  EXPECT_EQ(mesh->getHalfEdges()[11]->getNext()->getId(), "e9");
  EXPECT_EQ(mesh->getHalfEdges()[11]->getPrev()->getId(), "e10");

  EXPECT_EQ(mesh->getHalfEdges()[12]->getOrigin()->getId(), "v4");
  EXPECT_EQ(mesh->getHalfEdges()[12]->getTwin()->getId(), "e3");
  EXPECT_EQ(mesh->getHalfEdges()[12]->getFace()->getId(), "f4");
  EXPECT_EQ(mesh->getHalfEdges()[12]->getNext()->getId(), "e13");
  EXPECT_EQ(mesh->getHalfEdges()[12]->getPrev()->getId(), "e14");

  EXPECT_EQ(mesh->getHalfEdges()[13]->getOrigin()->getId(), "v8");
  EXPECT_EQ(mesh->getHalfEdges()[13]->getTwin()->getId(), "e33");
  EXPECT_EQ(mesh->getHalfEdges()[13]->getFace()->getId(), "f4");
  EXPECT_EQ(mesh->getHalfEdges()[13]->getNext()->getId(), "e14");
  EXPECT_EQ(mesh->getHalfEdges()[13]->getPrev()->getId(), "e12");

  EXPECT_EQ(mesh->getHalfEdges()[14]->getOrigin()->getId(), "v5");
  EXPECT_EQ(mesh->getHalfEdges()[14]->getTwin()->getId(), "e17");
  EXPECT_EQ(mesh->getHalfEdges()[14]->getFace()->getId(), "f4");
  EXPECT_EQ(mesh->getHalfEdges()[14]->getNext()->getId(), "e12");
  EXPECT_EQ(mesh->getHalfEdges()[14]->getPrev()->getId(), "e13");

  EXPECT_EQ(mesh->getHalfEdges()[15]->getOrigin()->getId(), "v5");
  EXPECT_EQ(mesh->getHalfEdges()[15]->getTwin()->getId(), "e7");
  EXPECT_EQ(mesh->getHalfEdges()[15]->getFace()->getId(), "f5");
  EXPECT_EQ(mesh->getHalfEdges()[15]->getNext()->getId(), "e16");
  EXPECT_EQ(mesh->getHalfEdges()[15]->getPrev()->getId(), "e17");

  EXPECT_EQ(mesh->getHalfEdges()[16]->getOrigin()->getId(), "v1");
  EXPECT_EQ(mesh->getHalfEdges()[16]->getTwin()->getId(), "e19");
  EXPECT_EQ(mesh->getHalfEdges()[16]->getFace()->getId(), "f5");
  EXPECT_EQ(mesh->getHalfEdges()[16]->getNext()->getId(), "e17");
  EXPECT_EQ(mesh->getHalfEdges()[16]->getPrev()->getId(), "e15");

  EXPECT_EQ(mesh->getHalfEdges()[17]->getOrigin()->getId(), "v4");
  EXPECT_EQ(mesh->getHalfEdges()[17]->getTwin()->getId(), "e14");
  EXPECT_EQ(mesh->getHalfEdges()[17]->getFace()->getId(), "f5");
  EXPECT_EQ(mesh->getHalfEdges()[17]->getNext()->getId(), "e15");
  EXPECT_EQ(mesh->getHalfEdges()[17]->getPrev()->getId(), "e16");

  EXPECT_EQ(mesh->getHalfEdges()[18]->getOrigin()->getId(), "v3");
  EXPECT_EQ(mesh->getHalfEdges()[18]->getTwin()->getId(), "e4");
  EXPECT_EQ(mesh->getHalfEdges()[18]->getFace()->getId(), "f6");
  EXPECT_EQ(mesh->getHalfEdges()[18]->getNext()->getId(), "e19");
  EXPECT_EQ(mesh->getHalfEdges()[18]->getPrev()->getId(), "e20");

  EXPECT_EQ(mesh->getHalfEdges()[19]->getOrigin()->getId(), "v4");
  EXPECT_EQ(mesh->getHalfEdges()[19]->getTwin()->getId(), "e16");
  EXPECT_EQ(mesh->getHalfEdges()[19]->getFace()->getId(), "f6");
  EXPECT_EQ(mesh->getHalfEdges()[19]->getNext()->getId(), "e20");
  EXPECT_EQ(mesh->getHalfEdges()[19]->getPrev()->getId(), "e18");

  EXPECT_EQ(mesh->getHalfEdges()[20]->getOrigin()->getId(), "v1");
  EXPECT_EQ(mesh->getHalfEdges()[20]->getTwin()->getId(), "e23");
  EXPECT_EQ(mesh->getHalfEdges()[20]->getFace()->getId(), "f6");
  EXPECT_EQ(mesh->getHalfEdges()[20]->getNext()->getId(), "e18");
  EXPECT_EQ(mesh->getHalfEdges()[20]->getPrev()->getId(), "e19");

  EXPECT_EQ(mesh->getHalfEdges()[21]->getOrigin()->getId(), "v1");
  EXPECT_EQ(mesh->getHalfEdges()[21]->getTwin()->getId(), "e6");
  EXPECT_EQ(mesh->getHalfEdges()[21]->getFace()->getId(), "f7");
  EXPECT_EQ(mesh->getHalfEdges()[21]->getNext()->getId(), "e22");
  EXPECT_EQ(mesh->getHalfEdges()[21]->getPrev()->getId(), "e23");

  EXPECT_EQ(mesh->getHalfEdges()[22]->getOrigin()->getId(), "v2");
  EXPECT_EQ(mesh->getHalfEdges()[22]->getTwin()->getId(), "e25");
  EXPECT_EQ(mesh->getHalfEdges()[22]->getFace()->getId(), "f7");
  EXPECT_EQ(mesh->getHalfEdges()[22]->getNext()->getId(), "e23");
  EXPECT_EQ(mesh->getHalfEdges()[22]->getPrev()->getId(), "e21");

  EXPECT_EQ(mesh->getHalfEdges()[23]->getOrigin()->getId(), "v3");
  EXPECT_EQ(mesh->getHalfEdges()[23]->getTwin()->getId(), "e20");
  EXPECT_EQ(mesh->getHalfEdges()[23]->getFace()->getId(), "f7");
  EXPECT_EQ(mesh->getHalfEdges()[23]->getNext()->getId(), "e21");
  EXPECT_EQ(mesh->getHalfEdges()[23]->getPrev()->getId(), "e22");

  EXPECT_EQ(mesh->getHalfEdges()[24]->getOrigin()->getId(), "v7");
  EXPECT_EQ(mesh->getHalfEdges()[24]->getTwin()->getId(), "e0");
  EXPECT_EQ(mesh->getHalfEdges()[24]->getFace()->getId(), "f8");
  EXPECT_EQ(mesh->getHalfEdges()[24]->getNext()->getId(), "e25");
  EXPECT_EQ(mesh->getHalfEdges()[24]->getPrev()->getId(), "e26");

  EXPECT_EQ(mesh->getHalfEdges()[25]->getOrigin()->getId(), "v3");
  EXPECT_EQ(mesh->getHalfEdges()[25]->getTwin()->getId(), "e22");
  EXPECT_EQ(mesh->getHalfEdges()[25]->getFace()->getId(), "f8");
  EXPECT_EQ(mesh->getHalfEdges()[25]->getNext()->getId(), "e26");
  EXPECT_EQ(mesh->getHalfEdges()[25]->getPrev()->getId(), "e24");

  EXPECT_EQ(mesh->getHalfEdges()[26]->getOrigin()->getId(), "v2");
  EXPECT_EQ(mesh->getHalfEdges()[26]->getTwin()->getId(), "e29");
  EXPECT_EQ(mesh->getHalfEdges()[26]->getFace()->getId(), "f8");
  EXPECT_EQ(mesh->getHalfEdges()[26]->getNext()->getId(), "e24");
  EXPECT_EQ(mesh->getHalfEdges()[26]->getPrev()->getId(), "e25");

  EXPECT_EQ(mesh->getHalfEdges()[27]->getOrigin()->getId(), "v2");
  EXPECT_EQ(mesh->getHalfEdges()[27]->getTwin()->getId(), "e10");
  EXPECT_EQ(mesh->getHalfEdges()[27]->getFace()->getId(), "f9");
  EXPECT_EQ(mesh->getHalfEdges()[27]->getNext()->getId(), "e28");
  EXPECT_EQ(mesh->getHalfEdges()[27]->getPrev()->getId(), "e29");

  EXPECT_EQ(mesh->getHalfEdges()[28]->getOrigin()->getId(), "v6");
  EXPECT_EQ(mesh->getHalfEdges()[28]->getTwin()->getId(), "e30");
  EXPECT_EQ(mesh->getHalfEdges()[28]->getFace()->getId(), "f9");
  EXPECT_EQ(mesh->getHalfEdges()[28]->getNext()->getId(), "e29");
  EXPECT_EQ(mesh->getHalfEdges()[28]->getPrev()->getId(), "e27");

  EXPECT_EQ(mesh->getHalfEdges()[29]->getOrigin()->getId(), "v7");
  EXPECT_EQ(mesh->getHalfEdges()[29]->getTwin()->getId(), "e26");
  EXPECT_EQ(mesh->getHalfEdges()[29]->getFace()->getId(), "f9");
  EXPECT_EQ(mesh->getHalfEdges()[29]->getNext()->getId(), "e27");
  EXPECT_EQ(mesh->getHalfEdges()[29]->getPrev()->getId(), "e28");

  EXPECT_EQ(mesh->getHalfEdges()[30]->getOrigin()->getId(), "v7");
  EXPECT_EQ(mesh->getHalfEdges()[30]->getTwin()->getId(), "e28");
  EXPECT_EQ(mesh->getHalfEdges()[30]->getFace()->getId(), "f10");
  EXPECT_EQ(mesh->getHalfEdges()[30]->getNext()->getId(), "e31");
  EXPECT_EQ(mesh->getHalfEdges()[30]->getPrev()->getId(), "e32");

  EXPECT_EQ(mesh->getHalfEdges()[31]->getOrigin()->getId(), "v6");
  EXPECT_EQ(mesh->getHalfEdges()[31]->getTwin()->getId(), "e9");
  EXPECT_EQ(mesh->getHalfEdges()[31]->getFace()->getId(), "f10");
  EXPECT_EQ(mesh->getHalfEdges()[31]->getNext()->getId(), "e32");
  EXPECT_EQ(mesh->getHalfEdges()[31]->getPrev()->getId(), "e30");

  EXPECT_EQ(mesh->getHalfEdges()[32]->getOrigin()->getId(), "v5");
  EXPECT_EQ(mesh->getHalfEdges()[32]->getTwin()->getId(), "e35");
  EXPECT_EQ(mesh->getHalfEdges()[32]->getFace()->getId(), "f10");
  EXPECT_EQ(mesh->getHalfEdges()[32]->getNext()->getId(), "e30");
  EXPECT_EQ(mesh->getHalfEdges()[32]->getPrev()->getId(), "e31");

  EXPECT_EQ(mesh->getHalfEdges()[33]->getOrigin()->getId(), "v5");
  EXPECT_EQ(mesh->getHalfEdges()[33]->getTwin()->getId(), "e13");
  EXPECT_EQ(mesh->getHalfEdges()[33]->getFace()->getId(), "f11");
  EXPECT_EQ(mesh->getHalfEdges()[33]->getNext()->getId(), "e34");
  EXPECT_EQ(mesh->getHalfEdges()[33]->getPrev()->getId(), "e35");

  EXPECT_EQ(mesh->getHalfEdges()[34]->getOrigin()->getId(), "v8");
  EXPECT_EQ(mesh->getHalfEdges()[34]->getTwin()->getId(), "e1");
  EXPECT_EQ(mesh->getHalfEdges()[34]->getFace()->getId(), "f11");
  EXPECT_EQ(mesh->getHalfEdges()[34]->getNext()->getId(), "e35");
  EXPECT_EQ(mesh->getHalfEdges()[34]->getPrev()->getId(), "e33");

  EXPECT_EQ(mesh->getHalfEdges()[35]->getOrigin()->getId(), std::string("v7"));
  EXPECT_EQ(mesh->getHalfEdges()[35]->getTwin()->getId(), "e32");
  EXPECT_EQ(mesh->getHalfEdges()[35]->getFace()->getId(), "f11");
  EXPECT_EQ(mesh->getHalfEdges()[35]->getNext()->getId(), "e33");
  EXPECT_EQ(mesh->getHalfEdges()[35]->getPrev()->getId(), "e34");
}
