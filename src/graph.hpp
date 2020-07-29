#include <string>
#include <array>
#include <iostream>
#include <fstream>

#include <wx/wfstream.h>
#include <wx/dc.h>

#define WIDTH 640
#define HEIGHT 480

using namespace std;

class Graph : public wxDC
{
public:
    Graph(string data_name, char classificiaton, bool is_sizes, bool log_scale, bool use_radius);
    Graph(string data_name, char classification, bool is_sizes, bool log_scale, bool use_radius, string path);
    inline void load_data(string data_name, char classificaiton, bool is_sizes);
    void update_graph(bool log_scale, bool use_radius);
    bool save_file(const string path)
    {
        return false;
        //return image.GetAsBitmap().SaveFile(path, wxBitmapType::wxBITMAP_TYPE_ANY);
    }
};