#include "graph.hpp"

BEGIN_EVENT_TABLE(Save_Graph, wxFilePickerCtrl)
 EVT_FILEPICKER_CHANGED(ID_SaveButton, Save_Graph::event)
END_EVENT_TABLE()

void Save_Graph::event(wxFileDirPickerEvent & event)
{
    graph->save_file();
}

BEGIN_EVENT_TABLE(Choice_Graph, wxChoice)
 EVT_CHOICE(ID_Option, Choice_Graph::event)
END_EVENT_TABLE()

void Choice_Graph::event(wxCommandEvent & event)
{
    graph->save_button->SetInitialDirectory(graph->save_button->GetFileName().GetPath());
    graph->load_data();
}

BEGIN_EVENT_TABLE(Title_Graph, wxTextCtrl)
 EVT_TEXT(ID_TitleSelect, Title_Graph::event)
END_EVENT_TABLE()

void Title_Graph::event(wxCommandEvent & event)
{
    graph->title = GetValue();
    graph->draw();
}

BEGIN_EVENT_TABLE(Graph_Panel, wxPanel)
 EVT_PAINT(Graph_Panel::paint_event)
END_EVENT_TABLE()

wxString const Graph_Panel::mode_choices[2] { "Sizes", "Distances" };
wxString const Graph_Panel::classification_choices[5] {"Hydrophobic", "Hydrophilic", "Charged +", "Charged -", "Stacked"};
wxBitmap const Graph_Panel::distances_image {"images/distances.jpg", wxBITMAP_TYPE_JPEG};
wxBitmap const Graph_Panel::sizes_image {"images/sizes.jpg", wxBITMAP_TYPE_JPEG};


Graph_Panel::Graph_Panel( wxFrame * parent ) :
    wxPanel               ( parent ),
    parent                { parent },
    mode_choice           { new Choice_Graph ( this, parent, 2, mode_choices) },
    classification_choice { new Choice_Graph ( this, parent, 5, classification_choices) },
    title_select          { new Title_Graph  ( this, parent ) },
    save_button           { new Save_Graph   ( this, parent ) }
{ 
    mode_choice->SetSelection(1);
    classification_choice->SetSelection(3);
    draw();
}

inline void Graph_Panel::render( wxDC & dc )
{
    wxSize dc_size{dc.GetSize()};
    wxRect drawing_space = (dc_size.GetX()*HEIGHT>dc_size.GetY()*WIDTH ?
           wxRect( (dc_size.GetX()-dc_size.GetY()*WIDTH/HEIGHT)/2, 0, dc_size.GetY()*WIDTH/HEIGHT, dc_size.GetY() ) :
           wxRect( 0, (dc_size.GetY()-dc_size.GetX()*HEIGHT/WIDTH)/2, dc_size.GetX(), dc_size.GetX()*HEIGHT/WIDTH ));
    if (drawing_space.GetX() > 0 || drawing_space.GetY() > 0)
        dc.DrawBitmap( wxBitmap( bitmap.ConvertToImage().Scale( drawing_space.width, drawing_space.height, wxIMAGE_QUALITY_HIGH ) ), drawing_space.x, drawing_space.y );
}

inline void Graph_Panel::set_brush(wxMemoryDC & bitmapDC, const int choise) const
{
    switch (choise)
    {
        case 0:
            bitmapDC.SetBrush(*wxRED_BRUSH);
            return ;
        case 1:
            bitmapDC.SetBrush(*wxBLUE_BRUSH);
            return ;
        case 2:
            bitmapDC.SetBrush(*wxGREEN_BRUSH);
            return ;
        case 3:
            bitmapDC.SetBrush( wxBrush(wxColor(200, 0, 200)) );
            return ;
    }
}

inline void Graph_Panel::draw( )
{
    wxMemoryDC   bitmapDC { bitmap };
    const wxFont font     { bitmapDC.GetFont() };
    const int    choise   { classification_choice->GetSelection() };
    wxString     marker   { to_string(graph_begin) };
    wxSize       extent;
    bitmapDC.SetBackground(*wxWHITE_BRUSH);
    bitmapDC.SetFont( font.Scaled(2.5) );
    bitmapDC.Clear();
    bitmapDC.SetPen(*wxBLACK_PEN);
    if (bins.empty())
    {
        extent = bitmapDC.GetTextExtent("No Data For Selected Options");
        bitmapDC.DrawText("No Data For Selected Options", (WIDTH-extent.GetWidth())/2, 30);
        wxClientDC dc(this);
        render(dc);
        return ;
    }
    int max {*max_element(bins.begin(), bins.end())};
    extent = bitmapDC.GetTextExtent(title);
    bitmapDC.DrawText(title, (WIDTH-extent.GetWidth())/2, 30);
    bitmapDC.DrawLine(wxPoint(WIDTH*.2, HEIGHT*.2), wxPoint(WIDTH*.2, HEIGHT*.81));
    bitmapDC.DrawLine(wxPoint(WIDTH*.19, HEIGHT*.8), wxPoint(WIDTH*.8, HEIGHT*.8));
    bitmapDC.SetFont( font.Scaled(2));
    extent = bitmapDC.GetTextExtent("Frequency");
    bitmapDC.DrawRotatedText("Frequency", WIDTH*.1-extent.GetHeight()*.5, (HEIGHT+extent.GetWidth())*.5, 90);
    set_brush(bitmapDC, choise);
    bitmapDC.SetFont( font );
    if (choise != 4)
    {
        extent = MARKER_EXTENT;
        while ((extent = MARKER_EXTENT).GetWidth()*bins.size()>WIDTH*.48)
        {
            marker = marker.substr(0, marker.size()-1);
        }
        bitmapDC.DrawText(marker, WIDTH*(.2)-extent.GetWidth()/2, HEIGHT*.81);
        for (int i = 0; i < bins.size(); ++i)
        {
            marker = to_string((log ? graph_begin*pow(graph_end/graph_begin, (i+1.0)/bins.size()) : graph_begin+(graph_end-graph_begin)*(i+1.0)/bins.size()));
            while ((extent = MARKER_EXTENT).GetWidth()*bins.size()>WIDTH*.48)
                marker = marker.substr(0, marker.size()-1);
            bitmapDC.DrawText(marker, WIDTH*(.2+(.6*i+.6)/bins.size())-extent.GetWidth()/2, HEIGHT*.81);
            bitmapDC.DrawLine(wxPoint(WIDTH*(.2+(.6*i+.6)/bins.size()), HEIGHT*.8), wxPoint(WIDTH*(.2+(.6*i+.6)/bins.size()), HEIGHT*.81));
            bitmapDC.DrawRectangle(WIDTH*(.2+(.6*i+.06)/bins.size()), HEIGHT*(.8-.6*(double)bins[i]/(double)max), WIDTH*.48/bins.size(),  HEIGHT*.6*((double)bins[i]/(double)max));
        }
    }
    else
    {
        extent = MARKER_EXTENT;
        while ((extent = MARKER_EXTENT).GetWidth()*bins.size()>WIDTH*1.92)
        {
            marker = marker.substr(0, marker.size()-1);
        }
        bitmapDC.DrawText(marker, WIDTH*(.2)-extent.GetWidth()/2, HEIGHT*.81);
        for (int i = 0; i < bins.size(); i+=4)
        {
            marker = to_string((log ? graph_begin*pow(graph_end/graph_begin, (i+4.0)/bins.size()) : graph_begin+(graph_end-graph_begin)*(i+4.0)/bins.size()));
            while ((extent = MARKER_EXTENT).GetWidth()*bins.size()>WIDTH*1.92)
                marker = marker.substr(0, marker.size()-1);
            bitmapDC.DrawText(marker, WIDTH*(.2+(.6*i+2.4)/bins.size())-extent.GetWidth()/2, HEIGHT*.81);
            bitmapDC.DrawLine(wxPoint(WIDTH*(.2+(.6*i+2.4)/bins.size()), HEIGHT*.8), wxPoint(WIDTH*(.2+(.6*i+2.4)/bins.size()), HEIGHT*.81));
            for (int j {3}; j>=0; --j)
            {
                set_brush(bitmapDC, j);
                if (j)
                    bitmapDC.DrawRectangle(WIDTH*(.2+(.6*i+.24)/bins.size()), HEIGHT*(.8-.6*(double)bins[i+j]/(double)max), WIDTH*1.92/bins.size(),  HEIGHT*.6*((double)(bins[i+j]/*-bins[i+j-1]*/)/(double)max));
                else
                    bitmapDC.DrawRectangle(WIDTH*(.2+(.6*i+.24)/bins.size()), HEIGHT*(.8-.6*(double)bins[ i ]/(double)max), WIDTH*1.92/bins.size(),  HEIGHT*.6*((double)bins[i]/(double)max));   
            }
        }
    }
    marker = "0";
    extent = MARKER_EXTENT;
    bitmapDC.DrawText(marker, WIDTH*.19-extent.GetWidth(), HEIGHT*.8-extent.GetHeight()/2);
    marker = to_string(max);
    extent = MARKER_EXTENT;
    bitmapDC.DrawText(marker, WIDTH*.19-extent.GetWidth(), HEIGHT*.2-extent.GetHeight()/2);
    bitmapDC.DrawLine(wxPoint(WIDTH*.2, HEIGHT*.2), wxPoint(WIDTH*.19, HEIGHT*.2));
    if (mode_choice->GetSelection())
        bitmapDC.DrawBitmap(distances_image, (WIDTH-distances_image.GetWidth())*.5, HEIGHT*.9-distances_image.GetHeight()*.5);
    else
        bitmapDC.DrawBitmap(sizes_image, (WIDTH-sizes_image.GetWidth())*.5, HEIGHT*.9-sizes_image.GetHeight()*.5);
    wxClientDC dc(this);
    render(dc);
}

inline void Graph_Panel::paint_event( wxPaintEvent & evt )
{
    wxPaintDC dc{this};
    render(dc);
}

inline void Graph_Panel::default_title( )
{
    title = (mode_choice->GetSelection() ? "Inter-Patch Distances" : " Patch-Sizes");
    title += " for ";
    switch (classification_choice->GetSelection())
    {
    case 0:
        title += "Hydrophobic ";
        break;
    case 1:
        title += "Hydrophilic ";
        break;
    case 2:
        title += "Charged + ";
        break;
    case 3:
        title += "Charged - ";
    }
    title+="Patches of ";
    title+=data_name;
    string file{mode_choice->GetSelection() ? "Distances" : " Sizes"};
    switch (classification_choice->GetSelection())
    {
    case 0:
        file += "Hydrophobic";
        break;
    case 1:
        file += "Hydrophilic";
        break;
    case 2:
        file += "Charged+";
        break;
    case 3:
        file += "Charged-";
        break;
    case 4:
        file += "Stacked";
    }
    file += ".jpg";
    save_button->SetFileName((wxFileName)file);
}

inline void Graph_Panel::load_data( )
{
    if (!data_name)
        return ;
    default_title( );
    set<double> * data[4] {new set<double>};
    data[1] = data[0];
    data[2] = data[0];
    data[3] = data[0];
    string path = "data/"+(string)data_name+"/"+(mode_choice->GetSelection() ? "distances" : "sizes");
    ifstream file{path};
    if (!file)
        throw runtime_error("Error opening file.");
    string temp;
    char code;
    switch (classification_choice->GetSelection())
    {
    case 0:
        code='o';
        break;
    case 1:
        code='i';
        break;
    case 2:
        code='+';
        break;
    case 3:
        code='-';
        break;
    case 4:
        code='a';
        data[1] = new set<double>;
        data[2] = new set<double>;
        data[3] = new set<double>;
    }
    try
    {
        while (file.good())
        {       
            switch (file.get()^code)
            {
                case 0:
                case 14:
                    file>>temp;
                    data[0]->insert(stod(temp));
                    break;
                case 8:
                    file>>temp;
                    data[1]->insert(stod(temp));
                    break;
                case 74:
                    file>>temp;
                    data[2]->insert(stod(temp));
                    break;
                case 76:
                    file>>temp;
                    data[3]->insert(stod(temp));
            }
            file.ignore(255, '\n');
        }
    }
    catch (const invalid_argument& ia) 
    {
        cerr << "Error Reading File: " << ia.what()<< endl;
    }
    bins.clear();
    if (data[0]->empty() && data[1]->empty() && data[2]->empty() && data[3]->empty())
    {
        draw( );
        return ;
    }
    if (code == 'a')
    {
        graph_end = 0;
        graph_begin = numeric_limits<double>::infinity();
        bins.resize(4*int(1+2*log2 (data[0]->size()+data[1]->size()+data[2]->size()+data[3]->size())));
        for (auto ptr : data)
        {
            if (ptr->empty())
                continue;
            if (*ptr->begin()  < graph_begin)
                graph_begin=*ptr->begin();
            if (*ptr->rbegin() > graph_end)
                graph_end=*ptr->rbegin();
        }
        //graph_begin-=.1;
        //graph_end+=.1;
        for (int j = 0; j<4; ++j)
        {
            int i {0};
            for (auto value : *data[j])
            {
                while (i+4<bins.size() && (value > (log ? graph_begin*pow(graph_end/graph_begin, (i+1.0)/bins.size()) : graph_begin+(graph_end-graph_begin)*(i+1.0)/bins.size())))
                    i+=4;
                switch (j)
                {
                    case 0:
                        ++bins[i];
                    case 1:
                        ++bins[i+1];
                    case 2:
                        ++bins[i+2];
                    case 3:
                        ++bins[i+3];
                }
            }
        }
    }
    else
    {
        bins.resize(1+2*log2 (data[0]->size()));
        graph_end   = *data[0]->rbegin();
        graph_begin = *data[0]->begin();
        int i{0};
        for (auto value : *data[0])
        {
            while (i+1 < bins.size() && value > (log ? graph_begin*pow(graph_end/graph_begin, (i+1.0)/bins.size()) : graph_begin+(graph_end-graph_begin)*(i+1.0)/bins.size()))
                ++i;
            ++bins[i];
        }
    }
    delete data[0];
    if (code=='a')
    {
        delete data[1];
        delete data[2];
        delete data[3];
    }
    draw( ); 
}

inline void Graph_Panel::save_file( )
{
    bitmap.SaveFile(save_button->GetPath(), wxBITMAP_TYPE_JPEG);
}

wxBoxSizer * Graph_Panel::get_options_sizer( )
{
    wxBoxSizer       * sizer            { new wxBoxSizer (wxHORIZONTAL) };
    wxStaticBoxSizer * choice_box       { new wxStaticBoxSizer ( wxHORIZONTAL, parent, "Graph Options:" ) };
    wxStaticBoxSizer * title_select_box { new wxStaticBoxSizer ( wxHORIZONTAL, parent, "Graph Title:" ) };
    wxStaticBoxSizer * save_button_box  { new wxStaticBoxSizer ( wxHORIZONTAL, parent, "Save Graph:" ) };
    
    choice_box->Add(mode_choice, 1, wxEXPAND);
    choice_box->Add(classification_choice, 1, wxEXPAND);
    title_select_box->Add(title_select, 1, wxEXPAND);
    save_button_box->Add(save_button, 1, wxEXPAND);

    sizer->Add(choice_box, 1, wxEXPAND);
    sizer->AddSpacer(10);
    sizer->Add(title_select_box, 1, wxEXPAND);
    sizer->AddSpacer(10);
    sizer->Add(save_button_box, 1, wxEXPAND);

    return sizer;
}