#include <iostream>
#include <string>

class BankAccount {

private:

std::string password;
std::string username;
int balance = 0;
bool isLoggedIn = false;

public:

void registerAccount() {
    std::cout << "Username: ";
    std::cin >> username;

    std::cout << "Password: ";
    std::cin >> password;

    std::cout << "Account registered!\n";
}

void login() {
    std::string user;
    std::string pass;

    std::cout << "Username: ";
    std::cin >> user;

    std::cout << "Password: ";
    std::cin >> pass;

    if (user == username && pass == password) {
        isLoggedIn = true;
        std::cout << "Logged in!\n";
    }
    else {
        std::cout << "Wrong username or password!\n";
    }
}

void deposit() {
    int amount;

    std::cout << "Amount to deposit: ";
    std::cin >> amount;

    if (isLoggedIn && amount > 0) {
        balance += amount;
    }
}

void withdraw() {
    int amount;

    std::cout << "Amount to withdraw: ";
    std::cin >> amount;

    if (isLoggedIn && amount > 0 && amount <= balance) {
        balance -= amount;
    }
}

void returnMe() {
    std::cout << "Username: " << username << "\n";
    std::cout << "Balance: " << balance << "\n";
    std::cout << "Logged in: " << isLoggedIn << "\n";
}


};

int main() {


BankAccount account;

bool run = true;
std::string CMD;

while (run) {

    std::cout << "\nCommand: ";
    std::cin >> CMD;

    if (CMD == "register") {
        account.registerAccount();
    }

    else if (CMD == "login") {
        account.login();
    }

    else if (CMD == "deposit") {
        account.deposit();
    }

    else if (CMD == "withdraw") {
        account.withdraw();
    }

    else if (CMD == "me") {
        account.returnMe();
    }

    else if (CMD == "quit") {
        run = false;
    }

    else {
        std::cout << "There is no command called " << CMD << "\n";
    }
}

return 0;
}