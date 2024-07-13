#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

// Function to get the current date and time as a string
string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-"
       << setfill('0') << setw(2) << ltm->tm_mday << " "
       << setfill('0') << setw(2) << ltm->tm_hour << ":"
       << setfill('0') << setw(2) << ltm->tm_min << ":"
       << setfill('0') << setw(2) << ltm->tm_sec;
    return ss.str();
}

// Function to add an expense
void addExpense() {
    string description;
    double amount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
    cout << "Enter expense description: ";
    getline(cin, description);
    cout << "Enter expense amount: ";
    cin >> amount;

    // Validation for amount
    if (cin.fail() || amount < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid amount. Please enter a non-negative number." << endl;
        return;
    }

    ofstream outFile("expenses.txt", ios::app);
    if (outFile.is_open()) {
        outFile << getCurrentDateTime() << " - " << description << " - $" << fixed << setprecision(2) << amount << endl;
        outFile.close();
        cout << "Expense recorded successfully." << endl;
    } else {
        cout << "Unable to open file for writing." << endl;
    }
}

// Function to display all expenses
void displayExpenses() {
    ifstream inFile("expenses.txt");
    string line;
    if (inFile.is_open()) {
        cout << "Recorded Expenses:" << endl;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    } else {
        cout << "Unable to open file for reading." << endl;
    }
}

// Function to summarize total expenses
void summarizeExpenses() {
    ifstream inFile("expenses.txt");
    string line;
    double total = 0.0;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            size_t pos = line.find_last_of('$');
            if (pos != string::npos) {
                string amountStr = line.substr(pos + 1);

                // Ensure the extracted substring is a valid number
                bool valid = true;
                for (size_t i = 0; i < amountStr.length(); ++i) {
                    char c = amountStr[i];
                    if (!isdigit(c) && c != '.' && c != ' ') {
                        valid = false;
                        break;
                    }
                }

                if (valid) {
                    try {
                        double amount = stod(amountStr);
                        total += amount;
                    } catch (const invalid_argument& e) {
                        cerr << "Error: Invalid amount format in line: " << line << endl;
                    } catch (const out_of_range& e) {
                        cerr << "Error: Amount out of range in line: " << line << endl;
                    }
                } else {
                    cerr << "Error: Invalid characters in amount: " << amountStr << endl;
                }
            } else {
                cerr << "Error: No dollar sign found in line: " << line << endl;
            }
        }
        inFile.close();
        cout << "Total expenses: $" << fixed << setprecision(2) << total << endl;
    } else {
        cout << "Unable to open file for reading." << endl;
    }
}

int main() {
    int choice;
    do {
        cout << "\nExpense Tracker Menu" << endl;
        cout << "1. Add Expense" << endl;
        cout << "2. Display Expenses" << endl;
        cout << "3. Summarize Expenses" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addExpense();
                break;
            case 2:
                displayExpenses();
                break;
            case 3:
                summarizeExpenses();
                break;
            case 4:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}

