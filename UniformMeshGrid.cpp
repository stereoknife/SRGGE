#include "UniformMeshGrid.h"

void UniformMeshGrid::initialize(TriangleMesh& mesh, int res) {
    // set resolution
    resolution = res;

    // init max & min values
    max = glm::vec3(-9999999);
    min = glm::vec3(9999999);

    // Get AABB max & min values
    for (auto v : mesh.vertices()) {
        max = glm::max(max, v);
        min = glm::min(min, v);
    }

    // Extend to cube
    glm::vec3 diff = max - min;
    float max_diff = glm::max(glm::max(diff.x, diff.y), diff.z);
    max = min + max_diff;

    // Calculate cell size
    step = max_diff / (float)resolution;

    // Iterate thru input vertices
    for (auto v : mesh.vertices()) {
        // Compute which cell they're in
        UMGCoords coords = get_coords(v);

        // Accumulate in cell (sum and count)
        UMGCell& cell = grid[coords];
        cell.count++;
        cell.position += v;
        //grid[coords] = cell;
    }

    // Iterate thru grid cells
    for (auto& c : grid) {
        // Get average position
        c.second.position /= c.second.count;
        // Add to output vertices
        output_verts.push_back(c.second.position);
        // Store output vertex index
        c.second.out_index = output_verts.size();
    }

    // Iterate thru in triangles
    for (int i = 0; i <= mesh.triangles().size(); i++) {
        // Get cell coords
        auto t0 = mesh.vertices()[i++];
        auto t1 = mesh.vertices()[i++];
        auto t2 = mesh.vertices()[i++];

        auto co0 = get_coords(t0);
        auto co1 = get_coords(t1);
        auto co2 = get_coords(t2);

        // Check that it's not degenerate
        if (co0 == co1 || co1 == co2 || co2 == co0) continue;

        // Get new triangle index
        auto c0 = grid[co0];
        auto c1 = grid[co1];
        auto c2 = grid[co2];

        // Output triangle index
        output_tris.push_back(c0.out_index);
        output_tris.push_back(c1.out_index);
        output_tris.push_back(c2.out_index);
    }

    save_simplification(mesh);
}

UMGCoords UniformMeshGrid::get_coords(glm::vec3 position) {
    glm::vec3 f_coords = glm::floor((position - min) / step);
    return {(int)f_coords.x, (int)f_coords.y, (int)f_coords.z};
}

void UniformMeshGrid::save_simplification(TriangleMesh& mesh) {
    mesh.vertices() = output_verts;
    mesh.triangles() = output_tris;
}
