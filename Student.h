#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"

class Student : public User // inherits from User, so it has all the basic user info plus student-specific info like year and verification status
{
private:
    int year;
    bool isVerified;

public:
    Student(string id = "", string p = "", string name = "", string dept = "",
            string cont = "", string mail = "", int trans = 0,
            int yr = 1, bool verified = false);

    int getYear() const { return year; }
    bool getVerified() const { return isVerified; }

    void updateYear(int newYear) { year = newYear; }
    void verifyStudent(bool status) { isVerified = status; }

    string getUserType() const override { return "Student"; }
    string getDetails() const override;
    void display(ostream &os) const override;

    string serialize() const override;
    static Student *deserialize(const string &data);
};

#endif