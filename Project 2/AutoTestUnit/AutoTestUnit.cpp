/////////////////////////////////////////////////////////////////////
// AutoTestUnit.cpp - Autotestunit								   //
// ver 1.0                                                         //
// Yihang Nan, CSE687 - Object Oriented Design, Fall 2018          //
/////////////////////////////////////////////////////////////////////
#include "AutoTestUnit.h"
#ifdef DEMO_Auto
//----------------<test all requiremnts>---------------
int main(int argc, char ** argv)
{
	Tests show;
	show.test_case_1(argc,argv);
	show.test_case_2(argc, argv);
	show.test_case_3(argc, argv);
	show.test_case_4(argc, argv);
	show.test_case_5(argc, argv);
	getchar();
	getchar();
	return 0;
}
#endif