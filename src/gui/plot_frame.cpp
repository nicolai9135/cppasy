#include "plot_frame.hpp"

plot_frame::plot_frame(gui_options o)
  : wxFrame(NULL, wxID_ANY, "Plot")
  , s(o)
{
    // initialize x-axis (constructor difficult because of include dependency graph...)
    x_axis.name = o.x_name;
    x_axis.index = get_index(x_axis.name);
    double x_interval_begin = string_to_double(o.initial_intervals[x_axis.index].first);
    double x_interval_end = string_to_double(o.initial_intervals[x_axis.index].second);
    x_axis.width = x_interval_end - x_interval_begin;
    x_axis.offset = x_interval_begin;

    // initialize x-axis
    y_axis.name = o.y_name;
    y_axis.index = get_index(y_axis.name);
    double y_interval_begin = string_to_double(o.initial_intervals[y_axis.index].first);
    double y_interval_end = string_to_double(o.initial_intervals[y_axis.index].second);
    y_axis.width = y_interval_end - y_interval_begin;
    y_axis.offset = y_interval_begin;
    
    this->SetClientSize(wxSize(400,400));

    // perform synthesis
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
    // get window size
    wxSize wx_size = this->GetClientSize();
    int x_plot_size = wx_size.GetWidth();
    int y_plot_size = wx_size.GetHeight();

    // calculate multiplication factor for plotting
    x_axis.scalar = x_plot_size / x_axis.width;
    y_axis.scalar = y_plot_size / y_axis.width;

    // create new dc
    wxPaintDC *dc = new wxPaintDC(this);

    // reset origin to bottom left corner of the window
    dc->SetLogicalOrigin(0, y_plot_size);
    // -x_offset, y_width-y_offset

    // adjust axis orientation
    // default by wxWidgets: (true, false) -> usual coordinate system: (true, true)
    dc->SetAxisOrientation(true, true);

    plot(dc);
}

void plot_frame::plot(wxDC *dc)
{
    plot_deque(dc, s.get_synthesis_areas_ptr()->unsafe_areas, wxColor(204, 7, 30));
    plot_deque(dc, s.get_synthesis_areas_ptr()->unknown_areas, wxColor(255, 255, 255));
    plot_deque(dc, s.get_synthesis_areas_ptr()->safe_areas, wxColor(87, 171, 39));
}

void plot_frame::plot_deque(wxDC *dc, std::deque<std::unique_ptr<polytope>> &area_deque, wxColour color)
{
    // no borders
    // wxPen p;
    // p.SetWidth(0);
    // dc->SetPen(p);

    // set fill color
    wxBrush b;
    b.SetColour(color);
    dc->SetBrush(b);

    // iterate over polytopes of the current queue to draw them
    for (const auto &p : area_deque)
    {
        p->draw_wxWidgets(dc, x_axis, y_axis);
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

double string_to_double(std::string s)
{
    z3::context ctx;
    const char* s_as_char = s.c_str();
    z3::expr s_as_z3 = ctx.real_val(s_as_char);
    double s_as_double = s_as_z3.as_double();
    return s_as_double;
}