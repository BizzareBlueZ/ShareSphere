#include "Request.h"
#include "User.h"
#include "Student.h"
#include "Item.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;

// Constructor
Request::Request(string reqID, string borrower, string item,
                 string dur, RequestStatus stat,
                 string date, string note)
    : requestID(reqID), borrowerID(borrower), itemID(item),
      duration(dur), status(stat), requestDate(date), notes(note)
{
}

// Convert status enum to string
string Request::getStatusString() const
{
    switch (status)
    {
    case RequestStatus::PENDING:
        return "PENDING";
    case RequestStatus::APPROVED:
        return "APPROVED";
    case RequestStatus::REJECTED:
        return "REJECTED";
    case RequestStatus::COMPLETED:
        return "COMPLETED";
    case RequestStatus::CANCELLED:
        return "CANCELLED";
    default:
        return "UNKNOWN";
    }
}

void Request::approve() { status = RequestStatus::APPROVED; }
void Request::reject() { status = RequestStatus::REJECTED; }
void Request::complete() { status = RequestStatus::COMPLETED; }
void Request::cancel() { status = RequestStatus::CANCELLED; }

void Request::updateNotes(string newNotes) { notes = newNotes; }

bool Request::isPending() const { return status == RequestStatus::PENDING; }
bool Request::isActive() const { return status == RequestStatus::APPROVED || status == RequestStatus::PENDING; }

bool Request::isApproved() const
{
    return status == RequestStatus::APPROVED;
}

bool Request::canViewContact(string viewerID) const
{
    if (status != RequestStatus::APPROVED)
        return false;
    return (viewerID == borrowerID); // Item owner check happens outside
}

string Request::getContactForViewer(string viewerID, const vector<User *> &allUsers) const
{
    if (!canViewContact(viewerID))
        return "Contact: Hidden until approved";

    string targetID;
    if (viewerID == borrowerID)
    {
        // Borrower wants lender's contact
        // Need item owner ID - this should come from outside
        return "Contact: Please check with system administrator";
    }
    else
    {
        // Lender wants borrower's contact
        targetID = borrowerID;
    }

    for (User *user : allUsers)
    {
        if (user->getID() == targetID)
        {
            return "Contact: " + user->getContact();
        }
    }

    return "Contact: Not available";
}

string Request::getEmailForViewer(string viewerID, const vector<User *> &allUsers) const
{
    if (!canViewContact(viewerID))
        return "Email: Hidden until approved";

    string targetID;
    if (viewerID == borrowerID)
    {
        return "Email: Please check with system administrator";
    }
    else
    {
        targetID = borrowerID;
    }

    for (User *user : allUsers)
    {
        if (user->getID() == targetID)
        {
            string email = user->getEmail();
            if (email.empty())
                return "Email: Not provided";
            return "Email: " + email;
        }
    }

    return "Email: Not available";
}

string Request::getDetailsForBorrower(const vector<User *> &users, const vector<Item *> &items) const
{
    stringstream ss;
    ss << "Request ID: " << requestID << "\n";
    ss << "Item: ";

    // Find item details
    for (Item *item : items)
    {
        if (item->getID() == itemID)
        {
            ss << item->getName() << " (" << item->getCategoryString() << ")\n";
            break;
        }
    }

    ss << "Status: " << getStatusString() << "\n";
    ss << "Duration: " << duration << "\n";
    ss << "Notes: " << notes << "\n";

    if (isApproved())
    {
        // Find lender (item owner) contact
        for (Item *item : items)
        {
            if (item->getID() == itemID)
            {
                string ownerID = item->getOwnerID();
                for (User *user : users)
                {
                    if (user->getID() == ownerID)
                    {
                        ss << "Lender Contact: " << user->getContact() << "\n";
                        if (!user->getEmail().empty())
                        {
                            ss << "Lender Email: " << user->getEmail() << "\n";
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    else
    {
        ss << "Lender Contact: Hidden until approved\n";
    }

    return ss.str();
}

string Request::getDetailsForLender(const vector<User *> &users, const vector<Item *> &items) const
{
    stringstream ss;
    ss << "Request ID: " << requestID << "\n";

    // Find borrower details
    for (User *user : users)
    {
        if (user->getID() == borrowerID)
        {
            Student *student = dynamic_cast<Student *>(user);
            if (student)
            {
                ss << "Borrower: " << student->getFullName()
                   << " (" << student->getDepartment()
                   << ", Year " << student->getYear() << ")\n";
            }
            break;
        }
    }

    // Find item details
    for (Item *item : items)
    {
        if (item->getID() == itemID)
        {
            ss << "Item: " << item->getName()
               << " - " << item->getDescription() << "\n";
            break;
        }
    }

    ss << "Duration: " << duration << "\n";
    ss << "Status: " << getStatusString() << "\n";
    ss << "Borrower Notes: " << notes << "\n";

    if (isApproved())
    {
        // Show borrower contact
        for (User *user : users)
        {
            if (user->getID() == borrowerID)
            {
                ss << "Borrower Contact: " << user->getContact() << "\n";
                if (!user->getEmail().empty())
                {
                    ss << "Borrower Email: " << user->getEmail() << "\n";
                }
                break;
            }
        }
    }
    else
    {
        ss << "Borrower Contact: Hidden until approved\n";
    }

    return ss.str();
}

// Serialization
string Request::serialize() const
{
    stringstream ss;
    ss << requestID << ","
       << borrowerID << ","
       << itemID << ","
       << duration << ","
       << static_cast<int>(status) << ","
       << requestDate << ","
       << notes;
    return ss.str();
}

// Deserialization
Request *Request::deserialize(const string &data)
{
    stringstream ss(data);
    string reqID, borrower, item, dur, date, note, statusStr;

    getline(ss, reqID, ',');
    getline(ss, borrower, ',');
    getline(ss, item, ',');
    getline(ss, dur, ',');
    getline(ss, statusStr, ',');
    getline(ss, date, ',');
    getline(ss, note);

    RequestStatus stat = RequestStatus::PENDING;
    try
    {
        int statusInt = stoi(statusStr);
        if (statusInt >= 0 && statusInt <= 4)
        {
            stat = static_cast<RequestStatus>(statusInt);
        }
    }
    catch (...)
    {
        // Keep default PENDING
    }

    return new Request(reqID, borrower, item, dur, stat, date, note);
}

// Generate Request ID
string Request::generateRequestID(int count)
{
    stringstream ss;
    ss << "REQ" << setw(4) << setfill('0') << count;
    return ss.str();
}

// === FRIEND FUNCTION + STREAMS: operator<< overload for Request ===
ostream &operator<<(ostream &os, const Request &req)
{
    os << "Request #" << req.requestID << " [" << req.getStatusString() << "]\n"
       << "  Borrower: " << req.borrowerID
       << " | Item ID: " << req.itemID
       << " | Duration: " << req.duration << "\n"
       << "  Date: " << req.requestDate
       << " | Notes: " << req.notes;
    return os;
}