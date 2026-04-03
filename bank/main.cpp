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

class Bank
{
public:
    int id;

private:
    string pass;
    string name;
    int status;
    double balance;

public:
    Bank(int t_id, string t_name, string t_pass, double t_balance, int t_status)
    {
        id = t_id;
        name = t_name;
        pass = t_pass;
        balance = t_balance;
        status = t_status;
    }

    Bank(const int &t_id, const string &t_name, string t_pass, double t_balance)
    {
        status = 1;
        this->id = t_id;
        name = t_name;
        pass = t_pass;
        if (t_balance >= 0)
        {
            balance = t_balance;
        }
        else
        {
            balance = 0;
            cout << "balance can't be -ve so your balance is set as (0)";
        }
    }

    bool IsActive() const { return status == 1; }
    string get_name() const { return name; }
    double get_balance() const { return balance; }
    int get_id() const { return id; }
    string get_pass() const { return pass; }

    void disActive() { status = 0; }

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

    void display()
    {
        cout << "Owner name : " << name << endl;
        cout << "Id : " << id << endl;
        cout << "balance : " << balance << endl;
        cout << "status : " << (IsActive() ? "Active" : "unActive") << endl;
    }
};

vector<Bank> account;

bool is_S(char c)
{
    int _c = int(c);
    return ((_c >= 33 && _c <= 47) ||
            (_c >= 58 && _c <= 64) ||
            (_c >= 91 && _c <= 96) ||
            (_c >= 123 && _c < 127));
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

bool IsStrong(string pass)
{
    bool is_not_space = true,
         is_length = false,
         is_upper = false,
         is_lower = false,
         is_sp = false,
         is_num = false;
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
        if (is_S(c))
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

int search(const vector<Bank> &account, int target)
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

bool is_id_there(const vector<Bank> &account, int t)
{
    return (search(account, t) != -1);
}

int Creat_ID(const vector<Bank> &account)
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

void load_account(vector<Bank> &account)
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
            Bank temp(stoi(id_s), t_name, t_pass, stod(bal_s), stoi(stat_s));
            account.push_back(temp);
        }
    }
}

Bank CreatAccount(vector<Bank> &account)
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
    return Bank(id, n, p, b);
}

void displayAllAccount(const vector<Bank> &account)
{
    system("cls");
    double totalBankMoney = 0;
    cout << left << setw(10) << "ID" << "| " << setw(20) << "Name" << "| " << setw(14) << "Balance" << "| " << setw(12) << "Status" << endl;
    for (const auto &acc : account)
    {
        cout << left << setw(10) << acc.get_id()
             << "| " << setw(20) << acc.get_name()
             << "| " << setw(14) << fixed << setprecision(2) << acc.get_balance()
             << "| " << (acc.IsActive() ? "Active" : "Frozen") << endl;
        totalBankMoney += acc.get_balance();
    }
}

int Menu()
{
    int x;
    cout << "\n1. Login " << endl;
    cout << "2. sign up " << endl;
    cout << "3. Login As Admin " << endl;
    cout << "4. Exit " << endl;
    cout << "Enter Your choice : ";
    cin >> x;
    return x;
}

int UserMenu()
{
    int x;
    cout << "\n1. deposit " << endl;
    cout << "2. withdraw " << endl;
    cout << "3. Active/Unactive Account " << endl;
    cout << "4. Show Account Details " << endl;
    cout << "5. Go To Main Menu " << endl;
    cout << "6. Exit " << endl;
    cout << "Enter Your choice : ";
    cin >> x;
    return x;
}

int Login(vector<Bank> &account, string &p)
{   system("cls");
    cout << "=====  welcome to login menu  =====";
    int i;
    cout << "\nEnter the ID : ";
    cin >> i;
    cout << "enter The password : ";
    p = EnterPass2();
    sort(account.begin(), account.end(), [](const Bank &a, const Bank &b)
         { return a.get_id() < b.get_id(); });
    int c_index = search(account, i);
    return c_index;
}

int AdminMenu()
{
    system("cls");
    int x;
    cout << "=====  welcome to login menu  =====";
    cout << "\n1. Display All Accounts " << endl;
    cout << "2. Search For Account (ID)" << endl;
    cout << "3. Login As Admin " << endl;
    cout << "4. Modify/Update Account " << endl;
    cout << "5. Close/Freeze Account " << endl;
    cout << "6. Exit " << endl;
    cout << "Enter Your choice : ";
    cin >> x;
    return x;
}

void Clear_Screen()
{
    system("cls");
}

bool isPassTheSame(string pass, string n_pass)
{
    return (pass == n_pass);
}

int main()
{
    srand(time(0));
    load_account(account);
    cout << "===================================" << endl;
    cout << "=====   Welcome to Our Bank   =====" << endl;
    cout << "===================================" << endl;

    while (true)
    {
        int choice = Menu();
        cout << "Loding....";
        Sleep(2000);

        if (choice == 4)
        {
            break;
        }

        switch (choice)
        {
        case 1: // login
        {
            Clear_Screen();
            cout << "====welcome to login Page====\n";
            bool user = true;
            while (true)
            {
                string p;
                int index = Login(account, p);

                if (index == -1)
                {
                    cout << "\nthis account dosn't found in the system.\n";
                    cout << "try to login Again\n ";
                    cout << endl;
                    continue;
                }

                Bank &t = account[index];

                while (true)
                {
                    if (!(isPassTheSame(t.get_pass(), p)))
                    {
                        cout << "\nthe password is incorrect\n"
                             << endl;
                        p = EnterPass2();
                    }
                    else
                    {
                        break;
                    }
                }
                if (!(t.IsActive()))
                {
                    cout << "this account is not active \n";
                    Sleep(2000);
                    Clear_Screen();
                    user = false;
                    continue;
                }

                cout << "\n login in.....\n";
                Sleep(1000);
                Clear_Screen();
                cout << "=====  welcome " << t.get_name() << "  =====\n";
                int choos = UserMenu();

                switch (choos)
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
                    while (true)
                    {
                        double a;
                        cout << "\nEnter the amount :";
                        cin >> a;
                        if (a <= t.get_balance())
                        {
                            t.withdraw(a);
                            break;
                        }
                    }
                    break;
                }
                break;
                case 3:
                {
                    cout << "Enter your password to unactive your Account ";
                    string pa = EnterPass2();
                    if (isPassTheSame(t.get_pass(), p))
                    {
                        t.disActive();
                        cout << "your Account disActived successfully.";
                        break;
                    }
                    break;
                }
                break;
                case 4:
                {
                    t.display();
                    break;
                }
                break;
                case 5:
                {
                    user = false;
                    break;
                }
                default:
                    cout << "invalid input";
                    break;
                    continue;
                }
            }
        }
        break;
        case 2: // sign up
        {
            Bank tempOpj = CreatAccount(account);
            account.push_back(tempOpj);
            cout << "Account Created Successfully\n";
            cout << "your ID is : " << tempOpj.get_id();
            cout << endl
                 << "Press any key to go to main menu : ";
            _getch();
            continue;
        }
        break;
        default:
            cout << "Invalid Input .";
            continue;
            break;
        }
    }

    return 0;
}
