/** 
 * @file llclassifiedinfo.cpp
 * @brief LLClassifiedInfo class definition
 *
 * $LicenseInfo:firstyear=2005&license=viewerlgpl$
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

#include "llviewerprecompiledheaders.h"
#include "llclassifiedinfo.h"

#include "lluuid.h"

LLClassifiedInfo::cat_map LLClassifiedInfo::sCategories;

// static
void LLClassifiedInfo::loadCategories(const LLSD& options)
{
	for(LLSD::array_const_iterator resp_it = options.beginArray(),
		end = options.endArray(); resp_it != end; ++resp_it)
	{
		LLSD name = (*resp_it)["category_name"];
		if(name.isDefined())
		{
			LLSD id = (*resp_it)["category_id"];
			if(id.isDefined())
			{
				LLClassifiedInfo::sCategories[id.asInteger()] = name.asString();
			}
		}
	}
}
