#pragma once
///////////////////////////////////////////////////////////////////////
// ServerPrototype.h - Console App that processes incoming messages  //
// ver 1.2                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  Package contains one class, Server, that contains a Message-Passing Communication
*  facility. It processes each message by invoking an installed callable object
*  defined by the message's command key.
*  - This is implemented with a message dispatcher (unodered_map<Msg.Id,ServerProc>
*    where ServerProcs are defined for each type of processing required by the server.
*
*  Message handling runs on a child thread, so the Server main thread is free to do
*  any necessary background processing (none, so far).
*
*  Required Files:
* -----------------
*  ServerPrototype.h, ServerPrototype.cpp
*  Comm.h, Comm.cpp, IComm.h
*  Message.h, Message.cpp (static library)
*  Process.h, Process.cpp (static library)
*  FileSystem.h, FileSystem.cpp
*  Utilities.h
*
*  Maintenance History:
* ----------------------
*  ver 2.0 : 30 Apr 2018 by Jinglu Yuan
*  - added checkin helper, checkout helper, browse helper to help do the check in, check out, browse work
*  - added initial information to the repository core
*  ver 1.2 : 22 Apr 2018
*  - added NoSqlDb to server members
*  - added simple demo of db in Server startup
*  ver 1.1 : 09 Apr 2018
*  - added ServerProcs for
*    - sending files for popup display
*    - executing remote analysis
*  ver 1.0 : 03/27/2018
*  - first release
*/
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../NoSqlDb/RepositoryCore/RepositoryCore.h"
#include <windows.h>
#include <tchar.h>

namespace Repository
{
  using File = std::string;
  using Files = std::vector<File>;
  using Dir = std::string;
  using Dirs = std::vector<Dir>;
  using SearchPath = std::string;
  using Key = std::string;
  using Msg = MsgPassingCommunication::Message;
  using ServerProc = std::function<Msg(Msg)>;
  using MsgDispatcher = std::unordered_map<Key,ServerProc>;
  
  const SearchPath storageRoot = "../NoSqlDb";  // root for all server file storage
  const MsgPassingCommunication::EndPoint serverEndPoint("localhost", 8080);  // listening endpoint
  const MsgPassingCommunication::EndPoint testClientEndPoint("localhost", 8083);

  class Server
  {
  public:
    Server(MsgPassingCommunication::EndPoint ep, const std::string& name);
    void start();
    void stop();
    void addMsgProc(Key key, ServerProc proc);
    bool hasMessageKey(const Key& key);
    void processMessages();
    void postMessage(MsgPassingCommunication::Message msg);
    MsgPassingCommunication::Message getMessage();
    static Dirs getDirs(const SearchPath& path = storageRoot);
    static Files getFiles(const SearchPath& path = storageRoot);
    MsgPassingCommunication::Context* getContext();
    std::string setSendFilePath(const std::string& relPath);
    std::string setSaveFilePath(const std::string& relPath);
    std::string getSendFilePath();
    std::string getSaveFilePath();
    void initializeDb();
	bool checkIn_helper(Msg _msg);
	bool checkOut_helper(Msg _msg);
	std::vector<std::string> Browse_helper(Msg _msg);
  private:
    MsgPassingCommunication::Comm comm_;
    MsgDispatcher dispatcher_;
    std::thread msgProcThrd_;
    RepositoryCore reCore;
  };
  //----< check in helper >---------------------------------------

  bool Server::checkIn_helper(Msg _msg)
  {
	  std::string name_space = _msg.value("name_space");
	  std::string filename = _msg.value("filename");
	  std::string description = _msg.value("descripction");
	  std::string path = _msg.value("path");
	  std::vector<std::string> ch;
	  std::vector<std::string> ca;
	  std::string ch_res;
	  std::string ca_res;

	  std::string ch_temp = _msg.value("children");
	  std::stringstream ch_input(ch_temp);
	  while (ch_input >> ch_res)
	  {
		  ch.push_back(ch_res);
	  }
	  
	  std::string ca_temp = _msg.value("categories");
	  std::stringstream ca_input(ca_temp);
	  while (ca_input >> ca_res)
	  {
		  ca.push_back(ca_res);
	  }
	  if (reCore.check_in_file(name_space, filename, description, ch, path, ca, "closed"))
		  return true;
	  return false;
	  
  }
  //----< check out helper >---------------------------------------------

  bool Server::checkOut_helper(Msg _msg)
  {
	  std::string filename = _msg.value("filename");
	  std::string path = _msg.value("path");
	  std::cout << "check out filename:" << filename << " " << "check out path:" << path << std::endl;
	  if (reCore.check_out_file(filename, path)) return true;
	  return false;
  }
  //----< browse helper >-------------------------------------------------

  std::vector<std::string> Server::Browse_helper(Msg _msg)
  {
	  std::string query_info = _msg.value("queryinfo");
	  std::cout << "the query information is :" << query_info << ", the query answer is:" << std::endl;
	  Conditions<PayLoad> cons;
	  cons.name(query_info);
	  return reCore.browsing_file(cons);
	  
  }

  //----< return reference to MsgPassingCommunication context >--------

  inline MsgPassingCommunication::Context* Server::getContext()
  {
    return comm_.getContext();
  }
  //----< initialize server endpoint and give server a name >----------

  inline Server::Server(MsgPassingCommunication::EndPoint ep, const std::string& name)
    : comm_(ep, name) {
    initializeDb();
  }

  inline std::string Server::setSendFilePath(const std::string& relPath)
  {
    comm_.setSendFilePath(relPath);
  }
  inline std::string Server::setSaveFilePath(const std::string& relPath)
  {
    comm_.setSaveFilePath(relPath);
  }
  inline std::string Server::getSendFilePath()
  {
    comm_.getSendFilePath();
  }
  inline std::string Server::getSaveFilePath()
  {
    comm_.getSaveFilePath();
  }
  //----< start server's instance of Comm >----------------------------

  inline void Server::start()
  {
    comm_.start();
  }
  //----< stop Comm instance >-----------------------------------------

  inline void Server::stop()
  {
    if(msgProcThrd_.joinable())
      msgProcThrd_.join();
    comm_.stop();
  }
  //----< pass message to Comm for sending >---------------------------

  inline void Server::postMessage(MsgPassingCommunication::Message msg)
  {
    comm_.postMessage(msg);
  }
  //----< get message from Comm >--------------------------------------

  inline MsgPassingCommunication::Message Server::getMessage()
  {
    Msg msg = comm_.getMessage();
    return msg;
  }
  //----< add ServerProc callable object to server's dispatcher >------

  inline void Server::addMsgProc(Key key, ServerProc proc)
  {
    dispatcher_[key] = proc;
  }
  //----< does server have specified key? >----------------------------

  inline bool Server::hasMessageKey(const Key& key)
  {
    for (auto item : dispatcher_)
    {
      if (item.first == key)
        return true;
    }
    return false;
  }
  //----< start processing messages on child thread >------------------

  inline void Server::processMessages()
  {
    auto proc = [&]()
    {
      if (dispatcher_.size() == 0)
      {
        std::cout << "\n  no server procs to call";
        return;
      }
      while (true)
      {
        Msg msg = getMessage();
        std::cout << "\n  received message: " << msg.command() << " from " << msg.from().toString()<<std::endl;
        if (msg.containsKey("verbose"))
        {
          std::cout << "\n";
          msg.show();
        }
        if (msg.command() == "serverQuit")
          break;
        Msg reply;
        reply.to(msg.from());
        reply.from(msg.to());
        if (hasMessageKey(msg.command()))
        {
          reply = dispatcher_[msg.command()](msg);
        }
        else
        {
          reply.command("error - unknown command");
        }

		if (msg.command() == "checkIn")
		{
			std::cout << "This is check in !!" << std::endl;
			std::cout << "=============================================" << std::endl;
			if (checkIn_helper(msg))
			{
				reply.attribute("checkin_status", "Check In Success");
				
			}
			else
			{
				reply.attribute("checkin_status", "Check In Fail");
			}
			NoSqlDb::showDb(reCore.db());
		}
		if (msg.command() == "checkOut")
		{
			std::cout << "This is check out !!" << std::endl;
			std::cout << "=============================================" << std::endl;
			if (checkOut_helper(msg))
			{
				reply.attribute("checkout_status", "Check Out Success");

			}
			else
			{
				reply.attribute("checkout_status", "Check Out Fail");
			}
		}
		if (msg.command() == "Browse")
		{
			std::cout << "This is browse !!" << std::endl;
			std::cout << "=============================================" << std::endl;
			size_t count = 0;
			if (Browse_helper(msg).size() <= 0)
			{
				reply.attribute("browse_status", "Browse Fail");
				reply.attribute("info" , "No such file in the repository core");
			}
			else
			{
				
				for (auto item : Browse_helper(msg))
				{
					std::string countStr = Utilities::Converter<size_t>::toString(++count);
					reply.attribute("info" + countStr, item);
				}
				reply.attribute("browse_status", "Browse Success");
			}
		}

        if (msg.to().port != msg.from().port)  // avoid infinite message loop
        {
          postMessage(reply);
          msg.show();
          reply.show();
        }
        else
          std::cout << "\n  server attempting to post to self";
      }
      std::cout << "\n  server message processing thread is shutting down";
    };
    std::thread t(proc);
    //SetThreadPriority(t.native_handle(), THREAD_PRIORITY_HIGHEST);
    std::cout << "\n  starting server thread to process messages"<<std::endl;
    msgProcThrd_ = std::move(t);
  }
  inline void Server::initializeDb()
  {
    NoSqlDb::DbElement<NoSqlDb::PayLoad> elem;
   
	Keys cat;
	Children children1;
	children1.push_back("Query::Query.h.1");
	children1.push_back("DbCore::DbCore.h.1");
	reCore.check_in_file("Edit", "Edit.h", "first version of Edit.h", children1, "../NoSqlDb/Edit/Edit.h", cat, "closed");
	reCore.check_in_file("Edit", "Edit.h", "second version of Edit.h", children1, "../NoSqlDb/Edit/Edit.h", cat, "closed");
	reCore.check_in_file("Edit", "Edit.h", "third version of Edit.h", children1, "../NoSqlDb/Edit/Edit.h", cat, "open");

	reCore.check_in_file("PayLoad", "IPayLoad.h", "payload helper", children1, "../NoSqlDb/PayLoad/IPayLoad.h", cat, "closed");
	Children children2;
	children2.push_back("Query::Query.h.1");
	children2.push_back("DbCore::DbCore.h.1");
	children2.push_back("PayLoad::IPayLoad.h.1");
	reCore.check_in_file("PayLoad", "PayLoad.h", "payload headfile", children2, "../NoSqlDb/PayLoad/PayLoad.h", cat, "closed");
	Children children3;
	children3.push_back("DbCore::DbCore.h.1");
	reCore.check_in_file("Query", "Query.h", "query headfile", children3, "../NoSqlDb/Query/Query.h", cat, "closed");
    NoSqlDb::showDb(reCore.db());
    std::cout << "\n";
  }
}