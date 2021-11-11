#include "plot_frame.hpp"

plot_frame::plot_frame(gui_options o)
  : wxFrame(NULL, wxID_ANY, "Plot"),
    s(o)
{
    s.execute();
    s.print_all_areas();

    // plot in frame
    // wxPaintDC *frame_plot = new wxPaintDC(this);
    // plot(frame_plot, s.get_synthesis_areas_ptr());
    
    // plot in svg file, TODO: move this
    // wxSVGFileDC *svg_plot = new wxSVGFileDC("svg_plot_file.svg");
    // plot(svg_plot, s.get_synthesis_areas())

}

void plot_frame::plot(wxDC *dc, areas *a)
{
    std::cout << "arrived in plot function" << std::endl;
}