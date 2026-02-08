#include "User.h"
#include "Student.h"
#include <sstream>
using namespace std;

User::User(string id, string name, string dept, string cont,
           string mail, int trans) {
    userID = id;
    fullName = name;
    department = dept;
    contact = cont;
    email = mail;
    totalTransactions = trans;
}

string User::getDetails() const {
    stringstream ss;
    ss << "ID: " << userID << "\n"
       << "Name: " << fullName << "\n"
       << "Department: " << department << "\n"
       << "Contact: " << contact << "\n"
       << "Email: " << email << "\n"
       << "Transactions: " << totalTransactions;
    return ss.str();
}

string User::serialize() const {
    stringstream ss;
    ss << userID << "|" << fullName << "|" << department << "|"
       << contact << "|" << email << "|" << totalTransactions;
    return ss.str();
}

User* User::deserialize(const string& data) {
    stringstream ss(data);
    string type, id, name, dept, cont, mail, transStr;
    
    getline(ss, type, '|');
    
    if(type == "Student") {
        return Student::deserialize(data);
    }
    
    if(type.find('|') == string::npos) {
        id = type;
        getline(ss, name, '|');
        getline(ss, dept, '|');
        getline(ss, cont, '|');
        getline(ss, mail, '|');
        getline(ss, transStr, '|');
        
        int trans = 0;
        if(!transStr.empty()) {
            trans = stoi(transStr);
        }
        
        return new Student(id, name, dept, cont, mail, trans);
    }
    
    return nullptr;
}