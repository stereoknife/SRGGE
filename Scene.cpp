//#include <iostream>
#include <fstream>
#include <iostream>
#include "Scene.h"
#include "UniformMeshGrid.h"


Scene::Scene() : meshes{ nullptr }
{
	cube = nullptr;
}

Scene::~Scene()
{
	for(vector<TriangleMeshInstance *>::iterator it=roomCubes.begin(); it!=roomCubes.end(); it++)
		delete *it;
	for(vector<TriangleMeshInstance *>::iterator it=meshInstances.begin(); it!=meshInstances.end(); it++)
		delete *it;
	if(cube != nullptr)
		delete cube;
}


// Initialize the scene. This includes the cube we will use to render
// the floor and walls, as well as the camera.

void Scene::init()
{
	buildRoom();
	camera.init(glm::vec3(0.f, 0.75f, 2.f), 180.f);
	currentTime = 0.0f;
}

// Loads the mesh into CPU memory and sends it to GPU memory (using GL)

bool Scene::loadMesh(const char *filename)
{
    const int resolutions[4] = {200, 150, 100, 50};

    /*
    TriangleMesh mesh{};
    bool bSuccess = mesh.loadFromFile(filename);

    if (bSuccess) {
        auto umg = UniformMeshGrid(mesh);

        for (int i = 0; i < 4; ++i) {
            if (meshes[i] != nullptr) {
                meshes[i]->free();
                delete meshes[i];
            }

            auto dest = new TriangleMesh();
            umg.simplify(resolutions[i], *dest);
            //dest->sendToOpenGL();
            meshes[i] = dest;
        }

        meshes[3]->sendToOpenGL();
        auto instance = new TriangleMeshInstance {};
        instance->init(meshes[3], glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
        instance->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
        meshInstances.push_back(instance);
    }
    */
    if (mesh != nullptr) {
        mesh->free();
        delete mesh;
    }

    meshes[0] = new TriangleMesh{};
    auto src = TriangleMesh{};
    bool bSuccess = src.loadFromFile(filename);

    if (bSuccess) {
        auto umg = UniformMeshGrid(src);

        for (int i = 0; i < 4; ++i) {
            if (meshes[i] != nullptr) {
                meshes[i]->free();
                delete meshes[i];
            }

            auto dest = new TriangleMesh{};
            umg.simplify(resolutions[i], *dest);
            meshes[i] = dest;
            meshes[i]->sendToOpenGL();
        }

        auto instance = new TriangleMeshInstance{};
        instance->init(meshes[1], glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
        instance->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
        meshInstances.push_back(instance);
    }

    //src->free();
    //delete src;

	return bSuccess;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

// Render the scene. First the room, then the mesh it there is one loaded.

void Scene::render()
{
	camera.sendToOpenGL();
	for(vector<TriangleMeshInstance *>::iterator it=roomCubes.begin(); it!=roomCubes.end(); it++)
		(*it)->render();
    for(vector<TriangleMeshInstance *>::iterator it=meshInstances.begin(); it!=meshInstances.end(); it++)
        (*it)->render();
    for (auto lod : lods) {
        lod.render();
    }
}

VectorCamera &Scene::getCamera()
{
	return camera;
}

// Render the room. Both the floor and the walls are instances of the
// same initial cube scaled and translated to build the room.

void Scene::buildRoom()
{
    if (!buildRoomFromFile()) buildDefaultRoom();
}

void Scene::buildDefaultRoom()
{
    //TODO: Custom room layout

	TriangleMeshInstance *cubeInstance;

	// Build cube mesh
	cube = new TriangleMesh();
	cube->buildCube();

	// Create all its instances to build the room
    // Floor
	cubeInstance = new TriangleMeshInstance();
	cubeInstance->init(cube, glm::vec4(0.8f, 0.95f, 0.95f, 1.0f));
	cubeInstance->scale(glm::vec3(20.f, 1.f, 20.f));
	cubeInstance->translate(glm::vec3(0.f, -0.5f, 0.f));
	roomCubes.push_back(cubeInstance);

    // Wall
	cubeInstance = new TriangleMeshInstance();
	cubeInstance->init(cube, glm::vec4(0.8f, 0.95f, 0.95f, 1.0f));
	cubeInstance->scale(glm::vec3(20.f, 2.f, 1.f));
	cubeInstance->translate(glm::vec3(0.f, 1.f, -9.5f));
	roomCubes.push_back(cubeInstance);

    // Wall
	cubeInstance = new TriangleMeshInstance();
	cubeInstance->init(cube, glm::vec4(0.8f, 0.95f, 0.95f, 1.0f));
	cubeInstance->scale(glm::vec3(20.f, 2.f, 1.f));
	cubeInstance->translate(glm::vec3(0.f, 1.f, 9.5f));
	roomCubes.push_back(cubeInstance);

    // Wall
	cubeInstance = new TriangleMeshInstance();
	cubeInstance->init(cube, glm::vec4(0.8f, 0.95f, 0.95f, 1.0f));
	cubeInstance->scale(glm::vec3(1.f, 2.f, 20.f));
	cubeInstance->translate(glm::vec3(-9.5f, 1.f, 0.f));
	roomCubes.push_back(cubeInstance);

    // Wall
	cubeInstance = new TriangleMeshInstance();
	cubeInstance->init(cube, glm::vec4(0.8f, 0.95f, 0.95f, 1.0f));
	cubeInstance->scale(glm::vec3(1.f, 2.f, 20.f));
	cubeInstance->translate(glm::vec3(9.5f, 1.f, 0.f));
	roomCubes.push_back(cubeInstance);
}

bool Scene::buildRoomFromFile()
{
    std::ifstream room;
    room.open("room.txt", ios::in);

    if (!room.is_open()) return false;

    int x = 0, z = 0;
    string line;
    TriangleMeshInstance *cubeInstance;

    cube = new TriangleMesh();
    cube->buildCube();

    while(getline(room, line))
    {
        x = 0;
        for(auto c : line) {
            switch (c) {
                case '0':
                    break;
                case '1':
                    cubeInstance = new TriangleMeshInstance();
                    cubeInstance->init(cube, glm::vec4(0.8f, 0.95f, 0.95f, 1.0f));
                    cubeInstance->scale(glm::vec3(1.f, 2.f, 1.f));
                    cubeInstance->translate(glm::vec3(x, 1.f, z));
                    roomCubes.push_back(cubeInstance);
                    break;
                default:
                    break;
            }
            x++;
        }
        z++;
    }

    cubeInstance = new TriangleMeshInstance();
    cubeInstance->init(cube, glm::vec4(0.8f, 0.95f, 0.95f, 1.0f));
    cubeInstance->scale(glm::vec3(x, 1.f, z));
    cubeInstance->translate(glm::vec3(((float)x)/2, -0.5f, ((float)z)/2));
    roomCubes.push_back(cubeInstance);

    room.close();

    return true;
}




