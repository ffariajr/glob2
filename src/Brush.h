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

#ifndef __BRUSH_H
#define __BRUSH_H

#include <vector>

//! A click of the brush tool to the map
struct BrushApplication
{
	BrushApplication(int x, int y, int figure) { this->x=x; this->y=y; this->figure=figure; }
	int x;
	int y;
	int figure;
};

//! A brush tool is the GUI and the settings container for a brush's operations
class BrushTool
{
public:
	enum Mode
	{
		MODE_NONE = 0,
		MODE_ADD,
		MODE_DEL
	};
	
protected:
	unsigned figure;
	Mode mode;
	
public:
	BrushTool();
	void draw(int x, int y);
	void handleClick(int x, int y);
	void unselect(void) { mode = MODE_NONE; }
	void drawBrush(int x, int y);
	unsigned getType(void) { return static_cast<unsigned>(mode); }
	unsigned getFigure(void) { return figure; }
	
	//! Return the full width of a brush
	static int getBrushWidth(unsigned figure);
	//! Return the full height of a brush
	static int getBrushHeight(unsigned figure);
	//! Return the half width minus 1 of a brush (its "ray" in x)
	static int getBrushDimX(unsigned figure);
	//! Return the half height minus 1 of a brush (its "ray" in y)
	static int getBrushDimY(unsigned figure);
	//! Return the value of a pixel of a given brush
	static bool getBrushValue(unsigned figure, int x, int y);
};

namespace Utilities
{
	class BitArray;
}

class BrushAccumulator
{
public:
	struct AreaDimensions
	{
		unsigned minX, minY, maxX, maxY;
	};
	
	std::vector<BrushApplication> applications;

	bool getBitmap(Utilities::BitArray *array, AreaDimensions *dim);
};

#endif