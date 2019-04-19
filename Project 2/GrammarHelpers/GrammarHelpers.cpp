/////////////////////////////////////////////////////////////////////////
// GrammarHelpers.cpp - Functions providing base grammatical analyses  //
// ver 1.4                                                             //
// Language:    C++, Visual Studio 2015                                //
// Application: Support for Parsing, CSE687 - Object Oriented Design   //
// Author:      Jim Fawcett, Syracuse University, CST 4-187            //
//              jfawcett@twcny.rr.com                                  //
/////////////////////////////////////////////////////////////////////////

#include "GrammarHelpers.h"
#include "../SemiExpression/Semi.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace CodeAnalysis;

//----< is tok a control keyword for either C++ or C# ? >------------

bool GrammarHelper::isControlKeyWord(const std::string& tok)
{
  const static size_t numKeys = 9;
  const static std::string keys[numKeys]
    = { "for", "foreach", "do", "while", "switch", "if", "else", "try", "catch" };

  for (int i = 0; i < numKeys; ++i)
  {
    if (tok == keys[i])
      return true;
  }
  return false;
}
//----< does Semi contain at least one control keyword ? >--------

bool GrammarHelper::hasControlKeyWord(const Lexer::ITokenCollection& se)
{
  for (size_t i = 0; i < se.size(); ++i)
  {
    if (GrammarHelper::isControlKeyWord(se[i]))
      return true;
  }
  return false;
}
//----< is Semi a function defin or declar ? >--------------------

bool GrammarHelper::isFunction(const Lexer::ITokenCollection& se)
{
  if (se.last() != "{")
    return false;
  size_t len;
  se.find("(", len);
  if (0 < len && len < se.size() && !isControlKeyWord(se[len - 1]))
  {
    size_t posEquals; 
    se.findLast("=", posEquals);
    if (posEquals < se.size() - 1 && se[posEquals + 1] == ">")
    {
      // C# lambda
      return false;
    }
    return true;
  }
  if (len == 0)
  {
    size_t posEquals;
    se.find("=", posEquals);
    if (posEquals < se.size() - 1 && se[posEquals + 1] == ">")
    {
      // C# lambda
      return true;
    }
  }
  return false;
}
//----< has function and function has at least one argument >--------

bool GrammarHelper::hasArgs(const Lexer::ITokenCollection& se)
{
  if (!isFunction(se))
    return false;

  size_t posOpenParen;
  se.find("(", posOpenParen);
  size_t posCloseParen;
  se.find(")", posCloseParen);
  if (posOpenParen < posCloseParen && posCloseParen < se.size())
  {
    if (posCloseParen > posOpenParen + 1)
      return true;
  }
  return false;
}
//----< is Semi a function defin or declar ? >--------------------

bool GrammarHelper::isFunctionDefinition(const Lexer::ITokenCollection& se)
{
  if (se.size() > 0 && se[se.size() - 1] != "{")
    return false;

  if (isFunction(se))
    return true;
  
  return false;
}
//----< find last matching token in Semi >------------------------

size_t GrammarHelper::findLast(const Lexer::ITokenCollection& se, const std::string& token)
{
  size_t endIndex = se.size() - 1;
  for (size_t i = 0; i < se.size(); ++i)
  {
    std::string debugTok = se[endIndex - i];
    if (se[endIndex - i] == token)
      return endIndex - i;
  }
  return se.size();
}
//----< is tok a type qualifier keyword ? >--------------------------

bool GrammarHelper::isQualifierKeyWord(const std::string& tok)
{
  const size_t numKeys = 21;            // its declar else executable.
  const static std::string keys[numKeys] = {
    "const", "extern", "friend", "mutable", "signed", "static", "abstract", 
    "typedef", "typename", "unsigned", "volatile", "&", "*", /*"std",*/ /*"::",*/
    "public", "protected", "private", ":", "typename", "typedef", "++", "--"
  };
  for (int i = 0; i < numKeys; ++i)
    if (tok == keys[i])
      return true;
  return false;
}
//----< strip qualifier keywords from Semi >----------------------

void GrammarHelper::removeQualifiers(Lexer::ITokenCollection& tc)
{
  size_t i = 0;
  while (i < tc.size())
  {
    if (isQualifierKeyWord(tc[i]))
      tc.remove(i);
    else
      ++i;
  }
}
//----< remove calling argument qualifiers after first paren >-------

void GrammarHelper::removeCallingArgQualifiers(Lexer::ITokenCollection& tc)
{
  //std::string debug = tc.show();

  size_t begin;
  tc.find("(", begin);
  size_t end;
  tc.find(")", end);
  if (begin == tc.size() || end == tc.size() || begin >= end)
    return;
  size_t i = begin + 1;
  while (true)
  {
    if (isQualifierKeyWord(tc[i]))
      tc.remove(tc[i]);
    if (tc[i] == ")")
      break;
    ++i;
  }
}
//----< is token a scope connector ? >-------------------------------

bool isScopeConnector(const std::string& tok)
{
  if (tok == "::" || tok == ".")
    return true;
  return false;
}
//----< is first function argument a declaration ? >-----------------

bool GrammarHelper::isFirstArgDeclaration(const Lexer::ITokenCollection& tc, const std::string& parentType)
{
  if(!isFunction(tc))
    return false;  // not a function

  if (parentType == "function")
    return false;  // functions don't declare other functions

  Lexer::Semi se;
  se.clone(tc);

  removeCallingArgQualifiers(se);

  size_t posOpenParen;
  se.find("(", posOpenParen);
  size_t posComma;
  se.find(",", posComma);
  size_t posEqual;
  se.find("=", posEqual);
  size_t posCloseParen;
  se.find(")", posCloseParen);
  
  // must have two adjacent terms without connector, e.g., type and name

  size_t posEnd = (posComma < posEqual) ? posComma : posEqual;
  posEnd = (posEnd < posCloseParen) ? posEnd : posCloseParen;

  // Note:
  // - This set of cases works correctly only if name is included in
  //   declaration, but that is not required by either C++ or C#.
  // - An example of the problem is X(tok);  If tok is a
  //   type then this is a constructor function declaration, otherwise
  //   it is a function invocation with named parameter tok.
  // - To fix this we would need to know all defined types, both
  //   those defined by our code and also by all the libraries used.
  // - for most analysis applications we do not need a completely
  //   reliable function declaration detector

  size_t numArgTerms = posEnd - posOpenParen - 1;
  switch (numArgTerms)
  {
  case 1:
    // must be executable
    return false;
    break;
  case 2:
    // type name
    return true;
    break;
  case 4:
    // namespace :: type name
    return true;
    break;
  default:
    // not an argument declaration
    break;
  }
  return false;
}
//----< is this a function declaration ? >---------------------------

bool GrammarHelper::isFunctionDeclaration(const Lexer::ITokenCollection& tc, const std::string& parentType)
{
  //std::string debug = tc.show();

  if (tc.size() == 0)
    return false;

  if (tc.size() == 1)  // ; or {
    return false;

  if (tc[tc.size() - 1] != ";")
    return false;

  std::string tok = tc[tc.size() - 2];
  if (tok == "delete" || tok == "default")
    return true;

  if (parentType == "function")
    return false;

  if (!isFunction(tc))
    return false;

  size_t posOper;
  tc.find("operator", posOper);
  if (posOper < tc.size())
    return true;

  Lexer::Semi se;
  se.clone(tc);

  if (isFirstArgDeclaration(se, parentType) || !hasArgs(se))
    return true;

  return false;
}
//----< is this a function invocation ? >----------------------------
/*
*  will return false unless invocation uses first paren in Semi
*/
bool GrammarHelper::isFunctionInvocation(const Lexer::ITokenCollection& tc, const std::string& parentType)
{
  if (tc.size() == 1)  // ; or {
    return false;

  if (parentType != "function")
    return false;

  if (isFunction(tc) && !isFunctionDeclaration(tc, parentType) && !isFunctionDefinition(tc))
    return true;
  
  return false;
}
//----< has function invocation somewhere in expression ? >----------

bool GrammarHelper::hasFunctionInvocation(const Lexer::ITokenCollection& tc, const std::string& parentType)
{
  if (tc.size() == 1)  // ; or {
    return false;

  if (tc.size() > 0 && tc[tc.size() - 1] == "{")
    return false;

  if (parentType != "function")
    return false;

  size_t posParen;
  tc.find("(", posParen);
  Lexer::Semi test;
  test.clone(tc, posParen - 1);
  std::string debug = test.show();
  while (true)
  {
    if (isFunctionInvocation(test, parentType))
      return true;
    test.find("(", posParen, posParen + 1);
    if (posParen == test.size())
      return false;
    Lexer::Semi innerTest;
    innerTest.clone(test, posParen - 1);
    test = innerTest;
    std::string debug = test.show();
  }
  return false;
}
//----< is this a data declaration ? >-------------------------------

bool GrammarHelper::isDataDeclaration(const Lexer::ITokenCollection& tc)
{
  std::string debug3 = tc.show();

  if (tc.size() == 1)  // ; or {
    return false;

  if (tc.size() > 0 && tc[tc.size() - 1] != ";")
    return false;

  if (tc.size() > 0 && (tc[0] == "~" || tc[0] == "return"))
    return false;

  if(hasControlKeyWord(tc))
    return false;

  size_t insert;
  tc.find("<<", insert);
  size_t extract;
  tc.find(">>", extract);
  if (insert < tc.size() || extract < tc.size())
    return false;

  std::string debug = tc.show();

  Lexer::Semi se;
  se.clone(tc);
  condenseTemplateTypes(se);
  removeQualifiers(se);
  removeCppInitializers(se);
  
  // is this a function declaration ?

  size_t posParen;
  se.find("(", posParen);
  if (posParen < se.size() && (posParen == 2 || posParen == 3))
    return false;

  removeFunctionArgs(se);
  
  // is stripped tc of the form "type name ;" or "namespace :: type name ;"

  std::string debug2 = se.show();

  if (se.size() == 3)
    return true;
  if (se.size() == 5 && se[1] == "::")
    return true;

  return false;
}
//----< is this an executable statement ? >--------------------------

bool GrammarHelper::isExecutable(const Lexer::ITokenCollection& tc, const std::string& parentType)
{
  if (tc.size() == 1)  // ; or {
    return false;

  if (isFunctionDefinition(tc))
    return false;
  if (isFunctionDeclaration(tc, parentType))
    return false;
  if (isDataDeclaration(tc))
    return false;
  return true;
}
//----< remove initializers from C++ Semi >-----------------------

void GrammarHelper::removeCppInitializers(Lexer::ITokenCollection& tc)
{
  std::string debug = tc.show();
  for (size_t i = 0; i < tc.size(); ++i)
  {
    if (tc[i] == "=" || tc[i] == "{")
    {
      size_t j = i;
      while (j < tc.size() && tc[j] != ";")
      {
        tc.remove(j);
      }
      break;
    }
  }
  if (tc.size() > 0 && tc[tc.size() - 1] != ";")
    tc.add(";");
}
//----< remove initializers from C# Semi >------------------------

void GrammarHelper::removeCSharpInitializers(Lexer::ITokenCollection& tc)
{
  std::string debug = tc.show();
  for (size_t i = 0; i < tc.size(); ++i)
  {
    if (tc[i] == "=")
    {
      size_t j = i;
      while (j < tc.size() && tc[j] != ";")
      {
        tc.remove(j);
      }
      break;
    }
  }
  if (tc.size() > 0 && tc[tc.size() - 1] != ";")
    tc.add(";");
}
//----< remove comments from Semi >-------------------------------

void GrammarHelper::removeComments(Lexer::ITokenCollection& tc)
{
  size_t i = 0;
  while (true)
  {
    if (tc.isComment(tc[i]))
      tc.remove(i);
    if (++i == tc.size())
      break;
  }
}
//----< condense template spec to single token >---------------------

void GrammarHelper::condenseTemplateTypes(Lexer::ITokenCollection& tc)
{
  size_t start;
  tc.find("<", start);
  size_t end;
  tc.find(">", end);
  if (start >= end || start == 0)
    return;
  else
  {
    if (end == tc.size())
      tc.find(">::", end);
    if (end == tc.size())
      return;
  }
  std::string save = tc[end];
  std::string tok = tc[start - 1];
  for (size_t i = start; i < end + 1; ++i)
  {
    tok += tc[i];
    if (tc[i] == "typename" || tc[i] == "class")
      tok += " ";
  }
  for (size_t i = start; i < end + 1; ++i)
    tc.remove(start);
  if (save == ">::")
  {
    tok += tc[start + 1];
    tc.remove(start);
  }
  tc[start - 1] = tok;
  //std::cout << "\n  -- " << tc.show();
}
//----< remove function's formal arguments >-------------------------

void GrammarHelper::removeFunctionArgs(Lexer::ITokenCollection& tc)
{
  std::string debug = tc.show();

  size_t start;
  tc.find("(", start);
  size_t end;
  tc.find(")", end);
  if (start >= end || end == tc.size() || start == 0)
    return;
  if (GrammarHelper::isControlKeyWord(tc[start - 1]))
    return;
  for (size_t i = start; i < end + 1; ++i)
    tc.remove(start);
  //std::cout << "\n  -- " << tc.show();
}
//----< show Semi with Dbug logger >------------------------------

void GrammarHelper::showParse(const std::string& msg, const Lexer::ITokenCollection& se, bool isResult)
{
  using Rslt = Logging::StaticLogger<0>;
  using Dbug = Logging::StaticLogger<2>;
  if (Dbug::running() || Rslt::running())
  {
    std::ostringstream out;
    out << "\n  " << std::left << std::setw(15) << msg << ": ";
    std::string temp = se.show();
    //if (temp.size() > 80)
    //  out << "\n    ";
    out << temp;
    if (isResult && Rslt::running())
      Rslt::write(out.str());
    else if (Dbug::running())
      Dbug::write(out.str());
  }
}

void GrammarHelper::showParseDemo(const std::string& msg, const Lexer::ITokenCollection& se)
{
  using Demo = Logging::StaticLogger<1>;
  if (Demo::running())
  {
    std::ostringstream out;
    out << "\n  " << std::left << std::setw(32) << msg << ": ";
    std::string temp = se.show();
    if (temp.size() > 80)
      out << "\n    ";
    out << temp;
    Demo::write(out.str());
  }
}

#ifdef TEST_GRAMMARHELPERS

#include <vector>
#include <iostream>
#include "../SemiExpression/Semi.h"
#include "../Utilities/Utilities.h"

using namespace Lexer;
using Token = std::string;
using Toks = std::vector<Token>;

void load(const Toks& toks, Semi& se)
{
  se.clear();
  for (auto item : toks)
    se.add(item);
}

std::string showQualifiers(const Semi& se)
{
  std::string temp;
  for (size_t i = 0; i < se.size(); ++i)
    if (GrammarHelper::isQualifierKeyWord(se[i]))
      temp += se[i] + " ";
  return temp;
}

void showPredicate(bool predicate, const std::string& msg)
{
  if (predicate)
    std::cout << "\n    " << msg;
  else
    std::cout << "\n    not " << msg;
}

std::string show(const ITokenCollection& se)
{
  std::string temp;
  for (size_t i = 0; i < se.size(); ++i)
    temp += se[i] + " ";
  return temp;
}

void testHelpers(const Semi& seIn)
{
  Lexer::Semi se;
  se.clone(seIn);

  Utilities::StringHelper::title("Testing GrammarHelpers");
  std::cout << "\n  " << show(se);
  se.trim();
  std::cout << "\n    removing comments:  " << show(se);
  std::cout << "\n    index of last \"(\" is " << GrammarHelper::findLast(seIn, "(");
  std::cout << "\n    qualifier keywords are: " << showQualifiers(seIn);

  std::string debug = se.show();
  showPredicate(GrammarHelper::isFunctionDefinition(se), "function definition");

  std::string parentType = "function";
  std::cout << "\n  parent type = " << parentType;
  showPredicate(GrammarHelper::isFunctionDeclaration(se, parentType), "function declaration");
  showPredicate(GrammarHelper::isFunctionInvocation(se, parentType), "function invocation");
  showPredicate(GrammarHelper::hasFunctionInvocation(se, parentType), "has function invocation");
  parentType = "class";
  std::cout << "\n  parent type = " << parentType;
  showPredicate(GrammarHelper::isFunctionDeclaration(se, parentType), "function declaration");
  showPredicate(GrammarHelper::isFunctionInvocation(se, parentType), "function invocation");
  showPredicate(GrammarHelper::hasFunctionInvocation(se, parentType), "has function invocation");

  showPredicate(GrammarHelper::isDataDeclaration(se), "data declaration");
  showPredicate(GrammarHelper::hasControlKeyWord(se), "found control Keyword");

  Semi testSE;
  testSE.clone(seIn);
  GrammarHelper::condenseTemplateTypes(testSE);
  std::cout << "\n    condensing template types: " << show(testSE);

  testSE.clone(seIn);
  GrammarHelper::removeQualifiers(testSE);
  std::cout << "\n    removing qualifiers: " << show(testSE);

  testSE.clone(seIn);
  GrammarHelper::removeCppInitializers(testSE);
  std::cout << "\n    removing C++ initializers: " << show(testSE);

  testSE.clone(seIn);
  GrammarHelper::removeFunctionArgs(testSE);
  std::cout << "\n    removing function args: " << show(testSE);

  testSE.clone(seIn);
  GrammarHelper::removeCallingArgQualifiers(testSE);
  std::cout << "\n    removing qualifiers in function calling sequence: " << show(testSE);
  std::cout << "\n";
} 
int main()
{
  Utilities::StringHelper::Title("Testing Grammar Functions");
  std::cout << "\n  Note:";
  std::cout << "\n  - Test functions are applied to Semis that they should change and those they should not.";
  std::cout << "\n  - The intent is to make sure they do what they are supposed to do without harming Semis";
  std::cout << "\n  - that should not change.\n";

  Toks tokset0 = { "/* Test0 */", ";" };
  Toks tokset1 = { "/* Test1 */", "extern", "int", "x", "[", "2", "]", "=", "{", "1", ",", "2", "}", ";" };
  Toks tokset2 = { "/* Test2 */", "extern", "int", "x", "[", "2", "]", "{", "1", ",", "2", "}", ";" };
  Toks tokset3 = { "/* Test3 */", "if", "(", "x", "==", "y", ")", "doThing", "(", "3", ")", ";" };
  Toks tokset4 = { "/* Test4 */", "void", "fun", "(", "int", "anInteger", ")", "{" };
  Toks tokset5 = { "/* Test5 */", "void", "fun", "(", "int", "anInteger", ")", ";" };
  Toks tokset6 = { "/* Test6 */", "int", "fun", "(", "const", "int", "&", "anItegerRef", ")", ";" };
  Toks tokset7 = { "/* Test7 */", "template", "<", "typename", "C", ">", "class", "A", "{" };
  Toks tokset8 = { "/* Test7 */", "template", "<", "typename", "C", ">", "void", "fun", "(", "const", "C", "&", "c", ")", "{" };
  Toks tokset9 = { "/* Test9 */", "std", "::","vector", "<", "std", "::", "string", ">", "aVec", ";" };
  Toks tokset10 = { "/* Test10 */", "ClassName", "(", ")", ";" };
  Toks tokset11 = { "/* Test11 */", "~", "ClassName", "(", ")", ";" };
  Toks tokset12 = { "/* Test12 */", "ClassName", "(", "int", "count", ")", ";" };
  Toks tokset13 = { "/* Test13 */", "ClassName", "(", "int", "count", ")", ":", "count_", "(", "count", ")", "{" };
  Toks tokset14 = { "/* Test14 */", "Lexer", "::", "Semi", "se", "(", "nullPtr", ")", ";" };
  Toks tokset15 = { "/* Test15 */", "public", "abstract", "void", "doAction", "(", "CSemi", ".", "CSemi", "semi", ")", ";" };

  Lexer::Semi se;

  load(tokset0, se);
  testHelpers(se);
  load(tokset1, se);
  testHelpers(se);
  load(tokset2, se);
  testHelpers(se);
  load(tokset3, se);
  testHelpers(se);
  load(tokset4, se);
  testHelpers(se);
  load(tokset5, se);
  testHelpers(se);
  load(tokset6, se);
  testHelpers(se);
  load(tokset7, se);
  testHelpers(se);
  load(tokset8, se);
  testHelpers(se);
  load(tokset9, se);
  testHelpers(se);
  load(tokset10, se);
  testHelpers(se);
  load(tokset11, se);
  testHelpers(se);
  load(tokset12, se);
  testHelpers(se);
  load(tokset13, se);
  testHelpers(se);
  load(tokset14, se);
  testHelpers(se);
  load(tokset15, se);
  testHelpers(se);
}
#endif


