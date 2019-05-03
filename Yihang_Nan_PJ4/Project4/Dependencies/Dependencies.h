#pragma once
#ifndef _DEPENDENCIES_H
#define _DEPENDENCIES_H
///////////////////////////////////////////////////////////////////////
// Dependencies.h -        class used to analyze dependencies between//
//                         files                                     //
// ver 1.0                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a class, Dependencies, used to analyze dependencies
*  between files
*
*  Required Files:
*  ---------------
*  Dependencies.h Dependencies.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 04 Mar 2019
*  - first release
*/
#include <vector>

//<-----------<class defination>------------------>
template<typename T,typename map>
class Dependencies
{
public:
	Dependencies(T* pItem,map mp_);
	~Dependencies() {}
	 std::vector<std::string> GetDep();
	 std::string insertLink();
private:
	void initial(T* pItem);
	std::vector<std::string> storedDep;
	map mp;
	bool hasDepinMap(std::string name);
};

//<-----------<construction function>------------------>
template<typename T, typename map>
inline Dependencies<T,map>::Dependencies(T* pItem,map mp_)
{
	mp = mp_;
	initial(pItem);
}

//<--------------------<function of initialization>----------------------->
template<typename T, typename map>
inline void Dependencies<T,map>::initial(T* pItem)
{
	auto iter = pItem->statements_.begin();
	std::string text;
	while (iter != pItem->statements_.end())
	{
		if ((*iter)->contains("include") && !(*iter)->contains("<") && !(*iter)->contains(">"))
		{
			text = (*iter)->show();
			if (text.find("/") == -1)
			{
				text = text.substr(text.find_first_of("\""));
				while (text.find("\"") != -1)
				{
					text = text.erase(text.find("\""), 1);
				}
				if (text.find("\n") != -1)
					text = text.erase(text.find("\n"), 1);
				if (text.find(" ") != -1)
					text = text.erase(text.find(" "), 1);
				if(hasDepinMap(text)==true)
				storedDep.push_back(text);
			}
			else
			{
				text = text.substr(text.find_last_of("/") + 1);
				while (text.find("\"") != -1)
				{
					text = text.erase(text.find("\""), 1);
				}
				if (text.find("\n") != -1)
					text = text.erase(text.find("\n"), 1);
				if (text.find(" ") != -1)
					text = text.erase(text.find(" "), 1);
				if (hasDepinMap(text) == true)
				storedDep.push_back(text);
			}
		}
		iter++;
	}
}

//<----------------<function for return back the stored dependent table>----------->
template<typename T, typename map>
std::vector<std::string> Dependencies<T,map>::GetDep()
{
	return storedDep;
}

//<----------------function for determing whether existing object in the map------------>
template<typename T,typename map>
bool Dependencies<T, map>::hasDepinMap(std::string name)
{
	if (mp.find(name) != mp.end())
		return true;
	return false;
}
//----------------------------<function for inserting the link elements and return back>-----------------
template<typename T, typename map>
inline std::string Dependencies<T, map>::insertLink()
{
	std::string text;
	for (std::string ele : storedDep)
	{
		if (ele.find(".h") != -1)
		{
			int pos = ele.find(".");
			ele = ele.substr(0, pos);
			text.append("<a href=\"" + ele + "_H.html\">" + ele + "_H.html" + "</a>" + "\n");
		}
		if (ele.find(".cpp") != -1)
		{
			int pos = ele.find(".");
			ele = ele.substr(0, pos);
			text.append("<a href=\"" + ele + "_CPP.html\">" + ele + "_CPP.html" "</a>" + "\n");
		}
	}
	return text;
}
#endif