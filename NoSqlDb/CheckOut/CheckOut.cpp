///////////////////////////////////////////////////////////////////////
// CheckOut.cpp - test check_out function                            //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "CheckOut.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

void CheckOut::checkout(DbCore<PayLoad>&db, Key key, std::string dst)
{
	std::queue<Key> visitqueue;
	std::set<Key> visited;
	visitqueue.push(key);

	while (visitqueue.size()>0)
	{
		Key current_key = visitqueue.front();
		visitqueue.pop();
		if (visited.find(current_key) == visited.end())
		{
			visited.insert(current_key);

			std::string name_space = current_key.substr(0, current_key.find_first_of(':'));
			std::string retrive_file_name = (current_key.substr(current_key.find_last_of(':') + 1)).substr(0, (current_key.substr(current_key.find_last_of(':') + 1)).find_last_of('.'));

			std::string temp_filename = (current_key.substr(current_key.find_last_of(':') + 1)).substr(0, (current_key.substr(current_key.find_last_of(':') + 1)).find_last_of('.'));
			std::string version_number = (current_key.substr(current_key.find_last_of(':') + 1)).substr((current_key.substr(current_key.find_last_of(':') + 1)).find_last_of('.'));
			std::string name_part = temp_filename.substr(0, temp_filename.find_last_of('.'));
			std::string type_part = temp_filename.substr(temp_filename.find_last_of('.'));
			std::string core_name = name_part + version_number + type_part;

			std::string src = "../NoSqlDb/Repository/" + name_space + "/" + core_name;
			File::copy(src, dst + "/" + retrive_file_name);
			std::cout << "retrive file:" << retrive_file_name << "     " << "Path: " << dst + "/" + retrive_file_name << std::endl;


			for (auto sub_key : db[current_key].children())
			{
				if (visited.find(sub_key) != visited.end())
					continue;
				visitqueue.push(sub_key);
			}
		}

	}
}

#ifdef TEST_CHECKOUT
bool test(DbCore<PayLoad>& db)
{
	Utilities::title("test check_out function");
	Utilities::putline();

	DbElement<PayLoad> demoelem;
	demoelem.name("checkout_test::checkout_test.txt.1");
	db["checkout_test::checkout_test.txt.1"] = demoelem;

	demoelem.name("checkout_test::checkout_test.txt.2");
	demoelem.children().push_back("checkout_test::dragonball.txt.1");
	demoelem.children().push_back("checkout_test::jojo.txt.1");
	db["checkout_test::checkout_test.txt.2"] = demoelem;

	demoelem.name("checkout_test::dragonball.txt.1");
	demoelem.children().push_back("checkout_test::goku.txt.1");
	db["checkout_test::dragonball.txt.1"] = demoelem;

	demoelem.name("checkout_test::goku.txt.1");
	db["checkout_test::goku.txt.1"] = demoelem;

	demoelem.name("checkout_test::jojo.txt.1");
	demoelem.children().push_back("checkout_test::dragonball.txt.1");
	db["checkout_test::jojo.txt.1"] = demoelem;

	CheckOut cot;
	std::string dst = "../Repository/test";
	cot.checkout(db, "checkout_test::checkout_test.txt.2", dst);
	return true;
}
int main()
{
	DbCore<PayLoad> db;
	test(db);
	getchar();
	getchar();
	return 0;
}
#endif // CHECKOUT


