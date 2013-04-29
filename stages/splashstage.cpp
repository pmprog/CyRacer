
#include "splashstage.h"
#include "splash3stage.h"

SplashStage::SplashStage(void)
{
}

SplashStage::~SplashStage(void)
{
}

void SplashStage::Begin()
{
	pmprogLogo = spLoadSurface(".//resource//pmprog.png");
	StageCount = 0;
}

void SplashStage::Pause()
{
}

void SplashStage::Resume()
{
}

void SplashStage::Finish()
{
  SDL_FreeSurface( pmprogLogo );
}

void SplashStage::Event(SDL_Event *e)
{
	if( e->type == SDL_KEYDOWN )
	{
		if( e->key.keysym.sym == SDLK_ESCAPE )
		  shutDown = 1;
		else
		  LeaveStage();
	}
}

void SplashStage::Update()
{
  StageCount++;

  if( StageCount >= 4 * FRAMES_PER_SECOND )
    LeaveStage();
}

void SplashStage::Render()
{
  spClearTarget( SDL_MapRGB( display->format, 0, 0, 0 ) );
	spBlitSurface( (gameSettings->ScreenWidth / 2), (gameSettings->ScreenHeight / 2), -1, pmprogLogo );
}

void SplashStage::LeaveStage()
{
  gameStack->Pop();
  gameStack->Push( (Stage*)(new Splash3Stage()) );
}
