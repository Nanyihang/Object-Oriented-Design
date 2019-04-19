#ifndef TOKER_H
#define TOKER_H
/////////////////////////////////////////////////////////////////////
// Toker.h - Collects words from a stream                          //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019       //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This tokenizer is implemented with the State Pattern, and:
 * - Collects words, called tokens, from a stream.  
 * - Discards all whitespace except for newlines which are returned as
 *   single character tokens.
 * - By default, collects and discards all comments, but has an option
 *   to return each comment as a single token.  
 * - Also returns quoted strings and quoted characters as tokens.
 *   Toker correctly handles the C# string @"...".
 * - This package demonstrates how to build a tokenizer based on the 
 *   State Pattern.  
  * 
 * Required Files:
 * ---------------
 * Toker.h, Toker.cpp
 * 
 * Maintenance History
 * -------------------
 * ver 1.1 : 27 Feb 2019
 * - fixed bugs in toker by checking for end-of-file in getTok() function
 *   in SingleLineCommentState and MultiLineCommentState
 * ver 1.0 : 09 Feb 2019
 * - first release - port of C# design
 */

#include <fstream>
#include <iostream>
#include <string>
#include <deque>
#include <unordered_set>
#include <cctype>
#include <locale>
#include "../CppProperties/Properties.h"

using namespace Utilities;
using Token = std::string;

namespace Lexer
{
  const std::locale loc;

  ///////////////////////////////////////////////////////////////////
  // ITokenSource interface
  // - Declares operations expected of any source of tokens
  // - Typically we would use either files or strings.  This demo
  //   provides a source only for Files, e.g., TokenFileSource, below.

  struct ITokenSource
  {
    virtual ~ITokenSource() {}
    virtual bool open(std::string path) = 0;
    virtual void close() = 0;
    virtual int next() = 0;
    virtual int peek(size_t n = 0) = 0;
    virtual bool end() = 0;
    Property<size_t> lineCount;
  };

  ///////////////////////////////////////////////////////////////////
  // ITokenState interface
  // - Declares operations expected of any token gathering state

  class TokenSourceFile;
  struct TokenContext;

  class ITokenState
  {
  public:
    virtual ~ITokenState() {}
    static void setContext(TokenContext* pContext);
    virtual Token getTok() = 0;
    virtual bool isWhiteSpace() = 0;
    virtual bool isPunctuation() = 0;
    virtual bool isDone() = 0;
    virtual ITokenState* nextState() = 0;
    Property<std::unordered_set<std::string>> oneCharTokens_;
    Property<std::unordered_set<std::string>> twoCharTokens_;

  protected:
    static TokenContext* pContext_;  // derived classes store context ref here
    TokenSourceFile* pSrc_ = nullptr;
  };

  ///////////////////////////////////////////////////////////////////
  // TokenContext class
  // - holds all the tokenizer states
  // - holds source of tokens

  struct TokenContext
  {
    ITokenState* pCurrentState_ = nullptr;
    ITokenState* pWhiteSpaceState_ = nullptr;
    ITokenState* pAlphNumState_ = nullptr;
    ITokenState* pPunctuationState_ = nullptr;
    ITokenState* pNewLineState_ = nullptr;
    ITokenState* pSingleLineCommentState_ = nullptr;
    ITokenState* pMultiLineCommentState_ = nullptr;
    ITokenState* pSingleQuoteState_ = nullptr;
    ITokenState* pDoubleQuoteState_ = nullptr;
    ITokenSource* pSrc_ = nullptr;

    TokenContext();
    ~TokenContext();
    TokenContext(const TokenContext&) = delete;
    TokenContext& operator=(const TokenContext&) = delete;
  };

  ///////////////////////////////////////////////////////////////////
  // TokenSourceFile class
  // - extracts integers from token source
  // - Streams often use terminators that can't be represented by
  //   a character, so we collect all elements as ints
  // - keeps track of the line number where a token is found
  // - uses StreamReader which correctly handles byte order mark
  //   characters and alternate text encodings.

  class TokenSourceFile : public ITokenSource
  {
  private:
    std::ifstream fs_;            // physical source of text
    std::deque<int> charQ_;       // enqueing ints but using as chars
    TokenContext* pContext_;

  public:
    TokenSourceFile(TokenContext* pContext);
    ~TokenSourceFile();
    bool open(std::string path);
    void close();
    int next();
    int peek(size_t n = 0);  // peek n ints into source without extracting them
    bool end();
  };

  ///////////////////////////////////////////////////////////////////
  // TokenState class
  // - base for all the tokenizer states

  class TokenState : public ITokenState
  {
  public:
    TokenState(const TokenState&) = delete;
    TokenState& operator=(const TokenState&) = delete;
    virtual ~TokenState() {}

    TokenState();
    static void setContext(TokenContext* pContext);
    std::unordered_set<std::string>& oneCharTokens();
    bool oneCharTokensContains(const std::string& tok);
    std::unordered_set<std::string>& twoCharTokens();
    bool twoCharTokensContains(const std::string& tok);
    bool addOneCharToken(const std::string& oneCharTok);
    bool removeOneCharToken(const std::string& oneCharTok);
    bool addTwoCharToken(const std::string& twoCharTok);
    bool removeTwoCharToken(const std::string& twoCharTok);
    bool open(const std::string& path);

    virtual Token getTok() = 0;

    bool isWhiteSpace();
    bool isNewLine();
    bool isAlphaNum();
    bool isSingleLineComment();
    bool isMultiLineComment();
    bool isDoubleQuote();
    bool isSingleQuote();
    bool isPunctuation();
    ITokenState* nextState();
    bool isDone();
    bool isEscaped(Token tok);
  };
  ///////////////////////////////////////////////////////////////////
  // Derived State Classes      getTok() returns
  // -------------------------  -------------------------------------
  /* - WhiteSpaceState          Token with space, tab, and return chars
   * - NewLineState             Token with newline
   * - AlphaNumState            Token with letters, digits, and underscore
   * - SingleLineCommentState   Token holding C++ style comment
   * - MultiLineCommentState    Token holding C style comment
   * - SingleQuoteState         Token holding a quoted character
   * - DoubleQuoteState         Token holding a quoted string
   * - PunctuationState         Token holding anything not included above
   * ----------------------------------------------------------------
   * - Each state class accepts a reference to the context in its
   *   constructor and saves in its inherited context_ property.
   * - It is only required to provide a getTok() method which
   *   returns a token conforming to its state, e.g., whitespace, ...
   * - getTok() assumes that the TokenSource's first character
   *   matches its type e.g., whitespace char, ...
   * - The nextState() method ensures that the condition, above, is
   *   satisfied.
   * - The getTok() method promises not to extract characters from
   *   the TokenSource that belong to another state.
   * - These requirements lead us to depend heavily on peeking into
   *   the TokenSource's content.
   */
   ///////////////////////////////////////////////////////////////////
   // WhiteSpaceState class
   // - extracts, from context_.src, contiguous whitespace chars as token
   // - will be thrown away by tokenizer

  class WhiteSpaceState : public TokenState
  {
  public:
    //----< keep extracting until get non-whitespace >---------------

    virtual Token getTok() override
    {
      Token tok;
      tok += (char)pContext_->pSrc_->next();     // first is WhiteSpace

      while (pContext_->pCurrentState_->isWhiteSpace())  // stop when non-WhiteSpace
      {
        tok += (char)pContext_->pSrc_->next();
      }
      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // NewLineState class
  // - extracts, from context_.src, a single newline character

  class NewLineState : public TokenState
  {
  public:
    //----< return first char in src, as it must be a newline >------

    virtual Token getTok() override
    {
      Token tok;
      tok += (char)pContext_->pSrc_->next();     // first is newline

      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // AlphaNumState class
  // - extracts contiguous letter and digit chars as a token

  class AlphaNumState : public TokenState
  {
  public:
    //----< keep extracting until get non-alphanum >-----------------

    virtual Token getTok() override
    {
      Token tok;
      tok += (char)pContext_->pSrc_->next();  // first is alphanum

      while (isAlphaNum())            // stop when non-alphanum
      {
        tok += (char)pContext_->pSrc_->next();
      }
      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // SingleLineCommentState class
  // - extracts single line comment as a token

  class SingleLineCommentState : public TokenState
  {
  public:
    //----< keep extracting until get newline >--------------

    virtual Token getTok() override
    {
      Token tok;
      tok += (char)pContext_->pSrc_->next();   // char is /
      tok += (char)pContext_->pSrc_->next();   // char is /

      char ch;
      while (true)   // stop when newline
      {
        ch = (char)pContext_->pSrc_->peek();
        if (ch == '\n' || int(ch) == -1)
          break;
        tok += (char)pContext_->pSrc_->next();
      }
      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // MulitpleLineComment class
  // - extracts multiple line comment as a token

  class MultiLineCommentState : public TokenState
  {
  public:
    //----< keep extracting until get comment termintor >------------

    virtual Token getTok() override
    {
      Token tok;
      tok += (char)pContext_->pSrc_->next();       // char is /
      tok += (char)pContext_->pSrc_->next();       // char is *

      char ch = ' ', prevCh = ' ';
      while (true)   // stop when newline
      {
        prevCh = ch;
        ch = (char)pContext_->pSrc_->next();
        tok += ch;
        if (prevCh == '*' && ch == '/' || int(ch) == -1)
          break;
      }
      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // SingleQuoteState class
  // - extracts single quoted char as a token with quotes

  class SingleQuoteState : public TokenState
  {
  public:
    //----< keep extracting until get end quote >--------------------

    virtual Token getTok() override
    {
      Token tok;
      tok += (char)pContext_->pSrc_->next();       // char is '\''

      while (true)
      {
        char ch = (char)pContext_->pSrc_->next();
        tok += ch;
        if (ch == '\'' && !isEscaped(tok))
          break;
      }
      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // DoubleQuoteState class
  // - extracts text in quotes as a token

  class DoubleQuoteState : public TokenState
  {
  public:
    //----< keep extracting until get end quote >--------------------

    virtual Token getTok() override
    {
      Token tok;
      tok += (char)pContext_->pSrc_->next();       // char is "\"" or "@"
      char nxt = (char)pContext_->pSrc_->peek();
      if (nxt == '\"' && tok[0] == '@')
        tok += (char)pContext_->pSrc_->next();

      while (true)
      {
        char ch = (char)pContext_->pSrc_->next();
        tok += ch;
        if (ch == '\"' && (!isEscaped(tok) || tok[0] == '@'))
          break;
      }
      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // PunctuationState class
  // - extracts contiguous punctuation chars as a token

  class PunctuationState : public TokenState
  {
  public:
    //----< keep extracting until get non-punctuator >---------------
    /*
     * Here is where we handle single char and two char special tokens
     * as well as other punctuators.
     */
    virtual Token getTok() override
    {
      // is this a two char special token?
      Token test;
      test += (char)pContext_->pSrc_->peek();
      test += (char)pContext_->pSrc_->peek(1);
      if (twoCharTokensContains(test))
      {
        pContext_->pSrc_->next();  // pop peeked char
        pContext_->pSrc_->next();  // pop peeked char
        return test;
      }
      // is this a single char special token?
      Token tok;
      tok += (char)pContext_->pSrc_->next();       // pop first punctuator
      if (oneCharTokensContains(tok))
        return tok;

      // not special token, so continue collecting punctuation chars
      while (pContext_->pCurrentState_->isPunctuation())
      {
        // check for other special cases starting in middle of punctuator
        if (
          isMultiLineComment() || isSingleLineComment() ||
          isDoubleQuote() || isSingleQuote()
          )
          break;
        tok += (char)pContext_->pSrc_->next();
      }
      return tok;
    }
  };
  ///////////////////////////////////////////////////////////////////
  // Toker class
  // - applications need to use only this class to collect tokens

  class Toker
  {
  private:
    TokenContext* pContext_ = nullptr;   // holds single instance of all states and token source

  public:
    Property<bool> doReturnComments;

    Toker();
    ~Toker();
    bool open(const std::string& path);
    void close();
    bool overwrite(Token tok);
    Token getTok();
    bool isDone();
    int lineCount();
    Property<std::unordered_set<std::string>>& oneCharTokens();
    Property<std::unordered_set<std::string>>& twoCharTokens();
    bool addOneCharToken(const std::string& oneCharTok);
    bool removeOneCharToken(const std::string& oneCharTok);
    bool addTwoCharToken(const std::string& twoCharTok);
    bool removeTwoCharToken(const std::string& twoCharTok);
    static bool isWhiteSpace(Token tok);
    static bool isNewLine(Token tok);
    static bool isAlphaNum(Token tok);
    static bool isPunctuator(Token tok);
    static bool isSingleLineComment(Token tok);
    static bool isMultipleLineComment(Token tok);
    static bool isDoubleQuote(Token tok);
    static bool isSingleQuote(Token tok);
  };
}
#endif
