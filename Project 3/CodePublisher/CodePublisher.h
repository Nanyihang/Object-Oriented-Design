#pragma once
///////////////////////////////////////////////////////////////////////////
// CodePublisher.h : defines facade/executive for OOD Pr1 S19            //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        :	Ammar,  Syracuse University                          //
//					Yihang Nan, Syracuse University                      //
//                 3158630892, ynan02@syr.edu	                         //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodePublisher which uses different components of
*  CodeConverter, Display and DirectoryN to put together the entire 
*  functionality of the CodePublisher. The mentioned packages are indep-
*  ndent of each other and their only interaction is through this facade
*
*  NOTE: there are TWO modes for compiling this file:
*   1) Preprocessor DEMO_CP : runs in full demo mode for Pr1.
*   2) Preprocessor USE_CP  : runs normal functionality. 
*
*  Required Files:
* =======================
*  CodePublisher.h CodePublisher.cpp CodeUtilities.h
*  Converter.h Converter.cpp DirExplorerN.h DirExplorerN.cpp
*  Display.h Display.cpp Logger.h Logger.cpp 
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 Feb 2019
*  - first release
*/

#include <string>
#include <vector>

#include "../Converter/Converter.h"
#include "../Display/Display.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../Parser/Parser.h"

class CodePublisher
{
public:
	CodePublisher();
	CodePublisher(int argc, char* argv[]);
	bool processCommandLineArgs(int argc, char** argv);

	void workingDirectory(const std::string& dir);
	const std::string& workingDirectory() const;

	void outputDirectory(const std::string& dir);
	const std::string& outputDirectory() const;

	bool extractFiles();

	void publish();
	void publish(const std::string& file);
	void publish(const std::vector<std::string>& files);
	void pass_path_parser();
	void display_files();

	void add_files_fromGUI(std::vector<std::string> from_gui_files);
	std::string get_command_path();
	std::string get_browser_path();
	std::vector<std::string> get_patterns();

	Utilities::DisplayMode displayMode() const;

private:
	Utilities::ProcessCmdLine* pcl_;
	CodeConverter cconv_;
	Display display_;

	std::string dirIn_;
	std::string dirOut_;
	std::vector<std::string> files_;
	std::string command_path;
	std::string browser_path;
	std::vector<std::string> patterns ;
};

