#pragma once
#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

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
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

