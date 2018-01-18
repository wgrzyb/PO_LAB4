#pragma once
#include <conio.h>
#include "DB_Connection.h"
#include "MD5.h"

enum account_type { ADMIN = 1, RUNNER = 2 };
typedef enum account_type account_type;

enum login_status { LOG_OUT = 0, LOG_IN = 1 };
typedef enum login_status login_status;

class Account {
protected:
	int user_id;
	int login_status;
	SAConnection *con;
	void log_out();
public:
	Account(int user_id, int login_status) :user_id(user_id), login_status(login_status), con(DB_Connection::getInstance().getCon()) {};
	~Account() {};
	virtual void menu()=0;
};

class Admin :public Account{
	void manage_runner();
	void add_runner();
	bool check_login(string login);
	int check_runner(); //sprawdza ilu biegaczy znajduje sie w bazie danych
	bool check_runner(int user_id); //sprawdza czy znajduje sie w bazie danych biegacz o podanym id
	void show_runner();
	int search_runner();
	void edit_runner();
	void edit_name(int user_id);
	void edit_last_name(int user_id);
	void edit_pwd(int user_id);
	bool check_pwd(string pwd, int user_id);
	void delete_runner();
	void manage_run();
	void add_run();
	int check_run(); //sprawdza ile biegow znajduje sie w bazie danych
	bool check_run(int run_id); //sprawdza czy znajduje sie w bazie danych bieg o podanym id
	void show_run();
	int search_run();
	void edit_run();
	void edit_run_name(int run_id);
	void edit_date(int run_id);
	void edit_distance(int run_id);
	void delete_run();
	void add_score();
	int check_score(); //sprawdza ile rezultatow znajduje sie w bazie danych
	int check_score(int user_id); //sprawdza ile rezultatow posiada biegacz o podanym id
	void show_score();
public:
	Admin(int user_id) :Account(user_id, LOG_IN) {};
	void menu();
};

class Runner :public Account{
	int check_score();
	int check_score(int user_id);
	void show_score(int user_id);
	void manage_stats();
	void mean_speed_stats();
	void min_mean_speed_stats();
	void max_mean_speed_stats();
	void sum_distance();
	void min_distance();
	void max_distance();
public:
	Runner(int user_id) :Account(user_id, LOG_IN) {};
	void menu();
};