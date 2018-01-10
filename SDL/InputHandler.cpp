#include "stdafx.h"
#include "InputHandler.h"
#include <iostream>

std::vector<SDL_Haptic*> InputHandler::m_haptics(MAX_JOYSTICKS, NULL);
InputHandler::InputHandler()
	: m_currentJoystickID(0)
	, m_joySticksEnabled(MAX_JOYSTICKS, false)
	, m_joySticks(MAX_JOYSTICKS, NULL)
	, m_joystickData(MAX_JOYSTICKS, NULL)
{
}

InputHandler::~InputHandler()
{
}

int InputHandler::init()
{
	SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	SDL_JoystickEventState(SDL_ENABLE);
	int currentID = 0;
	int numberOfJoysticks = SDL_NumJoysticks();
	if (numberOfJoysticks > 0)
	{
		for (int i = numberOfJoysticks - 1; i >= 0; i--)
		{
			SDL_Joystick* joystick = SDL_JoystickOpen(i);
			m_joySticks[i] = joystick;
			m_joystickData[i] = new JoystickData();
			m_joySticksEnabled[i] = true;
			m_joystickMapping[currentID] = i;
			m_haptics[i] = SDL_HapticOpen(i);
			SDL_HapticRumbleInit(m_haptics[i]);
			currentID++;
		}
		return numberOfJoysticks;
	}
	return -1;
}

bool InputHandler::update(SDL_Event & event)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return true;
					break;
				}
				break;
			case SDL_JOYAXISMOTION:
				onJoyStickAxisMove(event);
				break; 
			case SDL_JOYBUTTONDOWN:
				onJoystickButtonDown(event);
				break;
			case SDL_JOYBUTTONUP:
				onJoystickButtonUp(event);
				break;
			case SDL_CONTROLLERDEVICEADDED:
				onJoystickConnect(event);
				break;
			default:
				break;
		}
	}
	return false;
}

bool InputHandler::checkJoystickActive(int joystickID)
{
	return m_joySticksEnabled[joystickID];
}

void InputHandler::checkJoystickMapping(int joystickID)
{
	std::unordered_map<int, int>::const_iterator it;
	for (it = m_joystickMapping.begin(); it != m_joystickMapping.end(); ++it)
	{
		if (it->second == joystickID)
		{
			m_joystickMapping.erase(it);
			return;
		}
	}
}

void InputHandler::onJoyStickAxisMove(SDL_Event & event)
{
	if (event.type == SDL_JOYAXISMOTION)
	{
		int joystickID = event.jaxis.which;
		if (event.jaxis.axis == 0)
		{
			if (event.jaxis.value > DEADZONE)
			{
				m_joystickData[m_joystickMapping[joystickID]]->m_leftAnalogStick.x = (event.jaxis.value + DEADZONE) / (MAX_AXIS_VALUE + DEADZONE);
			}
			else if (event.jaxis.value < -DEADZONE)
				m_joystickData[m_joystickMapping[joystickID]]->m_leftAnalogStick.x = (event.jaxis.value + DEADZONE) / (MAX_AXIS_VALUE - DEADZONE);
			else
				m_joystickData[m_joystickMapping[joystickID]]->m_leftAnalogStick.x = 0;
		}
		if (event.jaxis.axis == 1)
		{
			if (event.jaxis.value > DEADZONE)
			{
				m_joystickData[m_joystickMapping[joystickID]]->m_leftAnalogStick.y = (event.jaxis.value + DEADZONE) / (MAX_AXIS_VALUE + DEADZONE);
			}
			else if (event.jaxis.value < -DEADZONE)
				m_joystickData[m_joystickMapping[joystickID]]->m_leftAnalogStick.y = (event.jaxis.value + DEADZONE) / (MAX_AXIS_VALUE - DEADZONE);
			else
				m_joystickData[m_joystickMapping[joystickID]]->m_leftAnalogStick.y = 0;
		}
		/*
		 Left Trigger axis = 2;
		 Right Stick xAxis  = 3;
		 Right Stick yAxis  = 3;
		 Right Trigger axis = 5;
		*/

	}
}

void InputHandler::onJoystickButtonDown(SDL_Event & event)
{
	int joyStickID = event.jaxis.which;
	m_joystickData[m_joystickMapping[joyStickID]]->m_currentButtonsPressed[event.jbutton.button] = true;
}

void InputHandler::onJoystickButtonUp(SDL_Event & event)
{
	int joyStickID = event.jaxis.which;
	m_joystickData[m_joystickMapping[joyStickID]]->m_currentButtonsPressed[event.jbutton.button] = false;
	m_joystickData[m_joystickMapping[joyStickID]]->m_previousButtonsPressed[event.jbutton.button] = false;
}
void InputHandler::onJoystickConnect(SDL_Event &event)
{
	//int joystickID = event.cdevice.which;
	//if (m_joySticks[joystickID] != nullptr)
	//{
	//	SDL_JoystickClose(m_joySticks[joystickID]);
	//	m_joySticks[joystickID] = nullptr;
	//	delete m_joystickData[joystickID];
	//	m_joystickData[joystickID] = nullptr;
	//	m_joySticksEnabled[joystickID] = false;
	//}
	//SDL_Joystick* joystick = SDL_JoystickOpen(joystickID);
	//m_joySticks[joystickID] = joystick;
	//m_joystickData[joystickID] = new JoystickData();
	//m_joySticksEnabled[joystickID] = true;
	//
	//checkJoystickMapping(joystickID);										// Delete map entry if joystick[x] already exists
	//m_joystickMapping[m_currentJoystickID] = joystickID;					// <JoystickID - Key, Joystick[x] -Value>
	//m_currentJoystickID++;
}
bool InputHandler::isButtonPressed(int joystickID, int buttonID)
{
	if (m_joystickData[joystickID]->m_currentButtonsPressed[buttonID] != m_joystickData[joystickID]->m_previousButtonsPressed[buttonID])
	{
		m_joystickData[joystickID]->m_previousButtonsPressed[buttonID] = true;
		return true;
	}
	return false;
}

bool InputHandler::didOtherButtonGetPressed(int joystickID, int buttonToIgnore)
{
	for (int i = 0; i < m_joystickData[joystickID]->m_currentButtonsPressed.size(); ++i)
	{
		if(i != buttonToIgnore)
		{
			if((m_joystickData[joystickID]->m_currentButtonsPressed[i] != m_joystickData[joystickID]->m_previousButtonsPressed[i]) &&  i != JoyStickButtonID::Button_A)
			{
				return true;
			}
		}
	}
	return false;
}

Vector2 InputHandler::getLeftAnalogAxis(int joystickID)
{
	return m_joystickData.at(joystickID)->m_leftAnalogStick;
}

Vector2 InputHandler::getRightAnalogAxis(int joystickID)
{
	return m_joystickData.at(joystickID)->m_rightAnalogStick;
}

Vector2 InputHandler::getTriggerAxis(int joystickID)
{
	return m_joystickData.at(joystickID)->m_triggers;
}

InputHandler::Direction InputHandler::getDirection(int joystickID)
{
	float x = getLeftAnalogAxis(joystickID).x;
	float y = getLeftAnalogAxis(joystickID).y;

	float xAbs = abs(x);
	float yAbs = abs(y);

	if(xAbs > yAbs)
	{
		if (x > (DEADZONE / MAX_AXIS_VALUE))
			return Right;
		else if(x < -(DEADZONE / MAX_AXIS_VALUE))
			return Left;
	}
	else
	{
		if (y >(DEADZONE / MAX_AXIS_VALUE))
			return Down;
		else if(y < -(DEADZONE / MAX_AXIS_VALUE))
			return Up;
	}

	return NoChange;
}
void InputHandler::vibrate(int joystickID, float strength , int duration)
{
	SDL_HapticRumblePlay(m_haptics[joystickID], strength, duration);
}
