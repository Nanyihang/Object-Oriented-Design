#pragma once
///////////////////////////////////////////////////////////////////////
// Loader.h -              class used to load files                  //
// ver 1.1                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a class, Loader, used to load files.
*
*  Required Files:
*  ---------------
*  Loader.h Loader.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 04 Mar 2019
*	Rename package and fix bugs
*  ver 1.0 : 05 Feb 2019
*  - first release
*/

#ifndef _LOADER_H
#define _LOADER_H

#include <iostream>
#include <vector>
#include "io.h"
#include <fstream>
#include <regex>
#include <stack>

// ---------<Define the class for loading files>---------------

class Loader
{
public:
	Loader(std::vector<std::string> paths);
	Loader(std::string path, std::vector<std::string> patterns);
	virtual~Loader() {}
	void showFilesPath();
	void showFilesName();
	void clear_files();
	std::vector<std::string> GetAllFilesPath();
	std::vector<std::string> GetAllFilesName();

private:
	void apply_pattern(std::string path, std::vector<std::string> patterns);
	void add_files(std::string path, std::string pattern);
	bool Has_same_ele(std::string ele);
	void add_files_name();
	std::vector<std::string> AllFilesName;
	std::vector<std::string> AllFilesPath;
};

// ---------<Initialize of class>---------------
inline Loader::Loader(std::string path, std::vector<std::string> patterns)
{
	apply_pattern(path, patterns);
	add_files_name();
}

inline Loader::Loader(std::vector<std::string> paths)
{
	AllFilesPath = paths;
	add_files_name();
}


// ---------<Function for return the stored files path>---------------

inline std::vector<std::string> Loader::GetAllFilesPath()
{
	return AllFilesPath;
}
// ---------<Function for return the stored files names>---------------

inline std::vector<std::string> Loader::GetAllFilesName()
{
	return AllFilesName;
}
// ---------<Function for adding the files names to member value>---------------

inline void Loader::add_files_name()
{
	std::vector <std::string> vtemp = AllFilesPath;
	std::stack<std::string> st;
	if (vtemp.empty())
		return;
	while (!vtemp.empty())
	{
		std::string stemp = vtemp.back();
		vtemp.pop_back();
		if (stemp.find_last_of("\\") != -1 && stemp.find_last_of("\\") != stemp.size() - 1)
		{
			st.push(stemp.substr(stemp.find_last_of("\\") + 1));
		}
	}
	while (!st.empty())
	{
		AllFilesName.push_back(st.top());
		st.pop();
	}
}

// ---------<Function of applying the patterns to select files>---------------
void Loader::apply_pattern(std::string path, std::vector<std::string> patterns)
{
	auto it1 = patterns.begin();
	while (it1 != patterns.end())
	{
		add_files(path, *it1);
		it1++;
	}
}
// ---------<Function for checking the same values in storage>---------------
inline bool Loader::Has_same_ele(std::string ele)
{
	for (auto eles : AllFilesPath)
	{
		if (eles == ele)
			return true;
	}
	return false;
}

// ---------<Function for adding the files path to member value>---------------
inline void Loader::add_files(std::string path = "../../", std::string pattern = ".*")
{
	long   hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					add_files(p.assign(path).append("\\").append(fileinfo.name), pattern);
			}
			else
			{
				p.assign(path).append("\\").append(fileinfo.name);
				if (std::regex_match(fileinfo.name, std::regex(pattern)) && Has_same_ele(p) == false)
					AllFilesPath.push_back(p);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

// ---------<Function for showing the member value of files path>---------------

inline void Loader::showFilesPath()
{
	if (AllFilesPath.empty() == true)
		std::cout << " No files are finded " << std::endl;
	for (auto file : AllFilesPath)
		std::cout << file.c_str() << std::endl;
}
// ---------<Function for showing the member value of files name>---------------
inline void Loader::showFilesName()
{
	if (AllFilesName.empty() == true)
		std::cout << "No files are finded" << std::endl;
	for (auto file : AllFilesName)
		std::cout << file.c_str() << std::endl;
}
// ---------<Function for clearing the storaged file paths and names>---------------

inline void Loader::clear_files()
{
	AllFilesPath.clear();
	AllFilesName.clear();
}
#endif 

