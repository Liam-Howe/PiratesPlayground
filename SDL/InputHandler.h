#pragma once
#include "SDL.h"
#include "Vector2.h"
#include <vector>
#include <unordered_map>
#include "Identifiers.h"

static int const MAX_BUTTONS = 10;
static int MAX_JOYSTICKS = 4;
static int MAX_AXIS_VALUE = 32768;
static float DEADZONE = 5000;


struct JoystickData
{
	JoystickData()
		: m_currentButtonsPressed(MAX_BUTTONS, false)
		, m_previousButtonsPressed(MAX_BUTTONS, false)
		, m_leftAnalogStick{ 0,0 }
		, m_rightAnalogStick{ 0,0 }
		, m_triggers{ 0,0 }
	{}

	std::vector<bool> m_currentButtonsPressed;
	std::vector<bool> m_previousButtonsPressed;
	Vector2 m_leftAnalogStick;
	Vector2 m_rightAnalogStick;
	Vector2 m_triggers;
};

class InputHandler
{
public:
	enum Direction
	{
		Up,
		Down,
		Left,
		Right,
		AnimateUp,
		NoChange
	};

	InputHandler();
	~InputHandler();
	int init();
	bool update(SDL_Event & event);
	bool checkJoystickActive(int joystickID);
	bool isButtonPressed(int joystickId, int buttonID);
	bool didOtherButtonGetPressed(int joystickID, int buttonToIgnore);
	Vector2 getLeftAnalogAxis(int joystickID);
	Vector2 getRightAnalogAxis(int joystickID);
	Vector2 getTriggerAxis(int joystickID);
	Direction getDirection(int joystickID);
	static void vibrate(int joystickID,float strength , int duration);

private:

	int m_currentJoystickID;
	std::vector<bool> m_joySticksEnabled;
	std::vector<SDL_Joystick*> m_joySticks;	
	static std::vector<SDL_Haptic*> m_haptics;
	std::vector<JoystickData*> m_joystickData;
	std::unordered_map<int, int> m_joystickMapping;

	void checkJoystickMapping(int joystickID);
	void onJoyStickAxisMove(SDL_Event &event);
	void onJoystickButtonDown(SDL_Event &event);
	void onJoystickButtonUp(SDL_Event &event);
	void onJoystickConnect(SDL_Event &event);
};

