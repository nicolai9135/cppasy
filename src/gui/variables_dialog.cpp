#include "variables_dialog.hpp"
#include "smtlib_parse.hpp"

variables_dialog::variables_dialog(main_frame *m)
  : wxDialog(NULL, wxID_ANY, "Set Variable Boundaries")
{
    wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    m->user_settings.formula_str = m->textctrl->GetValue().ToStdString();
    m->user_settings.variable_names = get_variable_names(m->user_settings.formula_str);

    // rows, cols, vgap, hgap
    wxFlexGridSizer *fgs = new wxFlexGridSizer((int) m->user_settings.variable_names.size(), 3, 9, 25);

    std::vector<wxStaticText*> wx_variable_names;
    std::vector<wxTextCtrl*> wx_input_lower;
    std::vector<wxTextCtrl*> wx_input_upper;

    for(const auto &v : m->user_settings.variable_names)
    {
        wx_variable_names.push_back(new wxStaticText(panel, -1, v));
        fgs->Add(wx_variable_names.back());
        wx_input_lower.push_back(new wxTextCtrl(panel, -1));
        fgs->Add(wx_input_lower.back(), 1, wxEXPAND);
        wx_input_upper.push_back(new wxTextCtrl(panel, -1));
        fgs->Add(wx_input_upper.back(), 1, wxEXPAND);
    }

    /*
    fgs->Add(thetitle);
    fgs->Add(tc1, 1, wxEXPAND);
    fgs->Add(author);
    fgs->Add(tc2, 1, wxEXPAND);
    fgs->Add(review, 1, wxEXPAND);
    fgs->Add(tc3, 1, wxEXPAND);
    */

    // fgs->AddGrowableRow(2, 1);
    //fgs->AddGrowableCol(1, 1);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    panel->SetSizer(hbox);
    Centre();
}