#include <iostream>
#include "Item.h"
using namespace std;

int main() {
    cout << "=== Testing Item Class ===\n\n";
    
    Item item("ITM001", "Lab Coat", "Clothing", 
              "Chemistry lab coat size M", 0.0, true, "STU001");
    
    cout << "vv Created: " << item.getName() << endl;
    cout << "vv ID: " << item.getID() << endl;
    cout << "vv Category: " << item.getCategory() << endl;
    cout << "vv Free? " << (item.isFree() ? "Yes" : "No") << endl;
    
    cout << "\nvv File string:\n" << item.toFileString() << endl;
    
    cout << "\nvv Search tests:" << endl;
    cout << "  'lab': " << (item.matchesSearch("lab") ? "Found vv" : "Not found xx") << endl;
    cout << "  'coat': " << (item.matchesSearch("coat") ? "Found vv" : "Not found xx") << endl;
    cout << "  'book': " << (item.matchesSearch("book") ? "Found vv" : "Not found xx") << endl;
    
    cout << "\n** Item class works!\n";
    return 0;
}