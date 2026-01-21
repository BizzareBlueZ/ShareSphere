#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

class Item {
private:
    string itemID;
    string name;
    string category;
    string description;
    double price;
    bool isAvailable;
    string ownerID;

public:
    Item(string id = "", string name = "", string category = "", 
         string description = "", double price = 0.0, 
         bool available = true, string owner = "");
    
    void updateStatus(bool available);
    string getDetails() const;
    bool isFree() const;
    bool matchesSearch(const string& keyword) const;
    
    string getID() const { return itemID; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    string getOwner() const { return ownerID; }
    bool getAvailable() const { return isAvailable; }
    double getPrice() const { return price; }
    
    string toFileString() const;
    static Item fromString(const string& data);
};

#endif