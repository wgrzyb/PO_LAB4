#include "Run.h"

Run::Run(string db_name, string user, string pwd) {
	SetConsoleTitle(L"Rozgrywki biegowe");
	DB_Connection::getInstance().setConnection(db_name.c_str(), user.c_str(), pwd.c_str());
	//DatabaseConnection::getInstance().setConnection();
	con = DB_Connection::getInstance().getCon();
	_getch();
}
Run::~Run() {
	DB_Connection::getInstance().disconnect();
}
void Run::menu() {
	char chr;
	do {
		system("cls");
		cout << "MENU" << endl;
		cout << "1 - Zaloguj sie" << endl;
		cout << "\nNacisnij ESC aby zamknac aplikacje" << endl;
		chr = _getch();
		switch (chr)
		{
		case '1':
			system("cls");
			log_in();
			break;
		case 27:
			system("cls");
			cout << "Zamykanie aplikacji" << endl;
			break;
		}
	} while (chr != 27);
}

void Run::log_in() {
	system("cls");
	string login, pwd;
	cout << "LOGOWANIE" << endl;
	cout << "Login: ";
	cin >> login;
	cout << "Haslo: ";
	cin >> pwd;
	pwd = md5(pwd);
	if (valid_data(login,pwd)) {
		Account *account;
		switch (get_account_type(login))
		{
		case ADMIN:
			account = new Admin(get_user_id(login));
			break;
		case RUNNER:
			account = new Runner(get_user_id(login));
			break;
		}
		account->menu();
		delete account;
		cout << "Zostales pomyslnie wylogowany." << endl;
	}
	else {
		cout << endl << "Niepoprawne dane logowania." << endl;
	}
	_getch();
}

bool Run::valid_data(string login, string pwd) {
	SACommand cmd(con, "Select COUNT(*)\
						From \"User\"\
						Where Login=:1 AND Pwd=:2");
	try
	{
		cmd.Param(1).setAsString() = login.c_str();
		cmd.Param(2).setAsString() = pwd.c_str();
		cmd.Execute();
		if (cmd.FetchNext())
		{
			return cmd.Field(1).asBool();
		}
		return 0;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
		return 0;
	}
}

int Run::get_account_type(string login) {
	SACommand cmd(con, "Select User_Type\
						From \"User\"\
						Where Login=:1");
	try
	{
		cmd.Param(1).setAsString() = login.c_str();
		cmd.Execute();
		if (cmd.FetchNext())
		{
			return cmd.Field(1).asLong();
		}
		return 0;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
		return 0;
	}
}

int Run::get_user_id(string login) {
	SACommand cmd(con, "Select User_ID\
						From \"User\"\
						Where Login=:1");
	try
	{
		cmd.Param(1).setAsString() = login.c_str();
		cmd.Execute();
		if (cmd.FetchNext())
		{
			return cmd.Field(1).asLong();
		}
		return 0;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
		return 0;
	}
}