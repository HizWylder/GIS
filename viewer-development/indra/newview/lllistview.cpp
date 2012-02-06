/** 
 * @file lllistview.cpp
 * @brief UI widget containing a scrollable, possibly hierarchical list of 
 * folders (LLListViewFolder) and items (LLListViewItem).
 *
 * $LicenseInfo:firstyear=2009&license=viewerlgpl$
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

#include "lllistview.h"

#include "lltextbox.h"
#include "lluictrlfactory.h"	// LLDefaultChildRegistry

// linker optimizes this out on Windows until there is a real reference
// to this file
static LLDefaultChildRegistry::Register<LLListView> r("list_view");

LLListView::Params::Params()
:	bg_color("bg_color"),
	fg_selected_color("fg_selected_color"),
	bg_selected_color("bg_selected_color")
{}

LLListView::LLListView(const Params& p)
:	LLUICtrl(p),
	mLabel(NULL),
	mBgColor(p.bg_color()),
	mFgSelectedColor(p.fg_selected_color()),
	mBgSelectedColor(p.bg_selected_color())
{
	LLRect label_rect(0, 20, 300, 0);
	LLTextBox::Params text_box_params;
	text_box_params.rect(label_rect);
	text_box_params.initial_value("This is a list-view");
	mLabel = LLUICtrlFactory::create<LLTextBox>(text_box_params);
	addChild(mLabel);
}

LLListView::~LLListView()
{}


// placeholder for setting a property
void LLListView::setString(const std::string& s)
{
	mLabel->setValue( LLSD(s) );
}