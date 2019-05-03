#pragma once
///////////////////////////////////////////////////////////////////////
// Converter.h -           class used to get file content and        //
//                         transfer to html content                  //
// ver 1.1                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides two classes, converter and _html_, used to get file content and
*  transfer to html content, and create the html files
*
*  Required Files:
*  ---------------
*  Converter.h Converter.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 04 Mar 2019
*	fix bugs and functions
*  ver 1.0 : 05 Feb 2019
*  - first release
*/

#ifndef _CONVERTER_H
#define _CONVERTER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include <string>
#include <map>

// ---------<Define the class>---------------

class converter
{
public:
	converter(std::vector<std::string> fnames, std::vector<std::string>fpaths, std::vector<std::string>htmlpaths);
	virtual ~converter() {}
	std::string ReadText(std::string path);
	std::string FToHtml(std::string Ftext);
	std::string GetFtext();
	std::string GetHtmltext();
	void FtextClear();
	void HtmltextClear();
	std::multimap<std::string, std::string> GetMap();
	
private:
	std::string Ftext;
	std::string HtmlText;
	std::multimap<std::string, std::string> mp;
	void createMap(std::vector<std::string> fnames, std::vector<std::string>fpaths, std::vector<std::string>htmlpaths);
};

// ---------<Define the class>---------------
class _html_
{
public:
	_html_(std::string repos_dir,std::vector<std::string> names);
	virtual ~_html_() { }
	void initial(std::string decler, std::string html, std::string head, std::string style, std::string body, std::string pre,
		std::string div, std::string scr);
	void add_pre(std::string pre);
	void create(std::string HtmlPath);
	void ele_clear();
	void reset_html(std::string r_decler, std::string r_html , std::string r_head , std::string r_style , std::string r_body , std::string r_pre,
					std::string r_div,std::string r_scr);
	std::vector<std::string> GetHtmlAddres();
	std::string GetEle();
	void add_head(std::string head);

private:
	std::string ele_decler;
	std::string ele_html;
	std::string ele_head;
	std::string ele_style;
	std::string ele_body;
	std::string ele_pre;
	std::string ele_scr;
	std::string ele_div;
	std::string ele;
	std::vector <std::string> HtmlAddress;
	std::string filePathTrans(std::string repos_dir, std::string name);
	void CreateHtmlPaths(std::string repos_dir, std::vector<std::string> names);
	void add_script();
	void add_style();
	void add_div();
};
class Union
{
public:
	void convert(int argc,std::vector<std::string>& argvec, std::vector<std::string>& pathvec);
};

#endif