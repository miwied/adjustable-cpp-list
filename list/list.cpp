#include <iostream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
short defaultTextColor = 7;

void clearConsole() 
{
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    cout << "\x1B[2J\x1B[H";
}

class CustomizableList
{
private:
    list<string> list;
    double rowRenderLimit = 4;
    int startingRow = 0;
    int page = 1;
    int rowIndex = 1;
public:
    void ResetParams(bool resetRow, bool resetPage, bool resetRowIndex)
    {
        if (resetRow)
        {
            startingRow = 0;
        }
        if (resetPage)
        {
            page = 1;
        }
        if (resetRowIndex)
        {
            rowIndex = 1;
        }
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
        ResetParams(true, true, true);
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
    void ClearRowContent()
    {
        ResetParams(true, true, true);
        list.clear();
    }
    void DrawList()
    {
        clearConsole();
        cout << "page: " << page << "/" << to_string(MaxPageNr()) << endl;
        cout << endl;
        // default row render limit
        if (rowRenderLimit == 0)
        {
            rowRenderLimit = 5;
        }

        cout << "/\\ " << endl;

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
        ResetParams(false, false, true);
        list.push_back(content);
    }
    void setRowRenderLimit(int rows)
    {
        ResetParams(true, true, true);
        rowRenderLimit = rows;
    }
};

CustomizableList firstDemoList;

string commands[10] =
{
    "help",
    "draw",
    "up",
    "down",
    "clear",
    "setRowView",
    "editContent",
    "addRow",
    "loadDemoData",
    "quit"
};

enum commandsMapping
{
    help,
    draw,
    up,
    down,
    clear,
    setRowView,
    editContent,
    addRow,
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
        firstDemoList.ResetParams(true, true, true);
        firstDemoList.DrawList();
        return;
    }
    if (inputCommand == commands[clear])
    {
        firstDemoList.ClearRowContent();
        firstDemoList.DrawList();
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
        bool loop = true;
        while (loop)
        {
            string content;
            cout << "content: ";
            cin.ignore();
            getline(cin, content);
            firstDemoList.AddRow(content);
            firstDemoList.DrawList();
            cout << endl;

            string input;
            cout << "add another row? (y/n): ";
            cin >> input;
            if (input != "y")
            {
                loop = false;
            }
            firstDemoList.ResetParams(true, true, true);
            firstDemoList.DrawList();
            cout << endl;
        }
        return;
    }
    if (inputCommand == commands[editContent])
    {
        int row;
        string content;
        cout << "row number: ";
        cin >> row;
        cout << "content: ";
        cin.ignore();
        getline(cin, content);
        firstDemoList.SetRowContent(row, content);
        firstDemoList.ResetParams(true, true, true);
        firstDemoList.DrawList();
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
    if (inputCommand == commands[loadDemoData])
    {
        firstDemoList.SetDemoData();
        firstDemoList.DrawList();
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
    firstDemoList.DrawList();
    cout << endl;

    while (true)
    {
        commandHandling();
        cout << endl;
    }
}