#pragma once
///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        :	Ammar,  Syracuse University                          //
//					Yihang Nan, Syracuse University                      //
//                 315-863-0892, ynan02@syr.edu                          //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages. 
*
*  Required Files:
* =======================
*  Converter.h Converter.cpp DependencyTable.h DependencyTable.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 6 Mar 2019
*  - first release
*/

#include "../DependencyTable/DependencyTable.h"
#include "../Parser/Parser.h"
#include <vector>
#include <string>
#include <fstream>
#include <map>

class CodeConverter
{
public:
  CodeConverter();
  CodeConverter(const DependencyTable& dt);

  const std::string& outputDir() const;
  void outputDir(const std::string& dir);
  bool createOutpurDir();

  void setDepTable(const DependencyTable& dt);

  std::vector<std::string> convert();
  std::string convert(const std::string& filepath);
  std::vector<std::string> convert(const std::vector<std::string>& files);
  //void PrintDenpen(std::unordered_map<std::string, std::vector<std::string>> table);
  void getTable_converter(std::unordered_map<std::string, std::vector<std::string>> temp);
  const std::vector<std::string> convertedFiles() const;
  void PrintDenpen();
  void clear();
  void get_file_name(std::string pwd_f);
  void row_class(std::map<int,int> temp_row);
  void row_function(std::map<int, int> temp_func);  
  void docheck_class();
  void docheck_func();

private:
  bool convertFile(std::string file);
  void addPreCodeHTML(const std::string& title);
  void addbutton();
  void addPreTag();
  void docheck_com(std::string line);
  void addDependencyLinks(std::string file);
  void addClosingTags();
  void skipSpecialChars(std::string& line);
  

private:
  DependencyTable dt_;
  std::string outputDir_ = "..\\ConvertedWebpages\\";
  std::vector<std::string> convertedFiles_;
  std::ofstream out_;
  int count_rows = 0;
  std::unordered_map<std::string, std::vector<std::string>> table_read;
  std::string pwd_file;
  std::map<int, int> row;
  std::map<int, int> func;
  int mark_row ;
  int mark_func ;
};

