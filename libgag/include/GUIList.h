/*
  Copyright (C) 2001, 2002 Stephane Magnenat & Luc-Olivier de Charri�e
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

#ifndef __GUILIST_H
#define __GUILIST_H

#include "GUIBase.h"
#include <vector>

class List: public RectangularWidget
{
protected:
	CLASSDEF(List)
		BASECLASS(RectangularWidget)
	MEMBERS
		ITEM(std::string, font)
		ITEM(std::vector<std::string>, strings)
		ITEM(Sint32, nth)
		ITEM(Uint32, disp)
	CLASSEND;

	//! Cached variables, do not serialise, reconstructed on paint() call
	//! Length of the scroll box, this is a cache
	unsigned blockLength, blockPos, textHeight;
	Font *fontPtr;

public:
	List():RectangularWidget() { fontPtr = NULL; }
	List(int x, int y, int w, int h, Uint32 hAlign, Uint32 vAlign, const char *font);
	virtual ~List();

	virtual void onTimer(Uint32 tick) { }
	virtual void onSDLEvent(SDL_Event *event);
	virtual void paint(void);

	void addText(const char *text, int pos);
	void addText(const char *text);
	void removeText(int pos);
	void clear(void);
	const char *getText(int pos) const;
	const char *get(void) const;
	//! Call this after all add has been done
	void commit(void) { repaint(); }
	//! Sorts the list (override it if you don't like it)
	virtual void sort(void);

	//! Called when selection changes (default: signal parent)
	virtual void selectionChanged();

	int getNth(void) const;
	void setNth(int nth);

	virtual void repaint(void);
	virtual void internalPaint(void);
};

#endif

