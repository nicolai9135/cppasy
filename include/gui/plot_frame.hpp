#ifndef PLOT_FRAME
#define PLOT_FRAME

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "parameter_synthesis.hpp"

class plot_frame : public wxFrame
{
private:
    synthesis s;
    std::string x_name;
    std::string y_name;
    double x_width;
    double y_width;
    double x_offset;
    double y_offset;
    void plot(wxDC *dc);
    void plot_deque(wxDC *dc, std::deque<std::unique_ptr<polytope>> &area_deque, wxColour color, unsigned int index_x, unsigned int index_y);
    unsigned int get_index(std::string var);
public:
    plot_frame(gui_options o);
    void OnPaint(wxPaintEvent& event);
};

#endif