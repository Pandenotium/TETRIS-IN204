#include "framework.h"
#include "Tetris.h"
#include "Puit.h"
#include <string>
#include "MyButton.h"

Tetris::Tetris(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(460, 600))
{
    m_background = new startMenuBackground(this);
    m_background->Show(true);

    // main menu
    m_start_button = new MyButton(m_background, wxID_FORWARD, wxT("Start Game                                            "), wxPoint(130, 300), "blue", wxSize(190, 49));
    m_info_button = new MyButton(m_background, wxID_ABOUT, wxT("Instructions                                             "), wxPoint(130, 375), "green", wxSize(190, 49));
    m_exit_button = new MyButton(m_background, wxID_EXIT, wxT("Quit Game                                            "), wxPoint(130, 450), "red", wxSize(190, 49));

    // game mode menu
    m_single_player = new MyButton(m_background, wxStandardID(ID_singleplayer), wxT("Single Player                                            "), wxPoint(130, 225), "blue", wxSize(190, 49));
    m_multi_player_local = new MyButton(m_background, wxStandardID(ID_multiplayer_local), wxT("Local Double Player                                          "), wxPoint(130, 300), "green", wxSize(190, 49));
    m_multi_player_online = new MyButton(m_background, wxStandardID(ID_multiplayer_online), wxT("Online Double Player                                          "), wxPoint(130, 375), "yellow", wxSize(190, 49));
    m_back_button = new MyButton(m_background, wxID_BACKWARD, wxT("Back                                               "), wxPoint(130, 450), "red", wxSize(190, 49));
    // hide game mode menu
    m_single_player->Hide();
    m_multi_player_local->Hide();
    m_multi_player_online->Hide();
    m_back_button->Hide();

    // online game menu
    m_create_game = new MyButton(m_background, wxStandardID(ID_create_game), wxT("Create Game                                            "), wxPoint(130, 225), "blue", wxSize(190, 49));
    m_join_game = new MyButton(m_background, wxStandardID(ID_join_game), wxT("Join Game                                            "), wxPoint(130, 300), "green", wxSize(190, 49));
    m_back_button_online = new MyButton(m_background, wxStandardID(ID_back_online), wxT("Back                                               "), wxPoint(130, 450), "red", wxSize(190, 49));
    // hide online game menu
    m_create_game->Hide();
    m_join_game->Hide();
    m_back_button_online->Hide();

    m_server = NULL;
    m_server_worker = NULL;
    m_client_worker = NULL;

    Njoueur = 1;

    Center();
}

// event table
BEGIN_EVENT_TABLE(Tetris, wxFrame)
EVT_BUTTON(wxID_EXIT, Tetris::OnQuit)
EVT_BUTTON(wxID_FORWARD, Tetris::EnterGameMenu)
EVT_BUTTON(wxID_ABOUT, Tetris::DisplayInst)
EVT_BUTTON(wxStandardID(ID_singleplayer), Tetris::OnStartSingle)
EVT_BUTTON(wxStandardID(ID_multiplayer_local), Tetris::OnStartMultiLocal)
EVT_BUTTON(wxStandardID(ID_multiplayer_online), Tetris::EnterOnlineMenu)
EVT_BUTTON(wxID_BACKWARD, Tetris::ReturnMainMenu)
EVT_BUTTON(wxStandardID(ID_create_game), Tetris::OnStartMultiServer)
EVT_BUTTON(wxStandardID(ID_join_game), Tetris::OnStartMultiClient)
EVT_BUTTON(wxStandardID(ID_back_online), Tetris::ReturnGameMenu)
EVT_SOCKET(wxStandardID(ID_server), Tetris::OnServerEvent) // 3.26
EVT_SOCKET(wxStandardID(ID_server_socket), Tetris::OnServerSocketEvent) // 3.26
EVT_SOCKET(wxStandardID(ID_client_socket), Tetris::OnClientSocketEvent) // 3.26
EVT_GAMEOVER(wxStandardID(ID_game_over), Tetris::OnGameOver)
END_EVENT_TABLE()

void Tetris::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void Tetris::GetPlayerName(bool is_online, std::vector<wxString>& uname_ptr) {
    if (!is_online && Njoueur > 1) {
        TwoTextDialog dialog(this->GetParent(), wxT("Game Start!"),
            wxT("Enter Player Name 1"), wxT("Enter Player Name 2"), wxT("Choose Your Level"),
            wxT("JOJO"), wxT("KIKI"));
        dialog.ShowModal();
        uname_ptr.push_back(dialog.GetValue1());
        uname_ptr.push_back(dialog.GetValue2());
        level = dialog.GetLevel();
    }
    else {
        OneTextDialog dialog(this->GetParent(), wxT("Game Start!"),
            wxT("Enter Player Name 1"), wxT("Choose Your Level"),
            wxT("JOJO"));
        dialog.ShowModal();
        uname_ptr.push_back(dialog.GetValue1());
        level = dialog.GetLevel();
    }
}

void Tetris::OnStart(bool is_online)
{
    // Set Size of the frame
    fWidth = 340 *Njoueur; fHeight = 500;
    SetClientSize(wxSize(fWidth, fHeight));
    mWidth = min(340./10, 500/.22) * 0.6 * 10;
    mHeight = mWidth * 2.2;

    // Debug info
    m_background->Hide();

    std::vector<wxString> uname_vct;
    GetPlayerName(is_online, uname_vct);
    
    m_gameboards = new BoardPanel(this, fWidth, fHeight, Njoueur, is_online, uname_vct, level);
    m_gameboards->SetSize(wxSize(fWidth, fHeight));
    m_gameboards->SetPosition(wxPoint(0,0));
    Fit();
    m_gameboards->Show(true);
    m_gameboards->SetFocus();
    //move the start to each single part to assure game will start after connection
}

void Tetris::OnStartSingle(wxCommandEvent& event)
{
    Njoueur = 1;
    OnStart(false);
    m_gameboards->Start();
}

void Tetris::OnStartMultiLocal(wxCommandEvent& event)
{
    Njoueur = 2;
    OnStart(false);
    m_gameboards->Start();

}

void Tetris::OnStartMultiServer(wxCommandEvent& event)
{
    Njoueur = 2;
    OnStart(true);
    OnServerStart();
}

void Tetris::OnStartMultiClient(wxCommandEvent& event)
{
    Njoueur = 2;
    OnStart(true);
    OnConnectToServer();
}

void Tetris::OnServerStart()
{
    wxIPV4address addr;
    addr.AnyAddress();
    addr.Service(3000);

    m_server = new wxSocketServer(addr);

    if (!m_server->Ok())
    {
        return;
    }

    m_server->SetEventHandler(*this, wxStandardID(ID_server));
    m_server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_server->Notify(true);
}

void Tetris::OnServerEvent(wxSocketEvent& event)
{
    wxSocketBase* sock = m_server->Accept(false);

    sock->SetEventHandler(*this, wxStandardID(ID_server_socket));
    sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
    sock->Notify(true);

    m_gameboards->Start();
}

void Tetris::OnServerSocketEvent(wxSocketEvent& event)
{
    wxSocketBase* sock = event.GetSocket();
    m_server_worker = sock;

    Datagram* buf;
    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
    {
        buf = this->m_gameboards->GetBoard(1);

        sock->Read(buf, sizeof(Datagram));

        if (buf->sig == -1) {
            this->m_gameboards->onlineFlag ^= 1 << 1;
        }

        this->m_gameboards->SetBoard(buf, 1);

        buf = this->m_gameboards->GetBoard(0);

        sock->Write(buf, sizeof(Datagram));

        break;
    }
    case wxSOCKET_LOST:
    {
        this->m_gameboards->onlineFlag ^= 1 << 1;
        wxMessageBox("Connection Lost...",
            "Game Over", wxOK | wxICON_INFORMATION);
        sock->Destroy();
        break;
    }
    default:
        event.Skip();
    }
}

void Tetris::OnConnectToServer()
{
    wxString IP;
    wxTextEntryDialog dialog(this->GetParent(), wxT("Enter the IP address you would like to connect:"),
        wxT("Please enter a string"),
        wxT("localhost"), wxOK | wxCANCEL);

    if (dialog.ShowModal() == wxID_OK)
        IP = dialog.GetValue();

    wxIPV4address addr;
    addr.Hostname(IP);
    addr.Service(3000);

    wxSocketClient* Socket = new wxSocketClient();

    Socket->SetEventHandler(*this, wxStandardID(ID_client_socket));
    Socket->SetNotify(wxSOCKET_CONNECTION_FLAG |
        wxSOCKET_INPUT_FLAG |
        wxSOCKET_LOST_FLAG);
    Socket->Notify(true);

    Socket->Connect(addr, false);

    m_gameboards->Start();
}

void Tetris::OnClientSocketEvent(wxSocketEvent& event)
{
    wxSocketBase* sock = event.GetSocket();
    m_client_worker = sock;

    Datagram* buf;
    switch (event.GetSocketEvent())
    {
    case wxSOCKET_CONNECTION:
    {
        
        buf = this->m_gameboards->GetBoard(0);

        sock->Write(buf, sizeof(Datagram));
        break;
    }
    case wxSOCKET_INPUT:
    {
        buf = this->m_gameboards->GetBoard(1);

        sock->Read(buf, sizeof(Datagram));

        if (buf->sig == -1) {
            this->m_gameboards->onlineFlag ^= 1 << 1;
        }

        this->m_gameboards->SetBoard(buf, 1);
        

        buf = this->m_gameboards->GetBoard(0);

        sock->Write(buf, sizeof(Datagram));

        break;
    }

    case wxSOCKET_LOST:
    {
        this->m_gameboards->onlineFlag ^= 1 << 1;
        wxMessageBox("Connection Lost...",
            "Game Over", wxOK | wxICON_INFORMATION);
        sock->Destroy();
        break;
    }
    default:
        event.Skip();
    }
}

void Tetris::EnterGameMenu(wxCommandEvent& event)
{
    // hide main menu
    m_start_button->Hide();
    m_info_button->Hide();
    m_exit_button->Hide();
    // display game menu
    m_single_player->Show();
    m_multi_player_local->Show();
    m_multi_player_online->Show();
    m_back_button->Show();
}

void Tetris::ReturnMainMenu(wxCommandEvent& event)
{
    // display main menu
    m_start_button->Show();
    m_info_button->Show();
    m_exit_button->Show();
    // hide game menu
    m_single_player->Hide();
    m_multi_player_local->Hide();
    m_multi_player_online->Hide();
    m_back_button->Hide();
}

void Tetris::EnterOnlineMenu(wxCommandEvent& event)
{
    // hide game menu
    m_single_player->Hide();
    m_multi_player_local->Hide();
    m_multi_player_online->Hide();
    m_back_button->Hide();
    // display online menu
    m_create_game->Show();
    m_join_game->Show();
    m_back_button_online->Show();
}

void Tetris::ReturnGameMenu(wxCommandEvent& event)
{
    // display game menu
    m_single_player->Show();
    m_multi_player_local->Show();
    m_multi_player_online->Show();
    m_back_button->Show();
    // hide online menu
    m_create_game->Hide();
    m_join_game->Hide();
    m_back_button_online->Hide();
}

void Tetris::DisplayInst(wxCommandEvent& event)
{
    wxMessageBox("Player 1/Player 2:\nUp/W: rotate the block\nLeft/A: move to left\nRight/D: move to right\nDown/S: slightly accelerate the falling\nSpace/X: drop current block down",
        "How to play", wxOK | wxICON_INFORMATION);
}

void Tetris::OnGameOver(wxGameOverEvent& event)
{
    wxMessageBox("Game Over !",
        "Game Over", wxOK | wxICON_INFORMATION);
    if (m_server != NULL) m_server->Destroy();
    if (m_server_worker != NULL) m_server_worker->Destroy();
    if (m_client_worker != NULL) m_client_worker->Destroy();
    delete m_gameboards;
    
    m_background->Show(true);
    m_background->SetFocus();
    this->SetSize(wxSize(460, 600));
}

int Tetris::GetPuitSum(int idx)
{
    return m_gameboards->GetPuitSum(idx);
}
