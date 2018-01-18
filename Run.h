#pragma once
#include "Account.h"

class Run {
	SAConnection *con;
	bool valid_data(string login, string pwd);
	int get_account_type(string login);
	int get_user_id(string login);
	void log_in();
public:
	Run(string db_name, string user, string pwd);
	void menu();
	~Run();
};