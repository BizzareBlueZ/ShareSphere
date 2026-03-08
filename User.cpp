#include "User.h"
#include "Student.h"
#include <sstream>
using namespace std;

User::User(string id, string p, string name, string dept,
           string cont, string mail, int trans)
{
    userID = id;
    pin = p;
    fullName = name;
    department = dept;
    contact = cont;
    email = mail;
    totalTransactions = trans;
}

string User::getDetails() const
{
    stringstream ss;
    ss << "ID: " << userID << "\n"
       << "PIN: " << pin << "\n"
       << "Name: " << fullName << "\n"
       << "Department: " << department << "\n"
       << "Contact: " << contact << "\n"
       << "Email: " << email << "\n"
       << "Transactions: " << totalTransactions;
    return ss.str();
}

string User::serialize() const
{
    stringstream ss;
    ss << userID << "|" << pin << "|" << fullName << "|" << department << "|"
       << contact << "|" << email << "|" << totalTransactions;
    return ss.str();
}

User *User::deserialize(const string &data)
{
    stringstream ss(data);
    string type, id, pin, name, dept, cont, mail, transStr;

    getline(ss, type, '|');

    if (type == "Student")
    {
        return Student::deserialize(data);
    }

    if (type.find('|') == string::npos)
    {
        id = type;
        getline(ss, name, '|');
        getline(ss, pin, '|');
        getline(ss, dept, '|');
        getline(ss, cont, '|');
        getline(ss, mail, '|');
        getline(ss, transStr, '|');

        int trans = 0;
        if (!transStr.empty())
        {
            trans = stoi(transStr);
        }

        return new Student(id, pin, name, dept, cont, mail, trans);
    }

    return nullptr;
}

// === VIRTUAL FUNCTION: display() writes user info to any output stream ===
void User::display(ostream &os) const
{
    os << "[" << getUserType() << "] " << fullName << " (ID: " << userID << ")\n"
       << "  Department: " << department << " | Contact: " << contact << "\n"
       << "  Email: " << (email.empty() ? "N/A" : email)
       << " | Transactions: " << totalTransactions;
}

// === FRIEND FUNCTION + STREAMS: operator<< uses virtual display() ===
ostream &operator<<(ostream &os, const User &user)
{
    user.display(os); // Polymorphic call - calls Student::display() for Student objects
    return os;
}