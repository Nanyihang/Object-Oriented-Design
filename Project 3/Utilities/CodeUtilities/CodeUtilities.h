#pragma once
///////////////////////////////////////////////////////////////////////
// CodeUtilities.h - small, generally useful, helper classes         //
// ver 1.3                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Ammar Salmon, TA, CSE687                             //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* - ProcessCmdLine    extracts path, options, patterns, and a number from command line
* - Converter<T>      converts T to and from strings
* - Box<T>            converts primitive type to instance of a class
* - PersistFactory<T> adds toXml() method to T
*
* Build Process:
* --------------
* Required Files: 
*   CodeUtilities.h
*
* Maintenance History:
* --------------------
* ver 1.3 : 16 Aug 2018
* - added default usage text
* ver 1.2 : 11 Aug 2018
* - added ProcessCmdLine::hasOption method
* - fixed bugs in ProcessCmdLine::showCmdLine
*   and ProcessCmdLine::showOptions
* ver 1.1 : 10 Aug 2018
* - added ProcessCmdLine class
* ver 1.0 : 12 Jan 2018
* - first release
* - refactored from earlier Utilities.h
*
* Notes:
* ------
* - Designed to provide all functionality in header file. 
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <regex>

namespace Utilities
{
  /////////////////////////////////////////////////////////////////////
  // preface function
  // - used to add a string preface to an output, e.g., "\n  "

  inline void preface(const std::string& msg = "", bool doReturn = true, std::ostream& out = std::cout, const std::string& prefix = "  ")
  {
    if (doReturn) out << "\n" << prefix << msg;
  }

  inline std::string defaultUsage()
  {
    std::string usage;
    usage += "\n  Command Line: path [/option]* [/pattern]* [integer]";
    usage += "\n    path is relative or absolute path where processing begins";
    usage += "\n    [/option]* are one or more options of the form /s, /r, etc.";
    usage += "\n    [pattern]* are one or more pattern strings used for matching";
    usage += "\n    [integer] is the maximum number of items to process";
    usage += "\n";
    return usage;
  }

  /////////////////////////////////////////////////////////////////////
  // ProcessCmdLine class
  // - extracts path, patterns, options, number

  enum DisplayMode {
    None,
    Debug,
    Demo
  };

  class ProcessCmdLine
  {
  public:
    using Usage = std::string;
    using Path = std::string;
    using Option = std::string;
    using Options = std::vector<Option>;
    using Pattern = std::string;
    using Patterns = std::vector<Pattern>;
	using Regexes = std::vector<std::string>;
    using Number = long int;

    ProcessCmdLine(int argc, char** argv, std::ostream& out = std::cout);
    ProcessCmdLine(const ProcessCmdLine&) = delete;
    ProcessCmdLine& operator=(const ProcessCmdLine&) = delete;

    bool parseError();
    Path path();
    void path(const Path& path);
    Options options();
    void option(Option op);
    bool hasOption(Option op);
    Patterns patterns();
	Regexes regexes();
    void pattern(const Pattern& patt);
    Number maxItems();
    void maxItems(Number number);
    void usage(const Usage& usage);
    void usage();
    void showCmdLine(int argc, char** argv);
    void showCmdLine();
    void showPath();
    void showOptions();
    void showPatterns();
	  void showRegexes();
    void showMaxItems();
    DisplayMode displayMode();

  private:
	  bool isValidRegex(const std::string& regex);
  private:
    Usage usage_;
    Path path_;
    Patterns patterns_;
	  Regexes regexes_;
    Options options_;
    int maxItems_ = 0;
    bool parseError_ = false;
    std::ostream& out_;
    DisplayMode dm_ = DisplayMode::None;
  };

  /*----< path operations >------------------------------------------*/

  inline void ProcessCmdLine::path(const Path& path) 
  { 
    path_ = path; 
  }

  inline ProcessCmdLine::Path ProcessCmdLine::path()
  {
    return path_;
  }

  inline void ProcessCmdLine::showPath()
  {
    out_ << path_ << " ";
  }

  /*----< options operations >---------------------------------------*/

  inline void ProcessCmdLine::option(Option option)
  {
    options_.push_back(option);
  }

  inline ProcessCmdLine::Options ProcessCmdLine::options()
  { 
    return options_; 
  }

  inline bool ProcessCmdLine::hasOption(Option op)
  {
    for (auto item : options_)
    {
      if (item == op)
      {
        return true;
      }
    }
    return false;
  }

  inline void ProcessCmdLine::showOptions()
  {
    for (auto opt : options_)
    {
      out_ << '/' << opt << " ";
    }
  }

  /*----< patterns operations >--------------------------------------*/

  inline void ProcessCmdLine::pattern(const Pattern& pattern)
  {
    patterns_.push_back(pattern);
  }

  inline ProcessCmdLine::Patterns ProcessCmdLine::patterns()
  {
    return patterns_;
  }

  inline ProcessCmdLine::Regexes ProcessCmdLine::regexes()
  {
	  return regexes_;
  }

  inline void ProcessCmdLine::showPatterns()
  {
    for (auto patt : patterns_)
    {
      out_ << patt << " ";
    }
  }

  inline void ProcessCmdLine::showRegexes()
  {
	  for (auto regex : regexes_)
	  {
		  out_ << regex << " ";
	  }
  }

  /*----< maxItems operations >--------------------------------------*/

  inline void ProcessCmdLine::maxItems(Number maxItems)
  {
    maxItems_ = maxItems;
  }

  inline ProcessCmdLine::Number ProcessCmdLine::maxItems()
  {
    return maxItems_;
  }

  inline void ProcessCmdLine::showMaxItems()
  {
    if(maxItems_ != 0)
      out_ << maxItems_ << " ";
  }

  inline DisplayMode ProcessCmdLine::displayMode()
  {
    return dm_;
  }

  inline bool ProcessCmdLine::isValidRegex(const std::string & regex)
  {
	  bool res = true;
	  try
	  {
		  std::regex tmp(regex);
	  }
	  catch (const std::regex_error& e)
	  {
		  (e);
		  res = false;
	  }
	  return res;
  }

  /*----< parseError operation >-------------------------------------*/

  inline bool ProcessCmdLine::parseError()
  {
    return parseError_;
  }

  /*----< command line operations >----------------------------------*/

  inline ProcessCmdLine::ProcessCmdLine(int argc, char** argv, std::ostream& out) : out_(out)
  {
    //usage_ = defaultUsage();
    if (argc < 2)
    {
      out << "\n  command line parse error";
      //usage();
      parseError_ = true;
      return;
    }

    path_ = argv[1];

    for (int i = 2; i < argc; ++i)
    {
      if (argv[i][0] == '/')
      {
        std::string opt(argv[i]);
        opt = opt.substr(1);
        if (opt == "demo")
          dm_ = DisplayMode::Demo;
        else if (opt == "debug")
          dm_ = DisplayMode::Debug;
        options_.push_back(opt);
      }
      else
      {
        int number = atoi(argv[i]);
        if (number > 0)
        {
          maxItems_ = number;
        }
        else if (isValidRegex(argv[i]))
        {
          regexes_.push_back(argv[i]);
        }
        else
        {
          patterns_.push_back(argv[i]);
        }
      }
    }

    // after processing, if both display modes are given, put parse error
    if (hasOption("demo") && hasOption("debug"))
      parseError_ = true;
  }

  inline void ProcessCmdLine::showCmdLine(int argc, char** argv)
  {
    if (argc > 2)
    {
      out_ << "  empty";
      return;
    }
    for (int i = 0; i < argc; ++i)
    {
      if (argv[i] == "0")
        continue;
      if(argv[i][0] == '/')
      { 
        out_ << "/" + char(argv[i][1]) << " ";
      }
      else
      {
        out_ << argv[i] << " ";
      }
    }
  }

  inline void ProcessCmdLine::showCmdLine()
  {
    if (path_ == "" && options_.size() == 0 && patterns_.size() == 0)
    {
      out_ << "empty";
      return;
    }
    showPath();
    preface("", false);
    showOptions();
    preface("", false);
    showPatterns();
    preface("", false);
    showMaxItems();
  }

  inline void ProcessCmdLine::usage(const Usage& usage)
  {
    usage_ = usage;
  }

  inline void ProcessCmdLine::usage()
  {
    out_ << usage_;
  }

  /////////////////////////////////////////////////////////////////////
  // Converter class
  // - supports converting unspecified types to and from strings
  // - a type is convertible if it provides insertion and extraction
  //   operators

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };
  //----< convert t to a string >--------------------------------------

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }
  //----< convert a string to an instance of T >-----------------------
  /*
  *  - the string must have been generated by Converter<T>::toString(const T& t)
  *  - T::operator>> must be the inverse of T::operator<< 
  */
  template<typename T>
  T Converter<T>::toValue(const std::string& src)
  {
    std::istringstream in(src);
    T t;
    in >> t;
    return t;
  }

  ///////////////////////////////////////////////////////////////////////
  // Box class
  // - wraps primitive type in class
  // - preserves primitive syntax

  template<typename T>
  class Box
  {
  public:
    Box() : primitive_(T()) {}
    Box(const T& t) : primitive_(t) {}
    operator T&() { return primitive_; }
    T& operator=(const T& t) { primitive_ = t; return primitive_; }
  private:
    T primitive_;
  };

  ///////////////////////////////////////////////////////////////////////
  // ToXml interface
  // - defines language for creating XML elements

  struct ToXml
  {
    virtual std::string toXml(const std::string& tag) = 0;
    virtual ~ToXml() {};
  };

  ///////////////////////////////////////////////////////////////////////
  // PersistFactory<T> class
  // - wraps an instance of user-defined type
  // - preserves semantics of user-defined type
  // - adds toXml("tag") method

  template<typename T>
  class PersistFactory : public T, ToXml
  {
  public:
    PersistFactory() = default;
    PersistFactory(const T& t)
    {
      T::operator=(t);
    }
    std::string toXml(const std::string& tag)
    {
      std::ostringstream out;
      out << "<" << tag << ">" << *this << "</" << tag << ">";
      return out.str();
    }
  };
}