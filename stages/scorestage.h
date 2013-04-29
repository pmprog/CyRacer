
#ifndef _scorestage_h
#define _scorestage_h

#include "..//stage.h"
#include "..//globals.h"

class ScoreStage : Stage
{
  private:
		SDL_Surface *settingsImg;
    spFontPointer fontNormHnd;
		spFontPointer fontShdwHnd;
    spFontPointer fontNormTHnd;
		spFontPointer fontShdwTHnd;
    spFontPointer fontNorm2Hnd;
		spFontPointer fontShdw2Hnd;

  public:
    ScoreStage(void);
    ~ScoreStage(void);

    // Stage control
    void Begin();
    void Pause();
    void Resume();
    void Finish();

    void Event(SDL_Event *e);
		void DialogueCallback(void* data) {};

    void Update();
    void Render();
};

#endif