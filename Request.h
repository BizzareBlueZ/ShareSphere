#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector> // ADD THIS
#include <iostream>
#include "User.h"
#include "Item.h"
class FileManager;

enum class RequestStatus
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
    std::string requestID;
    std::string borrowerID;
    std::string itemID;
    std::string duration;
    RequestStatus status;
    std::string requestDate;
    std::string notes;

public:
    Request(std::string reqID = "", std::string borrower = "", std::string item = "",
            std::string dur = "", RequestStatus stat = RequestStatus::PENDING,
            std::string date = "", std::string note = "");

    std::string getRequestID() const { return requestID; }
    std::string getID() const { return requestID; } // Alias for template compatibility
    std::string getBorrowerID() const { return borrowerID; }
    std::string getItemID() const { return itemID; }
    std::string getDuration() const { return duration; }
    RequestStatus getStatus() const { return status; }
    std::string getStatusString() const;
    std::string getRequestDate() const { return requestDate; }
    std::string getNotes() const { return notes; }

    void approve();
    void reject();
    void complete();
    void cancel();
    void updateNotes(std::string newNotes);

    bool isPending() const;
    bool isActive() const;
    bool isApproved() const;

    bool canViewContact(std::string viewerID) const;
    std::string getContactForViewer(std::string viewerID, const std::vector<User *> &allUsers) const;
    std::string getEmailForViewer(std::string viewerID, const std::vector<User *> &allUsers) const;

    std::string getDetailsForBorrower(const std::vector<User *> &users, const std::vector<Item *> &items) const;
    std::string getDetailsForLender(const std::vector<User *> &users, const std::vector<Item *> &items) const;

    std::string serialize() const;
    static Request *deserialize(const std::string &data);

    static std::string generateRequestID(int count);

    // === FRIEND FUNCTION + STREAMS === (operator<< for Request)
    friend std::ostream &operator<<(std::ostream &os, const Request &req);
};

#endif