#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "FileManager.h"
#include "User.h"

class SystemManager
{
private:
    FileManager fileManager;
    User *currentUser;

    // Window methods
    bool showLoginWindow();
    bool handleLogin();
    void handleCreateAccount();
    void showDashboardWindow();

    // Panel methods
    void displayReceivedRequestsPanel();
    void displayShareAreaPanel();
    void displayMyItemsPanel();
    void handleApproveRequest(int requestId);
    void handleRejectRequest(int requestId);
    void handleGotBack(int requestId);

    // Action methods
    void handleBorrowAction();
    void handleLendAction();

    // Profile method
    void displayUserProfile();

    // Utility
    int getMenuChoice(int min, int max);

public:
    SystemManager();
    void run();
};

#endif