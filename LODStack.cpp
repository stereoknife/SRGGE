//
// Created by kam on 5/7/24.
//

#include "LODStack.h"
#include "UniformMeshGrid.h"

LODStack::LODStack(TriangleMesh* stack) : instance{} {
    this->stack = stack;
}

void LODStack::set_position(glm::vec3 position) {
    this->position = position;
}

void LODStack::set_colour(glm::vec4 colour) {
    this->colour = colour;
}

void LODStack::set_level(int level) {
    instance.init(&stack[level], colour);
    instance.translate(position);
}

void LODStack::render() {
    instance.render();
}
