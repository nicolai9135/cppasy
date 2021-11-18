#ifndef VARIABLES_DIALOG
#define VARIABLES_DIALOG

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "main_frame.hpp"

class variables_dialog : public wxDialog
{
private:
public:
    variables_dialog(main_frame *m);
};

#endif