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
    void plot(wxDC *dc, areas *a);
public:
    plot_frame(gui_options o);
};

#endif