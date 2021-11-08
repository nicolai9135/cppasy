#include "cppasy_app.hpp"
#include "main_frame.hpp"

bool cppasy_app::OnInit()
{
    main_frame *frame = new main_frame();
    frame->Show(true);
    return true;
}