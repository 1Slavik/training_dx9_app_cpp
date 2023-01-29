#include "Screen.h"

using Screen = slv::frwk::appl::Screen;

int Screen::s_width = 800;
int Screen::s_height = 600;
float Screen::s_aspectRatio = (float)Screen::s_width / Screen::s_height;

int Screen::Init()
{
	s_width = 800;
	s_height = 600;
	s_aspectRatio = (float)s_width / s_height;
	return 0;
}