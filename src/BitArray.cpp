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

#include "BitArray.h"
#include <assert.h>

namespace Utilities
{
	BitArray::BitArray(size_t size, bool defaultValue)
	{
		resize(size, defaultValue);
	}
	
	void BitArray::resize(size_t size, bool defaultValue)
	{
		if (defaultValue)
			values.resize(bitToByte(size), 1);
		else
			values.resize(bitToByte(size), 0);
	}
	
	size_t BitArray::bitToByte(size_t v)
	{
		if (v&0x7)
			return (v>>3)+1;
		else
			return v>>3;
	}
	
	void BitArray::set(size_t pos, bool value)
	{
		size_t wordPos = pos / 8;
		size_t bitPos = pos % 8;
		
		if (value)
			values[wordPos] |= (1<<bitPos);
		else
			values[wordPos] &= ~(1<<bitPos);
	}
	
	bool BitArray::get(size_t pos)
	{
		size_t wordPos = pos / 8;
		size_t bitPos = pos % 8;

		assert(wordPos < values.size());

		return (values[wordPos] & (1<<bitPos)) != 0;
	}
	
	void BitArray::serialize(unsigned char *stream)
	{
		size_t l = values.size();
		std::copy(&values[0], &values[l], stream);
	}
	
	void BitArray::deserialize(const unsigned char *stream, size_t size)
	{
		values.resize(bitToByte(size));
		std::copy(stream, stream+values.size(), &values[0]);
	}
}