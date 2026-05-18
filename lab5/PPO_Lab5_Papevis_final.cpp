#include <iostream>
#include <string>
#include <iomanip>

using std::cout, std::cin, std::string, std::setw, std::left;

#define N 10

class IPrintable {
public:
    virtual ~IPrintable() = default;
    virtual string introduce() const = 0;
};

string getIntroduction(IPrintable* obj) {
    return obj->introduce();
}

class Person : public IPrintable {  
protected:
    string firstName = "";   
    string lastName = "";

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

    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }

    string introduce() const override { return "Osoba"; }
    virtual string getIdentifier() const = 0;
};

class Student : public Person {
private:
    string studentId = "";

    bool validateIndex(string value) {
        if (value.length() != 6) return false;
        for (char c : value) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

public:
    int setStudentId(string value) {
        if (!validateIndex(value)) return -1;
        studentId = value;
        return 0;
    }

    string getStudentId() const { return studentId; }

    string introduce() const override { return "Student"; }
    string getIdentifier() const override { return studentId; }
};

class PartTimeStudent : public Student {
public:
    string introduce() const override { return "Stud. Zaoczny"; }
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
    int setWorkerId(string value) {
        if (!validateWorkerId(value)) return -1;
        workerId = value;
        return 0;
    }

    string getWorkerId() const { return workerId; }

    string introduce() const override { return "Pracownik"; }
    string getIdentifier() const override { return workerId; }
};

class PersonRegister {
private:
    Person* tabPeople[N];
    bool tabPresence_A[N]; 
    bool tabPresence_B[N]; 

    string truncateString(string str, size_t width) {
        if (str.length() > width) {
            return str.substr(0, width - 3) + "...";
        }
        return str;
    }

    void inputPresenceForList(bool* listPresence, string subjectName) {
        string temp;
        cout << "\n--- Sprawdzanie obecnosci: " << subjectName << " ---\n";
        cout << "Wpisz: 1 jesli - \"tak\", 0 jesli - \"nie\"\n";
        for (int i = 0; i < N; i++) {
            if (tabPeople[i] != nullptr) {
                while (true) {
                    cout << "Czy " << tabPeople[i]->getLastName() << " jest obecny?: ";
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
            tabPeople[i] = nullptr;
            tabPresence_A[i] = false;
            tabPresence_B[i] = false;
        }
    }

    ~PersonRegister() {
        for(int i = 0; i < N; i++) {
            if(tabPeople[i] != nullptr) {
                delete tabPeople[i];
            }
        }
    }

    void inputList() {
        string temp;
        int personAddAmount = 0;
        int personAmount = 0;
        
        for (int i = 0; i < N; i++) {
            if (tabPeople[i] != nullptr) personAmount++;
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
                cout << "Wybierz typ osoby (1 - Student, 2 - Pracownik, 3 - Student Zaoczny): ";
                cin >> typeChoice;
                if (typeChoice == "1" || typeChoice == "2" || typeChoice == "3") break;
                cout << "Blad: Nieprawidlowy wybor!\n";
            }

            string inputData;

            if (typeChoice == "1" || typeChoice == "3") {
                if (typeChoice == "1") {
                    tabPeople[idx] = new Student();
                } else {
                    tabPeople[idx] = new PartTimeStudent();
                }
                
                while (true) {
                    cout << "Podaj 6-cyfrowy index: ";
                    cin >> inputData;
                    if (((Student*)tabPeople[idx])->setStudentId(inputData) == 0) break;
                    cout << "Blad: indeks musi miec dokladnie 6 cyfr!\n";
                }
            } else {
                tabPeople[idx] = new Worker();
                while (true) {
                    cout << "Podaj ID pracownika (tylko cyfry): ";
                    cin >> inputData;
                    if (((Worker*)tabPeople[idx])->setWorkerId(inputData) == 0) break;
                    cout << "Blad: ID musi skladac sie z cyfr!\n";
                }
            }

            while (true) {
                cout << "Podaj imie: "; 
                cin >> inputData;
                if (tabPeople[idx]->setFirstName(inputData) == 0) break;
                cout << "Blad: Imie za krotkie!\n";
            }

            while (true) {
                cout << "Podaj nazwisko: "; 
                cin >> inputData;
                if (tabPeople[idx]->setLastName(inputData) == 0) break;
                cout << "Blad: Nazwisko za krotkie!\n";
            }
        }
    }

    void takeAttendance() {
        int choice;
        cout << "\nDla ktorego przedmiotu chcesz sprawdzic obecnosc?\n";
        cout << "1. Przedmiot A\n";
        cout << "2. Przedmiot B\n";
        cout << "Wybor: ";
        cin >> choice;

        if (choice == 1) {
            inputPresenceForList(tabPresence_A, "Przedmiot A");
        } else if (choice == 2) {
            inputPresenceForList(tabPresence_B, "Przedmiot B");
        } else {
            cout << "Nieprawidlowy wybor.\n";
        }
    }

    void printList() {
        cout << "\n" << left 
             << setw(5) << "Nr" << "| " 
             << setw(14) << "Typ" << "| " 
             << setw(11) << "Indeks/ID" << "| " 
             << setw(15) << "Imie" << "| " 
             << setw(20) << "Nazwisko" << "| " 
             << setw(6) << "Ob. A" << "| " 
             << setw(6) << "Ob. B" << "\n";
        
        cout << "-----------------------------------------------------------------------------------------\n";
        for (int i = 0; i < N; i++) {
            if (tabPeople[i] != nullptr) {
                string nrStr = std::to_string(i + 1) + ")";
                cout << left 
                     << setw(5) << nrStr << "| " 
                     << setw(14) << getIntroduction(tabPeople[i]) << "| " 
                     << setw(11) << truncateString(tabPeople[i]->getIdentifier(), 11) << "| " 
                     << setw(15) << truncateString(tabPeople[i]->getFirstName(), 15) << "| " 
                     << setw(20) << truncateString(tabPeople[i]->getLastName(), 20) << "| " 
                     << setw(6) << (tabPresence_A[i] ? "1" : "0") << "| " 
                     << setw(6) << (tabPresence_B[i] ? "1" : "0") << "\n";
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
            if (tabPeople[i] != nullptr && tabPeople[i]->getLastName() == searchName) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            int subChoice;
            cout << "\nCo chcesz edytowac dla osoby? " << searchName << "?\n";
            cout << "1. Imie\n2. Obecnosc (Przedmiot A)\n3. Obecnosc (Przedmiot B)\n0. Powrot\nWybor: "; 
            cin >> subChoice;

            string inputData;
            if (subChoice == 1) {
                cout << "Nowe imie: "; 
                cin >> inputData;
                if(tabPeople[foundIndex]->setFirstName(inputData) == -1) cout << "Blad walidacji!\n";
                else cout << "Dane zaktualizowane\n";
            } else if (subChoice == 2) {
                cout << "Obecnosc PA (1-tak, 0-nie): ";
                int p; cin >> p;
                tabPresence_A[foundIndex] = (p == 1);
                cout << "Dane zaktualizowane\n";
            } else if (subChoice == 3) {
                cout << "Obecnosc PB (1-tak, 0-nie): ";
                int p; cin >> p;
                tabPresence_B[foundIndex] = (p == 1);
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
            if (tabPeople[i] != nullptr && tabPeople[i]->getLastName() == searchName) {
                deleteIndex = i;
                break;
            }
        }

        if (deleteIndex != -1) {
            delete tabPeople[deleteIndex];

            for (int j = deleteIndex; j < N - 1; j++) {
                tabPeople[j] = tabPeople[j + 1];
                tabPresence_A[j] = tabPresence_A[j + 1];
                tabPresence_B[j] = tabPresence_B[j + 1];
            }
            
            tabPeople[N - 1] = nullptr;
            tabPresence_A[N - 1] = false;
            tabPresence_B[N - 1] = false;
            
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