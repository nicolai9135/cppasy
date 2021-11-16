#include "plot_frame.hpp"
#include <math.h> // log10, floor
#include <stdlib.h>     /* abs */
#include <iomanip>

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
    x_axis.is_x_axis = true;

    // initialize x-axis
    y_axis.name = o.y_name;
    y_axis.index = get_index(y_axis.name);
    double y_interval_begin = string_to_double(o.initial_intervals[y_axis.index].first);
    double y_interval_end = string_to_double(o.initial_intervals[y_axis.index].second);
    y_axis.width = y_interval_end - y_interval_begin;
    y_axis.offset = y_interval_begin;
    y_axis.is_x_axis = false;


    this->SetClientSize(wxSize(x_plot_size_min + 2*margin, y_plot_size_min + 2*margin + button_space));
    wxSize minimum_size = this->GetSize();
    this->SetMinSize(minimum_size);
    
    this->SetClientSize(wxSize(x_plot_size_init + 2*margin, y_plot_size_init + 2*margin + button_space));

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
    int x_plot_size = wx_size.GetWidth() - 2*margin;
    int y_plot_size = wx_size.GetHeight() - 2*margin - button_space;

    // calculate multiplication factor for plotting
    x_axis.scalar = x_plot_size / x_axis.width;
    y_axis.scalar = y_plot_size / y_axis.width;

    // create new dc
    wxPaintDC *dc = new wxPaintDC(this);

    // reset origin to bottom left corner of the window
    dc->SetLogicalOrigin(- margin + (x_axis.offset * x_axis.scalar), - (y_plot_size + margin + (y_axis.offset * y_axis.scalar)));
    // -x_offset, y_width-y_offset

    int x = x_axis.offset*x_axis.scalar;
    int y = y_axis.offset*y_axis.scalar;
    
    dc->DrawLine((x_axis.offset * x_axis.scalar), 10, (x_axis.offset * x_axis.scalar), 30);

    // adjust axis orientation
    // default by wxWidgets: (true, false) -> usual coordinate system: (true, true)
    // dc->SetAxisOrientation(true, true);

    plot(dc);

    plot_axis(dc, 50, x_axis);
    plot_axis(dc, 50, y_axis);
}

void plot_frame::plot_axis(wxDC *dc, unsigned int min_tick_dist, axis a)
{
    // example: min_tick_dist: 50, width:2, plot_size: 400 => scalar: 200
    // => min_tick_dist_scaled = 0.25
    double min_tick_dist_scaled = min_tick_dist / a.scalar;
    // log10(0.25) = -0.6x => -1
    double order_of_magnitude = floor(log10(min_tick_dist_scaled));
    // 0.25*(10^(-(-1))) = 0.25 * 10 = 2.5
    double shifted = min_tick_dist_scaled * pow(10, -order_of_magnitude);

    // -> 2.5 > 2 => 5*10^-1 = 0.5
    double tick_dist;
    
    // take steps of size 10
    if(shifted > 5)
    {
        tick_dist = 10 * pow(10, order_of_magnitude);
    }
    // take steps of size 5
    else if(shifted > 2)
    {

        tick_dist = 5 * pow(10, order_of_magnitude);
    }
    // take steps of size 2
    else
    {
        tick_dist = 2 * pow(10, order_of_magnitude);
    }

    // get first tick position:
    double first_tick_pos;
    int offset_quotient = floor(a.offset / tick_dist);
    double offset_remainder = remainder(a.offset, tick_dist);
    if (offset_remainder < 0.00000001)
    {
        first_tick_pos = a.offset;
    }
    else
    {
        first_tick_pos = (offset_quotient+1) * tick_dist;
    }

    // calculate all tick positions
    std::vector<double> tick_positions;
    double current_tick_pos = first_tick_pos;
    while (current_tick_pos <= a.offset + a.width)
    {
        tick_positions.push_back(current_tick_pos);
        current_tick_pos += tick_dist;
    }
    
    for(const auto &pos : tick_positions)
    {
        // Create an output string stream
        std::ostringstream pos_stream;
        pos_stream << std::fixed;
        pos_stream << std::setprecision(2);
        pos_stream << pos;
        std::string pos_string = pos_stream.str();

        int x, y;
        if (a.is_x_axis)
        {
            // std::cout << pos << std::endl;
            x = pos * x_axis.scalar;
            // std::cout << x << std::endl;
            y = y_axis.offset * y_axis.scalar;
            // std::cout << y << std::endl;
            dc->DrawLine(x, -y, x, -y +10);
            dc->DrawText(pos_string, x, -y+20);
        }
        else
        {
            // std::cout << pos << std::endl;
            x = x_axis.offset * x_axis.scalar;
            // std::cout << x << std::endl;
            y = pos * y_axis.scalar;
            // std::cout << y << std::endl;
            dc->DrawLine(x, -y, x-10, -y);
        }
    }

    /*
    if (a.name == "x")
    {
        wxSize wx_size = this->GetClientSize();
        int x_plot_size = wx_size.GetWidth() - 100;

        std::cout << "min tick dist" << min_tick_dist << std::endl;
        std::cout << "    scaled" << min_tick_dist_scaled << std::endl;
        std::cout << "    oom" << min_tick_dist << std::endl;
        std::cout << "    shifted" << shifted << std::endl;

        std::cout << "plot width: " << x_plot_size << std::endl;
        std::cout << "tick_dist : " << tick_dist << std::endl;

        std::cout << " fist tick pos: " << first_tick_pos << std::endl;

        for(const auto &pos : tick_positions)
        {
            std::cout << pos << " ";
        }
    }
    */
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