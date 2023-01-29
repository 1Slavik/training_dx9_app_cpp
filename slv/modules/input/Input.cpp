#include "Input.h"

using Input = slv::modules::input::Input;

#pragma region direct input
LPDIRECTINPUT8 Input::s_pDirectInput = nullptr;
IDirectInputDevice8 *Input::s_pKeyboard = nullptr;
IDirectInputDevice8 *Input::s_pMouse = nullptr;
#pragma endregion direct input

#pragma region keyboard
BYTE Input::s_kbState[256];
BYTE Input::s_kbPrevState[256];
char Input::s_kbChangedState[256];
#pragma endregion keyboard

bool Input::Init(HINSTANCE hInstance, HWND hwnd)
{
	auto res = DirectInput8Create(hInstance,
								  DIRECTINPUT_VERSION,
								  IID_IDirectInput8,
								  (LPVOID *)& s_pDirectInput, nullptr);
	res = s_pDirectInput->CreateDevice(GUID_SysKeyboard, &s_pKeyboard, nullptr);
	res = s_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	res = s_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	return true;
}
void Input::Cleanup()
{
	ReleaseDI8Interfaces();
}

void Input::ReleaseDI8Interfaces()
{
	ReleaseInterface(s_pKeyboard);
	ReleaseInterface(s_pMouse);
	ReleaseInterface(s_pDirectInput);
}

int Input::ReadInput()
{
	s_pKeyboard->Acquire();
	s_pKeyboard->GetDeviceState(sizeof(s_kbState), (LPVOID)s_kbState);

	for (int i = 0; i < 256; ++i)
	{
		s_kbChangedState[i] = (s_kbState[i] >> 1) - (s_kbPrevState[i] >> 1);
	}

	memcpy(s_kbPrevState, s_kbState, sizeof(s_kbPrevState));
	return 0;
}