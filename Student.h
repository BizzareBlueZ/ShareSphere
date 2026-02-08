#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"

class Student : public User {
private:
    int year;
    string hall;
    bool isVerified;
    
public:
    Student(string id = "", string name = "", string dept = "",
            string cont = "", string mail = "", int trans = 0,
            int yr = 1, string hall = "", bool verified = false);
    
    int getYear() const { return year; }
    string gethall() const { return hall; }
    bool getVerified() const { return isVerified; }
    
    void updateYear(int newYear) { year = newYear; }
    void updatehall(string newhall) { hall = newhall; }
    void verifyStudent(bool status) { isVerified = status; }
    
    string getUserType() const override { return "Student"; }
    string getDetails() const override;
    
    string serialize() const override;
    static Student* deserialize(const string& data);
};

#endif