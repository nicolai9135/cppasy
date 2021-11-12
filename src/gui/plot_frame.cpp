#include "plot_frame.hpp"
#include "polytope.hpp"

plot_frame::plot_frame(gui_options o)
  : wxFrame(NULL, wxID_ANY, "Plot"),
    s(o),
    x_name(o.x_name),
    y_name(o.y_name),
    x_width(2),
    y_width(2),
    x_offset(0),
    y_offset(0)
{
    this->SetClientSize(wxSize(400,400));
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
    wxSize wx_size = this->GetClientSize();
    int x_frame_size = wx_size.GetWidth();
    int y_frame_size = wx_size.GetHeight();

    double x_scalar = x_frame_size / x_width;
    double y_scalar = y_frame_size / y_width;

    std::cout << x_scalar << y_scalar << std::endl;

    // plot in frame
    wxPaintDC *dc = new wxPaintDC(this);
    // dc->SetLogicalScale (x_scalar, y_scalar);
    dc->SetLogicalScale (100, 100);
    // dc->SetLogicalOrigin(-x_offset, y_width-y_offset);
    dc->SetLogicalOrigin(0, 4);
    dc->SetAxisOrientation(true, true);
    plot(dc);
}

void plot_frame::plot(wxDC *dc)
{
    std::cout << "arrived in plot function" << std::endl;



    unsigned int index_x = get_index(x_name);
    std::cout << index_x << std::endl;
    unsigned int index_y = get_index(y_name);
    std::cout << index_y << std::endl;

    plot_deque(dc, s.get_synthesis_areas_ptr()->unsafe_areas, wxColor(204, 7, 30), index_x, index_y);
    plot_deque(dc, s.get_synthesis_areas_ptr()->unknown_areas, wxColor(255, 255, 255), index_x, index_y);
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
        p->draw_wxWidgets(dc, index_x, index_y);
    }
}

unsigned int plot_frame::get_index(std::string var)
{
    unsigned int i = 0;

    for (const auto &candidate : s.get_variable_names())
    {
        std::stringstream ss_candidate;
        ss_candidate << candidate;
        std::string s_candidate = ss_candidate.str();
        if (s_candidate == var)
        {
            return i;
        }
        i++;
    }
    // TODO: THROW ERROR, a fitting candidate should always be found!!!!!
    return 0;
}