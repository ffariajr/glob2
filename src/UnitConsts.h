/*
  Copyright (C) 2001-2004 Stephane Magnenat & Luc-Olivier de Charri�re
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

#ifndef __UNIT_CONSTS_H
#define __UNIT_CONSTS_H

enum Abilities
{
	STOP_WALK=0,
	STOP_SWIM=1,
	STOP_FLY=2,
	
	WALK=3,
	SWIM=4,
	FLY=5,
	BUILD=6,
	HARVEST=7,
	ATTACK_SPEED=8,
	
	ATTACK_STRENGTH=9,
	ARMOR=10,
	HP=11,
	
	HEAL=12,
	FEED=13
};
const int NB_MOVE=9;
const int NB_ABILITY=12;


const int WORKER=0;
const int EXPLORER=1;
const int WARRIOR=2;
const int NB_UNIT_TYPE=3;

const int NB_UNIT_LEVELS=4;

#endif
 