#pragma once

#include "surface_analysis.hpp"
#include "graph.hpp"
#include "fetch.hpp"
#include "dialog.hpp"

#include <iostream>
#include <queue>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/filepicker.h>
#include <wx/busyinfo.h>

enum
{
    ID_FromPDB  = 1,
    ID_FromFile = 2,
    ID_Analyze  = 3,
    ID_Run      = 5,

    ID_List_Ctrl = 101,
};

struct List_Base
{
    virtual void list_update(wxListEvent& event)=0;
};

class List_Ctrl : public wxListCtrl, public List_Base
{
    List_Base * to_update;

public:

    void event(wxListEvent& event)
    {
        to_update->list_update(event);
    }

    List_Ctrl(wxWindow * parent, List_Base * to_update) : wxListCtrl(parent, ID_List_Ctrl, wxDefaultPosition, wxDefaultSize, wxLC_REPORT), to_update{to_update} {}

    void list_update(wxListEvent& event)
    {
        DeleteAllItems( );
        string line;
        for (wxString f = wxFindFirstFile("data/*", wxDIR); !f.empty(); f = wxFindNextFile())
        {
            wxString name = f.substr(5, f.size());
            string path = (string)f+"/.data";
            ifstream file {path};
            getline(file, line);
            if (line!=event.GetItem().GetText())
                continue;
            InsertItem(GetItemCount(), name);
            SetItem(GetItemCount()-1, 0, name);
            SetItem(GetItemCount()-1, 1, event.GetItem().GetText());
            SetItem(GetItemCount()-1, 2, "Completed");
            getline(file, line);
            SetItem(GetItemCount()-1, 3, line);
            if (line=="custom")
            {
                getline(file, line);
                SetItem(GetItemCount()-1, 4, line);
                getline(file, line);
                SetItem(GetItemCount()-1, 5, line);
                getline(file, line);
                SetItem(GetItemCount()-1, 6, line);
                getline(file, line);
                SetItem(GetItemCount()-1, 7, line);
            }
            else
            {
                SetItem(GetItemCount()-1, 4, "NA");
                SetItem(GetItemCount()-1, 5, "NA");
                SetItem(GetItemCount()-1, 6, "NA");
                SetItem(GetItemCount()-1, 7, "NA");
            }
            
        }
    }

    DECLARE_EVENT_TABLE()
};

class saApp : public wxApp
{
public:
    virtual bool OnInit();
};

class saFrame : public wxFrame, public List_Base
{
    void populate_lists ( );

    void on_from_PDB  ( wxCommandEvent& event );
    void on_from_file ( wxCommandEvent& event );
    void on_analyze   ( wxCommandEvent& event );
    void on_run       ( wxCommandEvent& event );
    void list_update  ( wxListEvent& event ) 
    {
        graph->data_name = event.GetItem().GetText();
        graph->load_data();
    };

    std::queue<saSurface_Analysis>    to_analyze;
    std::vector<wxString>        proteins, datas;               

    List_Ctrl    * const data_list_ctrl    { new List_Ctrl  ( this, this ) };
    List_Ctrl    * const protein_list_ctrl { new List_Ctrl  ( this, data_list_ctrl ) };
    Graph_Panel  * const graph             { new Graph_Panel ( this ) };

public:
    saFrame();
};

wxIMPLEMENT_APP(saApp);
