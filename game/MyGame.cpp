#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame()
: 	m_background("back.jpg"),
	m_ball(50, 32, 16, 16, CColor::Black(), 0)
{
	// initialise variables
	m_cannons[0] = NULL;
	m_cannons[1] = NULL;
	m_turn = 0;
	m_mode = MODE_AIM;
}

CMyGame::~CMyGame()
{
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	if (m_mode == MODE_SHOOT) 
	{
		/////////////////////////////////////////////////
		// this code will only be executed while shooting

		// TODO: 1. Add the gravitation

		// TODO: 2. Check if cannon hit? game over? 

		// TODO: 3. Check if the ball is outside the screen. Test the following cases:
		// - ball to the left of the screen
		// - ball to the right of the screen (screen width = 800)
		// - ball below the screen
			
		// TODO: 4. Check if the ball hit the castle?

	}
	
	// Update the ball's position
	m_ball.Update(GetTime());
}

void CMyGame::OnDraw(CGraphics* g)
{
	// Draw the Background
	g->Blit(CVectorI(0, 0), m_background);

	// Draw the Castle
	for each (CSprite *pSprite in m_castle)
		pSprite->Draw(g);

	// Draw the Cannon Ball - only in SHOOT mode and not Game Over!
	if (m_mode == MODE_SHOOT)
		m_ball.Draw(g);

	// Draw the cannons
	m_cannons[0]->Draw(g);
	m_cannons[1]->Draw(g);

	// Text
	if (IsGameOver())
		*g << font(56) << vcenter << center << "GAME OVER" << flush;
	else
	{
		*g << font(14) << bottom;
		if (m_turn == 0)
			*g << left;				// printed on the left side
		else
			*g << right;			// printed on the right side
		*g << "Left click and drag to aim and shoot!" << endl;
		*g << "Right click and drag to position the cannon." << endl;
	}

	// static int z = 0; if ((z++ / 60) % 2 == 1) *g << font(72) << color(CColor::DarkMagenta()) << centre << top << "TEST VERSION";
}

/////////////////////////////////////////////////////
// Cannon Special Operations

void CMyGame::NextTurn()
{
	m_turn = 1 - m_turn;
	m_mode = MODE_AIM;
}

void CMyGame::Aim(CVector point)
{
	CVector d = point - m_cannons[m_turn]->GetPos();
	float angle = RAD2DEG(atan2(d.m_x, d.m_y));
	if (m_turn == 0) angle -= 90; else angle += 90;
	m_cannons[m_turn]->SetRotation(angle);
}

void CMyGame::Move(float x)
{
	if ((m_turn == 0 && x >= 20 && x <= 130) || (m_turn == 1 && x >= 680 && x <= 780))
		m_cannons[m_turn]->SetX(x);
}

void CMyGame::Shoot()
{
	// calculate ball's initial position
	CVector vec(0, 15);
	m_cannons[m_turn]->LtoG(vec);

	// calculate ball's initial angle
	float angle = m_cannons[m_turn]->GetRotation();
	if (m_turn == 0)
		angle += 90;
	else
		angle -= 90;

	// shoot!
	m_ball.SetPosition(vec);
	m_ball.SetDirection(angle);
	m_ball.SetSpeed(520);
	m_mode = MODE_SHOOT;
}

/////////////////////////////////////////////////////
// Game Life Cycle

// one time initialisation
void CMyGame::OnInitialize()
{
	// create the cannons
	m_cannons[0] = new CSprite(50, 74, 1, 1, 0);
	m_cannons[1] = new CSprite(750, 74, 1, 1, 0);

	// load images
	m_cannons[0]->LoadImage("cannonl.gif", "cannon");
	m_cannons[1]->LoadImage("cannonr.gif", "cannon");
	m_cannons[0]->LoadImage("fire.gif", "fire");
	m_cannons[1]->LoadImage("fire.gif", "fire");
}

// called at the start of a new game - display menu here
void CMyGame::OnDisplayMenu()
{
	StartGame();
}

// called when Game Mode entered
void CMyGame::OnStartGame()
{
	// Revert to the cannon image
	m_cannons[0]->SetImage("cannon");
	m_cannons[1]->SetImage("cannon");

	// setup the cannons (pivot for realistic rotations and random x-coordinate)
	m_cannons[0]->SetPivotFromCenter(0, 0);
	m_cannons[1]->SetPivotFromCenter(0, 0);
	m_cannons[0]->SetPosition(50.f + rand() % 100, 74);
	m_cannons[1]->SetPosition(650.f + rand() % 100, 74);
	m_cannons[0]->SetPivotLocal(-11, -5);
	m_cannons[1]->SetPivotLocal( 11, -6);
	m_cannons[0]->SetRotation(0);
	m_cannons[1]->SetRotation(0);

	// initialise the turn and aiming mode
	m_turn = rand() % 2;
	m_mode = MODE_AIM;

	// dismantle the old castle
	for each (CSprite *pSprite in m_castle)
		delete pSprite;
	m_castle.clear();

	// build a new castle...
	const int W = 16;	// width of the walls
	const int N = 80;	// number of bricks
	int a[W];			// array of heights
	memset(a, 0, sizeof(a));
	for (int i = 0; i < N; i++)
	{
		// we build a random number out of 4 rand() calls to get a better shape
		int random = rand() % (W/4+1) + rand() % (W/4+1) + rand() % (W/4+1) + rand() % (W/4);
		a[random]++;
		if (a[random] > 14) a[random] = 14;	// maximum height limit

		// add a new brick: random is the horizontal position, a[random] is the height here
		m_castle.push_back(new CSprite(400.f + (random-W/2) * 32 + 16, 32.f + a[random] * 32, 32, 32, "brick.jpg", 0));
	}
}

// called when Game is Over
void CMyGame::OnGameOver()
{
	m_cannons[1 - m_turn]->SetImage("fire");
	m_mode = MODE_GAMEOVER;
}

// one time termination code
void CMyGame::OnTerminate()
{
}

/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	if (sym == SDLK_SPACE)
		PauseGame();
	if (sym == SDLK_F2)
		NewGame();
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	if (m_mode == MODE_AIM && bLeft)	// aim if in aiming mode and left button pressed
		Aim(CVector(x, y));
	if (m_mode == MODE_AIM && bRight)	// move if in aiming mode and right button pressed
		Move(x);
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	if (m_mode == MODE_AIM)			// aim if in aiming mode and left button pressed
		Aim(CVector(x, y));
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
	if (m_mode == MODE_AIM)			// shoot if in aiming mode!
	{
		Aim(CVector(x, y));
		Shoot();
	}
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
	if (m_mode == MODE_AIM)			// move if in aiming mode and right button pressed
		Move(x);
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
	if (m_mode == MODE_AIM)			// move if in aiming mode and right button was pressed
		Move(x);
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
