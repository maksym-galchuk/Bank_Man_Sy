#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

// Using const instead of macro as recommended
constexpr float MIN_BALANCE = 500;

class InsufficientFunds {};

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;
public:
    // Default constructor with initialization list
    Account() : accountNumber(0), firstName(""), lastName(""), balance(0) {}

    // Constructor with initialization list
    Account(string fname, string lname, float balance);

    // Accessor methods with const qualifier
    long getAccNo() const { return accountNumber; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    float getBalance() const { return balance; }

    void Deposit(float amount);
    void Withdraw(float amount);

    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    // Friend declarations for operator overloading
    // Hidden friend declarations inside the class scope
    friend ofstream& operator<<(ofstream& ofs, const Account& acc);
    friend ifstream& operator>>(ifstream& ifs, Account& acc);
    friend ostream& operator<<(ostream& os, const Account& acc);
};

long Account::NextAccountNumber = 0;

class Bank
{
private:
    map<long, Account> accounts;
public:
    Bank();
    Account OpenAccount(const string& fname, const string& lname, float balance);
    Account BalanceEnquiry(long accountNumber) const;
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts() const;
    ~Bank();
};

int main()
{
    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;

    cout << "***Banking System***" << endl;

    do
    {
        cout << "\n\tSelect one option below ";
        cout << "\n\t1 Open an Account";
        cout << "\n\t2 Balance Enquiry";
        cout << "\n\t3 Deposit";
        cout << "\n\t4 Withdrawal";
        cout << "\n\t5 Close an Account";
        cout << "\n\t6 Show All Accounts";
        cout << "\n\t7 Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            cout << "Enter initial Balance: ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance);
            cout << endl << "Congratulation Account is Created" << endl;
            cout << acc;
            break;

        case 2:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            acc = b.BalanceEnquiry(accountNumber);
            cout << endl << "Your Account Details" << endl;
            cout << acc;
            break;

        case 3:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            cout << "Enter Balance:";
            cin >> amount;
            acc = b.Deposit(accountNumber, amount);
            cout << endl << "Amount is Deposited" << endl;
            cout << acc;
            break;

        case 4:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            cout << "Enter Balance:";
            cin >> amount;
            acc = b.Withdraw(accountNumber, amount);
            cout << endl << "Amount Withdrawn" << endl;
            cout << acc;
            break;

        case 5:
            cout << "Enter Account Number:";
            cin >> accountNumber;
            b.CloseAccount(accountNumber);
            cout << endl << "Account is Closed" << endl;
            break;

        case 6:
            b.ShowAllAccounts();
            break;

        case 7:
            break;

        default:
            cout << "\nEnter correct choice";
            exit(0);
        }
    } while (choice != 7);

    return 0;
}

// Initialize all class members in the initialization list
Account::Account(string fname, string lname, float balance)
    : accountNumber(++NextAccountNumber), firstName(fname), lastName(lname), balance(balance)
{
    // Body intentionally left empty as initialization is done in the list
}

void Account::Deposit(float amount)
{
    balance += amount;
}

void Account::Withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}

ofstream& operator<<(ofstream& ofs, const Account& acc)
{
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream& operator>>(ifstream& ifs, Account& acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream& operator<<(ostream& os, const Account& acc)
{
    os << "First Name:" << acc.getFirstName() << endl;
    os << "Last Name:" << acc.getLastName() << endl;
    os << "Account Number:" << acc.getAccNo() << endl;
    os << "Balance:" << acc.getBalance() << endl;
    return os;
}

Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile)
    {
        //cout<<"Error in Opening! File Not Found!!"<<endl;
        return;
    }

    // Structured binding declaration for better readability
    while (!infile.eof())
    {
        infile >> account;
        accounts.try_emplace(account.getAccNo(), account);
    }

    Account::setLastAccountNumber(account.getAccNo());
    infile.close();
}

Account Bank::OpenAccount(const string& fname, const string& lname, float balance)
{
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.try_emplace(account.getAccNo(), account);

    outfile.open("Bank.data", ios::trunc);

    // Use structured binding for better readability
    for (const auto& [accountNumber, accountObj] : accounts)
    {
        outfile << accountObj;
    }

    outfile.close();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber) const
{
    auto itr = accounts.find(accountNumber);
    return itr->second;
}

Account Bank::Deposit(long accountNumber, float amount)
{
    auto itr = accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::Withdraw(long accountNumber, float amount)
{
    auto itr = accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
    auto itr = accounts.find(accountNumber);
    cout << "Account Deleted" << itr->second;
    accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts() const
{
    // Use structured binding for better readability
    for (const auto& [accountNumber, accountObj] : accounts)
    {
        cout << "Account " << accountNumber << endl << accountObj << endl;
    }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);

    // Use structured binding for better readability
    for (const auto& [accountNumber, accountObj] : accounts)
    {
        outfile << accountObj;
    }

    outfile.close();
}