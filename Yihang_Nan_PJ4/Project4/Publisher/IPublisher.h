#pragma once
///////////////////////////////////////////////////////////////
// IPublisher.h - Translates managed calls into native calls //
// ver 1.0                                                   //
// Source:                 Jim Fawcett, CSE687 - Object      //
//                         Oriented Design, Spring 2019      //
// Author:Bo Qiu           Master in Computer Engineering,   //
//                         Syracuse University               //
//                         (315) 278-2362, bqiu03@syr.edu    //
///////////////////////////////////////////////////////////////
/*
*  This package provides an interface for the Publisher class
*  and a declaration of an object factory function.  Using these
*  isolates the user from all implementation details in Publisher.
*
*  That will be important for projects #3 and #4.  For example,
*  in Project #4 the WPF GUI needs to use services of the Comm
*  channel.  Comm uses native threads, but neither C# nor C++\CLI
*  can compile native threads, so this isolation is crucial.
*
*  Publisher doesn't do anything useful, other than illustrating
*  how to call native C++ methods from C# via a C++\CLI translator.
*
*  The easiest way to use this package is to build it as a static
*  library and the Translator project makes a reference to this
*  project.
*
   Package Operations :
*  This package provides IPublisher interface to connect c++ interface
*
*  Required Files :
*-------------- -
*  IPublisher.h Publisher.cpp
*
*  Maintenance History :
*--------------------
*  ver 1.0 : 09 April 2019
* -first release
*/

#include <string>
#include <vector>

struct IPublisher
{
	virtual ~IPublisher() {}
	virtual void convert(int argc, std::vector<std::string>& argvec, std::vector<std::string>& pathvec) = 0;
	virtual std::vector<std::string> getPattern(int argc, std::vector<std::string>& vec) = 0;
	virtual std::string getBrowsePath(int argc, std::vector<std::string>& vec) = 0;
	virtual std::string getPath(int argc, std::vector<std::string>& vec) = 0;

};

IPublisher* createPublisher();
