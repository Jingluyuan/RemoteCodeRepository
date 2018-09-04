///////////////////////////////////////////////////////////////////////
// ConnectTestControl.xaml.cs - Control GUI for Connect Test         //
// ver 1.0                                                           //
// Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based control GUI for Project4Demo.  It's 
 * responsibilities are to:
 * - Provide a button to do the connection test.   
 * Required Files:
 * ---------------
 * ConnectTestControl.xaml, ConnectTestControl.xaml.cs
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
using System.Windows.Shapes;
using MsgPassingCommunication;

namespace WpfApp1
{
    
    public partial class ConnectTestControl : UserControl
    {
        internal CsEndPoint navEndPoint_;
        public ConnectTestControl()
        {
            InitializeComponent();
        }
        internal void connecttest()
        {
            Connect(this, null);
        }
        private void Connect(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8080;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(navEndPoint_));
            msg.add("command", "Connect");
            win.translater.postMessage(msg);
        }

    }
}
