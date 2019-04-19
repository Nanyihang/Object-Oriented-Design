#pragma once
/////////////////////////////////////////////////////////////////
// ITranslator.h - Translates managed calls into native calls  //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019   //
// Author        :	Yihang Nan, Syracuse University            //
/////////////////////////////////////////////////////////////////
/*
 * This package is responsible for translating managed types into
 * native types and for delegating managed calls to native calls.
 *
 * This file declares an interface and object factory for C# clients
 * to use, isolating them from the Translator implementation.
 * 
 * For C# clients to use this package it needs to be built as a dll,
 * and clients need to make a reference to the Translator project.
 *
 * In order to create a C++\CLI project you need to have installed
 * C++\CLI support.  See CSE687\Presentations\CreatingCppCli.pdf.
 */
using namespace System;
using namespace System::Collections::Generic;

public ref class ITranslator abstract
{
public:
  virtual void putString(String^ str) = 0;
  virtual String^ getString() = 0;
  virtual List<double>^ convertToSquares(List<double>^ dbles) = 0;
  virtual void transfer_file_code() = 0;
  virtual String^ get_patterns(String^ str) = 0; // TODO : get files vector     
  virtual List<String^>^ getPattern(int argc, List<String^>^ ls) = 0;
};

public ref class TranslatorFactory
{
public:
  static ITranslator^ createTranslator();
};






 