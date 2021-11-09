#ifndef MAIN_FRAME
#define MAIN_FRAME

#include "cppasy_app.hpp"

class main_frame : public wxFrame
{
private:
    // file menu actions
    // void OnNew(wxCommandEvent& event);
    // void OnOpen(wxCommandEvent& event);
    // void OnSave(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    // synthesis menu actions
    void OnPreferences(wxCommandEvent& event);
    void OnVariables(wxCommandEvent& event);
    void OnExecute(wxCommandEvent& event);

    // help menu actions
    // void OnAbout(wxCommandEvent& event);
    // void OnSMT2(wxCommandEvent& event);

public:
    main_frame();

    wxMenu *menu_file;
    wxMenu *menu_synthesis;
    // wxMenu *menu_help;
    wxMenuBar *menu_bar;
    wxTextCtrl *textctrl;
};

#endif