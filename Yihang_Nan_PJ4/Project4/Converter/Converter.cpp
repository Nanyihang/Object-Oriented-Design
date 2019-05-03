///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs -    Main window surface GUI                   //
//                                                                   //
// ver 1.1                                                           //
// Author: Yihang Nan          Master in Computer Engineering,       //
//                         Syracuse University                       //
//                         (315) 863-0892, ynan02@syr.edu            //
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include "Converter.h"
#include "../Display/Display.h"
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

// ---------<Initialize of class>---------------
 converter::converter(std::vector<std::string> fnames, std::vector<std::string>fpaths, std::vector<std::string>htmlpaths)
{
	createMap(fnames, fpaths, htmlpaths);
}

//----------<function for create map>-------------------
 void converter::createMap(std::vector<std::string> fnames, std::vector<std::string>fpaths, std::vector<std::string>htmlpaths)
{
	auto it1 = fnames.begin();
	auto it2 = fpaths.begin();
	auto it3 = htmlpaths.begin();
	while (it1 != fnames.end())
	{
		mp.insert(std::make_pair(*it1, *it2));
		it1++;
		it2++;
	}
	it1 = fnames.begin();
	while (it1 != fnames.end())
	{
		mp.insert(std::make_pair(*it1, *it3));
		it1++;
		it3++;
	}
}
//-----------<function for getting back the stored map>----------------------------------
 std::multimap<std::string, std::string> converter::GetMap()
{
	return mp;
}

// ---------<Function for transfering the file content to the html content>---------------
 std::string converter::FToHtml(std::string f_content)
{
	HtmlText = f_content;
	int n = HtmlText.size();
	for (int i = 0; i < n; i++)
	{
		switch (HtmlText[i])
		{
		case '\"': HtmlText.replace(i, 1, "&quot;"); i += 5; n += 5; break;
		case '&': HtmlText.replace(i, 1, "&amp;"); i += 4; n += 4; break;
		case '<': HtmlText.replace(i, 1, "&lt;"); i += 3; n += 3; break;
		case '>': HtmlText.replace(i, 1, "&gt;"); i += 3; n += 3; break;
		case ' ': HtmlText.replace(i, 1, "&nbsp;"); i += 5; n += 5; break;
		}
	}
	return HtmlText;
}
// ---------<Function for clearing file content>---------------

 void converter::FtextClear()
{
	Ftext.clear();
}
// ---------<Function for transfering the html content to the html content>---------------

 void converter::HtmltextClear()
{
	HtmlText.clear();
}
// ---------<Function for return the html content>---------------

 std::string converter::GetHtmltext()
{
	return HtmlText;
}
// ---------<Function for return the file content>---------------

 std::string converter::GetFtext()
{
	return Ftext;
}
// ---------<Function for adding the file content to member value>---------------

 std::string converter::ReadText(std::string path)
{
	std::ifstream in(path);
	std::istreambuf_iterator<char> begin(in);
	std::istreambuf_iterator<char> end;
	std::string some_str(begin, end);
	Ftext = some_str;
	return some_str;
}
//----------<Html class created function>-------
 _html_::_html_(std::string repos_dir, std::vector<std::string> names)
{
	initial("", "", "", "", "", "", "", "");
	CreateHtmlPaths(repos_dir, names);
	add_div();
	add_script();
	add_style();
}
//-----------<function for getting back html element>---------------
std::string _html_::GetEle()
{
	return ele;
}

// ---------<Initialize of class>---------------
 void _html_::initial(std::string decler = "", std::string html = "", std::string head = "", std::string style = "", std::string body = "", std::string pre = "",
	std::string div = "", std::string scr = "")
{
	ele_decler.append("<!Doctype html> \n").append(decler);
	ele_html.append("<html> \n").append(html).append("</html> \n");
	ele_head.append("<head> \n").append(head).append("</head> \n");
	ele_style.append("<style> \n").append(style).append("</style> \n");
	ele_body.append("<body> \n").append(body).append("</body> \n");
	ele_pre.append("<pre> \n").append(pre).append("</pre> \n");
	ele_div.append("<div> \n").append(div).append("</div> \n");
	ele_scr.append("<script> \n").append(scr).append("</script> \n");

	ele.append(ele_decler);
	ele.insert(ele.find(ele_decler) + ele_decler.size(), ele_html);
	ele.insert(ele.find("<html> \n") + sizeof("<html> \n") - 1, ele_head);
	ele.insert(ele.find("<head> \n") + sizeof("<head> \n") - 1, ele_style);
	ele.insert(ele.find("</style> \n") + sizeof("</style> \n") - 1, ele_scr);
	ele.insert(ele.find("</head> \n") + sizeof("</head> \n") - 1, ele_body);
	ele.insert(ele.find("<body> \n") + sizeof("<body> \n") - 1, ele_div);
	ele.insert(ele.find("</div> \n") + sizeof("</div> \n") - 1, ele_pre);
}

// ---------<Function for creating single html file path in repostory folder>---------------
 std::string _html_::filePathTrans(std::string repos_dir, std::string fname)
{
	std::string name = fname;
	std::string prepos = repos_dir;
	if (prepos.find_last_of("\\") == prepos.size() - 1)
	{
		;
	}
	else
		prepos.append("\\");

	if (name.find(".cpp") != -1)
	{
		name.erase(name.find(".cpp"), sizeof(".cpp"));
		name.append("_CPP").append(".html");
	}
	if (name.find(".h") != -1 && name.find(".html") == -1)
	{
		name.erase(name.find(".h"), sizeof(".h"));
		name.append("_H").append(".html");
	}
	return prepos + name;
}

// ---------<Function for creating html file path in repostory folder>---------------
 void _html_::CreateHtmlPaths(std::string repos_dir, std::vector<std::string> fnames)
{
	for (std::string fname : fnames)
	{
		HtmlAddress.push_back(filePathTrans(repos_dir, fname));
	}
}

// ---------<Function for creating html file in repostory folder>---------------

 void _html_::create(std::string HtmlPath)
{
	std::fstream ffile;
	ffile.open(HtmlPath, std::fstream::out);
	ffile << ele;
	std::cout << std::endl << HtmlPath << std::endl;
	ffile.close();
}

// ---------<Function for reset all html elements>---------------

 void _html_::reset_html(std::string r_decler = "", std::string r_html = "", std::string r_head = "", std::string r_style = "", std::string r_body = "", std::string r_pre = "",
	std::string r_div = "", std::string r_scr = "")
{
	ele.clear();
	ele_decler.clear();
	ele_html.clear();
	ele_head.clear();
	ele_style.clear();
	ele_body.clear();
	ele_pre.clear();
	ele_div.clear();
	ele_scr.clear();

	initial("", "", "", "", "", "", "");
	add_div();
	add_script();
	add_style();
}

// ---------<Function for adding content to pre element>---------------

 void _html_::add_pre(std::string pre = "NULL")
{
	ele.insert(ele.find("<pre> \n") + sizeof("<pre> \n") - 1, pre + "\n");
}
 // ---------<Function for adding content to head element>---------------

 void _html_::add_head(std::string head)
{
	ele.insert(ele.find("<head> \n") + sizeof("<pre> \n") - 1, head + "\n");
}

// ---------<Function for clearing ele content>---------------
 void _html_::add_script()
{
	std::string scr = "function showHideClass(className) {\n"
		"let elems = document.getElementsByClassName(className);\n"
		"if (elems === null) {\n"
		"return;\n"
		"}\n"
		"for (let i = 0; i < elems.length; ++i) {\n"
		"let displayProp = elems[i].style.display;\n"
		"if (displayProp === 'none')\n"
		"elems[i].style['display'] = \"inline\";\n"
		"else\n"
		"elems[i].style['display'] = \"none\";\n"
		"    }\n"
		"}\n";
	ele.insert(ele.find("<script> \n") + sizeof("<script> \n") - 1, scr + "\n");
}
//-----------<function for adding stytle tag elements>---------------
 void _html_::add_style()
{
	std::string sty = "body {\n"
		"padding:15px 40px;\n"
		"font-family: Consolas;\n"
		"font-size: 1.25em;\n"
		"font-weight: normal;\n"
		"}\n"
		"span{\n"
		"display:inline;\n"
		"}\n"
		"div{\n"
		"position:fixed;\n"
		"top:10px;\n"
		"right:10px;\n"
		"}\n"
		".toggleClassDisplay{}\n"
		".toggleMethodDisplay{}\n"
		".toggleCommentsDisplay{}\n";
	ele.insert(ele.find("<style> \n") + sizeof("<style> \n") - 1, sty + "\n");
}
//-----------<function for adding div tag element>---------------
 void _html_::add_div()
{
	std::string div = "<button onclick=\"showHideClass('toggleClassDisplay')\">Show/Hide Class Contents\n"
		"</button>\n"
		"<button onclick = \"showHideClass('toggleMethodDisplay')\">Show/Hide Method Contents\n"
		"</button>\n"
		"<button onclick = \"showHideClass('toggleCommentsDisplay')\">Show/Hide Comments Contenst\n"
		"</button>\n";
	ele.insert(ele.find("<div> \n") + sizeof("<div> \n") - 1, div + "\n");
}
// ---------<Function for cleaing all html elements>---------------
 void _html_::ele_clear()
{
	ele.clear();
}
// ---------<Function for return back the created html path>---------------
 std::vector<std::string> _html_::GetHtmlAddres()
{
	return HtmlAddress;
}
 /*<-----------Convert method for converting files to html------------->*/
 void Union::convert(int argc,std::vector<std::string>& argvec,std::vector<std::string>& pathvec)
{
	 std::cout << "argc is: " << argc << std::endl;
	 for (auto ele : argvec)
	 {
		 std::cout << "vect path is : "<<ele << std::endl;
	 }
	char** argv = new char*[argc];
	for (int i = 0; i < argc; i++)
	{
		argv[i] = (char*)argvec[i].c_str();
		std::cout << "Now input argv: " << argv[i] << std::endl;
	}
	std::string path;
	Executive exec(argc, argv);
	if (exec.has_command_path(argc, argv) == true)
	{
		path = exec.get_command_path();
		std::cout << "path: " <<path << std::endl;
	}
	else
		path = exec.Get_AB_root_solution_path();
	std::vector < std::string > patterns = exec.get_patterns();
	Loader ld(pathvec);
	std::string repos_dir = exec.Combine_path(exec.Get_AB_root_solution_path(), "RemoteFiles");
	exec.set_repos_path(repos_dir);
	_html_ html_(repos_dir, ld.GetAllFilesName());
	converter conv(ld.GetAllFilesName(), ld.GetAllFilesPath(), html_.GetHtmlAddres());
	std::multimap<std::string, std::string> mp = conv.GetMap();
	std::map<std::string, std::string>::iterator it;
	std::vector<std::string> names = ld.GetAllFilesName();
	auto it1 = names.begin();
	Display<ASTNode> dis;
	std::string fileSpec;
	while (it1 != names.end())  // iterate over files
	{
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
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return;
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
			std::string htmlcontent = conv.FToHtml(fcontent);
			std::string new_content = dis.ClsFuncWrap(pGlobalScope, htmlcontent);
			new_content = dis.CommentWrap(new_content);
			Dependencies<ASTNode, std::multimap<std::string, std::string>> dep(pGlobalScope, mp);
			html_.add_head(dep.insertLink());
			html_.add_pre(new_content);
			it++;
			html_.create((*it).second);
			conv.FtextClear();
			conv.HtmltextClear();
			html_.reset_html();
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
		std::cout << "\n";
		it1++;
	}
	std::cout << "\n";
	delete argv;	
}

#ifdef _Test_Converter
 
// ----------------------------<test stuhub>-------------------------------------
int main(int argc,char*argv[])
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
	while(it1!=names.end())  // iterate over files
	{
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
					continue;
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
			std::string htmlcontent = conv.FToHtml(fcontent);
			std::string new_content = dis.ClsFuncWrap(pGlobalScope, htmlcontent);
			new_content=dis.CommentWrap(new_content);
			Dependencies<ASTNode, std::multimap<std::string, std::string>> dep(pGlobalScope, mp);
			html_.add_head(dep.insertLink());
			html_.add_pre(new_content);
			it++;
			html_.create((*it).second);
			conv.FtextClear();
			conv.HtmltextClear();
			html_.reset_html();
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
		}
		std::cout << "\n";
		it1++;
	}
	std::cout << "\n";
	return 0;
}

#endif