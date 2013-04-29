
#include "settings.h"

int resolutionListW[6] = { 800, 800, 1024, 1280, 1600, 1920 };
int resolutionListH[6] = { 480, 600, 768, 720, 900, 1080 };


Settings::Settings()
{
	Init();
	Load();
}

Settings::~Settings()
{
}

void Settings::Init()
{
	ScreenWidth = 800;
	ScreenHeight = 480;
	ScreenWidthNew = ScreenWidth;
	ScreenHeightNew = ScreenHeight;
	FullScreen = 0;

	PlaySFX = 1;
	PlayMusic = 1;

	keyLeft = SDLK_LEFT;
	keyRight = SDLK_RIGHT;
	keyAccelerate = SDLK_UP;
	keyDecelerate = SDLK_DOWN;
	keyMenuAccept = SDLK_RETURN;
	keyQuit = SDLK_ESCAPE;

	for( int i = 0; i < 10; i++ )
	{
		Top10GameNum[i] = 0;
		Top10GameScores[i] = 0;
	}

}

void Settings::Load()
{
	FILE* fileHnd;

	fileHnd = fopen( "settings.cfg", "rb" );
	if( fileHnd != 0 )
	{
		fread( &ScreenWidth, sizeof( ScreenWidth ), 1, fileHnd );
		fread( &ScreenHeight, sizeof( ScreenHeight ), 1, fileHnd );
		ScreenWidthNew = ScreenWidth;
		ScreenHeightNew = ScreenHeight;
		fread( &FullScreen, sizeof( FullScreen ), 1, fileHnd );

		fread( &PlaySFX, sizeof( PlaySFX ), 1, fileHnd );
		fread( &PlayMusic, sizeof( PlayMusic ), 1, fileHnd );

		fread( &keyLeft, sizeof( keyLeft ), 1, fileHnd );
		fread( &keyRight, sizeof( keyRight ), 1, fileHnd );
		fread( &keyAccelerate, sizeof( keyAccelerate ), 1, fileHnd );
		fread( &keyDecelerate, sizeof( keyDecelerate ), 1, fileHnd );
		fread( &keyMenuAccept, sizeof( keyMenuAccept ), 1, fileHnd );
		fread( &keyQuit, sizeof( keyQuit ), 1, fileHnd );

		for( int i = 0; i < 10; i++ )
		{
			fread( &Top10GameNum[i], sizeof( Top10GameNum[i] ), 1, fileHnd );
			fread( &Top10GameScores[i], sizeof( Top10GameScores[i] ), 1, fileHnd );
		}

		fclose( fileHnd );
	} else {
		Init();
	}
}

void Settings::Save()
{
	FILE* fileHnd;

	fileHnd = fopen( "settings.cfg", "wb" );
	if( fileHnd != 0 )
	{
		fwrite( &ScreenWidthNew, sizeof( ScreenWidthNew ), 1, fileHnd );
		fwrite( &ScreenHeightNew, sizeof( ScreenHeightNew ), 1, fileHnd );
		fwrite( &FullScreen, sizeof( FullScreen ), 1, fileHnd );

		fwrite( &PlaySFX, sizeof( PlaySFX ), 1, fileHnd );
		fwrite( &PlayMusic, sizeof( PlayMusic ), 1, fileHnd );

		fwrite( &keyLeft, sizeof( keyLeft ), 1, fileHnd );
		fwrite( &keyRight, sizeof( keyRight ), 1, fileHnd );
		fwrite( &keyAccelerate, sizeof( keyAccelerate ), 1, fileHnd );
		fwrite( &keyDecelerate, sizeof( keyDecelerate ), 1, fileHnd );
		fwrite( &keyMenuAccept, sizeof( keyMenuAccept ), 1, fileHnd );
		fwrite( &keyQuit, sizeof( keyQuit ), 1, fileHnd );

		for( int i = 0; i < 10; i++ )
		{
			fwrite( &Top10GameNum[i], sizeof( Top10GameNum[i] ), 1, fileHnd );
			fwrite( &Top10GameScores[i], sizeof( Top10GameScores[i] ), 1, fileHnd );
		}

		fclose( fileHnd );
	}
}

void Settings::NextResolution()
{
	for( int i = 0; i < 6; i++ )
	{
		if( resolutionListW[i] == ScreenWidthNew && resolutionListH[i] == ScreenHeightNew )
		{
			ScreenWidthNew = resolutionListW[(i + 1) % 6];
			ScreenHeightNew = resolutionListH[(i + 1) % 6];
			break;
		}
	}
}

