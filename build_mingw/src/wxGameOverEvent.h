#pragma once
#include <wx/event.h>

class wxGameOverEvent :public wxCommandEvent
{
private:
	wxString m_string;
public:
	wxGameOverEvent(wxEventType commandType = wxEVT_NULL, int id = 0);
	virtual wxEvent* Clone() const;
	wxString getString();
};

typedef void (wxEvtHandler::* wxGameOverEventFunction)(wxGameOverEvent&);

BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(wxEVT_GameOver, 801)
END_DECLARE_EVENT_TYPES()

#define EVT_GAMEOVER(id, fn) \
DECLARE_EVENT_TABLE_ENTRY(wxEVT_GameOver, id, -1, (wxObjectEventFunction) (wxEventFunction)(wxGameOverEventFunction) &fn, (wxObject *)NULL),

