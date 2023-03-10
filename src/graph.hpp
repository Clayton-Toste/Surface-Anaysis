#pragma once

#define WIDTH 1280
#define HEIGHT 860
#define MARKER_EXTENT bitmapDC.GetTextExtent(marker)

#include <math.h>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/filepicker.h>
#include <wx/statbox.h>
#include <wx/choice.h>

using namespace std;

enum
{
    ID_Option,
    ID_TitleSelect,
    ID_SaveButton,
};

class Graph_Panel;

class Save_Graph : public wxFilePickerCtrl
{
    Graph_Panel * const graph;
public:
    Save_Graph(Graph_Panel * graph, wxWindow * parent) : wxFilePickerCtrl (parent, ID_SaveButton, wxEmptyString, wxFileSelectorPromptStr, "JPEG file (.jpg)|*.jpg",  wxDefaultPosition, wxDefaultSize, wxFLP_SAVE | wxFLP_OVERWRITE_PROMPT), graph(graph) { }
    void event (wxFileDirPickerEvent & event);

    DECLARE_EVENT_TABLE()
};

class Choice_Graph : public wxChoice
{
    Graph_Panel * const graph;
public:
    Choice_Graph(Graph_Panel * graph, wxWindow * parent, int n, const wxString choices[]) : wxChoice ( parent, ID_Option, wxDefaultPosition, wxDefaultSize, n, choices) , graph(graph) { }
    void event (wxCommandEvent & event);

    DECLARE_EVENT_TABLE()
};

class Title_Graph : public wxTextCtrl
{
    Graph_Panel * const graph;
public:
    Title_Graph(Graph_Panel * graph, wxWindow * parent) : wxTextCtrl ( parent, ID_TitleSelect, "TITLE", wxDefaultPosition), graph(graph) { }
    void event (wxCommandEvent & event);

    DECLARE_EVENT_TABLE()
};

class Graph_Panel : public wxPanel
{
    wxString static const mode_choices[2];
    wxString static const classification_choices[5];
    wxBitmap static const distances_image;
    wxBitmap static const sizes_image;

    wxString title;
    wxBitmap bitmap {WIDTH, HEIGHT};

    wxFrame      * const parent;
    Choice_Graph * const mode_choice;
    Choice_Graph * const classification_choice;
    Title_Graph  * const title_select;
    Save_Graph   * const save_button;

    vector<int>          bins;
    double               graph_begin, graph_end;

    bool log {false};

    void render ( wxDC & dc );
    void default_title ( );

public:
                 Graph_Panel        ( wxFrame * parent );
    void         draw               ( );
    void         paint_event        ( wxPaintEvent & evt );
    void         load_data          ( );
    void         save_file          ( );
    void         set_brush          ( wxMemoryDC & bitmapDC, const int choise ) const;
    wxBoxSizer * get_options_sizer  ( );

    wxString data_name;

    friend Title_Graph;
    friend Choice_Graph;
    friend Save_Graph;

    DECLARE_EVENT_TABLE()
};