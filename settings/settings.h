#pragma once

#include <stdio.h>
#include <SDL.h>
#include "..//consts.h"

class Settings
{
	public:
		Settings(void);
		~Settings(void);

		int ScreenWidth;
		int ScreenHeight;
		int ScreenWidthNew;
		int ScreenHeightNew;
		int FullScreen;

		int PlaySFX;
		int PlayMusic;

		void Init();
		void Load();
		void Save();

		SDLKey keyLeft;
		SDLKey keyRight;
		SDLKey keyAccelerate;
		SDLKey keyDecelerate;
		SDLKey keyMenuAccept;
		SDLKey keyQuit;

		int Top10GameNum[10];
		double Top10GameScores[10];

		void NextResolution();

};
