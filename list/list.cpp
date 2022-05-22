#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

short defaultTextColor = 7;


class List
{
    private:
        int rows = 0;
        string rowContent[8];
    public:
        void SetRows(int r)
        {
            rows = r;
        }
        int GetRows()
        {
            return rows;
        }
        void SetRowContent(int rowNr, string content)
        {
            rowContent[rowNr-1] = content;
        }
        void ClearRowContent()
        {
            for (int i = 0; i < rows; i++)
            {
                rowContent[i] = "";
            }
        }
        void DrawList()
        {
            cout << "/\\" << endl;
            for (int i = 1; i < (rows + 1); i++)
            {
                cout << "R" << i << ": " << rowContent[i-1] << endl;
            }
            cout << "\\/" << endl;
        }
};

List list;

string commands[9] =
{
    "help",
    "quit",
    "up",
    "down",
    "draw",
    "clear",
    "setRows",
    "setContent",
    "create"
};

enum commandsMapping
{
    help,
    quit,
    up,
    down,
    draw,
    clear,
    setRows,
    setContent,
    create
};

void command()
{
    string inputCommand = "";
    cout << "please type in your command:" << endl;
    SetConsoleTextAttribute(hConsole, 3);
    cin >> inputCommand;
    SetConsoleTextAttribute(hConsole, 7);

    cout << endl;

    if (inputCommand == commands[up])
    {
        return;
    }
    if (inputCommand == commands[down])
    {
        return;
    }
    if (inputCommand == commands[draw])
    {
        list.DrawList();
        return;
    }
    if (inputCommand == commands[clear])
    {
        list.ClearRowContent();
        return;
    }
    if (inputCommand == commands[setRows])
    {
        int numberofrows;
        cout << "number of rows: ";
        cin >> numberofrows;
        list.SetRows(numberofrows);
        list.ClearRowContent();
        return;
    }
    if (inputCommand == commands[setContent])
    {
        int row;
        string content;
        cout << "row number: ";
        cin >> row;
        cout << "content: ";
        /*cin >> content;*/
        cin.ignore();
        getline(cin, content);
        list.SetRowContent(row, content);
        return;
    }
    if (inputCommand == commands[help])
    {
        cout << "here are all valid commands:" << endl;
        SetConsoleTextAttribute(hConsole, 14);
        for (int i = 0; i < ((sizeof(commands)/sizeof(commands[0]))); i++)
        {
            cout << commands[i] << endl;
        }
        SetConsoleTextAttribute(hConsole, defaultTextColor);
        return;
    }
    if (inputCommand == commands[quit])
    {
        SetConsoleTextAttribute(hConsole, 10);
        cout << "press any key to close console" << endl;
        SetConsoleTextAttribute(hConsole, defaultTextColor);
        exit(1);
        return;
    }
    if (inputCommand == commands[create])
    {
        return;
    }
    SetConsoleTextAttribute(hConsole, 12);
    cout << "'" << inputCommand << "' " << "is a invalid command" << endl;
    SetConsoleTextAttribute(hConsole, defaultTextColor);
}

int pause;

int main()
{
    SetConsoleTextAttribute(hConsole, defaultTextColor);
    list.SetRows(8);
    list.SetRowContent(5, "Test");

    while (true)
    {
        command();
        cout << endl;
    }
}