
#include "titlestage.h"
#include "debugstage.h"
#include "settingsstage.h"
#include "scorestage.h"

TitleStage::TitleStage(void)
{
	showNumberSelector = 0;
	numSelect10s = 0;
}

TitleStage::~TitleStage(void)
{
}

// Stage control
void TitleStage::Begin()
{
	srand( SDL_GetTicks() );
	titleImg = spLoadSurface( ".//resource//title.png" );
	keyReqImg = spLoadSurface( ".//resource//key.png" );
	fontNormHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	fontSelHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	fontShdwHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	spFontAddRange( fontNormHnd, ' ', '~', SDL_MapRGB( display->format, 92, 128, 192 ) );
	spFontAddRange( fontSelHnd, ' ', '~', SDL_MapRGB( display->format, 192, 128, 0 ) );
	spFontAddRange( fontShdwHnd, ' ', '~', SDL_MapRGB( display->format, 0, 0, 0 ) );
	selectedItem = 0;
	gameNumber = rand() % 999999;

	titleMusic = 0;
	if( hasBASS != 0 && gameSettings->PlayMusic != 0 )
	{
		titleMusic = BASS_StreamCreateFile( FALSE, ".//resource//title.ogg", 0, 0, BASS_SAMPLE_LOOP );
		if( titleMusic != 0 )
			BASS_ChannelPlay( titleMusic, 0 );
	}
}

void TitleStage::Pause()
{
}

void TitleStage::Resume()
{
	srand( SDL_GetTicks() );
	gameNumber = rand() % 999999;

	if( titleMusic != 0 && ( hasBASS == 0 || gameSettings->PlayMusic == 0 ) )
	{
		BASS_ChannelStop( titleMusic );
		BASS_StreamFree( titleMusic );
		titleMusic = 0;
	}
	if( titleMusic == 0 && hasBASS != 0 && gameSettings->PlayMusic != 0 )
	{
		titleMusic = BASS_StreamCreateFile( FALSE, ".//resource//title.ogg", 0, 0, BASS_SAMPLE_LOOP );
		if( titleMusic != 0 )
			BASS_ChannelPlay( titleMusic, 0 );
	}
}

void TitleStage::Finish()
{
  SDL_FreeSurface( titleImg );
	SDL_FreeSurface( keyReqImg );
  spFontDelete( fontNormHnd );
	spFontDelete( fontSelHnd );
	spFontDelete( fontShdwHnd );

	if( titleMusic != 0 )
	{
		BASS_ChannelStop( titleMusic );
		BASS_StreamFree( titleMusic );
		titleMusic = 0;
	}
}

void TitleStage::Event(SDL_Event *e)
{
	if( showNumberSelector != 0 )
	{
		NumSelEvent( e );
		return;
	}

	if( e->type == SDL_KEYDOWN )
	{
		if( e->key.keysym.sym == gameSettings->keyQuit )
			shutDown = 1;
		if( e->key.keysym.sym == gameSettings->keyAccelerate )
			selectedItem = (selectedItem + 4) % 5;
		if( e->key.keysym.sym == gameSettings->keyDecelerate )
			selectedItem = (selectedItem + 1) % 5;

		if( e->key.keysym.sym == gameSettings->keyMenuAccept )
		{
			switch( selectedItem )
			{
				case 0:
					gameStack->Push( (Stage*)(new DebugStage( gameNumber )) );
					break;
				case 1:
					showNumberSelector = 1;
					break;
				case 2:
					gameStack->Push( (Stage*)(new ScoreStage()) );
					break;
				case 3:
					gameStack->Push( (Stage*)(new SettingsStage()) );
					break;
				case 4:
					shutDown = 1;
					break;
			}
		}
	}
}

void TitleStage::Update()
{
}

void TitleStage::Render()
{
	int xAdj = (gameSettings->ScreenWidth / 2) - (titleImg->w / 2);
	int yAdj = (gameSettings->ScreenHeight / 2) - (titleImg->h / 2);

	int menuLeft = (gameSettings->ScreenWidth / 2); // * 7;
	int lineHeight = fontNormHnd->maxheight;

	spSetZSet( 0 );
	spSetZTest( 0 );

	spClearTarget(SDL_MapRGB( display->format, 0, 0, 0 ));
	spBlitSurface( (gameSettings->ScreenWidth / 2), (gameSettings->ScreenHeight / 2), -10, titleImg );

	if( selectedItem == 0 )
	{
		spFontDraw( menuLeft + 2, yAdj + 12, -2, "[X] Play Game", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10, -1, "[X] Play Game", fontSelHnd );
	} else {
		spFontDraw( menuLeft + 2, yAdj + 12, -2, "[ ] Play Game", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10, -1, "[ ] Play Game", fontNormHnd );
	}


	char selGameMsg[200];
	if( selectedItem == 1 )
	{
		sprintf( (char*)&selGameMsg, "[X] Set Game Number:\n    %d", gameNumber );
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 1), -2, (char*)&selGameMsg, fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 1), -1, (char*)&selGameMsg, fontSelHnd );
	} else {
		sprintf( (char*)&selGameMsg, "[ ] Set Game Number:\n    %d", gameNumber );
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 1), -2, (char*)&selGameMsg, fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 1), -1, (char*)&selGameMsg, fontNormHnd );
	}

	if( selectedItem == 2 )
	{
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 3), -2, "[X] High Scores", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 3), -1, "[X] High Scores", fontSelHnd );
	} else {
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 3), -2, "[ ] High Scores", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 3), -1, "[ ] High Scores", fontNormHnd );
	}

	if( selectedItem == 3 )
	{
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 4), -2, "[X] Settings", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 4), -1, "[X] Settings", fontSelHnd );
	} else {
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 4), -2, "[ ] Settings", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 4), -1, "[ ] Settings", fontNormHnd );
	}

	if( selectedItem == 4 )
	{
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 5), -2, "[X] Quit", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 5), -1, "[X] Quit", fontSelHnd );
	} else {
		spFontDraw( menuLeft + 2, yAdj + 12 + (lineHeight * 5), -2, "[ ] Quit", fontShdwHnd );
		spFontDraw( menuLeft, yAdj + 10 + (lineHeight * 5), -1, "[ ] Quit", fontNormHnd );
	}

	if( showNumberSelector != 0 )
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
		
		int calcDigit;
		int gameMinusDigit;

		for( int i = 5; i >= 0; i-- )
		{
			double pow10d = pow(10.0, i);
			int pow10 = (int)pow10d;
			int drawAdj;
			calcDigit = (gameNumber / pow10) % 10;

			drawAdj = (int)((2.5 - (double)i) * 24);

			sprintf( (char*)&selGameMsg, "%d", calcDigit );

			spFontDrawMiddle( (gameSettings->ScreenWidth / 2) + drawAdj, (gameSettings->ScreenHeight / 2) - (fontSelHnd->maxheight/2), -1, (char*)&selGameMsg, (numSelect10s == i ? fontSelHnd : fontNormHnd) );

		}

	}

	spSetZSet( 1 );
	spSetZTest( 1 );

}

void TitleStage::NumSelEvent(SDL_Event *e)
{
	if( e->type == SDL_KEYDOWN )
	{
		if( e->key.keysym.sym == gameSettings->keyMenuAccept )
		{
			// TODO: Set game number
			showNumberSelector = 0;
		}
		if( e->key.keysym.sym == gameSettings->keyQuit )
			showNumberSelector = 0;

		int calcDigit;
		int gameMinusDigit;
		double pow10d = pow(10.0, numSelect10s);
		int pow10 = (int)pow10d;

		calcDigit = (gameNumber / pow10) % 10;
		gameMinusDigit = gameNumber - (calcDigit * pow10);

		if( e->key.keysym.sym == gameSettings->keyAccelerate )
			calcDigit = (calcDigit + 1) % 10;
		if( e->key.keysym.sym == gameSettings->keyDecelerate )
			calcDigit = (calcDigit + 9) % 10;
		gameNumber = gameMinusDigit + (calcDigit * pow10);

		if( e->key.keysym.sym == gameSettings->keyLeft && numSelect10s < 5 )
			numSelect10s++;
		if( e->key.keysym.sym == gameSettings->keyRight && numSelect10s > 0 )
			numSelect10s--;


	}
}
