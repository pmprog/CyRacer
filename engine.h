
#define _core
#include "globals.h"

int deltaTime;

void resizeWindow( Uint16 w, Uint16 h );
void engineDraw();
int engineUpdate(Uint32 steps);
void engineSDLEvent( SDL_Event *e );
