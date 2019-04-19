#include <iostream>
#include "AutoTestUnit.h"

#ifdef DEMO_CP
int main(int argc, char*argv[])
{
	Tests show;
	show.test_case_1(argc, argv);
	return 0;
}
#endif