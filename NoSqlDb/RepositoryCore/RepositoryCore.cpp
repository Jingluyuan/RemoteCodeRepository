///////////////////////////////////////////////////////////////////////
// RepositoryCore.cpp -  test functions in this class              //
// ver 1.0                                                           //
//Jinglu Yuan, CSE687 - Object Oriented Design, Spring 2018          //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "RepositoryCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"



bool RepositoryCore::check_in_file(std::string name_space, std::string filename, std::string description, Children children, std::string path, std::vector<std::string> categories, std::string flag)
{
	Version vsn;
	Key temp_key;
	DbElement<PayLoad> elem;
	CheckIn cki;

	if (vsn.initial_version_judge(db_, name_space, filename))
	{
		temp_key = name_space + "::" + filename + ".1";
	}
	else
	{
		if (db_[vsn.find_latest_version(db_, name_space, filename)].payLoad().flag().compare("open") == 0)
		{
			temp_key = vsn.find_latest_version(db_, name_space, filename);
		}
		else
		{
			temp_key = vsn.version_number_update(db_, name_space, filename);
		}
	}

	elem.name(temp_key);
	elem.descrip(description);
	elem.children(children);
	elem.payLoad().value(path);
	elem.payLoad().categories(categories);
	elem.payLoad().flag(flag);
	db_[temp_key] = elem;

	cki.checkin(db_, temp_key, name_space, elem);
	if (db_.contains(temp_key))
	{
		return true;
	}
	return false;

}

bool RepositoryCore::check_out_file(Key key, std::string dst)
{
	if (!db_.contains(key)) return false;
	CheckOut cot;
	cot.checkout(db_, key, dst);
	return true;
}

std::vector<std::string> RepositoryCore::browsing_file(Conditions<PayLoad>& cons_)
{
	Browse bro;
	return bro.browse(db_, cons_);
}

void RepositoryCore::status_control(Key key, std::string status)
{
	db_[key].payLoad().flag(status);
}
#ifdef TEST_REPOSITORYCORE
int main()
{
	
	return 0;
}

#endif 





