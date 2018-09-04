///////////////////////////////////////////////////////////////////////
// CheckIn.cpp - test checkin function                               //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "CheckIn.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

using namespace NoSqlDb;
using namespace FileSystem;

void CheckIn::checkin(DbCore<PayLoad>&db, Key key, std::string name_space, DbElement<PayLoad>& elem)
{
	std::string src = elem.payLoad().value();
	std::string dir_temp = "../NoSqlDb/Repository/" + name_space + "/";
	std::string temp_filename = (key.substr(key.find_last_of(':') + 1)).substr(0, (key.substr(key.find_last_of(':') + 1)).find_last_of('.'));
	std::string version_number = (key.substr(key.find_last_of(':') + 1)).substr((key.substr(key.find_last_of(':') + 1)).find_last_of('.'));
	std::string name_part = temp_filename.substr(0, temp_filename.find_last_of('.'));
	std::string type_part = temp_filename.substr(temp_filename.find_last_of('.'));
	std::string core_name = name_part + version_number + type_part;
	std::string dst = dir_temp + core_name;

	if (!Directory::exists(dir_temp)) Directory::create(dir_temp);
	File::copy(src, dst);


}


#ifdef TEST_CHECKIN
bool test(DbCore<PayLoad>& db)
{
	Utilities::title("test check_in function");
	Utilities::putline();

	DbElement<PayLoad> demoelem;
	demoelem.name("checkin_test::checkin_test.txt.2");
	demoelem.payLoad().value("../Repository/test/checkin_test.txt");

	CheckIn cek;
	cek.checkin(db, "checkin::checkin_test.txt.2", "checkin_test", demoelem);

	return true;
}
int main()
{
	DbCore<PayLoad> db;
	test(db);

	return 0;
}

#endif // TEST_CHECKIN


