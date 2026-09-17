#include <iostream>
#include <string>

using namespace std;

void damageValue (int health)
{
    health -= 20;
    cout << "\nValue: " << health;
}

void damageReference (int& health) {
    health -= 20;
    cout << "\nValue from ref: " << health;
}

void damagePointer (int* health) {
    *health -= 20;
    cout << "\nValue from pointer: " << *health;
    cout << "\nPointer: " << health;
}

int main () {
    int help;
    int health = 100;
    cout << "\nStart: " << health;

    damageValue(health);
    cout << "\nafterV: " << health;
    damageReference(health);
    cout << "\nafterR: " << health;
    damagePointer(&health);
    cout << "\nafterP: " << health;

    cin >> help;

    return 0;
}