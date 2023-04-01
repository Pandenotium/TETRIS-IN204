#define _CRT_SECURE_NO_WARNINGS
#ifndef Puit_H
#define Puit_H

#include <wx/wx.h>
#include <string>
#include <wx/numformatter.h>
#include "Forme.h"
#include "global.h"
#include "TransparentStaticText.h"
#include "wxGameOverEvent.h"
#include "TwoTextDialog.h"

class BaseMatrice;

enum BoardSize { BoardWidth = 10, BoardHeight = 22 };

struct Datagram
{
    int sig;
    Tetrominoes puit[BoardHeight * BoardWidth];
    Forme curPiece;
    int curX;
    int curY;
    int score;
    wxString name;
};

class BoardPanel : public wxPanel
{
public:
    BoardPanel(wxFrame* parent, float fWidth, float fHeight, int n_players, bool is_online, const std::vector<wxString>& uname_vect, int level); // ÐÞ¸Ä

    void Start();
    void Pause();
    Datagram* GetBoard(int idx);
    void SetBoard(Datagram* data, int idx);
    wxStatusBar* GetStatusBar() { return m_stsbar; }
    int GetPuitSum(int idx);
    void StopGame();
    int m_n_players;
    int m_n_players_on;
    int onlineFlag;
protected:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxCommandEvent& event);
private:
    void InitPuit(int idx, bool is_online, wxString uname);
    BaseMatrice** m_gameboards;
    wxStatusBar* m_stsbar;
    wxTimer* timer;
    float mWidth;
    float mHeight;
    bool isStarted;
    bool isPaused;
    int level;



    int PanelTop, PanelLeft;
};


class BaseMatrice // used as user sub panel
{
private:
    
public:
    BaseMatrice(BoardPanel* parent, float m_Width, float m_Height, int id, bool is_online, const wxString& uname, int level);

    Tetrominoes FormeAt(int x, int y)const { return puit[(y * BoardWidth) + x]; }

    void SetFormeAt(int x, int y, Tetrominoes& Forme) { puit[(y * BoardWidth) + x] = Forme; }
    float SquareWidth() const { return mWidth / BoardWidth; }
    float SquareHeight() const { return mHeight / BoardHeight; }
    
    void ClearBoard();
    void ClearShapeBox();
    void DrawSquare(wxPaintDC& dc, int x, int y, Tetrominoes Forme) const;
    void DrawMatrix(wxPaintDC& dc)const;
    void DrawMatrixFrame(wxPaintDC& dc) const;

    void DrawInfoFrame(wxPaintDC& dc, float top, float totHeight);
    void DrawAllInfoBox(wxPaintDC& dc);

    void DrawShapeBox(wxPaintDC& dc, float top, Tetrominoes* info);
    void UpdateScore(int add);

    void DrawCurrPiece(wxPaintDC& dc);
    void Refresh() { m_parent->Refresh(); }
    void HideAll();
    virtual void ShowAll()=0;
    int GetPuitSum() {
        int sum = 0;
        for (int i = 0; i < 220; i++)sum += puit[i];
        sum += curX;
        sum += curY;
        return sum;
    }

    BoardPanel* m_parent;
    bool isMain;
    bool isDisplayOnly;
    int id_player;
    bool isAlive;
    bool isOnline;
    int count;
    
    Forme curPiece;
    Forme nextPiece;
    int curX;
    int curY;
    int score;
    int level;
    Tetrominoes puit[BoardWidth * BoardHeight];
    Tetrominoes hold[6 * 6];
    Tetrominoes next[6 * 6];
    wxStatusBar* m_stsbar;

    float mWidth, mHeight;// size of matrix
    float SubPanelLeft, SubPanelTop; //offset of the Matrice-panel position in the window
    float SubPanelHeight, SubPanelWidth;
    float mLeft, mTop; //offset of the matrix position in the Matrice;

    float infoLeft, uNameTop, holdTop, nextTop, scoreTop;
    float infoWidth, uNameHeight, holdHeight, nextHeight, scoreHeight;
    float titleHeight, textHeight, shapeBoxHeight;

    TransparentStaticText* uNametitle, * scoretitle, * holdtitle, * nexttitle, \
        * uName, * uScore;

};

class Matrice : public BaseMatrice   // controlable matrix
{ 
public:
    Matrice(BoardPanel* parent, bool is_main, float m_Width, float m_Height, int id, bool is_online, const wxString& uname, int level) : BaseMatrice(parent, m_Width, m_Height, id, is_online, uname, level)
    {
        isMain = is_main;
        isDisplayOnly = false;
        isFallingFinished = false;
        numLinesRemoved = 0;
        
    }

    void DropDown();
    void OneLineDown();
    void PieceDropped();
    int RemoveFullLines();
    void NewPiece();
    bool TryMove(const Forme& newPiece, int newX, int newY) ;
    void ShowAll();

    bool isFallingFinished;
    int numLinesRemoved;

};

class DisplayOnlyMatrice : public BaseMatrice // uncontrolable matrix used for online games
{
public:
    
    DisplayOnlyMatrice(BoardPanel* parent, float m_Width, float m_Height, int id, bool is_online, const wxString& uname, int level) : BaseMatrice(parent, m_Width, m_Height, id, is_online,  uname, level)
    {
        isDisplayOnly = true;
    }
    void ShowAll();
};

#endif