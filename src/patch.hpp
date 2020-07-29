#pragma once

#include "vertex.hpp"
#include "distance.hpp"
#include <list>
#include <unordered_map>
#include <unordered_set>

struct Patch {
    Patch (char classification)
    { 
        this->classification = classification; 
    };
    ~Patch ()
    {
        for (Patch * & touch: touching)
            touch->touching.remove(this);
    };
    double area {0}; 
    char classification;
    bool selected {false};
    Vertex * center_vertex {nullptr};
    std::unordered_set<Vertex *, vertex_hash, vertex_hash> core, rim;
    std::list<Patch *> touching;
    std::list<Patch *> * surface {nullptr};
    std::unordered_map<std::pair<Vertex *, Vertex *>, double> distances;
};
