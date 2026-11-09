#pragma once
#include "Sprite.h"
const enum movementInputs 
{ 
	INPUT_NONE = 0
	, INPUT_UP = 38

	, INPUT_W = 87
	, INPUT_A = 65
	, INPUT_S = 83
	, INPUT_D = 68 
};
static movementInputs currentInput;

class Player : public Sprite
{
private:
	float mSpeed = 100;
	DirectX::SimpleMath::Vector2 mDirection;
	DirectX::SimpleMath::Vector2 mDesiredPos;
public:
	Player(MyD3D& d3d);
	static void UpdateInputValue(int vKey);
	void UpdatePosition(float elapsed);
};

