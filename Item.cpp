#include "Item.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>    
#include <iomanip> 

Item::Item(string id, string name, Category cat, string desc,
           bool available, string owner)
    : itemID(id), name(name), category(cat), description(desc),
      isAvailable(available), ownerID(owner), 
      dateListed(getCurrentDate()) {}

void Item::updateStatus(bool available) {
    isAvailable = available;
}

string Item::getCategoryString() const {
    return categoryToString(category);
}

string Item::getDetails() const {
    stringstream ss;
    ss << "Item ID: " << itemID << "\n"
       << "Name: " << name << "\n"
       << "Category: " << getCategoryString() << "\n"
       << "Description: " << description << "\n"
       << "Available: " << (isAvailable ? "Yes" : "No") << "\n"
       << "Owner ID: " << ownerID << "\n"
       << "Listed on: " << dateListed;
    return ss.str();
}

bool Item::matchesSearch(const string& keyword) const {
    if (keyword.empty()) return true;
    
    string keywordLower = keyword;
    transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);
    
    string nameLower = name;
    transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
    
    string descLower = description;
    transform(descLower.begin(), descLower.end(), descLower.begin(), ::tolower);
    
    string catLower = getCategoryString();
    transform(catLower.begin(), catLower.end(), catLower.begin(), ::tolower);
    
    return (nameLower.find(keywordLower) != string::npos) ||
           (descLower.find(keywordLower) != string::npos) ||
           (catLower.find(keywordLower) != string::npos);
}

string Item::toFileString() const {
    stringstream ss;
    ss << itemID << "|" << name << "|" << categoryToString(category) << "|"
       << description << "|" << (isAvailable ? "1" : "0") << "|" 
       << ownerID << "|" << dateListed;
    return ss.str();
}

Item Item::fromString(const string& data) {
    stringstream ss(data);
    string id, name, catStr, desc, availStr, owner, date;
    
    getline(ss, id, '|');
    getline(ss, name, '|');
    getline(ss, catStr, '|');
    getline(ss, desc, '|');
    getline(ss, availStr, '|');
    getline(ss, owner, '|');
    getline(ss, date, '|');
    
    Category cat = stringToCategory(catStr);
    bool available = (availStr == "1");
    
    return Item(id, name, cat, desc, available, owner);
}

string Item::categoryToString(Category cat) {
    switch(cat) {
        case Category::TEXTBOOK: return "TEXTBOOK";
        case Category::ELECTRONICS: return "ELECTRONICS";
        case Category::CLOTHING: return "CLOTHING";
        case Category::LAB_EQUIPMENT: return "LAB_EQUIPMENT";
        case Category::STATIONARY: return "STATIONARY";
        case Category::FOOD: return "FOOD";
        default: return "OTHER";
    }
}

Category Item::stringToCategory(const string& str) {
    if (str == "TEXTBOOK") return Category::TEXTBOOK;
    if (str == "ELECTRONICS") return Category::ELECTRONICS;
    if (str == "CLOTHING") return Category::CLOTHING;
    if (str == "LAB_EQUIPMENT") return Category::LAB_EQUIPMENT;
    if (str == "STATIONARY") return Category::STATIONARY;
    if (str == "FOOD") return Category::FOOD;
    return Category::OTHER;
}

string Item::getCurrentDate() {
auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm now_tm = *localtime(&now_time);
    
    stringstream ss;
    ss << setw(2) << setfill('0') << now_tm.tm_mday << "-";
    
    // Month names
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    ss << months[now_tm.tm_mon] << "-"
       << (1900 + now_tm.tm_year);
    
    return ss.str();
}