//
// Created by kam on 5/6/24.
//

#include <iostream>
#include "UniformMeshGrid.h"

UniformMeshGrid::UniformMeshGrid(TriangleMesh &mesh) : data{}, mesh{mesh} {
    // Get AABB max & min values
    for (auto v : mesh.vertices()) {
        max = glm::max(max, v);
        min = glm::min(min, v);
    }
    //cout << "Mesh cube - min: " << min << " max: " << max << endl;

    // Extend to cube
    glm::vec3 diff = max - min;
    side = glm::max(glm::max(diff.x, diff.y), diff.z);
    max = min + side;
}

void UniformMeshGrid::initialize(int resolution) {
    this->resolution = resolution;
    step = side / (float)resolution;
}

void UniformMeshGrid::simplify(int resolution, TriangleMesh& dest) {
    initialize(resolution);

    // Iterate thru vertices
    for (auto& v : mesh.vertices()) {
        // Get grid coord for vertex
        auto coords = get_coords(v);
        // Add vertex position to cell
        auto cell = data[coords];
        cell.position += v;
        cell.count++;
        data[coords] = cell;
    }

    // Create output structs
    int i_tri = 0;

    // Iterate thru grid cells
    //cout << "Iterating grid cells" << endl;
    for (auto& c : data) {
        if (c.second.count == 0) continue;

        // Get average position
        c.second.position /= (float)c.second.count;
        // Store output vertex index
        c.second.out_index = i_tri++;
        dest.addVertex(c.second.position);

        //cout << "Vert position: " << c.second.position.x << ", " << c.second.position.y << ", " << c.second.position.z << endl;
    }

    auto tris = mesh.triangles();
    auto verts = mesh.vertices();

    for (int i = 0; i < tris.size();) {
        // Get cell coords
        auto t0 = tris[i++];
        auto t1 = tris[i++];
        auto t2 = tris[i++];

        auto v0 = verts[t0];
        auto v1 = verts[t1];
        auto v2 = verts[t2];

        //cout << "co1" << endl;
        auto co0 = get_coords(v0);
        //cout << "co2" << endl;
        auto co1 = get_coords(v1);
        //cout << "co3" << endl;
        auto co2 = get_coords(v2);

        // Check that it's not degenerate
        //cout << "Check for degenerate triangle" << endl;
        if (co0 == co1 || co1 == co2 || co2 == co0) continue;

        //cout << "Get new indices" << endl;
        // Get new triangle index
        auto c0 = data[co0];
        auto c1 = data[co1];
        auto c2 = data[co2];

        //cout << "Output indices" << endl;
        // Output triangle index
        //cout << "Triangle indices: " << c0.out_index << ", " << c1.out_index << ", " << c2.out_index << endl;
        dest.addTriangle(c0.out_index, c1.out_index, c2.out_index);
    }

    /*
    for (auto& v : verts) {
        dest.addVertex(v);
    }

    for (int i = 0; i < tris.size();) {
        auto t0 = tris[i++];
        auto t1 = tris[i++];
        auto t2 = tris[i++];

        dest.addTriangle(t0, t1, t2);
    }
     */
}

bool Coords::operator==(const Coords &rhs) const {
    return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
}

Coords UniformMeshGrid::get_coords(glm::vec3 position) {
    glm::vec3 f_coords = glm::floor((position - min) / step);
    return {(int)f_coords.x, (int)f_coords.y, (int)f_coords.z};
    //return {position.x, position.y, position.z};
}