///////////////////////////////////////////////////////////////////////
// Version.cpp -  test functions in version class                    //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Version.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

bool Version::initial_version_judge(DbCore<PayLoad>& db_, std::string name_space, std::string file_name)
{
	if (db_.contains(name_space + "::" + file_name + ".1"))
	{
		return false;
	}
	return true;
}


Key Version::find_latest_version(DbCore<PayLoad>& db_, std::string name_space, std::string file_name)
{
	int i = 1;
	while (db_.contains(name_space + "::" + file_name + "." + std::to_string(i)))
	{
		i++;
	}
	return name_space + "::" + file_name + "." + std::to_string(i - 1);
}


Key Version::version_number_update(DbCore<PayLoad>& db_, std::string name_space, std::string file_name)
{
	int i = 1;
	while (db_.contains(name_space + "::" + file_name + "." + std::to_string(i)))
	{
		i++;
	}
	return name_space + "::" + file_name + "." + std::to_string(i);
}

#ifdef TEST_VERSION
bool test(DbCore<PayLoad>& db)
{
	Utilities::title("test version control functions");
	DbElement<PayLoad> demoelem = db["test::test.txt.1"];
	demoelem.name("test::test.txt.2");
	db["test::test.txt.2"];
	demoelem.name("test::test.txt.3");
	db["test::test.txt.3"];
	demoelem.name("test::test.txt.4");
	db["test::test.txt.4"];
	Utilities::putline();
	Version vsi;
	if (vsi.initial_version_judge(db, "test", "new_test.txt"))
	{
		std::cout << "file's version is the initial one!" << std::endl;
	}
	else
	{
		std::cout << "initial version judge fail!" << std::endl;
		return false;
	}

	std::cout << vsi.find_latest_version(db, "test", "test.txt") << std::endl;
	std::cout << vsi.version_number_update(db, "test", "test.txt") << std::endl;
	return true;
}

int main()
{
	DbCore<PayLoad> db;
	test(db);
	return 0;
}


#endif // VERSION

