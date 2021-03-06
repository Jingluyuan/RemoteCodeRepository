///////////////////////////////////////////////////////////////////////
// Browse.cpp -  test browse function                                //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Browse.h"
#include "../Process/Process/Process.h"
#include "../Utilities/StringUtilities/StringUtilities.h"


std::vector<std::string> Browse::browse(DbCore<PayLoad>& db, Conditions<PayLoad>& cons)
{
	Query<PayLoad> q1(db);
	q1.select(cons).show();
	std::vector<std::string> browse_result ;
	for (auto key : q1.select(cons).keys())
	{
		std::string dependencies_name = "";
		std::string name_space = key.substr(0, key.find_first_of(':'));
		std::string temp_filename = (key.substr(key.find_last_of(':') + 1)).substr(0, (key.substr(key.find_last_of(':') + 1)).find_last_of('.'));
		std::string version_number = (key.substr(key.find_last_of(':') + 1)).substr((key.substr(key.find_last_of(':') + 1)).find_last_of('.'));
		std::string name_part = temp_filename.substr(0, temp_filename.find_last_of('.'));
		std::string type_part = temp_filename.substr(temp_filename.find_last_of('.'));
		std::string re_filename = name_part + type_part;
		for (unsigned int i = 0; i < db[key].children().size(); i++)
		{
			dependencies_name = dependencies_name+" "+db[key].children()[i];
		}
		browse_result.push_back( "categories:"+ name_space + "; " +"filename:"+ re_filename + "; " +"dependencies:"+ dependencies_name + "; " +"version number:"+ version_number );
	}
	return browse_result;
}
 


#ifdef TEST_BROWSE
bool test(DbCore<PayLoad>& db)
{
	Utilities::title("test browsing function");
	Utilities::putline();

	DbElement<PayLoad> demoelem;
	demoelem.name("test::test.txt.1");
	demoelem.payLoad().value("/A ../Repository/test/test.1.txt");
	db["test::test.txt.1"] = demoelem;
	demoelem.name("test::test.txt.2");
	demoelem.payLoad().value("/A ../Repository/test/test.2.txt");
	db["test::test.txt.2"] = demoelem;

	Conditions <PayLoad> cons;
	cons.name("test.txt");

	Browse bro;
	bro.browse(db, cons);
	return true;
}
int main()
{
	DbCore<PayLoad> db;
	test(db);
	return 0;
}
#endif // TEST_BROWSE



