#include <vector>
#include "TriangleMesh.h"
#include "Application.h"
#include "PLYReader.h"


using namespace std;


TriangleMesh::TriangleMesh()
{
	vao = -1;
	vbo = -1;
}

TriangleMesh::~TriangleMesh()
{
	free();
}

bool TriangleMesh::loadFromFile(const char *filename)
{
	PLYReader reader;

	if(vao == -1)
		free();
	bool bSuccess = reader.readMesh(filename, *this);
	if(bSuccess)
		sendToOpenGL();
	
	return bSuccess;
}

void TriangleMesh::addVertex(const glm::vec3 &position)
{
	verts.push_back(position);
}

void TriangleMesh::addTriangle(int v0, int v1, int v2)
{
	tris.push_back(v0);
	tris.push_back(v1);
	tris.push_back(v2);
}

void TriangleMesh::initVertices(const vector<float> &newVertices)
{
	verts.resize(newVertices.size() / 3);
	for(unsigned int i=0; i<verts.size(); i++)
		verts[i] = glm::vec3(newVertices[3*i], newVertices[3*i+1], newVertices[3*i+2]);
}

void TriangleMesh::initTriangles(const vector<int> &newTriangles)
{
	tris = newTriangles;
}

const vector<glm::vec3> &TriangleMesh::vertices() const
{
	return verts;
}

vector<glm::vec3> &TriangleMesh::vertices()
{
	return verts;
}

const vector<int> &TriangleMesh::triangles() const
{
	return tris;
}

vector<int> &TriangleMesh::triangles()
{
	return tris;
}

void TriangleMesh::buildCube()
{
	float vertices[] = {-1, -1, -1,
                      1, -1, -1,
                      1,  1, -1,
                      -1,  1, -1,
                      -1, -1,  1,
                      1, -1,  1,
                      1,  1,  1,
                      -1,  1,  1
	};

	int faces[] = {3, 1, 0, 3, 2, 1,
                 5, 6, 7, 4, 5, 7,
                 7, 3, 0, 0, 4, 7,
                 1, 2, 6, 6, 5, 1,
                 0, 1, 4, 5, 4, 1,
                 2, 3, 7, 7, 6, 2
	};

	int i;

	for(i=0; i<8; i+=1)
		addVertex(0.5f * glm::vec3(vertices[3*i], vertices[3*i+1], vertices[3*i+2]));
	for(i=0; i<12; i++)
		addTriangle(faces[3*i], faces[3*i+1], faces[3*i+2]);
	sendToOpenGL();
}

void TriangleMesh::sendToOpenGL()
{
	vector<float> data;
	
	for(unsigned int tri=0; tri<tris.size(); tri+=3)
	{
		glm::vec3 normal;
	
		normal = glm::cross(verts[tris[tri+1]] - verts[tris[tri]], 
	                      verts[tris[tri+2]] - verts[tris[tri]]);
		normal = glm::normalize(normal);
		for(unsigned int vrtx=0; vrtx<3; vrtx++)
		{
			data.push_back(verts[tris[tri + vrtx]].x);
			data.push_back(verts[tris[tri + vrtx]].y);
			data.push_back(verts[tris[tri + vrtx]].z);

			data.push_back(normal.x);
			data.push_back(normal.y);
			data.push_back(normal.z);
		}
	}

	// Send data to OpenGL
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	posLocation = Application::instance().getShader()->bindVertexAttribute("position", 3, 6*sizeof(float), 0);
	normalLocation = Application::instance().getShader()->bindVertexAttribute("normal", 3, 6*sizeof(float), (void *)(3*sizeof(float)));
}

void TriangleMesh::render() const
{
	Application::instance().getShader()->use();

	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(normalLocation);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 3 * tris.size() / 3);
}

void TriangleMesh::free()
{
	if(vbo != -1)
		glDeleteBuffers(1, &vbo);
	if(vao != -1)
		glDeleteVertexArrays(1, &vao);
	
	verts.clear();
	tris.clear();
}



