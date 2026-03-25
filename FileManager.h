#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "User.h"
#include "Student.h"
#include "Item.h"
#include "Request.h"
#include "DataStore.h"
#include "ShareSphereException.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class FileManager;

struct WaitlistEntry
{
    string itemID;
    string studentID;
    string dateAdded;
};

class FileManager
{
private:
    string usersFile;
    string itemsFile;
    string requestsFile;
    string waitlistFile;
    vector<WaitlistEntry> waitlist;

    // template class used (DataStore<T> manages typed collections)
    DataStore<User> users;
    DataStore<Item> items;
    DataStore<Request> requests;
    User *currentUser;

    // Helper methods
    void createFileIfNotExists(const std::string &filename);
    void loadAllData();
    bool saveAllData();
    void loadWaitlist();
    void saveWaitlist();

public:
    FileManager();
    FileManager(std::string users, std::string items, std::string requests);
    ~FileManager();

    // Core data management
    bool loadAllDataFromFiles();
    bool saveAllDataToFiles();

    // User management
    bool addUser(User *user);
    User *findUserById(int id) const;
    User *findUserByStudentId(const std::string &studentId) const;
    vector<User *> getAllUsers() const;
    bool studentIdExists(const std::string &studentId) const;

    // Item management
    bool addItem(const Item &item);
    bool removeItem(int itemId);
    Item *findItemById(int id) const;
    vector<Item *> getAllItems() const;
    vector<Item *> searchAvailableItems(const std::string &query) const;

    // Request management
    bool addRequest(const Request &request);
    bool deleteRequest(int requestId);
    Request *findRequestById(int id) const;
    vector<Request *> getAllRequests() const;
    vector<Request *> getPendingRequestsForUser(int userId);
    vector<Request *> getApprovedRequestsForUser(int userId);
    bool updateRequestStatus(int requestId, const std::string &newStatus);

    // Waitlist management
    bool addToWaitlist(const std::string &itemID, const std::string &studentID);
    bool removeFromWaitlist(const std::string &itemID, const std::string &studentID);
    vector<WaitlistEntry> getWaitlistForItem(const std::string &itemID) const;
    bool isOnWaitlist(const std::string &itemID, const std::string &studentID) const;
    string promoteFromWaitlist(const std::string &itemID);
    int getWaitlistCount(const std::string &itemID) const;

    // Authentication
    bool login(const std::string &studentId, const std::string &pin);
    void logout();
    User *getCurrentUser() const;

    // Auto-increment IDs
    int getNextUserId() const;
    int getNextItemId() const;
    int getNextRequestId() const;

    // Statistics
    int countUsers() const;
    int countItems() const;
    int countRequests() const;

};

#endif