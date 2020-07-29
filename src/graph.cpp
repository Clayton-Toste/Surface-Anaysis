#include "graph.hpp"

inline void Graph::load_data(string data_name, char classificaiton, bool is_sizes)
{
    ifstream file{"data/"+data_name+'/'+ (is_sizes ? ".sizes" : ".distances")};
}

void Graph::update_graph(bool log_scale, bool use_radius)
{
    save_file("test.png");
}

Graph::Graph(string data_name, char classificaiton, bool is_sizes, bool log_scale, bool use_radius) :
{
    load_data(data_name, classificaiton, is_sizes);
    update_graph(log_scale, use_radius);
};

Graph::Graph(string data_name, char classificaiton, bool is_sizes, bool log_scale, bool use_radius, string path) : wxDC(wxDCImpl(this))
{
    load_data(data_name, classificaiton, is_sizes);
    update_graph(log_scale, use_radius);
    save_file(path);
};