#pragma once
///////////////////////////////////////////////////////////////////////
// Executive.h             class used to handle the path             //
// ver 1.1                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a class, Executive, used to process path.
*
*  Required Files:
*  ---------------
*  Executive.h Executive.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 04 Mar 2019
*	Rename package and fix bugs
*  ver 1.0 : 05 Feb 2019
*  - first release
*/

#ifndef _EXECUTIVE_H
#define _EXECUTIVE_H

#include <iostream>
#include <direct.h>
#include <vector>


// ---------<Define the class for managing the path>---------------
class Executive
{
public:
	Executive(int argc, char*argv[]);
	Executive();
	virtual ~Executive() { }
	std::string Get_AB_curworking_path();
	std::string Get_AB_root_solution_path();
	std::string Get_CSAB_root_solution_path();
	std::string Combine_path(std::string str1, std::string str2);
	void set_repos_path(std::string path);
	bool has_command_path(int argc, char*argv[]);
	std::string get_command_path();
	std::string get_browser_path();
	std::vector<std::string> get_patterns();
	int test();
private:
	std::string AB_curworking_path;
	std::string AB_root_solution_path;
	std::string CSAB_root_solution_path;
	bool bcmdpath = false;
	std::string command_path;
	std::string browser_path;
	std::vector<std::string> patterns;
	void cswpTosp();
	void wpTosp();
	void cwp();
	void Gbropath(int argc, char*argv[]);
	void Gpatterns(int argc, char*argv[]);
};

#endif