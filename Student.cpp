#include "Student.h"
#include <sstream>
using namespace std;

Student::Student(string id, string name, string dept, string cont,
                 string mail, int trans, int yr, string hstl, bool verified)
    : User(id, name, dept, cont, mail, trans) {
    year = yr;
    hall = hstl;
    isVerified = verified;
}

string Student::getDetails() const {
    stringstream ss;
    ss << User::getDetails() << "\n"
       << "Year: " << year << "\n"
       << "Hostel: " << hall << "\n"
       << "Verification: " << (isVerified ? "Verified (Email provided)" : "Not verified");
    return ss.str();
}

string Student::serialize() const {
    stringstream ss;
    ss << "Student|" << User::serialize() << "|"
       << year << "|" << hall << "|" << (isVerified ? "1" : "0");
    return ss.str();
}

Student* Student::deserialize(const string& data) {
    stringstream ss(data);
    string type, id, name, dept, cont, mail, transStr;
    string yearStr, hall, verifiedStr;
    
    getline(ss, type, '|');
    getline(ss, id, '|');
    getline(ss, name, '|');
    getline(ss, dept, '|');
    getline(ss, cont, '|');
    getline(ss, mail, '|');
    getline(ss, transStr, '|');
    getline(ss, yearStr, '|');
    getline(ss, hall, '|');
    getline(ss, verifiedStr, '|');
    
    int trans = stoi(transStr);
    int year = stoi(yearStr);
    bool verified = (verifiedStr == "1");
    
    return new Student(id, name, dept, cont, mail, trans, year, hall, verified);
}