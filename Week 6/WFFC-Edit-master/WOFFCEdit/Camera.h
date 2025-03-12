#pragma once
#include "d3d12.h"
#include <SimpleMath.h>
#include "StepTimer.h"
#include "SceneObject.h"
#include "DisplayObject.h"
#include "DisplayChunk.h"
#include "ChunkObject.h"
#include "InputCommands.h"
#include <vector>

class Camera
{
public:
	//functions
	void Initialise();
	void Update(InputCommands& Input);
	void Focus(DirectX::SimpleMath::Vector3 objectPosition);

	//position and rotation vectors
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 orientation;
	DirectX::SimpleMath::Vector3 focusTarget;
	DirectX::SimpleMath::Vector3 lookDir;
	DirectX::SimpleMath::Vector3 camRight;
	DirectX::SimpleMath::Vector3 camUp;

	DirectX::SimpleMath::Matrix focusedView;	//view calculation
	float sensitivity = 5.0f, moveSpeed = 1.5f;	//mouse sensitivity and movement speed
	float xOffset = 2, yOffset = 1, zOffset = -3;	//how much to offset the focus camera's osition compared to the object
	bool cameraActive;

private:
	//internal functions
	void RotCam();
	void MoveCam();

	//internal vars
	float prevMouseX, prevMouseY;	//where to compare the mouse's current pos to when moving, usually the position last frame
	bool cameraStart = true;	//whether or not the camera has just started moving (i.e. when the mouse button is first clicked)
	InputCommands inputs;	//reference to input commands (when mouse/kb is pressed)
};