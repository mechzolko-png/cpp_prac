#include <iostream>
#include <string>

class PRINT {
    void print (std::string data) {
        std::cout << "\n" << data;
    };

    void print (int data) {
        std::cout << "\n" << data;
    };

    void print (double data) {
        std::cout << "\n" << data;
    };
};


int square (int a) {
    return a*a;
};

bool isEven (int a) {
    return a % 2 == 0;
};


void print (std::string data) {
    std::cout << "\n" << data;
}

void print (int data) {
    std::cout << "\n" << data;
}

void print (double data) {
    std::cout << "\n" << data;
}

int add (int a, int b) {
    return a + b; 
}

int multiply (int a, int b) {
    return a * b;
}   

int main () {
    std::string name;
    int age;
    int favouritNumb;
    int help;

    double PI = 3.14;
    bool run = true;
    char ID = 'A';
    int num = 1;
    std::string word = "Badagang";

    bool likeCode;
    char fl;


    print("Name> ");std::cin >> name;
    print("age> ");std::cin >> age;
    print("favourite number> ");std::cin >> favouritNumb;
    std::cout << "First letter in your name> "; std::cin >> fl;
    std::cout << "Your name is " << name << "\n" << "You are " << age << " years old." << "\n" << "Your favourite number is " << favouritNumb << "\n";

    std::cout << "\n";

    if (age >= 18) {
        std::cout << "You are an adult.";
    } else if (age < 18) {
        std::cout << "You are a kid.";
    } else {
        std::cout << "Something went wrong.";
    }

    std::cout << "\n" << "\n" ;

    if (age <= 18 && age >= 13 ) {
        std::cout << "You are a teen";
    } else if (age == 7 || age == 19) {
        std::cout << "You are started learn or finished learn";
    } else {
        std::cout << "Something went wrong in the second part of ifs";
    }


    print("Hello from c++");
    int favNage = add(age,favouritNumb);
    int multipla = multiply(age,favouritNumb);
    print(favNage);
    print(multipla);
    print(3.14);

    std::cout << "\n" << "\n" ;

    for (int i = 1; i <= 5; i++) {
        print(i);
    }

    std::cout << "\n" << "\n" ;

    int i = 1;
    while (i <= 5) {
        print(i);
        i++;
    }

    std::cout << "\n" << "\n" ;

    i = 1;
    do {
        print(i);
        i++;
    } while (i <= 5);

    std::cout << "\n" << "\n" ;

    std::cout << "Your fav. number on the power of 2: ";std::cout << square(favouritNumb); 

    std::cout << "\n" << "\n" ;

    std::cout << "your age is even: "; std::cout << isEven(age);

    std::cin >> help;
    return 0;
}