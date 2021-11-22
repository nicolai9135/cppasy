#ifndef PLOT_FRAME
#define PLOT_FRAME

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "polytope.hpp"
#include "parameter_synthesis.hpp"

double string_to_double(std::string s);

class plot_frame : public wxFrame
{
private:
    synthesis s;
    axis x_axis;
    axis y_axis;
    int margin = 50;
    int button_space = 100;
    int button_width = 100;
    int button_height = 30;
    int x_plot_size_init = 400;
    int y_plot_size_init = 400;
    int x_plot_size_min = 100;
    int y_plot_size_min = 100;
    void plot(wxDC *dc);
    void plot_deque(wxDC *dc, std::deque<std::unique_ptr<polytope>> &area_deque, wxColour color);
    void plot_axis(wxDC *dc, unsigned int min_tick_dist, axis a);
    unsigned int get_index(std::string var);
public:
    wxButton *resume;
    plot_frame(options o);
    void OnPaint(wxPaintEvent& event);
    void OnResume(wxCommandEvent& event);
};

#endif