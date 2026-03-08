#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
using namespace std;

class User
{
protected:
    string userID;
    string pin;
    string fullName;
    string department;
    string contact;
    string email;
    int totalTransactions;

public:
    User(string id = "", string p = "", string name = "",
         string dept = "", string cont = "", string mail = "", int trans = 0);

    virtual ~User() {}

    string getPIN() const { return pin; }
    string getID() const { return userID; }
    string getFullName() const { return fullName; }
    string getDepartment() const { return department; }
    string getContact() const { return contact; }
    string getEmail() const { return email; }
    int getTotalTransactions() const { return totalTransactions; }

    void setPIN(string newPIN) { pin = newPIN; }
    void updateContact(string newContact) { contact = newContact; }
    void updateEmail(string newEmail) { email = newEmail; }
    void incrementTransactions() { totalTransactions++; }

    bool checkPIN(string inputPIN) const { return pin == inputPIN; }

    // === PURE VIRTUAL FUNCTION === (makes User an ABSTRACT CLASS - cannot be instantiated)
    virtual string getUserType() const = 0;

    // === VIRTUAL FUNCTIONS === (can be overridden by derived classes like Student)
    virtual string getDetails() const;
    virtual void display(ostream &os) const;

    virtual string serialize() const;
    static User *deserialize(const string &data);

    // === FRIEND FUNCTION === (operator<< can access private/protected members)
    // === STREAMS === (overloaded stream insertion operator)
    friend ostream &operator<<(ostream &os, const User &user);
};

#endif