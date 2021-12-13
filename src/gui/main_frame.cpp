#include "cppasy_app.hpp"
#include "main_frame.hpp"
#include "plot_frame.hpp"
#include "preferences_dialog.hpp"
#include "smtlib_parse.hpp"

main_frame::main_frame()
  : wxFrame(NULL, wxID_ANY, "cppasy", wxDefaultPosition, wxSize(750, 750))
{
    // create file menu
    menu_file = new wxMenu;
    menu_file->Append(wxID_EXIT);

    // create synthesis menu
    menu_synthesis = new wxMenu;
    menu_synthesis->Append(wxID_PREFERENCES, "Preferences...");
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
    Bind(wxEVT_MENU, &main_frame::OnExecute, this, wxID_EXECUTE);

    textctrl = new wxTextCtrl(this, -1, wxT(""), wxPoint(-1, -1), wxSize(400, 400), wxTE_MULTILINE);
    textctrl->AppendText("(declare-const x Real)\n");
    textctrl->AppendText("(declare-const y Real)\n");
    textctrl->AppendText("(assert (> y x))\n");
    textctrl->AppendText("(assert (< x 1))\n");

    // TODO: make teletype work!!!
    // textctrl->SetDefaultStyle(wxTextAttr(*wxRED));
    // textctrl->AppendText("Red text\n");
    // textctrl->SetDefaultStyle(wxTextAttr(wxFONTFAMILY_TELETYPE));
    // textctrl->AppendText("doesnt look like teletype\n");

    // formula is directly read from the textctrl, not from a file
    user_settings.formula_as_file = 0;

    Center();
}

void main_frame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

// synthesis

void main_frame::OnPreferences(wxCommandEvent& event)
{
    try
    {
        set_defaults_if_necessary();
        preferences_dialog *d = new preferences_dialog(this);
        d->ShowModal();
    }
    catch(const std::exception& e)
    {
        wxLogMessage("stmlib Input invalid!");
    }
    
}

void main_frame::OnExecute(wxCommandEvent& event)
{
    try
    {
        set_defaults_if_necessary();
        plot_frame *plot_f = new plot_frame(user_settings);
        plot_f->Show(true);
    }
    catch(...)
    {
        wxLogMessage("Input invalid! Reason could be preferences or smtlib input.");
    }
}

void main_frame::set_defaults_if_necessary()
{
    // get input from formula
    std::string current_input = textctrl->GetValue().ToStdString();

    // user settings have valid values w.r.t. current input (nothing changed since last save)
    if (current_input != user_settings.formula_str)
    {
        set_defaults();
    }
}

void main_frame::set_defaults()
{
    // set max depth
    user_settings.max_depth = 11;

    // read text and set it to formula
    user_settings.formula_str = textctrl->GetValue().ToStdString();

    // get variable names from formula
    user_settings.variable_names = find_variable_names(user_settings.formula_str, user_settings.formula_as_file);

    // set intial intervals and axis names
    user_settings.initial_intervals = {};
    for(const auto &variable_name : user_settings.variable_names)
    {
        user_settings.initial_intervals.push_back({variable_name, "-1", "2"});
    }
    user_settings.x_name = std::get<0>(user_settings.initial_intervals[0]);
    user_settings.y_name = std::get<0>(user_settings.initial_intervals[1]);

    user_settings.splitting_h = bisect_all;
    user_settings.sampling_h = no_sampling;
    user_settings.use_save_model = false;
    user_settings.use_split_samples = false;
}
