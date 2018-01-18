#include "Account.h"

void Account::log_out() {
	login_status = LOG_OUT;
}

void Admin::menu() {
	char chr;
	do {
		system("cls");
		cout << "MENU ADMINISTRATORA" << endl;
		cout << "1 - Zarzadzanie biegaczami" << endl;
		cout << "2 - Zarzadzanie biegami" << endl;
		cout << "\nNacisnij ESC aby sie wylogowac" << endl;
		chr = _getch();
		switch (chr)
		{
		case '1':
			system("cls");
			manage_runner();
			break;
		case '2':
			system("cls");
			manage_run();
			break;
		case 27:
			system("cls");
			cout << "Wylogowywanie" << endl;
			log_out();
			break;
		}
	} while (login_status);
}
void Admin::manage_runner() {
	char chr;
	do {
		system("cls");
		cout << "ZARZADZANIE BIEGACZAMI" << endl;
		cout << "1 - Zaloz konto biegacza" << endl;
		cout << "2 - Wyswietl biegaczy" << endl;
		cout << "3 - Edytuj dane biegacza" << endl;
		cout << "4 - Usun biegacza" << endl;
		cout << "5 - Dodaj rezultat biegaczowi" << endl;
		cout << "6 - Wyswietl rezultaty biegacza" << endl;
		cout << "\nNacisnij ESC aby wrocic" << endl;
		chr = _getch();
		switch (chr)
		{
		case '1':
			system("cls");
			add_runner();
			break;
		case '2':
			system("cls");
			show_runner();
			break;
		case '3':
			system("cls");
			edit_runner();
			break;
		case '4':
			system("cls");
			delete_runner();
			break;
		case '5':
			system("cls");
			add_score();
			break;
		case '6':
			system("cls");
			show_score();
			break;
		}
	} while (chr!='1' && chr != '2' && chr != '3' && chr != '4' && chr != '5' && chr != '6' && chr != 27);
}
void Admin::add_runner() {
	string login;
	bool v_exist;
	do {
		system("cls");
		cout << "ZAKLADANIE KONTA BIEGACZA" << endl;
		cout << "Login: ";
		cin >> login;
		v_exist = check_login(login);
		if (v_exist) {
			cout << endl << "Podany LOGIN jest juz zajety." << endl;
			_getch();
		}
	} while (v_exist);
	string pwd, name, last_name;
	cout << "Haslo: ";
	cin >> pwd;
	pwd = md5(pwd);
	cout << "Imie: ";
	cin >> name;
	cout << "Nazwisko: ";
	cin >> last_name;
	SACommand cmd(con, "Insert into \"User\" (NAME,LAST_NAME,LOGIN,PWD,USER_TYPE) Values (:1, :2, :3, :4, :5)");
	try
	{
		cmd.Param(1).setAsString() = name.c_str();
		cmd.Param(2).setAsString() = last_name.c_str();
		cmd.Param(3).setAsString() = login.c_str();
		cmd.Param(4).setAsString() = pwd.c_str();
		cmd.Param(5).setAsLong() = RUNNER;
		cmd.Execute();
		con->Commit();
		cout << "Dodano nowego biegacza do bazy danych." << endl;
	}
	catch (SAException &x)
	{
		cout << "Biegacz nie zostal dodany do bazy danych." << endl;
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
	cout << "Zakonczono dodawanie biegacza do bazy danych." << endl;
	_getch();
}
bool Admin::check_login(string login) {
	SAConnection *con = DB_Connection::getInstance().getCon();
	SACommand cmd(con, "Select COUNT(*)\
						From \"User\"\
						Where Login=:1");
	try
	{
		cmd.Param(1).setAsString() = login.c_str();
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
int Admin::check_runner() {
	SACommand cmd(con, "Select Count(*) From \"User\" Where USER_TYPE=:1");
	try
	{
		cmd.Param(1).setAsLong() = RUNNER;
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
bool Admin::check_runner(int user_id) {
	SACommand cmd(con, "Select Count(*) From \"User\" Where USER_TYPE=:1 AND USER_ID=:2");
	try
	{
		cmd.Param(1).setAsLong() = RUNNER;
		cmd.Param(2).setAsLong() = user_id;
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
void Admin::show_runner() {
	int ch_user;
	if (ch_user = check_runner()) {
		SACommand cmd(con, "Select USER_ID, NAME, LAST_NAME From \"User\" Where USER_TYPE=:1");
		try
		{
			cmd.Param(1).setAsLong() = RUNNER;
			cmd.Execute();
			int i = 1;
			while (cmd.FetchNext())
			{
				system("cls");
				cout << "Biegacz" << endl;
				cout << "------------------------" << endl;
				cout << "USER ID: " << cmd.Field(1).asLong() << endl;
				cout << "------------------------" << endl;
				cout << "Imie: " << (string)cmd.Field(2).asString() << endl;
				cout << "Nazwisko: " << (string)cmd.Field(3).asString() << endl;
				cout << "------------------------" << endl;
				cout << i++ << " z " << ch_user << endl;
				_getch();
			}
		}
		catch (SAException &x)
		{
			try
			{
				con->Rollback();
			}
			catch (SAException &) {}
			printf("%s\n", (const char*)x.ErrText());
		}
	}
	else {
		cout << "Brak zdefiniowanych biegaczy w bazie danych." << endl;
		_getch();
	}
}
int Admin::search_runner() {
	cout << "WYSZUKIWANIE BIEGACZA" << endl;
	int user_id;
	if (check_runner()) {
		cout << "Podaj USER_ID biegacza: ";
		cin >> user_id;
		if (check_runner(user_id)) {
			cout << "Znaleziono biegacza." << endl;
		}
		else {
			cout << "Biegacz o podanym ID nie istnieje." << endl;
			user_id = 0;
		}
	}
	else {
		cout << "Brak zdefiniowanych biegaczy w bazie danych." << endl;
		user_id = 0;
	}
	cout << "Zakonczono wyszukiwanie biegacza." << endl;
	_getch();
	return user_id;
}
void Admin::edit_runner() {
	int user_id = search_runner();
	if (user_id) {
		char chr;
		do {
			system("cls");
			cout << "EDYTOWANIE DANYCH BIEGACZA" << endl;
			cout << "1 - Zmien imie" << endl;
			cout << "2 - Zmien nazwisko" << endl;
			cout << "3 - Zmien haslo" << endl;
			cout << "\nNacisnij ESC aby wrocic" << endl;
			chr = _getch();
			switch (chr)
			{
			case '1':
				system("cls");
				edit_name(user_id);
				break;
			case '2':
				system("cls");
				edit_last_name(user_id);
				break;
			case '3':
				system("cls");
				edit_pwd(user_id);
				break;
			}
		} while (chr != '1' && chr != '2' && chr != '3' && chr != 27);
		cout << "Zakonczono edytowanie danych biegacza." << endl;
		_getch();
	}
}
void Admin::edit_name(int user_id) {
	string name;
	cout << "ZMIANA IMIENIA" << endl;
	cout << "Nowe imie: ";
	cin >> name;
	SACommand cmd(con, "Update \"User\" Set NAME=:1 Where USER_ID=:2");
	try
	{
		cmd.Param(1).setAsString() = name.c_str();
		cmd.Param(2).setAsLong() = user_id;
		cmd.Execute();
		con->Commit();
		cout << "Imie zostalo zmienione." << endl;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Admin::edit_last_name(int user_id) {
	string last_name;
	cout << "ZMIANA NAZWISKA" << endl;
	cout << "Nowe nazwisko: ";
	cin >> last_name;
	SACommand cmd(con, "Update \"User\" Set LAST_NAME=:1 Where USER_ID=:2");
	try
	{
		cmd.Param(1).setAsString() = last_name.c_str();
		cmd.Param(2).setAsLong() = user_id;
		cmd.Execute();
		con->Commit();
		cout << "Nazwisko zostalo zmienione." << endl;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Admin::edit_pwd(int user_id) {
	string curr_pwd;
	cout << "ZMIANA HASLA" << endl;
	cout << "Obecne haslo: ";
	cin >> curr_pwd;
	curr_pwd = md5(curr_pwd);
	if (check_pwd(curr_pwd, user_id)) {
		string pwd;
		cout << "Nowe haslo: ";
		cin >> pwd;
		pwd = md5(pwd);
		SACommand cmd(con, "Update \"User\" Set PWD=:1 Where USER_ID=:2");
		try
		{
			cmd.Param(1).setAsString() = pwd.c_str();
			cmd.Param(2).setAsLong() = user_id;
			cmd.Execute();
			con->Commit();
			cout << "Haslo zostalo zmienione." << endl;
		}
		catch (SAException &x)
		{
			try
			{
				con->Rollback();
			}
			catch (SAException &) {}
			printf("%s\n", (const char*)x.ErrText());
		}
	}
	else {
		cout << "Wprowadzone haslo jest nieprawidlowe." << endl;
	}
}
bool Admin::check_pwd(string pwd, int user_id) {
	SACommand cmd(con, "Select COUNT(*) From \"User\" Where PWD=:1 AND USER_ID=:2");
	try
	{
		cmd.Param(1).setAsString() = pwd.c_str();
		cmd.Param(2).setAsLong() = user_id;
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
void Admin::delete_runner() {
	int user_id = search_runner();
	if (user_id) {
		system("cls");
		cout << "USUWANIE BIEGACZA" << endl;
		SACommand cmd(con, "Delete \"User\"\
							Where user_id=:1");
		try
		{
			cmd.Param(1).setAsLong() = user_id;
			cmd.Execute();
			con->Commit();
			cout << "Uzytkownik zostal usuniety" << endl;
		}
		catch (SAException &x)
		{
			try
			{
				con->Rollback();
			}
			catch (SAException &) {}
			printf("%s\n", (const char*)x.ErrText());
		}
		cout << "Zakonczono usuwanie danych biegacza." << endl;
		_getch();
	}
}
void Admin::add_score() {
	int user_id = search_runner();
	if (user_id) {
		system("cls");
		int run_id = search_run();
		if (run_id) {
			int hour, minute, second, milisecond;
			system("cls");
			cout << "DODAWANIE UZYSKANEGO PRZEZ BIEGACZA REZULTATU" << endl;
			cout << "Podaj godziny: ";
			cin >> hour;
			cout << "Podaj minuty: ";
			cin >> minute;
			cout << "Podaj sekundy: ";
			cin >> second;
			cout << "Podaj milisekundy: ";
			cin >> milisecond;
			string interval = "INTERVAL '0 " + to_string(hour) + ":" + to_string(minute) + ":" + to_string(second) + "." + to_string(milisecond) + "' DAY TO SECOND";
			SACommand cmd(con);
			string query = "Insert into Score (User_ID, Run_ID, TIME) Values (:1, :2, " + interval + ")";
			cmd.setCommandText(query.c_str());
			try
			{
				cmd.Param(1).setAsLong() = user_id;
				cmd.Param(2).setAsLong() = run_id;
				cmd.Execute();
				con->Commit();
				cout << "Rezultat zostal dodany do bazy danych." << endl;
			}
			catch (SAException &x)
			{
				cout << "Rezultat nie zostal dodany do bazy danych." << endl;
				try
				{
					con->Rollback();
				}
				catch (SAException &) {}
				printf("%s\n", (const char*)x.ErrText());
			}
			cout << "Zakonczono dodawanie rezultatu do bazy danych." << endl;
			_getch();
		}
	}
}
int Admin::check_score() {
	SACommand cmd(con, "Select Count(*) From Score");
	try
	{
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
int Admin::check_score(int user_id) {
	SACommand cmd(con, "Select Count(*) From Score Where USER_ID=:1");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
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
void Admin::show_score() {
	int user_id = search_runner();
	if (user_id) {
		if (check_score()) {
			int ch_score = check_score(user_id);
			if (ch_score) {
				SACommand cmd(con, "Select \"User\".USER_ID, \"User\".NAME, \"User\".LAST_NAME, Run.RUN_ID, Run.NAME, Run.\"date\", Run.DISTANCE, Score.TIME\
									From \"User\", Score, Run\
									Where \"User\".USER_ID=:1 AND \"User\".USER_ID=Score.USER_ID AND RUN.run_id=Score.run_id");
				try
				{
					cmd.Param(1).setAsLong() = user_id;
					cmd.Execute();
					int i = 1;
					while (cmd.FetchNext())
					{
						system("cls");
						string date = (string)cmd.Field(6).asString();
						string time = (string)cmd.Field(8).asString();
						cout << "Wynik" << endl;
						cout << "------------------------" << endl;
						cout << "USER ID: " << cmd.Field(1).asLong() << endl;
						cout << "------------------------" << endl;
						cout << "Imie: " << (string)cmd.Field(2).asString() << endl;
						cout << "Nazwisko: " << (string)cmd.Field(3).asString() << endl;
						cout << "------------------------" << endl;
						cout << "RUN ID: " << cmd.Field(4).asLong() << endl;
						cout << "Nazwa: " << (string)cmd.Field(5).asString() << endl;
						cout << "Data: " << (string)date.substr(0, 10) << " " << date.substr(12, 16) << endl;
						cout << "------------------------" << endl;
						cout << "Dystans: " << cmd.Field(7).asDouble() << endl;
						cout << "Czas: " << time.substr(2, 16) << endl;
						cout << "------------------------" << endl;
						cout << i++ << " z " << ch_score << endl;
						_getch();
					}
				}
				catch (SAException &x)
				{
					try
					{
						con->Rollback();
					}
					catch (SAException &) {}
					printf("%s\n", (const char*)x.ErrText());
				}
			} else {
				cout << "Brak zdefiniowanych rezultatow dla biegacza o podanym ID" << endl;
				_getch();
			}
		} else {
			cout << "Brak zdefiniowanych rezultatow w bazie danych" << endl;
			_getch();
		}
	} else {
		cout << "Brak zdefiniowanych biegaczy w bazie danych." << endl;
		_getch();
	}
}
void Admin::manage_run() {
	char chr;
	do {
		system("cls");
		cout << "ZARZADZANIE BIEGAMI" << endl;
		cout << "1 - Dodaj bieg" << endl;
		cout << "2 - Wyswietl biegi" << endl;
		cout << "3 - Edytuj parametry biegu" << endl;
		cout << "4 - Usun bieg" << endl;
		cout << "\nNacisnij ESC aby wrocic" << endl;
		chr = _getch();
		switch (chr)
		{
		case '1':
			system("cls");
			add_run();
			break;
		case '2':
			system("cls");
			show_run();
			break;
		case '3':
			system("cls");
			edit_run();
			break;
		case '4':
			system("cls");
			delete_run();
			break;
		}
	} while (chr != '1' && chr != '2' && chr != '3' && chr != '4' && chr != 27);
}
void Admin::add_run() {
	string name;
	int day, month, year, hour, minute;
	float distance;
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {};
	system("cls");
	cout << "DODAWANIE BIEGU" << endl;
	cout << "Nazwa: ";
	getline(cin, name);
	cout << "Podaj dzien: ";
	cin >> day;
	cout << "Podaj miesiac: ";
	cin >> month;
	cout << "Podaj rok: ";
	cin >> year;
	cout << "Podaj godzine: ";
	cin >> hour;
	cout << "Podaj minuty: ";
	cin >> minute;
	cout << "Podaj dystans (km): ";
	cin >> distance;
	SACommand cmd(con, "Insert into Run (NAME,\"date\",DISTANCE) Values (:1, :2, :3)");
	try
	{
		cmd.Param(1).setAsString() = name.c_str();
		cmd.Param(2).setAsDateTime() = SADateTime(year, month, day, hour, minute, 0);
		cmd.Param(3).setAsDouble() = distance;
		cmd.Execute();
		con->Commit();
		cout << "Dodano nowy bieg do bazy danych." << endl;
	}
	catch (SAException &x)
	{
		cout << "Bieg nie zostal dodany do bazy danych." << endl;
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
	cout << "Zakonczono dodawanie biegu do bazy danych." << endl;
	_getch();
}
int Admin::check_run() {
	SACommand cmd(con, "Select Count(*) From Run");
	try
	{
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
bool Admin::check_run(int run_id) {
	SACommand cmd(con, "Select Count(*) From Run Where RUN_ID=:1");
	try
	{
		cmd.Param(1).setAsLong() = run_id;
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
void Admin::show_run() {
	int ch_run;
	if (ch_run = check_run()) {
		SACommand cmd(con, "Select RUN_ID, NAME, \"date\", DISTANCE From Run");
		try
		{
			cmd.Execute();
			int i = 1;
			while (cmd.FetchNext())
			{
				system("cls");
				string date = (string)cmd.Field(3).asString();
				cout << "Bieg" << endl;
				cout << "------------------------" << endl;
				cout << "RUN ID: " << cmd.Field(1).asLong() << endl;
				cout << "------------------------" << endl;
				cout << "Nazwa: " << (string)cmd.Field(2).asString() << endl;
				cout << "Data: " << date.substr(0,10) << " " << date.substr(12,16) << endl;
				cout << "Dystans (km): " << cmd.Field(4).asDouble() << endl;
				cout << "------------------------" << endl;
				cout << i++ << " z " << ch_run << endl;
				_getch();
			}
		}
		catch (SAException &x)
		{
			try
			{
				con->Rollback();
			}
			catch (SAException &) {}
			printf("%s\n", (const char*)x.ErrText());
		}
	}
	else {
		cout << "Brak zdefiniowanych biegow w bazie danych." << endl;
		_getch();
	}
}
int Admin::search_run() {
	cout << "WYSZUKIWANIE BIEGU" << endl;
	int run_id;
	if (check_run()) {
		cout << "Podaj RUN_ID biegu: ";
		cin >> run_id;
		if (check_run(run_id)) {
			cout << "Znaleziono bieg." << endl;
		}
		else {
			cout << "Bieg o podanym ID nie istnieje." << endl;
			run_id = 0;
		}
	}
	else {
		cout << "Brak zdefiniowanych biegow w bazie danych." << endl;
		run_id = 0;
	}
	cout << "Zakonczono wyszukiwanie biegu." << endl;
	_getch();
	return run_id;
}
void Admin::edit_run() {
	int run_id = search_run();
	if (run_id) {
		char chr;
		do {
			system("cls");
			cout << "EDYTOWANIE PARAMETROW BIEGU" << endl;
			cout << "1 - Zmien nazwe" << endl;
			cout << "2 - Zmien date" << endl;
			cout << "3 - Zmien dystans" << endl;
			cout << "\nNacisnij ESC aby wrocic" << endl;
			chr = _getch();
			switch (chr)
			{
			case '1':
				system("cls");
				edit_run_name(run_id);
				break;
			case '2':
				system("cls");
				edit_date(run_id);
				break;
			case '3':
				system("cls");
				edit_distance(run_id);
				break;
			}
		} while (chr != '1' && chr != '2' && chr != '3' && chr != 27);
		cout << "Zakonczono edytowanie danych biegu." << endl;
		_getch();
	}
}
void Admin::edit_run_name(int run_id) {
	string name;
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {};
	cout << "ZMIANA NAZWY BIEGU" << endl;
	cout << "Nowa nazwa: ";
	getline(cin, name);

	SACommand cmd(con, "Update Run Set NAME=:1 Where RUN_ID=:2");
	try
	{
		cmd.Param(1).setAsString() = name.c_str();
		cmd.Param(2).setAsLong() = run_id;
		cmd.Execute();
		con->Commit();
		cout << "Nazwa biegu zostala zmieniona." << endl;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Admin::edit_date(int run_id) {
	int day, month, year, hour, minute;
	cout << "ZMIANA DATY" << endl;
	cout << "Podaj dzien: ";
	cin >> day;
	cout << "Podaj miesiac: ";
	cin >> month;
	cout << "Podaj rok: ";
	cin >> year;
	cout << "Podaj godzine: ";
	cin >> hour;
	cout << "Podaj minuty: ";
	cin >> minute;
	SACommand cmd(con, "Update Run Set \"date\"=:1 Where RUN_ID=:2");
	try
	{
		cmd.Param(1).setAsDateTime() = SADateTime(year, month, day, hour, minute, 0);
		cmd.Param(2).setAsLong() = run_id;
		cmd.Execute();
		con->Commit();
		cout << "Data biegu zostala zmieniona." << endl;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Admin::edit_distance(int run_id) {
	float distance;
	cout << "ZMIANA DYSTANSU" << endl;
	cout << "Nowy dystans (km): ";
	cin >> distance;
	SACommand cmd(con, "Update Run Set DISTANCE=:1 Where RUN_ID=:2");
	try
	{
		cmd.Param(1).setAsDouble() = distance;
		cmd.Param(2).setAsLong() = run_id;
		cmd.Execute();
		con->Commit();
		cout << "Dystans zostal zmieniony." << endl;
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Admin::delete_run() {
	int run_id = search_run();
	if (run_id) {
		system("cls");
		cout << "USUWANIE BIEGU" << endl;
		SACommand cmd(con, "Delete Run\
							Where run_id=:1");
		try
		{
			cmd.Param(1).setAsLong() = run_id;
			cmd.Execute();
			con->Commit();
			cout << "Bieg zostal usuniety" << endl;
		}
		catch (SAException &x)
		{
			try
			{
				con->Rollback();
			}
			catch (SAException &) {}
			printf("%s\n", (const char*)x.ErrText());
		}
		cout << "Zakonczono usuwanie biegu." << endl;
		_getch();
	}
}

void Runner::menu() {
	char chr;
	do {
		system("cls");
		cout << "MENU BIEGACZA" << endl;
		cout << "1 - Wyswietl swoje rezultaty" << endl;
		cout << "2 - Statystyki" << endl;
		cout << "\nNacisnij ESC aby sie wylogowac" << endl;
		chr = _getch();
		switch (chr)
		{
		case '1':
			show_score(user_id);
			break;
		case '2':
			manage_stats();
			break;
		case 27:
			system("cls");
			cout << "Wylogowywanie" << endl;
			log_out();
			break;
		}
	} while (login_status);
}
int Runner::check_score() {
	SACommand cmd(con, "Select Count(*) From Score");
	try
	{
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
int Runner::check_score(int user_id) {
	SACommand cmd(con, "Select Count(*) From Score Where USER_ID=:1");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
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
void Runner::show_score(int user_id) {
	if (check_score()) {
		int ch_score = check_score(user_id);
		if (ch_score) {
			SACommand cmd(con, "Select \"User\".USER_ID, \"User\".NAME, \"User\".LAST_NAME, Run.RUN_ID, Run.NAME, Run.\"date\", Run.DISTANCE, Score.TIME\
								From \"User\", Score, Run\
								Where \"User\".USER_ID=:1 AND \"User\".USER_ID=Score.USER_ID AND RUN.run_id=Score.run_id");
			try
			{
				cmd.Param(1).setAsLong() = user_id;
				cmd.Execute();
				int i = 1;
				while (cmd.FetchNext())
				{
					system("cls");
					string date = (string)cmd.Field(6).asString();
					string time = (string)cmd.Field(8).asString();
					cout << "Wynik" << endl;
					cout << "------------------------" << endl;
					cout << "USER ID: " << cmd.Field(1).asLong() << endl;
					cout << "------------------------" << endl;
					cout << "Imie: " << (string)cmd.Field(2).asString() << endl;
					cout << "Nazwisko: " << (string)cmd.Field(3).asString() << endl;
					cout << "------------------------" << endl;
					cout << "RUN ID: " << cmd.Field(4).asLong() << endl;
					cout << "Nazwa: " << (string)cmd.Field(5).asString() << endl;
					cout << "Data: " << (string)date.substr(0, 10) << " " << date.substr(12, 16) << endl;
					cout << "------------------------" << endl;
					cout << "Dystans: " << cmd.Field(7).asDouble() << endl;
					cout << "Czas: " << time.substr(2, 16) << endl;
					cout << "------------------------" << endl;
					cout << i++ << " z " << ch_score << endl;
					_getch();
				}
			}
			catch (SAException &x)
			{
				try
				{
					con->Rollback();
				}
				catch (SAException &) {}
				printf("%s\n", (const char*)x.ErrText());
			}
		}
		else {
			cout << "Brak zdefiniowanych rezultatow dla biegacza o podanym ID" << endl;
			_getch();
		}
	}
	else {
		cout << "Brak zdefiniowanych rezultatow w bazie danych" << endl;
		_getch();
	}
}
void Runner::manage_stats() {
	if (check_score()) {
		if (check_score(user_id)) {
			char chr;
			do {
				system("cls");
				cout << "STATYSTYKI" << endl;
				cout << "1 - Srednie predkosci uzyskane w biegach" << endl;
				cout << "2 - Najmniejsza srednia predkosc uzyskana w biegu " << endl;
				cout << "3 - Najwieksza srednia predkosc uzyskana w biegu " << endl;
				cout << "4 - Przebyty dystans (suma)" << endl;
				cout << "5 - Najkrotszy przebyty dystans" << endl;
				cout << "6 - Najdluzszy przebyty dystans" << endl;
				cout << "\nNacisnij ESC aby wrocic" << endl;
				chr = _getch();
				switch (chr)
				{
				case '1':
					system("cls");
					mean_speed_stats();
					break;
				case '2':
					system("cls");
					min_mean_speed_stats();
					break;
				case '3':
					system("cls");
					max_mean_speed_stats();
					break;
				case '4':
					system("cls");
					sum_distance();
					break;
				case '5':
					system("cls");
					min_distance();
					break;
				case '6':
					system("cls");
					max_distance();
					break;
				}
			} while (chr != '1' && chr != '2' && chr != '3' && chr != '4' && chr != '5' && chr != '6' && chr != 27);
			cout << "Zakonczono pokazywanie statystyk biegacza." << endl;
			_getch();
		}
		else {
			cout << "Brak zdefiniowanych rezultatow oraz statystyk dla biegacza o podanym ID" << endl;
			_getch();
		}
	}
	else {
		cout << "Brak zdefiniowanych rezultatow oraz statystyk dla biegaczy w bazie danych" << endl;
		_getch();
	}
}
void Runner::mean_speed_stats() {
	SACommand cmd(con, "Select Run.RUN_ID, Run.NAME, Run.\"date\", Score.TIME, Run.DISTANCE,\
							   EXTRACT(DAY FROM Score.TIME) * 86400 + EXTRACT(HOUR FROM Score.TIME) * 3600 + EXTRACT(MINUTE FROM Score.TIME) * 60 + EXTRACT(SECOND FROM Score.TIME)\
						From Score, Run\
						Where Score.USER_ID=:1 AND RUN.run_id=Score.run_id");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
		cmd.Execute();
		int ch_score = check_score(user_id);
		int i = 1;
		while (cmd.FetchNext())
		{
			system("cls");
			string date = (string)cmd.Field(3).asString();
			string time = (string)cmd.Field(4).asString();
			double distance = cmd.Field(5).asDouble();
			double seconds = cmd.Field(6).asDouble();
			double mean_speed = distance * 1000 / seconds;
			cout << "Statystyka - srednia predkosc" << endl;
			cout << "------------------------" << endl;
			cout << "RUN ID: " << cmd.Field(1).asLong() << endl;
			cout << "Nazwa: " << (string)cmd.Field(2).asString() << endl;
			cout << "Data: " << (string)date.substr(0, 10) << " " << date.substr(12, 16) << endl;
			cout << "------------------------" << endl;
			cout << "Dystans (km): " << distance << endl;
			cout << "Czas: " << time.substr(2, 16) << endl;
			cout << "Czas (s): " << seconds << endl;
			printf( "Srednia predkosc (m/s): %.2f\n", mean_speed);
			cout << "------------------------" << endl;
			cout << i++ << " z " << ch_score << endl;
			_getch();
		}
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Runner::min_mean_speed_stats() {
	SACommand cmd(con, "Select Run.DISTANCE,\
							   EXTRACT(DAY FROM Score.TIME) * 86400 + EXTRACT(HOUR FROM Score.TIME) * 3600 + EXTRACT(MINUTE FROM Score.TIME) * 60 + EXTRACT(SECOND FROM Score.TIME)\
						From Score, Run\
						Where Score.USER_ID=:1 AND RUN.run_id=Score.run_id");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
		cmd.Execute();
		int i=1;
		double min_mean_speed;
		while (cmd.FetchNext())
		{
			double distance = cmd.Field(1).asDouble();
			double seconds = cmd.Field(2).asDouble();
			double mean_speed = distance * 1000 / seconds;
			if (i == 1)
				min_mean_speed = mean_speed;
			else
				min_mean_speed=[](double a, double b)->double {if (a > b) return b; else return a; }(min_mean_speed, mean_speed); //Wyrazenie lambda, porownanie liczb i zwrocenie mniejszej
			i++;
		}
		cout << "Statystyka - najmniejsza srednia predkosc" << endl;
		cout << "------------------------" << endl;
		printf("Srednia predkosc (m/s): %.2f\n", min_mean_speed);
		cout << "------------------------" << endl;
		_getch();
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Runner::max_mean_speed_stats() {
	SACommand cmd(con, "Select Run.DISTANCE,\
							   EXTRACT(DAY FROM Score.TIME) * 86400 + EXTRACT(HOUR FROM Score.TIME) * 3600 + EXTRACT(MINUTE FROM Score.TIME) * 60 + EXTRACT(SECOND FROM Score.TIME)\
						From Score, Run\
						Where Score.USER_ID=:1 AND RUN.run_id=Score.run_id");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
		cmd.Execute();
		int i = 1;
		double max_mean_speed;
		while (cmd.FetchNext())
		{
			double distance = cmd.Field(1).asDouble();
			double seconds = cmd.Field(2).asDouble();
			double mean_speed = distance * 1000 / seconds;
			if (i == 1)
				max_mean_speed = mean_speed;
			else
				max_mean_speed = [](double a, double b)->double {if (a < b) return b; else return a; }(max_mean_speed, mean_speed); //Wyrazenie lambda, porownanie liczb i zwrocenie wiekszej
			i++;
		}
		cout << "Statystyka - najwieksza srednia predkosc" << endl;
		cout << "------------------------" << endl;
		printf("Srednia predkosc (m/s): %.2f\n", max_mean_speed);
		cout << "------------------------" << endl;
		_getch();
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Runner::sum_distance() {
	SACommand cmd(con, "Select SUM(Run.DISTANCE)\
						From Score, Run\
						Where Score.USER_ID=:1 AND RUN.run_id=Score.run_id");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
		cmd.Execute();
		while (cmd.FetchNext())
		{
			system("cls");
			cout << "Statystyka - przebyty dystans (suma)" << endl;
			cout << "------------------------" << endl;
			cout << "Dystans (km): " << cmd.Field(1).asDouble() << endl;
			cout << "------------------------" << endl;
			_getch();
		}
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Runner::min_distance() {
	SACommand cmd(con, "Select Run.DISTANCE\
						From Score, Run\
						Where Score.USER_ID=:1 AND RUN.run_id=Score.run_id");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
		cmd.Execute();
		int i = 1;
		double min_distance;
		while (cmd.FetchNext())
		{
			double distance = cmd.Field(1).asDouble();
			if (i == 1)
				min_distance = distance;
			else
				min_distance = [](double a, double b)->double {if (a > b) return b; else return a; }(min_distance, distance); //Wyrazenie lambda, porownanie liczb i zwrocenie mniejszej
			i++;
		}
		cout << "Statystyka - najkrotszy przebyty dystans" << endl;
		cout << "------------------------" << endl;
		cout << "Dystans (km): " << min_distance << endl;
		cout << "------------------------" << endl;
		_getch();
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}
void Runner::max_distance() {
	SACommand cmd(con, "Select Run.DISTANCE\
						From Score, Run\
						Where Score.USER_ID=:1 AND RUN.run_id=Score.run_id");
	try
	{
		cmd.Param(1).setAsLong() = user_id;
		cmd.Execute();
		int i = 1;
		double max_distance;
		while (cmd.FetchNext())
		{
			double distance = cmd.Field(1).asDouble();
			if (i == 1)
				max_distance = distance;
			else
				max_distance = [](double a, double b)->double {if (a < b) return b; else return a; }(max_distance, distance); //Wyrazenie lambda, porownanie liczb i zwrocenie wiekszej
			i++;
		}
		cout << "Statystyka - najdluzszy przebyty dystans" << endl;
		cout << "------------------------" << endl;
		cout << "Dystans (km): " << max_distance << endl;
		cout << "------------------------" << endl;
		_getch();
	}
	catch (SAException &x)
	{
		try
		{
			con->Rollback();
		}
		catch (SAException &) {}
		printf("%s\n", (const char*)x.ErrText());
	}
}