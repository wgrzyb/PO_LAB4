#pragma once
#include <iostream>
#include <string>
#include <oraAPI.h>
#include <SQLAPI.h>
using namespace std;

class DB_Connection {
	SAConnection *con;
public:
	DB_Connection() {};
	DB_Connection(DB_Connection const&) = delete;
	DB_Connection& operator=(DB_Connection const&) = delete;
	~DB_Connection() { disconnect(); };
	static DB_Connection& getInstance()
	{
		static DB_Connection instance;
		return instance;
	}
	void setConnection(SAString db_name, SAString user, SAString pwd);
	void setConnection();
	void disconnect();
	SAConnection *getCon();
};