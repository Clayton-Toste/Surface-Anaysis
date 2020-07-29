#pragma once
#include "vertex.hpp"
#include <cstddef>
#include <functional>

namespace std {
    template<>
    struct hash<std::pair<Vertex *, Vertex *>> {
        inline std::size_t operator()(const std::pair<Vertex *, Vertex *> & other) const 
        {
            using std::hash;
            return ((hash<double>()(other.first->x)^hash<double>()(other.second->x))>>1)^
                   ((hash<double>()(other.first->y)^hash<double>()(other.second->y))   )^
                   ((hash<double>()(other.first->z)^hash<double>()(other.second->z))<<1);
        };
    };
    template<>
    struct equal_to<std::pair<Vertex *, Vertex *>> {
        inline bool operator()(const std::pair<Vertex *, Vertex *> & rhs, const std::pair<Vertex *, Vertex *> lhs) const 
        {
            return (lhs.first == rhs.first && lhs.second == rhs.second) || (lhs.first == rhs.second && lhs.second == rhs.first);
        };
    };
}
