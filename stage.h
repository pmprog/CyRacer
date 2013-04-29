
#ifndef _stage
#define _stage

#include <sparrow3d.h>

class Stage
{
  public:
    // Stage control
    virtual void Begin() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
    virtual void Finish() = 0;

    virtual void Event(SDL_Event *e) = 0;
		virtual void DialogueCallback(void* data) {};

    virtual void Update() = 0;
    virtual void Render() = 0;
};

#endif