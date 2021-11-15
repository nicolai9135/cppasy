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
    void plot(wxDC *dc);
    void plot_deque(wxDC *dc, std::deque<std::unique_ptr<polytope>> &area_deque, wxColour color);
    unsigned int get_index(std::string var);
public:
    plot_frame(gui_options o);
    void OnPaint(wxPaintEvent& event);
};

#endif