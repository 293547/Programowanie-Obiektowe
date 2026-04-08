#include <iostream>
#include <string>

using std::cout, std::cin, std::string;

#define N 10

// function to add students to the list
void inputList(string *listLastNames, string *listFirstNames, string *listIndexes) {
    string temp;
    int studentAddAmount = 0;
    int studentAmount = 0;
    
    for (int i = 0; i < N; i++) {
        if (!listLastNames[i].empty()) studentAmount++;
    }

    while (true) {
        bool isValid = true;
        cout << "\nIle studentow chcesz dodac? (Max " << N - studentAmount << ", 0 aby wyjsc): ";
        cin >> temp;

        for (int i = 0; i < temp.length(); i++) {
            if (temp[i] < '0' || temp[i] > '9') {
                isValid = false;
                break; 
            }
        }

        if (isValid && !temp.empty()) {
            studentAddAmount = stoi(temp);
            if (studentAddAmount == 0) return;
            if (studentAddAmount + studentAmount <= N) {
                break; 
            } else {
                cout << "Blad: Przekroczono limit miejsc!\n";
            }
        } else {
            cout << "Blad: Wprowadzona wartosc nie jest liczba!\n";
        }
    }

    for (int i = 0; i < studentAddAmount; i++) {
        int idx = studentAmount + i;
        cout << "\n--- Student " << idx + 1 << " ---\n";
        
        while (true) {
            cout << "Podaj 6-cyfrowy index: ";
            cin >> listIndexes[idx];
            bool onlyDigits = true;
            for(char c : listIndexes[idx]) if(!isdigit(c)) onlyDigits = false;

            if (listIndexes[idx].length() == 6 && onlyDigits) break;
            cout << "Blad: indeks musi miec dokladnie 6 cyfr!\n";
        }

        cout << "Podaj imie: "; cin >> listFirstNames[idx];
        cout << "Podaj nazwisko: "; cin >> listLastNames[idx];
    }
}

// function to assign attendance
void inputPresenceList(string *listLastNames, bool *listPresence) {
    string temp;
    cout << "Wpisz: 1 jesli - \"tak\", 0 jesli - \"nie\"\n";
    for (int i = 0; i < N; i++) {
        if (!listLastNames[i].empty()) {
            while (true) {
                cout << "Czy " << listLastNames[i] << " jest obecny?: ";
                cin >> temp;
                if (temp == "1") {
                    listPresence[i] = true;
                    break;
                } else if (temp == "0") {
                    listPresence[i] = false;
                    break;
                }
                cout << "Nieprawidlowa wartosc, wpisz tylko '0' lub '1'\n";
            }
        }
    }
}

// function to print the student list
void printList(string *listLastNames, bool *listPresence, string *listFirstNames, string *listIndexes) {
    cout << "\nNr | Indeks | Imie | Nazwisko | Obecnosc\n";
    cout << "------------------------------------------\n";
    for (int i = 0; i < N; i++) {
        if (!listLastNames[i].empty()) {
            cout << i + 1 << ") " << listIndexes[i] << " | " << listFirstNames[i] << " | " << listLastNames[i] << " | " << listPresence[i] << "\n";
        }
    }
}

// function to edit a student via index search
void editStudent(string *listLastNames, string *listFirstNames, bool *listPresence, string *listIndexes) {
    printList(listLastNames, listPresence, listFirstNames, listIndexes);
    string searchIndex;
    cout << "\nPodaj 6-cyfrowy indeks studenta do edycji: ";
    cin >> searchIndex;

    int foundIndex = -1;
    for (int i = 0; i < N; i++) {
        if (listIndexes[i] == searchIndex && !listIndexes[i].empty()) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        int subChoice;
        cout << "\nCo chcesz edytowac " << searchIndex << "?\n";
        cout << "1. Imie\n2. Nazwisko\n3. Obecnosc\n0. Powrot\nWybor: ";
        cin >> subChoice;

        if (subChoice == 1) {
            cout << "Nowe imie: "; cin >> listFirstNames[foundIndex];
        } else if (subChoice == 2) {
            cout << "Nowe nazwisko: "; cin >> listLastNames[foundIndex];
        } else if (subChoice == 3) {
            cout << "Obecnosc (1-tak, 0-nie): ";
            int p; cin >> p;
            listPresence[foundIndex] = (p == 1);
        }
        cout << "Dane sa zaktualizowane\n";
    } else {
        cout << "Blad: Nie znaleziono studenta o takim indeksie\n";
    }
}

// function to delete student via index search
void deleteStudent(string *listLastNames, string *listFirstNames, string *listIndexes, bool *listPresence) {
    string searchIndex;
    cout << "\nPodaj 6-cyfrowy indeks studenta do usuniecia: ";
    cin >> searchIndex;

    int delIdx = -1;
    for (int i = 0; i < N; i++) {
        if (listIndexes[i] == searchIndex && !listIndexes[i].empty()) {
            delIdx = i;
            break;
        }
    }

    if (delIdx != -1) {
        for (int j = delIdx; j < N - 1; j++) {
            listLastNames[j] = listLastNames[j + 1];
            listFirstNames[j] = listFirstNames[j + 1];
            listIndexes[j] = listIndexes[j + 1];
            listPresence[j] = listPresence[j + 1];
        }
        listLastNames[N - 1] = "";
        listFirstNames[N - 1] = "";
        listIndexes[N - 1] = "";
        listPresence[N - 1] = false;
        cout << "Student o indeksie " << searchIndex << " zostal usuniety\n";
    } else {
        cout << "Blad: Nie znaleziono studenta o takim indeksie\n";
    }
}

int main() {
    int choice;
    string listLastNames[N];
    string listFirstNames[N];
    string listIndexes[N];
    bool listPresence[N] = {false}; 

    do {
        cout << "\n----- MENU LISTY STUDENTOW -----\n";
        cout << "1. Dodac studentow do tablicy\n";
        cout << "2. Przypisac obecnosc studentow\n";
        cout << "3. Wyswietl liste studentow\n";
        cout << "4. Edytuj dane studentow\n";
        cout << "5. Usun studenta\n";
        cout << "0. Wyjscie\n\n";
        cout << "Wybor: ";
        cin >> choice;

        switch (choice) {
            case 1: inputList(listLastNames, listFirstNames, listIndexes); break;
            case 2: inputPresenceList(listLastNames, listPresence); break;
            case 3: printList(listLastNames, listPresence, listFirstNames, listIndexes); break;
            case 4: editStudent(listLastNames, listFirstNames, listPresence, listIndexes); break;
            case 5: deleteStudent(listLastNames, listFirstNames, listIndexes, listPresence); break;
            case 0: cout << "Koniec programu\n"; break;
            default: cout << "Niepoprawna opcja\n";
        }
    } while (choice != 0);

    return 0;
}