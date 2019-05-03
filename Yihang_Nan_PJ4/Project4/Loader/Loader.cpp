///////////////////////////////////////////////////////////////////////
// Loader.cpp -              class used to load files                //
// ver 1.1                                                           //
// Author: Bo Qiu          Master in Computer Engineering,           //
//                         Syracuse University                       //
//                         (315) 278-2362, bqiu03@syr.edu            //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Loader.h"
#include "../Executive/Executive.h"


#ifdef _Test_Loader
//---------------------------<test stuhub>------------------------------------
int main(int argc, char* argv[])
{
	Executive exec(argc, argv);
	std::string path;
	if (exec.has_command_path(argc, argv) == true)
		path = exec.get_command_path();
	else
		path = exec.Get_AB_root_solution_path();
	std::vector < std::string > patterns = exec.get_patterns();
	Loader ld(path, patterns);
	ld.showFilesPath();
	ld.showFilesName();
	ld.clear_files();
	return 0;
}
#endif
