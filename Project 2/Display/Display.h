#pragma once
///////////////////////////////////////////////////////////////////////////
// Display.h     : defines webpage display using browser functions       //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Ammar Salman, Syracuse University                     //
//                 313/788-4694, assalman@syr.edu                        //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines Display class which accepts a list of files as a
*  vector<string> and uses the default internet browser to display them
*  one by one. Please note that the functionality has limiations:
*   1) Opera/MS Edge: will pop-up all tabs instantly. 
*   2) Chrome/Firefox: will pop-up windows separately only if no
*      already existing Chrome/Firefox window is opened (all must be
*      closed before running this). 
*
*  Required Files:
* =======================
*  Display.h Display.cpp Process.h Process.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 Feb 2019
*  - first release
*/


#include <vector>
#include <string>
#include <unordered_map>

class Display
{
public:
  Display();
  
  void display(const std::string& file);
  void display(const std::vector<std::string>& files);
};

