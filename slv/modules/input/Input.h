#pragma once
#include <dinput.h>
#include "KeyCode.h"

namespace slv
{
	namespace modules
	{
		namespace input
		{
			class Input
			{
#pragma region direct input
				static LPDIRECTINPUT8 s_pDirectInput;
				static IDirectInputDevice8 *s_pKeyboard;
				static IDirectInputDevice8 *s_pMouse;
#pragma endregion direct input
				
#pragma region keyboard
				static BYTE s_kbState[256];
				static BYTE s_kbPrevState[256];
				static char s_kbChangedState[256];
#pragma endregion keyboard

			public:
				using KeyCode = KeyCode;

				static bool Init(HINSTANCE hInstance, HWND hwnd);
				static void Cleanup();
				
				static int ReadInput();
				static inline bool GetKey(KeyCode keyCode) { return (bool)s_kbState[keyCode]; }
				static inline bool GetKeyDown(KeyCode keyCode) { return s_kbChangedState[keyCode] > 0; }
				static inline bool GetKeyUp(KeyCode keyCode) { return s_kbChangedState[keyCode] < 0; }
			private:
				static void ReleaseDI8Interfaces();
				static inline void ReleaseInterface(IUnknown *pi) { if (pi) { pi->Release(); pi = nullptr; } }
			};
		}
	}
}