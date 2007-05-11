/*
  Copyright (C) 2007 Bradley Arsenault

  Copyright (C) 2001-2004 Stephane Magnenat & Luc-Olivier de Charrière
  for any question or comment contact us at <stephane at magnenat dot net> or <NuageBleu at gmail dot com>

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

#ifndef __YOGSCREEN_H
#define __YOGSCREEN_H

#include "MultiplayersJoin.h"
#include <vector>
#include "YOG.h"
#include "IRC.h"
#include <GUIList.h>
#include <GraphicContext.h>
#include "Glob2Screen.h"

namespace GAGGUI
{
	class TextInput;
	class TextArea;
	class TextButton;
}

/// A widget that maintains the list of players, and draws an icon based
/// on whether that player is from YOG or from IRC
class YOGPlayerList : public List
{
public:

	/// Constructor
	YOGPlayerList(int x, int y, int w, int h, Uint32 hAlign, Uint32 vAlign, const std::string &font);

	/// Destructor, release sprites
	virtual ~YOGPlayerList();
	
	/// Represents the type of network a player can be in
	enum NetworkType
	{
		ALL_NETWORK = 0,
		YOG_NETWORK,
		IRC_NETWORK,
	};

	/// Add a new player with its network;
	void addPlayer(const std::string &nick, NetworkType network);

	///Clears the lists of players
	void clear(void) { List::clear(); networks.clear(); }

private:
	//! An array that contains for each player the related network
	std::vector<NetworkType> networks;
	//! sprite for networks
	GAGCore::Sprite *networkSprite;

	///Draws an item on the screen
	virtual void drawItem(int x, int y, size_t element);
};

///This is the main YOG screen
class YOGScreen : public Glob2Screen
{
public:
	///This takes a YOGClient. The client must be logged in when this is called.
	YOGScreen(boost::shared_ptr<YOGClient> client);

	virtual ~YOGScreen();
	virtual void onTimer(Uint32 tick);
	void onAction(Widget *source, Action action, int par1, int par2);

	enum
	{
		CANCEL=2,
		CREATE_GAME=3,
		UPDATE_LIST=4,
		JOIN=7,
		
		STARTED=11
	};

private:
	List *gameList;
	TextArea *gameInfo;
	YOGPlayerList *playerList;
	TextInput *textInput;
	TextArea *chatWindow;
	
	TextButton *joinButton;
	
	IRC irc;

	void updateGameList(void);
	void updatePlayerList(void);

	int executionMode;
	boost::shared_ptr<YOGClient> client;

};

#endif
