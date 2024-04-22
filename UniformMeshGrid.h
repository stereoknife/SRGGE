#ifndef SRGGE_UNIFORMMESHGRID_H
#define SRGGE_UNIFORMMESHGRID_H

#include "TriangleMesh.h"
#include <glm/glm.hpp>
#include <unordered_map>

struct UMGCoords {
    int x;
    int y;
    int z;
};

bool operator==(const UMGCoords& lhs, const UMGCoords& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template<>
struct std::hash<UMGCoords>
{
    std::size_t operator()(const UMGCoords& s) const noexcept
    {
        auto h1 = std::hash<int>{}(s.x);
        auto h2 = std::hash<int>{}(s.y);
        auto h3 = std::hash<int>{}(s.z);
        return h1 ^ h2 ^ h3;
    }
};

struct UMGCell {
    int count;
    int out_index;
    glm::vec3 position;
};

class UniformMeshGrid {
    glm::vec3 min;
    glm::vec3 max;
    float step;
    int resolution;
    unordered_map<UMGCoords, UMGCell> grid;
    vector<glm::vec3> output_verts;
    vector<int> output_tris;

public:
    void initialize(TriangleMesh& mesh, int resolution);
    void save_simplification(TriangleMesh& mesh);
    UMGCoords get_coords(glm::vec3 position);
};


#endif //SRGGE_UNIFORMMESHGRID_H
