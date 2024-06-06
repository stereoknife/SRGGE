//
// Created by kam on 6/4/24.
//

#ifndef SRGGE_LODINSTANCE_H
#define SRGGE_LODINSTANCE_H


#include "TriangleMeshInstance.h"
#include "LODStack.h"

class LODInstance {
    weak_ptr<LODStack> lod;
    unique_ptr<TriangleMeshInstance> instance;
    glm::vec3 position;

public:
    LODInstance(weak_ptr<LODStack> lod);
    void render();
};


#endif //SRGGE_LODINSTANCE_H
