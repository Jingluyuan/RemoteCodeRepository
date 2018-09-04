#ifndef VERSION_H
#define VERSION_H
///////////////////////////////////////////////////////////////////////
// Version.h -  manage version numbering for all files               //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, Version:
*  -initial_version_judge() function is designed to judge wheteher the checkin file is the initial one;
*  -find_latest_version() function is designed to return the latest version of a specific file;
*  -version_number_update() function is designed to update the version number of the check-in file; 
*
*  Required Files:
*  ---------------
*	DbCore.h
*	Definitions.h
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 3 Mar 2018
*  - first release
*
*/
#include <string>
#include <iostream>
#include "../DbCore/DbCore.h"
#include "../DbCore/Definitions.h"
#include "../PayLoad/PayLoad.h"
using namespace NoSqlDb;


class Version
{
public:
	bool initial_version_judge(DbCore<PayLoad>& db_, std::string name_space, std::string file_name);
	Key find_latest_version(DbCore<PayLoad>& db_,std::string name_space,std::string file_name);
	Key version_number_update(DbCore<PayLoad>& db_, std::string name_space, std::string file_name);


};



#endif

