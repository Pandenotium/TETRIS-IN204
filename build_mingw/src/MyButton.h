#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MyButton: public wxButton
{
public:
	MyButton(wxPanel* parent, wxStandardID id, wxString title, wxPoint pos, std::string layout, wxSize size);
private:
	wxBitmap m_normal_layout;
	wxBitmap m_down_layout;
	wxBitmap m_enter_layout;
};
