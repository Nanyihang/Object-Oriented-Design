#pragma once
////////////////////////////////////////////////////////////////////////
// Process.h - class used to start process                            //
// ver 1.0                                                            //
// Author : Ammar Salman, SU, assalman@syr.edu                        //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018 //
////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a class, Process, used to start named processes.
*  It has a lot of potential, mostly unrealized by this simple beginning.
*
*  Possible future features include:
*  - interprocess communication between parent and child using pipes
*  - handling multiple process, perhaps in a process pool
*  - killing processes
*  - enumerating running processes that match a regular expression
*  - yada, yada, yada
*
*  Required Files:
*  ---------------
*  Process.h, Process.cpp Logger.h Logger.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 11 Feb 2019
*  - moved implementation to CPP
*  - moved string conversion functions to StringUtilities
*  - added Logger use
*  ver 1.0 : 19 Feb 2018
*  - first release
*/

#include <windows.h>
#include <string>
#include <functional>

using CBP = std::function<void(void)>;

class Process
{
public:
  Process();
  virtual ~Process() {}
  bool create(const std::string& path = "");
  void title(const std::string& title);
  void application(const std::string& app);
  void commandLine(const std::string& cmdLine);
  void registerCallback();
  void setCallBackProcessing(CBP cbp);
  void callback();
  HANDLE getProcessHandle();

private:
  STARTUPINFO startInfo_;
  PROCESS_INFORMATION procInfo_;  
  std::wstring title_;
  std::wstring appName_;
  std::wstring commandLine_;
  static CBP cbp_;
};


