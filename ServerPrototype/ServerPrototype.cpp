/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.2                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <chrono>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

//----< return name of every file on path >----------------------------

Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}
//----< return name of every subdirectory on path >--------------------

Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

namespace MsgPassingCommunication
{
  // These paths, global to MsgPassingCommunication, are needed by 
  // several of the ServerProcs, below.
  // - should make them const and make copies for ServerProc usage

  std::string sendFilePath;
  std::string saveFilePath;

  //----< show message contents >--------------------------------------

  template<typename T>
  void show(const T& t, const std::string& msg)
  {
    std::cout << "\n  " << msg.c_str();
    for (auto item : t)
    {
      std::cout << "\n    " << item.c_str();
    }
  }
  //----< test ServerProc simply echos message back to sender >--------

  std::function<Msg(Msg)> echo = [](Msg msg) {
    Msg reply = msg;
    reply.to(msg.from());
    reply.from(msg.to());
    return reply;
  };
  //----< getFiles ServerProc returns list of files on path >----------

  std::function<Msg(Msg)> getFiles = [](Msg msg) {
    Msg reply;
    reply.to(msg.from());
    reply.from(msg.to());
    reply.command("getFiles");
    std::string path = msg.value("path");
    if (path != "")
    {
      std::string searchPath = storageRoot;
      if (path != ".")
        searchPath = searchPath + "\\" + path;
      Files files = Server::getFiles(searchPath);
      size_t count = 0;
      for (auto item : files)
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("file" + countStr, item);
      }
    }
    else
    {
      std::cout << "\n  getFiles message did not define a path attribute";
    }
    return reply;
  };
  //----< getDirs ServerProc returns list of directories on path >-----

  std::function<Msg(Msg)> getDirs = [](Msg msg) {
    Msg reply;
    reply.to(msg.from());
    reply.from(msg.to());
    reply.command("getDirs");
    std::string path = msg.value("path");
    if (path != "")
    {
      std::string searchPath = storageRoot;
      if (path != ".")
        searchPath = searchPath + "\\" + path;
      Files dirs = Server::getDirs(searchPath);
      size_t count = 0;
      for (auto item : dirs)
      {
        if (item != ".." && item != ".")
        {
          std::string countStr = Utilities::Converter<size_t>::toString(++count);
          reply.attribute("dir" + countStr, item);
        }
      }
    }
    else
    {
      std::cout << "\n  getDirs message did not define a path attribute";
    }
    return reply;
  };

  //----< sendFile ServerProc sends file to requester >----------------
  /*
  *  - Comm sends bodies of messages with sendingFile attribute >------
  */
  std::function<Msg(Msg)> sendFile = [](Msg msg) {
    Msg reply;
    reply.to(msg.from());
    reply.from(msg.to());
    reply.command("sendFile");
    reply.attribute("sendingFile", msg.value("fileName"));
    reply.attribute("fileName", msg.value("fileName"));
    reply.attribute("verbose", "blah blah");
    std::string path = msg.value("path");
    if (path != "")
    {
      std::string searchPath = storageRoot;
      if (path != "." && path != searchPath)
        searchPath = searchPath + "\\" + path;
      if (!FileSystem::Directory::exists(searchPath))
      {
        std::cout << "\n  file source path does not exist";
        return reply;
      }
      std::string filePath = searchPath + "/" + msg.value("fileName");
      std::string fullSrcPath = FileSystem::Path::getFullFileSpec(filePath);
      std::string fullDstPath = sendFilePath;
      if (!FileSystem::Directory::exists(fullDstPath))
      {
        std::cout << "\n  file destination path does not exist";
        return reply;
      }
      fullDstPath += "/" + msg.value("fileName");
      FileSystem::File::copy(fullSrcPath, fullDstPath);
    }
    else
    {
      std::cout << "\n  getDirs message did not define a path attribute";
    }
    return reply;
  };
  //----< check in >------------------------------

  std::function<Msg(Msg)> checkin = [](Msg msg)
  {
	  Msg reply;
	  reply.to(msg.from());
	  reply.from(msg.to());
	  reply.command("checkIn");
	  return reply;
  };
  //----< check out >------------------------------
  std::function<Msg(Msg)> checkout = [](Msg msg)
  {
	  Msg reply;
	  reply.to(msg.from());
	  reply.from(msg.to());
	  reply.command("checkOut");
	  return reply;
  };
  //----< browse >------------------------------
  std::function<Msg(Msg)> browse = [](Msg msg)
  {
	  Msg reply;
	  reply.to(msg.from());
	  reply.from(msg.to());
	  reply.command("Browse");
	  return reply;
  };
  //----< connect >-------------------------------
  std::function<Msg(Msg)> connectTest = [](Msg msg)
  {
	  Msg reply;
	  reply.to(msg.from());
	  reply.from(msg.to());
	  reply.command("Connect");
	  reply.attribute("Connect", "Hello Client! Connect test recived!");
	  std::string str = msg.value("path");
	  std::cout << std::endl;
	  std::cout << str << std::endl;
	  return reply;
  };

}
void server_checkin_test(Server &server)
{
	std::cout << std::endl;
	std::cout << "This is requirement 2 checkin function test" << std::endl;
	std::cout << "==========================================" << std::endl;
	Msg msg(serverEndPoint, testClientEndPoint);
	msg.command("checkIn");
	msg.attribute("name_space","DbCore");
	msg.attribute("filename","DbCore.h");
	msg.attribute("descripction","DbCore headfile");
	msg.attribute("path","../NoSqlDb/DbCore/Dbcore.h");
	msg.attribute("children", "Query::Query.h.1");
	msg.attribute("categories", "head file");
	msg.attribute("flag","closed");
	server.postMessage(msg);
	

	msg.command("checkIn");
	msg.attribute("name_space", "Version");
	msg.attribute("filename", "Version.h");
	msg.attribute("descripction", "Version headfile");
	msg.attribute("path", "../NoSqlDb/Version/Version.h");
	msg.attribute("children", "DbCore::DbCore.h.1 DbCore::Definitions.h.1 PayLoad::PayLoad.h.1");
	msg.attribute("categories", "head file");
	msg.attribute("flag", "closed");
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
}

void server_checkout_test(Server &server)
{
	
	std::cout << "This is requirement 2 checkout function and browsing function test" << std::endl;
	std::cout << "==========================================" << std::endl;
	Msg msg(serverEndPoint, testClientEndPoint);
	msg.command("checkOut");
	msg.attribute("filename","PayLoad::PayLoad.h.1");
	msg.attribute("path", "../CheckOutFile");
	server.postMessage(msg);

	msg.command("Browse");
	msg.attribute("queryinfo", "PayLoad");
	server.postMessage(msg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	
}


using namespace MsgPassingCommunication;

int main()
{
  SetConsoleTitleA("Project4Sample Server Console");

  std::cout << "\n  Testing Server Prototype";
  std::cout << "\n ==========================";
  std::cout << "\n";

  //StaticLogger<1>::attach(&std::cout);
  //StaticLogger<1>::start();

  sendFilePath = FileSystem::Directory::createOnPath("./SendFiles");
  saveFilePath = FileSystem::Directory::createOnPath("./SaveFiles");

  Server server(serverEndPoint, "ServerPrototype");

  // may decide to remove Context
  MsgPassingCommunication::Context* pCtx = server.getContext();
  pCtx->saveFilePath = saveFilePath;
  pCtx->sendFilePath = sendFilePath;

  server.start();

  std::cout << "\n  testing getFiles and getDirs methods";
  std::cout << "\n --------------------------------------";
  Files files = server.getFiles();
  show(files, "Files:");
  Dirs dirs = server.getDirs();
  show(dirs, "Dirs:");
  std::cout << "\n";

  std::cout << "\n  testing message processing";
  std::cout << "\n ----------------------------";
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("sendFile", sendFile);
  server.addMsgProc("serverQuit", echo);
  server.addMsgProc("checkIn", checkin);
  server.addMsgProc("checkOut", checkout);
  server.addMsgProc("Browse", browse);
  server.addMsgProc("Connect", connectTest);
  server.processMessages();
  
  server_checkin_test(server);
  server_checkout_test(server);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "This is Requirement 3, 4 and 5，6" << std::endl;
  std::cout << "=============================================" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  
  Msg msg(serverEndPoint, serverEndPoint);  // send to self
  msg.name("msgToSelf");

  std::cout << "\n  press enter to exit\n";
  std::cin.get();
  std::cout << "\n";

  msg.command("serverQuit");
  server.postMessage(msg);
  server.stop();
  return 0;
}

