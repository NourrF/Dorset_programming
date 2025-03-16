#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Class to represent a single transaction : it keeps track of each operation (deposit, withdrawal, transfer)
// This allows users to see their transaction history.

class TransactionRecord {
private:
    string transactionType; // Type of transaction 
    double transactionAmount; // Amount involved in the transaction

public:
    // Constructor to initialize transaction details
    TransactionRecord(string type, double amount) : transactionType(type), transactionAmount(amount) {}

    // Method to display transaction details --> useful for showing the transaction history of an account
    void displayTransaction() const {
        cout << transactionType << ": €" << transactionAmount << endl;
    }
};

// Class to represent a bank account. It manges account information
class BankAccount {
private:
    int accountIdentifier; // For a unique account number
    double accountBalance; // For the current balance of the account
    vector<TransactionRecord> transactionHistory; // Transaction history of the account

public:
    // Constructor to initialize account details and create an account with a number and an initial balance 
    BankAccount(int accId, double initialBalance) 
        : accountIdentifier(accId), accountBalance(initialBalance) {}

    // Method to deposit money into the account. It checks that the amount is positive before updating the balance.
    void depositFunds(double amount) {
        if (amount > 0) {
            accountBalance += amount; // Update balance
            transactionHistory.push_back(TransactionRecord("Deposit", amount)); // Record transaction
            cout << "The Deposit is successful. Here is the New balance: €" << accountBalance << endl;
        } else {
            cout << "Oops ! This is an Invalid amount! Please enter a positive value." << endl;
        }
    }

    // Method to withdraw money from the account but it first checks if the balance is sufficient.
    // This is an important safeguard to prevent overdrafts.
    void withdrawFunds(double amount) {
        if (amount > 0 && amount <= accountBalance) {
            accountBalance -= amount; // Update balance
            transactionHistory.push_back(TransactionRecord("Withdrawal", amount)); // Record transaction
            cout << "The Withdrawal is successful. Here is the New balance: €" << accountBalance << endl;
        } else {
            cout << "Oops ! This is an Invalid withdrawal amount or insufficient balance." << endl;
        }
    }

    // Method to transfer money to another account. It checks that the amount is valid and that the balance is sufficient before proceeding.
    void transferFunds(BankAccount &recipientAccount, double amount) {
        if (amount > 0 && amount <= accountBalance) {
            accountBalance -= amount; // Deduct amount from the sender's account
            recipientAccount.accountBalance += amount; // Add amount to the recipient's account
            transactionHistory.push_back(TransactionRecord("Transfer to " + to_string(recipientAccount.accountIdentifier), amount)); // Record transaction
            recipientAccount.transactionHistory.push_back(TransactionRecord("Transfer from " + to_string(accountIdentifier), amount)); // Record transaction
            cout << "The Transfer is successful. Here is the New balance: €" << accountBalance << endl;
        } else {
            cout << "Oops ! Invalid transfer amount or insufficient balance." << endl;
        }
    }

    // Method to display account details and transaction history --> useful for users to see their balance and transaction history.
    void displayAccountDetails() const {
        cout << "Account Number: " << accountIdentifier << endl;
        cout << "Current Balance: €" << accountBalance << endl;
        cout << "Transaction History:" << endl;
        for (const auto &transaction : transactionHistory) {
            transaction.displayTransaction();
        }
    }

    // I added this method to allow access to the account number from other classes.
    int getAccountIdentifier() const {
        return accountIdentifier;
    }
};

// Class to represent a bank customer. It manages customer information, such as their name and accounts.
// This allows multiple accounts to be linked to a single customer, which is more realistic.
class Customer {
private:
    int customerIdentifier; // Unique customer ID
    string customerName; // Name of the customer
    string customerContactInfo; // Contact information of the customer
    vector<BankAccount> customerAccounts; // List of accounts owned by the customer

public:
    // Constructor to initialize customer details (ID, name, and contact information)
    Customer(int custId, string name, string contactInfo) 
        : customerIdentifier(custId), customerName(name), customerContactInfo(contactInfo) {}

    // This method allows creating an account and adding it to the customer's list of accounts.
    void addAccount(int accId, double initialBalance) {
        customerAccounts.push_back(BankAccount(accId, initialBalance)); // Add account to the list
        cout << "YAY ! Account added successfully!" << endl;
    }


    // This method is useful for displaying all information about a customer, including their accounts.
    void displayCustomerDetails() const {
        cout << "Customer ID: " << customerIdentifier << endl;
        cout << "Customer Name: " << customerName << endl;
        cout << "Contact Info: " << customerContactInfo << endl;
        cout << "Accounts:" << endl;
        for (const auto &account : customerAccounts) {
            account.displayAccountDetails();
            cout << "-------------------" << endl;
        }
    }

    // Method to get the customer ID
    int getCustomerIdentifier() const {
        return customerIdentifier;
    }

   
    // This method allows searching for a specific account among the customer's accounts.
    BankAccount* locateAccount(int accId) {
        for (auto &account : customerAccounts) {
            if (account.getAccountIdentifier() == accId) {
                return &account; // Return a pointer to the account if found
            }
        }
        return nullptr; // Return nullptr if the account is not found
    }
};

// Class to manage the banking system. This centralizes the management of customers and accounts.
class BankingSystem {
private:
    vector<Customer> allCustomers; // List of all customers in the system

public:
    // Method to create a new customer
    void createNewCustomer(int custId, string name, string contactInfo) {
        allCustomers.push_back(Customer(custId, name, contactInfo)); // Add customer to the list
        cout << "YAY ! Customer created successfully!" << endl;
    }

   
    // This method allows searching for a specific customer in the list of customers.
    Customer* locateCustomer(int custId) {
        for (auto &customer : allCustomers) {
            if (customer.getCustomerIdentifier() == custId) {
                return &customer; // Return a pointer to the customer if found
            }
        }
        return nullptr; // Return nullptr if the customer is not found
    }

    // Method to display details of all customers and their accounts
    void displayAllCustomerDetails() const {
        if (allCustomers.empty()) {
            cout << "Oops ! No customers found. Try again !" << endl;
        } else {
            for (const auto &customer : allCustomers) {
                customer.displayCustomerDetails();
                cout << "===================" << endl;
            }
        }
    }
};

// Main function to run the banking system
int main() {
    BankingSystem bankingApp; // Create an instance of the BankingSystem
    int userChoice;

    do {
        // The main menu
        cout << "\n--- Banking System Menu ---\n";
        cout << "1. Create Customer\n";
        cout << "2. Create Account\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Transfer Money\n";
        cout << "6. Display All Customers\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> userChoice;

        switch (userChoice) {
            case 1: {
                // Create a new customer
                int custId;
                string name, contactInfo;
                cout << "Please enter the customer ID: ";
                cin >> custId;
                cout << "Please enter the customer name: ";
                cin.ignore(); // Clear input buffer
                getline(cin, name); // Use getline to handle spaces in names
                cout << "Please enter contact info: ";
                getline(cin, contactInfo);
                bankingApp.createNewCustomer(custId, name, contactInfo);
                break;
            }
            case 2: {
                // Create a new account for a customer
                int custId, accId;
                double initialBalance;
                cout << "Please enter customer ID: ";
                cin >> custId;
                cout << "Please enter account number: ";
                cin >> accId;
                cout << "Please enter initial deposit: ";
                cin >> initialBalance;
                Customer* customer = bankingApp.locateCustomer(custId);
                if (customer) {
                    customer->addAccount(accId, initialBalance);
                } else {
                    cout << "Oops ! Customer not found. Try again !" << endl;
                }
                break;
            }
            case 3: {
                // Deposit money into an account
                int custId, accId;
                double depositAmount;
                cout << "Please enter customer ID: ";
                cin >> custId;
                cout << "Please enter account number: ";
                cin >> accId;
                cout << "Please enter amount to deposit: ";
                cin >> depositAmount;
                Customer* customer = bankingApp.locateCustomer(custId);
                if (customer) {
                    BankAccount* account = customer->locateAccount(accId);
                    if (account) {
                        account->depositFunds(depositAmount);
                    } else {
                        cout << "Oops ! Account not found. Try again !" << endl;
                    }
                } else {
                    cout << "Oops ! Customer not found. Try again !" << endl;
                }
                break;
            }
            case 4: {
                // Withdraw money from an account
                int custId, accId;
                double withdrawalAmount;
                cout << "Please enter customer ID: ";
                cin >> custId;
                cout << "Please enter account number: ";
                cin >> accId;
                cout << "Please enter amount to withdraw: ";
                cin >> withdrawalAmount;
                Customer* customer = bankingApp.locateCustomer(custId);
                if (customer) {
                    BankAccount* account = customer->locateAccount(accId);
                    if (account) {
                        account->withdrawFunds(withdrawalAmount);
                    } else {
                        cout << "Oops ! Account not found. Try again !" << endl;
                    }
                } else {
                    cout << "Oops ! Customer not found. Try again !" << endl;
                }
                break;
            }
            case 5: {
                // Transfer money between accounts
                int fromCustId, toCustId, fromAccId, toAccId;
                double transferAmount;
                cout << "Please enter source customer ID: ";
                cin >> fromCustId;
                cout << "Please enter source account number: ";
                cin >> fromAccId;
                cout << "Please enter destination customer ID: ";
                cin >> toCustId;
                cout << "Please enter destination account number: ";
                cin >> toAccId;
                cout << "Please enter amount to transfer: ";
                cin >> transferAmount;
                Customer* fromCustomer = bankingApp.locateCustomer(fromCustId);
                Customer* toCustomer = bankingApp.locateCustomer(toCustId);
                if (fromCustomer && toCustomer) {
                    BankAccount* fromAccount = fromCustomer->locateAccount(fromAccId);
                    BankAccount* toAccount = toCustomer->locateAccount(toAccId);
                    if (fromAccount && toAccount) {
                        fromAccount->transferFunds(*toAccount, transferAmount);
                    } else {
                        cout << "Oops ! One or both accounts not found." << endl;
                    }
                } else {
                    cout << "Oops ! One or both customers not found." << endl;
                }
                break;
            }
            case 6: {
                // Display details of all customers and their accounts
                bankingApp.displayAllCustomerDetails();
                break;
            }
            case 7: {
                // Exit the program
                cout << "Goodbye! Have a nice day :)" << endl;
                break;
            }
            default: {
                // Handle invalid choices
                cout << "Oh no ! Unvalid choice. Please try again." << endl;
                break;
            }
        }
    } while (userChoice != 7);

    return 0;
}