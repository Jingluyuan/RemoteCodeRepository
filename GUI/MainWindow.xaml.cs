///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project3HelpWPF                      //
// ver 2.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project3HelpWPF demo.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 3.0 : 1 May 2018 by Jinglu Yuan
 * - added check in dispatch to get check in return information
 * - added check out dispatch to get check out return information
 * - added browse dispatch to show the query answer
 * - added connect dispatch to show the connect test status
 * - added test to do automated test
 * ver 2.0 : 22 Apr 2018
 * - added tabbed display
 * - moved remote file view to RemoteNavControl
 * - migrated some methods from MainWindow to RemoteNavControl
 * - added local file view
 * - added NoSqlDb with very small demo as server starts up
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll
// - that can be fixed with a load failure event handler
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
using System.Threading;
using System.IO;
using MsgPassingCommunication;

namespace WpfApp1
{
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
      Console.Title = "Project4Demo GUI Console";
    }

    private Stack<string> pathStack_ = new Stack<string>();
    internal Translater translater;
    internal CsEndPoint endPoint_;
    private Thread rcvThrd = null;
    private Dictionary<string, Action<CsMessage>> dispatcher_ 
      = new Dictionary<string, Action<CsMessage>>();
    internal string saveFilesPath;
    internal string sendFilesPath;

        // ---< load check in processing into dispatcher dictionary >-------

        private void DispatcherCheckIn()
        {
            Action<CsMessage> CheckIn = (CsMessage rcvMsg) =>
            {
               
              
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("checkin_status"))
                    {
                        Action<string> docheckin = (string textmsg) =>
                        {                 
                            StatusBar.Text = textmsg;
                        };
                        Dispatcher.Invoke(docheckin, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("checkIn", CheckIn);
        }
        // ---< load check out processing into dispatcher dictionary >-------

        private void DispatcherCheckOut()
        {
            Action<CsMessage> CheckOut = (CsMessage rcvMsg) =>
            {

                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("checkout_status"))
                    {
                        Action<string> docheckout = (string textmsg) =>
                        {
                            StatusBar.Text = textmsg;
                        };
                        Dispatcher.Invoke(docheckout, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("checkOut", CheckOut);
        }
        // ---< load browse processing into dispatcher dictionary >-------

        private void DispatcherBrowse()
        {
            Action<CsMessage> PackageDrcp = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    BrowseTest.clearInfo();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("info"))
                    {
                        Action<string> dobrowse = (string textmsg) =>
                        {
                            BrowseTest.addInfo(textmsg);
                        };
                        Dispatcher.Invoke(dobrowse, new Object[] { enumer.Current.Value });
                    }
                    if (key.Contains("browse_status"))
                    {
                        Action<string> dobrowsestatus = (string textmsg) =>
                        {
                            StatusBar.Text = textmsg;
                        };
                        Dispatcher.Invoke(dobrowsestatus, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("Browse", PackageDrcp);
        }
        // ---< load connect test processing into dispatcher dictionary >-------

        private void DispatcherConnectTest()
        {
            Action<CsMessage> connectTest = (CsMessage rcvMsg) =>
            {

                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("Connect"))
                    {
                        Action<string> doconnectTest = (string textmsg) =>
                        {
                            StatusBar.Text = textmsg;
                        };
                        Dispatcher.Invoke(doconnectTest, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("Connect", connectTest);
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
            string msgId = msg.value("command");
            Console.Write("\n  client getting message \"{0}\"", msgId);
            if (dispatcher_.ContainsKey(msgId))
              dispatcher_[msgId].Invoke(msg);
          }
          catch(Exception ex)
          {
            Console.Write("\n  {0}", ex.Message);
            msg.show();
          }
        }
      };
      rcvThrd = new Thread(thrdProc);
      rcvThrd.IsBackground = true;
      rcvThrd.Start();
    }
    //----< add client processing for message with key >---------------

    private void addClientProc(string key, Action<CsMessage> clientProc)
    {
      dispatcher_[key] = clientProc;
    }
    ////----< load getDirs processing into dispatcher dictionary >-------

    private void DispatcherLoadGetDirs()
    {
      Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
      {
        Action clrDirs = () =>
        {
          //NavLocal.clearDirs();
          NavRemote.clearDirs();
        };
        Dispatcher.Invoke(clrDirs, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("dir"))
          {
            Action<string> doDir = (string dir) =>
            {
              NavRemote.addDir(dir);
            };
            Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
          }
        }
        Action insertUp = () =>
        {
          NavRemote.insertParent();
        };
        Dispatcher.Invoke(insertUp, new Object[] { });
      };
      addClientProc("getDirs", getDirs);
    }
    //----< load getFiles processing into dispatcher dictionary >------

    private void DispatcherLoadGetFiles()
    {
      Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
      {
        Action clrFiles = () =>
        {
          NavRemote.clearFiles();
        };
        Dispatcher.Invoke(clrFiles, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("file"))
          {
            Action<string> doFile = (string file) =>
            {
              NavRemote.addFile(file);
            };
            Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
          }
        }
      };
      addClientProc("getFiles", getFiles);
    }
    //----< load getFiles processing into dispatcher dictionary >------

    private void DispatcherLoadSendFile()
    {
      Action<CsMessage> sendFile = (CsMessage rcvMsg) =>
      {
        Console.Write("\n  processing incoming file");
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
          Action<string> act = (string fileNm) => { showFile(fileNm); };
          Dispatcher.Invoke(act, new object[] { fileName });
        }
      };
      addClientProc("sendFile", sendFile);
    }
    //----< load all dispatcher processing >---------------------------

    private void loadDispatcher()
    {
      DispatcherLoadGetDirs();
      DispatcherLoadGetFiles();
      DispatcherLoadSendFile();
      DispatcherCheckIn();
      DispatcherCheckOut();
      DispatcherConnectTest();
      DispatcherBrowse();
    }
    //----< start Comm, fill window display with dirs and files >------

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
            Thread.Sleep(5000);
      // start Comm
      endPoint_ = new CsEndPoint();
      endPoint_.machineAddress = "localhost";
      endPoint_.port = 8082;
      NavRemote.navEndPoint_ = endPoint_;
      ConnectTest.navEndPoint_ = endPoint_;
      BrowseTest.navEndPoint_ = endPoint_;

      translater = new Translater();
      translater.listen(endPoint_);

      // start processing messages
      processMessages();

      // load dispatcher
      loadDispatcher();

      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      pathStack_.Push("../NoSqlDb /Repository");

      NavRemote.PathTextBlock.Text = "NoSqlDb/Repository";
      NavRemote.pathStack_.Push("../NoSqlDb/Repository");

      NavLocal.PathTextBlock.Text = "LocalStorage";
      NavLocal.pathStack_.Push("");
      NavLocal.localStorageRoot_ = "../../../../LocalStorage";
      saveFilesPath = translater.setSaveFilePath("../../../../NoSqlDb /Repository");
      sendFilesPath = translater.setSendFilePath("../../../../NoSqlDb /Repository");

      NavLocal.refreshDisplay();
      NavRemote.refreshDisplay();

            test3();
            test4();
            Thread.Sleep(2000);
            test6(serverEndPoint, endPoint_);
    }
    //----< strip off name of first part of path >---------------------

    public string removeFirstDir(string path)
    {
      string modifiedPath = path;
      int pos = path.IndexOf("/");
      modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
      return modifiedPath;
    }
    //----< show file text >-------------------------------------------

    private void showFile(string fileName)
    {
      Paragraph paragraph = new Paragraph();
      string fileSpec = saveFilesPath + "\\" + fileName;
      string fileText = File.ReadAllText(fileSpec);
      paragraph.Inlines.Add(new Run(fileText));
      CodePopupWindow popUp = new CodePopupWindow();
      popUp.codeView.Blocks.Clear();
      popUp.codeView.Blocks.Add(paragraph);
      popUp.Show();
    }
        //----< first test not completed >---------------------------------

        void test3()
        {   
            NavLocal.checkintest();

            NavRemote.checkouttest();
            BrowseTest.browsetest();
        }

        void test4()
        {
            ConnectTest.connecttest();
        }
        void test6(CsEndPoint serverendpoint, CsEndPoint localendpoint)
        {
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverendpoint));
            msg.add("from", CsEndPoint.toString(localendpoint));
            msg.add("command", "sendFile");
            msg.add("path", "../NoSqlDb/Repository/Edit");
            msg.add("fileName", "Edit.3.h");
            this.translater.postMessage(msg);
        }
    }
}
