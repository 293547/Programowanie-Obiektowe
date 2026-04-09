#include <iostream>
#include <string>

using std::cout, std::cin, std::string;

#define N 10

class Person {  
private:
    string firstName = "";   
    string lastName = "";
    string index = "";

public:
    int setFirstName(string value) {
        if (value.length() < 2) return -1;
        firstName = value;
        return 0;
    }

    int setLastName(string value) {
        if (value.length() < 2) return -1;
        lastName = value;
        return 0;
    }

    int setIndex(string value) {
        if (value.length() != 6) return -1;
        for (char c : value) {
            if (!isdigit(c)) return -1;
        }
        index = value;
        return 0;
    }

    string getFirstName() { return firstName; }
    string getLastName() { return lastName; }
    string getIndex() { return index; }
};

class ClassRegister {
private:
    Person* students[N];
    bool presenceSubject1[N]; 
    bool presenceSubject2[N]; 

    void inputPresenceForList(bool* listPresence, string subjectName) {
        string temp;
        cout << "\n--- Sprawdzanie obecnosci: " << subjectName << " ---\n";
        cout << "Wpisz: 1 jesli - \"tak\", 0 jesli - \"nie\"\n";
        for (int i = 0; i < N; i++) {
            if (students[i] != nullptr) {
                while (true) {
                    cout << "Czy " << students[i]->getLastName() << " jest obecny?: ";
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
    ClassRegister() {
        for (int i = 0; i < N; i++) {
            students[i] = nullptr;
            presenceSubject1[i] = false;
            presenceSubject2[i] = false;
        }
    }

    ~ClassRegister() {
        for(int i = 0; i < N; i++) {
            if(students[i] != nullptr) {
                delete students[i];
            }
        }
    }

    void inputList() {
        string temp;
        int studentAddAmount = 0;
        int studentAmount = 0;
        
        for (int i = 0; i < N; i++) {
            if (students[i] != nullptr) studentAmount++;
        }

        while (true) {
            bool isValid = true;
            cout << "\nIle studentow chcesz dodac? (Max " << N - studentAmount << ", 0 aby wyjsc): ";
            cin >> temp;

            for (int i = 0; i < temp.length(); i++) {
                if (!isdigit(temp[i])) {
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
            students[idx] = new Person(); 
            
            cout << "\n--- Student " << idx + 1 << " ---\n";
            
            string inputData;
            while (true) {
                cout << "Podaj 6-cyfrowy index: ";
                cin >> inputData;
                if (students[idx]->setIndex(inputData) == 0) break;
                cout << "Blad: indeks musi miec dokladnie 6 cyfr!\n";
            }

            while (true) {
                cout << "Podaj imie: "; 
                cin >> inputData;
                if (students[idx]->setFirstName(inputData) == 0) break;
                cout << "Blad: Imie za krotkie!\n";
            }

            while (true) {
                cout << "Podaj nazwisko: "; 
                cin >> inputData;
                if (students[idx]->setLastName(inputData) == 0) break;
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
        string text = "Osoba";

// The magic number is 16 based on your exact data row spacing
int text_length = 15 + students[0]->getFirstName().length() + students[0]->getLastName().length();

// Calculate remaining space and divide by 2
int total_spaces = text_length - text.length();
int left_spaces = total_spaces / 2;
int right_spaces = total_spaces - left_spaces; 

// Generate padding strings
string left_padding(left_spaces, ' ');
string right_padding(right_spaces, ' ');

// Print header
cout << "\n" << left_padding << text << right_padding << "| Ob. 1 | Ob. 2 |\n";

// " | Ob. 1 | Ob. 2 |" is exactly 17 characters long
int whole_width = text_length + 17; 
string spacing_width(whole_width, '-');

cout << spacing_width << "\n";
        for (int i = 0; i < N; i++) {
            if (students[i] != nullptr) {
                cout << i + 1 << ") " << students[i]->getIndex() << "  " 
                     << students[i]->getFirstName() << "  " 
                     << students[i]->getLastName() << "  |   " 
                     << presenceSubject1[i] << "   |   " << presenceSubject2[i] << "   |\n";
            }
        }
    }

    void editStudent() {
        printList();
        string searchName;
        cout << "\nPodaj nazwisko studenta do edycji: "; 
        cin >> searchName;

        int foundIndex = -1;
        for (int i = 0; i < N; i++) {
            if (students[i] != nullptr && students[i]->getLastName() == searchName) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            int subChoice;
            cout << "\nCo chcesz edytowac dla studenta " << searchName << "?\n";
            cout << "1. Imie\n2. Indeks\n3. Obecnosc (Przedmiot 1)\n4. Obecnosc (Przedmiot 2)\n0. Powrot\nWybor: "; 
            cin >> subChoice;

            string inputData;
            if (subChoice == 1) {
                cout << "Nowe imie: "; 
                cin >> inputData;
                if(students[foundIndex]->setFirstName(inputData) == -1) cout << "Blad walidacji!\n";
                else cout << "Dane zaktualizowane\n";
            } else if (subChoice == 2) {
                cout << "Nowy indeks: "; 
                cin >> inputData;
                if(students[foundIndex]->setIndex(inputData) == -1) cout << "Blad walidacji!\n";
                else cout << "Dane zaktualizowane\n";
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
            cout << "Blad: Nie znaleziono studenta o takim nazwisku\n";
        }
    }

    void deleteStudent() {
        string searchName;
        cout << "\nPodaj nazwisko studenta do usuniecia: "; 
        cin >> searchName;

        int deleteIndex = -1;
        for (int i = 0; i < N; i++) {
            if (students[i] != nullptr && students[i]->getLastName() == searchName) {
                deleteIndex = i;
                break;
            }
        }

        if (deleteIndex != -1) {
            delete students[deleteIndex];

            for (int j = deleteIndex; j < N - 1; j++) {
                students[j] = students[j + 1];
                presenceSubject1[j] = presenceSubject1[j + 1];
                presenceSubject2[j] = presenceSubject2[j + 1];
            }
            
            students[N - 1] = nullptr;
            presenceSubject1[N - 1] = false;
            presenceSubject2[N - 1] = false;
            
            cout << "Student o nazwisku " << searchName << " zostal usuniety\n";
        } else {
            cout << "Blad: Nie znaleziono studenta o takim nazwisku\n";
        }
    }
};

int main() {
    int choice;
    ClassRegister registerSystem; 

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
            case 1: registerSystem.inputList(); break;
            case 2: registerSystem.takeAttendance(); break;
            case 3: registerSystem.printList(); break;
            case 4: registerSystem.editStudent(); break;
            case 5: registerSystem.deleteStudent(); break;
            case 0: cout << "Koniec programu\n"; break;
            default: cout << "Niepoprawna opcja\n";
        }
    } while (choice != 0);

    return 0;
}
