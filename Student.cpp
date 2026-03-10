#include "Student.h"
#include <sstream>
#include <iomanip>
using namespace std;

Student::Student(string id, string p, string name, string dept,
                 string cont, string mail, int trans, int yr,
                 bool verified, double trust)
    : User(id, p, name, dept, cont, mail, trans, trust)
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
       << " | Email: " << (getEmail().empty() ? "N/A" : getEmail())
       << "\n  Trust: " << fixed << setprecision(1) << getTrustScore() << " (" << getTrustLevel() << ")";
}

string Student::serialize() const
{
    stringstream ss;
    ss << "Student|" << User::serialize() << "|"
       << year << "|" << (isVerified ? "1" : "0") << "|"
       << fixed << setprecision(1) << getTrustScore();
    return ss.str();
}

Student *Student::deserialize(const string &data)
{
    stringstream ss(data);
    string type, id, pin, name, dept, cont, mail, transStr;
    string yearStr, verifiedStr, trustStr;

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
    getline(ss, trustStr, '|');

    int trans = stoi(transStr);
    int year = stoi(yearStr);
    bool verified = (verifiedStr == "1");
    double trust = 50.0;
    if (!trustStr.empty())
    {
        try
        {
            trust = stod(trustStr);
        }
        catch (...)
        {
            trust = 50.0;
        }
    }

    return new Student(id, pin, name, dept, cont, mail, trans, year, verified, trust);
}