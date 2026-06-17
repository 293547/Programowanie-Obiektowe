#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using std::cout, std::cin, std::string, std::setw, std::left;

#define N 10

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual string serialize() const = 0;
};

class Person {  
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

    virtual string getType() const = 0;
    virtual string getIdentifier() const = 0;
};

class Student : public Person, public ISerializable {
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

    string getType() const override { return "Student"; }
    string getIdentifier() const override { return studentId; }
    
    string serialize() const override { 
        return getType() + ";" + studentId + ";" + firstName + ";" + lastName; 
    }
};

class PartTimeStudent : public Person, public ISerializable {
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

    string getType() const override { return "Stud. Zaoczny"; }
    string getIdentifier() const override { return studentId; }
    
    string serialize() const override { 
        return getType() + ";" + studentId + ";" + firstName + ";" + lastName; 
    }
};

class Worker : public Person, public ISerializable {
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

    string getType() const override { return "Prowadzacy"; }
    string getIdentifier() const override { return workerId; }
    
    string serialize() const override { 
        return getType() + ";" + workerId + ";" + firstName + ";" + lastName; 
    }
};

class AttendanceList {
public:
    Person* tabPeople[N];
    bool tabPresence_A[N]; 
    bool tabPresence_B[N]; 

    AttendanceList() {
        for (int i = 0; i < N; i++) {
            tabPeople[i] = nullptr;
            tabPresence_A[i] = false;
            tabPresence_B[i] = false;
        }
    }

    ~AttendanceList() {
        for(int i = 0; i < N; i++) {
            if(tabPeople[i] != nullptr) {
                delete tabPeople[i];
            }
        }
    }
};

class UserInterface {
private:
    AttendanceList list;

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
            if (list.tabPeople[i] != nullptr) {
                while (true) {
                    cout << "Czy " << list.tabPeople[i]->getLastName() << " jest obecny?: ";
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

    void writeToFile(ISerializable* objects[], int count, string filename) {
        std::ofstream file(filename, std::ios::app);
        if (!file) {
            cout << "Blad otwarcia pliku!\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            if (objects[i] != nullptr) {
                file << objects[i]->serialize() << "\n";
            }
        }
        
        file.close();
        cout << "Pomyslnie zapisano " << count << " rekordow do pliku " << filename << ".\n";
    }

    void inputList() {
        string temp;
        int personAddAmount = 0;
        int personAmount = 0;
        
        for (int i = 0; i < N; i++) {
            if (list.tabPeople[i] != nullptr) personAmount++;
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
                cout << "Wybierz typ osoby (1 - Student, 2 - Prowadzacy, 3 - Student Zaoczny): ";
                cin >> typeChoice;
                if (typeChoice == "1" || typeChoice == "2" || typeChoice == "3") break;
                cout << "Blad: Nieprawidlowy wybor!\n";
            }

            string inputData;

            if (typeChoice == "1") {
                list.tabPeople[idx] = new Student();
                while (true) {
                    cout << "Podaj 6-cyfrowy index: ";
                    cin >> inputData;
                    if (dynamic_cast<Student*>(list.tabPeople[idx])->setStudentId(inputData) == 0) break;
                    cout << "Blad: indeks musi miec dokladnie 6 cyfr!\n";
                }
            } else if (typeChoice == "3") {
                list.tabPeople[idx] = new PartTimeStudent();
                while (true) {
                    cout << "Podaj 6-cyfrowy index: ";
                    cin >> inputData;
                    if (dynamic_cast<PartTimeStudent*>(list.tabPeople[idx])->setStudentId(inputData) == 0) break;
                    cout << "Blad: indeks musi miec dokladnie 6 cyfr!\n";
                }
            } else {
                list.tabPeople[idx] = new Worker();
                while (true) {
                    cout << "Podaj ID prowadzacego (tylko cyfry): ";
                    cin >> inputData;
                    if (dynamic_cast<Worker*>(list.tabPeople[idx])->setWorkerId(inputData) == 0) break;
                    cout << "Blad: ID musi skladac sie z cyfr!\n";
                }
            }

            while (true) {
                cout << "Podaj imie: "; 
                cin >> inputData;
                if (list.tabPeople[idx]->setFirstName(inputData) == 0) break;
                cout << "Blad: Imie za krotkie!\n";
            }

            while (true) {
                cout << "Podaj nazwisko: "; 
                cin >> inputData;
                if (list.tabPeople[idx]->setLastName(inputData) == 0) break;
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
            inputPresenceForList(list.tabPresence_A, "Przedmiot A");
        } else if (choice == 2) {
            inputPresenceForList(list.tabPresence_B, "Przedmiot B");
        } else {
            cout << "Nieprawidlowy wybor.\n";
        }
    }

    void printList() {
        cout << "\n" << left 
             << setw(5) << "Nr" << "| " 
             << setw(16) << "Typ" << "| " 
             << setw(11) << "Indeks/ID" << "| " 
             << setw(15) << "Imie" << "| " 
             << setw(20) << "Nazwisko" << "| " 
             << setw(6) << "Ob. A" << "| " 
             << setw(6) << "Ob. B" << "\n";
        
        cout << "------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < N; i++) {
            if (list.tabPeople[i] != nullptr) {
                string nrStr = std::to_string(i + 1) + ")";
                
                cout << left 
                     << setw(5) << nrStr << "| " 
                     << setw(16) << list.tabPeople[i]->getType() << "| " 
                     << setw(11) << truncateString(list.tabPeople[i]->getIdentifier(), 11) << "| " 
                     << setw(15) << truncateString(list.tabPeople[i]->getFirstName(), 15) << "| " 
                     << setw(20) << truncateString(list.tabPeople[i]->getLastName(), 20) << "| " 
                     << setw(6) << (list.tabPresence_A[i] ? "1" : "0") << "| " 
                     << setw(6) << (list.tabPresence_B[i] ? "1" : "0") << "\n";
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
            if (list.tabPeople[i] != nullptr && list.tabPeople[i]->getLastName() == searchName) {
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
                if(list.tabPeople[foundIndex]->setFirstName(inputData) == -1) cout << "Blad walidacji!\n";
                else cout << "Dane zaktualizowane\n";
            } else if (subChoice == 2) {
                cout << "Obecnosc PA (1-tak, 0-nie): ";
                int p; cin >> p;
                list.tabPresence_A[foundIndex] = (p == 1);
                cout << "Dane zaktualizowane\n";
            } else if (subChoice == 3) {
                cout << "Obecnosc PB (1-tak, 0-nie): ";
                int p; cin >> p;
                list.tabPresence_B[foundIndex] = (p == 1);
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
            if (list.tabPeople[i] != nullptr && list.tabPeople[i]->getLastName() == searchName) {
                deleteIndex = i;
                break;
            }
        }

        if (deleteIndex != -1) {
            delete list.tabPeople[deleteIndex];

            for (int j = deleteIndex; j < N - 1; j++) {
                list.tabPeople[j] = list.tabPeople[j + 1];
                list.tabPresence_A[j] = list.tabPresence_A[j + 1];
                list.tabPresence_B[j] = list.tabPresence_B[j + 1];
            }
            
            list.tabPeople[N - 1] = nullptr;
            list.tabPresence_A[N - 1] = false;
            list.tabPresence_B[N - 1] = false;
            
            cout << "Osoba o nazwisku " << searchName << " zostala usunieta\n";
        } else {
            cout << "Blad: Nie znaleziono osoby o takim nazwisku\n";
        }
    }

    void saveSinglePerson() {
        string searchName;
        cout << "\nPodaj nazwisko osoby do zapisu: ";
        cin >> searchName;

        int foundIndex = -1;
        for (int i = 0; i < N; i++) {
            if (list.tabPeople[i] != nullptr && list.tabPeople[i]->getLastName() == searchName) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            string filename;
            cout << "Podaj nazwe pliku (np. baza.txt): ";
            cin >> filename;

            ISerializable* objToSave[1];
            objToSave[0] = dynamic_cast<ISerializable*>(list.tabPeople[foundIndex]);
            
            if (objToSave[0]) {
                writeToFile(objToSave, 1, filename);
            } else {
                cout << "Blad: Wybrany obiekt nie wspiera serializacji.\n";
            }
        } else {
            cout << "Nie znaleziono osoby o takim nazwisku!\n";
        }
    }

    void saveWholeList() {
        string filename;
        cout << "\nPodaj nazwe pliku dla calej listy (np. cala_lista.txt): ";
        cin >> filename;

        ISerializable* objectsToSave[N];
        int count = 0;

        for (int i = 0; i < N; i++) {
            if (list.tabPeople[i] != nullptr) {
                ISerializable* serializableObj = dynamic_cast<ISerializable*>(list.tabPeople[i]);
                if (serializableObj) {
                    objectsToSave[count] = serializableObj;
                    count++;
                }
            }
        }

        if (count > 0) {
            writeToFile(objectsToSave, count, filename);
        } else {
            cout << "Lista jest pusta, brak danych do zapisu.\n";
        }
    }

    void saveSelectedGroup() {
        int groupChoice;
        cout << "\nWybierz grupe do zapisu:\n";
        cout << "1. Tylko Studenci\n";
        cout << "2. Tylko Prowadzacy\n";
        cout << "3. Tylko Studenci Zaoczni\n";
        cout << "Wybor: ";
        cin >> groupChoice;

        string targetType = "";
        if (groupChoice == 1) targetType = "Student";
        else if (groupChoice == 2) targetType = "Prowadzacy";
        else if (groupChoice == 3) targetType = "Stud. Zaoczny";
        else {
            cout << "Nieprawidlowy wybor!\n";
            return;
        }

        string filename;
        cout << "Podaj nazwe pliku (np. grupy.txt): ";
        cin >> filename;

        ISerializable* objectsToSave[N];
        int count = 0;

        for (int i = 0; i < N; i++) {
            if (list.tabPeople[i] != nullptr && list.tabPeople[i]->getType() == targetType) {
                ISerializable* serializableObj = dynamic_cast<ISerializable*>(list.tabPeople[i]);
                if (serializableObj) {
                    objectsToSave[count] = serializableObj;
                    count++;
                }
            }
        }

        if (count > 0) {
            writeToFile(objectsToSave, count, filename);
        } else {
            cout << "Brak osob z tej grupy do zapisu.\n";
        }
    }

public:
    void run() {
        int choice;
        do {
            cout << "\n------- MENU LISTY OSOB -------\n";
            cout << "1. Dodac osob do tablicy\n";
            cout << "2. Przypisac obecnosc osob\n";
            cout << "3. Wyswietl liste osob\n";
            cout << "4. Edytuj dane osob\n";
            cout << "5. Usun osobe\n";
            cout << "6. Zapisz jedna wybrana osobe do pliku\n";
            cout << "7. Zapisz cala obecna liste do pliku\n";
            cout << "8. Zapisz wybrana grupe osob do pliku\n";
            cout << "0. Wyjscie\n\n";
            cout << "Wybor: ";
            cin >> choice;

            switch (choice) {
                case 1: inputList(); break;
                case 2: takeAttendance(); break;
                case 3: printList(); break;
                case 4: editPerson(); break;
                case 5: deletePerson(); break;
                case 6: saveSinglePerson(); break;
                case 7: saveWholeList(); break;
                case 8: saveSelectedGroup(); break;
                case 0: cout << "Koniec programu\n"; break;
                default: cout << "Niepoprawna opcja\n";
            }
        } while (choice != 0);
    }
};

int main() {
    UserInterface ui;
    ui.run();
    return 0;
}