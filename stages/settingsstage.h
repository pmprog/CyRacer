
#ifndef _settingsstage_h
#define _settingsstage_h

#include "..//stage.h"
#include "..//globals.h"

class SettingsStage : Stage
{
  private:
		SDL_Surface *settingsImg;
		SDL_Surface *keyReqImg;
    spFontPointer fontNormHnd;
		spFontPointer fontSelHnd;
		spFontPointer fontShdwHnd;
    int selectedItem;

		int requestingKey;

  public:
    SettingsStage(void);
    ~SettingsStage(void);

    // Stage control
    void Begin();
    void Pause();
    void Resume();
    void Finish();

    void Event(SDL_Event *e);
		void DialogueCallback(void* data) {};

    void Update();
    void Render();
		void RenderMenuText(int optionNumber, char* Text);
};

#endif