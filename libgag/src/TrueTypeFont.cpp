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

#include "TrueTypeFont.h"
#include <Toolkit.h>
#include <SupportFunctions.h>
#include <FileManager.h>
#include <assert.h>
#include <SDL_image.h>
#include <iostream>

TrueTypeFont::TrueTypeFont()
{
	font = NULL;
}

TrueTypeFont::TrueTypeFont(const char *filename, unsigned size)
{
	load(filename, size);
}

TrueTypeFont::~TrueTypeFont()
{
	if (font)
	{
		for (std::map<CacheEntry, SDL_Surface *>::iterator it = cache.begin(); it != cache.end(); ++it)
			SDL_FreeSurface(it->second);
		TTF_CloseFont(font);
	}
}

bool TrueTypeFont::load(const char *filename, unsigned size)
{
	SDL_RWops *fontStream = Toolkit::getFileManager()->open(filename, "rb");
	if (fontStream)
	{
		font = TTF_OpenFontRW(fontStream, 1, size);
		if (font)
		{
			setStyle(Style(STYLE_NORMAL, 255, 255, 255));
			return true;
		}
	}
	return false;
}

int TrueTypeFont::getStringWidth(const char *string) const
{
	int w, h;
	TTF_SizeUTF8(font, string, &w, &h);
	return w;
}

int TrueTypeFont::getStringHeight(const char *string) const
{
	if (string)
	{
		int w, h;
		TTF_SizeUTF8(font, string, &w, &h);
		return h;
	}
	else
	{
		return TTF_FontHeight(font);
	}
}

void TrueTypeFont::setStyle(Style style)
{
	assert(font);
	
	while (styleStack.size() > 0)
		styleStack.pop();
	pushStyle(style);
}

void TrueTypeFont::pushStyle(Style style)
{
	assert(font);
	
	styleStack.push(style);
	TTF_SetFontStyle(font, style.shape);
}

void TrueTypeFont::popStyle(void)
{
	assert(font);
	
	if (styleStack.size() > 1)
	{
		styleStack.pop();
		TTF_SetFontStyle(font, styleStack.top().shape);
	}
}

Font::Style TrueTypeFont::getStyle(void) const
{
	assert(font);
	
	return styleStack.top();
}

void TrueTypeFont::drawString(SDL_Surface *Surface, int x, int y, int w, const char *text, SDL_Rect *clip)
{
	assert(text);
	assert(font);
	assert(styleStack.size()>0);
	
	CacheEntry entry;
	entry.text = text;
	entry.style = styleStack.top();
	
	SDL_Surface *s;
	
	if (cache.find(entry) == cache.end())
	{
		// create bitmap
		SDL_Color c;
		c.r = styleStack.top().r;
		c.g = styleStack.top().g;
		c.b = styleStack.top().b;
		c.unused = styleStack.top().a;
		
		s = TTF_RenderUTF8_Blended(font, text, c);
		if (s == NULL)
			return;
			
		// store in cache
		cache[entry] = s;
		std::cout << "String cache size for " << this << " is now " << cache.size() << std::endl;
	}
	else
	{
		s = cache[entry];
	}
	
	// render
	SDL_Rect sr;
	sr.x = 0;
	sr.y = 0;
	sr.w = static_cast<Uint16>(s->w);
	sr.h = static_cast<Uint16>(s->h);

	SDL_Rect r;
	r.x = static_cast<Sint16>(x);
	r.y = static_cast<Sint16>(y);
	if (w)
		r.w = static_cast<Uint16>(w);
	else
		r.w = static_cast<Uint16>(s->w);
	r.h = static_cast<Uint16>(s->h);

	SDL_Rect oc;
	if (clip)
	{
		SDL_GetClipRect(Surface, &oc);
		GAG::sdcRects(&sr, &r, *clip);
		SDL_SetClipRect(Surface, &r);
	}

	SDL_BlitSurface(s, &sr, Surface, &r);

	if (clip)
	{
		SDL_SetClipRect(Surface, &oc);
	}
}