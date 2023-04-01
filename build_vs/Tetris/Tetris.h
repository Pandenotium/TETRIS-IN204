#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <wx/wx.h>
#include <wx/socket.h>
#include <string>
#include <vector>
#include "resource.h"
#include "Puit.h"
#include "MyButton.h"
#include "TwoTextDialog.h"

class startMenuBackground : public wxPanel
{
public:
	startMenuBackground(wxFrame* parent) : wxPanel(parent)
	{
		// bind paint event
		Bind(wxEVT_PAINT, &startMenuBackground::OnPaint, this);
	}
	void OnPaint(wxPaintEvent& event)
	{
		wxPaintDC dc(this);

		dc.DrawBitmap(wxBitmap("img\\background.png", wxBITMAP_TYPE_PNG), wxPoint(0, 0));
	}
};

class Tetris : public wxFrame
{
public:
	Tetris(const wxString& title);
	void OnQuit(wxCommandEvent& event);
	void OnStart(bool is_online);
	void OnStartSingle(wxCommandEvent& event);
	void OnStartMultiLocal(wxCommandEvent& event);
	void OnStartMultiServer(wxCommandEvent& event);
	void OnStartMultiClient(wxCommandEvent& event);
	void OnServerStart();
	void OnServerEvent(wxSocketEvent& event);
	void OnServerSocketEvent(wxSocketEvent& event);
	void OnConnectToServer();
	void OnClientSocketEvent(wxSocketEvent& event);
	void EnterGameMenu(wxCommandEvent& event);
	void ReturnMainMenu(wxCommandEvent& event);
	void EnterOnlineMenu(wxCommandEvent& event);
	void ReturnGameMenu(wxCommandEvent& event);
	void DisplayInst(wxCommandEvent& event);
	void OnGameOver(wxGameOverEvent& event);
	int GetPuitSum(int idx);
	void GetPlayerName(bool is_online, std::vector<wxString>& uname_ptr);
private:
	int Njoueur;
	float  fWidth, fHeight, mWidth, mHeight;
	int level;

	// main menu buttons
	MyButton* m_start_button;
	MyButton* m_info_button;
	MyButton* m_exit_button;
	// game mode buttons
	MyButton* m_single_player;
	MyButton* m_multi_player_local;
	MyButton* m_multi_player_online;
	MyButton* m_back_button;
	// online mode buttons
	MyButton* m_create_game;
	MyButton* m_join_game;
	MyButton* m_back_button_online;

	startMenuBackground* m_background;
	BoardPanel* m_gameboards;

	// sockets 3.26
	wxSocketServer* m_server;
	wxSocketBase* m_server_worker;
	wxSocketBase* m_client_worker;

	DECLARE_EVENT_TABLE()
};