#include "MyButton.h"

MyButton::MyButton(wxPanel* parent, wxStandardID id, wxString title, wxPoint pos, std::string layout, wxSize size)
	: wxButton(parent, id, title, pos, size)
{

	wxImage::AddHandler(new wxPNGHandler);
	m_normal_layout.LoadFile(wxString(std::string("img\\") + layout + "0.png"), wxBITMAP_TYPE_PNG);
	m_normal_layout.SetSize(wxSize(190, 49)); // should not use in release version
	this->SetBitmapLabel(m_normal_layout);
	m_down_layout.LoadFile(wxString(std::string("img\\") + layout + "1.png"), wxBITMAP_TYPE_PNG);
	m_down_layout.SetSize(wxSize(190, 49)); // should not use in release version
	this->SetBitmapCurrent(m_down_layout);
	m_enter_layout.LoadFile(wxString(std::string("img\\") + layout + "2.png"), wxBITMAP_TYPE_PNG);
	m_enter_layout.SetSize(wxSize(190, 49)); // should not use in release version
	this->SetBitmapPressed(m_enter_layout);
	// adjust the position of bitmap
	this->SetBitmapMargins(wxSize(-3, 0));
}