#ifndef BROWSE_H
#define BROWSE_H
///////////////////////////////////////////////////////////////////////
// Browse.h -  locate files and view their contents                  //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, Browse:
*  -browsing function is provided to show all of the files match the select condition and open a notepad to show the the content in the file.
*
*  Required Files:
*  ---------------
*  Process.h   DbCore.h
*  PayLoad.h   Query.h
*
*  Maintenance History:
*  --------------------
*  Ver 1.0 : 3 Mar 2018
*  - first release
*
*/

#include <iostream>

#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../Query/Query.h"
 
using namespace NoSqlDb;


class Browse
{
public:
	std::vector < std::string > browse(DbCore<PayLoad>& db, Conditions<PayLoad>& cons);

};

#endif