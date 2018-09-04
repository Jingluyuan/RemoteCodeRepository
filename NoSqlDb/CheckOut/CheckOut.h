#ifndef CHECKOUT_H
#define CHECKOUT_H
///////////////////////////////////////////////////////////////////////
// CheckOut.h - retrive a file from the repository db                //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, CheckOut:
*  -checkout function will ba able to retrive a specific file to the given location and also retrive all of the files which are its dependencies.
*
*  Required Files:
*  ---------------
*  DbCore.h
*  PayLoad.h   FileSystem.h
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 3 Mar 2018
*  - first release
*
*/

#include <iostream>
#include <string>
#include <queue>
#include <set>
#include "../DbCore/DbCore.h"
#include "../../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../PayLoad/PayLoad.h"

using namespace NoSqlDb;
using namespace FileSystem;

class CheckOut
{
public:
	void checkout(DbCore<PayLoad>&db, Key key, std::string dst);

};


#endif 

#pragma once
