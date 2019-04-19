/////////////////////////////////////////////////////////////////////
// SelectionWindow.xaml.cs - Prototype for OOD Project #4          //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class SelectionWindow that provides a 
 * ListBox for displaying files selected in the main window.
 * 
 * Two way communication is provided:
 * - MainWindow creates SelectionWindow and stores a reference as
 *   member data.
 * - SelectionWindow provides a SetMainwindow(MainWindow mwin) function
 *   which MainWindow uses to give SelectionWindow access to its public
 *   members.
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 16 Nov 2018
 * - first release
 * 
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
using System.Windows.Shapes;

namespace PrototypeGui_OOD_Pr4
{
  ///////////////////////////////////////////////////////////////////
  // SelectionWindow class
  // - Displays files selected in the MainWindow.
  // - It should provide the facility to create a TestRequest
  //   from the selected files, and send to a Client component
  // - The Client component will contain a Comm instance and
  //   will send the test request and its library files to the
  //   TestHarness.

  public partial class SelectionWindow : Window
  {
    MainWindow mWin = null;

    public SelectionWindow()
    {
      InitializeComponent();
    }
    //----< called by MainWindow to give this window her ref >-------

    public void setMainWindow(MainWindow mwin)
    {
      mWin = mwin;
    }
    //----< called by MainWindow to add a selected file >------------

    public void Add(string file)
    {
      Selections.Items.Add(file);
    }
    //----< unselect files and set mWin.swin to null >---------------

    private void Window_Unloaded(object sender, RoutedEventArgs e)
    {
      mWin.unselectFiles();
      mWin.swin = null;
    }
    //----< may need this when SelectionWindow starts up >-----------

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
      //this.Topmost = true;
    }

  }
}
