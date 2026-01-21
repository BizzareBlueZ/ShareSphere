#include "Item.h"
#include <sstream>
#include <algorithm>
#include <cctype>

Item::Item(string id, string name, string category, string desc,
           double p, bool avail, string owner) 
    : itemID(id), name(name), category(category), description(desc),
      price(p), isAvailable(avail), ownerID(owner) {}

void Item::updateStatus(bool available) {
    isAvailable = available;
}

string Item::getDetails() const {
    stringstream ss;
    ss << "ID: " << itemID << "\n"
       << "Name: " << name << "\n"
       << "Category: " << category << "\n"
       << "Description: " << description << "\n"
       << "Price: $" << price << "\n"
       << "Available: " << (isAvailable ? "Yes" : "No") << "\n"
       << "Owner: " << ownerID;
    return ss.str();
}

bool Item::isFree() const {
    return price == 0.0;
}

bool Item::matchesSearch(const string& keyword) const {
    if (keyword.empty()) return true;
    
    string keywordLower = keyword;
    transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);
    
    string nameLower = name;
    transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
    
    string descLower = description;
    transform(descLower.begin(), descLower.end(), descLower.begin(), ::tolower);
    
    string catLower = category;
    transform(catLower.begin(), catLower.end(), catLower.begin(), ::tolower);
    
    return (nameLower.find(keywordLower) != string::npos) ||
           (descLower.find(keywordLower) != string::npos) ||
           (catLower.find(keywordLower) != string::npos);
}

string Item::toFileString() const {
    return itemID + "|" + name + "|" + category + "|" + 
           description + "|" + to_string(price) + "|" + 
           (isAvailable ? "1" : "0") + "|" + ownerID;
}

Item Item::fromString(const string& data) {
    stringstream ss(data);
    string id, name, cat, desc, priceStr, availStr, owner;
    
    getline(ss, id, '|');
    getline(ss, name, '|');
    getline(ss, cat, '|');
    getline(ss, desc, '|');
    getline(ss, priceStr, '|');
    getline(ss, availStr, '|');
    getline(ss, owner, '|');
    
    double price = stod(priceStr);
    bool available = (availStr == "1");
    
    return Item(id, name, cat, desc, price, available, owner);
}