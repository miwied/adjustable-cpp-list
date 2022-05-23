#include <iostream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
short defaultTextColor = 7;

class CustomizableList
{
private:
    list<string> list;
    double rowRenderLimit = 4;
    int startingRow = 0;
    int page = 1;
    int rowIndex = 1;
    void ResetParams()
    {
        startingRow = 0;
        page = 1;
        rowIndex = 1;
    }
public:
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
        list.push_back("r1 red");
        list.push_back("r2 green");
        list.push_back("r3 blue");
        list.push_back("r4 yellow");
        list.push_back("r5 brown");
        list.push_back("r6 purple");
        list.push_back("r7 blue");
        list.push_back("r8 yellow");
        list.push_back("r9 brown");
        list.push_back("r10 purple");
        list.push_back("r11 pink");
    }
    void SetRowContent(int rowNr, string content)
    {
        std::list<std::string>::iterator iter = list.begin();
        advance(iter, (rowNr - 1));
        if ((rowNr - 1) <= list.size())
        {
            /* list.erase(iter);
             list.insert(iter, content);*/
             //list.assign(rowNr, content);
        }
    }
    void ClearRowContent()
    {
        ResetParams();
        list.clear();
    }
    void DrawList()
    {
        // default row render limit
        if (rowRenderLimit == 0)
        {
            rowRenderLimit = 5;
        }

        cout << "/\\ " << page << "/" << to_string(MaxPageNr()) << endl;

        auto row = list.begin();
        if (startingRow <= list.size())
        {
            advance(row, startingRow);
        }

        for (int i = 0; i < rowRenderLimit; i++)
        {
            if (row == list.end() || list.empty())
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
        ResetParams();
        list.push_back(content);
    }
    void setRowRenderLimit(int rows)
    {
        ResetParams();
        rowRenderLimit = rows;
    }
};

CustomizableList firstDemoList;

string commands[9] =
{
    "help",
    "quit",
    "up",
    "down",
    "draw",
    "clear",
    "setRowView",
    "addRow",
    "setContent"
};

enum commandsMapping
{
    help,
    quit,
    up,
    down,
    draw,
    clear,
    setRowView,
    addRow,
    setContent
};

void command()
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
        int currentStartingRow = firstDemoList.GetStartingRow();
        int currentRenderLimit = firstDemoList.GetRowRenderLimit();
        int currentPage = firstDemoList.GetPage();
        if (currentPage > 1)
        {
            firstDemoList.SetStartingRow(currentStartingRow - currentRenderLimit);
            firstDemoList.SetRowIndex(currentStartingRow - currentRenderLimit + 1);
            firstDemoList.SetPage(currentPage - 1);
        }
        if (currentPage == 1)
        {
            firstDemoList.SetRowIndex(1);
        }
        firstDemoList.DrawList();
        return;
    }
    if (inputCommand == commands[down])
    {
        int maxPage = firstDemoList.MaxPageNr();
        int currentStartingRow = firstDemoList.GetStartingRow();
        int currentRenderLimit = firstDemoList.GetRowRenderLimit();
        int currentPage = firstDemoList.GetPage();
        if (currentPage < maxPage)
        {
            firstDemoList.SetStartingRow(currentStartingRow + currentRenderLimit);
            firstDemoList.SetRowIndex(currentStartingRow + currentRenderLimit + 1);
            firstDemoList.SetPage(currentPage + 1);
        }
        if (currentPage == maxPage)
        {
            int lastStartingRow = (((maxPage - 1) * currentRenderLimit) + 1);
            firstDemoList.SetRowIndex(lastStartingRow);
        }
        firstDemoList.DrawList();
        return;
    }
    if (inputCommand == commands[draw])
    {
        firstDemoList.DrawList();
        return;
    }
    if (inputCommand == commands[clear])
    {
        firstDemoList.ClearRowContent();
        return;
    }
    if (inputCommand == commands[setRowView])
    {
        int numberofrows;
        cout << "number of rows: ";
        cin >> numberofrows;
        firstDemoList.setRowRenderLimit(numberofrows);
        return;
    }
    if (inputCommand == commands[addRow])
    {
        string content;
        cout << "content: ";
        cin.ignore();
        getline(cin, content);
        firstDemoList.AddRow(content);
        return;
    }
    if (inputCommand == commands[setContent])
    {
        int row;
        string content;
        cout << "row number: ";
        cin >> row;
        cout << "content: ";
        cin.ignore();
        getline(cin, content);
        firstDemoList.SetRowContent(row, content);
        return;
    }
    if (inputCommand == commands[help])
    {
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
    firstDemoList.SetDemoData();

    while (true)
    {
        command();
        cout << endl;
    }
}