#include <windows.h>
#include <string>
#include <cassert>
#include <d3d11.h>
#include <vector>

#include "WindowUtils.h"

#include "Game.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;





//if ALT+ENTER or resize or drag window we might want do
//something like pause the game perhaps, but we definitely
//need to let D3D know what's happened (OnResize_Default).
void OnResize(int screenWidth, int screenHeight, MyD3D& d3d)
{
	d3d.OnResize_Default(screenWidth, screenHeight);
}

/*
Intercept and process raw input data events
*/
enum { RAWBUFF_SIZE = 512, KEYBUFF_SIZE = 255 };
unsigned char inBuffer[RAWBUFF_SIZE];
unsigned short keyBuffer[KEYBUFF_SIZE];  //virtual key codes, but only the standard ones (first 256)

//identify the key adn record it in the keyBuffer array
void RawInputEvent(HRAWINPUT rawInput)
{
	UINT dwSize;

	UINT res = GetRawInputData(rawInput, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	if (res != 0)
		return;//bad input so ignore it

	if (dwSize >= RAWBUFF_SIZE)
	{
		DBOUT("Buffer too small. Is " << RAWBUFF_SIZE << " wants " << dwSize);
		assert(false);
	}

	if (GetRawInputData(rawInput, RID_INPUT, inBuffer, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
	{
		DBOUT("GetRawInputData failed");
		assert(false);
	}

	RAWINPUT* raw = (RAWINPUT*)inBuffer;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		unsigned short vkey = raw->data.keyboard.VKey;
		if (vkey < 255)
		{
			unsigned short flags = raw->data.keyboard.Flags;
			if (flags & RI_KEY_BREAK) //key up
				keyBuffer[vkey] = 0;
			else
				keyBuffer[vkey] = 1;
		}
	}
}

//messages come from windows all the time, should we respond to any specific ones?
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 27:
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			break;
		}
	case WM_INPUT:
		//grab raw keyboard events
		RawInputEvent((HRAWINPUT)lParam);
		break;
	}

	//default message handling (resize window, full screen, etc)
	return WinUtil::DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

/*
Tell Windows we want raw keyboard input as well as the usual stuff
*/
void RegisterForRawInput()
{
	RAWINPUTDEVICE rid;
	//these ids trigger keyboard input
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x06;
	// adds HID keyboard and also optionally ignores legacy keyboard messages
	rid.dwFlags = RIDEV_INPUTSINK;// RIDEV_NOLEGACY;   
	rid.hwndTarget = WinUtil::Get().GetMainWnd();

	// RIDEV_NOLEGACY will stop normal message pump WM_CHAR type messages
	// sometimes we might want that, for now we'll leave it, then we can
	// still use the normal message pump for things like player name entry

	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) {
		MessageBox(0, "Cannot get keyboard and mouse input", 0, 0);
		assert(false);
	}
}

//main entry point for the game
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{

	int w(512), h(256);
	//int defaults[] = { 640,480, 800,600, 1024,768, 1280,1024 };
		//WinUtil::ChooseRes(w, h, defaults, 4);
	if (!WinUtil::Get().InitMainWindow(w, h, hInstance, "Fezzy", MainWndProc, true))
		assert(false);

	MyD3D d3d;
	if (!d3d.InitDirect3D(OnResize))
		assert(false);
	WinUtil::Get().SetD3D(d3d);
	d3d.GetCache().SetAssetPath("data/");
	Game game(d3d);
	
	RegisterForRawInput();
	
	bool canUpdateRender;
	float dTime = 0;
	while (WinUtil::Get().BeginLoop(canUpdateRender))
	{
		if (canUpdateRender && dTime>0)
		{
			game.Update(dTime);
			game.Render(dTime);
		}
		dTime = WinUtil::Get().EndLoop(canUpdateRender);
	}

	game.Release();
	d3d.ReleaseD3D(true);	
	return 0;
}

