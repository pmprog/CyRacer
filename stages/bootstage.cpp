
#include "bootstage.h"

#include "splashstage.h"
#include "debugstage.h"

BootStage::BootStage(void)
{
}

BootStage::~BootStage(void)
{
}

void BootStage::Begin()
{
	SDL_WM_SetCaption( "CyRacer 3D", NULL );
	// TODO: Load any pre-requisites
	gameStack->Pop();
	gameStack->Push( (Stage*)(new SplashStage()) );

	//srand( SDL_GetTicks() );
	//gameStack->Push( (Stage*)(new DebugStage( rand() )) );
}

void BootStage::Pause()
{
}

void BootStage::Resume()
{
}

void BootStage::Finish()
{
}

void BootStage::Event(SDL_Event *e)
{
}

void BootStage::Update()
{
}

void BootStage::Render()
{
}
