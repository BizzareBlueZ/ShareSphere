#include "FileManager.h"
#include "ShareSphereException.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

//create file if it doesn't exist, used in constructor to ensure all files are present before loading/saving
FileManager::FileManager() : currentUser(nullptr)
{
    usersFile = "users.txt";
    itemsFile = "items.txt";
    requestsFile = "requests.txt";
    waitlistFile = "waitlist.txt";

    createFileIfNotExists(usersFile);
    createFileIfNotExists(itemsFile);
    createFileIfNotExists(requestsFile);
    createFileIfNotExists(waitlistFile);

    loadAllData();
    loadWaitlist();
}

// Overloaded constructor to specify custom file paths (useful for testing)
FileManager::FileManager(string usrs, string itms, string reqs)
    : usersFile(usrs), itemsFile(itms), requestsFile(reqs), currentUser(nullptr)
{
    waitlistFile = "waitlist.txt";

    createFileIfNotExists(usersFile);
    createFileIfNotExists(itemsFile);
    createFileIfNotExists(requestsFile);
    createFileIfNotExists(waitlistFile);

    loadAllData();
    loadWaitlist();
}

FileManager::~FileManager()
{
    
    try
    {
        saveAllData();
    }
    catch (const ShareSphereException &e)
    {
        cerr << "Warning: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Warning: Failed to save data on exit." << endl;
    }
    currentUser = nullptr;
    // DataStore destructors automatically clean up all pointers
}


void FileManager::createFileIfNotExists(const string &filename)
{
    ifstream testFile(filename);
    if (!testFile.good())
    {
        ofstream newFile(filename);
        newFile.close();
    }
}

void FileManager::loadAllData()//reading from the data files, using ifstream and deserialization methods from User, Item, and Request classes
{
    // Clear existing data (DataStore::clear() handles memory deallocation)
    users.clear();
    items.clear();
    requests.clear();
    currentUser = nullptr;

    // EXCEPTION HANDLING + STREAMS: File loading with error handlings
    // Load users using User::deserialize()
    ifstream userFile(usersFile);
    if (!userFile.is_open())
    {
        throw FileException("Cannot open file: " + usersFile);
    }
    string line;
    while (getline(userFile, line))
    {
        if (!line.empty())
        {
            try
            {
                User *user = User::deserialize(line);
                if (user)
                    users.add(user); // Using DataStore<User>::add()
            }
            catch (const exception &e)
            {
                cerr << "Warning: Skipping malformed user record." << endl;
            }
        }
    }
    userFile.close();

    // Load items using Item::fromString()
    ifstream itemFile(itemsFile);
    if (!itemFile.is_open())
    {
        throw FileException("Cannot open file: " + itemsFile);
    }
    while (getline(itemFile, line))
    {
        if (!line.empty())
        {
            try
            {
                Item itemObj = Item::fromString(line);
                items.add(new Item(itemObj)); // Using DataStore<Item>::add()
            }
            catch (const exception &e)
            {
                cerr << "Warning: Skipping malformed item record." << endl;
            }
        }
    }
    itemFile.close();

    // Load requests using Request::deserialize()
    ifstream requestFile(requestsFile);
    if (!requestFile.is_open())
    {
        throw FileException("Cannot open file: " + requestsFile);
    }
    while (getline(requestFile, line))
    {
        if (!line.empty())
        {
            try
            {
                Request *req = Request::deserialize(line);
                if (req && req->getRequestID() != "")
                {
                    requests.add(req); // Using DataStore<Request>::add()
                }
            }
            catch (const exception &e)
            {
                cerr << "Warning: Skipping malformed request record." << endl;
            }
        }
    }
    requestFile.close();
}

bool FileManager::saveAllData()//writing to the data files, using ofstream and serialization methods from User, Item, and Request classes
{
    // STREAMS: Using ofstream for file output 
    // Save users
    ofstream userFile(usersFile);
    if (!userFile.is_open())
    {
        throw FileException("Cannot write to file: " + usersFile);
    }
    for (User *user : users) // Range-based for using DataStore iterators
    {
        userFile << user->serialize() << endl;
    }
    userFile.close();

    // Save items
    ofstream itemFile(itemsFile);
    if (!itemFile.is_open())
    {
        throw FileException("Cannot write to file: " + itemsFile);
    }
    for (Item *item : items)
    {
        itemFile << item->toFileString() << endl;
    }
    itemFile.close();

    // Save requests
    ofstream requestFile(requestsFile);
    if (!requestFile.is_open())
    {
        throw FileException("Cannot write to file: " + requestsFile);
    }
    for (Request *req : requests)
    {
        requestFile << req->serialize() << endl;
    }
    requestFile.close();

    return true;
}

// ==================== USER MANAGEMENT ====================

bool FileManager::addUser(User *user)
{
    users.add(user); // DataStore::add()

    // Append to file using ofstream
    ofstream file(usersFile, ios::app);
    if (!file.is_open())
    {
        throw FileException("Cannot append to file: " + usersFile);
    }
    file << user->serialize() << endl;
    file.close();

    return true;
}

User *FileManager::findUserById(int id) const
{
    // Using DataStore::findById() with string conversion
    return users.findById(to_string(id));
}

User *FileManager::findUserByStudentId(const string &studentId) const
{
    return users.findById(studentId); // DataStore::findById()
}

vector<User *> FileManager::getAllUsers() const
{
    return users.getAll(); // DataStore::getAll()
}

bool FileManager::studentIdExists(const string &studentId) const
{
    return users.findById(studentId) != nullptr;
}

// ==================== ITEM MANAGEMENT ====================

bool FileManager::addItem(const Item &item)
{
    items.add(new Item(item)); // DataStore::add()

    // Append to file
    ofstream file(itemsFile, ios::app);
    if (!file.is_open())
    {
        throw FileException("Cannot append to file: " + itemsFile);
    }
    file << item.toFileString() << endl;
    file.close();

    return true;
}

bool FileManager::removeItem(int itemId)
{
    // Using DataStore::removeById()
    bool removed = items.removeById(to_string(itemId));

    if (removed)
    {
        // Rewrite entire file
        ofstream file(itemsFile);
        if (!file.is_open())
        {
            throw FileException("Cannot write to file: " + itemsFile);
        }
        for (Item *item : items) // DataStore iterators
        {
            file << item->toFileString() << endl;
        }
        file.close();
    }

    return removed;
}

Item *FileManager::findItemById(int id) const
{
    return items.findById(to_string(id)); // DataStore::findById()
}

vector<Item *> FileManager::getAllItems() const
{
    return items.getAll(); // DataStore::getAll()
}

vector<Item *> FileManager::searchAvailableItems(const string &query) const
{
    vector<Item *> results;

    for (Item *item : items) // DataStore iterators
    {
        if (item->getAvailable() == false)
            continue;

        if (item->matchesSearch(query))
        {
            results.push_back(item);
        }
    }
    return results;
}

// ==================== REQUEST MANAGEMENT ====================

bool FileManager::addRequest(const Request &request)
{
    requests.add(new Request(request)); // DataStore::add()

    // Append to file
    ofstream file(requestsFile, ios::app);
    if (!file.is_open())
    {
        throw FileException("Cannot append to file: " + requestsFile);
    }
    file << request.serialize() << endl;
    file.close();

    return true;
}

bool FileManager::deleteRequest(int requestId)
{
    // Using DataStore::removeById()
    bool removed = requests.removeById(to_string(requestId));

    if (removed)
    {
        // Rewrite entire file
        ofstream file(requestsFile);
        if (!file.is_open())
        {
            throw FileException("Cannot write to file: " + requestsFile);
        }
        for (Request *req : requests) // DataStore iterators
        {
            file << req->serialize() << endl;
        }
        file.close();
    }

    return removed;
}

Request *FileManager::findRequestById(int id) const
{
    return requests.findById(to_string(id)); // DataStore::findById()
}

vector<Request *> FileManager::getAllRequests() const
{
    return requests.getAll(); // DataStore::getAll()
}

vector<Request *> FileManager::getPendingRequestsForUser(int userId)
{
    vector<Request *> results;
    for (Request *req : requests) // DataStore iterators
    {
        if (req->getStatusString() == "PENDING")
        {
            Item *item = findItemById(stoi(req->getItemID()));
            if (item && stoi(item->getOwnerID()) == userId)
            {
                results.push_back(req);
            }
        }
    }
    return results;
}

vector<Request *> FileManager::getApprovedRequestsForUser(int userId)
{
    vector<Request *> results;
    for (Request *req : requests) // DataStore iterators
    {
        if (req->getStatusString() == "APPROVED")
        {
            if (stoi(req->getBorrowerID()) == userId)
            {
                results.push_back(req);
            }
            else
            {
                Item *item = findItemById(stoi(req->getItemID()));
                if (item && stoi(item->getOwnerID()) == userId)
                {
                    results.push_back(req);
                }
            }
        }
    }
    return results;
}

bool FileManager::updateRequestStatus(int requestId, const string &newStatus)
{
    Request *req = findRequestById(requestId);
    if (!req)
    {
        throw NotFoundException("Request with ID " + to_string(requestId) + " not found");
    }

    if (newStatus == "APPROVED")
    {
        req->approve();
    }
    else if (newStatus == "REJECTED")
    {
        req->reject();
    }

    // Rewrite entire file
    ofstream file(requestsFile);
    if (!file.is_open())
    {
        throw FileException("Cannot write to file: " + requestsFile);
    }
    for (Request *r : requests)
    {
        file << r->serialize() << endl;
    }
    file.close();

    return true;
}

// ==================== AUTHENTICATION ====================

bool FileManager::login(const string &studentId, const string &pin)
{
    User *user = findUserByStudentId(studentId);
    if (user && user->getPIN() == pin)
    {
        currentUser = user;
        return true;
    }
    return false;
}

void FileManager::logout()
{
    currentUser = nullptr;
}

User *FileManager::getCurrentUser() const
{
    return currentUser;
}






//auto incrementation is here
// ==================== AUTO-INCREMENT IDs ====================
// Using DataStore::getMaxId() template method

int FileManager::getNextUserId() const
{
    return users.getMaxId() + 1;
}

int FileManager::getNextItemId() const
{
    return items.getMaxId() + 1;
}

int FileManager::getNextRequestId() const
{
    return requests.getMaxId() + 1;
}

// ==================== STATISTICS ====================

int FileManager::countUsers() const
{
    return users.count(); // DataStore::count()
}

int FileManager::countItems() const
{
    return items.count();
}

int FileManager::countRequests() const
{
    return requests.count();
}

bool FileManager::loadAllDataFromFiles()
{
    loadAllData();
    return true;
}

bool FileManager::saveAllDataToFiles()
{
    return saveAllData();
}



// ==================== WAITLIST MANAGEMENT ====================

void FileManager::loadWaitlist()
{
    waitlist.clear();
    ifstream file(waitlistFile);
    if (!file.is_open())
        return;
    string line;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            stringstream ss(line);
            WaitlistEntry entry;
            getline(ss, entry.itemID, '|');
            getline(ss, entry.studentID, '|');
            getline(ss, entry.dateAdded, '|');
            if (!entry.itemID.empty() && !entry.studentID.empty())
                waitlist.push_back(entry);
        }
    }
    file.close();
}

void FileManager::saveWaitlist()
{
    ofstream file(waitlistFile);
    if (!file.is_open())
        return;
    for (const auto &entry : waitlist)
    {
        file << entry.itemID << "|" << entry.studentID << "|" << entry.dateAdded << endl;
    }
    file.close();
}

bool FileManager::addToWaitlist(const string &itemID, const string &studentID)
{
    if (isOnWaitlist(itemID, studentID))
        return false;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream date_ss;
    date_ss << (1900 + ltm->tm_year) << "-"
            << setw(2) << setfill('0') << (1 + ltm->tm_mon) << "-"
            << setw(2) << setfill('0') << ltm->tm_mday;

    WaitlistEntry entry{itemID, studentID, date_ss.str()};
    waitlist.push_back(entry);
    saveWaitlist();
    return true;
}

bool FileManager::removeFromWaitlist(const string &itemID, const string &studentID)
{
    for (auto it = waitlist.begin(); it != waitlist.end(); ++it)
    {
        if (it->itemID == itemID && it->studentID == studentID)
        {
            waitlist.erase(it);
            saveWaitlist();
            return true;
        }
    }
    return false;
}

vector<WaitlistEntry> FileManager::getWaitlistForItem(const string &itemID) const
{
    vector<WaitlistEntry> result;
    for (const auto &entry : waitlist)
    {
        if (entry.itemID == itemID)
            result.push_back(entry);
    }
    return result;
}

bool FileManager::isOnWaitlist(const string &itemID, const string &studentID) const
{
    for (const auto &entry : waitlist)
    {
        if (entry.itemID == itemID && entry.studentID == studentID)
            return true;
    }
    return false;
}

string FileManager::promoteFromWaitlist(const string &itemID)
{
    for (auto it = waitlist.begin(); it != waitlist.end(); ++it)
    {
        if (it->itemID == itemID)
        {
            string studentID = it->studentID;
            waitlist.erase(it);
            saveWaitlist();
            return studentID;
        }
    }
    return "";
}

int FileManager::getWaitlistCount(const string &itemID) const
{
    int count = 0;
    for (const auto &entry : waitlist)
    {
        if (entry.itemID == itemID)
            count++;
    }
    return count;
}