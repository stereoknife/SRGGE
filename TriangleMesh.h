#ifndef _TRIANGLE_MESH_INCLUDE
#define _TRIANGLE_MESH_INCLUDE


#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "ShaderProgram.h"


using namespace std;


// Class TriangleMesh contains the geometry of a mesh built out of triangles.
// Both the vertices and the triangles are stored in vectors.
// TriangleMesh also manages the ids of the copy in the GPU, so as to 
// be able to render it using OpenGL.


class TriangleMesh
{

public:
	TriangleMesh();
	~TriangleMesh();

	void buildCube();		// Cube of size 1, between (-0.5, -0.5, -0.5) and (0.5, 0.5, 0.5)
	bool loadFromFile(const char *filename);	// Base at (0, 0, 0), inside a cube of size 1x1x1
	
	const vector<glm::vec3> &vertices() const;
	vector<glm::vec3> &vertices();
	const vector<int> &triangles() const;
	vector<int> &triangles();

public:
	void sendToOpenGL();
	void render() const;
	void free();

	void addVertex(const glm::vec3 &position);
	void addTriangle(int v0, int v1, int v2);

	void initVertices(const vector<float> &newVertices);
	void initTriangles(const vector<int> &newTriangles);

private:
	vector<glm::vec3> verts;
	vector<int> tris;

	GLuint vao;
	GLuint vbo;
	GLint posLocation, normalLocation;
	
};


#endif // _TRIANGLE_MESH_INCLUDE


