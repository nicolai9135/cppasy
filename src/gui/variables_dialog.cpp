#include "variables_dialog.hpp"
#include "smtlib_parse.hpp"

variables_dialog::variables_dialog(main_frame *m)
  : wxDialog(NULL, wxID_ANY, "Set Variable Boundaries")
{
    // panel: contains everything, parent frame for 
    wxPanel *panel = new wxPanel(this, -1);

    // hbox: contains gridsizer to add margins
    wxBoxSizer *hbox = new wxBoxSizer(wxVERTICAL);

    m->user_settings.formula_str = m->textctrl->GetValue().ToStdString();
    m->user_settings.variable_names = get_variable_names(m->user_settings.formula_str);

    // rows, cols, vgap, hgap
    wxFlexGridSizer *fgs = new wxFlexGridSizer((int) m->user_settings.variable_names.size()+1, 3, 9, 25);

    fgs->Add(new wxStaticText(panel, -1, "Variable"));
    fgs->Add(new wxStaticText(panel, -1, "Upper Bound"));
    fgs->Add(new wxStaticText(panel, -1, "Lower Bound"));

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

    wxButton *save = new wxButton(panel, wxID_SAVE, _T("Save"));
    Bind(wxEVT_BUTTON, &variables_dialog::OnSave, this);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    hbox->Add(save, 0, wxALIGN_CENTER | wxALL, 15);
    panel->SetSizerAndFit(hbox);
    SetClientSize(panel->GetSize());
    Centre();
}

void variables_dialog::OnSave(wxCommandEvent& event)
{
    std::cout << "arrived in save" << std::endl;
}