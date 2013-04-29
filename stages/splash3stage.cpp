
#include "splash3stage.h"
#include "titlestage.h"

Splash3Stage::Splash3Stage(void)
{
}

Splash3Stage::~Splash3Stage(void)
{
}

void Splash3Stage::Begin()
{
	sparrowLogo = spLoadSurface( ".//resource//sparrow.png" );
	fontHnd = spFontLoad( ".//resource//standard.ttf", 32 );
	spFontAddRange( fontHnd, ' ', '~', SDL_MapRGB( display->format, 255, 255, 255 ) );
	StageCount = 0;
}

void Splash3Stage::Pause()
{
}

void Splash3Stage::Resume()
{
}

void Splash3Stage::Finish()
{
  SDL_FreeSurface( sparrowLogo );
  spFontDelete( fontHnd );
}

void Splash3Stage::Event(SDL_Event *e)
{
	if( e->type == SDL_KEYDOWN )
	{
		if( e->key.keysym.sym == SDLK_ESCAPE )
		  shutDown = 1;
		else
		  LeaveStage();
	}
}

void Splash3Stage::Update()
{
  StageCount++;

  if( StageCount >= 4 * FRAMES_PER_SECOND )
    LeaveStage();
}

void Splash3Stage::Render()
{
  spClearTarget( SDL_MapRGB( display->format, 0, 0, 0 ) );

  spFontDrawMiddle( gameSettings->ScreenWidth / 2, (gameSettings->ScreenHeight / 2) - sparrowLogo->h - 30, -1, "Using Ziz's Sparrow3D Engine", fontHnd );
  spBlitSurface( (gameSettings->ScreenWidth / 2), (gameSettings->ScreenHeight / 2) - (sparrowLogo->h / 2), -1, sparrowLogo );

	spFontDrawMiddle( gameSettings->ScreenWidth / 2, (gameSettings->ScreenHeight / 2) + 20, -1, "With content from http://opengameart.org\nBy Daniel Stephens\nAnd Julius", fontHnd );

}

void Splash3Stage::LeaveStage()
{
  gameStack->Pop();
	// TODO: Start Main Menu stage here (whether that be the WorldStage (using a dialogue box for the menu), or a new one)
  gameStack->Push( (Stage*)(new TitleStage()) );
}
