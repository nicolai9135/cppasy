#ifndef MAIN_FRAME
#define MAIN_FRAME

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "parameter_synthesis.hpp"

class main_frame : public wxFrame
{
private:
    
    // file menu actions
    void OnExit(wxCommandEvent& event);

    // synthesis menu actions
    void OnPreferences(wxCommandEvent& event);
    void OnVariables(wxCommandEvent& event);
    void OnExecute(wxCommandEvent& event);

public:
    main_frame();
    void set_defaults();
    void set_defaults_if_necessary();

    // user options
    gui_options user_settings;

    wxMenu *menu_file;
    wxMenu *menu_synthesis;
    wxMenuBar *menu_bar;
    wxTextCtrl *textctrl;
};

#endif