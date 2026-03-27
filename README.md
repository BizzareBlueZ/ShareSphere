# ShareSphere

## The Idea
ShareSphere is a campus item-sharing platform developed in C++17 with a Qt-based graphical interface and a console-based alternative.

It enables students to lend and borrow items such as textbooks, lab equipment, and daily essentials through a structured and reliable system.

The platform replaces informal borrowing with a centralized solution that manages listings, requests, and interactions efficiently while maintaining user privacy.


## Features

- Secure authentication using student ID and PIN
- Item management (add, browse, search, remove listings)
- Borrow request system with approval/rejection workflow
- Waitlist system for unavailable items
- Connection management between users
- Dual interface:
  - Qt-based GUI (light/dark theme)
  - Console-based application

## Technologies Used

- Language: C++17  
- Framework: Qt (Qt Widgets)  
- Build System: CMake  
- Storage: Plain text files  
## Build and Execution

### Requirements
- C++17 compatible compiler (GCC / MinGW / MSVC)
- Qt 5 or 6 (with Qt Widgets)
- CMake

## Data Storage

The system uses text files for persistence:

- users.txt — user data  
- items.txt — item listings  
- requests.txt — borrow requests  
- waitlist.txt — waitlist entries  

All data is loaded at startup and updated immediately after any modification.

## Getting Started

Follow these steps to quickly clone, run, and test the project.

---

### 1. Clone the Repository
git clone https://github.com/BizzareBlueZ/ShareSphere.git
cd ShareSphere

---

### 2. Make Sure Data Files Exist

Before running, ensure these files are present in the root directory:

- users.txt  
- items.txt  
- requests.txt  
- waitlist.txt  

If they are empty, the system will still run and populate them during use.

---

### 3. Choose How You Want to Run

You can run the project in two ways:

- Console (CLI) version  
- GUI (Qt) version  

---

## Run Console Version (CLI|| build done with the help of makefile)

### Compile
make

### Run
make run

### Quick Demo (CLI)
- Log in using a valid user (or create one if supported)
- Add an item
- Search or browse items
- Send a borrow request
- Accept/reject from another account (if testing multiple users)

---

## Run GUI Version (Qt)

### Option 1 (Windows)
./runqt.bat

### Option 2 (Qt Creator)
- Open the project in Qt Creator
- Configure the Qt kit (compiler + Qt version)
- Click “Run”

### Quick Demo (GUI)
- Log in from the interface
- Add or browse items
- Send a borrow request
- Approve/reject requests from the UI
- Observe waitlist behavior if item is unavailable

---

## Notes

- All changes are saved automatically to `.txt` files  
- Both GUI and CLI use the same backend logic  
- You can switch between versions without losing data  

## Project Structure

main.cpp              - Console entry point  
main_qt.cpp           - GUI entry point  
FileManager.*         - File I/O and data persistence  
SystemManager.*       - Core application logic  
User.*, Student.*     - User hierarchy  
Item.*                - Item management  
Request.*             - Borrow request handling  
Validator.*           - Input validation  
*.txt                 - Data storage files  
runqt.bat             - Windows GUI launcher  


## Data Files

- `users.txt` — All users and students
- `items.txt` — All items
- `requests.txt` — All requests
- `waitlist.txt` — Waitlist info

