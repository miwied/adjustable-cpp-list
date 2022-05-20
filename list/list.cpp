#include <iostream>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class LightObject
{
    private:
        string name;
        bool state;
    public:
        void setName(string n)
        {
            name = n;
        }
        string getName()
        {
            return name;
        }
        void setState(bool s)
        {
            state = s;
        }
        bool getState()
        {
            return state;
        }
};

class List
{
    private:
        int rows = 0;
        string rowContent[8];
    public:
        void setRows(int r)
        {
            rows = r;
        }
        int getRows()
        {
            return rows;
        }
        void setRowContent(int rowNr, string content)
        {
            rowContent[rowNr-1] = content;
        }
        void clearRowContent()
        {
            for (int i = 0; i < rows; i++)
            {
                rowContent[i] = "";
            }
        }
        void DrawList()
        {
            cout << "^" << endl;
            for (int i = 1; i < (rows + 1); i++)
            {
                cout << "R" << i << ": " << rowContent[i-1] << endl;
            }
            cout << "v" << endl;
        }
};

List lights;

string commandList[7] =
{
    "up",
    "down",
    "draw",
    "clearList",
    "setRows",
    "setContent",
    "help"
};

void command()
{
    string inputCommand = "";
    cout << "please type in your command:" << endl;
    SetConsoleTextAttribute(hConsole, 3);
    cin >> inputCommand;
    SetConsoleTextAttribute(hConsole, 7);

    cout << endl;

    if (inputCommand == "up")
    {
        return;
    }
    if (inputCommand == "down")
    {
        return;
    }
    if (inputCommand == "draw")
    {
        lights.DrawList();
        return;
    }
    if (inputCommand == "clearList")
    {
        lights.clearRowContent();
        return;
    }
    if (inputCommand == "setRows")
    {
        int numberofrows;
        cout << "type number of rows: ";
        cin >> numberofrows;
        lights.setRows(numberofrows);
        lights.clearRowContent();
        return;
    }
    if (inputCommand == "setContent")
    {
        int row;
        string content;
        cout << "type row number: ";
        cin >> row;
        cout << "type content: ";
        cin >> content;
        lights.setRowContent(row, content);
        lights.clearRowContent();
        return;
    }
    if (inputCommand == "help")
    {
        cout << "here are the valid commands:" << endl;
        SetConsoleTextAttribute(hConsole, 14);
        for (int i = 0; i < ((sizeof(commandList)/sizeof(commandList[0]))); i++)
        {
            cout << commandList[i] << endl;
        }
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }
    SetConsoleTextAttribute(hConsole, 12);
    cout << "invalid command" << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

int pause;

int main()
{
    SetConsoleTextAttribute(hConsole, 7);
    lights.setRows(8);
    lights.setRowContent(5, "Test");
    while (true)
    {
        command();
        cout << endl;
    }
}