#include "Player.h"

Player::Player(MyD3D& d3d) :Sprite(d3d)
{
	currentInput = INPUT_NONE;
	mDirection = DirectX::SimpleMath::Vector2(0, 0);
}

void Player::UpdateInputValue(int vKey)
{
	currentInput = vKey;
	switch (vKey)
	{
	case (87):  //w
	case (38): //up arrow
		currentInput = INPUT_W;
		break;
	case (65):  //a
	case (37): //left arrow
		currentInput = INPUT_A;
		break;
	case (83):  //s
	case (40): //down arrow
		currentInput = INPUT_S;
		break;
	case (68):  //d
	case (39): //right arrow
		currentInput = INPUT_D;
		break;
	default:
		currentInput = INPUT_NONE;
	}
}
// Remove wierd static call, make it so you pass the keys pressed through to ship on games update.
// Expand the enum for inputs properly
// Make sure you add the cases in for these expanded enum things

void Player::UpdatePosition(float elapsed)
{
	switch (currentInput)
	{
	case (INPUT_UP):
	case (INPUT_W):
		mDirection = DirectX::SimpleMath::Vector2(0, -1);
		break;
	case (INPUT_A):
		mDirection = DirectX::SimpleMath::Vector2(-1, 0);
		break;
	case (INPUT_S):
		mDirection = DirectX::SimpleMath::Vector2(0, 1);
		break;
	case (INPUT_D):
		mDirection = DirectX::SimpleMath::Vector2(1, 0);
		break;
	default:
		//assert(false && "BAD INPUT!");
	}

	mDesiredPos = mPos + mDirection * mSpeed * elapsed;
	int w = WinUtil::Get().GetClientWidth();
	int h = WinUtil::Get().GetClientHeight();
	if (mDesiredPos.x <= 0) //check its not going too far left
	{
		mDesiredPos.x = w;
	}
	else if (mDesiredPos.x >= w) //check its not going too far right
	{
		mDesiredPos.x = 0;
	}
	else if (mDesiredPos.y <= 0) //check its not going to far up
	{
		mDesiredPos.y = h;
	}
	else if (mDesiredPos.y >= h) //check its not goind to far down
	{
		mDesiredPos.y = 0;
	}
	mPos = mDesiredPos;
}



