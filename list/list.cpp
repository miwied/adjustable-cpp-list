#include <iostream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// colors for console text
short defaultTextColor = 7; // (grey)
short red = 12;
short yellow = 14;
short blue = 3;
short green = 10;

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

void SetConsoleTextColor(int color)
{
    SetConsoleTextAttribute(hConsole, color);
}

void PrintConsoleText(string text, int color)
{
    SetConsoleTextColor(color);
    cout << text << endl;
    SetConsoleTextColor(defaultTextColor);
}

void ClearConsole()
{
    system("cls");
}

void CloseConsole()
{
    PrintConsoleText("press any key to close console", green);
    exit(1);
}

void PrintVailidCommands()
{
    ClearConsole();
    PrintConsoleText("here are all valid commands:", defaultTextColor);
    for (int i = 0; i < ((sizeof(commands) / sizeof(commands[0]))); i++)
    {
        PrintConsoleText(commands[i], yellow);
    }
}

class CustomizableList
{
private:
    list<string> list;
    double rowRenderLimit;
    int startingRow;
    int page;
    int rowIndex;
public:
    CustomizableList()
    {
        //default parameters
        rowRenderLimit = ceil(4);
        startingRow = 0;
        page = 1;
        rowIndex = 1;
    }
    void ResetParams()
    {
        startingRow = 0;
        page = 1;
        rowIndex = 1;
    }
    int MaxPageNr()
    {
        double result = list.size() / rowRenderLimit;
        if (result <= 0)
        {
            result = 1;
        }
        return ceil(result);
    }
    void UpdateFirstRowIndex()
    {
        rowIndex = ((page - 1) * rowRenderLimit) + 1;
    }
    void DrawList()
    {
        ClearConsole();
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
    void ClearList()
    {
        ResetParams();
        list.clear();
        DrawList();
    }
    void LoadDemoData()
    {
        ResetParams();
        ClearList();
        //just some demo data
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
        DrawList();
    }
    void EditRow()
    {
        int row;
        string content;
        SetConsoleTextColor(defaultTextColor);
        cout << "row number: ";
        SetConsoleTextColor(blue);
        cin >> row;
        SetConsoleTextColor(defaultTextColor);
        cout << "content: ";
        SetConsoleTextColor(blue);
        cin.ignore();
        getline(cin, content);
        SetConsoleTextColor(defaultTextColor);
        std::list<std::string>::iterator iter = list.begin();
        advance(iter, (row - 1));
        if ((row - 1) <= list.size())
        {
            list.insert(iter, content);
            list.erase(iter);
        }
        DrawList();
    }
    void AddRow()
    {
        bool loop = true;
        int previousMaxPage = MaxPageNr();
        while (loop)
        {
            string content;
            SetConsoleTextColor(defaultTextColor);
            cout << "content: ";
            SetConsoleTextColor(blue);
            cin.ignore();
            getline(cin, content);
            list.push_back(content);
            SetConsoleTextColor(defaultTextColor);
            DrawList();
            int currentMaxPage = MaxPageNr();
            if (currentMaxPage > previousMaxPage)
            {
                startingRow = startingRow + rowRenderLimit;
                page = page + 1;
                DrawList();
            }
            cout << endl;

            string input;
            cout << "add another row? (y/n): ";
            cin >> input;
            if (input != "y")
            {
                loop = false;
            }
            DrawList();
            cout << endl;
        }
    }
    void SetViewRange()
    {
        int numberofrows;
        SetConsoleTextColor(defaultTextColor);
        cout << "number of rows: ";
        SetConsoleTextColor(blue);
        cin >> numberofrows;
        SetConsoleTextColor(defaultTextColor);
        ResetParams();
        rowRenderLimit = numberofrows;
        DrawList();
    }
    void previousPage()
    {
        if (page > 1)
        {
            startingRow = startingRow - rowRenderLimit;
            page = page - 1;
        }
        DrawList();
    }
    void nextPage()
    {
        if (page < MaxPageNr())
        {
            page = page + 1;
            startingRow = startingRow + rowRenderLimit;
        }
        DrawList();
    }
};

CustomizableList demoList;

void commandHandling()
{
    string inputCommand = "";
    PrintConsoleText("please type in your command:", defaultTextColor);
    SetConsoleTextColor(blue);
    cin >> inputCommand;
    SetConsoleTextColor(defaultTextColor);
    cout << endl;

    if (inputCommand == commands[up])
    {
        demoList.previousPage();
        return;
    }
    if (inputCommand == commands[down])
    {
        demoList.nextPage();
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
        return;
    }
    if (inputCommand == commands[setViewRange])
    {
        demoList.SetViewRange();
        return;
    }
    if (inputCommand == commands[addRow])
    {
        demoList.AddRow();
        return;
    }
    if (inputCommand == commands[editRow])
    {
        demoList.EditRow();
        return;
    }
    if (inputCommand == commands[loadDemoData])
    {
        demoList.LoadDemoData();
        return;
    }
    if (inputCommand == commands[help])
    {
        PrintVailidCommands();
        return;
    }
    if (inputCommand == commands[quit])
    {
        CloseConsole();
        return;
    }
    //since no if-statement was correct its a invalid command
    string errorMsg = "'" + inputCommand + "' is a invalid command";
    PrintConsoleText(errorMsg, red);
}

int main()
{
    demoList.LoadDemoData();
    demoList.DrawList();
    cout << endl;

    while (true)
    {
        commandHandling();
        cout << endl;
    }
}