#ifndef VARIABLES_DIALOG
#define VARIABLES_DIALOG

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "main_frame.hpp"

class preferences_dialog : public wxDialog
{
private:
    std::vector<std::tuple<wxStaticText*, wxTextCtrl*, wxTextCtrl*>> interval_settings;
    wxChoice *x_axis_choice;
    wxChoice *y_axis_choice;
    wxChoice *splitting_choice;
    wxChoice *sampling_choice;
    wxChoice *split_samples_choice;
    wxChoice *save_model_choice;
    wxTextCtrl *depth_ctrl;
    main_frame *m;
public:
    preferences_dialog(main_frame *m);
    void OnSave(wxCommandEvent& event);
};

#endif