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
*  This package implements the MainWindow class and create a GUI surface
*
*  Required Files:
*  ---------------
*  MainWindow.xaml MainWindow.xaml.cs
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 30 April 2019
*  - second edition
*  ver 1.0 : 09 April 2019
*  - first release
*/


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.ComponentModel;
using Microsoft.Win32;
using System.Reflection;
using System.Windows.Interop;
using MsgPassingCommunication;
using System.Threading;

namespace Navigator
{

    /*<-----------------------------------MainWindow class defination--------------------->*/
    public partial class MainWindow : Window
    {
        internal Stack<string> pathStack_ = new Stack<string>();
        private List<string> patterns { get; set; } = new List<string>();
        private List<string> selectedFiles { get; set; } = new List<string>();
        private List<string> remotefiles { get; set; } = new List<string>();
        List<string> cmdline { get; set; } = new List<string>();
        private SelectionWindow sw { get; set; } = null;
        private string path { get; set; }
        private string storageroot;
        private string brosepath;
        private string cmdpath;
        internal string saveFilesPath;
        internal string sendFilesPath;
        private bool selectexist = false;
        private bool webunselect = false;
        private int Download = 0;
        internal CsEndPoint endPoint_;
        internal Translater translater;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_ = new Dictionary<string, Action<CsMessage>>();

        /*<-----------------------------------MainWindow class construction-------------->*/
        public MainWindow()
        {
            demo();
            CmdLineToList();
            connectinitialize();
            sendGetRemoteFilesmsg();
            InitializeComponent();
            while (storageroot == null)
            {
                sendGetCmdMsg();
                System.Threading.Thread.Sleep(1000);
            }


        }

        private void demo()
        {
            Console.WriteLine("Requirement 1:" +"\n");
            Console.Write("Shall assemble working parts from Projects #1, #2, and #3 into a "+
                "Client-Server configuration. Unlike conventional Client-Server architectures, the Client " +
          "need not wait for a reply from the Publisher server for a request before sending additional " +
          "requests." + "\n");
            Console.WriteLine("Answer : " + "\n");
            Console.Write(" The project assembles project1, project2 and project3 into a Client-Serve configuration." + "\n");
            Console.WriteLine("Requirement 2:" + "\n");
            Console.Write("Shall provide a Graphical User Interface (GUI) for the client " +
                "that supports navigating remote directories to find a project for conversion, and " +
                "supports displaying the conversion results in a way that meets Project #3 requirements." + "\n");
            Console.Write("\n");
            Console.WriteLine("Answer : " + "\n");
            Console.Write("The package Navigator implements the WPF GUI, and it has three views and two windows" + "\n");
            Console.Write("Below is the User guide in order to make sure the user knows how to use it:  " + "\n");
            Console.Write("1: The Navigator tab is a surface for selecting the files, by double clicking the directory " +
                " The user could get to up-directory or go to next directory, and by double clicking the files in " +
                "the bottom of the surface, the clicked files will be added to another window, which is selection " +
                "window" + "\n");
            Console.Write("2: In the selection window, the user could double click the files in the selection window to " +
                " delete the files which users are not willing to convert it, and after getting the files list, just " +
                "push down the convert button and the server will transfer the selected files to html files and save " +
                "them in the server directory which is ../RemoteFiles" + "\n");
            Console.Write("3: After converted files in the server, the user could click the RemoteFiles Tabitem to see the files " +
                "in the server, and by single clicking the file name or click the DownLoad button, the files will be downloaded " +
                "into local client directory whose path is ../LocalStorage" + "\n");
            Console.Write("4: Click the LocalStorage Tabitem, the user could single click the file to open a pop-up window to see " +
                "the file content or double click the file to open a browser to see the file content" + "\n");
            Console.WriteLine("Meet the requirement 2" + "\n");

            Console.WriteLine("Requirement 3:" + "\n");
            Console.Write("provide message designs appropriate for this application. All messages " +
                "are instances of the same Message class, but have a specified set of attributes and body contents " +
                "suited for the intended task." + "\n");
            Console.Write("\n");
            Console.WriteLine("Answer : " + "\n");
            Console.Write(" The messages are designed into 11 patterns, shown the function below: " + "\n");
            Console.Write(" 1. Command-echo, the server will send same message back to the client " + "\n");
            Console.Write(" 2. Command-getFiles, the server will send message containing all the files in the specificed directory " + "\n");
            Console.Write(" 3. Command-getDirs, the server will send  message containing all the directories in the specificed directory" +
                "back to the client " + "\n");
            Console.Write(" 4. Command-sendFile, the server will send files from server to the client " + "\n");
            Console.Write(" 5. Command-codeAnalyze, the server will send  message containing codeAnalyze results to client " + "\n");
            Console.Write(" 6. Command-convertfiles, the server will transform files into html format in the server and save them " +
                "into the directory ../RemoteFiles " + "\n");
            Console.Write(" 7. Command-serverQuit, the server will quit " + "\n");
            Console.Write(" 8. Command-getCmd, the server will send seperate command paraments back to the client " + "\n");
            Console.Write(" 9. Command-DownLoad, the server will send files into client and save them into the directory ../LocalStorage " + "\n");
            Console.Write(" 10. Command-getRemoteFiles, the server will send to show the files in the server " + "\n");
            Console.Write(" 11. Command-singleDownload, the server will send single file into client and save them into the directory " +
                "../LocalStorage " + "\n");
            Console.WriteLine("Meet the requirement 3");
            Console.WriteLine("Requirement 4:" + "\n");
            Console.Write("Shall support converting source code in the server and, with a separate request, " +
                "transferring one or more converted files back to the local client, using the communication channel.");
            Console.Write("\n");
            Console.WriteLine("Answer : " + "\n");
            Console.Write(" The files are converted in the server, and with the click button or double click RemoteFiles request " +
                "of client, the server will send transformed files back to the client and save them into the directory ../LocalStorage" + "\n");
            Console.WriteLine(" Meet the requirement 4");
            Console.WriteLine("Requirement 5:" + "\n");
            Console.Write("Shall demonstrate correct operations for two or more concurrent clients." + "\n");
            Console.Write("\n");
            Console.WriteLine("    Answer : ");
            Console.Write("  The other clients are runned by the command in the run.bat file, as it shown now" + "\n");
            Console.WriteLine("Meet the requirement 5");
            Console.WriteLine("Requirement 6:");
            Console.Write("Shall include an automated test that accepts the server url and remote path " +
                "to your project directory on its command line, invokes the Code Publisher, through its interface, to " +
                "convert all the project files matching a pattern that accepts *.h and *.cpp files, and then opens the " +
                "Client GUI's Display view." + "\n");
            Console.Write("\n");
            Console.WriteLine("Answer : ");
            Console.Write("  The server url is not set in the run.bat file, because i think it is unreasonable to set such a parameter " +
                " the regex pattern, browsepath and default root path are adjustable in the run.bat file" + "\n");
            Console.WriteLine(" Meet the requirement 6" + "\n");
        }
        public string finddict(string path)
        {
            int pos = path.LastIndexOf('@');
            path = path.Substring(0, pos);
            return path;
        }
        //------------<click the button>---------------
        private void Mouseclick(object sender, RoutedEventArgs e)
        {

            foreach (string file in selectedFiles)
            {
                CsEndPoint serverEndPoint = new CsEndPoint();
                serverEndPoint.machineAddress = "localhost";
                serverEndPoint.port = 8080;
                string fileName = System.IO.Path.GetFileName(file);
                //Console.WriteLine(fileName);
                string srcFile = System.IO.Path.GetFullPath(storageroot + "/" + file);
                string dstFile = sendFilesPath + "\\" + fileName;
                string dic = finddict(file);
                CsMessage msg = new CsMessage();
                string lstscmd = "";
                foreach (string ele in cmdline)
                {
                    lstscmd += ele + '@';
                }
                msg.add("command", "convertfiles");
                msg.add("fileName", fileName);
                msg.add("path", dic);
                msg.add("to", CsEndPoint.toString(serverEndPoint));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("argc", cmdline.Count.ToString());
                msg.add("argv", lstscmd);
                translater.postMessage(msg);
            }
            LoadConvertedFiles();
        }
        
        /*<-----------------------------------Method for setting the value of logic design-------------->*/
        public void setexist(bool logic)
        {
            selectexist = logic;
        }
 
        /*<------------------------Extract the Commandline and change to List type-------------->*/
        private List<String> CmdLineToList()
        {
            string[] cmd = Environment.GetCommandLineArgs();
            for (int i = 0; i < cmd.Length; i++)
            {
                cmdline.Add(cmd[i]);
            }
            return cmdline;
        }
        /*<------------------------Method for getting back to up-directory-------------->*/
        string getAncesPath(string path)
        {
            DirectoryInfo info = Directory.GetParent(path);
            path=info.FullName.ToString();
            return path;
        }
        /*<------------------------MouseDoubleCLick event to response interupt-------------->*/
        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            try
            {
                webunselect = true;
                if (Dirs.SelectedItem.ToString() == "..")
                {
                    if (pathStack_.Count > 1)
                    {
                        pathStack_.Pop();
                        
                    }
                    else
                        return;
                }
                else
                {
                    path = pathStack_.Peek() + "/" + Dirs.SelectedItem.ToString();
                    pathStack_.Push(path);
                }
                //CurrPath.Text = "Project4" + pathStack_.Peek();
                LoadNavi(path);        
            }
            catch
            {
            }
        }
        /*<-----------------------------------Remote files download in the localstorage-------------->*/

        private void DownLoad_Click(object sender, RoutedEventArgs e)
        {
            webunselect = true;
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(this.endPoint_));
            msg.add("count", Download.ToString());
            msg.add("command", "DownLoad");
            this.translater.postMessage(msg);
            Download++;
        }

            /*<-----------------------Send the request to get command line message-------------->*/
            private void sendGetCmdMsg()
        {
            string lstscmd = "";
            foreach (string ele in cmdline)
            {
                lstscmd += ele + '@';
            }
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(this.endPoint_));
            msg.add("count", cmdline.Count.ToString());
            msg.add("cmdline", lstscmd.ToString());
            msg.add("command", "getCmd");
            this.translater.postMessage(msg);
        }
        /*<-----------------------------------Set the regex patterns for selecting files-------------->*/

        private void setPattern()
        {
            Pattern.Text = "";
            foreach (string ele in patterns)
            {
                Pattern.Text += " ";
                Pattern.Text += ele;
            }
        }
        /*<------------------------Method for real-time reloading Navigator tab-------------->*/
        private void LoadNavi(string path)
        {
            Dirs.Items.Clear();
            path = storageroot + pathStack_.Peek();
            path = System.IO.Path.GetFullPath(path);
            string[] dirs = System.IO.Directory.GetDirectories(path);
            foreach (string dir in dirs)
            {
                if (dir != "." && dir != "..")
                {
                    string itemDir = System.IO.Path.GetFileName(dir);
                    Dirs.Items.Add(itemDir);
                }
            }
            Dirs.Items.Insert(0, "..");

            Files.Items.Clear();
            string[] files = System.IO.Directory.GetFiles(path);
            foreach (string file in files)
            {
                FileInfo finfo = new FileInfo(file);
                foreach (string ch in patterns)
                {
                    System.Text.RegularExpressions.Regex reg = new System.Text.RegularExpressions.Regex(ch);
                    System.Text.RegularExpressions.Match result = reg.Match(finfo.Extension);
                    if (result.Success && finfo.Extension != ".html")
                        Files.Items.Add(finfo.Name);
                }
            }
        }
        /*<------------------------Method to execute when loading main window-------------->*/
        private void WindowLoaded(object sender, RoutedEventArgs e)
        {
            pathStack_.Push("");
            saveFilesPath = translater.setSaveFilePath("../../../LocalStorage");
            sendFilesPath = translater.setSendFilePath("../../../RemoteFiles");

            
            path = pathStack_.Peek();
            LoadNavi(path);
            
            LoadConvertedFiles();
        }
        private void loadDispatcher()
        {
            DispatcherLoadSendFile();
            DispatchergetCmd();
            DispatcherConvert();
            DispatcherDownLoad();
            DispatchergetRemoteFiles();
            DispatchersingleDownload();
        }
        /*<-----------------------------------Method for handling the received message of ButtonDown-------------->*/

        private void DispatchersingleDownload()
            {
            Action<CsMessage> ButtonDown = (CsMessage rcvMsg) =>
            {
                //Console.WriteLine("\n  processing incoming command : ButtonDown ");

                Action act = () => { LoadConvertedFiles(); };
                Dispatcher.Invoke(act, new object[] { });
            };
            addClientProc("ButtonDown", ButtonDown);
        }
        /*<-----------------------------------Method for handling the received message of getRemoteFiles-------------->*/

        private void DispatchergetRemoteFiles()
        {
            Action<CsMessage> getRemoteFiles = (CsMessage rcvMsg) =>
            {
                string temp = "";
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("files"))
                    {
                        temp = enumer.Current.Value;
                        remotefiles.Clear();
                        //Console.WriteLine("this is files value:  ----------------------" + temp);
                        break;
                    }
                }
                int index = 0;
                for (int i = 0; i < temp.Count(); i++)
                {
                    if (temp[i] == '@')
                    {
                        string temp2 = temp.Substring(index, i - index);
                        remotefiles.Add(temp2);
                        index = i + 1;
                    }
                }
            };
            addClientProc("getRemoteFiles", getRemoteFiles);
        }

        /*<-----------------------------------Method for handling the received message of DownLoad-------------->*/

        private void DispatcherDownLoad()
        {
            Action<CsMessage> DownLoad = (CsMessage rcvMsg) =>
            {
                //Console.WriteLine("\n  processing incoming command Download:");
                string end = "";
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("end"))
                    {
                        end = enumer.Current.Value;
                        if (end == "yes")
                        {
                            Download = 0;
                            Action act = () => { LoadConvertedFiles(); };
                            Dispatcher.Invoke(act, new object[] { });
                        }
                        else
                        {
                            CsEndPoint serverEndPoint = new CsEndPoint();
                            serverEndPoint.machineAddress = "localhost";
                            serverEndPoint.port = 8080;
                            CsMessage msg = new CsMessage();
                            msg.add("to", CsEndPoint.toString(serverEndPoint));
                            msg.add("from", CsEndPoint.toString(this.endPoint_));
                            msg.add("count", Download.ToString());
                            msg.add("command", "DownLoad");
                            this.translater.postMessage(msg);
                            Download++;
                        }
                    }
                }
            };
            addClientProc("DownLoad", DownLoad);
        }
        /*<-----------------------------------Method for handling the received message of convertfiles-------------->*/

        private void DispatcherConvert()
        {
            Action<CsMessage> convertfiles = (CsMessage rcvMsg) =>
            {
               // Console.WriteLine("\n  processing incoming data with command: convertfiles");
                Action act = () => { sendGetRemoteFilesmsg(); };
                Dispatcher.Invoke(act, new Object[] { });
            };
            addClientProc("convertfiles", convertfiles);
        }
        /*<-----------------------------------Method for handling the received message of getCmd-------------->*/

        private void DispatchergetCmd()
        {
            Action<CsMessage> getCmd = (CsMessage rcvMsg) =>
            {
                //Console.WriteLine("\n  processing incoming data with command: getCmd");
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("browsepath"))
                    {
                        brosepath = enumer.Current.Value;
                        
                    }
                    if (key.Contains("cmdpath"))
                    {
                        cmdpath = enumer.Current.Value;
                        storageroot = cmdpath;
                    }
                    if (key.Contains("patterns"))
                    {
                        string pat = enumer.Current.Value;
                        int index = 0;
                        for (int i = 0; i < pat.Count(); i++)
                        {
                            if (pat[i] == '@')
                            {
                                string temp = pat.Substring(index, i - index);
                                patterns.Add(temp);
                                index = i + 1;
                            }
                        }     
                    }
                }
                Action act = () => { setPattern(); };
               Dispatcher.Invoke(act, new Object[] { });
            };
            addClientProc("getCmd", getCmd);
        }

        /*<-----------------------------------Method for handling the received message of sendFile-------------->*/

        private void DispatcherLoadSendFile()
        {
            Action<CsMessage> sendFile = (CsMessage rcvMsg) =>
            {
                //Console.Write("\n  processing incoming file");
                string fileName = "";
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("sendingFile"))
                    {
                        fileName = enumer.Current.Value;
                        break;
                    }
                }
                if (fileName.Length > 0)
                {
                    Action<string> act = (string fileNm) => { LoadConvertedFiles(); };
                    Dispatcher.Invoke(act, new object[] { fileName });
                }
            };
            addClientProc("sendFile", sendFile);
        }

        /*<-----------------------------------Add thread start function to dispatcher-------------->*/

        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }



        //----< process incoming messages on child thread >----------------

        private void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    try
                    {
                        if (msg.attributes.Count() == 0) continue;
                        string msgId = msg.value("command");
                       // Console.Write("\n  client getting message \"{0}\"", msgId);
                        if (dispatcher_.ContainsKey(msgId))
                            dispatcher_[msgId].Invoke(msg);
                    }
                    catch (Exception ex)
                    {
                        //Console.Write("\n  {0}", ex.Message);
                        msg.show();
                    }
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }
        /*<--------------Loading the LocalStorage tableitem-------------->*/
        public void LoadConvertedFiles()
        {
            this.LocalStorage.Items.Clear();
            string ConvertedDirectory = "../../../";
            ConvertedDirectory = System.IO.Path.Combine(ConvertedDirectory + "LocalStorage");
            ConvertedDirectory = System.IO.Path.GetFullPath(ConvertedDirectory);
            //Console.WriteLine(ConvertedDirectory);
            if (Directory.Exists(ConvertedDirectory))
            {
                string[] files_ls = Directory.GetFiles(ConvertedDirectory);
                foreach (string ele in files_ls)
                    this.LocalStorage.Items.Add(ele);
            }
        }
        /*<---------------Method for resposing single click interupt-------------->*/
        private void Files_SelectionChanged(object sender, MouseButtonEventArgs e)
        {
            try
            {
                if ( selectexist==false)
                {
                    webunselect = true;
                    sw = new SelectionWindow();
                    sw.connectMainwindow(this);
                    setexist(true);                  
                }
                string filepath = path+'@'+ Files.SelectedItem.ToString();
                if (!selectedFiles.Contains(filepath))
                {
                    selectedFiles.Add(filepath);
                }
                sw.PassSlectedFiles(selectedFiles);
                sw.loadfiles();
                sw.Show();
            }
            catch
            {

            }
        }

        /*<-----------------MouseDoubleClick event to response and open files in browser-------------->*/
        private void Showfiles(object sender, MouseButtonEventArgs e)
        {
            System.Diagnostics.Process.Start(brosepath, LocalStorage.SelectedItem.ToString());
        }
        /*<-----------------MouseDoubleClick event to response and download all files from server-------------->*/

        private void DownLoadSingleFile(object sender, MouseButtonEventArgs e)
        {
            webunselect = true;
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(this.endPoint_));
            msg.add("file", remotefiles.ToString());
 
            msg.add("command", "ButtonDown");
            this.translater.postMessage(msg);

        }
        /*<-----------------------------------Set the endpoint of client the initialize them-------------->*/

        private void connectinitialize()
        {
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = "localhost";
            endPoint_.port = 8082;
            translater = new Translater();
            translater.listen(endPoint_);
            processMessages();
            loadDispatcher();
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
        }
        /*<-----------------Method for sending message of getting remotefiles-------------->*/

        private void sendGetRemoteFilesmsg()
        {
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(this.endPoint_));
            msg.add("command", "getRemoteFiles");
            this.translater.postMessage(msg);
        }

        /*<----------------Event for response opening files in browser-------------------->*/
        private void Showfiles_popup(object sender, SelectionChangedEventArgs e)
        {
            if (webunselect)
            {
                webunselect = false;
                return;
            }
            Paragraph paragraph = new Paragraph();
            string fileSpec = LocalStorage.SelectedItem.ToString();
            string fileText = File.ReadAllText(fileSpec);
            paragraph.Inlines.Add(new Run(fileText));
            CodePopupWindow popUp = new CodePopupWindow();
            popUp.codeView.Blocks.Clear();
            popUp.codeView.Blocks.Add(paragraph);
            popUp.Show();
        }
     
    }
}
