///////////////////////////////////////////////////////////////////////
// BrowseControl.xaml.cs - Control GUI for Browse                    //
// ver 1.0                                                           //
// Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based control GUI for Project4Demo.  It's 
 * responsibilities are to:
 * - Provide a button to do the Browse.
 * - Provide a textbox to input query information;
 * - Provide a listbox to show the query answer.
 * Required Files:
 * ---------------
 * BrowseControl.xaml, BrowseControl.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 1 May 2018
 * - first release
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
using MsgPassingCommunication;

namespace WpfApp1
{
    
    public partial class BrowseControl : UserControl
    {
        internal CsEndPoint navEndPoint_;
        public BrowseControl()
        {
            InitializeComponent();
        }

        internal void addInfo(string info)
        {
            InfoList.Items.Add(info);
        }

        internal void clearInfo()
        {
            InfoList.Items.Clear();
        }
        internal void browsetest()
        {
            Browse(this, null);
        }
        //----< browse >---------------------------

        private void Browse(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(navEndPoint_));
            msg.add("command", "Browse");
            msg.add("queryinfo", text_info.Text);
            win.translater.postMessage(msg);
        }
    }
}
