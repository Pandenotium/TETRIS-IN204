#pragma once
#include <wx/wx.h>

class TransparentStaticText : public wxStaticText
{
	DECLARE_DYNAMIC_CLASS(TransparentStaticText)

public:
	TransparentStaticText();
	TransparentStaticText(
		wxWindow* parent,
		wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxStaticTextNameStr
	);

	bool Create(
		wxWindow* parent,
		wxWindowID id,
		const wxString& label,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxStaticTextNameStr
	);
	virtual void OnPaint(wxPaintEvent& event);
	DECLARE_EVENT_TABLE()
};