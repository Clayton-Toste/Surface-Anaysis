#pragma once

#include <list>
#include <tgmath.h>


#include <unordered_map>
#include <functional>

class Patch;

struct Vertex;

struct vertex_hash
{
   inline std::size_t operator()(Vertex * const & other) const;
   inline std::size_t operator()(int const & x, int const & y, int const & z) const;
   inline bool operator()(Vertex * const & rhs, Vertex * const & lhs) const;
};

struct vertex_set
{
   inline bool operator()(Vertex * const & rhs, Vertex * const & lhs) const;
};

struct Vertex
{
    Vertex(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z; 
    };
    bool operator==(const Vertex &other) const 
    { 
        return x==other.x && y==other.y && z==other.z; 
    };
    double const distance_to(Vertex * other) 
    {
        return sqrt( (x-other->x)*(x-other->x) + (y-other->y)*(y-other->y) + (z-other->z)*(z-other->z) );
    };
    double const distance_to(double x, double y, double z) 
    {
        return sqrt( (x-this->x)*(x-this->x) + (y-this->y)*(y-this->y) + (z-this->z)*(z-this->z) );
    };
    double x, y, z;
    union
    {
        double area {0};
        double distance;
    };
    char classification;
    unsigned int check {0};
    Patch * patch {nullptr};
    bool is_center {false};
    std::unordered_map<Vertex *, double, vertex_hash, vertex_hash> touching;
};

inline std::size_t vertex_hash::operator()(Vertex * const & other) const
{
    using std::hash;
    return ((hash<double>()(other->x))>>1)^
           ((hash<double>()(other->y))   )^
           ((hash<double>()(other->z))<<1);
}

inline std::size_t vertex_hash::operator()(int const & x, int const & y, int const & z) const
{
    using std::hash;
    return ((hash<double>()(x))>>1)^
           ((hash<double>()(y))   )^
           ((hash<double>()(z))<<1);
}

inline bool vertex_hash::operator()(Vertex * const & rhs, Vertex * const & lhs) const
{
    return (lhs->x==rhs->x&&
            lhs->y==rhs->y&&
            lhs->z==rhs->z);
}

inline bool vertex_set::operator()(Vertex * const & rhs, Vertex * const & lhs) const
{
    return lhs->distance > rhs->distance;
}