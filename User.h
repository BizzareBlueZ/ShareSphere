#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:
    string userID;
    string fullName;
    string department;
    string contact;
    string email;
    int totalTransactions;
    
public:
    User(string id = "", string name = "", string dept = "",
         string cont = "", string mail = "", int trans = 0);
    
    virtual ~User() {}
    
    string getID() const { return userID; }
    string getFullName() const { return fullName; }
    string getDepartment() const { return department; }
    string getContact() const { return contact; }
    string getEmail() const { return email; }
    int getTotalTransactions() const { return totalTransactions; }
    
    void updateContact(string newContact) { contact = newContact; }
    void updateEmail(string newEmail) { email = newEmail; }
    void incrementTransactions() { totalTransactions++; }
    
    virtual string getUserType() const = 0;
    virtual string getDetails() const;
    
    virtual string serialize() const;
    static User* deserialize(const string& data);
};

#endif