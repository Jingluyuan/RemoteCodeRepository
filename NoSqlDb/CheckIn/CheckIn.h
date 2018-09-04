#pragma once
///////////////////////////////////////////////////////////////////////
// CheckIn.h - store a file into the repository db                   //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, CheckIn:
*  -checkin function will be able to copy the file from its initial space to repository core,
*	also will create a directory according to the namespace of the file in the repository core to store the file,
*	and will update <key,dbelem> in the db core.
*
*  Required Files:
*  ---------------
*  FileSystem.h   DbCore.h
*  PayLoad.h
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 3 Mar 2018
*  - first release
*
*/

#include <iostream>
#include "../DbCore/DbCore.h"
#include "../../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../PayLoad/PayLoad.h"

using namespace NoSqlDb;
using namespace FileSystem;

class CheckIn
{
public:
	void checkin(DbCore<PayLoad>&db, Key key, std::string name_space, DbElement<PayLoad>& elem);
};

