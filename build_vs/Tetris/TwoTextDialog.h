#pragma once
#include <wx/wx.h>

#define HARD 1
#define EASY 0
class TwoTextDialog : public wxDialog {
public:
    TwoTextDialog(wxWindow* parent, const wxString& title,
        const wxString& message1, const wxString& message2, const wxString& message3,
        const wxString& defaultValue1 = wxEmptyString,
        const wxString& defaultValue2 = wxEmptyString,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER)
        : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, size, style)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* label1 = new wxStaticText(this, wxID_ANY, message1);
        sizer->Add(label1, wxSizerFlags().Border(wxALL, 5));

        m_textCtrl1 = new wxTextCtrl(this, wxID_ANY, defaultValue1);
        sizer->Add(m_textCtrl1, wxSizerFlags().Expand().Border(wxALL, 5));

        wxStaticText* label2 = new wxStaticText(this, wxID_ANY, message2);
        sizer->Add(label2, wxSizerFlags().Border(wxALL, 5));

        m_textCtrl2 = new wxTextCtrl(this, wxID_ANY, defaultValue2);
        sizer->Add(m_textCtrl2, wxSizerFlags().Expand().Border(wxALL, 5));

        wxStaticText* label3 = new wxStaticText(this, wxID_ANY, message3);
        sizer->Add(label3, wxSizerFlags().Border(wxALL, 5));

        wxArrayString choices = wxArrayString();
        choices.Add(wxT("easy"));
        choices.Add(wxT("hard"));
        choiceBox = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,  choices, 0L, wxDefaultValidator, wxT("Choose Level"));
        sizer->Add(choiceBox, wxSizerFlags().Expand().Border(wxALL, 5));

        wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer();
        buttonSizer->AddButton(new wxButton(this, wxID_OK, "OK"));
        buttonSizer->AddButton(new wxButton(this, wxID_CANCEL, "Cancel"));
        buttonSizer->Realize();

        sizer->Add(buttonSizer, wxSizerFlags().Expand().Border(wxALL, 5));

        SetSizerAndFit(sizer);
    }

    wxString GetValue1() const { return m_textCtrl1->GetValue(); }
    wxString GetValue2() const { return m_textCtrl2->GetValue(); }
    int GetLevel() const {
        switch (choiceBox->GetSelection()) {
        case(1):
            return HARD;
        case(0):
        default:
            return EASY;
        }
    }

private:
    wxTextCtrl* m_textCtrl1;
    wxTextCtrl* m_textCtrl2;
    wxChoice* choiceBox;

};



class OneTextDialog : public wxDialog {
public:
    OneTextDialog(wxWindow* parent, const wxString& title,
        const wxString& message1,  const wxString& message3,
        const wxString& defaultValue1 = wxEmptyString,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER)
        : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, size, style)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* label1 = new wxStaticText(this, wxID_ANY, message1);
        sizer->Add(label1, wxSizerFlags().Border(wxALL, 5));

        m_textCtrl1 = new wxTextCtrl(this, wxID_ANY, defaultValue1);
        sizer->Add(m_textCtrl1, wxSizerFlags().Expand().Border(wxALL, 5));

        //wxStaticText* label2 = new wxStaticText(this, wxID_ANY, message2);
        //sizer->Add(label2, wxSizerFlags().Border(wxALL, 5));

        //m_textCtrl2 = new wxTextCtrl(this, wxID_ANY, defaultValue2);
        //sizer->Add(m_textCtrl2, wxSizerFlags().Expand().Border(wxALL, 5));

        wxStaticText* label3 = new wxStaticText(this, wxID_ANY, message3);
        sizer->Add(label3, wxSizerFlags().Border(wxALL, 5));

        wxArrayString choices = wxArrayString();
        choices.Add(wxT("easy"));
        choices.Add(wxT("hard"));
        choiceBox = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices, 0L, wxDefaultValidator, wxT("Choose Level"));
        sizer->Add(choiceBox, wxSizerFlags().Expand().Border(wxALL, 5));

        wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer();
        buttonSizer->AddButton(new wxButton(this, wxID_OK, "OK"));
        buttonSizer->AddButton(new wxButton(this, wxID_CANCEL, "Cancel"));
        buttonSizer->Realize();

        sizer->Add(buttonSizer, wxSizerFlags().Expand().Border(wxALL, 5));

        SetSizerAndFit(sizer);
    }

    wxString GetValue1() const { return m_textCtrl1->GetValue(); }
    //wxString GetValue2() const { return m_textCtrl2->GetValue(); }
    int GetLevel() const {
        switch (choiceBox->GetSelection()) {
        case(1):
            return HARD;
        case(0):
        default:
            return EASY;
        }
    }

private:
    wxTextCtrl* m_textCtrl1;
    //wxTextCtrl* m_textCtrl2;
    wxChoice* choiceBox;

};
