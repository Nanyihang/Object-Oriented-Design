#pragma once
///////////////////////////////////////////////////////////////////////
// AutoTestUnit.h -        class used to run test case to meet       //
//                         the requirements                          //
// ver 1.1                                                           //
// Author: Yihang Nan      Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 863-0892, ynan02@syr.edu            //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a class, Tests, used to run test case to meet the requirements
*
*  Required Files:
*  ---------------
*  AutoTestUnit.h AutoTestUnit.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 05 Mar 2019
*  - customer requirements changed
*  ver 1.0 : 05 Feb 2019
*  - first release
*/
#ifndef _HEADER_H
#define _HEADER_H

#include <iostream>
#include <string>
#include <vector>
#include "../Parser/Parser.h"
#include "../CodePublisher/CodePublisher.h"
//#include "../Utilities/Utilities.h"
//using namespace Utilities;
using namespace CodeAnalysis;

class Tests
{
public:
	Tests();
	virtual~Tests() {}
	void test_case_1(int argc, char*argv[]);
	void test_case_2(int argc, char* argv[]);
	void test_case_3(int argc, char*argv[]);
	void test_case_4(int argc, char* argv[]);
	void test_case_5(int argc, char* argv[]);
	void test_case_6(int argc, char* argv[]);
	void test_case_7();
	void showreq(std::string req, std::string id);
	void showend();
	void Symbol();
	void output(std::string input);
private:
	int N = 100;
	std::string symbol;
};

// ---------<Initialize of class>---------------

Tests::Tests()
{
	Symbol();
}
// ---------<Function for outputing string to console>---------------
void Tests::output(std::string input)
{
	std::cout << " " << std::endl;
	for (int i = 1; i < ((int)input.size() + 1); i++)
	{
		std::cout << input[i - 1];
		if (i%N == 0)
		{
			std::cout << " " << std::endl;
		}
	}
}
// ---------<Function for creating symbol to Demonstration>---------------
void Tests::Symbol()
{
	for (int i = 0; i < N; i++)
	{
		symbol.append("=");
	}
}
// ---------<Function for showing the requirements>---------------
void Tests::showreq(std::string req, std::string id)
{
	std::string head;
	head = " Requirement: " + id;
	for (int i = 0; i < N / 3; i++)
		head.insert(0, " ");
	std::cout
		<< std::endl
		<< symbol.c_str()
		<< std::endl
		<< head.c_str()
		<< std::endl;
	output(req);
}
// ---------<Function for showing the end>---------------

void Tests::showend()
{
	std::cout
		<< std::endl
		<< symbol.c_str()
		<< std::endl;
}
// ---------<Function for running the test case 1>---------------
void Tests::test_case_1(int argc, char*argv[])
{
	showreq("Shall provide an Executive package that accepts, "
		"from its command line, a path containing"
		"files to be converted.", "1");
	std::cout
		<< std::endl
		<< std::endl
		<< "  Answer: "
		<< std::endl;

	output("");
	output(" Now process command line, the input command line context show below: ");
	for (int i = 0; i < argc; i++)
	{
		output(argv[i]);
	}

	output("");
	output("");
	output(" Now using Executive Package to extract the necessary information: ");
	CodePublisher exec;
	exec.processCommandLineArgs(argc, argv);
	output(" From the command we get the information of the Chrome path: ");
	//output(exec.get_browser_path());
	output("");
	//exec.get_command_path();
	output(" From the command we get the information of the path contains files to be converted: ");
	//output(exec.get_command_path());
	output("");
	//exec.get_patterns();
	output(" From the command we get the information of the input regex to select specified files: \n");
	//for (std::string st : exec.get_patterns())
		//std::cout << st.c_str() << std::endl;

	std::cout
		<< std::endl
		<< std::endl
		<< "                         The Test meets the requirement 1 ";
	showend();
}
#endif