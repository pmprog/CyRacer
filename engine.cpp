
#include "engine.h"
#include ".//stages//bootstage.h"

int main(int argc, char **argv)
{
	spInitCore();
	SDL_EnableUNICODE( 1 );

	gameSettings = new Settings();
	hasBASS = BASS_Init( -1, 44100, BASS_DEVICE_DEFAULT, 0, NULL );

	display = spCreateWindow( gameSettings->ScreenWidth, gameSettings->ScreenHeight, gameSettings->FullScreen, 0 );
	resizeWindow( display->w, display->h );

  gameStack = new StageStack();
  gameStack->Push( (Stage*)(new BootStage()) );

	deltaTime = 0;
	spLoop( engineDraw, engineUpdate, 10, resizeWindow, engineSDLEvent);

	spQuitCore();
	return 0;
}

void resizeWindow( Uint16 w, Uint16 h )
{
  //Setup of the new/resized window
  spSelectRenderTarget(spGetWindowSurface());
  spSetPerspective(50.0f,(float)spGetWindowSurface()->w/(float)spGetWindowSurface()->h,0.1f,100.0f);

	/*
  //Font Loading
  if (font)
    spFontDelete(font);
  font = spFontLoad("./font/StayPuft.ttf",20*spGetSizeFactor()>>SP_ACCURACY);
  spFontAddRange(font,' ','~',0);//whole ASCII
  spFontAddBorder(font,65535);
	*/
}

void engineDraw()
{
	if( gameStack->IsEmpty() )
		return;
  spResetZBuffer();
	gameStack->Current()->Render();
	spFlip();
}

int engineUpdate(Uint32 steps)
{
	int msPerFrame = (int)(1000.0 / FRAMES_PER_SECOND);
	if( gameStack->IsEmpty() || shutDown )
		return 1;
	deltaTime += steps;
	for( Uint32 i = 0; i < deltaTime / msPerFrame; i++ )
	{
		gameStack->Current()->Update();
	}
	deltaTime = deltaTime % msPerFrame;
	return 0;
}

void engineSDLEvent( SDL_Event *e )
{
	if( gameStack->IsEmpty() )
		return;
  if( e->type == SDL_QUIT )
  {
    shutDown = 1;
    return;
  }
	gameStack->Current()->Event( e );
}
