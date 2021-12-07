#include "preferences_dialog.hpp"
#include "smtlib_parse.hpp"
#include <string>

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

    // x by 2 sizer for all other options
    wxFlexGridSizer *axis_and_depth = new wxFlexGridSizer(7, 2, 9, 25);

    // print axis choice
    wxArrayString wx_variable_choices;
    for(auto const &variable_name : m->user_settings.variable_names)
    {
        wx_variable_choices.Add(variable_name);
    }
    x_axis_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_variable_choices);
    x_axis_choice->SetSelection(x_axis_choice->FindString(m->user_settings.x_name));
    y_axis_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_variable_choices);
    y_axis_choice->SetSelection(y_axis_choice->FindString(m->user_settings.y_name));
    axis_and_depth->Add(new wxStaticText(this, -1, "x-Axis"));
    axis_and_depth->Add(x_axis_choice);
    axis_and_depth->Add(new wxStaticText(this, -1, "y-Axis"));
    axis_and_depth->Add(y_axis_choice);

    // depth choice
    depth_ctrl = new wxTextCtrl(this, -1);
    depth_ctrl->AppendText(std::to_string(m->user_settings.max_depth));
    axis_and_depth->Add(new wxStaticText(this, -1, "depth"));
    axis_and_depth->Add(depth_ctrl);

    // splitting-heuristic choice
    wxArrayString wx_splitting_choices;
    for(const auto &choice : splitting_bimap.left)
    {
        wx_splitting_choices.Add(choice.first);
    }
    splitting_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_splitting_choices);
    splitting_choice->SetSelection(splitting_choice->FindString(splitting_bimap.right.find(m->user_settings.splitting_h)->second));
    axis_and_depth->Add(new wxStaticText(this, -1, "Split"));
    axis_and_depth->Add(splitting_choice);


    // sampling-heuristic choice
    wxArrayString wx_sampling_choices;
    for(const auto &choice : sampling_bimap.left)
    {
        wx_sampling_choices.Add(choice.first);
    }
    sampling_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_sampling_choices);
    sampling_choice->SetSelection(sampling_choice->FindString(sampling_bimap.right.find(m->user_settings.sampling_h)->second));
    axis_and_depth->Add(new wxStaticText(this, -1, "Sample"));
    axis_and_depth->Add(sampling_choice);

    // choices for binary decisions
    wxArrayString wx_no_yes_choices;
    wx_no_yes_choices.Add("no");
    wx_no_yes_choices.Add("yes");

    // split samples choice
    split_samples_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_no_yes_choices);
    split_samples_choice->SetSelection(m->user_settings.use_split_samples);
    axis_and_depth->Add(new wxStaticText(this, -1, "Split Samples"));
    axis_and_depth->Add(split_samples_choice);

    // save-model choice
    save_model_choice = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, wx_no_yes_choices);
    save_model_choice->SetSelection(m->user_settings.use_save_model);
    axis_and_depth->Add(new wxStaticText(this, -1, "Save Model"));
    axis_and_depth->Add(save_model_choice);

    // define save button
    wxButton *save = new wxButton(panel, wxID_SAVE, _T("Save"));
    Bind(wxEVT_BUTTON, &preferences_dialog::OnSave, this);

    hbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
    hbox->Add(axis_and_depth, 0, wxALIGN_CENTER | wxALL, 15);
    hbox->Add(save, 0, wxALIGN_CENTER | wxALL, 15);

    panel->SetSizerAndFit(hbox);
    SetClientSize(panel->GetSize());

    Centre();
}

void preferences_dialog::OnSave(wxCommandEvent& event)
{
    // handle intervals and perform sanity checks on them
    for (unsigned int i = 0; i < interval_settings.size(); i++)
    {
        std::string current_name = std::get<0>(m->user_settings.initial_intervals[i]);
        std::string current_lower = std::get<1>(interval_settings[i])->GetValue().ToStdString();
        std::string current_upper = std::get<2>(interval_settings[i])->GetValue().ToStdString();
        m->user_settings.initial_intervals[i] = {current_name, current_lower, current_upper};
    }
    try
    {
        m->user_settings.sanity_check_intervals();
    }
    catch(const std::exception& e)
    {
        wxLogMessage(e.what());
        return;
    }

    // handle max depth and perform sanity check on input
    try
    {
        m->user_settings.max_depth = std::stoi(depth_ctrl->GetValue().ToStdString());
    }
    catch (...) 
    {
        wxLogMessage("Depth input invalid!");
        return;
    }

    // handle choices (no sanity check necessary)
    m->user_settings.x_name = std::get<0>(m->user_settings.initial_intervals[x_axis_choice->GetSelection()]);
    m->user_settings.y_name = std::get<0>(m->user_settings.initial_intervals[y_axis_choice->GetSelection()]);
    m->user_settings.use_save_model = save_model_choice->GetSelection();
    m->user_settings.use_split_samples = split_samples_choice->GetSelection();
    m->user_settings.sampling_h = sampling_bimap.left.find(sampling_choice->GetString(sampling_choice->GetSelection()).ToStdString())->second;

    
    Close(true);
}