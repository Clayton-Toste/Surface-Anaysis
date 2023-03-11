#include "dialog.hpp"

BEGIN_EVENT_TABLE(Mode_Dialog, wxRadioBox)
EVT_RADIOBOX(ID_mode, Mode_Dialog::event)
END_EVENT_TABLE()

void Mode_Dialog::event(wxCommandEvent &event)
{
    select_sizer->Show(GetSelection() == 2);
}

Start_Button::Start_Button(saDialog *parent) : parent{parent}, wxButton((parent), ID_start, "Start Analysis") {}

BEGIN_EVENT_TABLE(Start_Button, wxButton)
EVT_BUTTON(ID_start, Start_Button::event)
END_EVENT_TABLE()

void Start_Button::event(wxCommandEvent &event)
{
    parent->EndModal(wxID_OK);
}

BEGIN_EVENT_TABLE(Number_Dialog, wxTextCtrl)
EVT_TEXT(wxID_ANY, Number_Dialog::event)
END_EVENT_TABLE()

void Number_Dialog::event(wxCommandEvent &event)
{
    wxString result, current = GetValue();
    bool decimal{true};
    for (int i = 0; i < current.Len(); i++)
    {
        char c = current.GetChar(i);
        switch (((48 <= c && c <= 57) || (45 == c && i == 0)) + 2 * (46 == c && decimal))
        {
        case 2:
            decimal = false;
        case 1:
            result.Append(current.GetChar(i));
        }
    }
    if (result != current)
    {
        ChangeValue(result);
    }
}

wxString const saDialog::mode_choices[3]{"Outer", "All", "Custom"};

saDialog::saDialog(wxWindow *parent, std::vector<wxString> &proteins) : wxDialog(parent, wxID_ANY, "Start Surface Analysis")
{
    wxBoxSizer *main_sizer{new wxBoxSizer(wxVERTICAL)};
    wxBoxSizer *name_sizer{new wxBoxSizer(wxHORIZONTAL)};
    wxBoxSizer *select_sizer{new wxBoxSizer(wxHORIZONTAL)};

    protein_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, proteins.size(), &proteins[0]);
    analyze_name = new wxTextCtrl(this, wxID_ANY);
    mode_select = new Mode_Dialog(this, select_sizer, 3, mode_choices);
    start = new Start_Button(this);

    selector_boxes[0] = new wxStaticBox(this, wxID_ANY, "x");
    selector_boxes[1] = new wxStaticBox(this, wxID_ANY, "y");
    selector_boxes[2] = new wxStaticBox(this, wxID_ANY, "z");
    selector_boxes[3] = new wxStaticBox(this, wxID_ANY, "Radius");
    selector_numbers[0] = new Number_Dialog(selector_boxes[0], ID_x);
    selector_numbers[1] = new Number_Dialog(selector_boxes[1], ID_y);
    selector_numbers[2] = new Number_Dialog(selector_boxes[2], ID_z);
    selector_numbers[3] = new Number_Dialog(selector_boxes[3], ID_radius);

    name_sizer->AddSpacer(10);
    name_sizer->Add(protein_choice, 1, wxEXPAND);
    name_sizer->AddSpacer(10);
    name_sizer->Add(analyze_name, 1, wxEXPAND);
    name_sizer->AddSpacer(10);

    select_sizer->AddSpacer(10);
    select_sizer->Add(selector_boxes[0], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
    select_sizer->AddSpacer(10);
    select_sizer->Add(selector_boxes[1], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
    select_sizer->AddSpacer(10);
    select_sizer->Add(selector_boxes[2], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
    select_sizer->AddSpacer(10);
    select_sizer->Add(selector_boxes[3], 1, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN);
    select_sizer->AddSpacer(10);

    main_sizer->AddSpacer(10);
    main_sizer->Add(name_sizer, 1, wxEXPAND);
    main_sizer->Add(mode_select, 1, wxEXPAND);
    main_sizer->Add(select_sizer, 1, wxEXPAND);
    main_sizer->AddSpacer(10);
    main_sizer->Add(start, 1, wxALIGN_CENTER);
    main_sizer->AddSpacer(10);

    SetSizer(main_sizer);

    select_sizer->Show(false);
}