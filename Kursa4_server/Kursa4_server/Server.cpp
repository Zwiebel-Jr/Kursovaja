#pragma comment(lib, "ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Employeer.h"



#pragma warning(disable: 4996)

SOCKET Connections[100];
int Counter = 0;

using namespace std;
/*
enum Packet {
	P_ChatMessage,
	P_Test
};*/

bool ProccesPacket(int index, Packet packettype) {
	switch (packettype)
	{
	case P_ChatMessage:
	{
		int msg_size,pas_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);
		recv(Connections[index], (char*)&pas_size, sizeof(int), NULL);
		char* pas = new char[pas_size + 1];
		pas[pas_size] = '\0';
		recv(Connections[index], pas, pas_size, NULL);
		Employee* user = new Employee[1 * sizeof(Employee)];
		Packet msgtype;

		msgtype=user->authorization(msg,pas);
		for (int i = 0; i < Counter; i++) {
			if (i == index)	
			{
				//continue;
			}

		//	Packet msgtype = P_Good;
			send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
		//	send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
		//	send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
		delete[] pas;
		break;
	}
	case P_ChatMessage_ad:
	{
		int msg_size, pas_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);
		recv(Connections[index], (char*)&pas_size, sizeof(int), NULL);
		char* pas = new char[pas_size + 1];
		pas[pas_size] = '\0';
		recv(Connections[index], pas, pas_size, NULL);
		Employee* user = new Employee[1 * sizeof(Employee)];
		Packet msgtype;

		msgtype = user->authorization_ad(msg, pas);
		for (int i = 0; i < Counter; i++) {
			if (i == index)
			{
				//continue;
			}

			//	Packet msgtype = P_Good;
			send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
			//	send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
			//	send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
		delete[] pas;
		break;
	}
	case P_New:
	{
		int msg_size, pas_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);
		recv(Connections[index], (char*)&pas_size, sizeof(int), NULL);
		char* pas = new char[pas_size + 1];
		char name[20], surname[20], position[20];
		double pay;
		pas[pas_size] = '\0';
		recv(Connections[index], pas, pas_size, NULL);
		recv(Connections[index], name, sizeof(char)*20, NULL);
		recv(Connections[index], surname, pas_size, NULL);
		recv(Connections[index], position, pas_size, NULL);
		recv(Connections[index], (char*)&pay, sizeof(double), NULL);
		Employee* user = new Employee[1 * sizeof(Employee)];
		Packet msgtype;
		int count;
		msgtype = user->create_new_user(user,&count,msg,pas,name,surname,position,pay);
		for (int i = 0; i < Counter; i++) {
			if (i == index)
			{
				//continue;
			}

			//	Packet msgtype = P_Good;
			send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
			//	send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
			//	send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
		delete[] pas;
		break;
	}
	case P_Del:
	{
		int msg_size, pas_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);
		//recv(Connections[index], (char*)&pas_size, sizeof(int), NULL);
		//char* pas = new char[pas_size + 1];
		//pas[pas_size] = '\0';
		//recv(Connections[index], pas, pas_size, NULL);
		Employee* user = new Employee[1 * sizeof(Employee)];
		Packet msgtype;
		int count;
		msgtype = user->delet_user(user, &count, msg);
		for (int i = 0; i < Counter; i++) {
			if (i == index)
			{
				//continue;
			}

			//	Packet msgtype = P_Good;
			send(Connections[i], (char*)&msgtype, sizeof(Packet), NULL);
			//	send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
			//	send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
	//	delete[] pas;
		break;
	}
	default:
		std::cout << "Unrecognized packet: " << packettype << std::endl;
		break;
	}
	return true;
}



void ClientHandler(int index)
{
	Packet packettype;
	while (true)
	{
		recv(Connections[index], (char*)&packettype, sizeof(Packet), NULL);
		if (!ProccesPacket(index, packettype))
		{
			break;
		}
	}
	closesocket(Connections[index]);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
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

	SOCKET slisten = socket(AF_INET, SOCK_STREAM, NULL);
	bind(slisten, (SOCKADDR*)&addr, sizeof(addr));
	listen(slisten, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(slisten, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0)
		{
			std::cout << "Error!\n";
		}
		else
		{
			std::cout << "Client Connected!\n";
			/*
			std::string msg = "Hello. It's my first network programm";
			int msg_size = msg.size();
			Packet msgtype = P_ChatMessage;
			send(newConnection, (char*)&msgtype, sizeof(Packet), NULL);
			send(newConnection, (char*)&msg_size, sizeof(int), NULL);
			send(newConnection, msg.c_str(), msg_size, NULL);
			*/
			Packet testpacket = P_Good;
			send(newConnection, (char*)&testpacket, sizeof(Packet), NULL);
			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
			
			
		}
	}

	system("pause");

	return 0;
}