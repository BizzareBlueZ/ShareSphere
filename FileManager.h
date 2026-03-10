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

class FileManager;

struct WaitlistEntry
{
    std::string itemID;
    std::string studentID;
    std::string dateAdded;
};

class FileManager
{
private:
    std::string usersFile;
    std::string itemsFile;
    std::string requestsFile;
    std::string waitlistFile;
    std::vector<WaitlistEntry> waitlist;

    // === TEMPLATE CLASS USAGE === (DataStore<T> manages typed collections)
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
    std::vector<User *> getAllUsers() const;
    bool studentIdExists(const std::string &studentId) const;

    // Item management
    bool addItem(const Item &item);
    bool removeItem(int itemId);
    Item *findItemById(int id) const;
    std::vector<Item *> getAllItems() const;
    std::vector<Item *> searchAvailableItems(const std::string &query) const;

    // Request management
    bool addRequest(const Request &request);
    bool deleteRequest(int requestId);
    Request *findRequestById(int id) const;
    std::vector<Request *> getAllRequests() const;
    std::vector<Request *> getPendingRequestsForUser(int userId);
    std::vector<Request *> getApprovedRequestsForUser(int userId);
    bool updateRequestStatus(int requestId, const std::string &newStatus);

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

    // Waitlist
    bool addToWaitlist(const std::string &itemID, const std::string &studentID);
    bool removeFromWaitlist(const std::string &itemID, const std::string &studentID);
    std::vector<WaitlistEntry> getWaitlistForItem(const std::string &itemID) const;
    bool isOnWaitlist(const std::string &itemID, const std::string &studentID) const;
    std::string promoteFromWaitlist(const std::string &itemID);
    int getWaitlistCount(const std::string &itemID) const;
};

#endif