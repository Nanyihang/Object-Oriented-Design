/////////////////////////////////////////////////////////////////////
// Semi.cpp - Collects groups of tokens that are useful for        //
// ver 1.2    grammatical analysis                                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019       //
/////////////////////////////////////////////////////////////////////

#include "Semi.h"
#include <sstream>

namespace Lexer
{
  Semi::Semi(bool createToker/* = false*/)
  {
    if (createToker)
      pToker_ = new Toker;
  }

  Semi::Semi(std::vector<Token> list, bool createToker/* = false*/)
  {
    if (createToker)
      pToker_ = new Toker;

    for (Token tok : list)
      toks.push_back(tok);
  }

  Semi::~Semi()
  {
    delete pToker_;
    pToker_ = nullptr;
  }

  Toker* Semi::getToker()
  {
    return pToker_;
  }

  void Semi::setToker(Toker* pToker)
  {
    if (pToker_ != nullptr)
      delete pToker_;
    pToker_ = pToker;
  }

  void Semi::addRange(TokColl coll)
  {
    for (Token tok : coll)
      toks.push_back(tok);
  }

  bool Semi::open(const std::string& source)
  {
    return pToker_->open(source);
  }

  void Semi::close()
  {
    pToker_->close();
  }
  //----< return number of tokens in semi-expression >-------------

  size_t Semi::size() const
  {
    return toks.size();
  }
  //----< convert semi to string >---------------------------------

  std::string Semi::ToString()
  {
    std::string temp;
    for (Token tok : toks)
    {
      temp += tok += " ";
    }
    return temp;
  }
  //----< return number of lines processed >-----------------------

  size_t Semi::lineCount()
  {
    return pToker_->lineCount();
  }
  //----< is tok a terminator for the current semi-expression? >---

  bool Semi::isTerminator(Token tok)
  {
    if (tok == ";" || tok == "{" || tok == "}")
      return true;
    if (tok == "\n")
    {
      trim();
      if (toks.size() > 0 && toks[0] == "#")
        return true;
    }
    return false;
  }
  //----< remove leading newlines >--------------------------------

  void Semi::trim()
  {
    size_t count = 0;
    for (count = 0; count < toks.size(); ++count)
    {
      if (toks[count] != "\n")
        break;
    }
    if (count == 0)
      return;

    TokColl temp;
    for (size_t i = count; i < toks.size(); ++i)
      temp.push_back(toks[i]);
    toks = temp;
  }
  //----< does semi contain specific token? >----------------------

  bool Semi::contains(Token tok) const
  {
    for (Token test : toks)
      if (test == tok)
        return true;
    return false;
  }
  //----< find token in semi >-------------------------------------

  bool Semi::find(Token tok, size_t& index, size_t offset) const
  {
    for (index = offset; index < size(); ++index)
    {
      if (toks[index] == tok)
        return true;
    }
    index = size();
    return false;
  }
  //----< find last occurance of tok >-----------------------------

  bool Semi::findLast(Token tok, size_t& index) const
  {
    int pos = -1;
    for (size_t i = 0; i < toks.size(); ++i)
    {
      if (toks[i] == tok)
        pos = int(i);
    }
    if (pos == -1)
      index = size();
    else
      index = pos;
    return (pos != -1);
  }
  //----< find predecessor of token >------------------------------

  Token Semi::predecessor(Token tok)
  {
    size_t index;
    if (find(tok, index) && index > 0)
    {
      return toks[index - 1];
    }
    return "";
  }
  //----< return last token in collection >------------------------

  Token Semi::last() const
  {
    return toks[toks.size() - 1];
  }
  //----< test for ordered sequence of tokens >--------------------

  bool Semi::hasSequence(TokColl tokSeq)
  {
    int position = 0;
    for (Token tok : toks)
    {
      if (position == tokSeq.size() - 1)
        return true;
      if (tok == tokSeq[position])
        ++position;
    }
    return (position == tokSeq.size() - 1);
  }
  //----< fold >---------------------------------------------------

  void Semi::fold()
  {
    TokColl tc1{ "for", "(", ":" };  // range-based for
    if (hasSequence(tc1))
      return;
    TokColl tc{ "for", "(", ";" };   // conventional for
    if (hasSequence(tc))
    {
      Semi temp;
      temp.addRange(this->toks);
      get();  // get i<N;
      temp.addRange(this->toks);
      get();
      temp.addRange(this->toks);  // get ++i) .. {
      toks = temp.toks;
    }
  }
  //----< used by parser to get the next collection of tokens >----

  bool Semi::get()
  {
    toks.clear();

    while (!pToker_->isDone())
    {
      Token tok = pToker_->getTok();
      if (tok != "\n")
        toks.push_back(tok);
      if (isComment(tok))
        return true;
      if (isTerminator(tok))
      {
        fold();
        return true;
      }
    }
    return false;
  }
  //----< clone replaces tokens in caller with tokens of argument >----

  void Semi::clone(const ITokenCollection& tc, size_t offset)
  {
    toks.clear();
    for (size_t i = offset; i < tc.size(); ++i)
    {
      toks.push_back(tc[i]);
    }
  }
  //----< clone tokens of existing SemiExp >---------------------------
  /*
  *  Note:
  *  - Returns pointer to new SemiExp on heap.  Application is
  *    responsible for deleting.
  *  - Does not clone pointer to _toker or hasToken members!
  */
  ITokenCollection* Semi::clone() const
  {
    Semi* pClone = new Semi;
    for (size_t i = 0; i < size(); ++i)
      pClone->add(toks[i]);
    return pClone;
  }
  //----< indexer allows us to index for a specific token >--------

  Token& Semi::operator[](size_t i)
  {
    if (i < 0 || toks.size() <= i)
      throw(std::exception());
    return toks[i];
  }
  //----< indexer allows us to index for a specific token >--------

  Token Semi::operator[](size_t i) const
  {
    if (i < 0 || toks.size() <= i)
      throw(std::exception());
    return toks[i];
  }
  //----< begin iterator >-----------------------------------------

  Semi::iterator Semi::begin()
  {
    return toks.begin();
  }
  //----< end iterator >-------------------------------------------

  Semi::iterator Semi::end()
  {
    return toks.end();
  }
  //----< add a token to the end of this semi-expression >---------

  ITokenCollection* Semi::add(Token token)
  {
    toks.push_back(token);
    return this;
  }
  //----< remove element at index >---------------------------------

  bool Semi::remove(size_t index)
  {
    iterator iter = toks.erase(toks.begin() + index);
    return (toks.begin() <= iter && iter < toks.end());
  }
  //----< remove first instance of tok >-----------------------------

  bool Semi::remove(Token tok)
  {
    size_t pos;
    if (!find(tok, pos))
      return false;
    return remove(pos);
  }
  //----< clear all the tokens from internal collection >----------

  void Semi::clear()
  {
    toks.clear();
  }
  //----< are we at the end of the token source stream? >----------

  bool Semi::isDone()
  {
    return pToker_->isDone();
  }
  //----< insert a token at position n >---------------------------

  bool Semi::insert(size_t n, Token tok)
  {
    if (n < 0 || n >= tok.size())
      return false;
    TokColl::iterator iter = toks.begin();
    toks.insert(iter + n, tok);
    return true;
  }
  //----< does this semi-expression contain a terminator? >--------

  bool Semi::hasTerminator()
  {
    if (toks.size() <= 0)
      return false;
    if (isTerminator(toks[toks.size() - 1]))
      return true;
    return false;
  }
  //----< display contents of semi-expression on console >---------

  std::string Semi::show() const
  {
    std::ostringstream out;
    out << "\n-- ";
    for (Token tok : toks)
    {
      if (tok != "\n")
        out << tok << " ";
      else
        out << "\n";
    }
    return out.str();
  }
  //----< is this token a comment? >---------------------------------

  bool Semi::isComment(Token tok)
  {
    Toker* pToker = getToker();
    return (pToker->isSingleLineComment(tok) || pToker->isMultipleLineComment(tok));
  }
}

#ifdef TEST_SEMI

using namespace Lexer;

int main()
{
  std::cout << "\n  testing Semi";
  std::cout << "\n ==============";

  // Access Semi through interface and object factory.
  // That isolates client from any changes that may occur to Semi
  // as long as ITokenCollection doesn't change.

  ITokenCollection* pSemi = Factory::create(true);  // true creates toker

  //std::string source = "../SemiExpression/semi.h";
  std::string source = "../TestFiles/DirExplorerN.h";
  if (!pSemi->open(source))
  {
    std::cout << "\n  Can't open \"" + source + "\"";
    return 1;
  }
  Toker* pToker = pSemi->getToker();
  pToker->doReturnComments(true);

  while (!pSemi->isDone())
  {
    pSemi->get();
    std::cout << pSemi->show();
  }
  std::cout << "\n";

  std::cout << "\n  demonstrating semi operations";
  std::cout << "\n -------------------------------";

  ITokenCollection* pTest = Factory::create();

  pTest->add("one")->add("two")->add("three");
  std::cout << pTest->show();
  TokColl coll1 { "one", "three" };
  if (pTest->hasSequence(coll1))
    std::cout << "\n  semi has token \"one\" followed by token \"three\"";
  TokColl coll2{ "foo", "Two" };
  if (!pTest->hasSequence(coll2))
    std::cout << "\n  semi does not have token \"foo\" followed by token \"two\"";
  if (!pTest->hasTerminator())
    std::cout << "\n  semi does not have terminator";

  std::cout << "\n  demonstrate changing semi with insert # and add \\n";
  pTest->insert(0, "#");
  pTest->add("\n");
  std::cout << pTest->show();

  std::cout << "\n  removing \\n";
  pTest->remove("\n");
  //size_t pos = pTest->find("\n");
  //pTest->remove(pos);
  std::cout << pTest->show();

  std::cout << "\n  demonstrate semi tests";
  if (pTest->hasTerminator())
    std::cout << "\n  semi has terminator";
  else
    std::cout << "\n  semi does not have terminator";

  size_t index;
  Token tok = "two";
  if (pTest->find(tok, index))
    std::cout << "\n  found token \"" << tok << "\" at position " << index;
  else
    std::cout << "\n  did not find token \"" << tok << "\"";
  tok = "foo";
  if (pTest->find(tok, index))
    std::cout << "\n  found token \"" << tok << "\" at position " << index;
  else
    std::cout << "\n  did not find token \"" << tok << "\"";

  tok = "one";
  Token tok2 = pTest->predecessor(tok);
  std::cout << "\n  predecessor of \"" << tok << "\" is \"" << tok2;
  tok = "bar";
  tok2 = pTest->predecessor(tok);
    std::cout << "\n  predecessor of \"" << tok << "\" is \"" << tok2 << "\"";

  std::cout << "\n  indexing semi\n  ";
  for (size_t i = 0; i < pTest->size(); ++i)
    std::cout << (*pTest)[i] << " ";
  std::cout << "\n  using range for:\n  ";
  for(auto tk : (*pTest))
    std::cout << tk << " ";

  std::cout << "\n  testing clones:";
  ITokenCollection* pTest2 = nullptr;
  std::cout << "\n    clone target after cloning:";
  pTest2 = pTest->clone();
  std::cout << pTest2->show();
  pTest->add("anotherTok")->add(";");
  pTest2->clone(*pTest);
  std::cout << pTest2->show();

  std::cout << "\n  testing detection of comments:";
  bool test1 = pTest2->isComment("// cpp comment");
  bool test2 = pTest2->isComment("/* a comment */");
  if (test1 && test2)
    std::cout << "\n  detection of comments passes";
  else
    std::cout << "\n  detection of comments fails";
  std::cout << "\n\n";
}
#endif

