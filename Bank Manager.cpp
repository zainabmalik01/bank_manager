#include <iostream>
#include <cstdlib>
#include <ctime>

class BankAccount {
private:
    int accountId;
    std::string username;
    std::string password;
    std::string accountType;
    double balance;

public:
    BankAccount(const std::string& uname = "", const std::string& pwd = "", const std::string& type = "", double initialBalance = 0.0)
        : username(uname), password(pwd), accountType(type), balance(initialBalance) {

        srand(static_cast<unsigned>(time(0)));
        accountId = rand() % 9000 + 1000;
    }

    int getAccountId() const { return accountId; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        balance += amount;
        std::cout << "Deposit of $" << amount << " successful. New balance: $" << balance << std::endl;
    }

    void withdraw(double amount) {
        if (amount > balance) {
            std::cout << "Insufficient funds. Withdrawal failed." << std::endl;
        }
        else {
            balance -= amount;
            std::cout << "Withdrawal of $" << amount << " successful. New balance: $" << balance << std::endl;
        }
    }

    void setPassword(const std::string& newPassword) {
        password = newPassword;
    }

    void setUsername(const std::string& newUsername) {
        username = newUsername;
    }

    void setInitialBalance(double newInitialBalance) {
        balance = newInitialBalance;
    }
};

class BankManager {
private:
    static const int MAX_ACCOUNTS = 100;
    BankAccount accounts[MAX_ACCOUNTS];
    BankAccount* currentAccount;
    int numAccounts;

    BankAccount* findAccountById(int accountId) {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i].getAccountId() == accountId) {
                return &accounts[i];
            }
        }
        return nullptr;
    }

public:
    BankManager() : numAccounts(0), currentAccount(nullptr) {}

    void signup() {
        std::string username, password, accountType;
        double initialBalance;

        std::cout << "Enter username: ";
        std::cin >> username;

        std::cout << "Enter password: ";
        std::cin >> password;

        while (true) {
            std::cout << "Enter account type (Saving/Current): ";
            std::cin >> accountType;

            if (accountType == "Saving" || accountType == "saving" || accountType == "Current" || accountType == "current") {
                break;
            }
            else {
                std::cout << "You've entered the wrong option, try again with the correct option: " << std::endl;
            }
        }

        std::cout << "Enter initial balance: $";
        std::cin >> initialBalance;

        if (numAccounts < MAX_ACCOUNTS) {
            accounts[numAccounts++] = BankAccount(username, password, accountType, initialBalance);
            std::cout << "Account created successfully. Your account ID is: " << accounts[numAccounts - 1].getAccountId() << std::endl;
        }
        else {
            std::cout << "Maximum number of accounts reached. Cannot create a new account." << std::endl;
        }
    }

    void login() {
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;

        std::cout << "Enter password: ";
        std::cin >> password;

        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i].getUsername() == username && accounts[i].getPassword() == password) {
                currentAccount = &accounts[i];
                std::cout << "Login successful. Welcome, " << username << "!\n";
                return;
            }
        }

        std::cout << "Invalid username or password. Login failed.\n";
    }

    void deposit() {
        double amount;
        std::cout << "Enter deposit amount: $";
        std::cin >> amount;

        currentAccount->deposit(amount);
    }

    void withdraw() {
        double amount;
        std::cout << "Enter withdrawal amount: $";
        std::cin >> amount;

        currentAccount->withdraw(amount);
    }

    void accountSettings() {
        std::cout << "Account ID: " << currentAccount->getAccountId() << std::endl;
        std::cout << "Username: " << currentAccount->getUsername() << std::endl;
        std::cout << "Account Type: " << currentAccount->getAccountType() << std::endl;
        std::cout << "Balance: $" << currentAccount->getBalance() << std::endl;
    }

    void logout() {
        std::cout << "Logging out. Goodbye, " << currentAccount->getUsername() << "!\n";
        currentAccount = nullptr;
    }

    void displayOptions() {
        std::cout << "\n1. Deposit\n2. Withdraw\n3. Transfer\n4. Account Settings\n5. Logout\n";
    }

    void performTransaction() {
        char choice;

        do {
            displayOptions();
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case '1':
                deposit();
                break;
            case '2':
                withdraw();
                break;
            case '3':
                transferFunds();
                break;
            case '4':
                int option;
                std::cout << std::endl;
                std::cout << std::endl;
                accountSettings();

                std::cout << "\nDo you want to change account settings?\n1. Yes\n0. No [EXIT]" << std::endl;
                std::cin >> option;
                if (option == 1) {
                    changeAccountSettings();
                }
                else {
                    std::cout << "Exiting Safely...";
                }
                break;

            case '5':
                logout();
                return;

            default:
                std::cout << "Invalid choice. Please try again.\n";
            }
        } while (true);
    }

    void changeAccountSettings() {
        std::string newPassword;
        std::string newUsername;
        std::cout << "Enter a new username: ";
        std::cin >> newUsername;

        std::cout << "Enter a new password: ";
        std::cin >> newPassword;

        currentAccount->setPassword(newPassword);
        currentAccount->setUsername(newUsername);
        std::cout << "Account settings updated successfully.\n";
    }

    void transferFunds() {
        int destinationAccountId;
        double amount;

        std::cout << "Enter destination account ID: ";
        std::cin >> destinationAccountId;

        BankAccount* destinationAccount = findAccountById(destinationAccountId);

        if (destinationAccount == nullptr) {
            std::cout << "Destination account not found.\n";
            return;
        }

        std::cout << "Enter transfer amount: $";
        std::cin >> amount;

        if (amount > 0 && amount <= currentAccount->getBalance()) {
            currentAccount->withdraw(amount);
            destinationAccount->deposit(amount);
            std::cout << "Funds transferred successfully.\n";
        }
        else {
            std::cout << "Invalid transfer amount or insufficient funds.\n";
        }
    }

    BankAccount* getCurrentAccount() { return currentAccount; }
};

int main() {
    BankManager bankManager;
    char choice;

    do {
        std::cout << "\n1. Login\n2. Signup\n3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case '1':
            bankManager.login();
            if (bankManager.getCurrentAccount() != nullptr) {
                bankManager.performTransaction();
            }
            break;
        case '2':
            bankManager.signup();
            break;
        case '3':
            std::cout << "Exiting program. Have a nice day!\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '3');

    return 0;
}
