#pragma once

namespace slv
{
	namespace frwk
	{
		namespace appl
		{
			class Screen
			{
				// screen/window width in pixels
				static int s_width;
				// screen/window height in pixels
				static int s_height;
				static float s_aspectRatio;
			public:
				static int Init();

				// screen/window width in pixels
				static inline int WidthPx() { return s_width; }
				// screen/window height in pixels
				static inline int HeightPx() { return s_height; }
				static inline float AspectRatio() { return s_aspectRatio; }
			};
		}
	}
}