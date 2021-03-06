/*
  Copyright (C) 2007 Bradley Arsenault

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/


#ifndef __YOGClientChatListener_h
#define __YOGClientChatListener_h

#include "boost/shared_ptr.hpp"

class YOGMessage;

///This class is a mix-in class for objects that want to listen for recieved texts
class YOGClientChatListener
{
public:
	virtual ~YOGClientChatListener() {}

	///Recieves a text message
	virtual void recieveTextMessage(boost::shared_ptr<YOGMessage> message)=0;
};

#endif
