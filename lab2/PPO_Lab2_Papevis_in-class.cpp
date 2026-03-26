#include <iostream>
#include <string>
// #include <cctype> 

using std::cout, std::cin, std::string;

#define N 10

class Person {
  public:
    string firstName = "";   
    string lastName = "";
    string index = "";
};

// function to add students to the list
void inputList(Person students[]) {
    string temp;
    int studentAddAmount = 0;
    int studentAmount = 0;
    
    for (int i = 0; i < N; i++) {
        if (!students[i].firstName.empty()) studentAmount++;
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
            cin >> students[idx].index;
            bool onlyDigits = true;
            for(char c : students[idx].index) if(!isdigit(c)) onlyDigits = false;

            if (students[idx].index.length() == 6 && onlyDigits) break;
            cout << "Blad: indeks musi miec dokladnie 6 cyfr!\n";
        }

        cout << "Podaj imie: "; cin >> students[idx].firstName;
        cout << "Podaj nazwisko: "; cin >> students[idx].lastName;
    }
}

// function to assign attendance
void inputPresenceList(Person students[], bool *listPresence) {
    string temp;
    cout << "Wpisz: 1 jesli - \"tak\", 0 jesli - \"nie\"\n";
    for (int i = 0; i < N; i++) {
        if (!students[i].lastName.empty()) {
            while (true) {
                cout << "Czy " << students[i].lastName << " jest obecny?: ";
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
void printList(Person students[], bool *listPresence) {
    cout << "\nNr | Indeks | Imie | Nazwisko | Obecnosc\n";
    cout << "------------------------------------------\n";
    for (int i = 0; i < N; i++) {
        if (!students[i].lastName.empty()) {
            cout << i + 1 << ") " << students[i].index << " | " << students[i].firstName << " | " << students[i].lastName << " | " << listPresence[i] << "\n";
        }
    }
}

// function to edit a student via index search
void editStudent(Person students[], bool *listPresence) {
    printList(students,listPresence);
    string searchIndex;
    cout << "\nPodaj 6-cyfrowy indeks studenta do edycji: ";
    cin >> searchIndex;

    int foundIndex = -1;
    for (int i = 0; i < N; i++) {
        if (students[i].index == searchIndex && !students[i].index.empty()) {
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
            cout << "Nowe imie: "; cin >> students[foundIndex].firstName;
        } else if (subChoice == 2) {
            cout << "Nowe nazwisko: "; cin >> students[foundIndex].lastName;
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
void deleteStudent(Person students[], bool *listPresence) {
    string searchIndex;
    cout << "\nPodaj 6-cyfrowy indeks studenta do usuniecia: ";
    cin >> searchIndex;

    int deleteIndex = -1;
    for (int i = 0; i < N; i++) {
        if (students[i].index == searchIndex && !students[i].index.empty()) {
            deleteIndex = i;
            break;
        }
    }

    if (deleteIndex != -1) {
        for (int j = deleteIndex; j < N - 1; j++) {
            students[j].lastName = students[j + 1].lastName;
            students[j].firstName = students[j + 1].firstName;
            students[j].index = students[j + 1].index;
            listPresence[j] = listPresence[j + 1];
        }
        students[N - 1].lastName = "";
        students[N - 1].firstName = "";
        students[N - 1].index = "";
        listPresence[N - 1] = false;
        cout << "Student o indeksie " << searchIndex << " zostal usuniety\n";
    } else {
        cout << "Blad: Nie znaleziono studenta o takim indeksie\n";
    }
}

int main() {
    int choice;
    Person students[N];
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
            case 1: inputList(students); break;
            case 2: inputPresenceList(students, listPresence); break;
            case 3: printList(students, listPresence); break;
            case 4: editStudent(students, listPresence); break;
            case 5: deleteStudent(students, listPresence); break;
            case 0: cout << "Koniec programu\n"; break;
            default: cout << "Niepoprawna opcja\n";
        }
    } while (choice != 0);

    return 0;
}