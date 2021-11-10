#include "cppasy_app.hpp"
#include "main_frame.hpp"
#include "plot_frame.hpp"
#include "preferences_frame.hpp"
#include "variables_frame.hpp"

main_frame::main_frame()
  : wxFrame(NULL, wxID_ANY, "cppasy")
{
    // create file menu
    menu_file = new wxMenu;
    menu_file->Append(wxID_EXIT);

    // create synthesis menu
    menu_synthesis = new wxMenu;
    menu_synthesis->Append(wxID_PREFERENCES);
    menu_synthesis->AppendSeparator();
    menu_synthesis->Append(id_variables, "Variable Boundaries...", "Set initial variable boundaries (default: [0,1])");
    menu_synthesis->AppendSeparator();
    menu_synthesis->Append(wxID_EXECUTE);

    // create menu bar
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&File");
    menu_bar->Append(menu_synthesis, "Synthesis");

    SetMenuBar( menu_bar );
    CreateStatusBar();
    SetStatusText("Welcome to cppasy");

    // file binds
    Bind(wxEVT_MENU, &main_frame::OnExit, this, wxID_EXIT);

    // synthesis binds
    Bind(wxEVT_MENU, &main_frame::OnPreferences, this, wxID_PREFERENCES);
    Bind(wxEVT_MENU, &main_frame::OnVariables, this, id_variables);
    Bind(wxEVT_MENU, &main_frame::OnExecute, this, wxID_EXECUTE);

    textctrl = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1), wxSize(400, 400), wxTE_MULTILINE);

    // TODO: make teletype work!!!
    // textctrl->SetDefaultStyle(wxTextAttr(*wxRED));
    // textctrl->AppendText("Red text\n");
    // textctrl->SetDefaultStyle(wxTextAttr(wxFONTFAMILY_TELETYPE));
    // textctrl->AppendText("doesnt look like teletype\n");
}

void main_frame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

// synthesis
void main_frame::OnPreferences(wxCommandEvent& event)
{
    preferences_frame *pref_f = new preferences_frame();
    pref_f->Show(true);
}

void main_frame::OnVariables(wxCommandEvent& event)
{
    variables_frame *var_f = new variables_frame();
    var_f->Show(true);
}

void main_frame::OnExecute(wxCommandEvent& event)
{
    plot_frame *plot_f = new plot_frame();
    plot_f->Show(true);
}