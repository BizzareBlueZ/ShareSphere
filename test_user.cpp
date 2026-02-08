#include <iostream>
#include "Validator.h"
#include "Student.h"
using namespace std;

Student *createStudentWithValidation()
{
    string id, name, dept, phone;
    int year;
    string hall;

    string email = "";
    bool verified = false;

    cout << "\n=== Create Student Profile ===" << endl;

    while (true)
    {
        cout << "Student ID (9 digits): ";
        cin >> id;
        if (Validator::isValidStudentID(id))
            break;
        cout << "Error: Student ID must be exactly 9 digits.\n";
    }

    cout << "Full Name: ";
    cin.ignore();
    getline(cin, name);

    while (true)
    {
        cout << "Department (";
        vector<string> depts = Validator::getDepartmentList();
        for (size_t i = 0; i < depts.size(); i++)
        {
            cout << depts[i];
            if (i < depts.size() - 1)
                cout << ", ";
        }
        cout << "): ";
        cin >> dept;
        if (Validator::isValidDepartment(dept))
            break;
        cout << "Error: Invalid department.\n";
    }

    while (true)
    {
        cout << "Phone (11 digits, starts with 01): ";
        cin >> phone;
        if (Validator::isValidPhone(phone))
            break;
        cout << "Error: Phone must be 11 digits starting with 01.\n";
    }

        cout << "Do you want to provide email for verification? (y/n): ";
        char emailChoice;
        cin >> emailChoice;
        cin.ignore();

        if (emailChoice == 'y' || emailChoice == 'Y')
        {while(true){
            cout << "Enter email for verification: ";
            getline(cin, email);

            if (Validator::isValidEmail(email))
            {
                verified = true;
                break;
            }
            else
                cout << "Invalid email format. Use name@domain.edu\n";}
        }

        while (true)
        {
            cout << "Year (1, 2, 3, or 4): ";
            cin >> year;
            if (Validator::isValidYear(year))
                break;
            cout << "Error: Year must be 1, 2, 3, or 4.\n";
        }

        while (true)
        {
            cout << "Hall (";
            vector<string> halls = Validator::getHallList();
            for (size_t i = 0; i < halls.size(); i++)
            {
                cout << halls[i];
                if (i < halls.size() - 1)
                    cout << ", ";
            }
            cout << "): ";
            cin >> hall;
            if (Validator::isValidHall(hall))
                break;
            cout << "Error: Invalid hall name.\n";
        }

        return new Student(id, name, dept, phone, email, 0, year, hall, verified);
    }

    void testSerialization(Student * student)
    {
        cout << "\n=== Testing Serialization ===" << endl;
        string serialized = student->serialize();
        cout << "Serialized: " << serialized << endl;

        Student *restored = Student::deserialize(serialized);
        cout << "\nRestored Student Details:" << endl;
        cout << restored->getDetails() << endl;

        delete restored;
    }

    int main()
    {
        cout << "ShareSphere - Day 2: User System with Validation" << endl;

        Student *student1 = createStudentWithValidation();

        cout << "\n=== Student Created ===" << endl;
        cout << student1->getDetails() << endl;

        testSerialization(student1);

        cout << "\n=== Testing Polymorphism ===" << endl;
        User *user = student1;
        cout << "User Type: " << user->getUserType() << endl;
        cout << "User Details:\n"
             << user->getDetails() << endl;

        delete student1;
        return 0;
    }