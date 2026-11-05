#include "Player.h"

Player::Player(MyD3D& d3d) :Sprite(d3d)
{
	currentInput = INPUT_NONE;
	mDirection = DirectX::SimpleMath::Vector2(0, 0);
}

void Player::UpdateInputValue(int vKey)
{
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

void Player::UpdatePosition(float elapsed)
{
	switch (currentInput)
	{
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
	}

	mPos = mPos + mDirection * mSpeed * elapsed;
}

