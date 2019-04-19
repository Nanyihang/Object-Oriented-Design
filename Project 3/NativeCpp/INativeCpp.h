#pragma once
///////////////////////////////////////////////////////////////
// INativeCpp.h - Translates managed calls into native calls //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019 //
// Author        :	Yihang Nan, Syracuse University                //
///////////////////////////////////////////////////////////////
/*
*  This package provides an interface for the NativeCpp class
*  and a declaration of an object factory function.  Using these
*  isolates the user from all implementation details in NativeCpp.
*
*  That will be important for projects #3 and #4.  For example,
*  in Project #4 the WPF GUI needs to use services of the Comm
*  channel.  Comm uses native threads, but neither C# nor C++\CLI
*  can compile native threads, so this isolation is crucial.
*
*  NativeCpp doesn't do anything useful, other than illustrating
*  how to call native C++ methods from C# via a C++\CLI translator.
*
*  The easiest way to use this package is to build it as a static
*  library and the Translator project makes a reference to this
*  project.
*/
#include <string>
#include <vector>

struct INativeCpp
{
  virtual ~INativeCpp() {}
  virtual void putString(const std::string& str) = 0;
  virtual std::string getString() = 0;
  virtual std::vector<double> convertToSquares(const std::vector<double>& vecDble) = 0;
  virtual void transfer_file_code() = 0;
  virtual std::string get_patterns(const std::string& str) = 0;
  virtual std::vector<std::string> getPattern(int argc, std::vector<std::string>& vec) = 0;

};

INativeCpp* createNativeCpp();
