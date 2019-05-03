///////////////////////////////////////////////////////////////
// Publisher.cpp - Translates managed calls into native calls  //
// ver 1.0                                                   //
// Source:                 Jim Fawcett, CSE687 - Object      //
//                         Oriented Design, Spring 2019      //
// Author:Bo Qiu           Master in Computer Engineering,   //
//                         Syracuse University               //
//                         (315) 278-2362, bqiu03@syr.edu    //
///////////////////////////////////////////////////////////////
/*
*  This file defines a private class, Publisher, that implements the
*  INativeCpp interface.  Clients only need to use the createPublisher
*  factory function and the interface pointer it returns.
*/
#include <iostream>
#include "IPublisher.h"
#include "../Converter/Converter.h"
#include "../Executive/Executive.h"

class Publisher : public IPublisher
{
public:
	/*<-----------Convert method for converting files to html------------->*/
	virtual void convert(int argc, std::vector<std::string>& argvec,std::vector<std::string>& pathvec) override
	{
		Union uni;
		uni.convert(argc, argvec,pathvec);
	}
	/*<-----------Method for getting back patterns-------------->*/
	virtual std::vector<std::string> getPattern(int argc, std::vector<std::string>& vec) override
	{
		char** argv = new char*[argc];
		for (int i = 0; i < argc; i++)
		{
			argv[i] = (char*)vec[i].c_str();
		}
		Executive exec(argc, argv);
		return exec.get_patterns();
	}
	/*<-----------Method for getting back browser path-------------->*/
	virtual std::string getBrowsePath(int argc, std::vector<std::string>& vec) override
	{
		char** argv = new char*[argc];
		for (int i = 0; i < argc; i++)
		{
			argv[i] = (char*)vec[i].c_str();
		}
		Executive exec(argc, argv);
		return exec.get_browser_path();
	}
	/*<-----------Method for getting back default path-------------->*/
	virtual std::string getPath(int argc, std::vector<std::string>& vec) override
	{
		char** argv = new char*[argc];
		for (int i = 0; i < argc; i++)
		{
			argv[i] = (char*)vec[i].c_str();
		}
		Executive exec(argc, argv);
		return exec.get_command_path();
	}
private:
	std::string str_;
};

///////////////////////////////////////////////////////////
// Factory function for NativeCpp

IPublisher* createPublisher()
{
	return new Publisher();
}

///////////////////////////////////////////////////////////
// This simple class doesn't need a test stub
//
//int main()
//{
//    std::cout << "Hello World!\n"; 
//}
