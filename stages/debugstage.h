#pragma once

#include <vector>
#include "..//stage.h"
#include "..//globals.h"


class DebugStage : Stage
{
  public:
		SDL_Surface* carTexture;
		spModelPointer carMesh;
		SDL_Surface* roadTexture;
		SDL_Surface* offroadTexture;
		SDL_Surface* roadsideTexture;
		Sint32 speed;
		Sint32 rotation;
		bool accDown;
		bool decDown;
		int dirDown;

		int gameNum;

		int carPos;
		int carGfxAt;
		int carGfxTo;
		int crashed;
		int crashSpeed;
		int crashTick;
		int crashHiScore;

		int LeftWall[64];
		int RightWall[64];
		int Cat[64];

		double score;
		spFontPointer fontHnd;
		spFontPointer fontHiHnd;
		spFontPointer fontHiSHnd;

		int lastRot;
		int lastRow;

    DebugStage(unsigned int gameNumber);
    ~DebugStage(void);

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
