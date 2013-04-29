
#include "scorestage.h"


ScoreStage::ScoreStage()
{
}

ScoreStage::~ScoreStage()
{
}

void ScoreStage::Begin()
{
	settingsImg = spLoadSurface( ".//resource//settings.png" );
	fontNormHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	fontShdwHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	spFontAddRange( fontNormHnd, ' ', '~', SDL_MapRGB( display->format, 192, 220, 255 ) );
	spFontAddRange( fontShdwHnd, ' ', '~', SDL_MapRGB( display->format, 0, 0, 0 ) );

	fontNormTHnd = spFontLoad( ".//resource//standard.ttf", 48 );
	fontShdwTHnd = spFontLoad( ".//resource//standard.ttf", 48 );
	spFontAddRange( fontNormTHnd, ' ', '~', SDL_MapRGB( display->format, 220, 192, 128 ) );
	spFontAddRange( fontShdwTHnd, ' ', '~', SDL_MapRGB( display->format, 0, 0, 0 ) );

	fontNorm2Hnd = spFontLoad( ".//resource//standard.ttf", 32 );
	fontShdw2Hnd = spFontLoad( ".//resource//standard.ttf", 32 );
	spFontAddRange( fontNorm2Hnd, ' ', '~', SDL_MapRGB( display->format, 220, 192, 128 ) );
	spFontAddRange( fontShdw2Hnd, ' ', '~', SDL_MapRGB( display->format, 0, 0, 0 ) );
}

void ScoreStage::Pause()
{
}

void ScoreStage::Resume()
{
}

void ScoreStage::Finish()
{
	SDL_FreeSurface( settingsImg );
  spFontDelete( fontNormHnd );
	spFontDelete( fontShdwHnd );
}


void ScoreStage::Event(SDL_Event *e)
{
	if( e->type == SDL_KEYDOWN )
		gameStack->Pop();
}


void ScoreStage::Update()
{
}

void ScoreStage::Render()
{
	char menuTextList[200];

	int xAdj = (gameSettings->ScreenWidth / 2) - (settingsImg->w / 2);
	int yAdj = (gameSettings->ScreenHeight / 2) - (settingsImg->h / 2);

	spSetZSet( 0 );
	spSetZTest( 0 );

	spClearTarget(SDL_MapRGB( display->format, 0, 0, 0 ));
	spBlitSurface( (gameSettings->ScreenWidth / 2), (gameSettings->ScreenHeight / 2), -10, settingsImg );

	spFontDrawMiddle( (gameSettings->ScreenWidth / 2) + 2, yAdj + 12, -5, "High Score Table", fontShdwTHnd );
	spFontDrawMiddle( (gameSettings->ScreenWidth / 2), yAdj + 10, -4, "High Score Table", fontNormTHnd );

	spFontDraw( xAdj + 26, yAdj + 58, -5, "Rank", fontShdw2Hnd );
	spFontDraw( xAdj + 24, yAdj + 56, -4, "Rank", fontNorm2Hnd );
	spFontDraw( xAdj + 26, yAdj + 54 + fontNorm2Hnd->maxheight - 2, -5, "------", fontShdw2Hnd );
	spFontDraw( xAdj + 24, yAdj + 52 + fontNorm2Hnd->maxheight - 2, -4, "------", fontNorm2Hnd );


	spFontDraw( xAdj + ((settingsImg->w / 5) * 1)+2, yAdj + 58, -5, "Game #", fontShdw2Hnd );
	spFontDraw( xAdj + ((settingsImg->w / 5) * 1), yAdj + 56, -4, "Game #", fontNorm2Hnd );
	spFontDraw( xAdj + ((settingsImg->w / 5) * 1)+2, yAdj + 54 + fontNorm2Hnd->maxheight - 2, -5, "------------", fontShdw2Hnd );
	spFontDraw( xAdj + ((settingsImg->w / 5) * 1), yAdj + 52 + fontNorm2Hnd->maxheight - 2, -4, "------------", fontNorm2Hnd );

	spFontDraw( xAdj + ((settingsImg->w / 5) * 3)+2, yAdj + 58, -5, "Score", fontShdw2Hnd );
	spFontDraw( xAdj + ((settingsImg->w / 5) * 3), yAdj + 56, -4, "Score", fontNorm2Hnd );
	spFontDraw( xAdj + ((settingsImg->w / 5) * 3)+2, yAdj + 54 + fontNorm2Hnd->maxheight - 2, -5, "------------", fontShdw2Hnd );
	spFontDraw( xAdj + ((settingsImg->w / 5) * 3), yAdj + 54 + fontNorm2Hnd->maxheight - 2, -4, "------------", fontNorm2Hnd );

	for( int i = 0; i < 10; i++ )
	{
		sprintf((char*)menuTextList, "%d", (i+1) );
		spFontDrawMiddle( xAdj + 62, yAdj + 112 + (i * 28), -5, (char*)menuTextList, fontShdw2Hnd );
		spFontDrawMiddle( xAdj + 60, yAdj + 110 + (i * 28), -4, (char*)menuTextList, fontNorm2Hnd );

		sprintf((char*)menuTextList, "%d", gameSettings->Top10GameNum[i] );
		spFontDraw( xAdj + ((settingsImg->w / 5) * 1)+2, yAdj + 112 + (i * 28), -5, (char*)menuTextList, fontShdwHnd );
		spFontDraw( xAdj + ((settingsImg->w / 5) * 1), yAdj + 110 + (i * 28), -4, (char*)menuTextList, fontNormHnd );

		sprintf((char*)menuTextList, "%d", (int)(gameSettings->Top10GameScores[i]) );
		spFontDraw( xAdj + ((settingsImg->w / 5) * 3)+2, yAdj + 112 + (i * 28), -5, (char*)menuTextList, fontShdwHnd );
		spFontDraw( xAdj + ((settingsImg->w / 5) * 3), yAdj + 110 + (i * 28), -4, (char*)menuTextList, fontNormHnd );

	}

	spSetZSet( 1 );
	spSetZTest( 1 );

}
