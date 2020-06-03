#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include "StreamTable.h"
#include <sstream>
#include <fstream> // работа с файлами
#include <iomanip>
#include <windows.h>
int vvod();

using namespace std;

enum Packet {
	P_ChatMessage,
	P_Test,
	P_Test1,
	P_Good,
	P_New,
	P_Del,
	P_ChatMessage_ad
};

class Worker
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
private:
	char password[20], login[20];
	int id, key;

public:
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

	void save_on_file_s(Employee* user, int count);
	Packet create_new_user(Employee* user, int* count, char* log, char* pas, char name[20], char surname[20], char position[20], double pay);
	Employee* load_from_file(Employee* user, int* count);
	void save_on_file(Employee* user, int count);
	Packet authorization_ad(char* log, char* pas);
	Packet authorization(char *log,char *pas);
	void print_user(Employee* user, int count);
//	void delet_user(Employee* user, int* count);
	Packet delet_user(Employee* user, int* count, char* log);
};

//static Employee user;

void TestStreamTable1()
{
	//ofstream fout("data_types.txt", ios_base::out | ios_base::trunc);
	//if (!fout.is_open()) // если файл небыл открыт
	//{
	//	cout << "Файл не может быть открыт или создан\n"; // напечатать соответствующее сообщение
	//	return ; // выполнить выход из программы
	//}
	StreamTable st(std::cout);
	st.AddCol(5);
	st.AddCol(15);
	st.AddCol(10);
	st.AddCol(10);


	//разкомментировать, если столбцы имеют одинаковую толщину
	//st.Clear();
	//st.SetCols(4, 10);

	st.SetVisible(4, false);//столбцы можно скрывать

	st.MakeBorderExt(true);
	st.SetDelimRow(true, '-');//st.SetDelimRow(false);//без символов-разделителей строк
	st.SetDelimCol(true, '|');//st.SetDelimCol(false);//без символов-разделителей строк

	//заголовок и значения выводятся одинаково
	st << "#" << "Property" << "Value" << "Unit";
	//enum { nr = 10 };
	for (int i = 0; i < 10; i++) {
		st << i + 1 << "Prop" << i << "Unit";
	}
	//fout.close();
}
/*
int main()
{
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");
	COORD coord;
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, &coord);
	HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleHandle, FOREGROUND_GREEN);

	int count = 1;
	Employee* user;
	user = new Employee[count * sizeof(Employee)];

	//user->save_on_file(user, count);
//	user->load_from_file(user, &count);
	//user->authorization();
	//user->create_new_user(user, &count);
	user->print_user(user, count);
	//user->delet_user(user, &count);

	TestStreamTable1();
	system("pause");


	return 0;
}
*/
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

Packet Employee::authorization_ad(char* log, char* pas)
{
	int count = 0;
	Employee* user = new Employee[1 * sizeof(Employee)];
	user = user->load_from_file(user, &count);
	Employee* worker = new Employee[1 * sizeof(Employee)];
	static int j, e = 0, cull = 0;
	e = 0;
	
		system("cls");
		e++;
		cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		//getchar();
		//printf("\n");

		/*if (fgets(worker->login_r(), 20, stdin))
		{
			char* p;
			if (p = strchr(worker->login_r(), '\n'))
			{
				*p = 0;
			}
			else
			{
				scanf("%*[^\n]"); scanf("%*c");
			}
		}*/

		
			//if (strcmp(worker->login_r(), user[j].login_r()) == 0)
		if (strcmp(log, user[0].login_r()) == 0)
		{
			cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
			/*
			if (fgets(worker->password_r(), 20, stdin))
			{
				char* p;
				if (p = strchr(worker->password_r(), '\n'))
				{
					*p = 0;
				}
				else
				{
					scanf("%*[^\n]"); scanf("%*c");
				}
			}*/
			if (strcmp(pas, user[0].password_r()) == 0)
			{
				cout << "Вы успешно вошли." << endl;;
				system("cls");
				return P_Good;
			}
			else
			{
				cout << "Неправильный пароль." << endl;;
				return P_Test1;
			}
		}
			else
			{
				cull++;
				e++;
				//return pik_menu();
			}

		
		if (cull != 0 && e >= 3)
		{
			puts("Нет такого пользователя.");
			return P_Test1;
		}
}

Packet Employee::authorization(char *log, char *pas)
{
	int count = 0;
	Employee* user = new Employee[1 * sizeof(Employee)];
	user = user->load_from_file(user, &count);
	Employee* worker = new Employee[1 * sizeof(Employee)];
	static int j, e = 0, cull = 0;
	e = 0;
	while (1)
	{
		system("cls");
		e++;
		cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		//getchar();
		//printf("\n");

		/*if (fgets(worker->login_r(), 20, stdin))
		{
			char* p;
			if (p = strchr(worker->login_r(), '\n'))
			{
				*p = 0;
			}
			else
			{
				scanf("%*[^\n]"); scanf("%*c");
			}
		}*/

		for (j = 0, cull = 0; j < count; j++)
		{
			//if (strcmp(worker->login_r(), user[j].login_r()) == 0)
				if (strcmp(log, user[j].login_r()) == 0)
			{
				cout << "Введите пароль (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
				/*
				if (fgets(worker->password_r(), 20, stdin))
				{
					char* p;
					if (p = strchr(worker->password_r(), '\n'))
					{
						*p = 0;
					}
					else
					{
						scanf("%*[^\n]"); scanf("%*c");
					}
				}*/
				if (strcmp(pas, user[j].password_r()) == 0)
				{
					cout << "Вы успешно вошли." << endl;;
					system("cls");
					return P_Good;
				}
				else
				{
					cout << "Неправильный пароль." << endl;;
					return P_Test1;
				}
			}
			else
			{
				cull++;
				e++;
				//return pik_menu();
			}

		}
		if (cull != 0 && e >= 3)
		{
			puts("Нет такого пользователя.");
			return P_Test1;
		}
	}
}

Packet Employee::create_new_user(Employee* user, int* count, char *log,char *pas,char name[20],char surname[20],char position[20],double pay)
{
	user = user->load_from_file(user, count);

	*count += 1;

	Employee* new_user = new Employee[*count * sizeof(Employee)];
	new_user[*count - 1].id = *count - 1;
	memcpy(new_user, user, (*count - 1) * sizeof(Employee));
	while (1)
	{
		for (int j = 0, k = 0; j < *count; j++)
		{
			strcpy(new_user[*count - 1].login, log);

			//if (strcmp(new_user[*count - 1].login, user[j].login) == 0)
			if (new_user[*count-1].id== user[j].id)
			{
				k++;
			//	cout << "Индекс занят" << endl;
				k++;
				new_user[*count - 1].id++;
			//	Packet msgtype = P_Test1;
				//send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
				//return msgtype;
			}
		}
		break;
	}
	//new_user[*count - 1].id = *count - 1;
	//struct user new_emp;

	while (1)
	{
		static int k = 0;
		cout << "Введите Логин (длинна логина не должна превышать 20 символа, иначе запомнятся только первые 20 ваших символов) :";
		fseek(stdin, 0, SEEK_END);
		/*if (fgets(new_user[*count - 1].login, 20, stdin))
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
		}*/
		for (int j = 0, k = 0; j < *count; j++)
		{
			strcpy(new_user[*count - 1].login, log);
			//if (strcmp(new_user[*count - 1].login, user[j].login) == 0)
			if (strcmp(log, user[j].login) == 0)
			{
				k++;
				cout << "Пользователь с данным логином существует" << endl;
				k++;
				Packet msgtype=P_Test1;
				//send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
				return msgtype;
			}
		}
		if (k == 0)
		{
			cout << "Введите Пароль (длинна пароля не должна превышать 20 символа, иначе запомнятся только первые 20 ваших симвов) :";
			/*if (fgets(new_user[*count - 1].password, 20, stdin))
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
			}*/
			strcpy(new_user[*count - 1].password, pas);
			int sum = 0, n;
			//for (n = 0; n < strlen(new_emp.password); n++) //Ключ вычисляется по формуле (сума битов всех симловов строки)/(количество символов)
			//	sum += new_emp.password[n];
			//new_emp.key = sum / n;
			//for (n = 0; n < strlen(new_emp.password); n++)
			//	new_emp.password[n] = new_emp.password[n] ^ new_emp.key * 2;
			//B = (struct user*) malloc(*count * sizeof(struct user));
			strcpy(new_user[*count - 1].name, name);
			strcpy(new_user[*count - 1].surname, surname);
			strcpy(new_user[*count - 1].position, position);
			new_user[*count - 1].pay = pay;
			new_user[*count - 1].count_pick_job = 0;
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
	Packet packettype=P_Good;
	return packettype=P_Good;
}

void Employee::print_user(Employee* user, int count)
{
	//int i;
	printf("\nСписок пользователей\n\n");
	//fseek(stdin, 0, SEEK_END);
	user = user->load_from_file(user, &count);
	fseek(stdin, 0, SEEK_END);
	StreamTable st(std::cout);
	st.AddCol(5);
	st.AddCol(20);
	//st.AddCol(20);


	//разкомментировать, если столбцы имеют одинаковую толщину
	//st.Clear();
	//st.SetCols(4, 10);

	//st.SetVisible(4, false);//столбцы можно скрывать

	st.MakeBorderExt(true);
	st.SetDelimRow(true, '-');//st.SetDelimRow(false);//без символов-разделителей строк
	st.SetDelimCol(true, '|');//st.SetDelimCol(false);//без символов-разделителей строк

	//заголовок и значения выводятся одинаково
	st << "ID" << "Login";
	//enum { nr = 10 };
	for (int i = 0; i < count; i++) {
		st << user[i].id << user[i].login;
	}
}

Packet Employee::delet_user(Employee* user, int* count, char* log)
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
		/*if (fgets(del, 20, stdin))
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
		}*/
		for (j = 0, cull = 0, cn = 0; j < *count; j++)
		{
			if (strcmp(log, user[j].login) == 0)
			{
				system("cls");
				/*puts("+-----------------------+-----------------------+\n");
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
					ch = vvod();
					printf("\n");
				} while (ch < 0 || ch > 2);*/
				if (user[j].id == 0)
				{
					cout << "Невозможно удалить администратора.\n";
					return P_Test1;
				}
				//if (ch == 1)
				//{
					cn++;
					num = j;

					//break;
				//}
				//else
					//continue;
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
			return P_Test1;
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
		return P_Good;

	}
}

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
