
#include <string>
#include <iostream>

#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/choice.h>

enum
{
    ID_mode,
    ID_start,
    ID_x,
    ID_y,
    ID_z,
    ID_radius,
};

class saDialog;

class Mode_Dialog : public wxRadioBox
{
public:
    wxBoxSizer *const select_sizer;
    Mode_Dialog(wxWindow *parent, wxBoxSizer *const select_sizer, int n, const wxString choices[]) : select_sizer{select_sizer}, wxRadioBox(parent, ID_mode, "Select a mode.", wxDefaultPosition, wxDefaultSize, n, choices) {}
    void event(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};

class Start_Button : public wxButton
{
public:
    saDialog *const parent;
    Start_Button(saDialog *parent);
    void event(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};

class Number_Dialog : public wxTextCtrl
{
public:
    Number_Dialog(wxWindow *parent, wxWindowID id) : wxTextCtrl(parent, id) {}
    void event(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};

class saDialog : public wxDialog
{
    wxChoice *protein_choice;
    wxTextCtrl *analyze_name;
    Mode_Dialog *mode_select;
    Start_Button *start;
    wxStaticBox *selector_boxes[4];
    Number_Dialog *selector_numbers[4];

    wxString static const mode_choices[3];

public:
    saDialog(wxWindow *parent);
};