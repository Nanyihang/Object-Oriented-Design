#pragma once
///////////////////////////////////////////////////////////////////////
// Display.h -             class used to wrap class, function        //
//                         and comments                              //
// ver 1.1                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a class, Display, used to wrap class, function
*	and commetns.
*
*  Required Files:
*  ---------------
*  Display.h Display.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 05 Mar 2019
*  - customer requirements changed
*  ver 1.0 : 05 Feb 2019
*  - first release
*/


#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <vector>
#include <iostream>
#include <conio.h>

// ---------<Define the class>---------------
template<typename T>
class Display
{
public:
	Display();
	~Display() {}
	std::string ClsFuncWrap(T* pItem, std::string &content);
	std::string CommentWrap(std::string& content);
private:
	

};
// ---------<Initialize of class>---------------
template<typename T>
inline 	Display<T>::Display()
{

}

//------------<function for wrapping class and function> ------------------
template<typename T>
inline std::string Display<T>::ClsFuncWrap(T* pItem, std::string &content)
{
	auto iter = pItem->children_.begin();
	if (pItem->type_ == "class" || pItem->type_ == "function")
	{
		int size = content.size();
		int Line = 0;
		for (int i = 0; i < size; i++)
		{
			if (content[i] == '\n')
				Line++;
			if (pItem->type_ == "function")
			{
				if (Line == (pItem->startLineCount_ - 1) && content[i] == '{')
				{
					content.insert(i, "<span class=\"toggleMethodDisplay\">");
					i += strlen("<span class=\"toggleMethodDisplay\">");
					size += strlen("<span class=\"toggleMethodDisplay\">");
				}
				if (Line == (pItem->endLineCount_ - 1) && content[i] == '}')
				{
					content.insert(i + 1, "</span>");
					i += strlen("</span>");
					size += strlen("</span>");
				}
			}
			if (pItem->type_ == "class")
			{
				if (Line == (pItem->startLineCount_ - 1) && content[i] == '{')
				{
					content.insert(i, "<span class=\"toggleClassDisplay\">");
					i += strlen("<span class=\"toggleClassDisplay\">");
					size += strlen("<span class=\"toggleClassDisplay\">");
				}
				if (Line == (pItem->endLineCount_ - 2) && content[i] == ';' && content[i - 1] == '}')
				{
					content.insert(i + 1, "</span>");
					i += strlen("</span>");
					size += strlen("</span>");
				}
			}
		}
	}
	while (iter != pItem->children_.end())
	{
		ClsFuncWrap(*iter, content);
		++iter;
	}
	return content;
}
//------------<function for wrapping comments> ------------------
template<typename T>
inline std::string Display<T>::CommentWrap(std::string& content)
{
	int size = content.size();
	int Line = 0;
	bool one_line = false;
	bool multi_line = false;
	int initialsize = content.size();
	for (int i = 0; i < size; i++)
	{
		if (content[i] == '\n')
			Line++;

		if (content[i] == '/' && content[i + 1] == '/' && !one_line)
		{
			size += strlen("<span class=\"toggleCommentsDisplay\">");
			content.insert(i, "<span class=\"toggleCommentsDisplay\">");
			i += strlen("<span class=\"toggleCommentsDisplay\">");
			one_line = true;
		}
		else if (content[i] == '\n' && one_line)
		{
			size += strlen("</span>");
			content.insert(i, "</span>");
			i += strlen("</span>");
			one_line = false;
		}
		if (content[i] == '/'&& content[i + 1] == '*' && !multi_line)
		{
			size += strlen("<span class=\"toggleCommentsDisplay\">");
			content.insert(i, "<span class=\"toggleCommentsDisplay\">");
			i += strlen("<span class=\"toggleCommentsDisplay\">");
			multi_line = true;
		}
		else if (content[i] == '*' && content[i + 1] == '/' && multi_line)
		{
			size += strlen("</span>");
			content.insert(i + 2, "</span>");
			i += strlen("</span>") + 2;
			multi_line = false;
		}
	}
	return content;
}
#endif
