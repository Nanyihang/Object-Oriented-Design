/////////////////////////////////////////////////////////////////////
// Toker.cpp - Collects words from a stream                        //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019       //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>
#include "Toker.h"

namespace Lexer
{

  TokenContext* ITokenState::pContext_;  // derived classes store context ref here
  
  void ITokenState::setContext(TokenContext* pContext)
  {
    pContext_ = pContext;
  }

  TokenContext::TokenContext()
  {
    pWhiteSpaceState_ = new WhiteSpaceState();
    pAlphNumState_ = new AlphaNumState();
    pPunctuationState_ = new PunctuationState();
    pNewLineState_ = new NewLineState();
    pSingleLineCommentState_ = new SingleLineCommentState();
    pMultiLineCommentState_ = new MultiLineCommentState();
    pSingleQuoteState_ = new SingleQuoteState();
    pDoubleQuoteState_ = new DoubleQuoteState();
    pCurrentState_ = pWhiteSpaceState_;
    pSrc_ = new TokenSourceFile(this);
  }
  void deleter(ITokenState*& pState)
  {
    delete pState;
    pState = nullptr;
  }
  TokenContext::~TokenContext()
  {
    //std::cout << "\n  TokenContext destroyed";
    deleter(pWhiteSpaceState_);
    deleter(pAlphNumState_);
    deleter(pPunctuationState_);
    deleter(pNewLineState_);
    deleter(pSingleLineCommentState_);
    deleter(pMultiLineCommentState_);
    deleter(pSingleQuoteState_);
    deleter(pDoubleQuoteState_);
    //delete pCurrentState_;
    delete pSrc_;
    pSrc_ = nullptr;
  }

  TokenSourceFile::TokenSourceFile(TokenContext* pContext)
  {
    lineCount(1);
    pContext_ = pContext;
  }

  TokenSourceFile::~TokenSourceFile()
  {
    //std::cout << "\n  TokenSourceFile instance closing";
    if (fs_.good())
      fs_.close();
    std::cout << "\n";
  }
  //----< attempt to open file with ifstream >---------------------

  bool TokenSourceFile::open(std::string path)
  {
    std::cout << "\n  attempting to open " << path;
    fs_ = std::ifstream(path, std::ios::in);
    if (fs_.good())
    {
      //std::cout << "\n  open successful";
      ITokenState* pCurr = pContext_->pCurrentState_;
      ITokenState* pNext = pCurr->nextState();
      pContext_->pCurrentState_ = pNext;
      return true;
    }
    std::cout << "\n  open failed";
    return false;
  }
  //----< close file >---------------------------------------------

  void TokenSourceFile::close()
  {
    if(fs_.good())
      fs_.close();
  }
  //----< extract the next available integer >---------------------
  /*
   *  - checks to see if previously enqueued peeked ints are available
   *  - if not, reads from stream
   */
  int TokenSourceFile::next()
  {
    int ch;
    if (charQ_.size() == 0)  // no saved peeked ints
    {
      if (end())
        return -1;
      ch = fs_.get();
    }
    else                    // has saved peeked ints, so use the first
    {
      ch = charQ_[0];
      charQ_.pop_front();      // pop from queue
    }
    if ((char)ch == '\n')   // track the number of newlines seen so far
      ++lineCount.ref();
    return ch;
  }
  //----< peek n ints into source without extracting them >--------
  /*
   *  - This is an organizing prinicple that makes tokenizing easier
   *  - We enqueue because file streams only allow peeking at the first int
   *    and even that isn't always reliable if an error occurred.
   *  - When we look for two punctuator tokens, like ==, !=, etc. we want
   *    to detect their presence without removing them from the stream.
   */
  int TokenSourceFile::peek(size_t n)
  {
    if (n < charQ_.size())  // nth already peeked, so return it
    {
      return charQ_[n];
    }
    else                  // nth int not yet peeked
    {
      for (size_t i = charQ_.size(); i <= n; ++i)
      {
        if (end())
          return -1;
        charQ_.push_back(fs_.get());  // read and enqueue
      }
      return charQ_[n];   // now return the last peeked
    }
  }
  //----< reached the end of the file stream? >--------------------

  bool TokenSourceFile::end()
  {
    return !(fs_.good());
  }

  TokenState::TokenState()
  {
    std::unordered_set<std::string> oneTemp
    {
      "<", ">", "[", "]", "(", ")", "{", "}", ".", ";", "=", "+", "-", "*"
    };
    oneCharTokens_(oneTemp);

    std::unordered_set<std::string> twoTemp
    {
      "<<", ">>", "::", "++", "--", "==", "+=", "-=", "*=", "/=", "&&", "||"
    };
    twoCharTokens_(twoTemp);
  }

  void TokenState::setContext(TokenContext* pContext)
  {
    pContext_ = pContext;
  }
  //----< return set of oneCharTokens >----------------------------

  std::unordered_set<std::string>& TokenState::oneCharTokens()
  {
    return oneCharTokens_.ref();
  }
  //----< does set contain this string? >--------------------------

  bool TokenState::oneCharTokensContains(const std::string& tok)
  {
    return oneCharTokens().find(tok) != oneCharTokens().end();
  }
  //----< return set of twoCharTokens >----------------------------

  std::unordered_set<std::string>& TokenState::twoCharTokens()
  {
    return twoCharTokens_.ref();
  }
  //----< does set contain this string? >--------------------------

  bool TokenState::twoCharTokensContains(const std::string& tok)
  {
    return twoCharTokens().find(tok) != twoCharTokens().end();
  }
  //----< add token to special one char tokens >-------------------

  bool TokenState::addOneCharToken(const std::string& oneCharTok)
  {
    if (oneCharTok.size() > 1)
      return false;
    oneCharTokens_.ref().insert(oneCharTok);
    return true;
  }
  //----< remove token from special one char tokens >--------------

  bool TokenState::removeOneCharToken(const std::string& oneCharTok)
  {
    return oneCharTokens_.ref().erase(oneCharTok);
  }
  //----< add token to special two char tokens >-------------------

  bool TokenState::addTwoCharToken(const std::string& twoCharTok)
  {
    if (twoCharTok.size() != 2)
      return false;
    twoCharTokens_.ref().insert(twoCharTok);
    return true;
  }
  //----< remove token from special two char tokens >--------------

  bool TokenState::removeTwoCharToken(const std::string& twoCharTok)
  {
    return twoCharTokens_.ref().erase(twoCharTok);
  }
  //----< delegate source opening to context's src >---------------

  bool TokenState::open(const std::string& path)
  {
    return pContext_->pSrc_->open(path);
  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isWhiteSpace()
  {
    char ch = (char)pContext_->pSrc_->peek();
    return (isspace(ch, loc) && ch != '\n');

  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isNewLine()
  {
    return ((char)pContext_->pSrc_->peek() == '\n');
  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isAlphaNum()
  {
    char ch = (char)pContext_->pSrc_->peek();
    return (isalnum(ch, loc) || ch == '_');
  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isSingleLineComment()
  {
    int first = pContext_->pSrc_->peek();
    int second = pContext_->pSrc_->peek(1);
    char chFirst = (char)first;
    char chSecond = (char)second;
    return (chFirst == '/' && chSecond == '/');
  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isMultiLineComment()
  {
    int first = pContext_->pSrc_->peek();
    int second = pContext_->pSrc_->peek(1);
    char chFirst = (char)first;
    char chSecond = (char)second;
    return (chFirst == '/' && chSecond == '*');
  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isDoubleQuote()
  {
    char ch = (char)pContext_->pSrc_->peek();
    if (ch == '@')
    {
      char nxt = (char)pContext_->pSrc_->peek(1);
      return (nxt == '\"');
    }
    return (ch == '\"');
  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isSingleQuote()
  {
    char ch = (char)pContext_->pSrc_->peek();
    return (ch == '\'');
  }
  //----< what's next in the TokenSource? >------------------------

  bool TokenState::isPunctuation()
  {
    bool test = isWhiteSpace() || isNewLine() || isAlphaNum();
    test = test || isSingleLineComment() || isMultiLineComment();
    test = test || isSingleQuote() || isDoubleQuote();
    return !test;
  }
  //----< return next state based on content of TokenSource >------

  ITokenState* TokenState::nextState()
  {
    int first = pContext_->pSrc_->peek();
    if (first < 0)
      return nullptr;

    if (isWhiteSpace())
      return pContext_->pWhiteSpaceState_;

    if (isNewLine())
      return pContext_->pNewLineState_;

    if (isAlphaNum())
      return pContext_->pAlphNumState_;

    if (isSingleLineComment())
      return pContext_->pSingleLineCommentState_;

    if (isMultiLineComment())
      return pContext_->pMultiLineCommentState_;

    if (isDoubleQuote())
      return pContext_->pDoubleQuoteState_;

    if (isSingleQuote())
      return pContext_->pSingleQuoteState_;

    // toker's definition of punctuation is anything that is not:
    // - whitespace     space, tab, return
    // - newline
    // - alphanumeric   abc123
    // - comment        /* comment */ or // comment
    // - quote          'a' or "a string"

    // Char.IsPunctuation is not inclusive enough

    return pContext_->pPunctuationState_;
  }
  //----< has tokenizer reached the end of its source? >-----------

  bool TokenState::isDone() {
    if (pContext_->pSrc_ == nullptr)
      return true;
    return pContext_->pSrc_->end();
  }
  //----< helper function to handle escaped chars in states >------
  /*
   * Tests to see if last char in token is preceded by an odd number
   * of escape chars, e.g.:
   * \\\' is escaped
   * \\"  is not escaped
   */
  bool TokenState::isEscaped(Token tok)
  {
    int size = int(tok.size());
    if (size < 2)
      return false;
    int count = 0;
    for (int i = 0; i < size - 1; ++i)
    {
      count = i % 2;
      if (tok[size - i - 2] != '\\')
        break;
    }
    if (count == 0)
      return false;
    return true;
  }

  //----< initialize state machine >-------------------------------

  Toker::Toker()
  {
    pContext_ = new TokenContext();
    ITokenState::setContext(pContext_);
    doReturnComments(false);
  }

  Toker::~Toker()
  {
    //std::cout << "\n  toker destroyed";
    delete pContext_;
  }
  //----< attempt to open source of tokens >-----------------------
  /*
   * If src is successfully opened, it uses TokenState.nextState(context_)
   * to set the initial state, based on the source content.
   */
  bool Toker::open(const std::string& path)
  {
    return pContext_->pSrc_->open(path);  // if true, src has set initial state
  }
  //----< close source of tokens >---------------------------------

  void Toker::close()
  {
    pContext_->pSrc_->close();
  }
  //----< extract a single token from TokenSource >----------------
  /*
   * Method promises to:
   * - extract all the text for a single token
   * - leave all the text for the next token in the TokenSource
   * - discard all whitespace except for newlines
   * - discard all comments unless doReturnComments is true
   */
  bool Toker::overwrite(Token tok)
  {
    if (isWhiteSpace(tok))
      return true;
    if (!doReturnComments() && ((isSingleLineComment(tok) || isMultipleLineComment(tok))))
      return true;
    if ((int)tok[0] < -1)  // is Byte Order Mark
      return true;
    return false;
  }
  //----< extract token >------------------------------------------

  Token Toker::getTok()
  {
    Token tok;
    while (!isDone())
    {
      tok = pContext_->pCurrentState_->getTok();
      pContext_->pCurrentState_ = pContext_->pCurrentState_->nextState();
      if (!overwrite(tok))
        break;
    }
    return tok;
  }
  //----< has Toker reached end of its source? >-------------------

  bool Toker::isDone()
  {
    if (pContext_->pCurrentState_ == nullptr)
      return true;
    return pContext_->pCurrentState_->isDone();
  }
  //----< return number of newlines encountered in file >----------

  int Toker::lineCount() { return int(pContext_->pSrc_->lineCount()); }

  //----< return set of oneCharTokens >----------------------------

  Property<std::unordered_set<std::string>>& Toker::oneCharTokens()
  {
    return pContext_->pCurrentState_->oneCharTokens_;
  }
  //----< return set of twoCharTokens >----------------------------

  Property<std::unordered_set<std::string>>& Toker::twoCharTokens()
  {
    return pContext_->pCurrentState_->twoCharTokens_;
  }
  //----< add token to special one char tokens >-------------------

  bool Toker::addOneCharToken(const std::string& oneCharTok)
  {
    if (oneCharTok.size() != 1)
      return false;
    pContext_->pCurrentState_->oneCharTokens_.ref().insert(oneCharTok);
    return true;
  }
  //----< remove token from special one char tokens >--------------

  bool Toker::removeOneCharToken(const std::string& oneCharTok)
  {
    return pContext_->pCurrentState_->oneCharTokens_.ref().erase(oneCharTok);
  }
  //----< add token to special two char tokens >-------------------

  bool Toker::addTwoCharToken(const std::string& twoCharTok)
  {
    if (twoCharTok.size() != 2)
      return false;
    pContext_->pCurrentState_->twoCharTokens_.ref().insert(twoCharTok);
    return true;
  }
  //----< remove token from special two char tokens >--------------

  bool Toker::removeTwoCharToken(const std::string& twoCharTok)
  {
    return pContext_->pCurrentState_->twoCharTokens_.ref().erase(twoCharTok);
  }
  //----< is this token whitespace? >------------------------------

  bool Toker::isWhiteSpace(Token tok)
  {
    if (tok.size() == 0)
      return false;
    return (isspace(tok[0], loc) && tok[0] != '\n');
  }
  //----< is this token a newline? >-------------------------------

  bool Toker::isNewLine(Token tok)
  {
    if (tok.size() == 0)
      return false;
    return (tok[0] == '\n');
  }
  //----< is this token alphanumeric? >----------------------------

  bool Toker::isAlphaNum(Token tok)
  {
    if (tok.size() == 0)
      return false;
    return (isalnum(tok[0]) || tok[0] == '_');
  }
  //----< is this token punctuator? >------------------------------

  bool Toker::isPunctuator(Token tok)
  {
    if (tok.size() == 0)
      return false;
    bool test = isWhiteSpace(tok) || isNewLine(tok) || isAlphaNum(tok);
    test = test || isSingleLineComment(tok) || isMultipleLineComment(tok);
    test = test || isSingleQuote(tok) || isDoubleQuote(tok);
    return !test;
  }
  //----< is this token a single line comment? >-------------------

  bool Toker::isSingleLineComment(Token tok)
  {
    if (tok.size() < 2)
      return false;
    if (tok[0] == '/' && tok[1] == '/')
      return true;
    return false;
  }
  //----< is this token a multiple line comment? >-----------------

  bool Toker::isMultipleLineComment(Token tok)
  {
    if (tok.size() < 2)
      return false;
    if (tok[0] == '/' && tok[1] == '*')
      return true;
    return false;
  }
  //----< is this token a double quoted string? >------------------

  bool Toker::isDoubleQuote(Token tok)
  {
    if (tok.size() == 0)
      return false;
    char ch = tok[0];
    if (ch == '@')
    {
      char nxt = tok[1];
      return (nxt == '\"');
    }
    return (ch == '\"');
  }
  //----< is this token a single-quoted character? >---------------

  bool Toker::isSingleQuote(Token tok)
  {
    if (tok.size() == 0)
      return false;
    return (tok[0] == '\'');
  }
}

#ifdef TEST_TOKER

using namespace Lexer;

class DemoToker
{
public:
  //----< tokenize a test file >-----------------------------------
  /*
    * This method allows us to easily test against several different
    * files with special cases that have to be handled correctly.
    */
  static bool testTokerWithComments(const std::string& path)
  {
    Toker toker;
    toker.doReturnComments(true);

    std::string fqf = path;  // FileSystem::Path::GetFullPath(path);
    if (!toker.open(fqf))
    {
      std::cout << "\n can't open \"" << fqf << "\"\n";
      return false;
    }
    else
    {
      std::cout << "\n  processing file: \"" << fqf << "\"";
    }
    while (!toker.isDone())
    {
      Token tok = toker.getTok();
      if (Toker::isNewLine(tok))
      {
        tok = "newline";
      }
      else if (Toker::isMultipleLineComment(tok))  // this is a cosmetic
        tok = "\n" + tok;
      std::cout << "\n -- line#" << std::setw(4) << toker.lineCount() << " " << tok;
    }
    toker.close();
    std::cout << "\n";
    return true;
  }
  static bool testTokerWithoutComments(const std::string& path)
  {
    Toker toker;
    toker.doReturnComments(false);

    std::string fqf = path;  // System.IO.Path.GetFullPath(path);
    if (!toker.open(fqf))
    {
      std::cout << "\n can't open \"" << fqf << "\"\n";
      return false;
    }
    else
    {
      std::cout << "\n  processing file: \"" << fqf << "\"\n";
    }
    while (!toker.isDone())
    {
      Token tok = toker.getTok();
      if (Toker::isNewLine(tok))
      {
        tok = "newline";
      }
      else if (Toker::isMultipleLineComment(tok))  // this is a cosmetic
        tok = "\n" + tok;
      std::cout << "\n -- line#" << std::setw(4) << toker.lineCount() << " " << tok;
    }
    toker.close();
    std::cout << "\n";
    return true;
  }
};

int main()
{
  std::cout << "\n  Demonstrate Toker class";
  std::cout << "\n =========================";

  std::string msg;
  msg += ("\n  Some things this Instructor's Solution does for CSE681 Project #2:");
  msg += ("\n  - collect comments as tokens");
  msg += ("\n  - collect double quoted strings as tokens");
  msg += ("\n  - collect single quoted strings as tokens");
  msg += ("\n  - collect specified single characters as tokens");
  msg += ("\n  - collect specified character pairs as tokens");
  msg += ("\n  - integrate with a Semiression collector");
  msg += ("\n  - provide the required package structure");
  msg += ("\n");

  //std::cout << msg;

  DemoToker::testTokerWithComments("../Tokenizer/Toker.h");
  DemoToker::testTokerWithoutComments("../Tokenizer/Toker.cpp");

  std::cout << "\n\n";
}

#endif
