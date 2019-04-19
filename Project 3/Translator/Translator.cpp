/////////////////////////////////////////////////////////////////
// Translator.cpp - Translates managed calls into native calls //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2019   //
// Author        :	Yihang Nan, Syracuse University                      //

#include "ITranslator.h"
#include "../NativeCpp/INativeCpp.h"

using namespace System;
using namespace System::Text;

public ref class Translator : ITranslator
{
public: 
  Translator()
  {
    pNativeCpp_ = createNativeCpp();
  }
  ~Translator()
  {
    delete pNativeCpp_;
    pNativeCpp_ = nullptr;
  }
  void putString(String^ str) override
  {
    pNativeCpp_->putString(mToN(str));
  }
  String^ getString() override
  {
    return nToM(pNativeCpp_->getString());
  }
  virtual List<double>^ convertToSquares(List<double>^ dbls) override
  {
    return nToM(pNativeCpp_->convertToSquares(mToN(dbls)));
  }
  virtual void transfer_file_code() override
  {
	  return pNativeCpp_->transfer_file_code();
  }
  String^ get_patterns(String^ str) override
  {
	  return nToM(pNativeCpp_->get_patterns(mToN(str)));
  }
  virtual List<String^>^ getPattern(int argc, List<String^>^ ls) override
  {
	  return vTol(pNativeCpp_->getPattern(argc, lTov(ls)));
  }
private:
  std::string mToN(String^ str);
  String^ nToM(const std::string& str);
  std::vector<double> mToN(List<double>^ listDbls);
  List<double>^ nToM(const std::vector<double>& vecDbls);
  INativeCpp* pNativeCpp_ = nullptr;
  std::vector<std::string> lTov(List<String^>^ ls);
  List<String^>^ vTol(std::vector<std::string> stl);
};

ITranslator^ TranslatorFactory::createTranslator()
{
  return gcnew Translator();
}
/*<-----------Method for transfering vector<string> to List<string>-------------->*/
List<String^>^ Translator::vTol(std::vector<std::string> stl)
{
	List<String^>^ temp = gcnew List<String^>();
	for (size_t i = 0; i < stl.size(); ++i)
		temp->Add(nToM(stl[i]));
	return temp;
}
/*<-----------Method for transfering List<string> to vector<string>-------------->*/
std::vector<std::string> Translator::lTov(List<String^> ^ ls)
{
	std::vector<std::string> temp;
	for (int i = 0; i < ls->Count; ++i)
		temp.push_back(mToN(ls[i]));
	return temp;
}

/*<-----------Method for transfering List<string> to vector<string>-------------->*/
std::string Translator::mToN(String^ str)
{
  std::string temp;
  for (int i = 0; i < str->Length; ++i)
    temp += char(str[i]);
  return temp;
}
/*<-----------Method for transfering std<string> to system<string>-------------->*/
String^ Translator::nToM(const std::string& str)
{
  StringBuilder^ sb = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    sb->Append((wchar_t)str[i]);
  return sb->ToString();
}
/*<-----------Method for transfering system<string> to std<string>-------------->*/
std::vector<double> Translator::mToN(List<double>^ dbls)
{
  std::vector<double> temp;
  for (int i = 0; i < dbls->Count; ++i)
    temp.push_back(dbls[i]);
  return temp;
}
/*<-----------Method for transfering List<string> to vector<string>-------------->*/
List<double>^ Translator::nToM(const std::vector<double>& dbls)
{
  List<double>^ temp = gcnew List<double>();
  for (size_t i = 0; i < dbls.size(); ++i)
    temp->Add(dbls[i]);
  return temp;
}

#include <iostream>

int main(array<System::String ^> ^args)
{
  ITranslator^ tran = TranslatorFactory::createTranslator();
  tran->putString("hello CSE687");
  Console::Write("\n  {0}\n", tran->getString());
  List<double>^ dbls = gcnew List<double>();
  dbls->Add(1.0);
  dbls->Add(1.5);
  dbls->Add(2.0);
  List<double>^ sqrs = tran->convertToSquares(dbls);
  Console::Write("\n  ");
  for (int i = 0; i < sqrs->Count; ++i)
    Console::Write("{0} ", sqrs[i]);
  Console::Write("\n\n");
  return 0;
}
