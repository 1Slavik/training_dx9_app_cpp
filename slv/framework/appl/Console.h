#pragma once
#include <iostream>
#include <Windows.h>

namespace slv
{
	// framework
	namespace frwk
	{
		// application
		namespace appl
		{
			// standart windows console
			class WinConsole
			{
			public:
				static bool AttachParentConsole(int16_t minLength);
				static bool CreateNewConsole(int16_t minLength);
				static void AdjustConsoleBuffer(int16_t minLength);
				static bool ReleaseConsole();
				static bool RedirectConsoleIO();
			};
		}
	}
}