#include <iostream>
#include "PowerExt.h"

using namespace std;

int main() {
    android::PowerExt powerExt = android::PowerExt();
    int choice;
    string mode;
    bool enabled;
    string boost;
    int durationMs;
    string packActName;
    int pid;
    int uid;
    bool active;

    while (true) {
        cout << "Select function to test:" << endl;
        cout << "1. setMode" << endl;
        cout << "2. isModeSupported" << endl;
        cout << "3. setBoost" << endl;
        cout << "4. isBoostSupported" << endl;
        cout << "5. notifyAppState" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter mode and enabled (1/0): ";
                cin >> mode >> enabled;
                cout << "Setting mode " << mode << " to " << (enabled ? "true" : "false") << endl;
                powerExt.setMode(mode, enabled);
                break;
            case 2:
                cout << "Enter mode: ";
                cin >> mode;
                cout << "Mode supported: " << (powerExt.isModeSupported(mode) ? "true" : "false") << endl;
                break;
            case 3:
                cout << "Enter boost and duration (in milliseconds): ";
                cin >> boost >> durationMs;
                cout << "Setting boost to " << boost << " for " << durationMs << " milliseconds" << endl;
                powerExt.setBoost(boost, durationMs);
                break;
            case 4:
                cout << "Enter boost: ";
                cin >> boost;
                cout << "Boost supported: " << (powerExt.isBoostSupported(boost) ? "true" : "false") << endl;
                break;
            case 5:
                cout << "Enter packActName, pid, uid, and active (1/0): ";
                cin >> packActName >> pid >> uid >> active;
                powerExt.notifyAppState(packActName, pid, uid, active);
                break;
            case 6:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 6." << endl;
        }
    }

    return 0;
}
