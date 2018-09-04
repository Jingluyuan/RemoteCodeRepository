#ifndef REPOSITORYCORE_H
#define REPOSITORYCORE_H
///////////////////////////////////////////////////////////////////////////
// RepositoryCore.h -  provides means to store, retrive or browse files  //
// ver 1.0                                                               //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018              //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, RepositoryCore:
*  - provide three functions to allow user to check-in files into the Repository Core, retrive a specific version file from the Core and browsing a specific file from the Core.
*  
*
*  Required Files:
*  ---------------
*    PayLoad.h, CheckIn.h
*    DbCore.h, CheckOut.h
*	 Browse.h, Query.h
*	 Version.h
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 3 Mar 2018
*  - first release
*
*/
#include <iostream>
#include <string>
#include <vector>
#include "../PayLoad/PayLoad.h"
#include "../DbCore/DbCore.h"
#include "../CheckIn/CheckIn.h"
#include "../CheckOut/CheckOut.h"
#include "../Browse/Browse.h"
#include "../Query/Query.h"
#include "../Version/Version.h"


using namespace NoSqlDb;
using namespace FileSystem;


class RepositoryCore
{
public:

	bool check_in_file(std::string name_space, std::string filename, std::string description, Children children, std::string path, std::vector<std::string> categories,std::string flag);
	bool check_out_file(Key key, std::string dst);
	std::vector<std::string> browsing_file(Conditions<PayLoad>& cons);
	void status_control(Key key, std::string status);
	DbCore<PayLoad>& db() { return db_; };
	DbCore<PayLoad> db() const { return db_; };
private:
	DbCore<PayLoad> db_;
};


#endif

