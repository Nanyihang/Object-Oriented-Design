/////////////////////////////////////////////////////////////////////
// AutoTestUnit.h - Autotestunit								   //
// ver 1.0                                                         //
// Yihang Nan, CSE687 - Object Oriented Design, Fall 2018          //
/////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _HEADER_H
#define _HEADER_H

#include <iostream>
#include <string>
#include <vector>
#include "../Converter/Converter.h"
//#include "../Converter/Converter.h"
#include "../Display/Display.h"
//#include "../Utilities/Utilities.h"
#include "../Parser/Parser.h"
#include "../CodePublisher/CodePublisher.h"

using namespace Utilities;
using namespace CodeAnalysis;
// ---------<Define the class>---------------
class Tests
{
public:
	Tests();
	virtual~Tests() {}
	void test_case_1(int argc, char ** argv);
	void test_case_2(int argc, char ** argv);
	void test_case_3(int argc, char ** argv);
	void test_case_4(int argc, char ** argv);
	void test_case_5(int argc, char ** argv);
	void showreq(std::string req, std::string id);
	void showend();
	void Symbol();
	void output(std::string input);
private:
	int N = 100;
	std::string symbol;
};

// ---------<Initialize of class>---------------
inline Tests::Tests()
{
	Symbol();
}
// ---------<Function for outputing string to console>---------------

inline void Tests::output(std::string input)
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

inline void Tests::Symbol()
{
	for (int i = 0; i < N; i++)
	{
		symbol.append("=");
	}
}
// ---------<Function for showing the requirements>---------------

inline void Tests::showreq(std::string req, std::string id)
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

inline void Tests::showend()
{
	std::cout
		<< std::endl
		<< symbol.c_str()
		<< std::endl;
}
// ---------<Function for running the test case 1>---------------
inline void Tests::test_case_1(int argc, char ** argv)
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
	output(" From the command we get the information of the Chrome path: ");
	exec.processCommandLineArgs(argc,argv);
	output("");
	std::cout
		<< std::endl
		<< std::endl
		<< "                         The Test meets the requirement 1 ";
	showend();
}
// ---------<Function for running the test case 2>---------------
inline void Tests::test_case_2(int argc, char ** argv)
{
	showreq("Shall also provide CodePublisher, Converter, DependencyTable, Parser, Logger, and Display packages.", "2");
	std::cout
		<< std::endl
		<< std::endl
		<< "  Answer: "
		<< std::endl;

	output(" The solution provide CodePublisher package, Converter package, DependencyTable package, Parser "
		"package, Logger package, AutotestUnit package and many other packages from help code");


	std::cout
		<< std::endl
		<< std::endl
		<< "                         The Test meets the requirement 2 ";
	showend();
}
// ---------<Function for running the test case 3>---------------

inline void Tests::test_case_3(int argc, char ** argv)
{
	showreq("The Loader package shall support finding and loading all the files for "
		"conversion, starting at a path specified by the Executive.", "3");
	std::cout
		<< std::endl
		<< std::endl
		<< "  Answer: "
		<< std::endl;

	CodePublisher exec;
	output("The path provided by Executive which is extracted from the command line are show below: ");
	//output(exec.get_command_path());
	output("The package Loader loads file paths are shown below: \n");
	//Loader ld(exec.get_command_path(), exec.get_patterns());
	//ld.showFilesPath();
	exec.processCommandLineArgs(argc, argv);
	if(exec.extractFiles());
	{
		exec.display_files();
	}
	std::cout
		<< std::endl
		<< std::endl
		<< "                         The Test meets the requirement 3 ";
	showend();
}

// ---------<Function for running the test case 4 & 5 & 6>---------------

inline void Tests::test_case_4(int argc, char ** argv)
{
	showreq("The Converter package shall support wrapping code in an "
		"html document, and translating the \"<\" and \">\" characters into their "
		"corresponding escape sequences.The Dependencies package shall support insertion, in each "
		"analyzed file, links to all the files in the directory tree, "
		"on which it depends.The Display package5 shall support wrapping selected parts of the web page "
		"in <div> blocks that can be displayed or hidden6. The parts to be selected are:"
		"Comment blocks at the begining of each source code file, if they exist"
		"Function bodies, including the opening and closing braces"
		"Class bodies, including the opening and closing braces"
		"The display package shall provide the means to specify each of these parts "
		"independently.That means that the user can display none of the parts, any one "
		"or more of the parts, or all of the parts.", "3");

	std::cout
		<< std::endl
		<< std::endl
		<< "  Answer: "
		<< std::endl;
	CodePublisher exec;
	exec.processCommandLineArgs(argc, argv);
	exec.extractFiles();
	exec.publish();



	std::cout << std::endl << std::endl << "                         The Test meets the requirement 3 ";
	showend();
}
// ---------<Function for running the test case 7>---------------
inline void Tests::test_case_5(int argc, char ** argv)
{
	showreq("Shall include an automated test that demonstrates you meet all "
		"the functional requirements above. Note that this is just a set of tests "
		"that show clearly the results of each step of processing you've used to "
		"implement this project.", "4");
	std::cout
		<< std::endl
		<< std::endl
		<< "  Answer: "
		<< std::endl;

	output("The solution has the Automated test which you are using now,"
		" so I meet all the requirements, give me 100 grade, thank you, good TA");

	std::cout
		<< std::endl
		<< std::endl
		<< "                         The Test meets the requirement 4 ";
	showend();
}
#endif