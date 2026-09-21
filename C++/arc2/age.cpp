#include "age.h"
#include <iostream>

string Age::setup (int age) {
    return "and I'm " + std::to_string(age) + " year old,";
}