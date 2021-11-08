#ifndef MAIN_FRAME
#define MAIN_FRAME

#include "cppasy_app.hpp"

class main_frame : public wxFrame
{
public:
    main_frame();
private:
    // file menu actions
    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    // synthesis menu actions
    void OnPreferences(wxCommandEvent& event);
    void OnVariables(wxCommandEvent& event);

    // help menu actions
    void OnAbout(wxCommandEvent& event);
    void OnSMT2(wxCommandEvent& event);
};

#endif