#pragma once
#include "piece.h"
#include <wx/wx.h>


class Ecran: public wxPanel
{
public:
    Ecran(wxFrame* parent);
    void Start();
    void Pause();

protected:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxCommandEvent& event);



private:
	int EcranLargeur = 10;
    int EcranHauteur = 20;







};

