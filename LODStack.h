//
// Created by kam on 5/7/24.
//

#ifndef SRGGE_LODSTACK_H
#define SRGGE_LODSTACK_H


#include "TriangleMesh.h"
#include "TriangleMeshInstance.h"
#include <memory>

class LODStack {
    vector<shared_ptr<TriangleMesh>> stack;

public:
    LODStack();
    bool load(const char *filename, const int resolutions[]);


    weak_ptr<TriangleMesh> get_mesh(int level);
};


#endif //SRGGE_LODSTACK_H
