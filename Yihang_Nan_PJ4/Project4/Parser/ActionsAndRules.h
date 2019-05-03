#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 3.5                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, Semiression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Required files
  ==============
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ITokenCollection.h, Semiression.h, Semiression.cpp, tokenizer.h, tokenizer.cpp

  Maintenance History:
  ====================
  ver 3.5 : 11 Feb 2019
  - small changes spread over many of the Rule and Action classes, caused
    by changing the lexical scanner, which forced some changes to the
    scanner's interface (sigh!)
  ver 3.4 : 28 May 2017
  - Added several tests for scopestack size in actions to fix exceptions
    thrown when processing code that won't compile.
  ver 3.3 : 26 Feb 2017
  - Fixed bug in public data analysis with changes to rule CppDeclaration
    and its action HandleCppDeclaration.
  - Fixed a minor bug in name collection for operators.  Won't be important
    for dependency analysis.
  ver 3.2 : 28 Aug 16
  - fixed errors in many rules and actions based on lots of testing
  - cleaned up text, removing comments and improving prologues
  ver 3.1 : 23 Aug 16
  - qualified input pointers in rules and actions as const
  - cleaned up code by removing unreachables and commented code, and by simplifying 
  ver 3.0 : 06 Aug 16
  - Added use of AbstrSynTree
  - Added new rules and actions
  - Renamed and modified most of the other actions and rules
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and Semi with Lexer namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokenCollection instead
    of a Semiression
  ver 1.0 : 12 Jan 06
  - first release

  Planned Changes:
  ================
  C++ lambda detection needs strengthening
*/
//
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
#include "Parser.h"
#include "../GrammarHelpers/GrammarHelpers.h"
#include "../SemiExpression/ITokenCollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

namespace CodeAnalysis
{  
  ///////////////////////////////////////////////////////////////////
  // Repository instance is used to share resources
  // among all actions.

  enum Language { C /* not implemented */, Cpp, CSharp };

  class Repository  // application specific
  {
  public:
    using Rslt = Logging::StaticLogger<0>;  // use for application results
    using Demo = Logging::StaticLogger<1>;  // use for demonstrations of processing
    using Dbug = Logging::StaticLogger<2>;  // use for debug output
    using Package = std::string;
    using Path = std::string;

  private:
    Language language_ = Language::Cpp;
    Path path_;
    ScopeStack<ASTNode*> stack;
    AbstrSynTree ast;
    ASTNode* pGlobalScope;
    Package package_;
    Lexer::Toker* p_Toker;
    Access currentAccess_ = Access::publ;
    static Repository* instance;
  
  public:
    
    Repository(Lexer::Toker* pToker) : ast(stack)
    {
      p_Toker = pToker;
      pGlobalScope = stack.top();  // installed in AST constructor
      instance = this;
    }

    ~Repository()
    {
      Dbug::write("\n  deleting repository");
    }

    Language& language() { return language_; }

    Package& package() { return package_; }

    Path& currentPath() { return path_; }

    Access& currentAccess() { return currentAccess_; }

    static Repository* getInstance() { return instance; }

    ScopeStack<ASTNode*>& scopeStack() { return stack; }

    AbstrSynTree& AST() { return ast; }

    ASTNode* getGlobalScope() { return pGlobalScope; }

    Lexer::Toker* Toker() { return p_Toker; }

    size_t lineCount() 
    { 
      return (size_t)(p_Toker->lineCount()); 
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect beginning of scope

  class BeginScope : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test begin scope", *pTc);

      // don't parse Semi with single semicolon token

      if (pTc->size() == 1 && (*pTc)[0] == ";")
        return IRule::Stop;

      if (pTc->contains("{"))
      {
        doActions(pTc);
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to handle scope stack at beginning of scope

  class HandleBeginScope : public IAction
  {
    Repository* p_Repos;
  public:
    HandleBeginScope(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("handle begin scope", *pTc);

      ASTNode* pElem = new ASTNode;
      pElem->type_ = "anonymous";
      pElem->name_ = "none";
      pElem->package_ = p_Repos->package();
      pElem->startLineCount_ = p_Repos->lineCount();
      pElem->endLineCount_ = 1;
      pElem->path_ = p_Repos->currentPath();
      /*
       * make this ASTNode child of ASTNode on stack top
       * then push onto stack
       */
      p_Repos->AST().add(pElem);
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect end of scope

  class EndScope : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test end scope", *pTc);

      size_t posCloseBrace;
      pTc->find("}", posCloseBrace);
      if (posCloseBrace < pTc->size())
      {
        doActions(pTc);
        return IRule::Stop;
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to handle scope stack at end of scope

  class HandleEndScope : public IAction
  {
    Repository* p_Repos;
  public:
    using Dbug = Logging::StaticLogger<2>;

    HandleEndScope(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle end scope", *pTc);

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* pElem = p_Repos->AST().pop();
      if (pElem == nullptr)
        return;

      pElem->endLineCount_ = p_Repos->lineCount();
      if (pElem->type_ == "class" || pElem->type_ == "struct")
        (pElem->endLineCount_)++;

      p_Repos->currentAccess() = Access::priv;
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect access for C++

  class DetectAccessSpecifier : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test access spec", *pTc);

      Repository* pRepo = Repository::getInstance();
      if (pRepo->language() != Language::Cpp)
        return IRule::Continue;

      size_t pos;
      pTc->find(":", pos);
      if (0 < pos && pos < pTc->size())
      {
        const std::string tok = (*pTc)[pos - 1];
        if (tok == "public" || tok == "protected" || tok == "private")
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to handle access specifier

  class HandleAccessSpecifier : public IAction
  {
    Repository* p_Repos;

  public:
    HandleAccessSpecifier(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle access spec: ", *pTc);

      std::string tok = (*pTc)[pTc->size() - 2];
      Access& access = p_Repos->currentAccess();
      if (tok == "public")
        access = Access::publ;
      else if (tok == "protected")
        access = Access::prot;
      else
        access = Access::priv;
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect preprocessor statements

  class PreprocStatement : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test preproc statement: ", *pTc);

      size_t pPos;
      pTc->find("#", pPos);
      if (pPos < pTc->size())
      {
        doActions(pTc);
        return IRule::Stop;
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add Semi to scope stack top statements_

  class HandlePreprocStatement : public IAction
  {
    Repository* p_Repos;

  public:
    HandlePreprocStatement(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle preproc statement: ", *pTc);

      Lexer::ITokenCollection* pClone = pTc->clone();
      if (p_Repos->scopeStack().size() == 0)
        return;
      ASTNode* pElem = p_Repos->scopeStack().top();
      pElem->statements_.push_back(pClone);

      GrammarHelper::showParse("Preproc Stmt", *pTc);
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect namespace statements

  class NamespaceDefinition : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test namespace definition: ", *pTc);

      const Lexer::ITokenCollection& tc = *pTc;
      if (tc[tc.size() - 1] == "{")
      {
        size_t len;
        tc.find("namespace", len);
        if (len < tc.size())
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add namespace info to scope stack top

  class HandleNamespaceDefinition : public IAction
  {
    Repository* p_Repos;

  public:
    HandleNamespaceDefinition(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle namespace definition: ", *pTc);

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* top = p_Repos->scopeStack().top();

      size_t pos;
      pTc->find("namespace", pos);
      std::string name = (*pTc)[pos + 1];
      top->type_ = "namespace";
      top->name_ = name;
      top->package_ = p_Repos->package();

      GrammarHelper::showParse("namespace def", *pTc);
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect class statements

  class ClassDefinition : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test class definition: ", *pTc);

      const Lexer::ITokenCollection& tc = *pTc;
      if (tc[tc.size() - 1] == "{")
      {
        size_t len;
        tc.find("class", len);
        if (len < tc.size())
        {
          doActions(pTc);
          return IRule::Stop;
        }
        if (tc.contains("interface"))
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add class info to scope stack top

  class HandleClassDefinition : public IAction
  {
    Repository* p_Repos;

  public:
    HandleClassDefinition(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle class definition: ", *pTc);

      p_Repos->currentAccess() = Access::priv;

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* top = p_Repos->scopeStack().top();
      size_t typeIndex;
      pTc->find("class", typeIndex);
      if (typeIndex < pTc->size())
      {
        size_t nameIndex = typeIndex + 1;
        std::string name = (*pTc)[nameIndex];
        top->type_ = "class";
        top->name_ = name;
        top->package_ = p_Repos->package();
        p_Repos->AST().typeMap()[name] = top;
        GrammarHelper::showParse("class def", *pTc);
      }
      else  // C#
      {
        pTc->find("interface", typeIndex);
        size_t nameIndex = typeIndex + 1;
        std::string name = (*pTc)[nameIndex];
        top->type_ = "interface";
        top->name_ = name;
        top->package_ = p_Repos->package();
        p_Repos->AST().typeMap()[name] = top;
        GrammarHelper::showParse("interface def", *pTc);
      }
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect struct statements

  class StructDefinition : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test struct definition: ", *pTc);

      const Lexer::ITokenCollection& tc = *pTc;
      if (tc[tc.size() - 1] == "{")
      {
        size_t len;
        tc.find("struct", len);
        if (len < tc.size())
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add struct info to scope stack top

  class HandleStructDefinition : public IAction
  {
    Repository* p_Repos;

  public:
    HandleStructDefinition(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle struct definition: ", *pTc);

      p_Repos->currentAccess() = Access::publ;

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* top = p_Repos->scopeStack().top();

      size_t pos;
      pTc->find("struct", pos);
      std::string name = (*pTc)[pos + 1];
      top->type_ = "struct";
      top->name_ = name;
      top->package_ = p_Repos->package();
      p_Repos->AST().typeMap()[name] = top;

      GrammarHelper::showParse("struct def", *pTc);
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect C++ function definitions

  class CppFunctionDefinition : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      Repository* pRepo = Repository::getInstance();
      if (pRepo->language() != Language::Cpp)
        return IRule::Continue;

      GrammarHelper::showParseDemo("Test C++ function definition: ", *pTc);

      ScopeStack<ASTNode*>& stack = pRepo->scopeStack();
      if (stack.size() < 2)
        return IRule::Continue;

      std::string parentType = stack.predOfTop()->type_;
      if (parentType != "namespace" && parentType != "class" && parentType != "struct")
        return IRule::Continue;

      const Lexer::ITokenCollection& tc = *pTc;

      if (tc[tc.size() - 1] == "{")
      {
        if (GrammarHelper::isFunction(*pTc))
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add function info to scope stack top

  class HandleCppFunctionDefinition : public IAction
  {
    Repository* p_Repos;

  public:
    HandleCppFunctionDefinition(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle C++ function definition: ", *pTc);

      ScopeStack<ASTNode*>& stack = p_Repos->scopeStack();
     
      if (stack.size() == 0)
        return;

      ASTNode* top = stack.top();

      size_t nameIndex;
      if(pTc->find("(", nameIndex))
      --nameIndex;
      else return;
      std::string name = (*pTc)[nameIndex];

      // is function a destructor?

      if (nameIndex > 0 && (*pTc)[nameIndex - 1] == "~")
      {
        --nameIndex;
        name = "~" + name;
      }

      top->type_ = "function";
      top->name_ = name;
      top->package_ = p_Repos->package();
      GrammarHelper::showParse("function def", *pTc);

      // is function an operator?

      size_t operIndex;
      pTc->find("operator", operIndex);
      if (operIndex < pTc->size())
      {
        name = "operator" + (*pTc)[operIndex + 1];// +(*pTc)[operIndex + 2];
        if ((*pTc)[operIndex + 2] != "(")
          name += (*pTc)[operIndex + 2];
        top->name_ = name;
        nameIndex = operIndex;
      }
      // is function a member of a class or struct?

      if (nameIndex > 1 && (*pTc)[nameIndex - 1] == "::")
      {
        //----< start find class name >--------------------

        std::string className = (*pTc)[nameIndex - 2];

        // is class a template?

        if (className == ">")
        {
          size_t startParam = GrammarHelper::findLast(*pTc, "<");
          if (0 < startParam && startParam < pTc->size())
            className = (*pTc)[startParam - 1];
        }
        //----< end find class name >----------------------
        /*
        * - this function's ASTNode is at stack top
        * - find ASTNode of function's class
        * - unlink function ASTNode from stack top predecessor
        * - relink function ASTNode to it's class ASTNode
        * - leave function ASTNode on stack top as it may have child nodes
        */
        ASTNode* pClassNode = p_Repos->AST().find(className);
        if (pClassNode == nullptr)
          return;
        if (p_Repos->scopeStack().size() < 2)
          return;

        ASTNode* pFunctNode = p_Repos->scopeStack().top();
        ASTNode* pParentNode = p_Repos->scopeStack().predOfTop();
        if(pParentNode->children_.size() > 0)
          pParentNode->children_.pop_back();           // unlink function
        pFunctNode->parentType_ = pClassNode->type_;
        pClassNode->children_.push_back(pFunctNode);   // relink function
        return;
      }
      // is this a lambda?

      std::string packageName = p_Repos->package();
      std::string ext = FileSystem::Path::getExt(packageName);

      size_t posOpenBracket;
      pTc->find("[", posOpenBracket);
      size_t posCloseBracket;
      pTc->find("]", posCloseBracket);
      size_t posBrace;
      pTc->find("{", posBrace);
      size_t posOpenParen;
      pTc->find("(", posOpenParen);
      if (posCloseBracket != posBrace + 1 && posCloseBracket != posOpenParen + 1) // []{ or [](
        return;
      if (posOpenBracket < posCloseBracket && posBrace == (posCloseBracket + 1) && posBrace < pTc->size())
      {
        std::string name;
        for (size_t i = posOpenBracket; i < posBrace; ++i)
          name += (*pTc)[i];
        top->name_ = name;
        top->type_ = "lambda";
      }
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect C# function definitions

  class CSharpFunctionDefinition : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      Repository* pRepo = Repository::getInstance();
      if (pRepo->language() != Language::CSharp)
        return IRule::Continue;

      GrammarHelper::showParseDemo("Test C# function definition: ", *pTc);

      const Lexer::ITokenCollection& tc = *pTc;
      if (tc[tc.size() - 1] == "{")
      {
        Lexer::Semi se;
        for (size_t i = 0; i < tc.size(); ++i)
          se.add(tc[i]);

        if (GrammarHelper::isFunction(se))
        {
          doActions(&se);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add function info to scope stack top

  class HandleCSharpFunctionDefinition : public IAction
  {
    Repository* p_Repos;

  public:
    HandleCSharpFunctionDefinition(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle C# function definition: ", *pTc);

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* top = p_Repos->scopeStack().top();

      size_t nameIndex;
      if (pTc->find("(", nameIndex))
        --nameIndex;
      else
        return;

      std::string name = (*pTc)[nameIndex];

      // is function a destructor?

      if (nameIndex > 0 && (*pTc)[nameIndex - 1] == "~")
      {
        --nameIndex;
        name = "~" + name;
      }

      top->type_ = "function";
      top->name_ = name;
      top->package_ = p_Repos->package();

      GrammarHelper::showParse("function def", *pTc);

      // is function an operator?

      size_t operIndex;
      pTc->find("operator", operIndex);
      if (operIndex < pTc->size())
      {
        name = "operator" + (*pTc)[operIndex + 1] + (*pTc)[operIndex + 2];
        top->name_ = name;
        nameIndex = operIndex;
      }

      // is lambda?

      size_t posParen;
      pTc->find("(", posParen);
      size_t posBrace;
      pTc->find("{", posBrace);
      size_t posEqual;
      pTc->find("=", posEqual);
      if (posParen < posBrace && posBrace < pTc->size())
      {
        if (posEqual < pTc->size() - 1 && (*pTc)[posEqual + 1] == ">")
        {
          std::string name;
          for (size_t i = posParen; i <= posBrace; ++i)
            name += (*pTc)[i];
          top->name_ = name;
          top->type_ = "lambda";
        }
      }
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect control definitions

  class ControlDefinition : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test control definition: ", *pTc);

      const Lexer::ITokenCollection& tc = *pTc;
      if (tc[tc.size() - 1] == "{")
      {
        size_t len;
        tc.find("(", len);
        if (len < tc.size() && GrammarHelper::isControlKeyWord(tc[len - 1]))
        {
          doActions(pTc);
          return IRule::Stop;
        }
        else if (tc.size() > 1 && GrammarHelper::isControlKeyWord(tc[tc.size() - 2]))
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add control info to scope stack top

  class HandleControlDefinition : public IAction
  {
    Repository* p_Repos;

  public:
    HandleControlDefinition(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle control definition: ", *pTc);

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* top;
      if ((p_Repos->scopeStack()).size() > 0)
        top = p_Repos->scopeStack().top();
      else
        return;

      size_t nameIndex;
      if(pTc->find("(", nameIndex))         // if, for, while, switch, catch
        --nameIndex;
      //if (nameIndex == pTc->size() - 1)     // do, try - they don't have parens
      else
        nameIndex = pTc->size() - 2;
      std::string name = (*pTc)[nameIndex];
      top->type_ = "control";
      top->name_ = name;

      GrammarHelper::showParse("control def", *pTc);
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to send semi-expression that starts a function def
  // to console

  class PrintFunction : public IAction
  {
    Repository* p_Repos;
  public:
    using Rslt = Logging::StaticLogger<0>;

    PrintFunction(Repository* pRepos)
    {
      p_Repos = pRepos;
    }
    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      std::ostringstream out;
      //out << "\n  FuncDef: " << pTc->show().c_str();
      Rslt::write(out.str());
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to send signature of a function def to console

  class PrettyPrintFunction : public IAction
  {
    Repository* p_Repos;
  public:
    using Rslt = Logging::StaticLogger<0>;

    PrettyPrintFunction(Repository* pRepos) : p_Repos(pRepos) {}

    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      size_t len;
      pTc->find(")", len);

      std::ostringstream out;
      out << "\n\n  Pretty Stmt:    ";
      for (size_t i = 0; i < len + 1; ++i)
        out << (*pTc)[i] << " ";
      out << "\n";
      Rslt::write(out.str());
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect C++ Declaration
  /*
  *  - Declaration ends in semicolon
  *  - has type, name, modifiers & initializers
  *  So:
  *  - strip off modifiers and initializers
  *  - if you have two things left it's a declar, else executable
  */
  class CppDeclaration : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      Repository* pRepo = Repository::getInstance();
      if (pRepo->language() != Language::Cpp)
        return IRule::Continue;

      GrammarHelper::showParseDemo("Test C++ declaration: ", *pTc);

      Lexer::Semi tc;
      tc.clone(*pTc);

      // begin added 2/26/2017

      Access access = pRepo->currentAccess();
      bool isPublic = false;

      if (pRepo->scopeStack().size() == 0)
        return false;

      std::string parentType = pRepo->scopeStack().top()->type_;

      size_t posPrivate;
      pTc->find("private", posPrivate);
      if (posPrivate < pTc->size())
      {
        isPublic = false;
        pRepo->currentAccess() = Access::priv;
      }

      size_t posProtected;
      pTc->find("protected", posProtected);
      if (posProtected < pTc->size())
      {
        isPublic = false;
        pRepo->currentAccess() = Access::prot;
      }

      size_t posPublic;
      pTc->find("public", posPublic);
      if (posPublic < pTc->size() && parentType != "function")
      {
        isPublic = true;
        pRepo->currentAccess() = Access::publ;
      }

      // end added 2/26/2017

      if (tc.size() > 0 && tc[0] == "using")
      {
        doActions(pTc);
        return IRule::Stop;
      }

      if (tc[tc.size() - 1] == ";" && tc.size() > 2)
      {
        std::string nextToLast = tc[tc.size() - 2];
        if (nextToLast == "delete" || nextToLast == "default" || nextToLast == "const")
        {
          {
            // function declaration
            doActions(pTc);
            return IRule::Stop;
          }
        }
        std::string parentType = pRepo->scopeStack().top()->type_;
        
        if (GrammarHelper::isDataDeclaration(tc) || GrammarHelper::isFunctionDeclaration(tc, parentType))
        {
          doActions(pTc);
          return IRule::Stop;
        }

        if (parentType != "function")
        {
          // can't be executable so must be declaration

          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add declaration info to scope stack top

  class HandleCppDeclaration : public IAction
  {
    Repository* p_Repos;

  public:
    HandleCppDeclaration(Repository* pRepos) : p_Repos(pRepos) {}

    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle C++ declaration: ", *pTc);

      // save declaration info in ASTNode

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* pCurrNode = p_Repos->scopeStack().top();
      DeclarationNode declNode;
      declNode.access_ = p_Repos->currentAccess();
      declNode.pTc = pTc->clone();
      declNode.package_ = p_Repos->package();
      declNode.line_ = p_Repos->lineCount();

      Lexer::Semi se;
      se.clone(*pTc);
      GrammarHelper::removeComments(se);

      if (se[0] == "using")
      {
        declNode.declType_ = DeclType::usingDecl;
        pCurrNode->decl_.push_back(declNode);

        GrammarHelper::showParse("using declar", *pTc);
        return;
      }

      if (GrammarHelper::isFunctionDeclaration(se,"parentNotFunction"))
      {
        return;
      }

      if (GrammarHelper::isDataDeclaration(se))
      {
        declNode.declType_ = DeclType::dataDecl;
        pCurrNode->decl_.push_back(declNode);
        GrammarHelper::showParse("data declar", *pTc);
      }
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect C# Declaration
  /*
  *  - Declaration ends in semicolon
  *  - has type, name, modifiers & initializers
  *  So:
  *  - strip of modifiers and initializers
  *  - if you have two things left it's a declar, else executable
  */
  class CSharpDeclaration : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      Repository* pRepo = Repository::getInstance();
      if (pRepo->language() != Language::CSharp)
        return IRule::Continue;

      GrammarHelper::showParseDemo("Test C# declaration: ", *pTc);

      Access access = pRepo->currentAccess();
      bool isPublic = false;

      if (pRepo->scopeStack().size() == 0)
        return false;

      std::string parentType = pRepo->scopeStack().top()->type_;

      size_t posPublic;
      pTc->find("public", posPublic);
      if (posPublic < pTc->size() && parentType != "function")
      {
        isPublic = true;
        pRepo->currentAccess() = Access::publ;
      }

      const Lexer::ITokenCollection& tc = *pTc;
      if (tc.size() > 0 && tc[0] == "using")
      {
        doActions(pTc);
        pRepo->currentAccess() = access;
        return IRule::Stop;
      }

      Lexer::Semi se;
      se.clone(*pTc);

      if (GrammarHelper::isDataDeclaration(se))
      {
        doActions(pTc);
        pRepo->currentAccess() = access;
        return IRule::Stop;
      }

      if (GrammarHelper::isFunctionDeclaration(se, parentType))
      {
        doActions(pTc);
        pRepo->currentAccess() = access;
        return IRule::Stop;
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to add declaration info to scope stack top

  class HandleCSharpDeclaration : public IAction
  {
    Repository* p_Repos;

  public:
    HandleCSharpDeclaration(Repository* pRepos) : p_Repos(pRepos) {}

    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle C# declaration: ", *pTc);

      // store declaration info in ASTNode

      if (p_Repos->scopeStack().size() == 0)
        return;

      ASTNode* pCurrNode = p_Repos->scopeStack().top();
      DeclarationNode declNode;
      declNode.access_ = p_Repos->currentAccess();
      declNode.pTc = pTc->clone();
      declNode.package_ = p_Repos->package();
      declNode.line_ = p_Repos->lineCount();

      Lexer::Semi se;
      se.clone(*pTc);
      GrammarHelper::removeComments(se);

      if (se[0] == "using")
      {
        declNode.declType_ = DeclType::usingDecl;
        pCurrNode->decl_.push_back(declNode);

        GrammarHelper::showParse("using declar", *pTc);
        return;
      }

      std::string parentType = p_Repos->scopeStack().top()->type_;

      if (GrammarHelper::isFunctionDeclaration(se, parentType))
      {
        Access adebug = declNode.access_;
        declNode.declType_ = DeclType::functionDecl;
        pCurrNode->decl_.push_back(declNode);
        GrammarHelper::showParse("function declar", *pTc);
      }
      else
      {
        Access adebug = declNode.access_;
        declNode.declType_ = DeclType::dataDecl;
        pCurrNode->decl_.push_back(declNode);
        GrammarHelper::showParse("data declar", *pTc);
      }
    }
  };

  ///////////////////////////////////////////////////////////////
  // rule to detect C++ Executable

  class CppExecutable : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      Repository* pRepo = Repository::getInstance();
      if (pRepo->language() != Language::Cpp)
        return IRule::Continue;

      GrammarHelper::showParseDemo("Test C++ executable: ", *pTc);

      Lexer::Semi tc;
      tc.clone(*pTc);

      if (tc[tc.size() - 1] == ";" && tc.size() > 2)
      {
        GrammarHelper::removeFunctionArgs(tc);
        GrammarHelper::condenseTemplateTypes(tc);

        // remove modifiers, comments, newlines, returns, and initializers

        Lexer::Semi se;
        for (size_t i = 0; i < tc.size(); ++i)
        {
          if (GrammarHelper::isQualifierKeyWord(tc[i]))
            continue;
          if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
            continue;
          if (tc[i] == "=" || tc[i] == ";")
          {
            se.add(";");
            break;
          }
          else
            se.add(tc[i]);
        }
        if (se.size() != 3)  // not a declaration
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };

  ///////////////////////////////////////////////////////////////
  // action to display C++ executable info

  class HandleCppExecutable : public IAction
  {
    Repository* p_Repo;

  public:
    HandleCppExecutable(Repository* pRepo) : p_Repo(pRepo) {}

    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle C++ executable: ", *pTc);

      GrammarHelper::showParse("executable", *pTc);
    }
  };
  ///////////////////////////////////////////////////////////////
  // rule to detect C# Executable

  class CSharpExecutable : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      Repository* pRepo = Repository::getInstance();
      if (pRepo->language() != Language::CSharp)
        return IRule::Continue;

      GrammarHelper::showParseDemo("Test C# executable: ", *pTc);

      const Lexer::ITokenCollection& in = *pTc;
      Lexer::Semi tc;
      for (size_t i = 0; i < in.size(); ++i)
        tc.add(in[i]);

      if (tc[tc.size() - 1] == ";" && tc.size() > 2)
      {
        GrammarHelper::removeFunctionArgs(tc);
        GrammarHelper::condenseTemplateTypes(tc);

        // remove modifiers, comments, newlines, returns, and initializers

        Lexer::Semi se;
        for (size_t i = 0; i < tc.size(); ++i)
        {
          if (GrammarHelper::isQualifierKeyWord(tc[i]))
            continue;
          if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
            continue;
          if (tc[i] == "=" || tc[i] == ";")
          {
            se.add(";");
            break;
          }
          else
            se.add(tc[i]);
        }
        if (se.size() != 3)  // not a declaration
        {
          doActions(pTc);
          return IRule::Stop;
        }
      }
      return IRule::Continue;
    }
  };
  ///////////////////////////////////////////////////////////////
  // action to display C# executable info

  class HandleCSharpExecutable : public IAction
  {
    Repository* p_Repo;

  public:
    HandleCSharpExecutable(Repository* pRepo) : p_Repo(pRepo) {}

    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle C# executable: ", *pTc);

      Lexer::Semi se;
      se.clone(*pTc);
      GrammarHelper::removeComments(se);

      GrammarHelper::showParse("executable", se);
    }
  };
  ///////////////////////////////////////////////////////////////
  // default rule
  // - this is here to catch any Semi that didn't parse
  // - We don't have rule for enums, so they are caugth here

  class Default : public IRule
  {
  public:
    bool doTest(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Test default: ", *pTc);

      doActions(pTc);  // catches everything
      return IRule::Stop;
    }
  };
  ///////////////////////////////////////////////////////////////
  // action to display default info

  class HandleDefault : public IAction
  {
    Repository* p_Repo;

  public:
    HandleDefault(Repository* pRepo) : p_Repo(pRepo) {}

    void doAction(const Lexer::ITokenCollection* pTc) override
    {
      GrammarHelper::showParseDemo("Handle default: ", *pTc);
      GrammarHelper::showParse("default: ", *pTc);
    }
  };
}
#endif
