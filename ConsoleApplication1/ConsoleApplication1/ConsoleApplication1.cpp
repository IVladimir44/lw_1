#include <iostream>
#include <winsock2.h>
#include <clocale>
#include <windows.h>
#include <sstream>
#include <vector>
#include <string>


using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 

#define BUFLEN 512
#define PORT 40650

bool breakrules = false;
int selected = 0;


struct Command {
    string comanda;
    vector<string> parameters;
};

Command check_command(const string& commandStr) {
    Command command;

    int number;
    selected = 0;

    istringstream get_mes_word(commandStr);
    string get_word;
    get_mes_word >> command.comanda;


    while (get_mes_word >> get_word) {
        if (get_word == ":")
        {
            break;
        }
        command.comanda += " " + get_word;
    }

    if (command.comanda == "clear display") {
        selected = 1;
    }
    if (command.comanda == "draw pixel") {
        selected = 10;
    }
    if (command.comanda == "draw line") {
        selected = 20;
    }
    if (command.comanda == "draw rectangle")
    {
        selected = 30;
    }
    if (command.comanda == "fill rectangle") {
        selected = 40;
    }
    if (command.comanda == "draw ellipse") {
        selected = 50;
    }
    if (command.comanda == "fill ellipse")
    {
        selected = 60;
    }
    if (command.comanda == "draw circle")
    {
        selected = 70;
    }
    if (command.comanda == "fill circle")
    {
        selected = 80;
    }
    if (command.comanda == "draw rounded rectangle")
    {
        selected = 90;
    }
    if (command.comanda == "fill rounded rectangle")
    {
        selected = 100;
    }
    if (command.comanda == "draw text")
    {
        selected = 110;
    }

    if (selected == 0) {
        breakrules = true;
        return command;
    }


    if (!breakrules)
    {
        while (get_mes_word >> get_word) {
            if (selected == 1 || selected >= 10 && selected <= 12 || selected >= 20 && selected <= 24 || selected >= 30 && selected <= 34
                || selected >= 40 && selected <= 44 || selected >= 50 && selected <= 54 || selected >= 60 && selected <= 64 || selected >= 70 && selected <= 73
                || selected >= 80 && selected <= 83 || selected >= 90 && selected <= 95 || selected >= 100 && selected <= 105 || selected >= 110 && selected <= 115)
            {
                command.parameters.push_back(get_word);
                if (selected == 115)
                {
                    breakrules = false;
                    return command;
                }
                try {
                    number = stoi(get_word);
                }
                catch (const invalid_argument& e) {
                    breakrules = true;
                    return command;
                }
                breakrules = false;
                selected++;
            }
        }
        breakrules = false;
        return command;
    }
    else {
        return command;
    }
}


int main()
{
    setlocale(LC_ALL, "ru");
    sockaddr_in server, client;


    WSADATA ws1;
    if (WSAStartup(MAKEWORD(2, 2), &ws1) != 0)
    {
        cout << "Ошибка";
        return 0;
    }

    SOCKET sockett;
    if ((sockett = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        cout << "Ошибка";
    }


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);


    if (bind(sockett, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        cout << "Ошибка";
        return 0;
    }
    int i = 1;
    while (true)
    {
        fflush(stdout);
        char Messages[BUFLEN] = {};


        int dlina;
        int dlina2 = sizeof(sockaddr_in);
        if (dlina = recvfrom(sockett, Messages, BUFLEN, 0, (sockaddr*)&client, &dlina2) == SOCKET_ERROR)
        {
            cout << "Error!";
            return 0;
        }

        cout << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << " - " << Messages << endl;

        istringstream parametersline(Messages);
        string get_word;
        i = 1;
        while (getline(parametersline, get_word, '\n')) {
            Command command = check_command(get_word);

            string f_word, s_word;
            istringstream nameStream(command.comanda);
            nameStream >> f_word >> s_word;

            if (!breakrules) {
                cout << "Команда: " << command.comanda << endl;
                cout << "Параметры:";
                for (const string& param : command.parameters) {
                    cout << endl << "param #" << i << ": " << param;
                    i++;
                }
                cout << endl;
            }
        }
        if (breakrules)
        {

            if (sendto(sockett, Messages, strlen(Messages), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
            {
                cout << "sendto() получил ошибку. Код ошибки: " << WSAGetLastError();
                return 3;
            }
        }

        if (!breakrules)
        {

            if (sendto(sockett, Messages, strlen(Messages), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
            {
                cout << "sendto() получил ошибку. Код ошибки: " << WSAGetLastError();
                return 3;
            }
        }


    }

    closesocket(sockett);
    WSACleanup();
}
