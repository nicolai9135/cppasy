
#include <wx/dcsvg.h>
#include "plot_frame.hpp"
#include "polytope.hpp"

plot_frame::plot_frame(gui_options o)
  : wxFrame(NULL, wxID_ANY, "Plot", wxDefaultPosition, wxSize(500, 500)),
    s(o),
    var_x(o.var_x),
    var_y(o.var_y),
    width_x(2),
    width_y(2)
{
    s.execute();
    s.print_all_areas();

    Bind(wxEVT_PAINT, &plot_frame::OnPaint, this);

    // plot in svg file, TODO: move this
    // wxSVGFileDC *svg_plot = new wxSVGFileDC("svg_plot_file.svg");
    // plot(svg_plot, s.get_synthesis_areas_ptr())
    Center();
}

void plot_frame::OnPaint(wxPaintEvent& event)
{
    // plot in frame
    wxPaintDC *dc = new wxPaintDC(this);
    plot(dc);
}

void plot_frame::plot(wxDC *dc)
{
    std::cout << "arrived in plot function" << std::endl;

    unsigned int index_x = get_index(var_x);
    std::cout << index_x << std::endl;
    unsigned int index_y = get_index(var_y);
    std::cout << index_y << std::endl;

    plot_deque(dc, s.get_synthesis_areas_ptr()->unsafe_areas, wxColor(87, 171, 39), index_x, index_y);
    plot_deque(dc, s.get_synthesis_areas_ptr()->unknown_areas, wxColor(87, 171, 39), index_x, index_y);
    plot_deque(dc, s.get_synthesis_areas_ptr()->safe_areas, wxColor(87, 171, 39), index_x, index_y);
}

void plot_frame::plot_deque(wxDC *dc, std::deque<std::unique_ptr<polytope>> &area_deque, wxColour color, unsigned int index_x, unsigned int index_y)
{
    // no borders
    wxPen p;
    p.SetWidth(0);
    dc->SetPen(p);

    // set fill color
    wxBrush b;
    b.SetColour(color);
    dc->SetBrush(b);

    for (const auto &p : area_deque)
    {
        // intervals boundaries = p->get_boundaries();
        double x_begin_scaled = 0;
        double x_end_scaled = 100;
        double y_begin_scaled = 0;
        double y_end_scaled = 100;

        const wxRealPoint top_left_real = wxRealPoint(x_begin_scaled, y_end_scaled);
        const wxRealPoint bottom_right_real = wxRealPoint(x_end_scaled, y_begin_scaled);

        const wxPoint top_left = wxPoint(top_left_real);
        const wxPoint bottom_right = wxPoint(bottom_right_real);

        const wxRect rectangle = wxRect(top_left, bottom_right);

        dc->DrawRectangle(rectangle);
    }
}

unsigned int plot_frame::get_index(std::string var)
{
    unsigned int i = 0;

    for (const auto &v : s.get_variable_names())
    {
        // v.get_string() not working. fix!
        if (v.get_string() == var)
        {
            return i;
        }
        i++;
    }
    // TODO: THROW ERROR!!!!!
    return 0;
}