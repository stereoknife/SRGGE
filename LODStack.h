//
// Created by kam on 5/7/24.
//

#ifndef SRGGE_LODSTACK_H
#define SRGGE_LODSTACK_H


#include "TriangleMesh.h"
#include "TriangleMeshInstance.h"

class LODStack {
    TriangleMesh* stack;
    TriangleMeshInstance instance;
    glm::vec3 position;
    glm::vec4 colour;

public:
    LODStack(TriangleMesh* stack);
    void set_position(glm::vec3 position);
    void set_colour(glm::vec4 colour);
    void set_level(int level);
    void render();
};


#endif //SRGGE_LODSTACK_H
