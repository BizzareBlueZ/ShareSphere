#include "Student.h"
#include <sstream>
using namespace std;

Student::Student(string id, string p, string name, string dept,
                 string cont, string mail, int trans, int yr,
                 bool verified)
    : User(id, p, name, dept, cont, mail, trans)
{
    year = yr;
    isVerified = verified;
}

string Student::getDetails() const
{
    stringstream ss;
    ss << User::getDetails() << "\n"
       << "Year: " << year << "\n"
       << "Verification: " << (isVerified ? "Verified (Email provided)" : "Not verified");
    return ss.str();
}

// === VIRTUAL FUNCTION OVERRIDE: Student-specific display ===
void Student::display(ostream &os) const
{
    os << "[" << getUserType() << "] " << getFullName() << " (ID: " << getID() << ")\n"
       << "  Department: " << getDepartment() << " | Year: " << year << "\n"
       << "  Verified: " << (isVerified ? "Yes" : "No")
       << " | Contact: " << getContact()
       << " | Email: " << (getEmail().empty() ? "N/A" : getEmail());
}

string Student::serialize() const
{
    stringstream ss;
    ss << "Student|" << User::serialize() << "|"
       << year << "|" << (isVerified ? "1" : "0");
    return ss.str();
}

Student *Student::deserialize(const string &data)
{
    stringstream ss(data);
    string type, id, pin, name, dept, cont, mail, transStr;
    string yearStr, verifiedStr;

    getline(ss, type, '|');
    getline(ss, id, '|');
    getline(ss, pin, '|');
    getline(ss, name, '|');
    getline(ss, dept, '|');
    getline(ss, cont, '|');
    getline(ss, mail, '|');
    getline(ss, transStr, '|');
    getline(ss, yearStr, '|');
    getline(ss, verifiedStr, '|');

    int trans = stoi(transStr);
    int year = stoi(yearStr);
    bool verified = (verifiedStr == "1");

    return new Student(id, pin, name, dept, cont, mail, trans, year, verified);
}