#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <vector>
using namespace std;

class Validator {
private:
    static const vector<string> VALID_DEPTS;
    
public:
    static bool isValidStudentID(const string& id);
    static bool isValidUniversityEmail(const string& email);
    static bool isValidPhone(const string& phone);
    static bool isValidDepartment(const string& dept);
    static bool isValidYear(int year);
    
    static vector<string> getDepartmentList();
    static bool isValidEmail(const string& email);
};

#endif