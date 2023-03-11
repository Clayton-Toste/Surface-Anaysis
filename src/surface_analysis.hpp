#include "vertex.hpp"
#include "distance.hpp"
#include "patch.hpp"

#include <list>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <tgmath.h>
#include <stdlib.h>
#include <chrono>

#include <wx/filefn.h> 

using namespace std;
using namespace std::chrono; 

enum saSELECT_TYPE
{
        ALL, 
        OUTER, 
        CUSTOM
};

class saSurface_Analysis
{
    void handle_meta ( );
    void make_face (Vertex * const vertex1, Vertex * const vertex2, Vertex * const vertex3);
    const char classification_from_color (const double red, const double green, const double blue) const;
    void read_wrl ( );
    void build_patches ( );
    void build_patches_distances ( );
    void build_centers ( );
    void build_surfaces ( );
    void build_distances ( );
    void write_files ( ) const;
    const saSELECT_TYPE select_type;
    double selector_x, selector_y, selector_z, selector_radius;
    Vertex * selected_vertex;
    list<Vertex> vertices;
    list<Patch>  patches;
    unordered_map<pair<Vertex *, Vertex *>, double> distances;
    list<list<Patch*>> surfaces;
public:
    const string protein_name;
    const string data_name;
    saSurface_Analysis(string protein_name, string data_name, saSELECT_TYPE select_type,
                      double selector_x=NAN, 
                      double selector_y=NAN, 
                      double selector_z=NAN, 
                      double selector_radius=NAN) 
                      : protein_name{protein_name}, data_name{data_name}, select_type{select_type}, selector_x{selector_x}, selector_y{selector_y}, selector_z{selector_z}, selector_radius{selector_radius} 
    {
        if ( select_type==CUSTOM && (selector_x==NAN || selector_y==NAN || selector_z==NAN || selector_radius==NAN) )
            throw domain_error("Selectype Custom But no selector info.");
    };
    void run();
};