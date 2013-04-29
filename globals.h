
#ifndef _globals
#define _globals

#include <bass.h>
#include <sparrow3d.h>
#include "stagestack.h"
#include ".//settings//settings.h"


#ifdef _core

	SDL_Surface*									display = 0;
  StageStack*                   gameStack = 0;
	Settings*											gameSettings = 0;
	int														shutDown = 0;
	int														hasBASS = 0;

#else

	extern SDL_Surface*						display;
  extern StageStack*						gameStack;
	extern Settings*							gameSettings;
	extern int										shutDown;
	extern int										hasBASS;

#endif

#endif
