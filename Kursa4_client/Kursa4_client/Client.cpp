#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include "StreamTable.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <iterator>
#include <fcntl.h>
#include <windows.h>
#include <fstream>
#include <sstream>

CRITICAL_SECTION cs;

#pragma warning(disable: 4996)

SOCKET Connection;

using namespace std;
//int vvod();
//double vvod_pay();

class InvalidPrice : public exception {
public:
	const char* what() const noexcept override {
		return "Price must be greater than 0!";
	}
};

template <typename T>
T vvod() {
	T a;
	while (!(cin >> a) || (cin.peek() != '\n'))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Error!" << endl << "Попробуйте ещё раз" << endl;
	}
	if (a <= 0) {
		throw InvalidPrice();
	}
	return a;
}


class Job {
public:
	vector<string> wor;
	void save_job(vector<string> wor, int count_job);
	vector<string> load_job(vector<string> wor, int* count_job);
	void new_job();
	void show_job();
	void del_job();
};





class Bank
{
protected:
	char title[20]="БелБанк";
	char address[20]="Минск";

	int count_employee;
public:
	//void show_info_bank() {};
};

class Director
{
protected:
	char nameD[20]="Владислав";
	char surnameD[20]="Бахмат";
public:
	//void show_info_director() {};
};

void menu(); 
int HeadMenu();
void Otprawka();
int AdminMenu();
int UserMenu();

enum Packet {
	P_ChatMessage,
	P_Test,
	P_Test1,
	P_Good,
	P_New,
	P_Del,
	P_ChatMessage_ad
};

class Worker : public Bank, public Director
{
protected:
	char name[20];
	char surname[20];
	double pay;
	char position[20];
public:
	int count_pick_job, * job;
	double koef;

	Worker* create_new_record(Worker* A, int* count_emp);
	void get_pay();
	//void het_job(int pos);

};

class Employee:public Worker {
public:
	char password[20], login[20];

	int key;

	int id;

//public:
	void see() {
		cout << login << ' ' << password << endl;
	}
	Employee() {
		char j[20] = "0";
		strcpy_s(password, j);
		strcpy_s(login, j);
		id = 0; key = 0;
	}
	char* login_r()
	{
		return login;
	}
	char* password_r()
	{
		return password;
	}
	int* id_r() {
		return &id;
	}
	int* key_r() {
		return &key;
	}
	Employee* sort_name(Employee* A, int* count_emp)
	{
		Employee sort;
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		char str[23];
		for (int i = 1; i < *count_emp; i++)
			for (int j = 0; j < *count_emp - i; j++)
				if (strcmp(A[j].name, A[j + 1].name) > 0)
				{
					memcpy(&sort, &A[j], sizeof(Employee));
					memcpy(&A[j], &A[j + 1], sizeof(Employee));
					memcpy(&A[j + 1], &sort, sizeof(Employee));
				}
		return A;
	}
	Employee* sort_gruppa(Employee* A, int* count_emp)
	{
		Employee sort;
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		char str[23];
		for (int i = 1; i < *count_emp; i++)
			for (int j = 0; j < *count_emp - i; j++)
				if (A[j].koef < A[j + 1].koef)
				{
					memcpy(&sort, &A[j], sizeof(Employee));
					memcpy(&A[j], &A[j + 1], sizeof(Employee));
					memcpy(&A[j + 1], &sort, sizeof(Employee));
				}
		return A;
	}
	void het_job(int pos, Employee* user, int* count);

	void show_info_bank() {
		cout << endl <<"Название банка:"<< title << endl<<"Адресс банка" << address << endl;
	}

	void show_info_director() {
		cout << endl<<"Имя директора" << nameD << ' '<< "Фамилия директора" << surnameD << endl;
	}

	void save_on_file_s(Employee* user, int count);
	Employee* create_new_user(Employee* user, int* count);
	Employee* load_from_file(Employee* user, int* count);
	void save_on_file(Employee* user, int count);
	Packet authorization(char* log, char* pas);
	void print_user(Employee* user, int count);
	//	void delet_user(Employee* user, int* count);
	void delet_user(Employee* user, int* count);
	void premia(Employee* user, int* count);
};


void Employee::print_user(Employee* user, int count)
{
	//int i;
	printf("\nСписок пользователей\n\n");
	//fseek(stdin, 0, SEEK_END);
	user=user->load_from_file(user, &count);
	fseek(stdin, 0, SEEK_END);
	StreamTable st(std::cout);
	st.AddCol(5);
	st.AddCol(20);
	st.AddCol(20);
	st.AddCol(20);
	st.AddCol(20);
	st.AddCol(10);
	st.AddCol(15);
	//st.AddCol(20);


	//разкомментировать, если столбцы имеют одинаковую толщину
	//st.Clear();
	//st.SetCols(4, 10);

	//st.SetVisible(4, false);//столбцы можно скрывать

	st.MakeBorderExt(true);
	st.SetDelimRow(true, '-');//st.SetDelimRow(false);//без символов-разделителей строк
	st.SetDelimCol(true, '|');//st.SetDelimCol(false);//без символов-разделителей строк

	//заголовок и значения выводятся одинаково
	st << "ID" << "Login" << "Name" << "Surname" << "Position" << "Pay" << "Count_pick_job";
	//enum { nr = 10 };
	for (int i = 0; i < count; i++) {
		st << user[i].id << user[i].login << user[i].name << user[i].surname << user[i].position << user[i].pay << user[i].count_pick_job;
	}
}

Employee* Employee::load_from_file(Employee* user, int* count)
{
	FILE* fp;
	if ((fp = fopen("C:\\Users\\Asus\\source\\repos\\Kursa4_client\\Kursa4_client\\user", "rb")) == NULL)
	{
		cout << "\nОшибка чтения файла.\n";
		return 0;
	}
	fread(count, sizeof(int), 1, fp);
	if (*count <= 0 || count == NULL)
		*count = 0;
	//free(user);
	user = NULL;
	user = new Employee[*count * sizeof(Employee)];
	for (int i = 0; i < *count; i++)
	{
		fread(&user[i].id, sizeof(int), 1, fp);
		fread(user[i].login, sizeof(char) * 20, 1, fp);
		fread(user[i].password, sizeof(char) * 20, 1, fp);
		fread(&user[i].key, sizeof(int), 1, fp);
		fread(user[i].name, sizeof(char) * 20, 1, fp);
		fread(user[i].surname, sizeof(char) * 20, 1, fp);
		fread(user[i].position, sizeof(char) * 20, 1, fp);
		fread(&user[i].pay, sizeof(double), 1, fp);
		fread(&user[i].count_pick_job, sizeof(int), 1, fp);
		user[i].job = new int[user[i].count_pick_job * sizeof(int)];
		for (int j = 0; j < user[i].count_pick_job; j++)
		{
			fread(&user[i].job[j], sizeof(int), 1, fp);
		}
		for (int n = 0; n < strlen(user[i].password); n++)
			user[i].password[n] = user[i].password[n] ^ user[i].key * 2;
	}
	fclose(fp);
	return user;
}

Packet packetty;

bool ProccesPacket(Packet packettype) {
	switch (packettype)
	{
	case P_ChatMessage:
	{
		int msg_size,pas_size;
		//std::cout << "Authorization!\n";
		//std::cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		fseek(stdin, 0, SEEK_END);
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		//std::cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		fseek(stdin, 0, SEEK_END);
		recv(Connection, (char*)&pas_size, sizeof(int), NULL);
		char* pas = new char[pas_size + 1];
		pas[pas_size] = '\0';
		recv(Connection, pas, pas_size, NULL);
		//std::cout << msg << std::endl;
		puts("+-----------------------+-----------------------+\n");
		printf("| Логин                 | Пароль                |\n");

		puts("+-----------------------+-----------------------+\n");
		printf("|%23s", msg);
		printf("|%23s|", pas);

		printf("\n");
		puts("+-----------------------+-----------------------+\n");
		delete[] msg;
		break;
	}
	case P_Test:
	{
		//std::cout << "Authorization!\n";
		std::cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		std::string msgl, pas;
		fseek(stdin, 0, SEEK_END);
		std::getline(std::cin, msgl);
		int msg_size = msgl.size();
		//int pas_size = pas.size();
		//Packet packettype = P_ChatMessage;
		//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msgl.c_str(), msg_size, NULL);
		std::cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		fseek(stdin, 0, SEEK_END);
		std::getline(std::cin, pas);
		//int msg_size = msgl.size();
		int pas_size = pas.size();

		send(Connection, (char*)&pas_size, sizeof(int), NULL);
		send(Connection, pas.c_str(), pas_size, NULL);
		break;
	}
	case P_Good:
	{
		packetty = packettype;
		std::cout << "It's good\n";
		break;
	}
	case P_Test1:
	{
		std::cout<<"\nПовторите попытку\n";
		packetty = packettype;
		system("pause");
		//Packet packettype = P_ChatMessage;
		//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		//Otprawka();
		break;
	}
	case P_Del:
	{
		int msg_size, pas_size;
		//std::cout << "Authorization!\n";
		//std::cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		fseek(stdin, 0, SEEK_END);
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		//std::cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		fseek(stdin, 0, SEEK_END);
		recv(Connection, (char*)&pas_size, sizeof(int), NULL);
		char* pas = new char[pas_size + 1];
		pas[pas_size] = '\0';
		recv(Connection, pas, pas_size, NULL);
		//std::cout << msg << std::endl;
		puts("+-----------------------+-----------------------+\n");
		printf("| Логин                 | Пароль                |\n");

		puts("+-----------------------+-----------------------+\n");
		printf("|%23s", msg);
		printf("|%23s|", pas);

		printf("\n");
		puts("+-----------------------+-----------------------+\n");
		delete[] msg;
		break;
	}
	default:
		std::cout << "Unrecognized packet: " << packettype << std::endl;
		break;
	}

	return true;
}

void ClientHandler() {
	Packet packettype;
	while (true) {
		EnterCriticalSection(&cs);
		recv(Connection, (char*)&packettype, sizeof(Packet), NULL);

		if (!ProccesPacket(packettype)) {
			break;
		}
		LeaveCriticalSection(&cs);
	}
	closesocket(Connection);
}

void Otprawka() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//Packet packettype = P_ChatMessage;
	fseek(stdin, 0, SEEK_END);
	//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
	system("pause");
//	getchar();
	fseek(stdin, 0, SEEK_END);
	std::cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
	fseek(stdin, 0, SEEK_END);
	std::string msgl, pas;
	
	std::getline(std::cin, msgl);
	int msg_size = msgl.size();
	//int pas_size = pas.size();
	//Packet packettype = P_ChatMessage;
	//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
	send(Connection, (char*)&msg_size, sizeof(int), NULL);
	send(Connection, msgl.c_str(), msg_size, NULL);
	std::cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
	fseek(stdin, 0, SEEK_END);
	std::getline(std::cin, pas);
	//int msg_size = msgl.size();
	int pas_size = pas.size();
	int sum = 0, n;
	char l = 0;
Tul:
	printf("Введите имя: ");
	fseek(stdin, 0, SEEK_END);
	char name[20];
	if (fgets(name, 20, stdin))
	{
		char* p;
		if (p = strchr(name, '\n'))
		{
			*p = 0;
		}
		else
		{
			scanf("%*[^\n]"); scanf("%*c");
		}
	}
	l = name[0];
	//cout << (int)l << endl;
	for (int i = 0, l = name[i]; i < 22;)
	{
		if (name[i] != '\0')
		{
			if ((int)l <= -65 || (int)l >= -1)
			{
				puts("Используйте только русские буквы");
				goto Tul;
			}
		}
		else
			break;
		l = name[++i];
		//printf("|%23s", ch);
	}
Tuk:
	printf("Введите фамилию: ");
	char surname[20];
	fseek(stdin, 0, SEEK_END);
	if (fgets(surname, 20, stdin))
	{
		char* p;
		if (p = strchr(surname, '\n'))
		{
			*p = 0;
		}
		else
		{
			scanf("%*[^\n]"); scanf("%*c");
		}
	}
	l = surname[0];
	//cout << (int)l << endl;
	for (int i = 0, l = surname[i]; i < 22;)
	{
		if (surname[i] != '\0')
		{
			if ((int)l <= -65 || (int)l >= -1)
			{
				puts("Используйте только русские буквы");
				goto Tuk;
			}
		}
		else
			break;
		l = surname[++i];
		//printf("|%23s", ch);
	}
	/*
Tum:
	printf("Введите отчество: ");
	fseek(stdin, 0, SEEK_END);
	if (fgets(new_emp.patronymic, 22, stdin))
	{
		char* p;
		if (p = strchr(new_emp.patronymic, '\n'))
		{
			*p = 0;
		}
		else
		{
			scanf("%*[^\n]"); scanf("%*c");
		}
	}
	l = new_emp.patronymic[0];
	//cout << (int)l << endl;
	for (int i = 0, l = new_emp.patronymic[i]; i < 22;)
	{
		if (new_emp.patronymic[i] != '\0')
		{
			if ((int)l <= -65 || (int)l >= -1)
			{
				puts("Используйте только русские буквы");
				goto Tum;
			}
		}
		else
			break;
		l = new_emp.patronymic[++i];
		//printf("|%23s", ch);
	}*/
Tuh:
	printf("Введите спeциальность: ");
	fseek(stdin, 0, SEEK_END);
	char position[20];
	if (fgets(position, 22, stdin))
	{
		char* p;
		if (p = strchr(position, '\n'))
		{
			*p = 0;
		}
		else
		{
			scanf("%*[^\n]"); scanf("%*c");
		}
	}
	l = position[0];
	//cout << (int)l << endl;
	for (int i = 0, l = position[i]; i < 22;)
	{
		if (position[i] != '\0')
		{
			if ((int)l <= -65 || (int)l >= -1)
			{
				puts("Используйте только русские буквы");
				goto Tuh;
			}
		}
		else
			break;
		l = position[++i];
		//printf("|%23s", ch);
	}
	printf("Зарплата сотрудника: ");
	double pay;
	pay = vvod<double>();
	send(Connection, (char*)&pas_size, sizeof(int), NULL);
	send(Connection, pas.c_str(), pas_size, NULL);
	send(Connection, name, sizeof(char) * 20, NULL);
	send(Connection, surname, sizeof(char) * 20, NULL);
	send(Connection, position, sizeof(char) * 20, NULL);
	send(Connection, (char*)&pay, sizeof(double), NULL);
	Sleep(50);
}
int pos = 0;
void Otprawka_avto() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//Packet packettype = P_ChatMessage;
	fseek(stdin, 0, SEEK_END);
	//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
	system("pause");
	//	getchar();
	fseek(stdin, 0, SEEK_END);
	std::cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
	fseek(stdin, 0, SEEK_END);
	std::string msgl, pas;

	std::getline(std::cin, msgl);
	int msg_size = msgl.size();
	//int pas_size = pas.size();
	//Packet packettype = P_ChatMessage;
	//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
	send(Connection, (char*)&msg_size, sizeof(int), NULL);
	send(Connection, msgl.c_str(), msg_size, NULL);
	std::cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
	fseek(stdin, 0, SEEK_END);
	std::getline(std::cin, pas);
	//int msg_size = msgl.size();
	int pas_size = pas.size();
	int sum = 0, n;
	char l = 0;

	send(Connection, (char*)&pas_size, sizeof(int), NULL);
	send(Connection, pas.c_str(), pas_size, NULL);
	Employee* user;
	int count = 1;
	user = new Employee[count * sizeof(Employee)];
	user = user->load_from_file(user, &count);
	for (int i = 0; i < count; i++)
	{
		if (strcmp(msgl.c_str(), user[i].login_r()) == 0)
		{
			pos = i;
		}
	}
	Sleep(50);
}

void Otprawka_del() {
	//Packet packettype = P_ChatMessage;
	fseek(stdin, 0, SEEK_END);
	//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
	system("pause");
	//	getchar();
	fseek(stdin, 0, SEEK_END);
	std::cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
	fseek(stdin, 0, SEEK_END);
	std::string msgl, pas;

	std::getline(std::cin, msgl);
	int msg_size = msgl.size();
	//int pas_size = pas.size();
	//Packet packettype = P_ChatMessage;
	//send(Connection, (char*)&packettype, sizeof(Packet), NULL);
	send(Connection, (char*)&msg_size, sizeof(int), NULL);
	send(Connection, msgl.c_str(), msg_size, NULL);
	//std::cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
	fseek(stdin, 0, SEEK_END);
	//std::getline(std::cin, pas);
	//int msg_size = msgl.size();
	//int pas_size = pas.size();

	//send(Connection, (char*)&pas_size, sizeof(int), NULL);
	//send(Connection, pas.c_str(), pas_size, NULL);
	Sleep(50);
}

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&cs);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//setlocale(LC_ALL, "Russian");
	menu();
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "ERROR!" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) 
	{
		std::cout << "Error: failed connect to server. \n";
		return 1;
	};
	std::cout << "Connection!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	
	std::string msgl,pas;
	Packet packettype = P_Good;
//	system("pause");
	
	while (1)
	{
		int error;
		error=HeadMenu();
		//	Otprawka();
		system("pause");
		//EnterCriticalSection(&cs);
		switch (error)
		{
		case 1:
		{
			if (packetty == P_Test1)
			{
				Sleep(50);
				HeadMenu();
				//Otprawka();
			}if (packetty == P_Good)
			{
				std::cout << "\nВы успешно вошли...\n";
				while (1)
				{

					if (AdminMenu() == 2)
						break;

					system("pause");
					//break;
				}
				break;
			}
		}
		case 2:
		{
			if (packetty == P_Test1)
			{
				Sleep(50);
				HeadMenu();
				//Otprawka();
			}if (packetty == P_Good)
			{
				std::cout << "\nВы успешно вошли...\n";
				while (1)
				{

					if (UserMenu() == 2)
						break;

					system("pause");
					//break;
				}
				break;
			}
		}
		case 3:
		{
			exit(0);
		}
		default:
			break;
		}
		
		//LeaveCriticalSection(&cs);
	}
	//while (true)
	//{
		//std::getline(std::cin, pas);
		int msg_size = msgl.size();
	//	int pas_size = pas.size();
	//	Packet packettype = P_ChatMessage;
		
	//	send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		//send(Connection, (char*)&msg_size, sizeof(int), NULL);
		//send(Connection, msgl.c_str(), msg_size, NULL);
		//send(Connection, (char*)&pas_size, sizeof(int), NULL);
		//send(Connection, pas.c_str(), pas_size, NULL);
		Sleep(10);
	//}

	//system("pause");

	return 0;
}

int HeadMenu() {
	bool exit_t = false;
	bool first = true;
	int choose = 0;
	char color[10] = "";
	const char* color_1 = "color f";
	const char* color_2 = "color a";
	const char* color_3 = "color b";
	const char* color_4 = "color c";
	const char* color_5 = "color d";
	int num = 0;

	while (exit_t == false) {
		if (first == false) {
			num = _getch();
			std::cout << int(num) << std::endl;
			switch (num) {
			case 72:
				if (choose > 0) {
					choose--;
				}
				else {
					choose = 2;
				}
				break;
			case 80:
				if (choose < 2) {
					choose++;
				}
				else {
					choose = 0;
				}
				break;
			case 27:
				exit_t = true;
				break;
			case 13:
				exit_t = true;
				//exit = false;
				break;
				//continue;
			}
			system("cls");
		}
		//Draw_logo();
		const char* colors[] = { "Войти как админ", "Войти как пользователь", "Выход " };
		for (int i = 0; i < sizeof(colors) / sizeof(*colors); ++i) {
			std::cout << i << ") " << colors[i] << "     " << (choose == i ? "<-\n" : "  \n");
		}
		first = false;
	}

	std::cout << "\n";

	switch (choose) {
	case 0:
	{
		//strcat_s(color, color_1);
		//system("color f");
		Packet packettype = P_ChatMessage_ad;
		send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		Sleep(50);
		Otprawka_avto();
		return 1;
		break;

	}
	case 1:
	{
		Packet packettype = P_ChatMessage;
		send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		Sleep(50);
		Otprawka_avto();
		return 2;
		break;
	}
	case 2:
	{
		system("pause");
		exit(0);
		return 3;
		break;
	} /*case 3:
		strcat_s(color, color_4);
		system("color c");
		break;
	case 4:
		strcat_s(color, color_5);
		system("color d");
		break;*/
	default:
		//strcat_s(color, color_1);
		//system("color f");
		break;
	}
}

int AdminMenu() {
	bool exit_t = false;
	bool first = true;
	int choose = 0;
	char color[10] = "";
	const char* color_1 = "color f";
	const char* color_2 = "color a";
	const char* color_3 = "color b";
	const char* color_4 = "color c";
	const char* color_5 = "color d";
	int num = 0;

	while (exit_t == false) {
		if (first == false) {
			num = _getch();
			std::cout << int(num) << std::endl;
			switch (num) {
			case 72:
				if (choose > 0) {
					choose--;
				}
				else {
					choose = 9;
				}
				break;
			case 80:
				if (choose < 10) {
					choose++;
				}
				else {
					choose = 0;
				}
				break;
			case 27:
				exit_t = true;
				break;
			case 13:
				exit_t = true;
				//exit = false;
				break;
				//continue;
			}
			system("cls");
		}
		//Draw_logo();
		const char* colors[] = { "Добавить пользователя", "Удалить пользователя", "Показать всех пользователей","Добавить работу","Удалить работу","Просмотреть весь список работ","Выбрать работы","Выдать премии","Сортировать по имени", "Сортировать по весу работы","Выход" };
		for (int i = 0; i < sizeof(colors) / sizeof(*colors); ++i) {
			std::cout << i << ") " << colors[i] << "     " << (choose == i ? "<-\n" : "  \n");
		}
		first = false;
	}

	std::cout << "\n";

	switch (choose) {
	case 0:
	{
		//strcat_s(color, color_1);
		//system("color f");
		int count = 0;
		Employee* user = new Employee[1 * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user->create_new_user(user, &count);
	//	Packet packettype = P_New;
	//	send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		Sleep(50);
	//	Otprawka();
		break;
	}
	case 1:
	{
		int count = 0;
		Employee* user = new Employee[1 * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user->delet_user(user, &count);
		//while (1)
		//{
			//Packet packettype = P_Del;
			//send(Connection, (char*)&packettype, sizeof(Packet), NULL);

			Sleep(50);
			//Otprawka_del();
			//if ();
		//}
		break;
	}
	case 2:
	{
		int count=0;
		Employee* user = new Employee[1 * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user->print_user(user, count);
		system("pause");
		//exit(0);
		break;
	} 
	case 3:
	{
		Job wo;
		wo.new_job();
		system("pause");
		//exit(0);
		break;
	}
	case 6:
	{
		Job wo;
		Employee* user;
		int count = 1;
		user = new Employee[count * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user->het_job(pos, user, &count);
	
		system("pause");
		//exit(0);
		break;
	}
	case 4:
	{
		Job wo;
		wo.del_job();
		system("pause");
		//exit(0);
		break;
	}
	case 5:
	{
		Job wo;
		wo.show_job();
		system("pause");
		//exit(0);
		break;//exit(0);
	}
	case 7:
	{
		Job wo;
		Employee* user;
		int count = 1;
		user = new Employee[count * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user->premia(user, &count);

		system("pause");
		//exit(0);
		break;
	}
	case 8:
	{
		Job wo;
		Employee* user;
		int count = 1;
		user = new Employee[count * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user = user->sort_name(user, &count);
		user->print_user(user, count);

		system("pause");
		//exit(0);
		break;
	}
	case 9:
	{
		Job wo;
		Employee* user;
		int count = 1;
		user = new Employee[count * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user = user->sort_gruppa(user, &count);
		user->print_user(user, count);

		system("pause");
		//exit(0);
		break;
	}
	case 10:
	{
		return 2;
		break;
	}	/*
	case 4:
		strcat_s(color, color_5);
		system("color d");
		break;*/
	default:
		//strcat_s(color, color_1);
		//system("color f");
		break;
	}
}

int UserMenu() {
	bool exit_t = false;
	bool first = true;
	int choose = 0;
	char color[10] = "";
	const char* color_1 = "color f";
	const char* color_2 = "color a";
	const char* color_3 = "color b";
	const char* color_4 = "color c";
	const char* color_5 = "color d";
	int num = 0;

	while (exit_t == false) {
		if (first == false) {
			num = _getch();
			std::cout << int(num) << std::endl;
			switch (num) {
			case 72:
				if (choose > 0) {
					choose--;
				}
				else {
					choose = 5;
				}
				break;
			case 80:
				if (choose < 5) {
					choose++;
				}
				else {
					choose = 0;
				}
				break;
			case 27:
				exit_t = true;
				break;
			case 13:
				exit_t = true;
				//exit = false;
				break;
				//continue;
			}
			system("cls");
		}
		//Draw_logo();
		const char* colors[] = { "Просмотреть весь список работ","Выбрать работы","Посмотреть информацию о банке", "Посмотреть информацию о директоре" ,"Выход" };
		for (int i = 0; i < sizeof(colors) / sizeof(*colors); ++i) {
			std::cout << i << ") " << colors[i] << "     " << (choose == i ? "<-\n" : "  \n");
		}
		first = false;
	}

	std::cout << "\n";

	switch (choose) {
	case 0:
	{
		Job wo;
		wo.show_job();
		system("pause");
		//exit(0);
		break;
	}
	case 1:
	{
		Job wo;
		Employee* user;
		int count = 1;
		user = new Employee[count * sizeof(Employee)];
		user = user->load_from_file(user, &count);
		user->het_job(pos, user, &count);

		system("pause");
		//exit(0);
		break;
	}
	case 2:
	{
		Employee* user;
		int count = 1;
		user = new Employee[count * sizeof(Employee)];
		user->show_info_bank();
		break;
	}
	case 3:
	{
		Employee* user;
		int count = 1;
		user = new Employee[count * sizeof(Employee)];
		user->show_info_director();
		break;
	}
	case 4:
	{
		return 2;
		break;
	}
	
	default:
		//strcat_s(color, color_1);
		//system("color f");
		break;
	}
}

void menu() {
	bool exit = false;
	bool first = true;
	int choose = 0;
	char color[10] = "";
	const char* color_1 = "color f";
	const char* color_2 = "color a";
	const char* color_3 = "color b";
	const char* color_4 = "color c";
	const char* color_5 = "color d";
	int num = 0;

	while (exit == false) {
		if (first == false) {
			num = _getch();
			std::cout << int(num) << std::endl;
			switch (num) {
			case 72:
				if (choose > 0) {
					choose--;
				}
				else {
					choose = 4;
				}
				break;
			case 80:
				if (choose < 4) {
					choose++;
				}
				else {
					choose = 0;
				}
				break;
			case 27:
				exit = true;
				break;
			case 13:
				exit = true;
				//exit = false;
				break;
				//continue;
			}
			system("cls");
		}
		//Draw_logo();
		const char* colors[] = { "Белый", "Зелёный", "Синий", "Красный  ", "Розовый " };
		for (int i = 0; i < sizeof(colors) / sizeof(*colors); ++i) {
			std::cout << i << ") " << colors[i] << "     " << (choose == i ? "<-\n" : "  \n");
		}
		first = false;
	}

	std::cout << "\n";

	switch (choose) {
	case 0:
		strcat_s(color, color_1);
		system("color f");
		break;
	case 1:
		strcat_s(color, color_2);
		system("color a");
		break;
	case 2:
		strcat_s(color, color_3);
		system("color b");
		break;
	case 3:
		strcat_s(color, color_4);
		system("color c");
		break;
	case 4:
		strcat_s(color, color_5);
		system("color d");
		break;
	default:
		strcat_s(color, color_1);
		system("color f");
		break;
	}
}

vector<string> Job::load_job(vector<string> work, int* count_job)
{
	/*FILE* fp;
	if ((fp = fopen("outData.txt", "rb")) == NULL)
	{
		cout << "\nОшибка чтения файла.\n";
		return;
	}
	fread(count_job, sizeof(int), 1, fp);
	cout << *count_job << endl;
	vector<string> vec2(*count_job);
	for (int i = 0; i < *count_job; i++)
	{

		string h;
		int size_wor = sizeof(vec2[i]);
		fread(&vec2[i], size_wor, 1, fp);
		cout << vec2[i] << ' ';
	}
	wor.assign(vec2.begin(), vec2.end());
	fclose(fp);*/

	std::fstream outF("C:\\Users\\Asus\\source\\repos\\Kursa4_client\\Kursa4_client\\outData.txt");
	//outF.read((char*)&count_job, sizeof(int));
	//std::vector<str> v;
	std::copy(std::istream_iterator<string>(outF), std::istream_iterator<string>(), std::back_inserter(work));
//	cout << work.size() << endl;
	*count_job = stoi(work[0]);
	//cout << *count_job << endl;
	/*
	for (int i = 0; i < work.size(); i++)
	{
		string h;
		//int size_wor = sizeof(vec2[i]);
		//fread(&vec2[i], size_wor, 1, fp);
		cout << work[i] << ' ';
	}
	*/
	//*count_job = stoi(wor[0]) + 1;
//	wor[0] = to_string(*count_job);
	vector<string> new_wo(*count_job * 2 + 2);
	for (int i = 0, j = 0, g = 0, exit = 0; i < work.size(); i++)
	{
		if (work[i] == "|")
		{

			j++;
			new_wo[j] = work[i];
			//i++;
			j++;
			exit++;
			g = 0;
			if (exit == *count_job + 1)
			{
				break;
			}
		}
		else if (i == 0)
		{
			//new_wo[0] = to_string(count_job);
			new_wo.insert(new_wo.begin(), to_string(*count_job));
			j++;
		}
		else
		{
			//new_wo[j].append(wor[i]);
			g++;
			if (g >= 2)
			{
				new_wo[j].append(" ");
			}
			new_wo[j].append(work[i]);

		}
	}
	return new_wo;
	//cout << *count_job << endl;
}


void Job::save_job(vector<string> work, int count_job) {
	/*int i = 3;
	string t = to_string(i);
	wor.insert(wor.end(), t);
	wor.insert(wor.end(), "Use sayls");
	wor.insert(wor.end(), "|");
//	i = 2;
	//t = to_string(i);
	//wor.insert(wor.end(), t);
	wor.insert(wor.end(), "Del sayls");
	wor.insert(wor.end(), "|");
	//i = 3;
	//t = to_string(i);
	//wor.insert(wor.end(), t);
	wor.insert(wor.end(), "read");
	wor.insert(wor.end(), "|");
	count_job = wor.size();*/


	/*FILE* fp;
	if ((fp = fopen("outData.txt", "wb")) == NULL)
	{
		cout << "Файл не открылся.\n";
		return;
	}
	fwrite(&count_job, sizeof(int), 1, fp);
	for (int i = 0; i < wor.size(); i++)
	{
		string h;
		int size_wor = sizeof(wor[i]);
		fwrite(wor[i].c_str(), size_wor, 1, fp);

	}*/
	ofstream outF("C:\\Users\\Asus\\source\\repos\\Kursa4_client\\Kursa4_client\\outData.txt");
	//outF.write((char*)&count_job, sizeof(int));
	//fwrite(count_job, sizeof(int), 1, outF);
	copy(work.begin(), work.end(), ostream_iterator<string>(outF, "\n"));
	/*for(int i = 0; i < temp.size(); i++)
		outF << temp[i];*/
	outF.close();
}

void Job::new_job() {
	int count_job = 0;
	wor = load_job(wor, &count_job);

	string new_e;
	count_job = stoi(wor[0]) + 1;
	wor[0] = to_string(count_job);
	vector<string> new_wo(count_job * 2);
	for (int i = 0, j = 0, g = 0; i < wor.size(); i++)
	{
		if (wor[i] == "|")
		{

			j++;
			new_wo[j] = wor[i];
			//i++;
			j++;
			g = 0;
		}
		else if (i == 0)
		{
			//new_wo[0] = to_string(count_job);
			new_wo.insert(new_wo.begin(), to_string(count_job));
			j++;
		}
		else
		{
			g++;
			if (g >= 2)
			{
				new_wo[j].append(" ");
			}
			new_wo[j].append(wor[i]);

			//new_wo[j].append(" ");
		}
	}

	string job;
	int c = 1;
	while (c != 0)
	{
		cout << "Введите новую работу:\n";
		getline(std::cin, job);
		for (int i = 0; i < new_wo.size(); i++)
		{
			if (job == new_wo[i])
			{
				cout << "Такая работа уже есть, повторите попытку\n";
				break;
			}
			else if (i == (new_wo.size() - 1))
			{
				new_wo.insert(new_wo.end(), job);
				new_wo.insert(new_wo.end(), "|");
				save_job(new_wo, count_job);
				c = 0;
				break;
			}
		}

	}
	//wor.insert(wor.end(), job);

	//wor.insert(wor.end(), "Use sayls");
	//wor.insert(wor.end(), "Del sayls");
	//wor.insert(wor.end(), "read");

}

void Job::del_job()
{
	int count_job = 0;
	wor = load_job(wor, &count_job);

	string new_e;
	count_job = stoi(wor[0]);
	wor[0] = to_string(count_job);
	vector<string> new_wo(count_job * 2 + 2);
	for (int i = 0, j = 0, g = 0; i < wor.size(); i++)
	{
		if (wor[i] == "|")
		{

			j++;
			new_wo[j] = wor[i];
			//i++;
			j++;
			g = 0;
		}
		else if (i == 0)
		{
			//new_wo[0] = to_string(count_job);
			new_wo.insert(new_wo.begin(), to_string(count_job));
			j++;
		}
		else
		{
			//new_wo[j].append(wor[i]);
			g++;
			if (g >= 2)
			{
				new_wo[j].append(" ");
			}
			new_wo[j].append(wor[i]);

		}
	}
	//int c;
	string job;
	int c = 1;
	while (c != 0)
	{
		cout << "\nВведите название работы:  ";
		getline(std::cin, job);
		for (int i = 0; i < new_wo.size(); i++)
		{
			if (job == new_wo[i])
			{
				count_job = stoi(new_wo[0]) - 1;
				new_wo[0] = to_string(count_job);
				auto iter = new_wo.cbegin();
				new_wo.erase(iter + i, iter + i + 2);
				//new_wo.erase(iter + i);
				cout << "Успешно удалено\n";
				c = 0;
				save_job(new_wo, count_job);
				break;
			}
			else if (i == (new_wo.size() - 1))
			{
				cout << "Нет такой работы\n";
				c = 0;
				break;
			}
		}

	}

}

void Job::show_job()
{
	int count_job = 0;
	vector<string> work = load_job(work, &count_job);
	//vector<string> new_wo;
	string new_e;
	count_job = stoi(work[0]);
	work[0] = to_string(count_job);
	vector<string> new_wo(count_job * 2 + 2);
	for (int i = 0, j = 0, g = 0, exit = 0; i < work.size(); i++)
	{
		if (work[i] == "|")
		{

			j++;
			new_wo[j] = work[i];
			//i++;
			j++;
			exit++;
			g = 0;
			if (exit == count_job + 1)
			{
				break;
			}
		}
		else if (i == 0)
		{
			//new_wo[0] = to_string(count_job);
			new_wo.insert(new_wo.begin(), to_string(count_job));
			j++;
		}
		else
		{
			//new_wo[j].append(wor[i]);
			g++;
			if (g >= 2)
			{
				new_wo[j].append(" ");
			}
			new_wo[j].append(work[i]);

		}
	}

	copy(new_wo.begin(), new_wo.end(), ostream_iterator<string>(cout, " "));


	/*
	FILE* fp;
	if ((fp = fopen("work", "rb")) == NULL)
	{
		cout << "\nОшибка чтения файла.\n";
		return ;
	}
	for (int i = 0; i < wor.size(); i++)
	{
		string h;
		int size_wor = sizeof(wor[i]);
		fread(&wor[i], size_wor, 1, fp);
		cout << wor[i] << ' ';
	}*/

	//fread(count, sizeof(int), 1, fp);
}

void Employee::het_job(int pos, Employee* user, int* count)
{

	int count_job = 0;
	//Employee *user;
	user = user->load_from_file(user, count);
	user[pos].count_pick_job = 0;
	//*count = 0;
//	*count += 1;
	Job work;
	vector<string> worrr;
	worrr = work.load_job(worrr, &count_job);
//	work.show_job();
	cout << endl;
	for (int i = 3, j = 1; i < worrr.size(); j++)
	{
		cout << j << ' ' << worrr[i] << endl;
		i = i + 2;
	}
	cout << "Выберите работые, которые будете выполнять(вводите через пробел): ";
	int jober;
	vector<int> worke;
	fseek(stdin, 0, SEEK_END);
	while (cin.peek() != '\n')
	{
		cin >> jober;
		if (jober <= stoi(worrr[0]))
		{

			worke.insert(worke.end(), jober);
		}

	}
	sort(worke.begin(), worke.end());
	user[pos].count_pick_job = worke.size();
	//while (!(cin >> jober) || (cin.peek() != '\n'))
	//{
		//cin.clear();
	//	worke.insert(worke.end(), jober);
		//while (cin.get() != '\n');
		//cout << "Error!" << endl << "Попробуйте ещё раз" << endl;
	//}
	user[pos].job = new int[worke.size()];
	for (int i = 0; i < worke.size(); i++)
	{
		user[pos].job[i] = worke[i];
		cout << user[pos].job[i] << ' ';
	}
	cout << endl;
	user->save_on_file(user, *count);
	user->save_on_file_s(user, *count);
	system("pause");
}

void Employee::premia(Employee* user, int* count)
{
	user = user->load_from_file(user, count);
	Employee* sor = new Employee[*count * sizeof(Employee)];

	StreamTable st(std::cout);
	st.AddCol(20);
	st.AddCol(20);
	int max = 0;
	double sum = 0;

	for (int i = 0; i < *count; i++)
	{
		sum += user[i].count_pick_job;
	}
	for (int i = 0; i < *count; i++)
	{
		user[i].koef = user[i].count_pick_job / sum;
		//sum += user[i].count_pick_job;
	}
	memcpy(sor, user, *count * sizeof(Employee));
	vector<string> wor;
	Job boj;
	int count_job = 0;
	wor = boj.load_job(wor, &count_job);
	max = stoi(wor[0]);
	for (int i = 0; i < max; i++)
	{
		st.AddCol(9);
	}
	st.MakeBorderExt(true);
	st.SetDelimRow(true, '-');//st.SetDelimRow(false);//без символов-разделителей строк
	st.SetDelimCol(true, '|');//st.SetDelimCol(false);//без символов-разделителей строк
	st << "Name" << "SurName";
	for (int i = 0; i < max; i++)
	{
		string e{ "Работа " };
		string q = e + to_string(i + 1);
		st << q;
	}
	//enum { nr = 10 };
	for (int i = 0; i < *count; i++) {
		st << user[i].name << user[i].surname;
		for (int j = 0, h = 0; j < max; j++)
		{
			int error = 0;
			if (h < user[i].count_pick_job)
			{
				error = 0;
				for (int w = 1; w <= j + 1; w++)
				{
					if (user[i].job[h] == w)
					{
						error = 1;
						w = max + 1;
						h++;
					}
					else
						error = 0;

					//st << "-";
				}
			}
			if (error == 1)
			{
				st << "1";
			}
			else
				st << "-";
			/*
			if(user[i].job[j]==j+1)
			{
				st << "1";
			}
			else
				st << "-";*/
		}
	}

	sor = sor->sort_gruppa(sor, count);
	if (*count > 3)
	{
		for (int i = 0; i < *count; i++)
		{
			if (sor[0].id == user[i].id)
			{
				user[i].pay += 50;
				sor[0].pay += 50;
			}
		}
		for (int i = 0; i < *count; i++)
		{
			if (sor[1].id == user[i].id)
			{
				user[i].pay += 50;
				sor[1].pay += 50;
			}
		}
		for (int i = 0; i < *count; i++)
		{
			if (sor[2].id == user[i].id)
			{
				user[i].pay += 50;
				sor[2].pay += 50;
			}
		}
	}
	else
	{
		for (int i = 0; i < *count; i++)
		{
			if (sor[0].id == user[i].id)
			{
				user[i].pay += 50;
				sor[0].pay += 50;
			}

		}
	}
	sor->print_user(sor, *count);
	user->save_on_file(user, *count);
	user->save_on_file_s(user, *count);
}

void Employee::save_on_file(Employee* user, int count)
{
	int i, sum = 0, n;
	FILE* fp;
	if ((fp = fopen("C:\\Users\\Asus\\source\\repos\\Kursa4_server\\Kursa4_server\\user", "wb")) == NULL)
	{
		cout << "Файл не открылся.\n";
		return;
	}
	int max = 0;
	fwrite(&count, sizeof(int), 1, fp);
	for (i = 0; i < count; i++)
	{

		sum = 0;
		for (n = 0; n < strlen(user[i].password); n++) //Ключ вычисляется по формуле (сума битов всех симловов строки)/(количество символов)
			sum += user[i].password[n];
		user[i].key = sum / n;

		for (n = 0; n < strlen(user[i].password); n++)
			user[i].password[n] = user[i].password[n] ^ user[i].key * 2;
	}

	for (int i = 0; i < count; i++)
	{
		fwrite(&user[i].id, sizeof(int), 1, fp);
		fwrite(user[i].login, sizeof(char) * 20, 1, fp);
		fwrite(user[i].password, sizeof(char) * 20, 1, fp);
		fwrite(&user[i].key, sizeof(int), 1, fp);
		fwrite(user[i].name, sizeof(char) * 20, 1, fp);
		fwrite(user[i].surname, sizeof(char) * 20, 1, fp);
		fwrite(user[i].position, sizeof(char) * 20, 1, fp);
		fwrite(&user[i].pay, sizeof(double), 1, fp);
		fwrite(&user[i].count_pick_job, sizeof(int), 1, fp);
		for (int j = 0; j < user[i].count_pick_job; j++)
		{
			fwrite(&user[i].job[j], sizeof(int), 1, fp);
		}
	}
	fclose(fp);
	ofstream fout("C:\\Users\\Asus\\source\\repos\\Kursa4_server\\Kursa4_server\\users.txt", ios_base::out | ios_base::trunc);
	if (!fout.is_open()) // если файл небыл открыт
	{
		cout << "Файл не может быть открыт или создан\n"; // напечатать соответствующее сообщение
		return; // выполнить выход из программы
	}
	StreamTable st(fout);
	st.AddCol(5);
	st.AddCol(15);
	st.AddCol(10);
	st.AddCol(10);
	st.AddCol(20);
	st.AddCol(20);
	st.AddCol(20);
	st.AddCol(10);
	st.AddCol(15);

	//st.AddCol(20);


	//разкомментировать, если столбцы имеют одинаковую толщину
	//st.Clear();
	//st.SetCols(4, 10);

	st.SetVisible(4, false);//столбцы можно скрывать

	st.MakeBorderExt(true);
	st.SetDelimRow(true, '-');//st.SetDelimRow(false);//без символов-разделителей строк
	st.SetDelimCol(true, '|');//st.SetDelimCol(false);//без символов-разделителей строк

	//заголовок и значения выводятся одинаково
	st << "ID" << "Login" << "Password" << "Key" << "Name" << "Surname" << "Position" << "Pay" << "Count_pick_job";
	//enum { nr = 10 };
	for (int i = 0; i < count; i++) {
		st << user[i].id << user[i].login << user[i].password << user[i].key << user[i].name << user[i].surname << user[i].position << user[i].pay << user[i].count_pick_job;
	}
	fout.close();
}

void Employee::save_on_file_s(Employee* user, int count)
{
	int i, sum = 0, n;
	FILE* fp;
	if ((fp = fopen("C:\\Users\\Asus\\source\\repos\\Kursa4_client\\Kursa4_client\\user", "wb")) == NULL)
	{
		cout << "Файл не открылся.\n";
		return;
	}
	int max = 0;
	fwrite(&count, sizeof(int), 1, fp);
	for (i = 0; i < count; i++)
	{

		sum = 0;
		for (n = 0; n < strlen(user[i].password); n++) //Ключ вычисляется по формуле (сума битов всех симловов строки)/(количество символов)
			sum += user[i].password[n];
		user[i].key = sum / n;

		for (n = 0; n < strlen(user[i].password); n++)
			user[i].password[n] = user[i].password[n] ^ user[i].key * 2;
	}

	for (int i = 0; i < count; i++)
	{
		fwrite(&user[i].id, sizeof(int), 1, fp);
		fwrite(user[i].login, sizeof(char) * 20, 1, fp);
		fwrite(user[i].password, sizeof(char) * 20, 1, fp);
		fwrite(&user[i].key, sizeof(int), 1, fp);
		fwrite(user[i].name, sizeof(char) * 20, 1, fp);
		fwrite(user[i].surname, sizeof(char) * 20, 1, fp);
		fwrite(user[i].position, sizeof(char) * 20, 1, fp);
		fwrite(&user[i].pay, sizeof(double), 1, fp);
		fwrite(&user[i].count_pick_job, sizeof(int), 1, fp);
		for (int j = 0; j < user[i].count_pick_job; j++)
		{
			fwrite(&user[i].job[j], sizeof(int), 1, fp);
		}
	}
	fclose(fp);
	ofstream fout("C:\\Users\\Asus\\source\\repos\\Kursa4_client\\Kursa4_client\\users.txt", ios_base::out | ios_base::trunc);
	if (!fout.is_open()) // если файл небыл открыт
	{
		cout << "Файл не может быть открыт или создан\n"; // напечатать соответствующее сообщение
		return; // выполнить выход из программы
	}
	StreamTable st(fout);
	st.AddCol(5);
	st.AddCol(15);
	st.AddCol(10);
	st.AddCol(10);
	st.AddCol(20);
	st.AddCol(20);
	st.AddCol(20);
	st.AddCol(10);
	st.AddCol(15);

	//st.AddCol(20);


	//разкомментировать, если столбцы имеют одинаковую толщину
	//st.Clear();
	//st.SetCols(4, 10);

	st.SetVisible(4, false);//столбцы можно скрывать

	st.MakeBorderExt(true);
	st.SetDelimRow(true, '-');//st.SetDelimRow(false);//без символов-разделителей строк
	st.SetDelimCol(true, '|');//st.SetDelimCol(false);//без символов-разделителей строк

	//заголовок и значения выводятся одинаково
	st << "ID" << "Login" << "Password" << "Key" << "Name" << "Surname" << "Position" << "Pay" << "Count_pick_job";
	//enum { nr = 10 };
	for (int i = 0; i < count; i++) {
		st << user[i].id << user[i].login << user[i].password << user[i].key << user[i].name << user[i].surname << user[i].position << user[i].pay << user[i].count_pick_job;
	}
	fout.close();
}

Employee* Employee::create_new_user(Employee* user, int* count)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	user = user->load_from_file(user, count);
	//*count = 1;
	*count += 1;

	Employee* new_user = new Employee[*count * sizeof(Employee)];

	memcpy(new_user, user, (*count - 1) * sizeof(Employee));
	new_user[*count - 1].id = *count - 1;
	//struct user new_emp;
	new_user[*count - 1].count_pick_job = 0;
	while (1)
	{
		static int k = 0;
		cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		fseek(stdin, 0, SEEK_END);
		if (fgets(new_user[*count - 1].login, 20, stdin))
		{
			char* p;
			if (p = strchr(new_user[*count - 1].login, '\n'))
			{
				*p = 0;
			}
			else
			{
				scanf("%*[^\n]"); scanf("%*c");
			}
		}
		for (int j = 0, k = 0; j < *count; j++)
		{
			if (strcmp(new_user[*count - 1].login, user[j].login) == 0)
			{
				k++;
				cout << "Пользователь с данным логином существует" << endl;
				k++;
				return user;
			}
		}
		if (k == 0)
		{
			cout << "Введите Пароль (длинна пароля не должна превышать 20 символа, иначе запомнятся только первые 20 ваших симвов) :";
			if (fgets(new_user[*count - 1].password, 20, stdin))
			{
				char* p;
				if (p = strchr(new_user[*count - 1].password, '\n'))
				{
					*p = 0;
				}
				else
				{
					scanf("%*[^\n]"); scanf("%*c");
				}
			}
			int sum = 0, n;
			char l = 0;
		Tul:
			printf("Введите имя: ");
			fseek(stdin, 0, SEEK_END);

			if (fgets(new_user[*count - 1].name, 20, stdin))
			{
				char* p;
				if (p = strchr(new_user[*count - 1].name, '\n'))
				{
					*p = 0;
				}
				else
				{
					scanf("%*[^\n]"); scanf("%*c");
				}
			}
			l = new_user[*count - 1].name[0];
			//cout << (int)l << endl;
			for (int i = 0, l = new_user[*count - 1].name[i]; i < 22;)
			{
				if (new_user[*count - 1].name[i] != '\0')
				{
					if ((int)l <= -65 || (int)l >= -1)
					{
						puts("Используйте только русские буквы");
						goto Tul;
					}
				}
				else
					break;
				l = new_user[*count - 1].name[++i];
				//printf("|%23s", ch);
			}
		Tuk:
			printf("Введите фамилию: ");
			fseek(stdin, 0, SEEK_END);
			if (fgets(new_user[*count - 1].surname, 20, stdin))
			{
				char* p;
				if (p = strchr(new_user[*count - 1].surname, '\n'))
				{
					*p = 0;
				}
				else
				{
					scanf("%*[^\n]"); scanf("%*c");
				}
			}
			l = new_user[*count - 1].surname[0];
			//cout << (int)l << endl;
			for (int i = 0, l = new_user[*count - 1].surname[i]; i < 22;)
			{
				if (new_user[*count - 1].surname[i] != '\0')
				{
					if ((int)l <= -65 || (int)l >= -1)
					{
						puts("Используйте только русские буквы");
						goto Tuk;
					}
				}
				else
					break;
				l = new_user[*count - 1].surname[++i];
				//printf("|%23s", ch);
			}
			/*
		Tum:
			printf("Введите отчество: ");
			fseek(stdin, 0, SEEK_END);
			if (fgets(new_emp.patronymic, 22, stdin))
			{
				char* p;
				if (p = strchr(new_emp.patronymic, '\n'))
				{
					*p = 0;
				}
				else
				{
					scanf("%*[^\n]"); scanf("%*c");
				}
			}
			l = new_emp.patronymic[0];
			//cout << (int)l << endl;
			for (int i = 0, l = new_emp.patronymic[i]; i < 22;)
			{
				if (new_emp.patronymic[i] != '\0')
				{
					if ((int)l <= -65 || (int)l >= -1)
					{
						puts("Используйте только русские буквы");
						goto Tum;
					}
				}
				else
					break;
				l = new_emp.patronymic[++i];
				//printf("|%23s", ch);
			}*/
			printf("Введите спeциальность: ");
			fseek(stdin, 0, SEEK_END);
		Tuh:
			if (fgets(new_user[*count - 1].position, 22, stdin))
			{
				char* p;
				if (p = strchr(new_user[*count - 1].position, '\n'))
				{
					*p = 0;
				}
				else
				{
					scanf("%*[^\n]"); scanf("%*c");
				}
			}
			l = new_user[*count - 1].position[0];
			//cout << (int)l << endl;
			for (int i = 0, l = new_user[*count - 1].position[i]; i < 22;)
			{
				if (new_user[*count - 1].position[i] != '\0')
				{
					if ((int)l <= -65 || (int)l >= -1)
					{
						puts("Используйте только русские буквы");
						goto Tuh;
					}
				}
				else
					break;
				l = new_user[*count - 1].position[++i];
				//printf("|%23s", ch);
			}
			printf("Зарплата сотрудника: ");
			new_user[*count - 1].pay = vvod<double>();
			//for (n = 0; n < strlen(new_emp.password); n++) //Ключ вычисляется по формуле (сума битов всех симловов строки)/(количество символов)
			//	sum += new_emp.password[n];
			//new_emp.key = sum / n;
			//for (n = 0; n < strlen(new_emp.password); n++)
			//	new_emp.password[n] = new_emp.password[n] ^ new_emp.key * 2;
			//B = (struct user*) malloc(*count * sizeof(struct user));
			user = new Employee[*count * sizeof(Employee)];
			//memcpy(&tm[*count - 1], &new_emp, sizeof(struct user));
			memcpy(user, new_user, *count * sizeof(Employee));
			printf("\nЗапись добавлена!\n\n");
			free(new_user);
			user->save_on_file(user, *count);
			user->save_on_file_s(user, *count);
			break;

		}
		else
		{
			break;
		}
	}
	return user;
}

void Employee::delet_user(Employee* user, int* count)
{
	user = user->load_from_file(user, count);
	//setlocale(LC_ALL, "Russian");
	Employee delet;
	int j, cn = 0, cull = 0;
	static int num;
	char del[20], del_name[20];
	while (1)
	{

		puts("Введите логин пользователя для удаления.");
		//	getchar();
		fseek(stdin, 0, SEEK_END);
		if (fgets(del, 20, stdin))
		{
			char* p;
			if (p = strchr(del, '\n'))
			{
				*p = 0;
			}
			else
			{
				scanf("%*[^\n]"); scanf("%*c");
			}
		}
		for (j = 0, cull = 0, cn = 0; j < *count; j++)
		{
			if (strcmp(del, user[j].login) == 0)
			{
				system("cls");
				puts("+-----------------------+-----------------------+\n");
				printf("| Логин                 | Пароль                |\n");

				puts("+-----------------------+-----------------------+\n");
				printf("|%23s", user[j].login);
				printf("|%23s|", user[j].password);

				printf("\n");
				puts("+-----------------------+-----------------------+\n");


				setlocale(LC_ALL, "Russian");
				int ch;
				puts("");
				puts("Этого пользователя вы хотите удалить?");
				puts("1. Да");
				puts("2. Нет");
				do {
					printf("\nВаш выбор:");
					fflush(stdin);
					ch = vvod<int>();
					printf("\n");
				} while (ch < 0 || ch > 2);
				if (user[j].id == 0)
				{
					cout << "Невозможно удалить администратора.\n";
				}
				if (ch == 1)
				{
					cn++;
					num = j;

					break;
				}
				else
					continue;
			}
			else
			{
				cull++;
				continue;
			}

		}
		if (cull != 0 && cn == 0)
		{
			puts("Нет такого пользователя");
			return;
		}


		*count = *count - 1;
		for (int i = num; i < *count; i++)
		{

			memcpy(&user[i], &user[i + 1], sizeof(Employee));
		}
		user->save_on_file(user, *count);
		user->save_on_file_s(user, *count);
		/*	FILE* f;
			int i;
			if ((f = fopen("user", "wb")) == NULL)
			{
				printf("\nОшибка чтения файла!\n");
				return;
			}
			fwrite(&count, sizeof(int), 1, f);
			for (i = 0; i < *count; ++i)
			{
				size_t t = fwrite(&user[i], sizeof(Employee), 1, f);
				std::cout << "TYPE: " << t << std::endl;
			}
			fclose(f);*/
		return;

	}
}
/*
int vvod()
{
	int a;
	while (!(cin >> a) || (cin.peek() != '\n'))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Error!" << endl << "Попробуйте ещё раз" << endl;
	}
	return a;
}

double vvod_pay()
{
	double a;
	while (!(cin >> a) || (cin.peek() != '\n'))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Error!" << endl << "Попробуйте ещё раз" << endl;
	}
	if (a <= 0) {
		throw InvalidPrice();
	}
	return a;
}
*/
