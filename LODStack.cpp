//
// Created by kam on 5/7/24.
//

#include "LODStack.h"
#include "UniformMeshGrid.h"

LODStack::LODStack() = default;

bool LODStack::load(const char *filename, const int resolutions[]) {
    auto mesh = std::make_shared<TriangleMesh>();
    bool bSuccess = mesh->loadFromFile(filename);

    if (bSuccess) {
        stack.push_back(mesh);
        auto umg = UniformMeshGrid(*mesh);

        for (int i = 0; i < 4; ++i) {
            auto dest = std::make_shared<TriangleMesh>();
            umg.simplify(resolutions[i], *dest);
            stack.push_back(dest);
        }

        for (auto &m : stack) {
            m->sendToOpenGL();
        }
    }

    return bSuccess;
}

void LODStack::set_level(int level) {
    auto mesh = &stack[level];
    instance.init(mesh, glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
    instance.translate(glm::vec3(-1.0f, 0.0f, 0.0f));
}

weak_ptr<TriangleMesh> LODStack::get_mesh(int level) {
    return stack[level];
}
