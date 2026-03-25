#include "SystemManager.h"
#include "FileManager.h"
#include "Validator.h"
#include "ShareSphereException.h"
#include "Item.h"
#include "User.h"
#include "Student.h"
#include "Request.h"
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <algorithm>


//cli part not used for gui ...basically assimilation working part

using namespace std;

namespace
{
    const int BOX_INNER_WIDTH = 58;

    string fitToBox(const string &text)
    {
        if (static_cast<int>(text.size()) >= BOX_INNER_WIDTH)
        {
            return text.substr(0, BOX_INNER_WIDTH);
        }
        return text + string(BOX_INNER_WIDTH - text.size(), ' ');
    }

    string centerToBox(const string &text)
    {
        if (static_cast<int>(text.size()) >= BOX_INNER_WIDTH)
        {
            return text.substr(0, BOX_INNER_WIDTH);
        }
        int totalPad = BOX_INNER_WIDTH - static_cast<int>(text.size());
        int leftPad = totalPad / 2;
        int rightPad = totalPad - leftPad;
        return string(leftPad, ' ') + text + string(rightPad, ' ');
    }

    void printBoxBorder(char fill = '=')
    {
        cout << "  +" << string(BOX_INNER_WIDTH, fill) << "+\n";
    }

    void printBoxLine(const string &text)
    {
        cout << "  |" << fitToBox(text) << "|\n";
    }

    void printBoxCentered(const string &text)
    {
        cout << "  |" << centerToBox(text) << "|\n";
    }
}

// Constructor
SystemManager::SystemManager() : fileManager(), currentUser(nullptr)
{
    srand(time(0));
}

// Main program loop
void SystemManager::run()
{
    cout << "\n";
    printBoxBorder('=');
    printBoxCentered("");
    printBoxCentered("S H A R E S P H E R E");
    printBoxCentered("Student Item Sharing Platform");
    printBoxCentered("");
    printBoxCentered("Share, Borrow and Connect with Peers");
    printBoxBorder('=');
    cout << "\n  System loaded: " << fileManager.countUsers() << " users, "
         << fileManager.countItems() << " items, "
         << fileManager.countRequests() << " active requests.\n";

    while (true)
    {
        // === EXCEPTION HANDLING: Catch errors during main loop ===
        try
        {
            if (!showLoginWindow())
            {
                break; // User chose to exit program
            }

            // User logged in successfully
            showDashboardWindow();

            // After logout, clear current user
            currentUser = nullptr;
            fileManager.logout();
        }
        catch (const ShareSphereException &e)
        {
            cerr << "\n  [!] Error: " << e.what() << endl;
            cerr << "  Returning to login screen...\n";
            currentUser = nullptr;
            fileManager.logout();
        }
        catch (const exception &e)
        {
            cerr << "\n  [!] Unexpected error: " << e.what() << endl;
            cerr << "  Returning to login screen...\n";
            currentUser = nullptr;
            fileManager.logout();
        }
    }

    cout << "\n  Thank you for using ShareSphere. Goodbye!\n\n";
}

// ==================== WINDOW 1: LOGIN/CREATE ACCOUNT ====================

bool SystemManager::showLoginWindow()
{
    while (true)
    {
        cout << "\n";
        printBoxBorder('-');
        printBoxCentered("LOGIN / REGISTER");
        printBoxBorder('-');
        printBoxLine("  1. Login with Student ID");
        printBoxLine("  2. Create New Student Account");
        printBoxLine("  3. Exit Program");
        printBoxBorder('-');
        cout << "  Enter choice (1-3): ";

        int choice = getMenuChoice(1, 3);

        switch (choice)
        {
        case 1:
            if (handleLogin())
            {
                return true; // Login successful
            }
            break;
        case 2:
            handleCreateAccount();
            break;
        case 3:
            return false; // Exit program
        }
    }
}

bool SystemManager::handleLogin()
{
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS)
    {
        cout << "\n  --- LOGIN ---\n";

        string studentID, pin;

        cout << "  Student ID: ";
        getline(cin, studentID);

        cout << "  PIN (4 digits): ";
        getline(cin, pin);

        // === EXCEPTION HANDLING: Validate input before login attempt ===
        try
        {
            if (studentID.empty() || pin.empty())
            {
                throw InvalidInputException("Student ID and PIN cannot be empty.");
            }

            if (fileManager.login(studentID, pin))
            {
                currentUser = fileManager.getCurrentUser();
                cout << "\n  [OK] Login successful! Welcome, " << currentUser->getFullName() << "!\n";
                return true;
            }
            else
            {
                throw AuthenticationException("Invalid Student ID or PIN.");
            }
        }
        catch (const AuthenticationException &e)
        {
            attempts++;
            cout << "\n  [!] " << e.what() << " (Attempt " << attempts << "/" << MAX_ATTEMPTS << ")\n";

            if (attempts < MAX_ATTEMPTS)
            {
                cout << "  1. Try again\n";
                cout << "  2. Back to main menu\n";
                cout << "  Choice: ";

                int retryChoice = getMenuChoice(1, 2);
                if (retryChoice == 2)
                {
                    return false;
                }
            }
        }
        catch (const InvalidInputException &e)
        {
            attempts++;
            cout << "\n  [!] " << e.what() << " (Attempt " << attempts << "/" << MAX_ATTEMPTS << ")\n";
        }
    }

    cout << "\n  [!] Maximum login attempts reached. Returning to main menu.\n";
    return false;
}

void SystemManager::handleCreateAccount()
{
    cout << "\n";
    printBoxBorder('-');
    printBoxCentered("CREATE STUDENT ACCOUNT");
    printBoxBorder('-');

    string studentID, fullName, dept, phone, email = "";
    int year;
    bool verified = false;

    // 1. Student ID validation
    while (true)
    {
        cout << "  Student ID (9 digits, format 2100XXXXX): ";
        cin >> studentID;
        cin.ignore();

        // === EXCEPTION HANDLING: Input validation ===
        try
        {
            if (!Validator::isValidStudentID(studentID))
            {
                throw InvalidInputException("Invalid Student ID format.");
            }
            if (fileManager.studentIdExists(studentID))
            {
                throw InvalidInputException("This Student ID is already registered.");
            }
            break; // Valid and unique
        }
        catch (const InvalidInputException &e)
        {
            cout << "  [!] " << e.what() << " Try again.\n";
        }
    }

    // 2. Full Name
    cout << "  Full Name: ";
    getline(cin, fullName);

    // 3. Department with validation
    while (true)
    {
        cout << "  Department (";
        vector<string> depts = Validator::getDepartmentList();
        for (size_t i = 0; i < depts.size(); i++)
        {
            cout << depts[i];
            if (i < depts.size() - 1)
                cout << ", ";
        }
        cout << "): ";
        cin >> dept;
        try
        {
            if (!Validator::isValidDepartment(dept))
                throw InvalidInputException("Invalid department.");
            break;
        }
        catch (const InvalidInputException &e)
        {
            cout << "  [!] " << e.what() << "\n";
        }
    }

    // 4. Phone with validation
    while (true)
    {
        cout << "  Phone (11 digits, starts with 01): ";
        cin >> phone;
        try
        {
            if (!Validator::isValidPhone(phone))
                throw InvalidInputException("Phone must be 11 digits starting with 01.");
            break;
        }
        catch (const InvalidInputException &e)
        {
            cout << "  [!] " << e.what() << "\n";
        }
    }

    // 5. Email (optional) with validation
    cout << "  Do you want to provide email for verification? (y/n): ";
    char emailChoice;
    cin >> emailChoice;
    cin.ignore();

    if (emailChoice == 'y' || emailChoice == 'Y')
    {
        while (true)
        {
            cout << "  Email: ";
            getline(cin, email);

            try
            {
                if (!Validator::isValidEmail(email))
                    throw InvalidInputException("Invalid email format. Use name@domain.edu");
                verified = true;
                break;
            }
            catch (const InvalidInputException &e)
            {
                cout << "  [!] " << e.what() << "\n";
            }
        }
    }

    // 6. Year with validation
    while (true)
    {
        cout << "  Year (1, 2, 3, or 4): ";
        cin >> year;
        try
        {
            if (!Validator::isValidYear(year))
                throw InvalidInputException("Year must be 1, 2, 3, or 4.");
            break;
        }
        catch (const InvalidInputException &e)
        {
            cout << "  [!] " << e.what() << "\n";
        }
    }

    cin.ignore();

    // Generate a random 4-digit PIN
    string pin = to_string(1000 + rand() % 9000);

    cout << "\n";
    printBoxBorder('-');
    printBoxLine("  [OK] Account created successfully!");
    printBoxLine("  Your auto-generated PIN: " + pin);
    printBoxLine("  (Please remember this PIN!)");
    printBoxBorder('-');

    // Convert year string from student ID to actual year
    string yearStr = studentID.substr(0, 2);
    int entryYear = stoi(yearStr);
    int currentYear = 2024;
    int studentYear = currentYear - 2000 - entryYear + 1;
    if (studentYear < 1)
        studentYear = 1;
    if (studentYear > 4)
        studentYear = 4;

    // Create Student object
    Student *newStudent = new Student(studentID, pin, fullName, dept,
                                      phone, email, 0, studentYear, verified);

    // Add to system
    try
    {
        if (fileManager.addUser(newStudent))
        {
            if (fileManager.login(studentID, pin))
            {
                currentUser = fileManager.getCurrentUser();
                cout << "  Press Enter to continue to dashboard...";
                cin.ignore();
            }
        }
    }
    catch (const ShareSphereException &e)
    {
        cout << "  [!] " << e.what() << "\n";
        delete newStudent;
    }
}

// ==================== WINDOW 2: DASHBOARD ====================

void SystemManager::showDashboardWindow()
{
    while (true)
    {
        // === EXCEPTION HANDLING: Wrap dashboard operations ===
        try
        {
            Student *stu = dynamic_cast<Student *>(currentUser);
            cout << "\n";
            printBoxBorder('=');
            printBoxCentered("DASHBOARD");
            printBoxBorder('=');
            printBoxLine("  Student: " + currentUser->getFullName());
            printBoxLine("  ID: " + currentUser->getID() + " | " + currentUser->getDepartment() +
                         " Year " + (stu ? to_string(stu->getYear()) : "?"));
            printBoxLine("  Items: " + to_string(fileManager.countItems()) +
                         " | Requests: " + to_string(fileManager.countRequests()));
            printBoxBorder('=');

            // PANEL 1: Received Requests
            displayReceivedRequestsPanel();

            // PANEL 2: Share Area
            displayShareAreaPanel();

            // ACTIONS
            cout << "\n";
            printBoxBorder('-');
            printBoxCentered("ACTIONS");
            printBoxBorder('-');
            printBoxLine("  1. BORROW  - Search and Request an Item");
            printBoxLine("  2. LEND    - Add New Item to Share");
            printBoxLine("  3. REFRESH - Reload Dashboard");
            printBoxLine("  4. PROFILE - View My Profile");
            printBoxLine("  5. MANAGE  - Manage My Items");
            printBoxLine("  6. LOGOUT  - Return to Login");
            printBoxLine("  7. EXIT    - Close Program");
            printBoxBorder('-');
            cout << "  Enter choice (1-7): ";

            int choice = getMenuChoice(1, 7);

            switch (choice)
            {
            case 1:
                handleBorrowAction();
                break;
            case 2:
                handleLendAction();
                break;
            case 3:
                cout << "  Refreshing...\n";
                break;
            case 4:
                displayUserProfile();
                break;
            case 5:
                displayMyItemsPanel();
                break;
            case 6:
                fileManager.logout();
                cout << "\n  [OK] Logged out successfully.\n";
                return;
            case 7:
                cout << "\n  Exiting ShareSphere...\n";
                fileManager.saveAllDataToFiles();
                exit(0);
            }
            // ==================== PANEL: MANAGE MY ITEMS ====================
            void SystemManager::displayMyItemsPanel()
            {
                cout << "\n  --- MY ITEMS ---\n";
                vector<Item *> myItems;
                for (Item *item : fileManager.getAllItems())
                {
                    if (item->getOwnerID() == currentUser->getID())
                    {
                        myItems.push_back(item);
                    }
                }
                if (myItems.empty())
                {
                    cout << "  (You have not added any items.)\n";
                    cout << "  Press Enter to return...";
                    cin.ignore();
                    return;
                }
                while (true)
                {
                    cout << "\n  Your Items:" << endl;
                    for (size_t i = 0; i < myItems.size(); ++i)
                    {
                        cout << "  " << (i + 1) << ". " << myItems[i]->getName() << " | "
                             << myItems[i]->getCategoryString() << " | "
                             << myItems[i]->getDescription() << " | "
                             << (myItems[i]->getAvailable() ? "Available" : "Borrowed") << endl;
                    }
                    cout << "\n  Select item to [E]dit, [D]elete, or [B]ack: ";
                    string input;
                    getline(cin, input);
                    if (input == "B" || input == "b")
                        break;
                    if (input.empty())
                        continue;
                    int idx = -1;
                    char action = 0;
                    if (isdigit(input[0]))
                    {
                        idx = stoi(input) - 1;
                        cout << "  [E]dit or [D]elete this item? ";
                        getline(cin, input);
                        if (!input.empty())
                            action = toupper(input[0]);
                    }
                    else
                    {
                        continue;
                    }
                    if (idx < 0 || idx >= (int)myItems.size())
                    {
                        cout << "  [!] Invalid item number.\n";
                        continue;
                    }
                    Item *item = myItems[idx];
                    if (action == 'E')
                    {
                        cout << "  New name (leave blank to keep): ";
                        string newName;
                        getline(cin, newName);
                        if (!newName.empty())
                            item->setName(newName);
                        cout << "  New description (leave blank to keep): ";
                        string newDesc;
                        getline(cin, newDesc);
                        if (!newDesc.empty())
                            item->setDescription(newDesc);
                        fileManager.saveAllDataToFiles();
                        cout << "  [OK] Item updated.\n";
                    }
                    else if (action == 'D')
                    {
                        cout << "  Are you sure you want to delete this item? (y/n): ";
                        string confirm;
                        getline(cin, confirm);
                        if (confirm == "y" || confirm == "Y")
                        {
                            fileManager.removeItem(stoi(item->getID()));
                            fileManager.saveAllDataToFiles();
                            myItems.erase(myItems.begin() + idx);
                            cout << "  [OK] Item deleted.\n";
                            if (myItems.empty())
                            {
                                cout << "  (No more items.)\n";
                                break;
                            }
                        }
                    }
                    else
                    {
                        cout << "  [!] Invalid action.\n";
                    }
                }
                cout << "  Press Enter to return...";
                cin.ignore();
            }
        }
        catch (const ShareSphereException &e)
        {
            cerr << "\n  [!] Error: " << e.what() << endl;
            cout << "  Press Enter to continue...";
            cin.ignore();
        }
    }
}

// ==================== PANEL 1: RECEIVED REQUESTS ====================

void SystemManager::displayReceivedRequestsPanel()
{
    cout << "\n  --- RECEIVED REQUESTS (Requests for YOUR items) ---\n";

    vector<Request *> pendingRequests = fileManager.getPendingRequestsForUser(
        stoi(currentUser->getID()));

    if (pendingRequests.empty())
    {
        cout << "  (No pending requests)\n";
        return;
    }

    cout << "  You have " << pendingRequests.size() << " pending request(s):\n\n";

    for (size_t i = 0; i < pendingRequests.size(); i++)
    {
        Request *req = pendingRequests[i];
        Item *item = fileManager.findItemById(stoi(req->getItemID()));
        User *borrower = fileManager.findUserByStudentId(req->getBorrowerID());

        if (item && borrower)
        {
            cout << "  " << i + 1 << ". Request #" << req->getRequestID() << "\n";
            cout << "     Item: " << item->getName() << " ("
                 << item->getCategoryString() << ")\n";
            cout << "     From: " << borrower->getFullName();

            // Using dynamic_cast to access Student-specific data
            if (Student *student = dynamic_cast<Student *>(borrower))
            {
                cout << " | " << student->getDepartment()
                     << " Year " << student->getYear();
            }
            cout << "\n     Duration: " << req->getDuration()
                 << " | Notes: " << req->getNotes() << "\n";

            cout << "     [A]pprove | [R]eject | [S]kip: ";
            string action;
            getline(cin, action);

            if (action == "A" || action == "a")
            {
                handleApproveRequest(stoi(req->getRequestID()));
                break;
            }
            else if (action == "R" || action == "r")
            {
                handleRejectRequest(stoi(req->getRequestID()));
                break;
            }
        }
    }
}

void SystemManager::handleApproveRequest(int requestId)
{
    cout << "\n  --- APPROVE REQUEST ---\n";

    try
    {
        if (fileManager.updateRequestStatus(requestId, "APPROVED"))
        {
            Request *req = fileManager.findRequestById(requestId);
            if (req)
            {
                Item *item = fileManager.findItemById(stoi(req->getItemID()));
                if (item)
                {
                    item->updateStatus(false);
                    fileManager.saveAllDataToFiles();

                    cout << "  [OK] Request approved!\n";
                    cout << "  Item '" << item->getName() << "' is now marked as borrowed.\n";
                    cout << "  Connection created in Share Area.\n";
                }
            }
        }
    }
    catch (const ShareSphereException &e)
    {
        cout << "  [!] " << e.what() << "\n";
    }

    cout << "  Press Enter to continue...";
    cin.ignore();
}

void SystemManager::handleRejectRequest(int requestId)
{
    cout << "\n  Reject this request? (y/n): ";
    string choice;
    getline(cin, choice);

    if (choice == "y" || choice == "Y")
    {
        try
        {
            if (fileManager.deleteRequest(requestId))
            {
                cout << "  [OK] Request rejected and removed.\n";
            }
        }
        catch (const ShareSphereException &e)
        {
            cout << "  [!] " << e.what() << "\n";
        }
    }
}

// ==================== PANEL 2: SHARE AREA ====================

void SystemManager::displayShareAreaPanel()
{
    cout << "\n  --- IN SHARE (Your Active Connections) ---\n";

    vector<Request *> connections = fileManager.getApprovedRequestsForUser(
        stoi(currentUser->getID()));

    if (connections.empty())
    {
        cout << "  (No active connections)\n";
        return;
    }

    cout << "  You have " << connections.size() << " active connection(s):\n\n";

    for (size_t i = 0; i < connections.size(); i++)
    {
        Request *req = connections[i];
        Item *item = fileManager.findItemById(stoi(req->getItemID()));
        if (!item)
            continue;
        User *lender = fileManager.findUserByStudentId(item->getOwnerID());
        User *borrower = fileManager.findUserByStudentId(req->getBorrowerID());

        if (lender && borrower)
        {
            cout << "  " << i + 1 << ". Connection #" << req->getRequestID() << "\n";
            cout << "     Item: " << item->getName() << " (" << item->getCategoryString() << ")\n";
            cout << "     Lender:   " << lender->getFullName()
                 << " | Phone: " << lender->getContact()
                 << " | Email: " << (lender->getEmail().empty() ? "N/A" : lender->getEmail()) << "\n";
            cout << "     Borrower: " << borrower->getFullName()
                 << " | Phone: " << borrower->getContact()
                 << " | Email: " << (borrower->getEmail().empty() ? "N/A" : borrower->getEmail()) << "\n";

            if (currentUser->getID() == item->getOwnerID())
            {
                cout << "     [G]ot Back | [S]kip: ";
                string action;
                getline(cin, action);

                if (action == "G" || action == "g")
                {
                    handleGotBack(stoi(req->getRequestID()));
                    break;
                }
            }
            else
            {
                cout << "     (You are the borrower)\n";
            }
            cout << endl;
        }
    }
}

void SystemManager::handleGotBack(int requestId)
{
    cout << "\n  --- ITEM RETURN CONFIRMATION ---\n";

    try
    {
        Request *req = fileManager.findRequestById(requestId);
        if (!req)
        {
            throw NotFoundException("Request #" + to_string(requestId) + " not found.");
        }

        Item *item = fileManager.findItemById(stoi(req->getItemID()));
        if (!item)
        {
            throw NotFoundException("Item not found for this request.");
        }

        cout << "  Item: " << item->getName() << "\n";
        cout << "  Has the item been returned to you?\n";
        cout << "  1. YES - Item returned, make available again\n";
        cout << "  2. NO  - Item returned but remove from system\n";
        cout << "  3. CANCEL - Item not returned yet\n";
        cout << "  Choice (1-3): ";

        int choice = getMenuChoice(1, 3);

        if (choice == 1)
        {
            User *lender = fileManager.findUserByStudentId(item->getOwnerID());
            User *borrower = fileManager.findUserByStudentId(req->getBorrowerID());
            if (lender)
            {
                lender->incrementTransactions();
                // trust score removed
            }
            if (borrower)
            {
                borrower->incrementTransactions();
            }

            item->updateStatus(true);
            fileManager.deleteRequest(requestId);
            cout << "  [OK] Item marked as available. Connection closed.\n";
            cout << "  [OK] Transactions updated.\n";

            // Check waitlist and auto-promote next user
            string nextStudentID = fileManager.promoteFromWaitlist(item->getID());
            if (!nextStudentID.empty())
            {
                int newReqId = fileManager.getNextRequestId();
                time_t wnow = time(0);
                tm *wltm = localtime(&wnow);
                stringstream wl_date;
                wl_date << (1900 + wltm->tm_year) << "-"
                        << setw(2) << setfill('0') << (1 + wltm->tm_mon) << "-"
                        << setw(2) << setfill('0') << wltm->tm_mday;

                Request autoReq(to_string(newReqId), nextStudentID, item->getID(),
                                "1 week", RequestStatus::PENDING, wl_date.str(),
                                "Auto-created from waitlist");
                fileManager.addRequest(autoReq);

                User *waitUser = fileManager.findUserByStudentId(nextStudentID);
                cout << "  [!] Waitlisted user "
                     << (waitUser ? waitUser->getFullName() : nextStudentID)
                     << " auto-notified with a pending request!\n";
            }
        }
        else if (choice == 2)
        {
            User *lender = fileManager.findUserByStudentId(item->getOwnerID());
            User *borrower = fileManager.findUserByStudentId(req->getBorrowerID());
            if (lender)
            {
                lender->incrementTransactions();
                // trust score removed
            }
            if (borrower)
            {
                borrower->incrementTransactions();
            }

            fileManager.removeItem(stoi(item->getID()));
            fileManager.deleteRequest(requestId);
            cout << "  [OK] Item removed from system. Connection closed.\n";
            cout << "  [OK] Transactions updated.\n";
        }
        else
        {
            cout << "  Action cancelled.\n";
            return;
        }

        fileManager.saveAllDataToFiles();
    }
    catch (const ShareSphereException &e)
    {
        cout << "  [!] " << e.what() << "\n";
    }

    cout << "  Press Enter to continue...";
    cin.ignore();
}

// ==================== ACTION 1: BORROW ====================

void SystemManager::handleBorrowAction()
{
    cout << "\n";
    printBoxBorder('-');
    printBoxCentered("BORROW AN ITEM");
    printBoxBorder('-');
    cout << "  Search by name, category, or keyword\n";
    cout << "  (Categories: TEXTBOOK, ELECTRONICS, CLOTHING,\n";
    cout << "   LAB_EQUIPMENT, STATIONARY, FOOD, OTHER)\n";
    cout << "  Search: ";
    string query;
    getline(cin, query);

    vector<Item *> results = fileManager.searchAvailableItems(query);

    if (results.empty())
    {
        cout << "\n  [!] No available items found matching '" << query << "'.\n";

        // Offer waitlist for unavailable items
        vector<Item *> unavailable;
        for (Item *item : fileManager.getAllItems())
        {
            if (!item->getAvailable() && item->matchesSearch(query) &&
                item->getOwnerID() != currentUser->getID())
            {
                unavailable.push_back(item);
            }
        }
        if (!unavailable.empty())
        {
            cout << "\n  " << unavailable.size() << " unavailable item(s) match your search.\n";
            cout << "  Would you like to join a waitlist? (y/n): ";
            string wChoice;
            getline(cin, wChoice);
            if (wChoice == "y" || wChoice == "Y")
            {
                for (size_t i = 0; i < unavailable.size(); i++)
                {
                    int wCount = fileManager.getWaitlistCount(unavailable[i]->getID());
                    cout << "  " << (i + 1) << ". [" << unavailable[i]->getCategoryString() << "] "
                         << unavailable[i]->getName() << " (" << wCount << " waiting)\n";
                }
                cout << "  Enter item number (0 to cancel): ";
                int wPick = getMenuChoice(0, static_cast<int>(unavailable.size()));
                if (wPick > 0)
                {
                    if (fileManager.addToWaitlist(unavailable[wPick - 1]->getID(), currentUser->getID()))
                    {
                        cout << "  [OK] Added to waitlist! You'll get a request when available.\n";
                    }
                    else
                    {
                        cout << "  [!] You're already on the waitlist for this item.\n";
                    }
                }
            }
        }
        return;
    }

    cout << "\n  Found " << results.size() << " available item(s):\n";
    cout << "  -------------------------------------------\n";
    for (size_t i = 0; i < results.size(); i++)
    {
        Item *item = results[i];
        User *owner = fileManager.findUserByStudentId(item->getOwnerID());

        cout << "  " << i + 1 << ". [" << item->getCategoryString() << "] "
             << item->getName() << "\n";
        cout << "     " << item->getDescription() << "\n";
        cout << "     Owner: " << (owner ? owner->getFullName() : "Unknown");
        if (owner)
        {
            cout << " (" << owner->getDepartment() << ")";
        }
        cout << " | Listed: " << item->getDateListed() << "\n\n";
    }

    cout << "  Enter item number to request (0 to cancel): ";
    int itemChoice = getMenuChoice(0, results.size());

    if (itemChoice > 0)
    {
        Item *selectedItem = results[itemChoice - 1];

        if (selectedItem->getOwnerID() == currentUser->getID())
        {
            cout << "  [!] You cannot request to borrow your own item.\n";
            return;
        }

        cout << "\n  Request to borrow: " << selectedItem->getName() << endl;
        cout << "  Enter duration (e.g., '1 week', '3 days'): ";
        string duration;
        getline(cin, duration);

        cout << "  Additional notes (optional): ";
        string notes;
        getline(cin, notes);

        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream date_ss;
        date_ss << (1900 + ltm->tm_year) << "-"
                << setw(2) << setfill('0') << (1 + ltm->tm_mon) << "-"
                << setw(2) << setfill('0') << ltm->tm_mday;
        string currentDate = date_ss.str();

        int requestId = fileManager.getNextRequestId();

        Request newReq(to_string(requestId),
                       currentUser->getID(),
                       selectedItem->getID(),
                       duration,
                       RequestStatus::PENDING,
                       currentDate,
                       notes);

        try
        {
            if (fileManager.addRequest(newReq))
            {
                cout << "\n  [OK] Request sent to item owner!\n";
                cout << "  Request ID: " << requestId << "\n";
                cout << "  They will see it in their 'Received Requests' panel.\n";
            }
        }
        catch (const ShareSphereException &e)
        {
            cout << "  [!] " << e.what() << "\n";
        }
    }
}

// ==================== ACTION 2: LEND ====================
// In handleLendAction() - SIMPLIFIED VERSION:
void SystemManager::handleLendAction()
{
    cout << "\n";
    printBoxBorder('-');
    printBoxCentered("LEND AN ITEM");
    printBoxBorder('-');

    string name, description, category;

    cout << "  Item Name: ";
    getline(cin, name);

    cout << "  Description: ";
    getline(cin, description);

    // Category selection
    cout << "\n  Select Category:\n";
    cout << "  1. TEXTBOOK       2. ELECTRONICS\n";
    cout << "  3. CLOTHING       4. LAB_EQUIPMENT\n";
    cout << "  5. STATIONARY     6. FOOD\n";
    cout << "  7. OTHER\n";
    cout << "  Enter choice (1-7): ";

    int catChoice = getMenuChoice(1, 7);
    Category cat;
    switch (catChoice)
    {
    case 1:
        cat = Category::TEXTBOOK;
        break;
    case 2:
        cat = Category::ELECTRONICS;
        break;
    case 3:
        cat = Category::CLOTHING;
        break;
    case 4:
        cat = Category::LAB_EQUIPMENT;
        break;
    case 5:
        cat = Category::STATIONARY;
        break;
    case 6:
        cat = Category::FOOD;
        break;
    default:
        cat = Category::OTHER;
        break;
    }

    int itemId = fileManager.getNextItemId();

    Item newItem(to_string(itemId),
                 name,
                 cat,
                 description,
                 true,
                 currentUser->getID());

    try
    {
        if (fileManager.addItem(newItem))
        {
            cout << "\n  [OK] Item added successfully!\n";
            cout << "  Item ID: " << itemId << "\n";
            cout << "  Other students can now request to borrow it.\n";
        }
    }
    catch (const ShareSphereException &e)
    {
        cout << "  [!] " << e.what() << "\n";
    }

    cout << "  Press Enter to continue...";
    cin.ignore();
}

// ==================== USER PROFILE ====================

void SystemManager::displayUserProfile()
{
    cout << "\n";
    printBoxBorder('=');
    printBoxCentered("MY PROFILE");
    printBoxBorder('=');

    // === STREAMS: Using operator<< with polymorphic dispatch ===
    cout << "  " << *currentUser << "\n"; // Calls virtual display() via friend operator<<

    if (Student *student = dynamic_cast<Student *>(currentUser))
    {
        cout << "  Year: " << student->getYear() << "\n";
        cout << "  Verified: " << (student->getVerified() ? "Yes (Email provided)" : "No") << "\n";
    }

    cout << "  Total Transactions: " << currentUser->getTotalTransactions() << "\n";
    // trust score display removed
    printBoxBorder('=');

    cout << "\n  Press Enter to return to dashboard...";
    cin.ignore();
}

// ==================== UTILITY METHODS ====================

int SystemManager::getMenuChoice(int min, int max)
{
    int choice;
    while (true)
    {
        cin >> choice;
        if (cin.fail() || choice < min || choice > max)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input. Please enter " << min << "-" << max << ": ";
        }
        else
        {
            cin.ignore();
            return choice;
        }
    }
}