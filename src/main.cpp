#include "main.hpp"

BEGIN_EVENT_TABLE(List_Ctrl, wxListCtrl)
EVT_LIST_ITEM_SELECTED(ID_List_Ctrl, List_Ctrl::event)
END_EVENT_TABLE()

bool saApp::OnInit()
{
    wxInitAllImageHandlers();
    
    saFrame * frame {new saFrame};

    frame->Show(true);

    return true;
}

saFrame::saFrame() : wxFrame(NULL, wxID_ANY, "Surface_Analysis")
{   
    SetInitialSize(wxSize(1000, 1000));
    
    wxMenu * menuFile {new wxMenu};
    menuFile->Append(ID_FromPDB, "&From PDB...\tCtrl-P",
                     "Load Protein From PDB Using 4-letter Code.");
    menuFile->Append(ID_FromFile, "&From File...\tCtrl-O",
                     "Load Protein Local File.");
    wxMenu * menuAnalyze {new wxMenu};
    menuAnalyze->Append(ID_Analyze, "&Analyze...\tCtrl-A",
                     "Add an analysis to the queue.");
    menuAnalyze->Append(ID_Run, "&Run...\tCtrl-R",
                     "Run all analyses in the queue.");
    wxMenuBar * menuBar {new wxMenuBar};
    menuBar->Append(menuFile, "&Protein");
    menuBar->Append(menuAnalyze, "&Analyze");

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Idle");

    Bind(wxEVT_MENU, &saFrame::on_from_PDB, this, ID_FromPDB);
    Bind(wxEVT_MENU, &saFrame::on_from_file, this, ID_FromFile);
    Bind(wxEVT_MENU, &saFrame::on_analyze, this, ID_Analyze);
    Bind(wxEVT_MENU, &saFrame::on_run, this, ID_Run);

    wxBoxSizer * main_sizer{ new wxBoxSizer (wxVERTICAL) };
    wxBoxSizer * table_sizer{ new wxBoxSizer (wxHORIZONTAL) };

    protein_list_ctrl->AppendColumn("Protein Name", wxLIST_FORMAT_LEFT, 105);
    protein_list_ctrl->AppendColumn("Protein Code", wxLIST_FORMAT_LEFT, 100);

    data_list_ctrl->AppendColumn("Data Name", wxLIST_FORMAT_LEFT, 85);
    data_list_ctrl->AppendColumn("Protein Name", wxLIST_FORMAT_LEFT, 105);
    data_list_ctrl->AppendColumn("Status", wxLIST_FORMAT_LEFT, 60);
    data_list_ctrl->AppendColumn("Selector Type", wxLIST_FORMAT_LEFT, 105);
    data_list_ctrl->AppendColumn("Selector x", wxLIST_FORMAT_LEFT, 80);
    data_list_ctrl->AppendColumn("Selector y", wxLIST_FORMAT_LEFT, 80);
    data_list_ctrl->AppendColumn("Selector z", wxLIST_FORMAT_LEFT, 80);
    data_list_ctrl->AppendColumn("Selector Radius", wxLIST_FORMAT_LEFT, 115);

    table_sizer->Add(protein_list_ctrl, 205, wxEXPAND);
    table_sizer->AddSpacer(10);
    table_sizer->Add(data_list_ctrl, 710, wxEXPAND);

    main_sizer->Add( table_sizer, 1, wxEXPAND);
    main_sizer->AddSpacer(10);
    main_sizer->Add(graph, 3, wxEXPAND);
    main_sizer->AddSpacer(10);
    main_sizer->Add(graph->get_options_sizer(), 0, wxEXPAND);

    populate_lists();

    SetSizer(main_sizer);
    Layout();
}

void saFrame::populate_lists( )
{
    int i{0};
    for (wxString f = wxFindFirstFile("proteins/*", wxDIR); !f.empty(); f = wxFindNextFile())
    {
        proteins.push_back(f.substr(9, f.size()));
        string path=(string)f+"/.protein";
        ifstream file(path);
        string code;
        file>>code;
        file.close();
        protein_list_ctrl->InsertItem(protein_list_ctrl->GetItemCount(), proteins.back());
        protein_list_ctrl->SetItem(protein_list_ctrl->GetItemCount()-1, 0, proteins.back());
        protein_list_ctrl->SetItem(protein_list_ctrl->GetItemCount()-1, 1, code);
        ++i;
    }
    
}

void saFrame::on_from_PDB(wxCommandEvent& event)
{
    wxString code = wxGetTextFromUser("Please enter the code for the protein");
    wxString name = wxGetTextFromUser("Please enter a name for the protein");
    if (code.IsEmpty() || name.IsEmpty())
    {
        return ;
    }
    saFetch(name, code);
    populate_lists();
}

void saFrame::on_from_file(wxCommandEvent& event)
{
    wxString file = wxFileSelector("Please select a file.");
    wxString name = wxGetTextFromUser("Please enter a name for the protein");
    if (name.IsEmpty() || name.IsEmpty())
    {
        return ;
    }
    saFetch(name, file, true);
    populate_lists();
}

void saFrame::on_analyze(wxCommandEvent& event)
{
    saDialog dialog {this, proteins};

    if (dialog.ShowModal() == wxID_OK)
    {
        switch (dialog.mode_select->GetSelection())
        {
        case 0:
            to_analyze.emplace(string(proteins[dialog.protein_choice->GetSelection()]), string(dialog.analyze_name->GetLineText(0)), OUTER);
            break;
        case 1:
            to_analyze.emplace(string(proteins[dialog.protein_choice->GetSelection()]), string(dialog.analyze_name->GetLineText(0)), ALL);
            break;
        case 2:
            to_analyze.emplace(string(proteins[dialog.protein_choice->GetSelection()]), string(dialog.analyze_name->GetLineText(0)), CUSTOM, 
                stoi(string(dialog.selector_numbers[0]->GetLineText(0))), stoi(string(dialog.selector_numbers[1]->GetLineText(0))), 
                stoi(string(dialog.selector_numbers[2]->GetLineText(0))), stoi(string(dialog.selector_numbers[3]->GetLineText(0))));
            break;
        }
    }

}

void saFrame::on_run(wxCommandEvent& event)
{
    while (!to_analyze.empty())
    {
        saSurface_Analysis surface_analysis = to_analyze.front();
        SetStatusText("Runing analysis for "+surface_analysis.protein_name+'.');
        surface_analysis.run();
        to_analyze.pop();
    }
}