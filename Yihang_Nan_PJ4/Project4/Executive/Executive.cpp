///////////////////////////////////////////////////////////////////////
// Executive.cpp           class used to handle the path             //
// ver 1.1                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include "Executive.h"
#include <string>



// ---------<Function for return back the command path>---------------

std::string Executive::get_command_path()
{
	return command_path;
}

// ---------<Function for return back the browser path>---------------
std::string Executive::get_browser_path()
{
	return browser_path;
}
// ---------<Function for return back the patterns>---------------
std::vector<std::string> Executive::get_patterns()
{
	return patterns;
}


// ---------<Determine whether there is an input of command path>---------------

bool Executive::has_command_path(int argc, char*argv[])
{
	bcmdpath = false;
	command_path.clear();
	for (int i = 0; i < argc; i++)
	{
		std::string x = *argv;
		if (x.find("\\") != -1 || x.find("/") != -1)
		{
			if (x.find(".exe") == -1)
			{
				bcmdpath = true;
				command_path = x;
			}
		}
		argv++;
	}
	return bcmdpath;
}
// ---------<Get the browser path from the command line and save it in the member value>---------------
void Executive::Gbropath(int argc, char*argv[])
{
	browser_path.clear();
	for (int i = 0; i < argc; i++)
	{
		std::string x = *argv;
		if (x.find(".exe") != -1)
		{
			browser_path = x;
		}
		argv++;
	}
	if (browser_path == "")
		std::cout << std::endl << "  Error: The browser path are wrong, please input the right path" << std::endl;
}

// ---------<Get the patterns from the command line and save it in the member value>---------------
void Executive::Gpatterns(int argc, char*argv[])
{
	patterns.clear();
	for (int i = 0; i < argc; i++)
	{
		std::string x = *argv;
		if (x == ".cpp" || x == ".h" || x == ".txt" || x == ".pdf")
		{
			patterns.push_back(".*\\" + x);
		}
		argv++;
	}
	if (patterns.size() == 0)
	{
		patterns.push_back(".*");
	}
}

// ---------<Get back the absolute path of current working path>---------------
std::string Executive::Get_AB_curworking_path()
{
	return AB_curworking_path;
}
// ---------<Getting the absolute path of the solution path>---------------
std::string Executive::Get_AB_root_solution_path()
{
	return AB_root_solution_path;
}
// ---------<Getting the absolute c# path of the solution path>---------------

std::string Executive::Get_CSAB_root_solution_path()
{
	return CSAB_root_solution_path;
}
// ---------<Class Initialize>---------------
 Executive::Executive(int argc, char*argv[])
{
	cwp();
	wpTosp();
	cswpTosp();
	has_command_path(argc, argv);
	Gbropath(argc, argv);
	Gpatterns(argc, argv);
}
 // ---------<Class Initialize>---------------
 Executive::Executive()
 {

 }
// ---------<Get the absolute path of current working path and store in the member value>---------------
void Executive::cwp()
{
	char *buffer;
	buffer = _getcwd(NULL, 0);
	AB_curworking_path = buffer;
	free(buffer);
}
// ---------<Get the absolute path of solution path and store in the member value>---------------
void Executive::wpTosp()
{
	std::string sub = AB_curworking_path;
	int i = sub.find_last_of("\\");
	sub = sub.substr(0, i);
	AB_root_solution_path = sub;
}
// ---------<Get the c# absolute path of solution path and store in the member value>---------------
void Executive::cswpTosp()
{
	std::string sub = AB_curworking_path;
	int i = sub.find_last_of("\\");
	sub = sub.substr(0, i);
	i = sub.find_last_of("\\");
	sub = sub.substr(0, i);
	i = sub.find_last_of("\\");
	sub = sub.substr(0, i);
	CSAB_root_solution_path = sub;
}
// ---------<A public member function for combining to path together>---------------
std::string Executive::Combine_path(std::string str1, std::string str2)
{
	std::string temp = str1;
	if (temp.find_last_of("\\") != temp.size() - 1)
	{
		temp.append("\\").append(str2);
	}
	else
		temp.append(str2);
	return temp;
}

// ---------<Function for create the repostory folder>---------------

void Executive::set_repos_path(std::string path)
{
	path.append("\\");
	_mkdir(path.c_str());
}

#ifdef _Test_Executive
/*
//------------------------------------------<test stuhub>------------------------------
int main(int argc,char*argv[])
{
	Executive exec(argc,argv);
	if (exec.has_command_path(argc,argv) == true)
		std::cout << std::endl << "find the command path ---"+ exec.get_command_path() << std::endl;
	else
		std::cout << std::endl << " we cant find the command path" << std::endl;


	std::cout << exec.Get_AB_curworking_path() << std::endl;

	std::cout << exec.Get_AB_root_solution_path() << std::endl;

	std::cout << exec.get_browser_path() << std::endl;

	std::cout << exec.get_command_path() << std::endl;

	for (std::string pt : exec.get_patterns())
		std::cout << pt << std::endl;

	std::string repos = exec.Combine_path(exec.Get_AB_root_solution_path(), "convertedpages");
	std::cout << repos << std::endl;

	std::cout << exec.Combine_path(repos, "converted.html") << std::endl;

	return 0;
}
*/
#endif

