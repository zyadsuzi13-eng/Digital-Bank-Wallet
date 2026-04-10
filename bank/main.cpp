#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <string>
#include <vector>
#include <windows.h>
#include <ctime>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <conio.h>
#include <cstdlib>
#include <sstream>
using namespace std;

int AdminID = 100000;
string Admin_pass = "Admin123!";
vector<user> account;

void Sleep(int ms)
{
    std::cout << "Loading....";
    Sleep(1000);
}

void Clear_Screen()
{
    system("cls");
}

bool isPassTheSame(string pass, string n_pass)
{
    return (pass == n_pass);
}

string code(string t, int id)
{
    string res = "";
    int shift = id % 10;
    for (int i = 0; i < int(t.length()); i++)
    {
        unsigned char c = t[i];
        res += char((c + shift) % 256);
    }
    return res;
}

class Bank
{
public:
    int id;

protected:
    string pass;
    string name;
    int status;
    double balance;

public:
    bool IsActive() const { return status == 1; }
    string get_name() const { return name; }
    double get_balance() const { return balance; }
    int get_id() const { return id; }
    string get_pass() const { return pass; }
    void disActive()
    {
        if (IsActive())
        {
            status = 0;
            cout << "Account is now Inactive";
        }
        else
        {
            status = 1;
            cout << "Account is now Active";
        }
    }

    void set_id(int id)
    {
        if (IsActive())
        {
            this->id = id;
            cout << "Successful change";
        }
        else
        {
            cout << "Sorry this account isn't active";
        }
    }

    void set_name(string name)
    {
        if (IsActive())
        {
            this->name = name;
            cout << "Successful change";
        }
        else
        {
            cout << "Sorry this account isn't active";
        }
    }
    virtual void Menu(user &t) = 0;
    virtual bool logout() = 0;
};

bool IsStrong(string pass)
{
    bool is_not_space = true, is_length = false, is_upper = false, is_lower = false, is_sp = false, is_num = false;
    if (pass.length() >= 8)
        is_length = true;
    for (size_t i = 0; i < pass.length(); i++)
    {
        char c = pass[i];
        if (c == 32)
            is_not_space = false;
        if (isupper(c))
            is_upper = true;
        if (islower(c))
            is_lower = true;
        if (ispunct(c))
            is_sp = true;
        if (isdigit(c))
            is_num = true;
    }
    return (is_length && is_upper && is_lower && is_sp && is_num && is_not_space);
}

string EnterPass()
{
    string pass = "";
    char ch;
    while (true)
    {
        pass = "";
        while (true)
        {
            ch = _getch();
            if (ch == 13)
                break;
            if (ch == 8)
            {
                if (pass.length() > 0)
                {
                    pass.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                pass.push_back(ch);
                cout << "*";
            }
        }
        if (IsStrong(pass))
            break;
        else
            cout << "\nyour password is weak\nenter password again : ";
    }
    return pass;
}

string EnterPass2()
{
    string pass = "";
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == 13)
            break;
        if (ch == 8)
        {
            if (pass.length() > 0)
            {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            pass.push_back(ch);
            cout << "*";
        }
    }
    return pass;
}

int search(const vector<user> &account, int target)
{
    int left = 0, right = account.size() - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (account[mid].id == target)
            return mid;
        if (target < account[mid].id)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1;
}

bool is_id_there(const vector<user> &account, int t)
{
    return (search(account, t) != -1);
}

int Creat_ID(const vector<user> &account)
{
    while (true)
    {
        int id = 10000 + rand() % 90000;
        if (!is_id_there(account, id))
        {
            return id;
        }
    }
}

void load_account(vector<user> &account)
{
    ifstream infile("account.txt");
    if (!infile)
        return;
    string line;
    string t_name, t_pass;
    while (getline(infile, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string id_s, bal_s, stat_s;
        getline(ss, id_s, ',');
        getline(ss, t_name, ',');
        getline(ss, bal_s, ',');
        getline(ss, t_pass, ',');
        getline(ss, stat_s, ',');

        if (!id_s.empty())
        {
            user temp(stoi(id_s), t_name, t_pass, stod(bal_s), stoi(stat_s));
            account.push_back(temp);
        }
    }
}

void save_account(const vector<user> &account)
{
    ofstream outfile("account.txt");
    if (!outfile)
    {
        cout << "Error: Could not open file for saving!" << endl;
        return;
    }

    for (const auto &acc : account)
    {
        outfile << acc.get_id() << ","
                << acc.get_name() << ","
                << acc.get_balance() << ","
                << acc.get_pass() << ","
                << (acc.IsActive() ? 1 : 0) << endl;
    }

    outfile.close();
}

class user : public Bank
{
public:
    void deposit(double amount)
    {
        if (IsActive())
        {
            this->balance += amount;
            cout << "Successful deposit";
        }
        else
        {
            cout << "Sorry this account isn't active";
        }
    }

    void withdraw(double amount)
    {
        if (IsActive())
        {
            this->balance -= amount;
            cout << "Successful withdraw";
        }
        else
        {
            cout << "Sorry this account isn't active";
        }
    }

    user(int id, string name, string pass, double balance, int status)
    {
        this->id = id;
        this->name = name;
        this->pass = pass;
        this->balance = balance;
        this->status = status;
    }

    user(int id, string name, string pass, double balance)
    {
        this->id = id;
        this->name = name;
        this->pass = pass;
        this->balance = balance;
        this->status = 1; // active by default
    }

    user()
    {
        this->id = 0;
        this->name = "";
        this->pass = "";
        this->balance = 0.0;
        this->status = 1;
    }

    int Login(vector<user> &account, string &p)
    {

        cout << "=====  welcome to login page  =====";
        int i;
        cout << "\nEnter the ID : ";
        cin >> i;
        cout << "enter The password : ";
        p = EnterPass2();
        int c_index = search(account, i);
        return c_index;
    }

    void display()
    {
        cout << "ID: " << get_id() << endl;
        cout << "Name: " << get_name() << endl;
        cout << "Balance: " << fixed << setprecision(2) << get_balance() << endl;
        cout << "Status: " << (IsActive() ? "Active" : "Inactive") << endl;
    }

    int signup(vector<user> &account)
    {
        system("cls");
        cout << "=====  welcome to sign up menu  =====\n";
        int id = Creat_ID(account);
        double b;
        string n, p;
        cout << "enter your name : ";
        cin >> n;
        cout << "Enter your password : ";
        p = EnterPass();
        cout << endl
             << "enter started balance : ";
        cin >> b;
        user temp(id, n, p, b);
        account.push_back(temp);
        cout << "Account created successfully with ID: " << id << endl;
        return id;
    }

    void changeAccountStatus()
    {
        if (IsActive())
        {
            status = 0;
            cout << "Account is now Inactive";
        }
        else
        {
            status = 1;
            cout << "Account is now Active";
        }
    }

    bool logout() override
    {
        bool user_ = true;
        cout << "Are you sure you want to logout? (y/n): ";
        char confirm;
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y')
        {
            cout << "Logging out...";
            user_ = false;
            Sleep(1500);
            Clear_Screen();
        }
        else
        {
            cout << "Logout cancelled. Returning to user menu.\n";
            Sleep(1500);
            Clear_Screen();
        }
    }

    bool user_ = true;

    user logedin(vector<user> &account)
    {
        cout << "====welcome to login Page====\n";
        string p;
        int index = Login(account, p);

        if (index == -1)
        {
            cout << "\nthis account doesn't found in the system.\n";
            cout << "try to login Again\n ";
            cout << endl;
        }

        user &t = account[index];

        int tries = 3;

        while (tries > 0)
        {
            if (tries == 1)
            {
                cout << "\nYou have entered the wrong password 3 times and your account is now locked. Returning to main menu.\n";
                Sleep(2000);
                Clear_Screen();
                t.disActive();
                user_ = false;
                break;
            }
            if (!(isPassTheSame(t.get_pass(), p)))
            {
                cout << "\nthe password is incorrect" << endl;
                cout << "try to login Again : ";
                tries--;
                p = EnterPass2();
            }
            else
            {
                return account[index];
                break;
            }
        }
    }

    void Menu(user &t) override
    {
        Clear_Screen();
        bool userin = true;
        int x;
        cout << "=========================================" << endl;
        cout << "      Welcome " << get_name() << " to the menu   ====" << endl;
        cout << "=========================================" << endl
             << endl;
        cout << "Please choose from the following options : ";
        cout << "\n1. Deposit " << endl;
        cout << "2. Withdraw " << endl;
        cout << "3. Inactive Account " << endl;
        cout << "4. Display Account info " << endl;
        cout << "5. Logout " << endl;
        cout << "Enter Your choice : ";
        cin >> x;

        switch (x)
        {

        case 1:
        {
            double a;
            cout << "Enter the amount\n :";
            cin >> a;
            t.deposit(a);

            break;
        }
        break;

        case 2:
        {
            double a;
            cout << "Loading....";
            Sleep(1000);
            Clear_Screen();
            cout << "======  Withdraw  ======\n";
            cout << "\nEnter the amount :";
            cin >> a;
            if (a <= t.get_balance())
            {
                t.withdraw(a);
                break;
            }
            else
            {
                cout << "your balance isn't enough .";
                cout << "press any key to go back.";
                _getch();
                cout << "Loading....";
                Sleep(500);
                Clear_Screen();
            }
        }
        break;
        case 3:
        {
            cout << "Enter your password to Inactive your Account ";
            string p = EnterPass2();
            if (isPassTheSame(t.get_pass(), p))
            {
                t.disActive();
                cout << "your Account disActivated successfully.";
                break;
            }
            break;
        }
        break;
        case 4:
        {
            t.display();
            cout << "\nPress any key to go to main menu : ";
            _getch();
            break;
        }
        break;
        case 5:
        {
            cout << "Are you sure you want to logout? (y/n): ";
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y')
            {
                user_ = false;
                userin = false;
                cout << "Logging out...";
                Sleep(1500);
                Clear_Screen();
            }
            else
            {
                cout << "Logout cancelled. Returning to user menu.\n";
                cout << "Loading....";
                Sleep(1500);
                Clear_Screen();
            }
        }
        default:
            cout << "invalid input";
            break;
        }
    }
};

bool IsStrong(string pass)
{
    bool is_not_space = true, is_length = false, is_upper = false, is_lower = false, is_sp = false, is_num = false;
    if (pass.length() >= 8)
        is_length = true;
    for (size_t i = 0; i < pass.length(); i++)
    {
        char c = pass[i];
        if (c == 32)
            is_not_space = false;
        if (isupper(c))
            is_upper = true;
        if (islower(c))
            is_lower = true;
        if (ispunct(c))
            is_sp = true;
        if (isdigit(c))
            is_num = true;
    }
    return (is_length && is_upper && is_lower && is_sp && is_num && is_not_space);
}

string EnterPass()
{
    string pass = "";
    char ch;
    while (true)
    {
        pass = "";
        while (true)
        {
            ch = _getch();
            if (ch == 13)
                break;
            if (ch == 8)
            {
                if (pass.length() > 0)
                {
                    pass.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                pass.push_back(ch);
                cout << "*";
            }
        }
        if (IsStrong(pass))
            break;
        else
            cout << "\nyour password is weak\nenter password again : ";
    }
    return pass;
}

string EnterPass2()
{
    string pass = "";
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == 13)
            break;
        if (ch == 8)
        {
            if (pass.length() > 0)
            {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            pass.push_back(ch);
            cout << "*";
        }
    }
    return pass;
}

int search(const vector<user> &account, int target)
{
    int left = 0, right = account.size() - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (account[mid].id == target)
            return mid;
        if (target < account[mid].id)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return -1;
}

void load_account(vector<user> &account)
{
    ifstream infile("account.txt");
    if (!infile)
        return;
    string line;
    string t_name, t_pass;
    while (getline(infile, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string id_s, bal_s, stat_s;
        getline(ss, id_s, ',');
        getline(ss, t_name, ',');
        getline(ss, bal_s, ',');
        getline(ss, t_pass, ',');
        getline(ss, stat_s, ',');

        if (!id_s.empty())
        {
            user temp(stoi(id_s), t_name, t_pass, stod(bal_s), stoi(stat_s));
            account.push_back(temp);
        }
    }
}

void save_account(const vector<user> &account)
{
    ofstream outfile("account.txt");
    if (!outfile)
    {
        cout << "Error: Could not open file for saving!" << endl;
        return;
    }

    for (const auto &acc : account)
    {
        outfile << acc.get_id() << ","
                << acc.get_name() << ","
                << acc.get_balance() << ","
                << acc.get_pass() << ","
                << (acc.IsActive() ? 1 : 0) << endl;
    }

    outfile.close();
}

class Admin : public Bank
{

public:
    bool islogin;
    Admin(int id, string name, string pass)
    {
        islogin = false;
        this->id = id;
        this->name = name;
        this->pass = pass;
    }
    void display(const vector<user> &account)
    {
        system("cls");
        double totalBankMoney = 0;
        cout << left << setw(10) << "ID" << "| " << setw(20) << "Name" << "| " << setw(14) << "Balance" << "| " << setw(12) << "Status" << endl;
        for (const auto &acc : account)
        {
            cout << left << setw(10) << acc.get_id()
                 << "| " << setw(20) << acc.get_name()
                 << "| " << setw(14) << fixed << setprecision(2) << acc.get_balance()
                 << "| " << (acc.IsActive() ? "Active" : "Inactive") << endl;
            totalBankMoney += acc.get_balance();
        }
        cout << "\nTotal balance in the bank: " << fixed << setprecision(2) << totalBankMoney << endl;
    }

    bool Login(int _id, string pass)
    {
        Sleep(1000);
        system("cls");
        cout << "=====  welcome to Admin login page  =====";
        int i;
        cout << "\nEnter the ID : ";
        cin >> i;
        cout << "enter The password : ";
        string p = EnterPass2();
        if (isPassTheSame(p, pass) && isPassTheSame(to_string(i), to_string(_id)))
        {
            islogin = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool Menu()
    {
        Sleep(1000);
        system("cls");
        if (islogin())
        {

            int x;
            bool in = true;
            cout << "===================================" << endl;
            cout << "=====  welcome to Admin menu  =====" << endl;
            cout << "===================================" << endl
                 << endl;
            cout << "Please choose from the following options : ";
            cout << "\n1. Display All Accounts " << endl;
            cout << "2. Search For Account (ID)" << endl;
            cout << "3. change  Account status (ID)" << endl;
            cout << "4. Exit " << endl;
            cout << "Enter Your choice : ";
            cin >> x;
            cout << "\nlogin in....";
            Sleep(1000);
            while (in)
            {
                int x = AdminMenu();
                switch (x)
                {
                case 1:
                {
                    displayAllAccount(account);
                    cout << endl;
                    cout << "press any key exit : ";
                    _getch();
                    return true;
                    break;
                }
                break;

                case 2:
                {
                    int D;
                    cout << "\nLoading....";
                    Sleep(750);
                    Clear_Screen();
                    cout << "=====  Search For Account  =====\n";
                    cout << "Enter the ID : ";
                    cin >> D;
                    cout << "\nSearching....";
                    Sleep(1000);
                    sort(account.begin(), account.end(), [](const Bank &a, const Bank &b)
                         { return a.get_id() < b.get_id(); });
                    int index = search(account, D);
                    if (index == -1)
                    {
                        cout << "this account doesn't found in the system ";
                        cout << endl
                             << "press any key to exit : ";
                        _getch();
                        return true;
                        continue;
                    }
                    Bank &b = account[index];
                    Clear_Screen();
                    b.display();
                    cout << "press * to change this Account status to " << (b.IsActive() ? "(InActive)" : "(Active)") << " or any other key to exit :";
                    char x;
                    cin >> x;
                    cin.ignore();
                    if (x == '*')
                    {
                        cout << "Are you sure you want to do this process ? [y/n] : ";
                        char y;
                        cin >> y;
                        if (y == 'y' || y == 'Y')
                        {
                            b.disActive();
                            cout << "done " << (b.IsActive() ? "(unActivated)" : "(Activated)") << " account " << endl;
                            cout << "press any key to exit :";
                            _getch();
                            cout << "\nLoading....";
                            Sleep(1500);
                            return true;
                        }
                    }
                    else
                    {
                        cout << "invalid input press any key to go back : ";
                        _getch();
                        return true;
                    }
                }
                break;

                case 3:
                {
                    cout << "\nLoading....";
                    Sleep(1500);
                    Clear_Screen();
                    cout << "Enter the ID of the account : ";
                    int h;
                    cin >> h;
                    int index = search(account, h);
                    if (index != -1)
                    {
                        user &b = account[index];
                        cout << "Are you sure you want to " << (b.IsActive() ? "unActive" : "Active") << "? [y/n] : ";
                        char y;
                        cin >> y;
                        if (y == 'y' || y == 'Y')
                        {
                            b.disActive();
                            cout << "done " << (b.IsActive() ? "(Activated)" : "(unActivated)") << " account " << endl;
                            save_account(account);
                            cout << "press any key to exit :";
                            _getch();
                            sleep(1000);
                            return true;
                            continue;
                        }
                        else
                        {
                            cout << "\ninvalid input press any key to go back : ";
                            _getch();
                            return true;
                            break;
                        }
                    }
                    else
                    {
                        cout << "this account not found in the system";
                        cout << "\npress any key to go back";
                        _getch();
                        sleep(750);
                        return true;
                        continue;
                    }
                }
                break;
                case 4:
                {
                    cout << "\nExiting....";
                    Sleep(1000);
                    in = false;
                    return false;
                }
                default:
                    cout << "\ninvalid input ";
                    return false;
                    break;
                }
            }
        }
        else
        {
            cout << "\nWrong data !" << endl;
            cout << "Press Any key to go to the main menu : ";
            _getch();
            sleep(1000);
            Clear_Screen();
            in = false;
        }

        return in;
    }
};

int mainMenu()
{
    int x;
    cout << "===================================" << endl;
    cout << "=====   Welcome to main menu   ====" << endl;
    cout << "===================================" << endl;
    cout << "Please choose from the following options : " << endl;
    cout << "1. Login " << endl;
    cout << "2. sign up " << endl;
    cout << "3. Login As Admin " << endl;
    cout << "4. Exit " << endl;
    cout << "Enter Your choice : ";
    cin >> x;
    return x;
}

int main()
{
    srand(time(0));
    load_account(account);
    sort(account.begin(), account.end(), [](const Bank &a, const Bank &b)
         { return a.get_id() < b.get_id(); }); // sort the account vector to be ready for binary search
    cout << "===================================" << endl;
    cout << "=====   Welcome to Our Bank   =====" << endl;
    cout << "===================================" << endl;

    while (true)
    {
        int choice = Menu();

        if (choice == 4) // exit
        {
            save_account(account);
            Sleep(2000);
            return 0;
        }

        cout << "\nLoading....";
        Sleep(2000);
        switch (choice)
        {
        case 1: // login
        {
            Clear_Screen(); // clear the screen before login page
            user u;
            u.logedin(account);
        }
        break;
        case 2: // sign up
        {

            Clear_Screen(); // clear the screen before sign up page
            user temp;
            temp.register(account);
            save_account(account);
            cout << "\nPress any key to go to the main menu : ";
            _getch();
            Clear_Screen();
        }
        break;

        case 3: // login as admin
        {
            Clear_Screen(); // clear the screen before admin login page
            Admin admin(AdminID, "Admin", Admin_pass);
            if (admin.Login(AdminID, Admin_pass))
            {
                while (admin.Login(AdminID, Admin_pass))
                {
                    admin.Menu();
                }
            }
            else
            {
                cout << "\nWrong data !" << endl;
                cout << "Press Any key to go to the main menu : ";
                _getch();
                Sleep(1000);
                Clear_Screen();
            }
        }
        default:
            cout << "\nInvalid Input .";
            cout << "\nEnter Any key to try again : ";
            _getch();
            cout << "\nLoading....";
            Sleep(1500);
            Clear_Screen();
            break;
        }
    }
}

return 0;
}
