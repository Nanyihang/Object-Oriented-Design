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
*  This package implements the SelectionWindow class and create a GUI surface
*
*  Required Files:
*  ---------------
*  SelectionWindow.xaml SelectionWindow.xaml.cs
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
using System.Windows.Shapes;
using System.IO;
using MsgPassingCommunication;
namespace Navigator
{
    /// <summary>
    /// Interaction logic for SelectionWindow.xaml
    /// </summary>
  
    /*<--------------------SelectionWindow class defination--------------------->*/
    public partial class SelectionWindow : Window
    {
        List<string> selectedFiles { get; set; } = new List<string>();


        MainWindow mwin { get; set; } = null;
        /*<--------------------Getting back the MainWindow handle--------------------->*/
        public void connectMainwindow(MainWindow min)
        {
            mwin = min;
        }
        /*<--------------------Method for MainWindow use just for passing value to SelectionWindow--------------------->*/
        public void PassSlectedFiles(List<string> ele)
        {
            selectedFiles = ele;
        }
        /*<--------------------SelectionWindow class construction--------------------->*/
        public SelectionWindow()
        {
            InitializeComponent();
        }
        /*<--------------------Method for getting the SelectionFiles value--------------------->*/
        public void loadfiles()
        {
            SelectedFiles.Items.Clear();
            foreach (string ele in selectedFiles)
                SelectedFiles.Items.Add(ele.ToString());
        }
        /*<--------------------Method to execute when SelectionWindo loaded--------------------->*/
        public void SelectionLoaded(object sender, RoutedEventArgs e)
        {
            loadfiles();
        }

        /*<------------------------MouseDoubleClick for deleting files-------------->*/
        private void Deletefiles(object sender, MouseEventArgs e)
        {
            string delitem = SelectedFiles.SelectedItem.ToString();
            selectedFiles.Remove(delitem);
            loadfiles();
        }
        /*<------------------------Method to execute when SelectionWindow unloaded-------------->*/
        private void SelectionUnloaded(object sender, RoutedEventArgs e)
        {
            mwin.setexist(false);
        }
    }
}
