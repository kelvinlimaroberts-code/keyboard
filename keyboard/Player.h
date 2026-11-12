#pragma once
#include "Sprite.h"
const enum movementInputs 
{ 
	INPUT_NONE = 0,

	INPUT_LEFT = 37,
	INPUT_UP = 38,
	INPUT_RIGHT = 39,
	INPUT_DOWN = 40,

	INPUT_A = 65,
	INPUT_D = 68,
	INPUT_S = 83,
	INPUT_W = 87
};

class Player : public Sprite
{
private:
	float mSpeed = 100;
	DirectX::SimpleMath::Vector2 mDirection;
	DirectX::SimpleMath::Vector2 mDesiredPos;
	movementInputs currentInput;

public:
	Player(MyD3D& d3d);
	void UpdateInputValue(unsigned short vKey);
	void UpdatePosition(float elapsed);
};

