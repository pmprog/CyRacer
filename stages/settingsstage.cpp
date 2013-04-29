
#include "settingsstage.h"


SettingsStage::SettingsStage(void)
{
}

SettingsStage::~SettingsStage(void)
{
}

// Stage control
void SettingsStage::Begin()
{
	settingsImg = spLoadSurface( ".//resource//settings.png" );
	keyReqImg = spLoadSurface( ".//resource//key.png" );
	fontNormHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	fontSelHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	fontShdwHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	spFontAddRange( fontNormHnd, ' ', '~', SDL_MapRGB( display->format, 192, 220, 255 ) );
	spFontAddRange( fontSelHnd, ' ', '~', SDL_MapRGB( display->format, 255, 220, 128 ) );
	spFontAddRange( fontShdwHnd, ' ', '~', SDL_MapRGB( display->format, 0, 0, 0 ) );
	selectedItem = 0;
	requestingKey = 0;
}

void SettingsStage::Pause()
{
}

void SettingsStage::Resume()
{
}

void SettingsStage::Finish()
{
	SDL_FreeSurface( settingsImg );
	SDL_FreeSurface( keyReqImg );
  spFontDelete( fontNormHnd );
	spFontDelete( fontSelHnd );
	spFontDelete( fontShdwHnd );
}

void SettingsStage::Event(SDL_Event *e)
{
	if( e->type == SDL_KEYDOWN )
	{
		if( requestingKey != 0 )
		{
			if( e->key.keysym.sym != gameSettings->keyQuit )
			{
				switch( selectedItem )
				{
					case 4:
						gameSettings->keyLeft = e->key.keysym.sym;
						break;
					case 5:
						gameSettings->keyRight = e->key.keysym.sym;
						break;
					case 6:
						gameSettings->keyAccelerate = e->key.keysym.sym;
						break;
					case 7:
						gameSettings->keyDecelerate = e->key.keysym.sym;
						break;
					case 8:
						gameSettings->keyMenuAccept = e->key.keysym.sym;
						break;
					case 9:
						gameSettings->keyQuit = e->key.keysym.sym;
						break;
				}
			}
			requestingKey = 0;
			return;
		}

		if( e->key.keysym.sym == gameSettings->keyQuit )
		{
			gameSettings->Load();
			gameStack->Pop();
			return;
		}
		if( e->key.keysym.sym == gameSettings->keyAccelerate )
			selectedItem = (selectedItem + 11) % 12;
		if( e->key.keysym.sym == gameSettings->keyDecelerate )
			selectedItem = (selectedItem + 1) % 12;

		if( e->key.keysym.sym == gameSettings->keyMenuAccept )
		{
			switch( selectedItem )
			{
				case 0:
					gameSettings->NextResolution();
					break;
				case 1:
					gameSettings->FullScreen = !gameSettings->FullScreen;
					break;
				case 2:
					gameSettings->PlaySFX = !gameSettings->PlaySFX;
					break;
				case 3:
					gameSettings->PlayMusic = !gameSettings->PlayMusic;
					break;

				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					requestingKey = 1;
					break;

				case 10:
					gameSettings->Save();
					gameStack->Pop();
					break;
				case 11:
					gameSettings->Load();
					gameStack->Pop();
					break;
			}
		}
	}
}

void SettingsStage::Update()
{
}

void SettingsStage::Render()
{
	char menuTextList[200];

	spSetZSet( 0 );
	spSetZTest( 0 );

	spClearTarget(SDL_MapRGB( display->format, 0, 0, 0 ));
	spBlitSurface( (gameSettings->ScreenWidth / 2), (gameSettings->ScreenHeight / 2), -10, settingsImg );

	sprintf((char*)menuTextList, "[%s] Resolution : %d x %d", (selectedItem == 0 ? "X" : " "), gameSettings->ScreenWidthNew, gameSettings->ScreenHeightNew );
	RenderMenuText( 0, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Fullscreen : %s", (selectedItem == 1 ? "X" : " "), (gameSettings->FullScreen ? "Yes" : "No") );
	RenderMenuText( 1, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Play SFX : %s", (selectedItem == 2 ? "X" : " "), (gameSettings->PlaySFX ? "Yes" : "No") );
	RenderMenuText( 2, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Play Music : %s", (selectedItem == 3 ? "X" : " "), (gameSettings->PlayMusic ? "Yes" : "No") );
	RenderMenuText( 3, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Steer Left Key : %s", (selectedItem == 4 ? "X" : " "), SDL_GetKeyName( gameSettings->keyLeft ) );
	RenderMenuText( 4, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Steer Right Key : %s", (selectedItem == 5 ? "X" : " "), SDL_GetKeyName( gameSettings->keyRight ) );
	RenderMenuText( 5, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Accelerate Key: %s", (selectedItem == 6 ? "X" : " "), SDL_GetKeyName( gameSettings->keyAccelerate ) );
	RenderMenuText( 6, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Decelerate Key : %s", (selectedItem == 7 ? "X" : " "), SDL_GetKeyName( gameSettings->keyDecelerate ) );
	RenderMenuText( 7, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Menu Accept Key : %s", (selectedItem == 8 ? "X" : " "), SDL_GetKeyName( gameSettings->keyMenuAccept ) );
	RenderMenuText( 8, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Menu Cancel/Quit Key : %s", (selectedItem == 9 ? "X" : " "), SDL_GetKeyName( gameSettings->keyQuit ) );
	RenderMenuText( 9, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Save and Return", (selectedItem == 10 ? "X" : " ") );
	RenderMenuText( 10, (char*)menuTextList );

	sprintf((char*)menuTextList, "[%s] Cancel", (selectedItem == 11 ? "X" : " ") );
	RenderMenuText( 11, (char*)menuTextList );

	if( requestingKey != 0 )
	{
		int y = (gameSettings->ScreenHeight / 2);
		int x = (keyReqImg->w / 2);
		while( x - (keyReqImg->w / 2) <= gameSettings->ScreenWidth )
		{
			spBlitSurface( x, y, -3, keyReqImg );
			x += keyReqImg->w;
		}
		int x1 = (gameSettings->ScreenWidth / 4);
		int x2 = (gameSettings->ScreenWidth / 4) * 3;
		int y1 = y - (keyReqImg->h / 2) + 4;
		int y2 = y + (keyReqImg->h / 2) - 4;
		spQuad( x1, y1, -2, x1, y2, -2, x2, y2, -2, x2, y1, -2, SDL_MapRGB( display->format, 0, 0, 0 ) );
		
		spFontDrawMiddle( (gameSettings->ScreenWidth / 2), (gameSettings->ScreenHeight / 2) - (fontSelHnd->maxheight/2), -1, "Press Required Key", fontSelHnd );
	}
	spSetZSet( 1 );
	spSetZTest( 1 );

}

void SettingsStage::RenderMenuText(int optionNumber, char* Text)
{
	int menuLeft = (gameSettings->ScreenWidth / 2) - (settingsImg->w / 2) + 20;
	int lineHeight = fontNormHnd->maxheight;

	spFontDraw( menuLeft + 2, (gameSettings->ScreenHeight / 2) - (settingsImg->h / 2) + 22 + (lineHeight * optionNumber), -5, (char*)Text, fontShdwHnd );
	spFontDraw( menuLeft, (gameSettings->ScreenHeight / 2) - (settingsImg->h / 2) + 20 + (lineHeight * optionNumber), -4, (char*)Text, (selectedItem == optionNumber ? fontSelHnd : fontNormHnd) );
}
