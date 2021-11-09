#include "cppasy_app.hpp"
#include "main_frame.hpp"

main_frame::main_frame()
    : wxFrame(NULL, wxID_ANY, "cppasy")
{
    // create file menu
    menu_file = new wxMenu;
    // menu_file->Append(wxID_NEW);
    // menu_file->AppendSeparator();
    // menu_file->Append(wxID_OPEN);
    // menu_file->AppendSeparator();
    // menu_file->Append(wxID_SAVE);
    // menu_file->AppendSeparator();
    menu_file->Append(wxID_EXIT);

    // create synthesis menu
    menu_synthesis = new wxMenu;
    menu_synthesis->Append(wxID_PREFERENCES);
    menu_synthesis->AppendSeparator();
    menu_synthesis->Append(id_variables, "Variable Boundaries...", "Set initial variable boundaries (default: [0,1])");
    menu_synthesis->AppendSeparator();
    menu_synthesis->Append(wxID_EXECUTE);

    // create help menu
    // menu_help = new wxMenu;
    // menu_help->Append(wxID_ABOUT);
    // menu_help->AppendSeparator();
    // menu_help->Append(id_help_smt2, "'.smt2' Formatting", "External link to the '.smt2' language");

    // create menu bar
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&File");
    menu_bar->Append(menu_synthesis, "Synthesis");
    // menu_bar->Append(menu_help, "&Help");

    SetMenuBar( menu_bar );
    CreateStatusBar();
    SetStatusText("Welcome to cppasy");

    // file binds
    // Bind(wxEVT_MENU, &main_frame::OnNew, this, wxID_NEW);
    // Bind(wxEVT_MENU, &main_frame::OnOpen, this, wxID_OPEN);
    // Bind(wxEVT_MENU, &main_frame::OnSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &main_frame::OnExit, this, wxID_EXIT);

    // synthesis binds
    Bind(wxEVT_MENU, &main_frame::OnPreferences, this, wxID_PREFERENCES);
    Bind(wxEVT_MENU, &main_frame::OnVariables, this, id_variables);
    Bind(wxEVT_MENU, &main_frame::OnExecute, this, wxID_EXECUTE);

    // help binds
    // Bind(wxEVT_MENU, &main_frame::OnAbout, this, wxID_ABOUT);
    // Bind(wxEVT_MENU, &main_frame::OnSMT2, this, id_help_smt2);

    textctrl = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1), wxSize(400, 400), wxTE_MULTILINE);

    // TODO: make teletype work!!!
    // textctrl->SetDefaultStyle(wxTextAttr(*wxRED));
    // textctrl->AppendText("Red text\n");
    // textctrl->SetDefaultStyle(wxTextAttr(wxFONTFAMILY_TELETYPE));
    // textctrl->AppendText("doesnt look like teletype\n");
}

// file
// void main_frame::OnNew(wxCommandEvent& event)
// {
//     wxLogMessage("TODO");
// }
// 
// void main_frame::OnOpen(wxCommandEvent& event)
// {
//     wxLogMessage("TODO");
// }
// 
// void main_frame::OnSave(wxCommandEvent& event)
// {
//     wxLogMessage("TODO");
// }

void main_frame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

// synthesis
void main_frame::OnPreferences(wxCommandEvent& event)
{
    wxLogMessage("TODO");
}

void main_frame::OnVariables(wxCommandEvent& event)
{
    wxLogMessage("TODO");
}

void main_frame::OnExecute(wxCommandEvent& event)
{
    wxLogMessage("TODO");
}

// help
// void main_frame::OnAbout(wxCommandEvent& event)
// {
//     wxLogMessage("TODO");
// }
// 
// void main_frame::OnSMT2(wxCommandEvent& event)
// {
//     wxLogMessage("TODO");
// }