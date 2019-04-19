#ifndef SEMI_H
#define SEMI_H
/////////////////////////////////////////////////////////////////////
// Semi.h - Collects groups of tokens that are useful for          //
// ver 1.1   grammatical analysis                                  //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019       //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * - This package contains a Semi class that implements ITokenCollection
 *   interface.  It also contains a Factory class that creates instances
 *   of Semi.
 * - Semiressions are collections of tokens that are useful for 
 *   detecting specific grammatical constructs.  It is important that
 *   each instance of Semi contains all tokens necessary to make a 
 *   decision about a grammatical construct, e.g., is this a class?
 * - It is also important that each Semi instance does not contain 
 *   tokens relevant for more than one detection.
 * - This code demonstrates how to build a semi-expression collector,
 *   using the state-based tokenizer. 
 * 
 * Required Files:
 * ---------------
 * Semi.h, Semi.cpp,
 * ITokenCollection.h, 
 * Toker.h, Toker.cpp
 * 
 * Maintenance History
 * -------------------
 * ver 1.1 : 27 Feb 2019
 * - fixed bug in fold() by returning if range-based for
 * ver 1.0 : 09 Jan 2019
 * - first release
 * 
 * Note:
 * - This is a redesign of the Semi package you will find in the Parser folder
 *   in the Repository.
 */

#include <string>
#include <vector>
#include "ITokenCollection.h"
#include "../Tokenizer/Toker.h"

namespace Lexer
{
  ///////////////////////////////////////////////////////////////////
  // Semi class
  // - collects tokens from Lexer::Toker.
  // - terminates collection on collecting the special tokens:
  //     ";", "{", "}", and "\n" when the first character is "#"
  // - It implements the IEnumerable interface, which is a contract
  //   to return an Enumerater object that foreach uses to step through
  //   items in a collection.
  // - It also implements the ITokenCollection interface that the
  //   parser will use to extract token sequences for rule-checking.

  class Semi : public ITokenCollection
  {
  private:
    TokColl toks;  // private collection of tokens
    Toker* pToker_ = nullptr;

  public:
    Semi(bool createToker = false);
    Semi(std::vector<Token> list, bool createToker = false);
    virtual ~Semi();

    virtual Toker* getToker() override;
    virtual void setToker(Toker* pToker) override;
    virtual void addRange(TokColl coll) override;
    virtual bool open(const std::string& source) override;
    virtual void close() override;
    virtual size_t size() const override;
    virtual std::string ToString() override;
    virtual size_t lineCount() override;
    bool isTerminator(Token tok);
    virtual void trim() override;
    virtual bool contains(Token tok) const override;
    // note find functions return index reference of founct token position
    virtual bool find(Token tok, size_t& index, size_t offset = 0) const override;   // find tok if in semi
    virtual bool findLast(Token tok, size_t&index) const override;
    virtual Token predecessor(Token tok) override;
    virtual Token last() const override;
    virtual bool hasSequence(TokColl tokSeq) override;
    void fold();
    virtual bool get() override;
    virtual void clone(const ITokenCollection& tc, size_t offset = 0) override;
    virtual ITokenCollection* clone() const override;
    virtual Token& operator[](size_t i) override;
    virtual Token operator[](size_t i) const override;
    virtual iterator begin() override;
    virtual iterator end() override;
    virtual ITokenCollection* add(Token token) override;
    virtual bool remove(size_t index) override;
    virtual bool remove(Token tok) override;
    virtual void clear() override;
    virtual bool isDone() override;
    virtual bool insert(size_t n, Token tok) override;
    virtual bool hasTerminator() override;
    virtual std::string show() const override;
    virtual bool isComment(Token tok) override;
  };

  class Factory
  {
  public:
    static ITokenCollection* create(bool createToker = false)
    {
      Semi* rtn = new Semi(createToker);
      return rtn;
    }
  };
}

#endif
