#pragma once

#include "..//stage.h"
#include "..//globals.h"

class BootStage : Stage
{
  public:
    BootStage(void);
    ~BootStage(void);

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
