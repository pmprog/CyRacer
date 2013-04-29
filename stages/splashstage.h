#pragma once

#include "..//stage.h"
#include "..//globals.h"

class SplashStage : Stage
{
  private:
    SDL_Surface *pmprogLogo;
    int StageCount;
    void LeaveStage();

  public:
    SplashStage(void);
    ~SplashStage(void);

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
