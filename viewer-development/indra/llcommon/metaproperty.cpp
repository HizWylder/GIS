/** 
 * @file metaproperty.cpp
 * @author Babbage
 * @date 2006-05-15
 * @brief Implementation of LLMetaProperty.
 *
 * $LicenseInfo:firstyear=2006&license=viewerlgpl$
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

#include "linden_common.h" 

#include "metaproperty.h"

#include "metaclass.h"

LLMetaProperty::LLMetaProperty(const std::string& name, const LLMetaClass& object_class) : 
	mName(name), mObjectClass(object_class) 
{
}

//virtual 
LLMetaProperty::~LLMetaProperty()
{
}

const LLMetaClass& LLMetaProperty::getObjectMetaClass() const
{
	return mObjectClass;
}

void LLMetaProperty::checkObjectClass(const LLReflective* object) const
{
	if(! mObjectClass.isInstance(object))
	{
		throw "class cast exception";
	}
}
