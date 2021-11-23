#include "preferences_dialog.hpp"
#include "smtlib_parse.hpp"

preferences_dialog::preferences_dialog(main_frame *m)
  : wxDialog(NULL, wxID_ANY, "Preferences")
  , m(m)
{
    // panel: contains everything, parent frame for ...
    wxPanel *panel = new wxPanel(this, -1);

    // hbox: contains gridsizer to add margins
    wxBoxSizer *hbox = new wxBoxSizer(wxVERTICAL);

    // rows, cols, vgap, hgap
    wxFlexGridSizer *fgs = new wxFlexGridSizer((int) m->user_settings.variable_names.size()+1, 3, 9, 25);

    fgs->Add(new wxStaticText(panel, -1, "Variable"));
    fgs->Add(new wxStaticText(panel, -1, "Upper Bound"));
    fgs->Add(new wxStaticText(panel, -1, "Lower Bound"));

    interval_settings = {};

    for(const auto &ii : m->user_settings.initial_intervals)
    {
        interval_settings.push_back({new wxStaticText(panel, -1, std::get<0>(ii)), new wxTextCtrl(panel, -1), new wxTextCtrl(panel, -1)});
        fgs->Add(std::get<0>(interval_settings.back()));
        std::get<1>(interval_settings.back())->AppendText(std::get<1>(ii));
        fgs->Add(std::get<1>(interval_settings.back()));
        std::get<2>(interval_settings.back())->AppendText(std::get<2>(ii));
        fgs->Add(std::get<2>(interval_settings.back()));
    }

    // define save button
    wxButton *save = new wxButton(panel, wxID_SAVE, _T("Save"));
    Bind(wxEVT_BUTTON, &preferences_dialog::OnSave, this);

    wxFlexGridSizer *axis_and_depth = new wxFlexGridSizer(3, 2, 9, 25);

    wxArrayString wx_variable_choices;
    for(auto const &variable_name : m->user_settings.variable_names)
    {
        wx_variable_choices.Add(variable_name);
    }

    x_axis_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_variable_choices);
    x_axis_choice->SetSelection(0);
    y_axis_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_variable_choices);
    y_axis_choice->SetSelection(1);
    depth_ctrl = new wxTextCtrl(this, -1);
    depth_ctrl->AppendText("5");

    axis_and_depth->Add(new wxStaticText(this, -1, "x-Axis"));
    axis_and_depth->Add(x_axis_choice);
    axis_and_depth->Add(new wxStaticText(this, -1, "y-Axis"));
    axis_and_depth->Add(y_axis_choice);
    axis_and_depth->Add(new wxStaticText(this, -1, "depth"));
    axis_and_depth->Add(depth_ctrl);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    hbox->Add(axis_and_depth, 0, wxALIGN_CENTER | wxALL, 15);
    hbox->Add(save, 0, wxALIGN_CENTER | wxALL, 15);

    panel->SetSizerAndFit(hbox);
    SetClientSize(panel->GetSize());

    Centre();
}

void preferences_dialog::OnSave(wxCommandEvent& event)
{
    for (unsigned int i = 0; i < interval_settings.size(); i++)
    {
        std::string current_name = std::get<0>(m->user_settings.initial_intervals[i]);
        std::string current_lower = std::get<1>(interval_settings[i])->GetValue().ToStdString();
        std::string current_upper = std::get<2>(interval_settings[i])->GetValue().ToStdString();
        m->user_settings.initial_intervals[i] = {current_name, current_lower, current_upper};
    }
    m->user_settings.x_name = std::get<0>(m->user_settings.initial_intervals[x_axis_choice->GetSelection()]);
    m->user_settings.y_name = std::get<0>(m->user_settings.initial_intervals[y_axis_choice->GetSelection()]);
    try
    {
        m->user_settings.max_depth = std::stoi(depth_ctrl->GetValue().ToStdString());
    }
    catch (...) 
    {
        wxLogMessage("Depth input invalid!");
        return;
    }
    Close(true);
}