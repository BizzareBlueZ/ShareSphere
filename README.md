# Welcome to ShareSphere!

ShareSphere is your friendly campus sharing hub, built with C++ and Qt. Whether you want to lend, borrow, or just keep track of your stuff and connections, ShareSphere makes it easy to navigate around in the daily needs sharing world. Primarily it is designed for students in a particular campus who want a simple, modern way to manage shared items and requests.

## What Can You Do with ShareSphere?

- Log in securely with your student ID and PIN
- Add, edit, or remove items you want to share
- Request to borrow items from others, or accept/reject requests for your own
- Join a waitlist if something you want isn’t available yet
- See and manage your connections with other users
- Enjoy a clean, modern interface with both light and dark themes

## Getting Started

**What you’ll need:**

- Qt 5 or 6 (with Qt Widgets)
- A C++17 compatible compiler (MinGW, MSVC, GCC, etc.)
- CMake

**How to set up:**

1. Clone this repository to your computer
2. Open it in your favorite IDE or use CMake to configure the project
3. Build the project (with your IDE or `make`/`mingw32-make`)

**How to run ShareSphere from the terminal in vscode**

**To run the Qt (GUI) version:** 
		./runqt.bat

**To run the console version:** - 
        make run
		

## How to Use

1. Open ShareSphere and log in
2. Browse available items, or add your own
3. Send requests to borrow, or respond to requests from others
4. Manage your waitlist and connections
5. Enjoy sharing and connecting!

## Project Structure

- `main_qt.cpp` — The heart of the GUI
- `main.cpp` — Console version entry point
- `FileManager.*` — Handles all the data behind the scenes
- `Student.*`, `User.*` — User and student details
- `Item.*` — All about the items
- `Request.*` — Request logic
- `Validator.*` — Checks your input
- `SystemManager.*` — Console logic
- `*.txt` — Where your data lives (users, items, requests, waitlist)
- `runqt.bat` — Quick launch for Windows

## Data Files

- `users.txt` — All users and students
- `items.txt` — All items
- `requests.txt` — All requests
- `waitlist.txt` — Waitlist info

## Thanks!
