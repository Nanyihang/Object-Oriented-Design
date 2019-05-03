///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs -    Main window surface GUI                   //
//                                                                   //
// ver 1.1                                                           //
// Author: Yihang Nan          Master in Computer Engineering,       //
//                         Syracuse University                       //
//                         (315) 863-0892, ynan02@syr.edu            //
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
*  ver 1.1 : 30 April 2019
*  - second edit
*  ver 1.0 : 09 April 2019
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
            putwords("                 Shall assemble working parts from Projects #1, #2, and #3 into a "+
                "Client-Server configuration. Unlike conventional Client-Server architectures, the Client "+
                "need not wait for a reply from the Publisher server for a request before sending additional "+
                "requests.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The project assembles project1, project2 and project3 into a Client-Serve configuration.");
            Console.WriteLine("                                        Meet the requirement 1");
            putsymbol();
        }
        /*<-----------------------------Testcase 2 for test requirement--------------------------->*/
        private void testcase2()
        {
            putsymbol();
            Console.WriteLine("Requirement 2:");
            putwords("                 Shall provide a Graphical User Interface (GUI) for the client "+
                "that supports navigating remote directories to find a project for conversion, and "+
                "supports displaying the conversion results in a way that meets Project #3 requirements.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The package Navigator implements the WPF GUI, and it has three views and two windows");
            putwords(" Below is the User guide in order to make sure the user knows how to use it:  ");
            putwords(" 1: The Navigator tab is a surface for selecting the files, by double clicking the directory " +
                " The user could get to up-directory or go to next directory, and by double clicking the files in " +
                "the bottom of the surface, the clicked files will be added to another window, which is selection " +
                "window");
            putwords(" 2: In the selection window, the user could double click the files in the selection window to " +
                " delete the files which users are not willing to convert it, and after getting the files list, just " +
                "push down the convert button and the server will transfer the selected files to html files and save " +
                "them in the server directory which is ../RemoteFiles");
            putwords(" 3: After converted files in the server, the user could click the RemoteFiles Tabitem to see the files "+
                "in the server, and by single clicking the file name or click the DownLoad button, the files will be downloaded "+
                "into local client directory whose path is ../LocalStorage");
            putwords(" 4: Click the LocalStorage Tabitem, the user could single click the file to open a pop-up window to see "+
                "the file content or double click the file to open a browser to see the file content");
            Console.WriteLine("                                        Meet the requirement 2");
            putsymbol();
        }
        /*<-----------------------------Testcase 3 for test requirement--------------------------->*/
        private void testcase3()
        {
            putsymbol();
            Console.WriteLine("Requirement 3:");
            putwords("                 provide message designs appropriate for this application. All messages "+
                "are instances of the same Message class, but have a specified set of attributes and body contents "+
                "suited for the intended task.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The messages are designed into 11 patterns, shown the function below: ");
            putwords(" 1. Command-echo, the server will send same message back to the client ");
            putwords(" 2. Command-getFiles, the server will send message containing all the files in the specificed directory ");
            putwords(" 3. Command-getDirs, the server will send  message containing all the directories in the specificed directory"+
                "back to the client ");
            putwords(" 4. Command-sendFile, the server will send files from server to the client ");
            putwords(" 5. Command-codeAnalyze, the server will send  message containing codeAnalyze results to client ");
            putwords(" 6. Command-convertfiles, the server will transform files into html format in the server and save them "+
                "into the directory ../RemoteFiles ");
            putwords(" 7. Command-serverQuit, the server will quit ");
            putwords(" 8. Command-getCmd, the server will send seperate command paraments back to the client ");
            putwords(" 9. Command-DownLoad, the server will send files into client and save them into the directory ../LocalStorage ");
            putwords(" 10. Command-getRemoteFiles, the server will send to show the files in the server ");
            putwords(" 11. Command-singleDownload, the server will send single file into client and save them into the directory "+
                "../LocalStorage ");
            Console.WriteLine("                                        Meet the requirement 3");
            putsymbol();
        }
        /*<-----------------------------Testcase 4 for test requirement--------------------------->*/
        private void testcase4()
        {
            putsymbol();
            Console.WriteLine("Requirement 4:");
            putwords("                 Shall support converting source code in the server and, with a separate request, "+
                "transferring one or more converted files back to the local client, using the communication channel.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords(" The files are converted in the server, and with the click button or double click RemoteFiles request " +
                "of client, the server will send transformed files back to the client and save them into the directory ../LocalStorage");
            Console.WriteLine("                                        Meet the requirement 4");
            putsymbol();
        }
        /*<-----------------------------Testcase 5 for test requirement--------------------------->*/
        private void testcase5()
        {
            putsymbol();
            Console.WriteLine("Requirement 5:");
            putwords("                  Shall demonstrate correct operations for two or more concurrent clients.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords("  The other clients are runned by the command in the run.bat file, as it shown now");
            Console.WriteLine("                                        Meet the requirement 5");
            putsymbol();
        }
        /*<-----------------------------Testcase 6 for test requirement--------------------------->*/
        private void testcase6()
        {
            putsymbol();
            Console.WriteLine("Requirement 6:");
            putwords("                  Shall include an automated test that accepts the server url and remote path "+
                "to your project directory on its command line, invokes the Code Publisher, through its interface, to " +
                "convert all the project files matching a pattern that accepts *.h and *.cpp files, and then opens the " +
                "Client GUI's Display view.");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            putwords("  The server url is not set in the run.bat file, because i think it is unreasonable to set such a parameter " +
                " the regex pattern, browsepath and default root path are adjustable in the run.bat file");
            Console.WriteLine("                                        Meet the requirement 6");
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
            test.testcase6();
            Console.ReadKey();
        }
    }
}
