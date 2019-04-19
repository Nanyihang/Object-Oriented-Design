#pragma once
/////////////////////////////////////////////////////////////////////
// DirExplorerN.h - Naive directory explorer                       //
// ver 1.3                                                         //
// Author  : Ammar Salman, assalman@syr.edu                        //
// Source  : Jim Fawcett, CSE687 - OOD, Fall 2018                  //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * DirExplorerN provides a class, of the same name, that executes a
 * depth first search of a directory tree rooted at a specified path.
 * Each time it enters a directory, it invokes its member function
 * doDir, and for every file in that directory, it invokes doFile.
 *
 * We call this a "Naive Directory Explorer" because, while it carries
 * out its assigned responsibilities well, there is no way to change
 * what its doDir and doFile functions do, without changing the class
 * itself.  It would be much better to provide a mechanism to allow
 * a using application to supply this processing without changing
 * the Directory Navigator. 
 *
 * Other projects in this solution do just that, in different ways.  

 * - DirExplorer-Naive:
 *     Implements basic processing well, but applications have to
 *     change its code to affect how files and directories are
 *     handled.
 * - DirExplorer-Template:
 *     Applications provide template class parameters to define file
 *      and directory processing.
 * - DirExplorer-Inheritance:
 *     Applications subclass DirExplorerI, overriding virtual functions
 *     doFile, doDir, and doQuit to define how files and directories
 *     are handled, and whether processing should terminate before
 *     visiting all directories.
 * - DirExplorer-Events:
 *     Applications use Event Interfaces, published by DirExplorerE,
 *     by subscribing event-handler functions, and control terminate
 *      processing using a plug-in interface.
 * - DirExplorer-Provider:
 *     Applications implement a file system provider that implements
 *     a Provider interface published by DirExplorerP.
 *     
 * We'll be using this solution to illustrate techniques for building
 * flexible software.
 *
 * Required Files:
 * ---------------
 * DirExplorerN.h, DirExplorerN.cpp
 * FileSystem.h, FileSystem.cpp      // Directory and Path classes
 * StringUtilities.h                 // Title function
 * CodeUtilities.h                   // ProcessCmdLine class
 *
 * Maintenance History:
 * --------------------
 * ver 1.4 : 11 Feb 2019
 * - Added file containers
 *   Added regex matching
 * ver 1.3 : 19 Aug 2018
 * - Removed some options to make this version simple.  Those are
 *   implemented in the more advanced navigators, presented here.
 * ver 1.2 : 17 Aug 2018
 * - Moved method definitions to inlines, below the class declaration.
 * ver 1.1 : 16 Aug 2018
 * - Made no recursion default, added option /s for recursion.
 * ver 1.0 : 11 Aug 2018
 * - first release
 *
*/

#include <vector>
#include <iostream>
#include "../FileSystem/FileSystem.h"

namespace FileSystem
{
  class DirExplorerN
  {
  public:
    using patterns = std::vector<std::string>;
    using files = std::vector<std::string>;

    static std::string version() { return "ver 1.4"; }

    DirExplorerN(const std::string& path);

    // set options for analysis

    void addPattern(const std::string& patt);
    void recurse(bool doRecurse = true);

    // conduct depth first search

    bool search();
    void find(const std::string& path);
    bool match_regexes(const std::vector<std::string>& regexes);

    // define what happens when a file or dir is encountered

    void doFile(const std::string& dir, const std::string& filename);
    void doDir(const std::string& dirname);

    // extract traversal statistics 
    const files& filesList() const; 
    size_t fileCount();
    size_t dirCount();
    void showStats();

  private:
    std::string path_;
    patterns patterns_;
    files files_;
    size_t dirCount_ = 0;
    size_t fileCount_ = 0;
    bool recurse_ = false;
  };

  
}