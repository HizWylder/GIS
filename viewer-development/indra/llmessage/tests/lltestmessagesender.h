/** 
 * @file 
 * @brief 
 *
 * $LicenseInfo:firstyear=2008&license=viewerlgpl$
 * GIS Viewer Source Code
 * Copyright (C) 2010, Linden Research, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

/* Macro Definitions */
#ifndef LL_LLTESTMESSAGESENDER_H
#define LL_LLTESTMESSAGESENDER_H


#include "linden_common.h"
#include "llmessagesenderinterface.h"
#include <vector>



class LLTestMessageSender : public LLMessageSenderInterface
{
public:
	virtual ~LLTestMessageSender();
	virtual S32 sendMessage(const LLHost& host, LLStoredMessagePtr message);

	std::vector<LLHost> mSendHosts;
	std::vector<LLStoredMessagePtr> mSendMessages;
};



#endif //LL_LLTESTMESSAGESENDER_H
