#pragma once
#include <dinput.h>

namespace slv
{
	namespace modules
	{
		namespace input
		{
			enum KeyCode
			{
				A = DIK_A, B = DIK_B,
				C = DIK_C, D = DIK_D,
				E = DIK_E, F = DIK_F,
				G = DIK_G, H = DIK_H,
				I = DIK_I, J = DIK_J,
				K = DIK_K, L = DIK_L,
				M = DIK_M, N = DIK_N,
				O = DIK_O, P = DIK_P,
				Q = DIK_Q, R = DIK_R,
				S = DIK_S, T = DIK_T,
				U = DIK_U, V = DIK_V,
				W = DIK_W, X = DIK_X,
				Y = DIK_Y, Z = DIK_Z,

				Up = DIK_UP,
				Down = DIK_DOWN,
				Left = DIK_LEFT,
				Right = DIK_RIGHT,

				Esc = DIK_ESCAPE,
				LCtrl = DIK_LCONTROL,
				RCtrl = DIK_RCONTROL,
				LShift = DIK_LSHIFT,
				RShift = DIK_RSHIFT,
			};
		}
	}
}