#pragma once

struct InputCommands
{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool up, down;
	bool rotRight;
	bool rotLeft;
	bool leftMouseDown = false;
	bool rightMouseDown = false;

	int mouseY = 0;
	int mouseX = 0;

	bool focus, unfocus;
};
