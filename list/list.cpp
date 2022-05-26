#include <iostream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
short defaultTextColor = 7;

void clearConsole() 
{
    system("cls");
}

class CustomizableList
{
private:
    list<string> list;
    //default parameters
    double rowRenderLimit = 4;
    int startingRow = 0;
    int page = 1;
    int rowIndex = 1;
public:
    void ResetParams()
    {
        startingRow = 0;
        page = 1;
        rowIndex = 1;
    }
    int MaxPageNr()
    {
        double res = list.size() / rowRenderLimit;
        if (res <= 0)
        {
            res = 1;
        }
        return ceil(res);
    }
    void SetPage(int p)
    {
        page = p;
    }
    int GetPage()
    {
        return page;
    }
    void SetRowIndex(int r)
    {
        rowIndex = r;
    }
    void UpdateFirstRowIndex()
    {
        rowIndex = ((page - 1) * rowRenderLimit) + 1;
    }
    int GetRowIndex()
    {
        return rowIndex;
    }
    void SetStartingRow(int r)
    {
        startingRow = r;
    }
    int GetStartingRow()
    {
        return startingRow;
    }
    void SetRowRenderLimit(int r)
    {
        rowRenderLimit = r;
    }
    int GetRowRenderLimit()
    {
        return rowRenderLimit;
    }
    void SetDemoData()
    {
        ResetParams();
        //just some colors as demo data
        list.push_back("red");
        list.push_back("green");
        list.push_back("blue");
        list.push_back("yellow");
        list.push_back("brown");
        list.push_back("purple");
        list.push_back("orange");
        list.push_back("grey");
        list.push_back("black");
        list.push_back("white");
        list.push_back("pink");
    }
    void SetRowContent(int rowNr, string content)
    {
        std::list<std::string>::iterator iter = list.begin();
        advance(iter, (rowNr - 1));
        if ((rowNr - 1) <= list.size())
        {
             list.insert(iter, content);
             list.erase(iter);
        }
    }
    void ClearList()
    {
        ResetParams();
        list.clear();
    }
    void DrawList()
    {
        clearConsole();
        cout << "page: " << page << "/" << to_string(MaxPageNr()) << endl;
        cout << endl;

        cout << "/\\ " << endl;

        auto row = list.begin();
        if (startingRow <= list.size())
        {
            advance(row, startingRow);
        }
        UpdateFirstRowIndex();
        for (int i = 0; i < rowRenderLimit; i++)
        {
            if (row == list.end())
            {
                cout << "R" << rowIndex << ": " << "---" << endl;
            }
            else
            {
                const string& ref = *row;
                string copy = *row;
                cout << "R" << rowIndex << ": " << ref << endl;
                row++;
            }
            rowIndex++;
        }

        cout << "\\/" << endl;
    }
    void AddRow(string content)
    {
        list.push_back(content);
    }
    void setRowRenderLimit(int rows)
    {
        ResetParams();
        rowRenderLimit = rows;
    }
};

CustomizableList demoList;

string commands[10] =
{
    "help",
    "draw",
    "up",
    "down",
    "clearList",
    "setViewRange",
    "addRow",
    "editRow",
    "loadDemoData",
    "quit"
};

enum commandsMapping
{
    help,
    draw,
    up,
    down,
    clearList,
    setViewRange,
    addRow,
    editRow,
    loadDemoData,
    quit
};

void commandHandling()
{
    string inputCommand = "";
    SetConsoleTextAttribute(hConsole, defaultTextColor);
    cout << "please type in your command:" << endl;
    SetConsoleTextAttribute(hConsole, 3);
    cin >> inputCommand;
    SetConsoleTextAttribute(hConsole, 7);
    cout << endl;

    if (inputCommand == commands[up])
    {
        int currentStartingRow = demoList.GetStartingRow();
        int currentRenderLimit = demoList.GetRowRenderLimit();
        int currentPage = demoList.GetPage();
        if (currentPage > 1)
        {
            demoList.SetStartingRow(currentStartingRow - currentRenderLimit);
            demoList.SetPage(currentPage - 1);
        }
        demoList.DrawList();
        return;
    }
    if (inputCommand == commands[down])
    {
        int maxPage = demoList.MaxPageNr();
        int currentStartingRow = demoList.GetStartingRow();
        int currentRenderLimit = demoList.GetRowRenderLimit();
        int currentPage = demoList.GetPage();
        if (currentPage < maxPage)
        {
            demoList.SetStartingRow(currentStartingRow + currentRenderLimit);
            demoList.SetPage(currentPage + 1);
        }
        demoList.DrawList();
        return;
    }
    if (inputCommand == commands[draw])
    {
        demoList.ResetParams();
        demoList.DrawList();
        return;
    }
    if (inputCommand == commands[clearList])
    {
        demoList.ClearList();
        demoList.DrawList();
        return;
    }
    if (inputCommand == commands[setViewRange])
    {
        int numberofrows;
        cout << "number of rows: ";
        cin >> numberofrows;
        demoList.setRowRenderLimit(numberofrows);
        demoList.DrawList();
        return;
    }
    if (inputCommand == commands[addRow])
    {
        bool loop = true;
        int previousMaxPage = demoList.MaxPageNr();
        while (loop)
        {
            string content;
            cout << "content: ";
            cin.ignore();
            getline(cin, content);
            demoList.AddRow(content);
            demoList.DrawList();
            int currentMaxPage = demoList.MaxPageNr();
            if (currentMaxPage > previousMaxPage)
            {
                int maxPage = demoList.MaxPageNr();
                int currentStartingRow = demoList.GetStartingRow();
                int currentRenderLimit = demoList.GetRowRenderLimit();
                int currentPage = demoList.GetPage();
                demoList.SetStartingRow(currentStartingRow + currentRenderLimit);
                demoList.SetPage(currentPage + 1);
                demoList.DrawList();
            }
            cout << endl;

            string input;
            cout << "add another row? (y/n): ";
            cin >> input;
            if (input != "y")
            {
                loop = false;
            }
            demoList.DrawList();
            cout << endl;
        }
        return;
    }
    if (inputCommand == commands[editRow])
    {
        int row;
        string content;
        cout << "row number: ";
        cin >> row;
        cout << "content: ";
        cin.ignore();
        getline(cin, content);
        demoList.SetRowContent(row, content);
        demoList.DrawList();
        return;
    }
    if (inputCommand == commands[loadDemoData])
    {
        demoList.ClearList();
        demoList.SetDemoData();
        demoList.DrawList();
        return;
    }
    if (inputCommand == commands[help])
    {
        clearConsole();
        cout << "here are all valid commands:" << endl;
        SetConsoleTextAttribute(hConsole, 14);
        for (int i = 0; i < ((sizeof(commands) / sizeof(commands[0]))); i++)
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
    //since no if-statement was correct its a invalid command
    SetConsoleTextAttribute(hConsole, 12);
    cout << "'" << inputCommand << "' " << "is a invalid command" << endl;
    SetConsoleTextAttribute(hConsole, defaultTextColor);
}

int main()
{
    demoList.SetDemoData();
    demoList.DrawList();
    cout << endl;

    while (true)
    {
        commandHandling();
        cout << endl;
    }
}