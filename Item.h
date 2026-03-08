#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

enum class Category {
    TEXTBOOK,
    ELECTRONICS, 
    CLOTHING,
    LAB_EQUIPMENT,
    STATIONARY,
    FOOD,
    OTHER
};

class Item {
private:
    string itemID;          // Format: ITM001, ITM002
    string name;
    Category category;
    string description;
    bool isAvailable;
    string ownerID;         // Format: STU001
    string dateListed;      // Format: 2024-03-15
    
public:
    // Constructor
    Item(string id = "", string name = "", Category cat = Category::OTHER,
         string desc = "", bool available = true, 
         string owner = "");
    
    // Getters
    string getID() const { return itemID; }
    string getName() const { return name; }
    Category getCategory() const { return category; }
    string getDescription() const { return description; }
    bool getAvailable() const { return isAvailable; }
    string getOwnerID() const { return ownerID; }
    string getDateListed() const { return dateListed; }
    
    // Setters
    void updateStatus(bool available);
    
    // Utility
    string getCategoryString() const;
    string getDetails() const;
    bool matchesSearch(const string& keyword) const;
    
    // File operations
    string toFileString() const;
    static Item fromString(const string& data);
    
    // Static converters
    static string categoryToString(Category cat);
    static Category stringToCategory(const string& str);

     static string getCurrentDate();
};

#endif