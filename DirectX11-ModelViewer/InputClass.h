#pragma once
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include<dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();
	bool IsUpArrowPressed();
	bool IsDownArrowPressed();

	bool IsWPressed();
	bool IsAPressed();
	bool IsSPressed();
	bool IsDPressed();

	void GetMouseLocation(int&, int&);
	bool IsMouse0Pressed();
	bool IsMouse1Pressed();

	float GetMouseX();
	float GetMouseY();
	

public:
	static InputClass* GetInstance()
	{
		return instance;
	}

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
private:
	static InputClass* InputClass::instance;

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};


