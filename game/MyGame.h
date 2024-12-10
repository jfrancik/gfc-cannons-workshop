#pragma once

class CMyGame : public CGame
{
	// Sprites and Sprite Lists
	CSpriteList m_castle;		// The castle is a list of bricks (sprites)
	CSprite *m_cannons[2];		// Cannons represented as an array of two pointers
	CSpriteOval m_ball;			// cannon ball

	// Graphics (Images)
	CGraphics m_background;		// background image
	
	// Whose Turn
	int m_turn;					// 0 = left cannon, 1 = right cannon

	// Game Mode
	enum { MODE_AIM, MODE_SHOOT, MODE_GAMEOVER } m_mode;

public:
	CMyGame(void);
	~CMyGame(void);

	// Cannon Special Operations
	void NextTurn();				// switches the turn to the next player
	void Aim(CVector point);		// rotates the cannon to aim
	void Move(float x);				// moves the cannon left or right
	void Shoot();					// shoots!

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
