#pragma once

#include "..//stage.h"
#include "..//globals.h"

class Splash3Stage : Stage
{
  private:
    SDL_Surface *sparrowLogo;
    spFontPointer fontHnd;
    int StageCount;
    void LeaveStage();

  public:
    Splash3Stage(void);
    ~Splash3Stage(void);

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
