#pragma once
#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#include "GraphicsClass.h"
#include "Time.h"
#include "InputClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	float DeltaTime() const { return mTime->GetTime(); }

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int& screenWidth, int& screenHeight);
	void ShutdownWindows();

private:
	LPCWSTR mApplicationName;
	HINSTANCE mHinstance;
	HWND mHwnd;

	GraphicsClass* mGraphics;
	Time* mTime;
	InputClass* mInput;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

