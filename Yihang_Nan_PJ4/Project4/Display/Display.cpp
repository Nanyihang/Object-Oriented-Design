///////////////////////////////////////////////////////////////////////
// Display.cpp -           class used to wrap class, function        //
//                         and comments                              //
// ver 1.1                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <vector>
#include "Display.h"
#include "../Converter/Converter.h"
#include "../Dependencies/Dependencies.h"
#include "../Executive/Executive.h"
#include "../Loader/Loader.h"
#include "../Utilities/Utilities.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Toker.h"
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../SemiExpression/Semi.h"
#include "../SemiExpression/ITokenCollection.h"
#include "../GrammarHelpers/GrammarHelpers.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

using namespace Utilities;
using namespace CodeAnalysis;
//--------------------------------<test stuhub>--------------------------------------
#ifdef _Test_Display
int main(int argc, char*argv[])
{
	std::string path;
	Executive exec(argc, argv);
	if (exec.has_command_path(argc, argv) == true)
		path = exec.get_command_path();
	else
		path = exec.Get_AB_root_solution_path();
	std::vector < std::string > patterns = exec.get_patterns();
	Loader ld(path, patterns);

	std::string repos_dir = exec.Combine_path(exec.Get_AB_root_solution_path(), "converter_test");
	exec.set_repos_path(repos_dir);

	_html_ html_(repos_dir, ld.GetAllFilesName());
	converter conv(ld.GetAllFilesName(), ld.GetAllFilesPath(), html_.GetHtmlAddres());

	std::multimap<std::string, std::string> mp = conv.GetMap();
	std::map<std::string, std::string>::iterator it;
	std::vector<std::string> names = ld.GetAllFilesName();
	auto it1 = names.begin();
	Display<ASTNode> dis;
	std::string fileSpec;

	it = mp.find(*it1);
	fileSpec = (*it).second;
	std::string msg = "Processing file" + fileSpec;
	StringHelper::title(msg);
	ConfigParseForCodeAnal configure;
	Parser* pParser = configure.Build();
	conv.ReadText((*it).second);
	std::string name;
	try
	{
		if (pParser)
		{
			name = FileSystem::Path::getName((*it).second);
			if (!configure.Attach(fileSpec))
			{
				std::cout << "\n  could not open file " << name << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return 1;
		}
		Repository* pRepo = Repository::getInstance();
		pRepo->package() = name;
		while (pParser->next())
		{
			pParser->parse();
		}
		std::cout << "\n";
		ASTNode* pGlobalScope = pRepo->getGlobalScope();
		complexityEval(pGlobalScope);
		TreeWalk(pGlobalScope);
		std::string fcontent = conv.GetFtext();
		std::cout << std::endl << fcontent << std::endl;
		std::string new_content = dis.ClsFuncWrap(pGlobalScope, fcontent);
		new_content = dis.CommentWrap(new_content);
		std::cout << std::endl << new_content;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
	}
	std::cout << "\n";
	std::cout << "\n";
	return 0;
}
#endif