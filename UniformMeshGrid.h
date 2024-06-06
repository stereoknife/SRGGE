//
// Created by kam on 5/6/24.
//

#ifndef SRGGE_UNIFORMMESHGRID_H
#define SRGGE_UNIFORMMESHGRID_H


#include <glm/vec3.hpp>
#include <unordered_map>
#include "TriangleMesh.h"

struct Cell {
    int count;
    int out_index;
    glm::vec3 position;
};

struct Coords {
    int x;
    int y;
    int z;

    bool operator==(const Coords& rhs) const;
};

namespace std {
    template<>
    struct hash<Coords> {
        std::size_t operator()(const Coords &s) const noexcept {
            auto h1 = std::hash<float>{}(s.x);
            auto h2 = std::hash<float>{}(s.y);
            auto h3 = std::hash<float>{}(s.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
};

class UniformMeshGrid {
    glm::vec3 min;
    glm::vec3 max;
    float side;
    float step;
    int resolution;
    unordered_map<Coords, Cell> data;
    TriangleMesh& mesh;

public:
    explicit UniformMeshGrid(TriangleMesh& mesh);
    Coords get_coords(glm::vec3 position);
    void simplify(int resolution, TriangleMesh& dest);

private:
    void initialize(int resolution);
};

#endif //SRGGE_UNIFORMMESHGRID_H
