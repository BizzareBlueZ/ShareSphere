#include "Validator.h"
#include <cctype>
using namespace std;

const vector<string> Validator::VALID_DEPTS = {
    "CSE", "EEE", "MPE", "CEE", "TVE", "BTM"
};

const vector<string> Validator::VALID_HALLS = {
    "North","South", "Female"
};

bool Validator::isValidStudentID(const string& id) {
    if(id.length() != 9) return false;
    
    for(char c : id) {
        if(!isdigit(c)) return false;
    }
    
    // id[0]id[1]:21-24
    string yearStr = id.substr(0, 2);
    int year = stoi(yearStr);
    if(year < 21 || year > 24) return false;
    
    // id[2]id[3]: 00
    if(id.substr(2, 2) != "00") return false;
    
    // id[4]: 1-6
    char deptCode = id[4];
    if(deptCode < '1' || deptCode > '6') return false;
    
    // id[5]: program type
    char programType = id[5];
    
    // If deptcode is 1 or 4, id[5] can be 1 or 2
    if(deptCode == '1' || deptCode == '4') {
        if(programType == '1' || programType == '2') return true;
        else return false;
    }
    // Else id[5] must be 1
    else {
        if(programType != '1') return false;
    }
    
    // id[6]: section
    char specCode = id[6];
    
    if(programType == '1') {
        if(deptCode == '2' || deptCode == '5') {
            if(specCode < '1' || specCode > '3') return false;
        }
        else {
            if(specCode != '1' && specCode != '2') return false;
        }
    }
    else if(programType == '2') {
        if(specCode != '1') return false;
    }
    
    return true;
}

bool Validator::isValidEmail(const string& email) {
    if(email.empty()) return true;
    if(email.find("@") == string::npos) return false;
    if(email.find(".edu") == string::npos) return false;
    return true;
}

bool Validator::isValidPhone(const string& phone) {
    if(phone.length() != 11) return false;
    if(phone.substr(0, 2) != "01") return false;
    
    for(char c : phone) {
        if(!isdigit(c)) return false;
    }
    
    return true;
}

bool Validator::isValidDepartment(const string& dept) {
    for(const string& validDept : VALID_DEPTS) {
        if(dept == validDept) return true;
    }
    return false;
}

bool Validator::isValidHall(const string& hall) {
    for(const string& validHall : VALID_HALLS) {
        if(hall == validHall) return true;
    }
    return false;
}

bool Validator::isValidYear(int year) {
    return year >= 1 && year <= 4;
}

vector<string> Validator::getDepartmentList() {
    return VALID_DEPTS;
}

vector<string> Validator::getHallList() {
    return VALID_HALLS;
}