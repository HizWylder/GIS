/** 
 * @file llhudmanager.h
 * @brief LLHUDManager class definition
 *
 * $LicenseInfo:firstyear=2002&license=viewerlgpl$
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

#ifndef LL_LLHUDMANAGER_H
#define LL_LLHUDMANAGER_H

// Responsible for managing all HUD elements.

#include "llhudobject.h"
#include "lldarrayptr.h"

class LLViewerObject;
class LLHUDEffect;
//Ventrella 9/16/05
class LLHUDAnimalControls;
// End Ventrella
class LLMessageSystem;

class LLHUDManager : public LLSingleton<LLHUDManager>
{
public:
	LLHUDManager();
	~LLHUDManager();

	LLHUDEffect *createViewerEffect(const U8 type, BOOL send_to_sim = TRUE, BOOL originated_here = TRUE);

	void updateEffects();
	void sendEffects();
	void cleanupEffects();

	static void shutdownClass();

	static void processViewerEffect(LLMessageSystem *mesgsys, void **user_data);

	static LLColor4 sParentColor;
	static LLColor4 sChildColor;

protected:
	LLDynamicArrayPtr<LLPointer<LLHUDEffect>				> mHUDEffects;
};

#endif // LL_LLHUDMANAGER_H
