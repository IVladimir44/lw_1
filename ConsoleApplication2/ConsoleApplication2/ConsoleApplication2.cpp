#include <iostream>
#include <winsock2.h>
#include <locale.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 40650

int main()
{
    setlocale(LC_ALL, "ru");

    WSADATA ws2;
    if (WSAStartup(MAKEWORD(2, 2), &ws2) != 0)
    {
        cout << "Ошибка";
        return 0;
    }


    sockaddr_in server;
    int sockett2;
    if ((sockett2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        cout << "Ошибка";
        return 0;
    }


    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);


    while (true)
    {
        char message[BUFLEN];
        cout << "(Вы): ";
        cin.getline(message, BUFLEN);


        if (sendto(sockett2, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            cout << "Ошибка";
            return 0;
        }


        char answer[BUFLEN] = {};
        int slen = sizeof(sockaddr_in);
        int answer_length;
        if (answer_length = recvfrom(sockett2, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR)
        {
            cout << "Ошибка";
            return 0;
        }

        cout << "(Сервер): " << answer << endl;
    }

    closesocket(sockett2);
    WSACleanup();
}
