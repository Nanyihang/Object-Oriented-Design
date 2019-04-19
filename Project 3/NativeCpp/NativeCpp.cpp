/////////////////////////////////////////////////////////////////
// NativeCpp.cpp - Translates managed calls into native calls  //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019   //
// Author        :	Yihang Nan, Syracuse University                //
/////////////////////////////////////////////////////////////////
/*
*  This file defines a private class, NativeCpp, that implements the
*  INativeCpp interface.  Clients only need to use the createNativeCpp
*  factory function and the interface pointer it returns.
*/
#include "INativeCpp.h"
#include "../CodePublisher/CodePublisher.h"
#include <iostream>

class NativeCpp : public INativeCpp
{
public:
	//--------------<put string and get all files we need>--------------
  virtual void putString(const std::string& str) override
  {
    str_ = str;
	get_all_files.push_back(str_);
  }
  //---------------<get string >--------------------------
  virtual std::string getString() override
  {
    return str_;
  }
  virtual std::vector<double> convertToSquares(const std::vector<double>& vecDble) override
  {
    std::vector<double> squares;
    for (double item : vecDble)
      squares.push_back(item*item);
    return squares;
  }
  //-------<get file converted and display >--------------
  virtual void transfer_file_code()
  {
	  CodePublisher exec;
	  exec.add_files_fromGUI(get_all_files);
	  exec.publish();
  }
  //-----------<get pattern test>---------------------
  virtual std::string get_patterns(const std::string& str)
  {
	  CodePublisher exec;
	  //exec.processCommandLineArgs(argc, argv);
	  return("111");
  }
  /*<-----------Method for getting back patterns-------------->*/
  virtual std::vector<std::string> getPattern(int argc, std::vector<std::string>& vec) override
  {
	  char** argv = new char* [argc];
	  for (int i = 0; i < argc; i++)
	  {
		  argv[i] = (char*)vec[i].c_str();
	  }
	  CodePublisher exec;
	  exec.processCommandLineArgs(argc, argv);
	  exec.extractFiles();

	  return exec.get_patterns();
  }
private:
  std::string str_;
  std::vector<std::string> get_all_files;
};

///////////////////////////////////////////////////////////
// Factory function for NativeCpp

INativeCpp* createNativeCpp()
{
  return new NativeCpp();
}

///////////////////////////////////////////////////////////
// This simple class doesn't need a test stub
//
//int main()
//{
//    std::cout << "Hello World!\n"; 
//}
