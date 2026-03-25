#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector> 
#include <iostream>
#include "User.h"
#include "Item.h"
class FileManager;

using namespace std;

enum class RequestStatus// Represents the current status of a borrow request
{
    PENDING,
    APPROVED,
    REJECTED,
    COMPLETED,
    CANCELLED
};

class Request
{
private:
    string requestID;
    string borrowerID;
    string itemID;
    string duration;
    RequestStatus status;
    string requestDate;
    string notes;

public:
    Request(string reqID = "", string borrower = "", string item = "",
            string dur = "", RequestStatus stat = RequestStatus::PENDING,
            string date = "", string note = "");

    string getRequestID() const { return requestID; }
    string getID() const { return requestID; } // Alias for template compatibility
    string getBorrowerID() const { return borrowerID; }
    string getItemID() const { return itemID; }
    string getDuration() const { return duration; }

    RequestStatus getStatus() const { return status; }
    string getStatusString() const;
    string getRequestDate() const { return requestDate; }
    string getNotes() const { return notes; }

    void approve();
    void reject();
    void complete();
    void cancel();
    void updateNotes(string newNotes);

    bool isPending() const;
    bool isActive() const;
    bool isApproved() const;

    bool canViewContact(std::string viewerID) const;
    string getContactForViewer(std::string viewerID, const vector<User *> &allUsers) const;
    string getEmailForViewer(std::string viewerID, const vector<User *> &allUsers) const;

    string getDetailsForBorrower(const vector<User *> &users, const std::vector<Item *> &items) const;
    string getDetailsForLender(const vector<User *> &users, const std::vector<Item *> &items) const;

    string serialize() const;
    static Request *deserialize(const string &data);

    static string generateRequestID(int count);

    friend ostream &operator<<(ostream &os, const Request &req);
};

#endif