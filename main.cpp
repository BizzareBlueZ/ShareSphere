#include "SystemManager.h"
#include "ShareSphereException.h"
#include <iostream>

using namespace std;

int main()
{
    // === EXCEPTION HANDLING: Top-level try-catch for unhandled exceptions ===
    try
    {
        SystemManager system;
        system.run();
    }
    catch (const ShareSphereException &e)
    {
        cerr << "\n  [FATAL] ShareSphere Error: " << e.what() << endl;
        cerr << "  The program will now exit.\n";
        return 1;
    }
    catch (const exception &e)
    {
        cerr << "\n  [FATAL] Unexpected Error: " << e.what() << endl;
        cerr << "  The program will now exit.\n";
        return 1;
    }

    return 0;
}