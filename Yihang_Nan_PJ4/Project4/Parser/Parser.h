#ifndef PARSER_H
#define PARSER_H
/////////////////////////////////////////////////////////////////////
//  Parser.h - Analyzes C++ and C# language constructs             //
//  ver 1.0                                                        //
//  Language:      Visual C++, Visual Studio 2015                  //
//  Platform:      Dell XPS 8920, Windows 10                       //
//  Application:   Prototype for CSE687 - OOD Projects             //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines a Parser class.  Its instances collect 
  semi-expressions from one or more files for analysis.  
  Analysis consists of applying a set of rules to the semi-expression, 
  and for each rule that matches, invoking a set of one or more actions.

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, 
    - ITokenCollection.h, 
    - Semi.h, Semi.cpp,
    - toker.h, toker.cpp,
    - ActionsAndRules.h, ActionsAndRules.cpp,
    - GrammarHelpers.h, GrammarHelpers.cpp,
    - AbstrSynTree.h, AbstrSynTree.cpp,
    - ScopeStack.h, ScopeStack.cpp
    - ConfigureParser.h, ConfigureParser.cpp,
    - FileSystem.h, FileSystem.cpp,
    - Logger.h, Logger.cpp,
    - Utilities.h, Utilities.cpp

  Maintenance History:
  ===================
  ver 1.0 : 11 Feb 2019
  - first release
  - This is a modification of the parser used in CodeAnalyzer.
  - The most important change is replacing its tokenizer with a 
    State Pattern based design

*/

#include <string>
#include <iostream>
#include <vector>
#include "../SemiExpression/ITokenCollection.h"

namespace CodeAnalysis
{
  ///////////////////////////////////////////////////////////////
  // abstract base class for parsing actions
  //   - when a rule succeeds, it invokes any registered action

  class Parser;

  class IBuilder
  {
  public:
    virtual ~IBuilder() {}
    virtual Parser* Build() = 0;
  };

  ///////////////////////////////////////////////////////////////
  // abstract base class for parsing actions
  //   - when a rule succeeds, it invokes any registered action

  class IAction
  {
  public:
    virtual ~IAction() {}
    virtual void doAction(const Lexer::ITokenCollection* pTc) = 0;
  };

  ///////////////////////////////////////////////////////////////
  // abstract base class for parser language construct detections
  //   - rules are registered with the parser for use

  class IRule
  {
  public:
    static const bool Continue = true;
    static const bool Stop = false;
    virtual ~IRule() {}
    void addAction(IAction* pAction);
    void doActions(const Lexer::ITokenCollection* pTc);
    virtual bool doTest(const Lexer::ITokenCollection* pTc) = 0;
  protected:
    std::vector<IAction*> actions;
  };

  class Parser
  {
  public:
    Parser(Lexer::ITokenCollection* pTokCollection);
    ~Parser();
    void addRule(IRule* pRule);
    bool parse();
    bool next();
  private:
    Lexer::ITokenCollection* pTokColl;
    std::vector<IRule*> rules;
  };

  inline Parser::Parser(Lexer::ITokenCollection* pTokCollection) : pTokColl(pTokCollection) {}

  inline Parser::~Parser() {}
}
#endif
