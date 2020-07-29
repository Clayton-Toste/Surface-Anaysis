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

class Surface_Anaysis
{
    void make_face(Vertex * vertex1, Vertex * vertex2, Vertex * vertex3);
    char classification_from_color(double red, double green, double blue);
    void read_wrl();
    void build_patches();
    void build_patches_distances();
    void build_centers();
    void filter_patches();
    void build_surfaces();
    void build_distances();
    void const write_files();
    const string protein_name;
    const string data_name;
    ofstream * const log_file;
    const enum{all, outer, custom} select_type;
    double selector_x, selector_y, selector_z, selector_radius;
    list<Vertex> vertices;
    list<Patch> patches;
    unordered_map<pair<Vertex *, Vertex *>, double> distances;
    list<list<Patch*>> surfaces;
public:
    Surface_Anaysis(string protein_name, string data_name, ofstream * log, bool outer) : protein_name{protein_name}, data_name{data_name}, select_type{(outer ? this->outer : all)}, log_file{log}, selector_x{0}, selector_y{0}, selector_z{0}, selector_radius{0} {};
    Surface_Anaysis(string protein_name, string data_name, ofstream * log, double x, double y, double z, double radius) : protein_name{protein_name}, data_name{data_name}, log_file{log}, select_type{custom}, selector_x{x}, selector_y{y}, selector_z{z}, selector_radius{radius} {};
    void run();
};