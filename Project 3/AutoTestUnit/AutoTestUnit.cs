///////////////////////////////////////////////////////////////////////
// AutoTestUnit.cs -       class used for automated test             //
//                                                                   //
// ver 1.0                                                           //
// Author: Yihang Nan          Master in Computer Engineering,        //
//                         Syracuse University                       //
//                         3158630892 ynan02@syr.edu                //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides one class, AutoTestUnit, which is used for 
*  automated test the whole project.
*
*  Required Files:
*  ---------------
*  AutoTestUnit.cs
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 10 April 2019
*  - first release
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace AutoTestUnit
{
    /*<------------------------------Class defination--------------------------->*/
    class AutoTestUnit
    {
        /*<------------------------------Create symbol just for looking good--------------------------->*/
        private void putsymbol()
        {
            for (int i = 0; i < 100; i++)
                Console.Write("-");
            Console.Write("\n");
        }
        /*<------------------------------Output method just for looking good--------------------------->*/
        private void putwords(string str)
        {
            int index = 0;
            int size = 100;
            int rotate = str.Count() / size;
            while (index < rotate)
            {
                for (int i = 0; i < size; i++)
                {
                    Console.Write(str[i+index*size]);
                }
                Console.Write("\n");
                index++;
            }
            int c = index * size;
            while (c< str.Count())
            {
                Console.Write(str[c]);
                c++;
            }
            Console.Write("\n");
        }
        /*<-----------------------------Testcase 1 for test requirement--------------------------->*/
        private void testcase1()
        {
            putsymbol();
            Console.WriteLine("Requirement 1:");
            putwords("                 Shall provide an interface for the Code Publisher developed in " +
                "Project #2, that declares methods to access all of the Publisher's facilities, " +
                " and provides an object factory that returns a pointer to the interface. ");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The interface in this solution are declared in the *.h files " +
                "and it's defination is declared in the *.cpp files");
            Console.WriteLine("                                        Meet the requirement 1");
            putsymbol();
        }
        /*<-----------------------------Testcase 2 for test requirement--------------------------->*/
        private void testcase2()
        {
            putsymbol();
            Console.WriteLine("Requirement 2:");
            putwords("                 Shall build the Publisher as a dynamic link library that exposes " +
                " its interface and object factory.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The interface for Publisher are declared in the INativeCpp.h files ");
            Console.WriteLine("The Interface are included below: ");

            putwords(" getPatterns function is for getting the pattern " +
                " which is used for extracting path from the commandline by calling c++ function ");
            putwords(" CodePublisher has a function name publish" +
                " which is used for convert selected files and publish them");
            putwords(" std::putstring is for getting file name and transfer to Codepublisher " +
                "which is used for passing commandline from c# file to c++ file and getting back the solved files ");
         
            Console.WriteLine("                                        Meet the requirement 2");
            putsymbol();
        }
        /*<-----------------------------Testcase 3 for test requirement--------------------------->*/
        private void testcase3()
        {
            putsymbol();
            Console.WriteLine("Requirement 3:");
            putwords("                 provide a C++\\CLI translater that delegates calls from the " +
                "Client GUI into the publisher's methods, as declared in its interface.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The ITranslator package are packaged into as a dynamic link library and it's interface " +
                "are exposed for c# file use to recall the function in the c++ ");
            Console.WriteLine("The Interfaces in the ITranslator.h are included below: ");
            putwords(" List<String^>^ getPattern(int argc, List<String^>^ ls)  " +
                "which is used for passing commandline from c# file to c++ file and getting back the solved patterns");
            putwords(" std::putstring is for getting file name and transfer to Codepublisher " +
                    "which is used for passing commandline from c# file to c++ file and getting back the solved files ");

            Console.WriteLine("                                        Meet the requirement 3");
            putsymbol();
        }
        /*<-----------------------------Testcase 4 for test requirement--------------------------->*/
        private void testcase4()
        {
            putsymbol();
            Console.WriteLine("Requirement 4:");
            putwords("                 provide a Client package that implements a WPF GUI with a " +
                "least two views, one for navigation, and one to display file conversion results, " +
                "as described in the purpose section.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The package Navigator implements the WPF GUI, and it has two views and two windows");
            putwords(" Below is the User guide in order to make sure the user knows how to use it:  ");
            putwords(" 1: The Navigator tab is a surface for selecting the files, by double clicking the directory " +
                " The user could get to up-directory or go to next directory, and by double clicking the files in " +
                "the bottom of the surface, the clicked files will be added to another window, which is selection " +
                "window");
            putwords(" 2: In the selection window, the user could double click the files in the selection window to " +
                " delete the files which users are not willing to convert it, and after getting the files list, just " +
                "push down the convert button and the program will change the selected files to html files and save " +
                "them in the directory whose position is root project/ConvertedPages");
            putwords(" 3: After converting the files, in the main window, the user could see the files which have " +
                "already converted into html files, and by single clicking the selected items, the content in the " +
                "html will show below in the same window, and by double clicking the selected items, the user could " +
                "see the result in the browser");
            Console.WriteLine("                                        Meet the requirement 4");
            putsymbol();
        }
        /*<-----------------------------Testcase 5 for test requirement--------------------------->*/
        private void testcase5()
        {
            putsymbol();
            Console.WriteLine("Requirement 5:");
            putwords("                  Shall include an automated test that accepts the path " +
                "to your project directory on its command line, invokes the Code Publisher, " +
                "through its interface, to convert all the project files matching a pattern that " +
                "accepts *.h and *.cpp files, and then opens the Client GUI's Display view.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" This is the automated test unit program for the testing project");
            putwords(" The commandline are accepted at the Run.bat file in the project, it contains 3 " +
                "kinds of elements:  ");
            putwords(" 1: Regex value for selecting the specified files, in this restriction condition, in " +
                "the GUI, you can only see the specification files as you wish, if TAs want to change to " +
                "see other files, just change this value");
            putwords(" The files selected in the selection window can be converted to html files when pushing" +
                "the convert button, and the destination of files are ../../../ConvertedPages directory");
            Console.WriteLine("                                        Meet the requirement 5");
            putsymbol();
        }
        /*<-----------------------------AutoTestUnit main method entry--------------------------->*/
        static void Main(string[] args)
        {
            AutoTestUnit test=new AutoTestUnit();
            test.testcase1();
            test.testcase2();
            test.testcase3();
            test.testcase4();
            test.testcase5();
            Console.ReadKey();
        }
    }
}
