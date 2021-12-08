#include "plot_frame.hpp"
#include "cppasy_app.hpp"
#include <math.h> // log10, floor
#include <stdlib.h>     /* abs */
#include <iomanip>
#include <chrono>

plot_frame::plot_frame(options o)
  : wxFrame(NULL, wxID_ANY, "Plot")
  , s(o)
{
    // initialize x-axis (constructor difficult because of include dependency graph...)
    x_axis.name = o.x_name;
    x_axis.index = get_index(x_axis.name);
    double x_interval_begin = string_to_double(std::get<1>(o.initial_intervals[x_axis.index]));
    double x_interval_end = string_to_double(std::get<2>(o.initial_intervals[x_axis.index]));
    x_axis.width = x_interval_end - x_interval_begin;
    x_axis.offset = x_interval_begin;
    x_axis.is_x_axis = true;
    x_axis.extra_space = 0;

    // initialize x-axis
    y_axis.name = o.y_name;
    y_axis.index = get_index(y_axis.name);
    double y_interval_begin = string_to_double(std::get<1>(o.initial_intervals[y_axis.index]));
    double y_interval_end = string_to_double(std::get<2>(o.initial_intervals[y_axis.index]));
    y_axis.width = y_interval_end - y_interval_begin;
    y_axis.offset = y_interval_begin;
    y_axis.is_x_axis = false;
    y_axis.extra_space = 60;



    resume = new wxButton(this, id_resume, _T("Resume"));

    this->SetClientSize(wxSize(x_plot_size_min + 2*margin + y_axis.extra_space, y_plot_size_min + 2*margin + button_space + x_axis.extra_space));
    wxSize minimum_size = this->GetSize();
    this->SetMinSize(minimum_size);
    
    this->SetClientSize(wxSize(x_plot_size_init + 2*margin + y_axis.extra_space, y_plot_size_init + 2*margin + button_space + x_axis.extra_space));

#if EVAL > 0
    auto total_time_begin = std::chrono::steady_clock::now();
#endif
    s.execute();
#if EVAL > 0
    auto total_time_end = std::chrono::steady_clock::now();
    s.eval.total_time += (total_time_end - total_time_begin);
    s.eval.print();
#endif

    // s.print_all_areas();

    Bind(wxEVT_PAINT, &plot_frame::OnPaint, this);
    Bind(wxEVT_BUTTON, &plot_frame::OnResume, this);

    // plot in svg file, TODO: move this
    // wxSVGFileDC *svg_plot = new wxSVGFileDC("svg_plot_file.svg");
    // plot(svg_plot, s.get_synthesis_areas_ptr())
    Center();
}

void plot_frame::OnPaint(wxPaintEvent& event)
{
    // get window size
    wxSize wx_size = this->GetClientSize();
    int frame_width = wx_size.GetWidth();
    int frame_height = wx_size.GetHeight();

    resume->SetSize(button_width, button_height);
    int y_button = frame_height - button_space + (button_space - button_height)/2 ;
    int x_button = (frame_width-button_width)/2;
    resume->SetPosition(wxPoint(x_button, y_button));

    int x_plot_size = frame_width - 2*margin - y_axis.extra_space;
    int y_plot_size = frame_height - 2*margin - button_space - x_axis.extra_space;
    

    // calculate multiplication factor for plotting
    x_axis.scalar = x_plot_size / x_axis.width;
    y_axis.scalar = y_plot_size / y_axis.width;

    // create new dc
    wxPaintDC *dc = new wxPaintDC(this);

    // reset origin to bottom left corner of the plot
    dc->SetLogicalOrigin(- margin - y_axis.extra_space + (x_axis.offset * x_axis.scalar), - (y_plot_size + margin + (y_axis.offset * y_axis.scalar)));

    // plot axis
    plot_axis(dc, 50, x_axis);
    // add axis labels
    dc->DrawText(x_axis.name, (x_axis.offset + x_axis.width/2) * x_axis.scalar, - y_axis.offset*y_axis.scalar + 50);

    // plot axis
    plot_axis(dc, 50, y_axis);
    // add axis labels
    dc->DrawText(y_axis.name, x_axis.offset * x_axis.scalar - y_axis.extra_space - 20, - (y_axis.offset + y_axis.width/2)*y_axis.scalar -10);

    plot(dc);
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
    // 5 !< 2.5 > 2 => tick_dist:= 5*10^-1 = 0.5
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
    // this is a workaround - use correct numbers!
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
    
    // draw ticks and their labels
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
            x = pos * x_axis.scalar;
            y = y_axis.offset * y_axis.scalar;
            dc->DrawLine(x, -y, x, -y +10);
            dc->DrawText(pos_string, x-15, -y+20);
        }
        else
        {
            x = x_axis.offset * x_axis.scalar;
            y = pos * y_axis.scalar;
            dc->DrawLine(x, -y, x-10, -y);
            dc->DrawText(pos_string, x-y_axis.extra_space, -y-10);
        }
    }
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

void plot_frame::OnResume(wxCommandEvent& event)
{
    // std::cout << "button clicked" << std::endl;
    s.continue_synthesis(1);
    Refresh();
    Update();
    //s.print_all_areas();
}