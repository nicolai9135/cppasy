#ifndef CPPASY_APP
#define CPPASY_APP

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class cppasy_app : public wxApp
{
public:
    virtual bool OnInit();
};

enum
{
    id_resume
};
#endif