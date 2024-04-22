#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "VectorCamera.h"
#include "TriangleMeshInstance.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	bool loadMesh(const char *filename);
	void update(int deltaTime);
	void render();

	VectorCamera &getCamera();

private:
	void buildRoom();
    void buildDefaultRoom();
    bool buildRoomFromFile();

private:
	VectorCamera camera;
	TriangleMesh *mesh, *cube;
	vector<TriangleMeshInstance *> meshInstances;
	vector<TriangleMeshInstance *> roomCubes;
	float currentTime;

};


#endif // _SCENE_INCLUDE
