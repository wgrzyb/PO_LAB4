#include "DB_Connection.h"

void DB_Connection::setConnection() {
	con = new SAConnection();
	con->setClient(SA_Oracle_Client);
	string db_name, user, pwd;
	cout << "Laczenie z baza danych..." << endl;
	cout << "Podaj db_name: ";
	cin >> db_name;
	cout << "Podaj nazwe uzytkownika: ";
	cin >> user;
	cout << "Podaj haslo: ";
	cin >> pwd;
	try
	{
		con->Connect(db_name.c_str(), user.c_str(), pwd.c_str());
		cout << "Polaczono z Oracle database." << endl;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());

		cout << "Nie udalo sie polaczyc z baza danych." << endl << "Zamykanie aplikacji" << endl << endl;
		system("pause");
		exit(0);
	}
}
void DB_Connection::setConnection(SAString db_name, SAString user, SAString pwd) {
	con = new SAConnection();
	con->setClient(SA_Oracle_Client);
	cout << "Laczenie z baza danych..." << endl;
	try
	{
		con->Connect(db_name, user, pwd);
		cout << "Polaczono z Oracle database." << endl;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());

		cout << "Nie udalo sie polaczyc z baza danych." << endl << "Zamykanie aplikacji" << endl << endl;
		system("pause");
		exit(0);
	}
}
void DB_Connection::disconnect() {
	if (con->isConnected()) {
		con->Disconnect();
		cout << "Rozlaczono z Oracle database." << endl;
	}
}
SAConnection *DB_Connection::getCon() {
	return con;
}