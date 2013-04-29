
#ifndef _title_h
#define _title_h

#include "..//stage.h"
#include "..//globals.h"
#include <math.h>


class TitleStage : Stage
{
  private:
    SDL_Surface *titleImg;
		SDL_Surface *keyReqImg;
    spFontPointer fontNormHnd;
		spFontPointer fontSelHnd;
		spFontPointer fontShdwHnd;
    int selectedItem;
		int gameNumber;
		HSTREAM titleMusic;

		int showNumberSelector;
		int numSelect10s;

  public:
    TitleStage(void);
    ~TitleStage(void);

    // Stage control
    void Begin();
    void Pause();
    void Resume();
    void Finish();

    void Event(SDL_Event *e);
		void NumSelEvent(SDL_Event *e);
		void DialogueCallback(void* data) {};

    void Update();
    void Render();
};


#endif