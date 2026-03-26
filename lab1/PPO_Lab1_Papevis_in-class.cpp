#include <iostream>
#include <string>

using namespace std;

const int N = 10; 

void inputList(string listLastNames[]) {
    cout << "\n";
    for (int i = 0; i < N; i++) {
        cout << "Podaj nazwisko studenta numer " << (i + 1) << " do tablicy: ";
        cin >> listLastNames[i];
    }
}

void inputPresenceList(int listPresence[], string listLastNames[]) {
    cout << "\nObecnosc Wpisz: 1 jesli - \"tak\", 0 jesli - \"nie\"\n";
    for (int i = 0; i < N; i++) {
        cout << "Czy " << listLastNames[i] << " jest obecny? ";
        cin >> listPresence[i];
    }
}

void printList(string listLastNames[], int listPresence[]) {
    cout << "\nWyswietlona lista studentow, (obecny: 1 - tak, 0 - nie))\n";
    cout << "Nazwisko | Obecnosc\n";
    cout << "-------------------\n";
    for (int i = 0; i < N; i++) {
        cout << listLastNames[i] << " | " << listPresence[i] << "\n";
    }
}

int main() {
    string listLastNames[N];
    
    int listPresence[N] = {0}; 
    int choice;

    do {
        cout << "\n---------MENU Lista---------\n";
        cout << "1. Dodac studentow do tablicy\n";
        cout << "2. Przypisac obecnosc studentow\n";
        cout << "3. Wyswietlij cala liste studentow\n";
        cout << "0. Wyjscie\n";
        cout << "Wybor: ";
        cin >> choice;

        switch(choice) {
            case 1: 
                inputList(listLastNames); 
                break;
            case 2: 
                inputPresenceList(listPresence, listLastNames); 
                break;
            case 3: 
                printList(listLastNames, listPresence);
                break;
            case 0: 
                cout << "Koniec programu\n"; 
                break;
            default: 
                cout << "Niepoprawna opcja\n"; 
                break;
        }
    } while (choice != 0);

    return 0;
}