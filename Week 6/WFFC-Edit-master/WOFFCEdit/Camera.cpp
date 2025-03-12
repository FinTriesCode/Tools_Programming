#include "Camera.h"
#include <algorithm>
#include "Game.h"

void Camera::Initialise()
{
	//default vectors
	pos.x = 0.0f;
	pos.y = 3.7f;
	pos.z = -3.5f;

	orientation.x = 0;
	orientation.y = 0;
	orientation.z = 0;

	focusTarget.x = 0.0f;
	focusTarget.y = 0.0f;
	focusTarget.z = 0.0f;

	lookDir.x = 0.0f;
	lookDir.y = 0.0f;
	lookDir.z = 1.0f;

	camUp.x = 0;
	camUp.y = 0;
	camUp.z = 0;

	camRight.x = 1.0f;
	camRight.y = 0.0f;
	camRight.z = 0.0f;
}

void Camera::Update(InputCommands& Input)
{
	if (cameraActive)
	{
		inputs = Input;	//get reference to inputs (for WASD and mouse)

		MoveCam();

		//rotate if pushing right mouse button, otherwise tell program to reset camera center to mouse pos next time its pressed (prevent snapping if user moves the mouse
		//when not rotating the camera
		if (inputs.rightMouseDown)
			RotCam();
		else
			cameraStart = true;

		//update lookat point
		focusTarget = pos + lookDir;

		//apply camera vectors
		focusedView = DirectX::SimpleMath::Matrix::CreateLookAt(pos, focusTarget, DirectX::SimpleMath::Vector3::UnitY);
	}
}

void Camera::MoveCam()
{
	pos += (inputs.right - inputs.left) * moveSpeed * camRight;
	pos.y += (inputs.up - inputs.down) * moveSpeed;
	pos += (inputs.forward - inputs.back) * moveSpeed * lookDir;
}

void Camera::RotCam()
{
	//recenter camera around mouse if it has been moved without the camera being rotated (i.e. if moved to the right since last rotation, the camera would snap right without this)
	if (cameraStart)
	{
		prevMouseX = inputs.mouseX;
		prevMouseY = inputs.mouseY;
		cameraStart = false;
		return;
	}

	float sensitivity = 0.3f;

	//work out movement based on where mouse was last frame compared to now
	float deltaX = (inputs.mouseX - prevMouseX) * sensitivity;
	float deltaY = (inputs.mouseY - prevMouseY) * sensitivity;

	//make this mouse pos the prev for next frame
	prevMouseX = inputs.mouseX;
	prevMouseY = inputs.mouseY;

	//change yaw and pitch
	orientation.y += deltaX; //yaw (side)
	orientation.x -= deltaY; //pitch(up)

	//clamp to stop from flipping
	orientation.x = (std::max)(-89.0f, (std::min)(orientation.x, 89.0f));

	// Convert to radians
	float yaw = orientation.y;
	float pitch = orientation.x;

	//use formula from wiki
	lookDir = DirectX::SimpleMath::Vector3(cos(yaw * 3.1415f / 180) * cos(pitch * 3.1415 / 180), sin(pitch * 3.1415 / 180),
		sin(yaw * 3.1415 / 180) * cos(pitch * 3.1415 / 180));
	lookDir.Normalize();

	//change right vec
	lookDir.Cross(DirectX::SimpleMath::Vector3::UnitY, camRight);
}

void Camera::Focus(DirectX::SimpleMath::Vector3 target)
{
	pos.x = target.x + xOffset;
	pos.y = target.y + yOffset;
	pos.z = target.z + zOffset;

	///set the camera to look at (focus) the selected object's coordinates
	focusTarget = target;

	//get look direction by calculating the relative position
	lookDir = focusTarget - pos;
	lookDir.Normalize();

	//get the right vector
	lookDir.Cross(DirectX::SimpleMath::Vector3::UnitY, camRight);
	camRight.Normalize();

	//update to return data to the main camera of game class
	focusedView = DirectX::SimpleMath::Matrix::CreateLookAt(pos, focusTarget, DirectX::SimpleMath::Vector3::UnitY);
}