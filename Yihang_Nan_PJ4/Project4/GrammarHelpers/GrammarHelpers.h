#ifndef GRAMMARHELPERS_H
#define GRAMMARHELPERS_H
/////////////////////////////////////////////////////////////////////////
// GrammarHelpers.h - Functions providing base grammatical analyses    //
// ver 1.4                                                             //
// Language:    C++, Visual Studio 2015                                //
// Application: Support for Parsing, CSE687 - Object Oriented Design   //
// Author:      Jim Fawcett, Syracuse University, CST 4-187            //
//              jfawcett@twcny.rr.com                                  //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a single class, GrammarHelper, with all static methods.
* Each method provides a useful piece of analysis on a Semiression, designed
* to support source code parsing activities.
*
* The purpose of this package is to factor out of the ActionsAndRules package
* some complex processing so it gets implemented in only one place.  It also
* factors a lot of grammar processing into small testable chunks.
*
* Build Process:
* --------------
* Required Files:
*   GrammarHelpers.h, GrammarHelpers.cpp,
*   Semiression.h, Semiression.cpp, Tokenizer.h, Tokenizer.cpp,
*   Logger.h, Logger.cpp, Utilities.h, Utilities.cpp
*
* Build Command: devenv Analyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.4 : 07 Jun 2017
* - temporarily disabled finding C# lambdas in isFunction
* ver 1.3 : 26 Feb 2017
* - modified isFunctionDeclaration to support a bug fix in ActionsAndRules
*   associated with failure to detect some public data declarations
* Ver 1.2 : 28 Aug 2016
* - fixed errors found in last round of testing
* Ver 1.1 : 25 Aug 2016
* - added many new functions
* - changed some of the function signatures
* Ver 1.0 : 10 Aug 2016
* - first release
*
* Planned Additions and Changes:
* ------------------------------
* - These functions may result in scanning a Semi instance several times.
*   It should be straight forward to reduce some of that.
*/


#include <string>
#include "../SemiExpression/ITokenCollection.h"

namespace CodeAnalysis
{
  class GrammarHelper
  {
  public:
    static bool isControlKeyWord(const std::string& tok);
    static bool hasControlKeyWord(const Lexer::ITokenCollection& se);
    static bool isFunction(const Lexer::ITokenCollection& se);
    static bool hasArgs(const Lexer::ITokenCollection& se);
    static bool isFunctionDefinition(const Lexer::ITokenCollection& se);
    static bool isFunctionDeclaration(const Lexer::ITokenCollection& se, const std::string& parentType);
    static bool isFunctionInvocation(const Lexer::ITokenCollection& se, const std::string& parentType);
    static bool hasFunctionInvocation(const Lexer::ITokenCollection& se, const std::string& parentType);
    static bool isDataDeclaration(const Lexer::ITokenCollection& se);
    static bool isExecutable(const Lexer::ITokenCollection& se, const std::string& parentType);
    static size_t findLast(const Lexer::ITokenCollection& se, const std::string& token);
    static bool isQualifierKeyWord(const std::string& tok);
    static void removeQualifiers(Lexer::ITokenCollection& tc);
    static void removeCallingArgQualifiers(Lexer::ITokenCollection& tc);
    static bool isFirstArgDeclaration(const Lexer::ITokenCollection& tc, const std::string& parentType);
    static bool hasReturnType(const Lexer::ITokenCollection& tc);
    static void removeCppInitializers(Lexer::ITokenCollection& tc);
    static void removeCSharpInitializers(Lexer::ITokenCollection& tc);
    static void removeComments(Lexer::ITokenCollection& tc);
    static void condenseTemplateTypes(Lexer::ITokenCollection& tc);
    static void removeFunctionArgs(Lexer::ITokenCollection& tc);
    static void showParse(const std::string& msg, const Lexer::ITokenCollection& se, bool isResult = false);
    static void showParseDemo(const std::string& msg, const Lexer::ITokenCollection& se);
  };
}
#endif
