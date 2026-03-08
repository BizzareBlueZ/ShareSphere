#ifndef DATASTORE_H
#define DATASTORE_H

#include <vector>
#include <string>
#include <algorithm>

// ============================================================
// === TEMPLATE CLASS: Generic data store for pointer types  ===
// ============================================================
// Demonstrates: Class Templates, Iterator Support, Memory Management

template <typename T>
class DataStore
{
private:
    std::vector<T *> data;

public:
    DataStore() = default;

    // Destructor frees all stored pointers
    ~DataStore()
    {
        clear();
    }

    // Prevent copying to avoid double-delete issues
    DataStore(const DataStore &) = delete;
    DataStore &operator=(const DataStore &) = delete;

    // Add an element to the store
    void add(T *item)
    {
        if (item)
            data.push_back(item);
    }

    // Find element by ID (requires T to have getID() returning std::string)
    T *findById(const std::string &id) const
    {
        for (auto *item : data)
        {
            if (item->getID() == id)
                return item;
        }
        return nullptr;
    }

    // Remove element by ID, returns true if found and removed
    bool removeById(const std::string &id)
    {
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            if ((*it)->getID() == id)
            {
                delete *it;
                data.erase(it);
                return true;
            }
        }
        return false;
    }

    // Get all elements as a vector
    std::vector<T *> getAll() const { return data; }

    // Get count of elements
    int count() const { return static_cast<int>(data.size()); }

    // Check if empty
    bool empty() const { return data.empty(); }

    // Clear all elements and free memory
    void clear()
    {
        for (auto *item : data)
            delete item;
        data.clear();
    }

    // Get the maximum numeric ID (assumes IDs are numeric strings)
    int getMaxId() const
    {
        int maxId = 0;
        for (const auto *item : data)
        {
            try
            {
                int id = std::stoi(item->getID());
                if (id > maxId)
                    maxId = id;
            }
            catch (...)
            {
                // Skip non-numeric IDs
            }
        }
        return maxId;
    }

    // === Iterator support for range-based for loops ===
    typename std::vector<T *>::iterator begin() { return data.begin(); }
    typename std::vector<T *>::iterator end() { return data.end(); }
    typename std::vector<T *>::const_iterator begin() const { return data.begin(); }
    typename std::vector<T *>::const_iterator end() const { return data.end(); }
};

#endif
