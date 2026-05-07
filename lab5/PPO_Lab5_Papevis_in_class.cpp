#include <iostream>
#include <string>

using std::cout, std::cin, std::string;

#define N 10

class Person {  
protected:
    string firstName = "";   
    string lastName = "";
    string role = "";

    bool validateString(string value) {
        return value.length() >= 2;
    }

public:
    virtual ~Person() = default;

    int setFirstName(string value) {
        if (!validateString(value)) return -1;
        firstName = value;
        return 0;
    }

    int setLastName(string value) {
        if (!validateString(value)) return -1;
        lastName = value;
        return 0;
    }

    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    string getRole() { return role; }

    virtual string getIdentifier() { 
        return "-"; 
    }
};

class Student : public Person {
private:
    string index = "";

    bool validateIndex(string value) {
        if (value.length() != 6) return false;
        for (char c : value) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

public:
    Student() {
        role = "Student";
    }

    int setIndex(string value) {
        if (!validateIndex(value)) return -1;
        index = value;
        return 0;
    }

    string getIdentifier() override { 
        return index; 
    }
};

class Worker : public Person {
private:
    string workerId = "";

    bool validateWorkerId(string value) {
        if (value.length() < 1) return false;
        for (char c : value) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

public:
    Worker() {
        role = "Pracownik";
    }

    int setWorkerId(string value) {
        if (!validateWorkerId(value)) return -1;
        workerId = value;
        return 0;
    }

    string getIdentifier() override { 
        return workerId; 
    }
};

class PersonRegister {
private:
    Person* people[N];
    bool presenceSubject1[N]; 
    bool presenceSubject2[N]; 

    void inputPresenceForList(bool* listPresence, string subjectName) {
        string temp;
        cout << "\n--- Sprawdzanie obecnosci: " << subjectName << " ---\n";
        cout << "Wpisz: 1 jesli - \"tak\", 0 jesli - \"nie\"\n";
        for (int i = 0; i < N; i++) {
            if (people[i] != nullptr) {
                while (true) {
                    cout << "Czy " << people[i]->getLastName() << " jest obecny?: ";
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

public:
    PersonRegister() {
        for (int i = 0; i < N; i++) {
            people[i] = nullptr;
            presenceSubject1[i] = false;
            presenceSubject2[i] = false;
        }
    }

    ~PersonRegister() {
        for(int i = 0; i < N; i++) {
            if(people[i] != nullptr) {
                delete people[i];
            }
        }
    }

    void inputList() {
        string temp;
        int personAddAmount = 0;
        int personAmount = 0;
        
        for (int i = 0; i < N; i++) {
            if (people[i] != nullptr) personAmount++;
        }

        while (true) {
            bool isValid = true;
            cout << "\nIle osob chcesz dodac? (Max " << N - personAmount << ", 0 aby wyjsc): ";
            cin >> temp;

            for (int i = 0; i < temp.length(); i++) {
                if (!isdigit(temp[i])) {
                    isValid = false;
                    break; 
                }
            }

            if (isValid && !temp.empty()) {
                personAddAmount = stoi(temp);
                if (personAddAmount == 0) return;
                if (personAddAmount + personAmount <= N) {
                    break; 
                } else {
                    cout << "Blad: Przekroczono limit miejsc!\n";
                }
            } else {
                cout << "Blad: Wprowadzona wartosc nie jest liczba!\n";
            }
        }

        for (int i = 0; i < personAddAmount; i++) {
            int idx = personAmount + i;
            string typeChoice;
            
            while (true) {
                cout << "\n--- Osoba " << idx + 1 << " ---\n";
                cout << "Wybierz typ osoby (1 - Student, 2 - Pracownik): ";
                cin >> typeChoice;
                if (typeChoice == "1" || typeChoice == "2") break;
                cout << "Blad: Nieprawidlowy wybor!\n";
            }

            string inputData;

            if (typeChoice == "1") {
                people[idx] = new Student(); 
                while (true) {
                    cout << "Podaj 6-cyfrowy index: ";
                    cin >> inputData;
                    if (((Student*)people[idx])->setIndex(inputData) == 0) break;
                    cout << "Blad: indeks musi miec dokladnie 6 cyfr!\n";
                }
            } else {
                people[idx] = new Worker();
                while (true) {
                    cout << "Podaj ID pracownika (tylko cyfry): ";
                    cin >> inputData;
                    if (((Worker*)people[idx])->setWorkerId(inputData) == 0) break;
                    cout << "Blad: ID musi skladac sie z cyfr!\n";
                }
            }

            while (true) {
                cout << "Podaj imie: "; 
                cin >> inputData;
                if (people[idx]->setFirstName(inputData) == 0) break;
                cout << "Blad: Imie za krotkie!\n";
            }

            while (true) {
                cout << "Podaj nazwisko: "; 
                cin >> inputData;
                if (people[idx]->setLastName(inputData) == 0) break;
                cout << "Blad: Nazwisko za krotkie!\n";
            }
        }
    }

    void takeAttendance() {
        int choice;
        cout << "\nDla ktorego przedmiotu chcesz sprawdzic obecnosc?\n";
        cout << "1. Przedmiot 1 (Lista 1)\n";
        cout << "2. Przedmiot 2 (Lista 2)\n";
        cout << "Wybor: ";
        cin >> choice;

        if (choice == 1) {
            inputPresenceForList(presenceSubject1, "Przedmiot 1");
        } else if (choice == 2) {
            inputPresenceForList(presenceSubject2, "Przedmiot 2");
        } else {
            cout << "Nieprawidlowy wybor.\n";
        }
    }

    void printList() {
        cout << "\nNr | Typ | Indeks/ID | Imie | Nazwisko | Ob. 1 | Ob. 2\n";
        cout << "--------------------------------------------------------\n";
        for (int i = 0; i < N; i++) {
            if (people[i] != nullptr) {
                cout << i + 1 << ") " << people[i]->getRole() << " | "
                     << people[i]->getIdentifier() << " | " 
                     << people[i]->getFirstName() << " | " 
                     << people[i]->getLastName() << " |   " 
                     << presenceSubject1[i] << "   |   " << presenceSubject2[i] << "\n";
            }
        }
    }

    void editPerson() {
        printList();
        string searchName;
        cout << "\nPodaj nazwisko osoby do edycji: "; 
        cin >> searchName;

        int foundIndex = -1;
        for (int i = 0; i < N; i++) {
            if (people[i] != nullptr && people[i]->getLastName() == searchName) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            int subChoice;
            string role = people[foundIndex]->getRole();
            
            cout << "\nCo chcesz edytowac dla osoby? " << searchName << "?\n";
            cout << "1. Imie\n";
            if (role == "Student") {
                cout << "2. Indeks\n";
            } else {
                cout << "2. ID Pracownika\n";
            }
            cout << "3. Obecnosc (Przedmiot 1)\n4. Obecnosc (Przedmiot 2)\n0. Powrot\nWybor: "; 
            cin >> subChoice;

            string inputData;
            if (subChoice == 1) {
                cout << "Nowe imie: "; 
                cin >> inputData;
                if(people[foundIndex]->setFirstName(inputData) == -1) cout << "Blad walidacji!\n";
                else cout << "Dane zaktualizowane\n";
            } else if (subChoice == 2) {
                if (role == "Student") {
                    cout << "Nowy indeks: "; 
                    cin >> inputData;
                    if(((Student*)people[foundIndex])->setIndex(inputData) == -1) cout << "Blad walidacji!\n";
                    else cout << "Dane zaktualizowane\n";
                } else {
                    cout << "Nowe ID Workera: "; 
                    cin >> inputData;
                    if(((Worker*)people[foundIndex])->setWorkerId(inputData) == -1) cout << "Blad walidacji!\n";
                    else cout << "Dane zaktualizowane\n";
                }
            } else if (subChoice == 3) {
                cout << "Obecnosc P1 (1-tak, 0-nie): ";
                int p; cin >> p;
                presenceSubject1[foundIndex] = (p == 1);
                cout << "Dane zaktualizowane\n";
            } else if (subChoice == 4) {
                cout << "Obecnosc P2 (1-tak, 0-nie): ";
                int p; cin >> p;
                presenceSubject2[foundIndex] = (p == 1);
                cout << "Dane zaktualizowane\n";
            }
        } else {
            cout << "Blad: Nie znaleziono osoby o takim nazwisku\n";
        }
    }

    void deletePerson() {
        string searchName;
        cout << "\nPodaj nazwisko osoby do usuniecia: "; 
        cin >> searchName;

        int deleteIndex = -1;
        for (int i = 0; i < N; i++) {
            if (people[i] != nullptr && people[i]->getLastName() == searchName) {
                deleteIndex = i;
                break;
            }
        }

        if (deleteIndex != -1) {
            delete people[deleteIndex];

            for (int j = deleteIndex; j < N - 1; j++) {
                people[j] = people[j + 1];
                presenceSubject1[j] = presenceSubject1[j + 1];
                presenceSubject2[j] = presenceSubject2[j + 1];
            }
            
            people[N - 1] = nullptr;
            presenceSubject1[N - 1] = false;
            presenceSubject2[N - 1] = false;
            
            cout << "Osoba o nazwisku " << searchName << " zostala usunieta\n";
        } else {
            cout << "Blad: Nie znaleziono osoby o takim nazwisku\n";
        }
    }
};

int main() {
    int choice;
    PersonRegister registerSystem; 

    do {
        cout << "\n------- MENU LISTY OSOB -------\n";
        cout << "1. Dodac osob do tablicy\n";
        cout << "2. Przypisac obecnosc osob\n";
        cout << "3. Wyswietl liste osob\n";
        cout << "4. Edytuj dane osob\n";
        cout << "5. Usun osobe\n";
        cout << "0. Wyjscie\n\n";
        cout << "Wybor: ";
        cin >> choice;

        switch (choice) {
            case 1: registerSystem.inputList(); break;
            case 2: registerSystem.takeAttendance(); break;
            case 3: registerSystem.printList(); break;
            case 4: registerSystem.editPerson(); break;
            case 5: registerSystem.deletePerson(); break;
            case 0: cout << "Koniec programu\n"; break;
            default: cout << "Niepoprawna opcja\n";
        }
    } while (choice != 0);

    return 0;
}
