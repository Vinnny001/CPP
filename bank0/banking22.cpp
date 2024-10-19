#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include <regex>
#include <C:\Users\VINCENT OGWENO\OneDrive\Desktop\MASOMO\JKUAT\ASSIGNMENTS\YEAR 2\SEM 1\OOP\include\nlohmann\json.hpp>

using namespace std;
using json = nlohmann::json;

void mainMenu();


struct Account {
    string username;
    string password;
    string accountType;
    double savings;
    double current;
    double salary;
    string employeeNumber;

    // Account constructor
    Account(string u = "", string p = "", string type = "", double s = 0.0, double c = 0.0, double sal = 0.0, string empNum = "")
        : username(u), password(p), accountType(type), savings(s), current(c), salary(sal), employeeNumber(empNum) {}
};

// Global variable for accounts data
json accountsData;

// Load accounts from the JSON file
void loadAccounts() {
    ifstream inFile("accounts.json");
    if (inFile.is_open()) {
        try {
            inFile >> accountsData;
        } catch (json::parse_error& e) {
            cout << "Error parsing JSON: " << e.what() << endl;
        }
        inFile.close();
    } else {
        cout << "Error: Unable to open accounts file!" << endl;
    }
}

// Save accounts to the JSON file
void saveAccounts() {
    ofstream outFile("accounts.json");
    outFile << accountsData.dump(4);
    outFile.close();
}


// Login functions
class authentification {
    string usern() {
        string username;
        cout << "Enter username: ";
        getline(cin, username);

        while (username.empty()) {
            cout << endl << "You entered nothing!" << endl;
            cout << "Enter username: ";
            getline(cin, username);
        }

        cout << "\n\n";
        return username;
    }

    string passwordFunction() {
    string pass;
    char ch;
    int cursorPosition = 0;  // Track cursor position in the password string

    cout << "Enter password: ";
    
    while ((ch = _getch()) != '\r') // '\r' is the Enter key
    {
        if (ch == 8) { // Backspace (ASCII 8)
            if (cursorPosition > 0) {
                pass.erase(cursorPosition - 1, 1);  // Remove character to the left of the cursor
                cursorPosition--; // Move cursor left
                cout << "\b \b";  // Remove last '*' from the console

                // Reprint all characters after the cursor
                for (int i = cursorPosition; i < pass.size(); i++) {
                    cout << "*";
                }

                // Clear extra space after reprinting
                cout << ' ';
                for (int i = cursorPosition; i <= pass.size(); i++) {
                    cout << "\b";  // Move back to the current cursor position
                }
            }
        } else if (ch == 127 || ch == 83) { // Delete key (ASCII 127 or extended key for Delete)
            if (cursorPosition < pass.size()) {
                pass.erase(cursorPosition, 1);  // Remove character at cursor position

                // Reprint all characters after the cursor
                for (int i = cursorPosition; i < pass.size(); i++) {
                    cout << "*";
                }

                // Clear extra space after reprinting
                cout << ' ';
                for (int i = cursorPosition; i <= pass.size(); i++) {
                    cout << "\b";  // Move back to the current cursor position
                }
            }
        } else if (ch == 0 || ch == 224) { // Arrow keys or special keys
            ch = _getch(); // Get the next character to identify the specific key
            if (ch == 75) { // Left arrow (ASCII 0x4B)
                if (cursorPosition > 0) {
                    cursorPosition--; // Move cursor left
                    cout << "\b"; // Move console cursor left
                }
            } else if (ch == 77) { // Right arrow (ASCII 0x4D)
                if (cursorPosition < pass.size()) {
                    cout << "*"; // Reprint the masked character at the new cursor position
                    cursorPosition++; // Move cursor right
                }
            }
        } else if (ch >= 32 && ch <= 126) { // Printable characters
            pass.insert(pass.begin() + cursorPosition, ch);  // Insert character at cursor
            cursorPosition++;  // Move cursor right

            // Reprint the updated masked password
            for (int i = cursorPosition - 1; i < pass.size(); i++) {
                cout << "*";
            }

            // Move back to the cursor position
            for (int i = cursorPosition; i < pass.size(); i++) {
                cout << "\b"; // Move cursor back to original position
            }
        
        }
    

    }
    if (pass.empty()){
        cout<<"\n\nYou entered nothing!\n";
        passwordFunction();
    }

        return pass;
    }

public:
    Account login(const string& expectedAccountType) { 
        
        string username = usern();

        // Check if username exists in the accounts data
        if (!accountsData.contains(username)) {
            cout << "Account not found!" << "\n\n";
            mainMenu();  // Redirect to main menu if username doesn't exist
            return Account();  // Return empty Account
        }

        // Check if the account type matches the expected account type
        string actualAccountType = accountsData[username]["accountType"];
        if (actualAccountType != expectedAccountType) {
            cout << "\nUnauthorized access. Only " << expectedAccountType 
                 << " can log in here.\n" << endl;
            mainMenu();  // Redirect to main menu if account type doesn't match
            //return Account();  // Return empty Account
        }

        // Proceed to password verification
        string password = passwordFunction();

        
        while (accountsData[username]["password"] != password) {
            cout << "\nIncorrect password!" << endl;
            password = passwordFunction();
        }

        cout << "\n-----------------\n";
        cout << "Login successful!\n";
        cout << "-----------------\n";

        // Retrieve account details and return the account object
        double savings = accountsData[username]["savings"];
        double current = accountsData[username]["current"];
        double salary = accountsData[username].value("salary", 0.0);  // Default 0 for non-employees
        string employeeNumber = accountsData[username].value("employeeNumber", "");

        return Account(username, password, actualAccountType, savings, current, salary, employeeNumber);
    }
};



// Function to create employee accounts (Administrator only)
void createEmployeeAccount() {
    string username, password, employeeNumber;
    double salary;
    cout << "Enter new Employee username: ";
    cin >> username;
    cout << "Enter new Employee password: ";
    cin >> password;
    cout << "Enter Employee number: ";
    cin >> employeeNumber;
    cout << "Enter Employee salary: ";
    cin >> salary;

    // Save employee account to JSON
    accountsData[username] = {
        {"password", password},
        {"accountType", "Employee"},
        {"savings", 0.0},
        {"current", 0.0},
        {"salary", salary},
        {"employeeNumber", employeeNumber}
    };
    saveAccounts();

    cout << "Employee account created!" << endl;
}

// Function to create customer accounts (Employee only)
void createCustomerAccount() {
    string username, password;
    cout << "Enter new Customer username: ";
    cin >> username;
    cout << "Enter new Customer password: ";
    cin >> password;

    // Save customer account to JSON
    accountsData[username] = {
        {"password", password},
        {"accountType", "Customer"},
        {"savings", 0.0},
        {"current", 0.0}
    };
    saveAccounts();

    cout << "Customer account created!" << endl;
}

// View account details (Customer)
void viewAccount(Account& account) {
    cout << "Account Details:\nUsername: " << account.username
         << "\nSavings: " << account.savings
         << "\nCurrent: " << account.current << endl;
}

void trackBalance(Account& account){
    cout << "Current Balance: "<< account.current << endl;

}

// View transaction history (for Employees)
void viewTransactionHistory() {
    cout << "Displaying transaction history (dummy data, replace with actual log)." << endl;
}

// View total savings and current account amounts (for Employees)
void viewTotalAmount() {
    double totalSavings = 0.0;
    double totalCurrent = 0.0;

    // Iterate through all accounts and safely access the savings and current fields
    for (auto& account : accountsData.items()) {
        totalSavings += account.value().value("savings", 0.0);
        totalCurrent += account.value().value("current", 0.0);
    }

    cout << "Total Savings: " << totalSavings << "\nTotal Current: " << totalCurrent << endl;
}

// Deposit money into account
void deposit(Account& account) {
    double amount;
    string accountType;
    cout << "Enter amount to deposit: ";
    cin >> amount;

    cout << "Deposit to (savings/current): ";
    cin >> accountType;

    if (accountType == "savings") {
        account.savings += amount;
    } else if (accountType == "current") {
        account.current += amount;
    } else {
        cout << "Invalid account type!" << endl;
        return;
    }

    // Save updated balance to JSON
    accountsData[account.username]["savings"] = account.savings;
    accountsData[account.username]["current"] = account.current;
    saveAccounts();

    cout << "Deposit successful!" << endl;
}

// Withdraw money from account
void withdraw(Account& account) {
    double amount;
    string accountType;
    cout << "Enter amount to withdraw: ";
    cin >> amount;

    cout << "Withdraw from (savings/current): ";
    cin >> accountType;

    if (accountType == "savings" && account.savings >= amount) {
        account.savings -= amount;
    } else if (accountType == "current" && account.current >= amount) {
        account.current -= amount;
    } else {
        cout << "Insufficient balance or invalid account type!" << endl;
        return;
    }

    // Save updated balance to JSON
    accountsData[account.username]["savings"] = account.savings;
    accountsData[account.username]["current"] = account.current;
    saveAccounts();

    cout << "Withdrawal successful!" << endl;
}

// Send money to another account
void sendMoney(Account& sender) {
    string recipientUsername;
    string amountInput;
    double amount;
    string accountType;

    cout << "Enter recipient username: ";
    cin >> recipientUsername;

    // Check if the recipient exists
    if (!accountsData.contains(recipientUsername)) {
        cout << "Recipient account not found!" << endl;
        return;
    }

    cout << "Enter amount to send: ";
    while(true){
        cin>>amountInput;
        if (regex_match(amountInput,regex("^[0-9]+$"))){

            amount=stoi(amountInput);

            if (amount>0){

                break;
            }
        

        }


        cout<<"Invalid amount!\nPlease enter a valid positive whole number: ";
        cin.clear(); //Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(),'\n');


    }

    //cout << "Send from (savings/current): ";
    //cin >> accountType;

    /*if (accountType == "savings" && sender.savings >= amount) {
        sender.savings -= amount;
        accountsData[recipientUsername]["savings"] = accountsData[recipientUsername]["savings"].get<double>() + amount;
    } */
   if (recipientUsername != sender.username){
      

     if (sender.current >= amount) {
        sender.current -= amount;
        accountsData[recipientUsername]["current"] = accountsData[recipientUsername]["current"].get<double>() + amount;
    } else {
        cout << "Insufficient balance!" << endl;
        return;
    }

    // Save updated balances to JSON
    accountsData[sender.username]["savings"] = sender.savings;
    accountsData[sender.username]["current"] = sender.current;
    saveAccounts();

    cout << "Money sent successfully!" << endl;
    trackBalance(sender);

   }

   else{

    cout<<"\nYou can not send money to your own account!\n";
    return;
   }
   

   
}

// Request account modification (Customer)
void requestAccountModification(Account& account) {
    string modificationType;
    cout << "Enter the modification you want to request (e.g., change password): ";
    cin.ignore();  // Clear input buffer
    getline(cin, modificationType);

    // Dummy implementation - In a real system, this would create a modification request in a log or notify an admin
    cout << "Request to modify account (" << modificationType << ") submitted!" << endl;
}


// Customer Menu
void customerMenu(Account& account) {
    int choice;
    do {
        cout << "\n1. View Account\n2. Request Account Modification\n3. Withdraw\n4. Send Money\n5. Save\n6. View Current Balance\n7. View Savings Balance\n8. Logout\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewAccount(account);
                break;
            case 2:
                requestAccountModification(account);
                break;
            /*case :
                deposit(account);
                break;*/
            case 3:
                withdraw(account);
                break;
            case 4:
                sendMoney(account);
                break;
            case 5:
                //save();
                break;
            case 6:
                cout << "Current account balance: " << account.current << endl;
                break;
            case 7:
                cout << "Savings account balance: " << account.savings << endl;
                break;
            case 8:
                //cout << "Logging out..." << endl;
                mainMenu();
                break;

            case 9:
                cout << "Exiting..." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 8);
}


// Employee Menu
void employeeMenu(Account& employee) {
    int choice;
    do {
        cout << "\n1. Create Customer Account\n2. View Transaction History\n3. View Total Amount in Bank\n4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createCustomerAccount();
                break;
            case 2:
                viewTransactionHistory();
                break;
            case 3:
                viewTotalAmount();
                break;
            case 4:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 4);
}

// Administrator Menu
void adminMenu(Account& admin) {
    int choice;
    do {
        cout << "\n1. Create Employee Account\n2. Logout\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createEmployeeAccount();
                break;
            case 2:
                //cout << "Logging out..." << endl;
                mainMenu();
                break;
            case 3:
                cout << "Exiting..." << endl;
                exit(0);
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 2);
}

// Main Menu
void mainMenu() {
    authentification auth;
    int accountType;
    cout << "\n\n---------------\n";
    cout << "Welcome to the Bank\n";
    cout << "---------------\n";
    cout << "\n1. Administrator\n2. Employee\n3. Customer\n4. Exit\n";
    cout << "Enter your choice: ";
    cin >> accountType;
    cin.ignore();  // Clear the input buffer

    switch (accountType) {
        case 1: {
            Account admin = auth.login("Administrator");
            adminMenu(admin);
            break;
        }
        case 2: {
            Account employee = auth.login("Employee");
            employeeMenu(employee);
            break;
        }
        case 3: {
            Account customer = auth.login("Customer");
            customerMenu(customer);
            break;
        }
        case 4: {
            cout<<"\nExiting...";
            exit(0);
            break;
        }
        default:
            cout << "Invalid choice!" << endl;
    }
}

// Main Function
int main() {
    loadAccounts();  // Load accounts data from JSON file
    mainMenu();  // Show the main menu
    return 0;
}
