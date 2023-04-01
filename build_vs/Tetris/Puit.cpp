#include "Puit.h"
#include "wx/textdlg.h"

BoardPanel::BoardPanel(wxFrame* parent, float fWidth, float fHeight, int n_players, bool is_online, const std::vector<wxString>& uname_vect, int Level)
    : wxPanel(parent, wxID_ANY, wxPoint(0,0),
        wxSize(fWidth, fHeight), wxBORDER_NONE)
{

    timer = new wxTimer(this, 1);

    isStarted = false;
    isPaused = false;

    m_n_players = n_players;
    m_n_players_on = n_players;
    onlineFlag = pow(2, n_players) - 1;
    this->level = Level;

    m_gameboards = new BaseMatrice * [n_players];

    wxSize size = GetClientSize();
    mWidth = fmin(float(size.GetWidth())/ 10/n_players, float(size.GetHeight()) /22) * 0.7 * 10;
    mHeight = mWidth * 2.2;
    for (int i = 0; i < m_n_players; i++) {
        if (uname_vect.size() > i) {
            InitPuit(i, is_online, uname_vect[i]);
        }
        else
        {
            InitPuit(i, is_online, wxT("JOJO"));
        }
        
    }

    Connect(wxEVT_PAINT, wxPaintEventHandler(BoardPanel::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(BoardPanel::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(BoardPanel::OnTimer));
    wxMessageBox("Initialized.",
        "Debug info", wxOK | wxICON_INFORMATION);
}

void BoardPanel::InitPuit(int idx, bool is_online, wxString uname) {
    if (idx == 0)
        m_gameboards[idx] = new Matrice(this, true, mWidth, mHeight, idx, is_online, uname, level);
    else if (is_online)
        m_gameboards[idx] = new DisplayOnlyMatrice(this, mWidth, mHeight, idx, is_online, uname, level);
    else
        m_gameboards[idx] = new Matrice(this, false, mWidth, mHeight, idx, is_online, uname, level);
    
    Update();
}

void BoardPanel::Start()
{
    if (isPaused)
        return;

    isStarted = true;
    for (int i = 0; i < m_n_players; i++) {
        m_gameboards[i]->ShowAll();
        if (m_gameboards[i]->isDisplayOnly) continue;
        ((Matrice*)m_gameboards[i])->isFallingFinished = false;
        ((Matrice*)m_gameboards[i])->numLinesRemoved = 0;
        ((Matrice*) m_gameboards[i])->NewPiece();
    }

    timer->Start(300);
}

void BoardPanel::Pause()
{
    if (!isStarted)
        return;

    isPaused = !isPaused;
    if (isPaused) {
        timer->Stop();
    }
    else {
        timer->Start(300);
        wxString str;
    }
    Refresh();
}

Datagram* BoardPanel::GetBoard(int idx)
{
    Datagram* temp = new Datagram;

    if (m_gameboards[idx]->isAlive) {
        temp->sig = idx;
    }
    else {
        temp->sig = -1;
    }
    
    memcpy(temp->puit, m_gameboards[idx]->puit, BoardHeight * BoardWidth * sizeof(Tetrominoes));
    memcpy(&(temp->curPiece), &(m_gameboards[idx]->curPiece), sizeof(Forme));
    temp->curX = m_gameboards[idx]->curX;
    temp->curY = m_gameboards[idx]->curY;
    temp->score = m_gameboards[idx]->score;
    temp->name = m_gameboards[idx]->uName->GetLabelText();
    return temp;
}

void BoardPanel::SetBoard(Datagram* data, int idx)
{
    memcpy(m_gameboards[idx]->puit, data->puit, BoardHeight * BoardWidth * sizeof(Tetrominoes));
    memcpy(&(m_gameboards[idx]->curPiece), &(data->curPiece), sizeof(Forme));
    m_gameboards[idx]->score = data->score;
    m_gameboards[idx]->UpdateScore(0);
    m_gameboards[idx]->curX = data->curX;
    m_gameboards[idx]->curY = data->curY;
    m_gameboards[idx]->uName->SetLabelText(data->name);

    if (data->sig == -1) {
        m_gameboards[idx]->isAlive = false;
    }
}


int BoardPanel::GetPuitSum(int idx) {
    return m_gameboards[idx]->GetPuitSum();
}

void BoardPanel::StopGame()
{
    timer->Stop();
    isStarted = false;
    wxGameOverEvent event(wxEVT_GameOver, wxStandardID(9));
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
}

void BoardPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    
    for (int p = 0; p < m_n_players; p++) {
        m_gameboards[p]->DrawMatrix(dc);
        m_gameboards[p]->DrawCurrPiece(dc);
        m_gameboards[p]->DrawMatrixFrame(dc);
        m_gameboards[p]->DrawAllInfoBox(dc);
    }
}

void BoardPanel::OnKeyDown(wxKeyEvent& event)
{
    for (int i = 0; i < m_n_players; i++) {
        if (m_gameboards[i]->isDisplayOnly) continue;
        if (!isStarted || m_gameboards[i]->curPiece.GetForme() == NoForme || !m_gameboards[i]->isAlive) {
            event.Skip();
            continue;
        }

        int keycode = event.GetKeyCode();

        if (keycode == 'p' || keycode == 'P') {
            Pause();
            return;
        }

        if (isPaused)
            return;

        if (m_gameboards[i]->isMain) {
            switch (keycode) {
            case WXK_LEFT:
                ((Matrice*)m_gameboards[i])->TryMove(m_gameboards[i]->curPiece, m_gameboards[i]->curX - 1, m_gameboards[i]->curY);
                break;
            case WXK_RIGHT:
                ((Matrice*)m_gameboards[i])->TryMove(m_gameboards[i]->curPiece, m_gameboards[i]->curX + 1, m_gameboards[i]->curY);
                break;
            case WXK_DOWN:
                ((Matrice*)m_gameboards[i])->OneLineDown();
                break;
            case WXK_UP:
                ((Matrice*)m_gameboards[i])->TryMove(m_gameboards[i]->curPiece.RotateLeft(), m_gameboards[i]->curX, m_gameboards[i]->curY);
                break;
            case WXK_SPACE:
                ((Matrice*)m_gameboards[i])->DropDown();
                break;
            default:
                event.Skip();
            }
        }
        else {
            switch (keycode) {
            case 'A':
                ((Matrice*)m_gameboards[i])->TryMove(m_gameboards[i]->curPiece, m_gameboards[i]->curX - 1, m_gameboards[i]->curY);
                break;
            case 'D':
                ((Matrice*)m_gameboards[i])->TryMove(m_gameboards[i]->curPiece, m_gameboards[i]->curX + 1, m_gameboards[i]->curY);
                break;
            case 'W':
                ((Matrice*)m_gameboards[i])->TryMove(m_gameboards[i]->curPiece.RotateRight(), m_gameboards[i]->curX, m_gameboards[i]->curY);
                break;
            case 'S':
                ((Matrice*)m_gameboards[i])->OneLineDown();
                break;
            case 'X':
                ((Matrice*)m_gameboards[i])->DropDown();
                break;
            default:
                event.Skip();
            }
        }
    }
}

void BoardPanel::OnTimer(wxCommandEvent& event)
{
    if (m_n_players_on == 0) StopGame();
    for (int i = 0; i < m_n_players; i++) {
        if (m_gameboards[i]->isDisplayOnly ) {
            int tmp = m_gameboards[i]->GetPuitSum();
            if (tmp != m_gameboards[i]->count) {
                Refresh(); m_gameboards[i]->count = tmp;
            }
            continue;
        }
            
        if (!m_gameboards[i]->isAlive ) {
            if (m_gameboards[i]->isOnline ) {
                if(onlineFlag == 0) StopGame();
            }
            continue;
        }
        if (((Matrice*)m_gameboards[i])->isFallingFinished) {
            ((Matrice*)m_gameboards[i])->isFallingFinished = false;
            ((Matrice*)m_gameboards[i])->NewPiece();
        }
        else {
            ((Matrice*)m_gameboards[i])->OneLineDown();
        }
    }

}

BaseMatrice::BaseMatrice(BoardPanel* parent, float m_Width, float m_Height, int id, bool is_online, const wxString& uname, int Level)
{
    // parameter members
    m_parent = parent;
    id_player = id;
    mWidth = m_Width;
    mHeight = m_Height;
    isOnline = is_online;
    level = Level;
    isAlive = true;
    wxSize size = m_parent->GetSize();

    curX = 0;
    curY = 0;
    score = 0;
    count = 0;

    nextPiece.SetForme(NoForme);

    // subpanel layout
    SubPanelTop = 0;
    SubPanelHeight = size.GetHeight();
    SubPanelWidth = size.GetWidth()/ (m_parent->m_n_players);
    SubPanelLeft = SubPanelWidth * id_player;
    mTop = (SubPanelHeight - mHeight) / 2;

    // UI layout
    infoWidth = SquareWidth() * 6;
    infoLeft = SubPanelLeft + (SubPanelWidth - mWidth - infoWidth) / 2;
    mLeft = infoLeft + infoWidth;
    titleHeight = textHeight = SquareHeight();
    shapeBoxHeight = 6 * SquareHeight();
    uNameTop = mTop; 
    holdTop = uNameTop + titleHeight + textHeight;
    nextTop = holdTop + titleHeight + shapeBoxHeight;
    scoreTop = nextTop + titleHeight + shapeBoxHeight;
    uNametitle = new TransparentStaticText(parent, -1, wxT("Player Name"), wxPoint(infoLeft + 1, uNameTop + 1), \
        wxSize(infoWidth-2, titleHeight-2), wxTE_MULTILINE);
    scoretitle = new TransparentStaticText(parent, -1, wxT("score"), wxPoint(infoLeft+1, scoreTop+1), \
        wxSize(infoWidth-2, titleHeight-2), wxTE_MULTILINE);
    uName = new TransparentStaticText(parent, -1, uname, wxPoint(infoLeft + 1, uNameTop + titleHeight + 1), \
        wxSize(infoWidth - 2, textHeight - 2), wxTE_MULTILINE);
    uScore = new TransparentStaticText(parent, -1, wxT("0"), wxPoint(infoLeft + 1, scoreTop + titleHeight + 1), \
        wxSize(infoWidth - 2, textHeight - 2), wxTE_MULTILINE);
    holdtitle = new TransparentStaticText(parent, -1, wxT("hold"), wxPoint(infoLeft + 1, holdTop + 1), \
        wxSize(infoWidth - 2, titleHeight - 2), wxTE_MULTILINE);
    nexttitle = new TransparentStaticText(parent, -1, wxT("next"), wxPoint(infoLeft + 1, nextTop + 1), \
        wxSize(infoWidth - 2, titleHeight - 2), wxTE_MULTILINE);

    // clear UI
    HideAll();
    ClearBoard();
    ClearShapeBox();
}

void BaseMatrice::HideAll() {
    uNametitle->Hide();
    scoretitle->Hide();
    uName->Hide();
    uScore->Hide();
    holdtitle->Hide();
    nexttitle->Hide();
}

void Matrice::ShowAll() {
    uNametitle->Show();
    scoretitle->Show();
    uName->Show();
    uScore->Show();
    holdtitle->Show();
    nexttitle->Show();
}

void DisplayOnlyMatrice::ShowAll() {
    uNametitle->Show();
    scoretitle->Show();
    uName->Show();
    uScore->Show();
}


void BaseMatrice::ClearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
        puit[i] = NoForme;

}
void BaseMatrice::ClearShapeBox()
{

    for (int i = 0; i < 36; i++) {
        next[i] = NoForme;
        hold[i] = NoForme;
    }
}

void BaseMatrice::DrawSquare(wxPaintDC& dc, int x, int y, Tetrominoes Forme) const
{
    static wxColour colors[] = { wxColour(0, 0, 0), wxColour(204, 102, 102),
             wxColour(102, 204, 102), wxColour(102, 102, 204),
             wxColour(204, 204, 102), wxColour(204, 102, 204),
             wxColour(102, 204, 204), wxColour(218, 170, 0) };

    static wxColour light[] = { wxColour(0, 0, 0), wxColour(248, 159, 171),
             wxColour(121, 252, 121), wxColour(121, 121, 252),
             wxColour(252, 252, 121), wxColour(252, 121, 252),
             wxColour(121, 252, 252), wxColour(252, 198, 0) };

    static wxColour dark[] = { wxColour(0, 0, 0), wxColour(128, 59, 59),
             wxColour(59, 128, 59), wxColour(59, 59, 128),
             wxColour(128, 128, 59), wxColour(128, 59, 128),
             wxColour(59, 128, 128), wxColour(128, 98, 0) };


    wxPen pen(light[int(Forme)]);
    pen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(pen);

    dc.DrawLine(x, y + SquareHeight() - 1, x, y);
    dc.DrawLine(x, y, x + SquareWidth() - 1, y);

    wxPen darkpen(dark[int(Forme)]);
    darkpen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkpen);

    dc.DrawLine(x + 1, y + SquareHeight() - 1,
        x + SquareWidth() - 1, y + SquareHeight() - 1);
    dc.DrawLine(x + SquareWidth() - 1,
        y + SquareHeight() - 1, x + SquareWidth() - 1, y + 1);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(colors[int(Forme)]));
    dc.DrawRectangle(x + 1, y + 1, SquareWidth() - 2,
        SquareHeight() - 2);
}

void BaseMatrice::DrawMatrixFrame(wxPaintDC& dc) const {
    wxPen pen(*wxBLACK_PEN);
    pen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(pen);
    dc.DrawLine(wxPoint(mLeft, mTop), wxPoint(mLeft, mTop + mHeight));
    dc.DrawLine(wxPoint(mLeft + mWidth, mTop), wxPoint(mLeft + mWidth, mTop + mHeight));
    dc.DrawLine(wxPoint(mLeft, mTop), wxPoint(mLeft + mWidth, mTop));
    dc.DrawLine(wxPoint(mLeft, mTop + mHeight), wxPoint(mLeft + mWidth, mTop + mHeight));


}

void BaseMatrice::DrawMatrix(wxPaintDC& dc) const{
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            const Tetrominoes Forme = FormeAt(j, BoardHeight - i - 1);
            if (Forme != NoForme)
                DrawSquare(dc, mLeft + j * SquareWidth(), \
                    mTop + i * SquareHeight(), Forme);
        }
    }
}

void BaseMatrice::DrawCurrPiece(wxPaintDC& dc) {
    if (curPiece.GetForme() != NoForme) {
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            if(y<BoardHeight)DrawSquare(dc, mLeft + x * SquareWidth(), \
                mTop + (BoardHeight - y - 1) * SquareHeight(), \
                curPiece.GetForme());
        }
        DrawMatrixFrame(dc);
    }
}

void BaseMatrice::DrawInfoFrame(wxPaintDC& dc,  float top, float totHeight) {
    wxPen pen(*wxBLACK_PEN);
    pen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(pen);
    dc.DrawLine(wxPoint(infoLeft, top), wxPoint(infoLeft, top + totHeight));
    dc.DrawLine(wxPoint(infoLeft + infoWidth, top), wxPoint(infoLeft + infoWidth, top + totHeight));
    dc.DrawLine(wxPoint(infoLeft, top), wxPoint(infoLeft + infoWidth, top));
    dc.DrawLine(wxPoint(infoLeft, top + totHeight), wxPoint(infoLeft + infoWidth, top + totHeight));
    dc.DrawLine(wxPoint(infoLeft, top+titleHeight), wxPoint(infoLeft+infoWidth, top + titleHeight));
}
 
void BaseMatrice::DrawAllInfoBox(wxPaintDC& dc) {
    DrawInfoFrame(dc, uNameTop, titleHeight + textHeight);
    DrawInfoFrame(dc, scoreTop, titleHeight + textHeight);
    if (isDisplayOnly) return;
    DrawInfoFrame(dc, nextTop, titleHeight + shapeBoxHeight);
    DrawInfoFrame(dc, holdTop, titleHeight + shapeBoxHeight);
    DrawShapeBox(dc, holdTop + titleHeight, hold);
    DrawShapeBox(dc, nextTop+titleHeight, next);
}

void BaseMatrice::DrawShapeBox(wxPaintDC& dc, float top, Tetrominoes* info) {
    
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (info[i*6+j] != NoForme)
                DrawSquare(dc, infoLeft + j * SquareWidth(), \
                    top + i * SquareHeight(), info[i * 6 + j]);
        }
    }
}


void BaseMatrice::UpdateScore(int add) {    
    score += add;
    std::string str = std::to_string(score);
    wxString tmp(str);
    uScore->SetLabelText(tmp);

}

// member functions for class Matrice
void Matrice::DropDown()
{
    int newY = curY;
    while (newY > 0) {
        if (!TryMove(curPiece, curX, newY - 1))
            break;
        --newY;
    }
    PieceDropped();
}

void Matrice::OneLineDown()
{
    if (!TryMove(curPiece, curX, curY - 1))
        PieceDropped();
}

void Matrice::PieceDropped()
{
    for (int i = 0; i < 4; ++i) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        Tetrominoes tmp = curPiece.GetForme();
        SetFormeAt(x, y, tmp);
    }

    int nLine_rm = RemoveFullLines();
    
    int cur_y_max = curY - curPiece.MinY() - nLine_rm;
    if (cur_y_max >= BoardHeight) {
        isFallingFinished = true;
        if (isAlive) {
            isAlive = false;
            m_parent->m_n_players_on--;
        }
    }

    if (!isFallingFinished)
        NewPiece();
}



int Matrice::RemoveFullLines()
{
    int numFullLines = 0;
    int niveau = BoardHeight;

    for (int i = BoardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < BoardWidth; ++j) {
            if (FormeAt(j, i) == NoForme) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            ++numFullLines;
            if (i < niveau) { niveau = i; }
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j) {
                    Tetrominoes tmp = FormeAt(j, k + 1);
                    SetFormeAt(j, k, tmp);
                }
            }
        }
    }

    if (numFullLines > 0) {
        numLinesRemoved += numFullLines;
        int add;
        switch (numFullLines)
        {
        case 1:
            add = 40;
            break;
        case(2):
            add = 100;
            break;
        case(3):
            add = 300;
            break;
        case(4):
            add = 1200;
            break;
        default:
            break;
        }
        if (level == EASY)UpdateScore(add);
        else UpdateScore(add * (niveau+1));

        isFallingFinished = true;
        curPiece.SetForme(NoForme);

        Refresh();
    }
    return numFullLines;
}

void Matrice::NewPiece()
{
    if (nextPiece.GetForme() == NoForme) {
        curPiece.SetRandomForme();
        nextPiece.SetRandomForme();
    }
    else {
        curPiece.SetForme(nextPiece.GetForme());
        nextPiece.SetRandomForme();
    }

    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.MinY();

    if (!TryMove(curPiece, curX, curY)) {
        wxString str;
        str.Printf(wxT("%s calling.\n"), __FUNCTION__);
        curPiece.SetForme(NoForme);

        if (m_parent->m_n_players > 1) {
            if (isOnline) {
                isAlive = false;
                m_parent->onlineFlag ^= 1 << 0;
            }
            else if(isAlive){
                isAlive = false;
                m_parent->m_n_players_on--;
            }
        }
        else {
            m_parent->StopGame();
        }
    }

    Forme tmpCurr = curPiece, tmpNext = nextPiece;
    ClearShapeBox();

    for (int i = 0; i < 4; ++i) {
        int xhold = tmpCurr.x(i) - tmpCurr.MinX() + 1;
        int yhold = tmpCurr.y(i) - tmpCurr.MinY() + 1;
        hold[yhold * 6 + xhold] = tmpCurr.GetForme();
        int xnext = tmpNext.x(i) - tmpNext.MinX() + 1;
        int ynext = tmpNext.y(i) - tmpNext.MinY() + 1;
        next[ynext * 6 + xnext] = tmpNext.GetForme();
    }

}

bool Matrice::TryMove(const Forme& newPiece, int newX, int newY)
{
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (y<BoardHeight && FormeAt(x, y) != NoForme)
            return false;
    }

    curPiece = newPiece;
    curX = newX;
    curY = newY;

    Refresh();

    return true;
}