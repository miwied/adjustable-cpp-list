/*
miwied - adjustable-cpp-list - may 2022
https://github.com/miwied/adjustable-cpp-list
*/

#include <iostream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

// console handle for text-color use
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// colors for console text
short defaultTextColor = 7; // (grey)
short red = 12;
short yellow = 14;
short blue = 3;
short green = 10;

// commands as string
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
// commands as enum (for easier use of the arrarys-index in the program)
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

//  print text to console + reset color
void PrintConsoleText(string text, int color)
{
    SetConsoleTextColor(color);
    cout << text << endl;
    SetConsoleTextColor(defaultTextColor);
}

// clear everything on the console
void ClearConsole()
{
    system("cls");
}

// exit / close console
void CloseConsole()
{
    PrintConsoleText("press any key to close console", green);
    exit(1);
}

// prints the whole commands[] array
void PrintVailidCommands()
{
    ClearConsole();
    PrintConsoleText("here are all valid commands:", defaultTextColor);
    for (int i = 0; i < ((sizeof(commands) / sizeof(commands[0]))); i++)
    {
        PrintConsoleText(commands[i], yellow);
    }
}

class AdjustableList
{
private:
    list<string> list; // creating a object of the std::list class
    double rowRenderLimit; // adjustable row render limit
    int startingRow; // is used to change the pages
    int page; // for page display and calculation of the row index 
    int rowIndex; // index which is printed at the beginning of the line
public:
    // default parameters
    void ResetParams()
    {
        startingRow = 0;
        page = 1;
        rowIndex = 1;
    }
    // inital parameters
    AdjustableList()
    {
        rowRenderLimit = ceil(4); 
        ResetParams();
    }
    // returns the maximum page number according to the size of the list & row-render 
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
        cout << "page: " << page << "/" << to_string(MaxPageNr()) << endl; // page: x / x
        cout << endl;

        cout << "/\\ " << endl; // up arrow

        auto row = list.begin(); // start of the list
        if (startingRow <= list.size())
        {
            advance(row, startingRow); // page change
        }
        UpdateFirstRowIndex();
        for (int i = 0; i < rowRenderLimit; i++)
        {
            // print row number + "---" if view range is bigger then list
            if (row == list.end())
            {
                cout << "R" << rowIndex << ": " << "---" << endl;
            }
            // prints rows with corresponding list-content
            else
            {
                const string& ref = *row; // pointer to the (first) row of the list
                string copy = *row;
                cout << "R" << rowIndex << ": " << ref << endl;
                row++; // go to the next row of the list
            }
            rowIndex++;
        }

        cout << "\\/" << endl; // down arrow
    }
    // list reset
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
        // demo data
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
            list.insert(iter, content); // insert new content
            list.erase(iter); // delete old content
        }
        DrawList();
    }
    // adds row at the bottom of the list
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
            list.push_back(content); // add the content to the bottom (end) of the list
            SetConsoleTextColor(defaultTextColor);
            DrawList();
            int currentMaxPage = MaxPageNr();

            // go to the next page if the end of the list is reached
            if (currentMaxPage > previousMaxPage)
            {
                startingRow = startingRow + rowRenderLimit;
                page = page + 1;
                DrawList();
            }

            cout << endl;

            // 'add another row' - loop
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
    void PreviousPage()
    {
        if (page > 1)
        {
            startingRow = startingRow - rowRenderLimit;
            page = page - 1;
        }
        DrawList();
    }
    void NextPage()
    {
        if (page < MaxPageNr())
        {
            page = page + 1;
            startingRow = startingRow + rowRenderLimit;
        }
        DrawList();
    }
};

void CommandHandling(AdjustableList *list)
{
    string inputCommand = "";
    PrintConsoleText("please type in your command:", defaultTextColor);
    SetConsoleTextColor(blue);
    cin >> inputCommand;
    SetConsoleTextColor(defaultTextColor);
    cout << endl;

    // check if the entered line is a valid command (if so: call the corresponding method)

    if (inputCommand == commands[up])
    {
        list->PreviousPage();
        return;
    }
    if (inputCommand == commands[down])
    {
        list->NextPage();
        return;
    }
    if (inputCommand == commands[draw])
    {
        list->ResetParams();
        list->DrawList();
        return;
    }
    if (inputCommand == commands[clearList])
    {
        list->ClearList();
        return;
    }
    if (inputCommand == commands[setViewRange])
    {
        list->SetViewRange();
        return;
    }
    if (inputCommand == commands[addRow])
    {
        list->AddRow();
        return;
    }
    if (inputCommand == commands[editRow])
    {
        list->EditRow();
        return;
    }
    if (inputCommand == commands[loadDemoData])
    {
        list->LoadDemoData();
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

    // since no if-statement was correct its a invalid command
    string errorMsg = "'" + inputCommand + "' is a invalid command";
    PrintConsoleText(errorMsg, red);
}

int main()
{
    AdjustableList demoList;
    // draw list with demo data at the beginning of the programm
    demoList.LoadDemoData();
    demoList.DrawList();
    cout << endl;

    // command handling loop
    while (true)
    {
        CommandHandling(&demoList); // pass the (address of the) AdjustableList-object to the handling function
        cout << endl;
    }
}