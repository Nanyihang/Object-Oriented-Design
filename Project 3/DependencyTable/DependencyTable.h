#pragma once
///////////////////////////////////////////////////////////////////////////
// DependencyTable.h : defines dependency table structure                //
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
*  This package defines DependencyTable class which is used to hold files
*  relationships after dependency analysis is performed. It is simply a
*  wrapper around std::unordered_map which uses string key and vector
*  value. Therefore, every file in the analysis will point to a vector
*  of all the files it depends upon. 
*
*
*  Required Files:
* =======================
*  DependencyTable.h DependencyTable.cpp Logger.h Logger.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 11 Feb 2019
*  - first release
*/

#include <unordered_map>
#include <string>
#include <vector>

class DependencyTable
{
public:
  using file = std::string;
  using files = std::vector<file>;
  using dependencies = std::vector<file>;
  using iterator = std::unordered_map<file, dependencies>::iterator;


  DependencyTable();
  DependencyTable(const files& list);
  ~DependencyTable();

  bool has(const std::string& file);

  void addDependency(file filename, file dependency);
  void removeDependency(file filename, file dependency);
  std::unordered_map<file, dependencies> get_table();
  void removeEntry(file filename);

  const dependencies& getDependencies(file filename) const;
  const dependencies& operator[](const file& filename) const;

  // these two functions allow 'for' loops in the form of
  // for(auto entry : dt) {...}
  iterator begin();
  iterator end();
  void clear() { table_.clear(); }

  files getFiles() const;

  void display();
  void get_file(std::string pwd_);
private:
  std::unordered_map<file, dependencies> table_;
  std::string pwd_file_1;
};

