#include "hello.h"
#include "name.h"
#include "age.h"
#include "end.h"
#include "sentence.h"


string Sentence::setup () {
    Hello ohello;
    Age oage;
    Name oname;
    End oend;

    string hi;
    string name;
    int age;


    cout << "\n How you usually say hi?: ";
    cin >> hi;

    cout << "\n What is your name?: ";
    cin >> name;

    cout << "\n How old are you?: ";
    cin >> age;

    return ohello.setup(hi) + oname.setup(name) + oage.setup(age) + oend.setup();
}