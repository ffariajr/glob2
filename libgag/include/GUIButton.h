/*
  Copyright (C) 2001-2004 Stephane Magnenat & Luc-Olivier de Charrière
  for any question or comment contact us at nct@ysagoon.com or nuage@ysagoon.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef __GUIBUTTON_H
#define __GUIBUTTON_H

#include "GUIBase.h"
#include <string>

namespace GAGCore
{
	class Sprite;
	class Font;
}

namespace GAGGUI
{
	class Button: public HighlightableWidget
	{
	protected:
		Uint16 unicodeShortcut;
		Sint32 standardId;
		Sint32 highlightID;
		std::string sprite;
	
		//! cache, recomputed on internalInit
		GAGCore::Sprite *archPtr;
	
	public:
		Button() { unicodeShortcut=0; highlighted=false; standardId=-1; highlightID=-1; archPtr=NULL; }
		Button(int x, int y, int w, int h, Uint32 hAlign, Uint32 vAlign, const char *sprite, int standardId, int highlightID, int returnCode, Uint16 unicodeShortcut=0);
		virtual ~Button() { }
	
		virtual void onSDLEvent(SDL_Event *event);
		virtual void init(void);
		virtual void paint(GAGCore::DrawableSurface *gfx);
	};
	
	class TextButton:public Button
	{
	protected:
		std::string text;
		std::string font;
	
		// cache, recomputed on internalInit
		GAGCore::Font *fontPtr;
	
	public:
		TextButton() { fontPtr=NULL; }
		TextButton(int x, int y, int w, int h, Uint32 hAlign, Uint32 vAlign, const char *sprite, int standardId, int highlightID, const char *font, const char *text, int retuxrnCode, Uint16 unicodeShortcut=0);
		virtual ~TextButton() { }
		virtual void init(void);
		virtual void paint(GAGCore::DrawableSurface *gfx);
	
		void setText(const char *text);
	};
	
	class OnOffButton:public HighlightableWidget
	{
	protected:
		bool state;
	
	public:
		OnOffButton() { state=false; returnCode=0; highlighted=false; }
		OnOffButton(int x, int y, int w, int h, Uint32 hAlign, Uint32 vAlign, bool startState, int returnCode);
		virtual ~OnOffButton() { }
	
		virtual void onSDLEvent(SDL_Event *event);
		virtual void paint(GAGCore::DrawableSurface *gfx);
		virtual bool getState(void) { return state; }
		virtual void setState(bool newState);
	};
	
	//! A button that can have multiple color
	class ColorButton:public HighlightableWidget
	{
	protected:
		class Color
		{
		public:
			Color() { r=g=b=0; }
			Color(int r, int g, int b) { this->r=r; this->g=g; this->b=b; }
		public:
			Sint32 r;
			Sint32 g;
			Sint32 b;
		};
	
		Sint32 selColor;
		std::vector<Color> v;
	
	public:
		ColorButton() { selColor=returnCode=0; }
		//! ColorButton constructor
		ColorButton(int x, int y, int w, int h, Uint32 hAlign, Uint32 vAlign, int returnCode);
		//! ColorButton destructor
		virtual ~ColorButton() { }
	
		//! Process SDL event
		virtual void onSDLEvent(SDL_Event *event);
		virtual void paint(GAGCore::DrawableSurface *gfx);
		//! Add a color to the color list
		virtual void addColor(int r, int g, int b) { v.push_back(Color(r, g, b)); }
		//! Clear the color list
		virtual void clearColors(void) { v.clear(); }
		//! Set the color selection to default
		virtual void setSelectedColor(int c=0) { selColor=c; }
		//! Return the color sel
		virtual int getSelectedColor(void) { return selColor; }
		//! Return the number of possible colors
		virtual size_t getNumberOfColors(void) { return v.size(); }
	};
	
	//! A button that can have multiple texts
	class MultiTextButton:public TextButton
	{
	protected:
		std::vector<const char *> texts;
		unsigned textIndex;
	
	public:
		MultiTextButton() { textIndex=0; returnCode=0; }
		MultiTextButton(int x, int y, int w, int h, Uint32 hAlign, Uint32 vAlign, const char *sprite, int standardId, int highlightID, const char *font, const char *text, int retuxrnCode, Uint16 unicodeShortcut=0);
		virtual ~MultiTextButton() { }
	
		virtual void onSDLEvent(SDL_Event *event);
		
		virtual void addText(const char *s);
		virtual void clearTexts(void);
		virtual void setTextIndex(int i);
		virtual void setFirstTextIndex(int i);
		virtual int getTextIndex(void) { return textIndex; }
		virtual size_t getTextsSize(void) { return texts.size(); }
	};
}

#endif
