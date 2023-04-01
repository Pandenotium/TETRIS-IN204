#include "wxGameOverEvent.h"

DEFINE_EVENT_TYPE(wxEVT_GameOver)

wxGameOverEvent::wxGameOverEvent(wxEventType commandType, int id)
	: wxCommandEvent(commandType, id)
{
	m_string = "Game Over";
}

wxEvent* wxGameOverEvent::Clone() const
{
	return new wxGameOverEvent(*this);
}

wxString wxGameOverEvent::getString()
{
	return m_string;
}