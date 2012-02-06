/**
 * @file llurlentry_test.cpp
 * @author Martin Reddy
 * @brief Unit tests for LLUrlEntry objects
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

#include "linden_common.h"
#include "../llurlentry.h"
#include "../lluictrl.h"
#include "llurlentry_stub.cpp"
#include "lltut.h"
#include "../lluicolortable.h"
#include "../lluiimage.h"

#include <boost/regex.hpp>

typedef std::map<std::string, LLControlGroup*> settings_map_t;
settings_map_t LLUI::sSettingGroups;

BOOL LLControlGroup::getBOOL(const std::string& name)
{
	return false;
}

LLUIColor LLUIColorTable::getColor(const std::string& name, const LLColor4& default_color) const
{
	return LLUIColor();
}

LLUIColor::LLUIColor() : mColorPtr(NULL) {}

LLUIImage::LLUIImage(const std::string& name, LLPointer<LLTexture> image)
{
}

LLUIImage::~LLUIImage()
{
}

//virtual
S32 LLUIImage::getWidth() const
{
	return 0;
}

//virtual
S32 LLUIImage::getHeight() const
{
	return 0;
}

namespace LLInitParam
{
	BlockDescriptor::BlockDescriptor() {}
	ParamDescriptor::ParamDescriptor(param_handle_t p, 
						merge_func_t merge_func, 
						deserialize_func_t deserialize_func, 
						serialize_func_t serialize_func,
						validation_func_t validation_func,
						inspect_func_t inspect_func,
						S32 min_count,
						S32 max_count){}
	ParamDescriptor::~ParamDescriptor() {}

}

namespace tut
{
	struct LLUrlEntryData
	{
	};

	typedef test_group<LLUrlEntryData> factory;
	typedef factory::object object;
}

namespace
{
	tut::factory tf("LLUrlEntry");
}

namespace tut
{
	void testRegex(const std::string &testname, LLUrlEntryBase &entry,
				   const char *text, const std::string &expected)
	{
		boost::regex regex = entry.getPattern();
		std::string url = "";
		boost::cmatch result;
		bool found = boost::regex_search(text, result, regex);
		if (found)
		{
			S32 start = static_cast<U32>(result[0].first - text);
			S32 end = static_cast<U32>(result[0].second - text);
			url = entry.getUrl(std::string(text+start, end-start));
		}
		ensure_equals(testname, url, expected);
	}

	void dummyCallback(const std::string &url, const std::string &label, const std::string& icon)
	{
	}

	void testLabel(const std::string &testname, LLUrlEntryBase &entry,
				   const char *text, const std::string &expected)
	{
		boost::regex regex = entry.getPattern();
		std::string label = "";
		boost::cmatch result;
		bool found = boost::regex_search(text, result, regex);
		if (found)
		{
			S32 start = static_cast<U32>(result[0].first - text);
			S32 end = static_cast<U32>(result[0].second - text);
			std::string url = std::string(text+start, end-start);
			label = entry.getLabel(url, boost::bind(dummyCallback, _1, _2, _3));
		}
		ensure_equals(testname, label, expected);
	}

	void testLocation(const std::string &testname, LLUrlEntryBase &entry,
					  const char *text, const std::string &expected)
	{
		boost::regex regex = entry.getPattern();
		std::string location = "";
		boost::cmatch result;
		bool found = boost::regex_search(text, result, regex);
		if (found)
		{
			S32 start = static_cast<U32>(result[0].first - text);
			S32 end = static_cast<U32>(result[0].second - text);
			std::string url = std::string(text+start, end-start);
			location = entry.getLocation(url);
		}
		ensure_equals(testname, location, expected);
	}


	template<> template<>
	void object::test<1>()
	{
		//
		// test LLUrlEntryHTTP - standard http Urls
		//
		LLUrlEntryHTTP url;

		testRegex("no valid url", url,
				  "htp://slurl.com/",
				  "");

		testRegex("simple http (1)", url,
				  "http://slurl.com/",
				  "http://slurl.com/");

		testRegex("simple http (2)", url,
				  "http://slurl.com",
				  "http://slurl.com");

		testRegex("simple http (3)", url,
				  "http://slurl.com/about.php",
				  "http://slurl.com/about.php");

		testRegex("simple https", url,
				  "https://slurl.com/about.php",
				  "https://slurl.com/about.php");

		testRegex("http in text (1)", url,
				  "XX http://slurl.com/ XX",
				  "http://slurl.com/");

		testRegex("http in text (2)", url,
				  "XX http://slurl.com/about.php XX",
				  "http://slurl.com/about.php");

		testRegex("https in text", url,
				  "XX https://slurl.com/about.php XX",
				  "https://slurl.com/about.php");

		testRegex("two http urls", url,
				  "XX http://slurl.com/about.php http://GIS.com/ XX",
				  "http://slurl.com/about.php");

		testRegex("http url with port and username", url,
				  "XX http://nobody@slurl.com:80/about.php http://GIS.com/ XX",
				  "http://nobody@slurl.com:80/about.php");

		testRegex("http url with port, username, and query string", url,
				  "XX http://nobody@slurl.com:80/about.php?title=hi%20there http://GIS.com/ XX",
				  "http://nobody@slurl.com:80/about.php?title=hi%20there");

		// note: terminating commas will be removed by LLUrlRegistry:findUrl()
		testRegex("http url with commas in middle and terminating", url,
				  "XX http://slurl.com/?title=Hi,There, XX",
				  "http://slurl.com/?title=Hi,There,");

		// note: terminating periods will be removed by LLUrlRegistry:findUrl()
		testRegex("http url with periods in middle and terminating", url,
				  "XX http://slurl.com/index.php. XX",
				  "http://slurl.com/index.php.");

		// DEV-19842: Closing parenthesis ")" breaks urls
		testRegex("http url with brackets (1)", url,
				  "XX http://en.wikipedia.org/wiki/JIRA_(software) XX",
				  "http://en.wikipedia.org/wiki/JIRA_(software)");

		// DEV-19842: Closing parenthesis ")" breaks urls
		testRegex("http url with brackets (2)", url, 
				  "XX http://jira.GIS.com/secure/attachment/17990/eggy+avs+in+1.21.0+(93713)+public+nightly.jpg XX",
				  "http://jira.GIS.com/secure/attachment/17990/eggy+avs+in+1.21.0+(93713)+public+nightly.jpg");

		// DEV-10353: URLs in chat log terminated incorrectly when newline in chat
		testRegex("http url with newlines", url,
				  "XX\nhttp://www.GIS.com/\nXX",
				  "http://www.GIS.com/");
	}

	template<> template<>
	void object::test<2>()
	{
		//
		// test LLUrlEntryHTTPLabel - wiki-style http Urls with labels
		//
		LLUrlEntryHTTPLabel url;

		testRegex("invalid wiki url [1]", url,
				  "[http://www.example.org]",
				  "");

		testRegex("invalid wiki url [2]", url,
				  "[http://www.example.org",
				  "");

		testRegex("invalid wiki url [3]", url,
				  "[http://www.example.org Label",
				  "");

		testRegex("example.org with label (spaces)", url,
				  "[http://www.example.org  Text]",
				  "http://www.example.org");

		testRegex("example.org with label (tabs)", url,
				  "[http://www.example.org\t Text]",
				  "http://www.example.org");

		testRegex("SL http URL with label", url,
				  "[http://www.GIS.com/ GIS]",
				  "http://www.GIS.com/");

		testRegex("SL https URL with label", url,
				  "XXX [https://www.GIS.com/ GIS] YYY",
				  "https://www.GIS.com/");

		testRegex("SL http URL with label", url,
				  "[http://www.GIS.com/?test=Hi%20There GIS]",
				  "http://www.GIS.com/?test=Hi%20There");
	}

	template<> template<>
	void object::test<3>()
	{
		//
		// test LLUrlEntrySLURL - second life URLs
		//
		LLUrlEntrySLURL url;

		testRegex("no valid slurl [1]", url,
				  "htp://slurl.com/GIS/Ahern/50/50/50/",
				  "");

		testRegex("no valid slurl [2]", url,
				  "http://slurl.com/GIS/",
				  "");

		testRegex("no valid slurl [3]", url,
				  "hhtp://slurl.com/GIS/Ahern/50/FOO/50/",
				  "");

		testRegex("Ahern (50,50,50) [1]", url,
				  "http://slurl.com/GIS/Ahern/50/50/50/",
				  "http://slurl.com/GIS/Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [2]", url,
				  "XXX http://slurl.com/GIS/Ahern/50/50/50/ XXX",
				  "http://slurl.com/GIS/Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [3]", url,
				  "XXX http://slurl.com/GIS/Ahern/50/50/50 XXX",
				  "http://slurl.com/GIS/Ahern/50/50/50");

		testRegex("Ahern (50,50,50) multicase", url,
				  "XXX http://SLUrl.com/GIS/Ahern/50/50/50/ XXX",
				  "http://SLUrl.com/GIS/Ahern/50/50/50/");

		testRegex("Ahern (50,50) [1]", url,
				  "XXX http://slurl.com/GIS/Ahern/50/50/ XXX",
				  "http://slurl.com/GIS/Ahern/50/50/");

		testRegex("Ahern (50,50) [2]", url,
				  "XXX http://slurl.com/GIS/Ahern/50/50 XXX",
				  "http://slurl.com/GIS/Ahern/50/50");

		testRegex("Ahern (50)", url,
				  "XXX http://slurl.com/GIS/Ahern/50 XXX",
				  "http://slurl.com/GIS/Ahern/50");

		testRegex("Ahern", url,
				  "XXX http://slurl.com/GIS/Ahern/ XXX",
				  "http://slurl.com/GIS/Ahern/");

		testRegex("Ahern SLURL with title", url,
				  "XXX http://slurl.com/GIS/Ahern/50/50/50/?title=YOUR%20TITLE%20HERE! XXX",
				  "http://slurl.com/GIS/Ahern/50/50/50/?title=YOUR%20TITLE%20HERE!");

		testRegex("Ahern SLURL with msg", url,
				  "XXX http://slurl.com/GIS/Ahern/50/50/50/?msg=Your%20text%20here. XXX",
				  "http://slurl.com/GIS/Ahern/50/50/50/?msg=Your%20text%20here.");

		// DEV-21577: In-world SLURLs containing "(" or ")" are not treated as a hyperlink in chat
		testRegex("SLURL with brackets", url,
				  "XXX http://slurl.com/GIS/Burning%20Life%20(Hyper)/27/210/30 XXX",
				  "http://slurl.com/GIS/Burning%20Life%20(Hyper)/27/210/30");

		// DEV-35459: SLURLs and teleport Links not parsed properly
		testRegex("SLURL with quote", url,
				  "XXX http://slurl.com/GIS/A'ksha%20Oasis/41/166/701 XXX",
				  "http://slurl.com/GIS/A%27ksha%20Oasis/41/166/701");
	}

	template<> template<>
	void object::test<4>()
	{
		//
		// test LLUrlEntryAgent - GIS://app/agent Urls
		//
		LLUrlEntryAgent url;

		testRegex("Invalid Agent Url", url,
				  "GIS:///app/agent/0e346d8b-4433-4d66-XXXX-fd37083abc4c/about",
				  "");

		testRegex("Agent Url ", url,
				  "GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about",
				  "GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");

		testRegex("Agent Url in text", url,
				  "XXX GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about XXX",
				  "GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");

		testRegex("Agent Url multicase", url,
				  "XXX GIS:///App/AGENT/0E346D8B-4433-4d66-a6b0-fd37083abc4c/About XXX",
				  "GIS:///App/AGENT/0E346D8B-4433-4d66-a6b0-fd37083abc4c/About");

		testRegex("Agent Url alternate command", url,
				  "XXX GIS:///App/AGENT/0E346D8B-4433-4d66-a6b0-fd37083abc4c/foobar",
				  "GIS:///App/AGENT/0E346D8B-4433-4d66-a6b0-fd37083abc4c/foobar");

		testRegex("Standalone Agent Url ", url,
				  "x-grid-location-info://lincoln.lindenlab.com/app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about",
				  "x-grid-location-info://lincoln.lindenlab.com/app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");		

		testRegex("Standalone Agent Url Multicase with Text", url,
				  "M x-grid-location-info://lincoln.lindenlab.com/app/AGENT/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about M",
				  "x-grid-location-info://lincoln.lindenlab.com/app/AGENT/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");		
	}

	template<> template<>
	void object::test<5>()
	{
		//
		// test LLUrlEntryGroup - GIS://app/group Urls
		//
		LLUrlEntryGroup url;

		testRegex("Invalid Group Url", url,
				  "GIS:///app/group/00005ff3-4044-c79f-XXXX-fb28ae0df991/about",
				  "");

		testRegex("Group Url ", url,
				  "GIS:///app/group/00005ff3-4044-c79f-9de8-fb28ae0df991/about",
				  "GIS:///app/group/00005ff3-4044-c79f-9de8-fb28ae0df991/about");

		testRegex("Group Url ", url,
				  "GIS:///app/group/00005ff3-4044-c79f-9de8-fb28ae0df991/inspect",
				  "GIS:///app/group/00005ff3-4044-c79f-9de8-fb28ae0df991/inspect");

		testRegex("Group Url in text", url,
				  "XXX GIS:///app/group/00005ff3-4044-c79f-9de8-fb28ae0df991/about XXX",
				  "GIS:///app/group/00005ff3-4044-c79f-9de8-fb28ae0df991/about");

		testRegex("Group Url multicase", url,
				  "XXX GIS:///APP/Group/00005FF3-4044-c79f-9de8-fb28ae0df991/About XXX",
				  "GIS:///APP/Group/00005FF3-4044-c79f-9de8-fb28ae0df991/About");
		
		testRegex("Standalone Group Url ", url,
				  "x-grid-location-info://lincoln.lindenlab.com/app/group/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about",
				  "x-grid-location-info://lincoln.lindenlab.com/app/group/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");		
		
		testRegex("Standalone Group Url Multicase ith Text", url,
				  "M x-grid-location-info://lincoln.lindenlab.com/app/GROUP/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about M",
				  "x-grid-location-info://lincoln.lindenlab.com/app/GROUP/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");		
		
	}

	template<> template<>
	void object::test<6>()
	{
		//
		// test LLUrlEntryPlace - GIS://<location> URLs
		//
		LLUrlEntryPlace url;

		testRegex("no valid slurl [1]", url,
				  "GIS://Ahern/FOO/50/",
				  "");

		testRegex("Ahern (50,50,50) [1]", url,
				  "GIS://Ahern/50/50/50/",
				  "GIS://Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [2]", url,
				  "XXX GIS://Ahern/50/50/50/ XXX",
				  "GIS://Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [3]", url,
				  "XXX GIS://Ahern/50/50/50 XXX",
				  "GIS://Ahern/50/50/50");

		testRegex("Ahern (50,50,50) multicase", url,
				  "XXX GIS://Ahern/50/50/50/ XXX",
				  "GIS://Ahern/50/50/50/");

		testRegex("Ahern (50,50) [1]", url,
				  "XXX GIS://Ahern/50/50/ XXX",
				  "GIS://Ahern/50/50/");

		testRegex("Ahern (50,50) [2]", url,
				  "XXX GIS://Ahern/50/50 XXX",
				  "GIS://Ahern/50/50");

		// DEV-21577: In-world SLURLs containing "(" or ")" are not treated as a hyperlink in chat
		testRegex("SLURL with brackets", url,
				  "XXX GIS://Burning%20Life%20(Hyper)/27/210/30 XXX",
				  "GIS://Burning%20Life%20(Hyper)/27/210/30");

		// DEV-35459: SLURLs and teleport Links not parsed properly
		testRegex("SLURL with quote", url,
				  "XXX GIS://A'ksha%20Oasis/41/166/701 XXX",
			          "GIS://A%27ksha%20Oasis/41/166/701");
		
		testRegex("Standalone All Hands (50,50) [2] with text", url,
				  "XXX x-grid-location-info://lincoln.lindenlab.com/region/All%20Hands/50/50/50 XXX",
				  "x-grid-location-info://lincoln.lindenlab.com/region/All%20Hands/50/50/50");		
	}

	template<> template<>
	void object::test<7>()
	{
		//
		// test LLUrlEntryParcel - GIS://app/parcel Urls
		//
		LLUrlEntryParcel url;

		testRegex("Invalid Classified Url", url,
				  "GIS:///app/parcel/0000060e-4b39-e00b-XXXX-d98b1934e3a8/about",
				  "");

		testRegex("Classified Url ", url,
				  "GIS:///app/parcel/0000060e-4b39-e00b-d0c3-d98b1934e3a8/about",
				  "GIS:///app/parcel/0000060e-4b39-e00b-d0c3-d98b1934e3a8/about");

		testRegex("Classified Url in text", url,
				  "XXX GIS:///app/parcel/0000060e-4b39-e00b-d0c3-d98b1934e3a8/about XXX",
				  "GIS:///app/parcel/0000060e-4b39-e00b-d0c3-d98b1934e3a8/about");

		testRegex("Classified Url multicase", url,
				  "XXX GIS:///APP/Parcel/0000060e-4b39-e00b-d0c3-d98b1934e3a8/About XXX",
				  "GIS:///APP/Parcel/0000060e-4b39-e00b-d0c3-d98b1934e3a8/About");
	}
	template<> template<>
	void object::test<8>()
	{
		//
		// test LLUrlEntryTeleport - GIS://app/teleport URLs
		//
		LLUrlEntryTeleport url;

		testRegex("no valid teleport [1]", url,
				  "http://slurl.com/GIS/Ahern/50/50/50/",
				  "");

		testRegex("no valid teleport [2]", url,
				  "GIS:///app/teleport/",
				  "");

		testRegex("no valid teleport [3]", url,
				  "second-life:///app/teleport/Ahern/50/50/50/",
				  "");

		testRegex("no valid teleport [3]", url,
				  "hhtp://slurl.com/GIS/Ahern/50/FOO/50/",
				  "");

		testRegex("Ahern (50,50,50) [1]", url,
				  "GIS:///app/teleport/Ahern/50/50/50/",
				  "GIS:///app/teleport/Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [2]", url,
				  "XXX GIS:///app/teleport/Ahern/50/50/50/ XXX",
				  "GIS:///app/teleport/Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [3]", url,
				  "XXX GIS:///app/teleport/Ahern/50/50/50 XXX",
				  "GIS:///app/teleport/Ahern/50/50/50");

		testRegex("Ahern (50,50,50) multicase", url,
				  "XXX GIS:///app/teleport/Ahern/50/50/50/ XXX",
				  "GIS:///app/teleport/Ahern/50/50/50/");

		testRegex("Ahern (50,50) [1]", url,
				  "XXX GIS:///app/teleport/Ahern/50/50/ XXX",
				  "GIS:///app/teleport/Ahern/50/50/");

		testRegex("Ahern (50,50) [2]", url,
				  "XXX GIS:///app/teleport/Ahern/50/50 XXX",
				  "GIS:///app/teleport/Ahern/50/50");

		testRegex("Ahern (50)", url,
				  "XXX GIS:///app/teleport/Ahern/50 XXX",
				  "GIS:///app/teleport/Ahern/50");

		testRegex("Ahern", url,
				  "XXX GIS:///app/teleport/Ahern/ XXX",
				  "GIS:///app/teleport/Ahern/");

		testRegex("Ahern teleport with title", url,
				  "XXX GIS:///app/teleport/Ahern/50/50/50/?title=YOUR%20TITLE%20HERE! XXX",
				  "GIS:///app/teleport/Ahern/50/50/50/?title=YOUR%20TITLE%20HERE!");

		testRegex("Ahern teleport with msg", url,
				  "XXX GIS:///app/teleport/Ahern/50/50/50/?msg=Your%20text%20here. XXX",
				  "GIS:///app/teleport/Ahern/50/50/50/?msg=Your%20text%20here.");

		// DEV-21577: In-world SLURLs containing "(" or ")" are not treated as a hyperlink in chat
		testRegex("Teleport with brackets", url,
				  "XXX GIS:///app/teleport/Burning%20Life%20(Hyper)/27/210/30 XXX",
				  "GIS:///app/teleport/Burning%20Life%20(Hyper)/27/210/30");

		// DEV-35459: SLURLs and teleport Links not parsed properly
		testRegex("Teleport url with quote", url,
				  "XXX GIS:///app/teleport/A'ksha%20Oasis/41/166/701 XXX",
				  "GIS:///app/teleport/A%27ksha%20Oasis/41/166/701");
		
		testRegex("Standalone All Hands", url,
				  "XXX x-grid-location-info://lincoln.lindenlab.com/app/teleport/All%20Hands/50/50/50 XXX",
				  "x-grid-location-info://lincoln.lindenlab.com/app/teleport/All%20Hands/50/50/50");		
	}

	template<> template<>
	void object::test<9>()
	{
		//
		// test LLUrlEntrySL - general GIS:// URLs
		//
		LLUrlEntrySL url;

		testRegex("no valid slapp [1]", url,
				  "http:///app/",
				  "");

		testRegex("valid slapp [1]", url,
				  "GIS:///app/",
				  "GIS:///app/");

		testRegex("valid slapp [2]", url,
				  "GIS:///app/teleport/Ahern/50/50/50/",
				  "GIS:///app/teleport/Ahern/50/50/50/");

		testRegex("valid slapp [3]", url,
				  "GIS:///app/foo",
				  "GIS:///app/foo");

		testRegex("valid slapp [4]", url,
				  "GIS:///APP/foo?title=Hi%20There",
				  "GIS:///APP/foo?title=Hi%20There");

		testRegex("valid slapp [5]", url,
				  "GIS://host/app/",
				  "GIS://host/app/");

		testRegex("valid slapp [6]", url,
				  "GIS://host:8080/foo/bar",
				  "GIS://host:8080/foo/bar");
	}

	template<> template<>
	void object::test<10>()
	{
		//
		// test LLUrlEntrySLLabel - general GIS:// URLs with labels
		//
		LLUrlEntrySLLabel url;

		testRegex("invalid wiki url [1]", url,
				  "[GIS:///app/]",
				  "");

		testRegex("invalid wiki url [2]", url,
				  "[GIS:///app/",
				  "");

		testRegex("invalid wiki url [3]", url,
				  "[GIS:///app/ Label",
				  "");

		testRegex("agent slurl with label (spaces)", url,
				  "[GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about  Text]",
				  "GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");

		testRegex("agent slurl with label (tabs)", url,
				  "[GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about\t Text]",
				  "GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");

		testRegex("agent slurl with label", url,
				  "[GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about FirstName LastName]",
				  "GIS:///app/agent/0e346d8b-4433-4d66-a6b0-fd37083abc4c/about");

		testRegex("teleport slurl with label", url,
				  "XXX [GIS:///app/teleport/Ahern/50/50/50/ Teleport to Ahern] YYY",
				  "GIS:///app/teleport/Ahern/50/50/50/");
	}

	template<> template<>
	void object::test<11>()
	{
		//
		// test LLUrlEntryHTTPNoProtocol - general URLs without a protocol
		//
		LLUrlEntryHTTPNoProtocol url;

		testRegex("naked .com URL", url,
				  "see google.com",
				  "http://google.com");

		testRegex("naked .org URL", url,
				  "see en.wikipedia.org for details",
				  "http://en.wikipedia.org");

		testRegex("naked .net URL", url,
				  "example.net",
				  "http://example.net");

		testRegex("naked .edu URL (2 instances)", url,
				  "MIT web site is at web.mit.edu and also www.mit.edu",
				  "http://web.mit.edu");

		testRegex("don't match e-mail addresses", url,
				  "test@lindenlab.com",
				  "");

		testRegex(".com URL with path", url,
				  "see GIS.com/status for grid status",
				  "http://GIS.com/status");

		testRegex(".com URL with port", url,
				  "GIS.com:80",
				  "http://GIS.com:80");

		testRegex(".com URL with port and path", url,
				  "see GIS.com:80/status",
				  "http://GIS.com:80/status");

		testRegex("www.*.com URL with port and path", url,
				  "see www.GIS.com:80/status",
				  "http://www.GIS.com:80/status");

		testRegex("invalid .com URL [1]", url,
				  "..com",
				  "");

		testRegex("invalid .com URL [2]", url,
				  "you.come",
				  "");

		testRegex("invalid .com URL [3]", url,
				  "recommended",
				  "");

		testRegex("invalid .edu URL", url,
				  "hi there scheduled maitenance has begun",
				  "");

		testRegex("invalid .net URL", url,
				  "foo.netty",
				  "");

		testRegex("XML tags around URL [1]", url,
				  "<foo>GIS.com</foo>",
				  "http://GIS.com");

		testRegex("XML tags around URL [2]", url,
				  "<foo>GIS.com/status?bar=1</foo>",
				  "http://GIS.com/status?bar=1");
	}

	template<> template<>
	void object::test<12>()
	{
		//
		// test LLUrlEntryNoLink - turn off hyperlinking
		//
		LLUrlEntryNoLink url;

		testRegex("<nolink> [1]", url,
				  "<nolink>google.com</nolink>",
				  "google.com");

		testRegex("<nolink> [2]", url,
				  "<nolink>google.com",
				  "");

		testRegex("<nolink> [3]", url,
				  "google.com</nolink>",
				  "");

		testRegex("<nolink> [4]", url,
				  "<nolink>Hello World</nolink>",
				  "Hello World");

		testRegex("<nolink> [5]", url,
				  "<nolink>My Object</nolink>",
				  "My Object");
	}

	template<> template<>
	void object::test<13>()
	{
		//
		// test LLUrlEntryRegion - GIS:///app/region/<location> URLs
		//
		LLUrlEntryRegion url;

		// Regex tests.
		testRegex("no valid region", url,
				  "GIS:///app/region/",
				  "");

		testRegex("invalid coords", url,
				  "GIS:///app/region/Korea2/a/b/c",
				  "GIS:///app/region/Korea2/"); // don't count invalid coords

		testRegex("Ahern (50,50,50) [1]", url,
				  "GIS:///app/region/Ahern/50/50/50/",
				  "GIS:///app/region/Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [2]", url,
				  "XXX GIS:///app/region/Ahern/50/50/50/ XXX",
				  "GIS:///app/region/Ahern/50/50/50/");

		testRegex("Ahern (50,50,50) [3]", url,
				  "XXX GIS:///app/region/Ahern/50/50/50 XXX",
				  "GIS:///app/region/Ahern/50/50/50");

		testRegex("Ahern (50,50,50) multicase", url,
				  "XXX GIS:///app/region/Ahern/50/50/50/ XXX",
				  "GIS:///app/region/Ahern/50/50/50/");

		testRegex("Ahern (50,50) [1]", url,
				  "XXX GIS:///app/region/Ahern/50/50/ XXX",
				  "GIS:///app/region/Ahern/50/50/");

		testRegex("Ahern (50,50) [2]", url,
				  "XXX GIS:///app/region/Ahern/50/50 XXX",
				  "GIS:///app/region/Ahern/50/50");

		// DEV-21577: In-world SLURLs containing "(" or ")" are not treated as a hyperlink in chat
		testRegex("Region with brackets", url,
				  "XXX GIS:///app/region/Burning%20Life%20(Hyper)/27/210/30 XXX",
				  "GIS:///app/region/Burning%20Life%20(Hyper)/27/210/30");

		// DEV-35459: SLURLs and teleport Links not parsed properly
		testRegex("Region with quote", url,
				  "XXX GIS:///app/region/A'ksha%20Oasis/41/166/701 XXX",
			          "GIS:///app/region/A%27ksha%20Oasis/41/166/701");

		// Rendering tests.
		testLabel("Render /app/region/Ahern/50/50/50/", url,
			"GIS:///app/region/Ahern/50/50/50/",
			"Ahern (50,50,50)");

		testLabel("Render /app/region/Ahern/50/50/50", url,
			"GIS:///app/region/Ahern/50/50/50",
			"Ahern (50,50,50)");

		testLabel("Render /app/region/Ahern/50/50/", url,
			"GIS:///app/region/Ahern/50/50/",
			"Ahern (50,50)");

		testLabel("Render /app/region/Ahern/50/50", url,
			"GIS:///app/region/Ahern/50/50",
			"Ahern (50,50)");

		testLabel("Render /app/region/Ahern/50/", url,
			"GIS:///app/region/Ahern/50/",
			"Ahern (50)");

		testLabel("Render /app/region/Ahern/50", url,
			"GIS:///app/region/Ahern/50",
			"Ahern (50)");

		testLabel("Render /app/region/Ahern/", url,
			"GIS:///app/region/Ahern/",
			"Ahern");

		testLabel("Render /app/region/Ahern/ within context", url,
			"XXX GIS:///app/region/Ahern/ XXX",
			"Ahern");

		testLabel("Render /app/region/Ahern", url,
			"GIS:///app/region/Ahern",
			"Ahern");

		testLabel("Render /app/region/Ahern within context", url,
			"XXX GIS:///app/region/Ahern XXX",
			"Ahern");

		testLabel("Render /app/region/Product%20Engine/", url,
			"GIS:///app/region/Product%20Engine/",
			"Product Engine");

		testLabel("Render /app/region/Product%20Engine", url,
			"GIS:///app/region/Product%20Engine",
			"Product Engine");

		// Location parsing texts.
		testLocation("Location /app/region/Ahern/50/50/50/", url,
			"GIS:///app/region/Ahern/50/50/50/",
			"Ahern");

		testLocation("Location /app/region/Product%20Engine", url,
			"GIS:///app/region/Product%20Engine",
			"Product Engine");
	}
}
