#ifndef ITOKENCOLLECTION_H
#define ITOKENCOLLECTION_H
/////////////////////////////////////////////////////////////////////
// ITokenCollection.h - Lexical Scanner interface                  //
// ver 1.0   grammatical analysis                                  //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019       //
/////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

namespace Lexer
{
  using Token = std::string;
  using TokColl = std::vector<std::string>;

  class Toker;

  struct ITokenCollection
  {
    using iterator = std::vector<std::string>::iterator;
    virtual Toker* getToker() = 0;
    virtual void setToker(Toker* pToker) = 0;
    virtual bool open(const std::string& src) = 0;        // attach toker to source
    virtual void close() = 0;                             // close toker's source
    virtual bool get() = 0;                               // collect semi
    virtual void clone(const ITokenCollection& tc, size_t offset = 0) = 0;  // invoker clones tc
    virtual ITokenCollection* clone() const = 0;          // returns pointer to newly allocated clone
    virtual size_t size() const = 0;                      // number of tokens
    virtual Token& operator[](size_t n) = 0;              // index semi
    virtual Token operator[](size_t n) const = 0;         // index semi
    virtual iterator begin() = 0;                         // iterator start
    virtual iterator end() = 0;                           // iterator end
    virtual ITokenCollection* add(Token token) = 0;       // add a token to collection
    virtual bool remove(size_t index) = 0;                // remove token at index
    virtual bool remove(Token tok) = 0;                   // remove first tok
    virtual void addRange(TokColl tc) = 0;                // append collection of toks to semi
    virtual bool insert(size_t n, Token tok) = 0;         // insert tok at index n
    virtual void clear() = 0;                             // clear all tokens
    virtual bool contains(Token token) const = 0;         // has token?
    virtual bool find(Token tok, size_t& index, size_t offset = 0) const = 0;  // find tok if in semi
    virtual bool findLast(Token tok, size_t&index) const = 0;
    virtual Token last() const = 0;                       // return last token in semi
    virtual void trim() = 0;                              // remove leading whitespace
    virtual Token predecessor(Token tok) = 0;             // find token before tok
    virtual bool hasSequence(TokColl tokSeq) = 0;         // does semi have this sequence of tokens?
    virtual bool hasTerminator() = 0;                     // does semi have a valid terminator
    virtual bool isDone() = 0;                            // at end of tokenSource?
    virtual size_t lineCount() = 0;                       // get number of lines processed
    virtual std::string ToString() = 0;                   // concatenate tokens with intervening spaces
    virtual std::string show() const = 0;                 // display semi in string
    virtual bool isComment(Token tok) = 0;                // is this token a comment?
  };
}
#endif
