/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Prototype for OOD Project #4               //
// ver 1.2                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
// Author        :	Yihang Nan, Syracuse University                //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through local directory to find files for testing
 *   - Shows local directories and files
 *   - Can navigate by double clicking directories
 *   - Can select files by selecting.  That displays selections in a popup window.
 * - Request Tests: left for students
 * - Show Results:  left for students
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.2 : 28 Mar 2019
 * - fixed bug in DirsMouse_DoubleClick by using try catch block to
 *   handle exception that occurs if user clicks on unpopulated listbox item.
 * ver 1.1 : 16 Nov 2018
 * - fixed bug in Files_SelectionChanged by checking e.AddedItems.Count
 *   and returning if 0.
 * ver 1.0 : 15 Nov 2018
 * - first release
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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

namespace PrototypeGui_OOD_Pr4
{
  ///////////////////////////////////////////////////////////////////
  // MainWindow class
  // - Provides navigation and selection to find libraries to test.
  // - Creates a popup window to handle selections.
  
  public partial class MainWindow : Window
  {
        public string path { get; set; }
        private List<string> patterns { get; set; } = new List<string>();
        List<string> selectedFiles { get; set; } = new List<string>();
        public SelectionWindow swin { get; set; } = null;
        List<string> cmdline { get; set; } = new List<string>();
        bool unselecting = false;

        public MainWindow()
        {
            InitializeComponent();
            CmdLineToList();
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
        /*<------------------------Extracting patterns from commandline and apply it-------------->*/
        private void setPattern()
        {
            Pattern.Text = "";
            ITranslator tran = TranslatorFactory.createTranslator();
            patterns = tran.getPattern(cmdline.Count, cmdline);
            foreach (string ele in patterns)
            {
                Pattern.Text += " ";
                Pattern.Text += ele;
            }
        }
        //---------<test function>-----------------------
        private void Double_Mouseclick(object sender, RoutedEventArgs e)
        {
            ITranslator tran = TranslatorFactory.createTranslator();
            foreach (string selectedFile in selectedFiles)
            {
                tran.putString(selectedFile);
            }
            tran.transfer_file_code();
        }

        //----< open window showing contents of project directory >------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
           

            path = Directory.GetCurrentDirectory();
            path = getAncestorPath(4, path);  
            LoadNavTab(path);
            setPattern();
        }
    //----< find parent paths >--------------------------------------

    string getAncestorPath(int n, string path)
    {
      for(int i=0; i<n; ++i)
        path = Directory.GetParent(path).FullName;
      return path;
    }
    //----< file Find Libs tab with subdirectories and files >-------

    void LoadNavTab(string path)
    {
            Files.Items.Clear();
            Dirs.Items.Clear();
            CurrPath.Text = path;
     string[] dirs = Directory.GetDirectories(path);
      Dirs.Items.Add("..");
      foreach (string dir in dirs)
      {
        DirectoryInfo di = new DirectoryInfo(dir);
        string name = System.IO.Path.GetDirectoryName(dir);
        Dirs.Items.Add(di.Name);
      }
      Files.Items.Clear();
      string[] files = Directory.GetFiles(path); //get all file need to match
            //-------------------<regex file>-----------------------
      foreach (string file in files)
      {
                //string name = System.IO.Path.GetFileName(file);
                // Files.Items.Add(name);
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
    //----< handle selections in files listbox >---------------------

    private void Files_SelectionChanged(
      object sender, SelectionChangedEventArgs e
    )
    {
      if(unselecting)
      {
        unselecting = false;
        return;
      }
      if (swin == null)
      {
        swin = new SelectionWindow();
        swin.setMainWindow(this);
      }
      swin.Show();

      if (e.AddedItems.Count == 0)
        return;
      string selStr = e.AddedItems[0].ToString();
      selStr = System.IO.Path.Combine(path, selStr);
      if (!selectedFiles.Contains(selStr))
      {
        selectedFiles.Add(selStr);
        swin.Add(selStr);
      }
    }
    //----< unselect files called by unloading SelectionWindow >-----

    public void unselectFiles()
    {
      unselecting = true;  // needed to avoid using selection logic
      //selectedFiles.Clear();
      Files.UnselectAll();
    }
    //----< move into double-clicked directory, display contents >---

    private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      try
      {
        string selectedDir = Dirs.SelectedItem.ToString();
        if (selectedDir == "..")
          path = getAncestorPath(1, path);
        else
          path = System.IO.Path.Combine(path, selectedDir);
        LoadNavTab(path);
      }
      catch
      {
        // just return
      }
    }
    
        //----< shut down the SelectionWindow if open >------------------

    private void Window_Unloaded(object sender, RoutedEventArgs e)
    {
      swin.Close();
    }
  }
}
