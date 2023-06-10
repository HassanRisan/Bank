







#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>


using namespace std;
const string ClientsFileName = "Clients.txt";
const string UserFileName = "User.txt";


void ShowMainMenue();
void ShowTransactionsMenueScreen();
void ShowManagUserMenueScreen();
enum enMainMenuePermissions
{
    eAll = -1, pListClients = 1 , pAddNewClient = 2,
    pDeleteClient = 4 , pUpdateClients = 8 , pFindClient = 16,
    pTranactions = 32, pMangaUsers = 64

};
bool CheadAcces(enMainMenuePermissions Permissions);
void ShowAccessPermissions();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

struct sUser
{
    string UserName = "";
    string Pas = "";
    int Acces;
    bool MarkForDelelte = false;
    bool Add = false;
};

 sUser UserToCheakAcces;

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /*erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}
///////////////////////////////////////////////
sUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
    sUser User ;
    vector <string> vUser;
    vUser = SplitString(Line, Seperator);

    User.UserName = vUser[0];
    User.Pas = vUser[1];
    User.Acces = stoi(vUser[2]);
    return User;
}

void Print1User(sUser U)
{
    cout << "=========================\n";
    cout << "User Name: " << U.UserName << endl;
    cout << "Passowrd: " << U.Pas << endl;
    cout << "Acces: " << U.Acces << endl;
    cout << "=========================\n";

}



string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
//////////////////////////////////////////////
string ConvertRecordToLineUser(sUser User, string Seperator = "#//#")
{
    string Line = "";
    Line = User.UserName + Seperator;
    Line += User.Pas + Seperator;
    Line += to_string(User.Acces);
    return Line;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); //read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}


vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}
///////////////////////////////////
vector <sUser> LoadUsersDataFromFile(string FileName)
{
    vector <sUser> vUser;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLinetoRecord(Line);
            vUser.push_back(User);
        }
        MyFile.close();
    }
    return vUser;
}


void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen()
{

    if (!CheadAcces(enMainMenuePermissions::pListClients))
    {
        ShowAccessPermissions();
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}


bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
 ///////////////////////////
bool FindUsertByUserName(string UserName,string Pas, sUser & User)
{
    vector <sUser> vUser = LoadUsersDataFromFile(UserFileName);

    for (sUser U : vUser)
    {
        if (U.UserName == UserName && U.Pas == Pas)
        {
            User = U;
            return true;
        }
    }
    return false;
}


bool FindUsertByUserName(string UserName)
{
    vector <sUser> vUser = LoadUsersDataFromFile(UserFileName);

    for (sUser U : vUser)
    {
        if (U.UserName == UserName)
        {
            return true;
        }
    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}
////////////////////////////////////////////////////////////////////
bool MarkUserForDeleteByUserName(string UserNumber, vector <sUser>& vUser)
{

    for (sUser& U : vUser)
    {

        if (U.UserName == UserNumber)
        {
            U.MarkForDelelte = true;
            return true;
        }
    }
    return false;
}



vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}
//////////////////////////////////////////////////////////
void SaveUserDataToFile(string FileName, vector <sUser> vUser)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sUser U : vUser)
        {

            if (U.MarkForDelelte == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLineUser(U);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}



void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}
///////////////////////////////////////////////////////////////////
bool DeleteUserByUserName(string UserName, string Pas, vector <sUser> & vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUsertByUserName(UserName,Pas, User))
    {

        Print1User(User);

        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUserDataToFile(UserFileName, vUsers);

            //Refresh Clients 
            vUsers = LoadUsersDataFromFile(ClientsFileName);

            cout << "\nUser Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << UserName << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient & C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber(bool User = true)
{
    string AccountNumber = "";
    if (User)
        cout << "\nPlease enter AccountNumber?   ";
    else
        cout << "\nPlease enter User Name.    ";

    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheadAcces(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessPermissions();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{

    if (!CheadAcces(enMainMenuePermissions::pUpdateClients))
    {
        ShowAccessPermissions();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    if (!CheadAcces(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessPermissions();
        return;
    }
    if (!CheadAcces(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessPermissions();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();
}

void ShowFindClientScreen()
{
    if (!CheadAcces(enMainMenuePermissions::pFindClient))
    {
        ShowAccessPermissions();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransaction = 6,
    ManagUsers = 7,eLogeOut = 8
};

enum enTransactionsMenueOption {
    eDesposit = 1, eWithdraw = 2,
    eTotalBalances = 3, eManiMenue = 4
};

enum enManageUserMenueOption
{
    ListUsers = 1, AddNewUser = 2,
    DleleteUser = 3, UpdateUser = 4,
    FindUser = 5 , MainMenue = 6
};



void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void ShowAccessPermissions()
{
    cout << "----------------------------------\n";
    cout << "   You don't have acces here.\n";
    cout << "----------------------------------\n";
    GoBackToMainMenue();
}

bool CheadAcces(enMainMenuePermissions Permissions)
{
    if (UserToCheakAcces.Acces == enMainMenuePermissions::eAll)
        return true;
    if ((Permissions & UserToCheakAcces.Acces) == Permissions)
        return true;
    return false;
}


void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions menue screen...";
    system("pause>0");
    ShowTransactionsMenueScreen();
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4].\n";
    short Choose = 0;
    cin >> Choose;
    return Choose;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient> vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {

            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }
}

void ShowDepositScreem()
{
    cout << "- - - - - - - - - - - - - - - - - - - \n";
    cout << "\t   Deposit Screen\n";
    cout << "- - - - - - - - - - - - - - - - - - - \n";

    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client); double Amount = 0;
    
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}

void ShowWithDrawScreen()
{
    cout << "- - - - - - - - - - - - - - - - - - - \n";
    cout << "\t   Withdra Screen\n";
    cout << "- - - - - - - - - - - - - - - - - - - \n";
    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);
    cout << "\nPlease enter whitdraw amount...   ";
    double Amount = 0;
    cin >> Amount;
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up : " << Client.AccountBalance << endl;
        cout << "Please enter another amount...   ";
        cin >> Amount;
    }
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void PrintClientJustTotalBalance(sClient& Client)
{
    cout << "| " << setw(26) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(23) << left << Client.AccountBalance;
}

long int SumTotlaBalace(vector <sClient> vClients)
{
    long int Sum = 0;
    for (sClient& C : vClients)
    {
        Sum += C.AccountBalance;
    }
    return Sum;
}

void ShowTotalBalance()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
   
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(26) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(23) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sClient& C : vClients)
    {
        PrintClientJustTotalBalance(C);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "\n\t\t\t\t\t Total Balance = " << SumTotlaBalace(vClients) << endl;
}

void PerformTransactionsMenueOption(enTransactionsMenueOption TransactionsMenueOption)
{
    switch (TransactionsMenueOption)
    {
    case eDesposit:
        system("cls");
        ShowDepositScreem();
        GoBackToMainMenue();
        break;
    case eWithdraw:
        system("cls");
        ShowWithDrawScreen();
        GoBackToMainMenue();
        break;
    case eTotalBalances:
        system("cls");
        ShowTotalBalance();
        GoBackToMainMenue();
        break;
    case eManiMenue:
        ShowMainMenue();
        break;
    default:
        break;
    }
}

void ShowTransactionsMenueScreen()
{
    if (!CheadAcces(enMainMenuePermissions::pTranactions))
    {
        ShowAccessPermissions();
        return;
    }    cout << system("cls");
    cout << "==============================================\n";
    cout << "\tTransactions Menue Screen\n";
    cout << "==============================================\n";
    cout << "\t [1] Deposit.\n";
    cout << "\t [2] Withdraw.\n";
    cout << "\t [3] Total Balances.\n";
    cout << "\t [4] Main Mnue.\n";
    cout << "==============================================\n";

    PerformTransactionsMenueOption((enTransactionsMenueOption)ReadTransactionsMenueOption());
}

void PrintUser(sUser User)
{
    cout << "| " << setw(20) << left << User.UserName;
    cout << "| " << setw(15) << left << User.Pas;
    cout << "| " << setw(15) << left << User.Acces << "\n\n";
}

void ShowAllUsersScreen()
{

    if (!CheadAcces(enMainMenuePermissions::pListClients))
    {
        ShowAccessPermissions();
        return;
    }
    vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);

    cout << "                                 User List (" << vUsers.size() << ") User(s).\n";
    cout << "----------------------------------------------------------------------";
    cout << "----------------------\n\n";
    cout << "| " << setw(20) << left << "User Name";
    cout << "| " << setw(15) << left << "Password";
    cout << "| " << setw(15) << left << "Permissions\n";
    cout << "------------------------------------------------------------------";
    cout << "----------------------\n\n";
    for (sUser& U : vUsers)
    {
        PrintUser(U);
    }
    cout << "----------------------------------------------------------------------";
    cout << "-----------------------\n\n";
}


void ShowFindUserScreen()
{

       vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);
       string UserName = ReadClientAccountNumber();
       for (sUser& U : vUsers)
       {
           if (UserName == U.UserName)
               Print1User(U);
       }
}

void ShowDeleteUserScreen()
{
    vector<sUser> vUsers = LoadUsersDataFromFile(UserFileName);
    string UserName  = ReadClientAccountNumber(false);
    cout << "Please enter the passowrd.    ";
    string Pas = "";
    cin >> Pas;
    DeleteUserByUserName(UserName,Pas,vUsers);
}

void GoBackToManagUsersMenue()
{
    cout << "\n\nPress any key to go back to Manag Users...";
    system("pause>0");
    ShowManagUserMenueScreen();
}

sUser ReadFullUser()
{

    sUser User;

    cout << "Please enter the user name.   ";
    getline(cin >> ws, User.UserName);

    while (FindUsertByUserName(User.UserName))
    {
        cout << "This name is not avalibal.\n";
        cout << "Please enter the user name.   ";
        getline(cin >> ws, User.UserName);
    }
    cout << "Please enter the passowrd.    ";
    getline(cin, User.Pas);


    char cha = 'y';

    cout << "Do you want give this user full acces? Y/N   ";
    cin >> cha;
    if (cha == 'y' || cha == 'Y')
    {
        User.Acces = -1;
    }
    else
    { 
        User.Acces = 0;

        cout << "Do you want give acess to clients list?   Y/N   ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces = (int)enMainMenuePermissions::pListClients;
        }
        cout << "Do you want give acess to add new client?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces = (int)enMainMenuePermissions::pAddNewClient;
        }
        cout << "Do you want give acess to delete client?   Y/N   ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += (int)enMainMenuePermissions::pDeleteClient;
        }
        cout << "Do you want give acess to update client?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += (int)enMainMenuePermissions::pUpdateClients;
        }
        cout << "Do you want give acess to find client?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += (int)enMainMenuePermissions::pFindClient;
        }
        cout << "Do you want give acess to tranactions?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += (int)enMainMenuePermissions::pTranactions;
        }
        cout << "Do you want give acess to ManageUsers?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += (int)enMainMenuePermissions::pMangaUsers;
        }
    }
    return User;
}

void AddUsers()
{

    sUser User = ReadFullUser();
    string Line = ConvertRecordToLineUser(User);
    AddDataLineToFile(UserFileName, Line);
    cout << "The user has been successfully added.\n";
}

void ShowAddNewUser()
{
    string AddMore = "";
    do
    {
        AddUsers();
        cout << "Do you want add other user? Yes or No.";
        cin >> AddMore;
    } while (AddMore == "Yes" || AddMore == "yes");

}

sUser updateUser(sUser User)
{
    char cha = 'y';
    cout << "Do you want chang passowrd? Y/N   ";
    cin >> cha;
    if (cha == 'y' || cha == 'Y')
    {
        cout << "Enter the Passowrd.   ";
        cin >> User.Pas;
    }

    cout << "Do you want give this user full acces? Y/N   ";
    cin >> cha;
    if (cha == 'y' || cha == 'Y')
    {
        User.Acces = -1;
    }
    else
    {
        cout << "Do you want give acess to clients list?   Y/N   ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces = enMainMenuePermissions::pListClients;
        }
        cout << "Do you want give acess to add new client?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += enMainMenuePermissions::pAddNewClient;
        }
        cout << "Do you want give acess to delete client?   Y/N   ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += enMainMenuePermissions::pDeleteClient;
        }
        cout << "Do you want give acess to update client?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += enMainMenuePermissions::pUpdateClients;
        }
        cout << "Do you want give acess to find client?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += enMainMenuePermissions::pFindClient;
        }
        cout << "Do you want give acess to tranactions?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += enMainMenuePermissions::pTranactions;
        }
        cout << "Do you want give acess to ManageUsers?   Y/N  ";
        cin >> cha;
        if (cha == 'y' || cha == 'Y')
        {
            User.Acces += enMainMenuePermissions::pMangaUsers;
        }
    }
    return User;
}

void ShowUpdateUser()
{

    vector <sUser> vUsers = LoadUsersDataFromFile(UserFileName);

    cout << "Please enter the user name.   ";
    string Pas, UserName;
    cin >> UserName;
    cout << "Please enter the passowrd.    ";
    cin >> Pas;

    for (sUser & User : vUsers)
    {
      User = updateUser(User);
      SaveUserDataToFile(UserFileName, vUsers);
    }

}

void PerformManagUsersMenueOption(enManageUserMenueOption ManageMenueOption)
{
    switch (ManageMenueOption)
    {
    case ListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManagUsersMenue();
        break;
    case AddNewUser:
        system("cls");
        ShowAddNewUser();
        GoBackToManagUsersMenue();
        break;
    case DleleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManagUsersMenue();
        break;
    case UpdateUser:
        system("cls");
        ShowUpdateUser();
        GoBackToManagUsersMenue();

        break;
    case FindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManagUsersMenue();
        break;
    case MainMenue:
        ShowMainMenue();
        break;
    default:
        break;
    }
}


void ShowManagUserMenueScreen()
{
    if (!CheadAcces(enMainMenuePermissions::pMangaUsers))
    {
        ShowAccessPermissions();
        return;
    }

    system("cls");
    cout << "====================================================\n";
    cout << "            Manage Users Menue Screen\n";
    cout << "====================================================\n";
    cout << "               [1] List Users.\n";
    cout << "               [2] Add New User.\n";
    cout << "               [3] Delete User.\n";
    cout << "               [4] Update.\n";
    cout << "               [5] Find User.\n";
    cout << "               [6] Main Menue.\n";
    cout << "====================================================\n";
    cout << "Choose whate do yoy want to do ? [1 to 6].   ";
    short Choose = 0;
    cin >> Choose;
    PerformManagUsersMenueOption(enManageUserMenueOption(Choose));
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eTransaction:
        system("cls");
        ShowTransactionsMenueScreen();
        /////////////////////////////////////////////
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::ManagUsers:
        system("cls");
        ShowManagUserMenueScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::eLogeOut:
        system("cls");
        ShowEndScreen();
        break;
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transaction.\n";
    cout << "\t[7] Manag Users.\n";
    cout << "\t[8] Logeout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool IsTrueUser()
{
    string UserName = ReadClientAccountNumber();
    cout << "Enter The Pas...  ";
    string Pas = "";
    getline(cin >> ws, Pas);

    if (FindUsertByUserName(UserName, Pas, UserToCheakAcces))
        return true;
    else
        return false;
}

void Logein()
{


    while (true)
    {
        system("cls");
        cout << "------------------------\n";
        cout << "        logein          \n";
        cout << "------------------------\n";
        if (IsTrueUser())
        {
            ShowMainMenue();
            break;
        }
        cout << "\nNot Avalibal!\n";
        cout << "\n Press any key to try loge in again.\n";
        system("pause>0");

    }
}

int main()
{
    Logein();
    system("pause>0");

    return 0;
}