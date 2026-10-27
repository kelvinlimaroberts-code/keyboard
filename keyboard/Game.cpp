#include "Game.h"
#include "WindowUtils.h"
#include "CommonStates.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;



Game::Game(MyD3D& d3d)
	: mPMode(d3d), mD3D(d3d), mpSB(nullptr)
{
	mpSB = new SpriteBatch(&mD3D.GetDeviceCtx());
}


//any memory or resources we made need releasing at the end
void Game::Release()
{
	delete mpSB;
	mpSB = nullptr;
}

//called over and over, use it to update game logic
void Game::Update(float dTime)
{
	switch (state)
	{
	case State::PLAY:
		mPMode.Update(dTime);
	}
}

//called over and over, use it to render things
void Game::Render(float dTime)
{
	mD3D.BeginRender(Colours::Black);


	CommonStates dxstate(&mD3D.GetDevice());
	mpSB->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &mD3D.GetSampler(true));

	switch (state)
	{
	case State::PLAY:
		mPMode.Render(dTime, *mpSB);
	}

	mpSB->End();

	mD3D.EndRender();
}


PlayMode::PlayMode(MyD3D & d3d)
	:mD3D(d3d), mShip(d3d)
{
	mShip.SetTex(*mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "ship.dds", "ship", true));
	const TexCache::Data& d = mShip.GetTexData();
	mShip.origin = Vector2(d.dim.x/2.f, d.dim.y/2.f);
	mShip.SetScale(Vector2(0.1f, 0.1f));
	mShip.mPos = Vector2(mShip.GetScreenSize().x*0.6f, WinUtil::Get().GetClientHeight() / 2.f);
	mShip.rotation = PI/2.f;
}

void PlayMode::Update(float dTime)
{
}

void PlayMode::Render(float dTime, DirectX::SpriteBatch & batch) 
{
	//hack mode engaged
	Sprite spr(mD3D);
	spr.SetTex(*mD3D.GetCache().LoadTexture(&mD3D.GetDevice(), "backgroundlayers/InTheRightOrderTheyLookLikeThis.dds", "bgnd", true));
	spr.SetScale(Vector2(1, 1));
	spr.Draw(batch);

	//ship
	mShip.Draw(batch);
}



