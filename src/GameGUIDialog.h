/*
 * Globulation 2 game dialog
 * (c) 2001 Stephane Magnenat, Luc-Olivier de Charriere, Ysagoon
 */

#ifndef __GAME_GUI_DIALOG_H
#define __GAME_GUI_DIALOG_H

#include "GAG.h"
#include "Player.h"

class InGameScreen:public Screen
{
public:
	InGameScreen(int w, int h);
	virtual ~InGameScreen();

	virtual void translateAndProcessEvent(SDL_Event *event);
	virtual void paint(int x, int y, int w, int h);

public:
	int endValue;
	int decX, decY;
};

class InGameSaveScreen:public InGameScreen
{
public:
	char *filename;

public:
	InGameSaveScreen();
	virtual ~InGameSaveScreen() { }
	virtual void onAction(Widget *source, Action action, int par1, int par2);
	virtual void onSDLEvent(SDL_Event *event);
	//virtual void paint(int x, int y, int w, int h);
};

class InGameMainScreen:public InGameScreen
{
public:
	InGameMainScreen();
	virtual ~InGameMainScreen() { }
	virtual void onAction(Widget *source, Action action, int par1, int par2);
	virtual void onSDLEvent(SDL_Event *event);
	//virtual void paint(int x, int y, int w, int h);
};

class GameGUI;

class InGameAlliance8Screen:public InGameScreen
{
public:
	OnOffButton *allied[8];
	OnOffButton *vision[8];
	OnOffButton *chat[8];
	char names[8][BasePlayer::MAX_NAME_LENGTH];
	bool firstPaint;
	GameGUI *gameGUI;

public:
	InGameAlliance8Screen(GameGUI *gameGUI);
	virtual ~InGameAlliance8Screen() { }
	virtual void onAction(Widget *source, Action action, int par1, int par2);
	virtual void onSDLEvent(SDL_Event *event);
	virtual void paint(int x, int y, int w, int h);
	Uint32 getAllianceMask(void);
	Uint32 getVisionMask(void);
	Uint32 getChatMask(void);

protected:
	void setCorrectValueForPlayer(int i);
};



#endif
