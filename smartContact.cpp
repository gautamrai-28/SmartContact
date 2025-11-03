#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

class Contact {
public:
    string name;
    string phone;
    string email;


    Contact() {}
    Contact(string n, string p, string e) {
        name = n;
        phone = p;
        email = e;
    }

    void display() const {
        cout << left << setw(20) << name
             << setw(15) << phone
             << setw(25) << email << endl;
    }
};

// Convert string to lowercase (safe version)
string toLowerStr(const string &s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(),
              [](unsigned char c) { return tolower(c); });
    return out;
}

// Save all contacts to file
void saveToFile(const vector<Contact> &contacts, const string &filename) {
    ofstream file(filename);
    if (!file) {
        cout << " Error opening file for writing.\n";
        return;
    }
    for (auto &c : contacts)
        file << c.name << "," << c.phone << "," << c.email << "\n";
    file.close();
}

// Load contacts from file
vector<Contact> loadFromFile(const string &filename) {
    vector<Contact> contacts;
    ifstream file(filename);
    if (!file) return contacts;

    string line;
    while (getline(file, line)) {
        size_t p1 = line.find(',');
        size_t p2 = line.rfind(',');
        if (p1 == string::npos || p2 == string::npos) continue;

        string n = line.substr(0, p1);
        string p = line.substr(p1 + 1, p2 - p1 - 1);
        string e = line.substr(p2 + 1);
        contacts.push_back(Contact(n, p, e));
    }
    file.close();
    return contacts;
}

// Add new contact
void addContact(vector<Contact> &contacts, const string &filename) {
    string name, phone, email;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Phone: ";
    getline(cin, phone);
    cout << "Enter Email: ";
    getline(cin, email);

    contacts.push_back(Contact(name, phone, email));
    saveToFile(contacts, filename);
    cout << " Contact added and saved successfully!\n";
}

// View all contacts
void viewContacts(const vector<Contact> &contacts) {
    if (contacts.empty()) {
        cout << "No contacts found.\n";
        return;
    }
    cout << left << setw(20) << "Name"
         << setw(15) << "Phone"
         << setw(25) << "Email" << endl;
    cout << string(60, '-') << endl;
    for (auto &c : contacts)
        c.display();
}

// Search contact (case-insensitive)
void searchContact(const vector<Contact> &contacts) {
    string query;
    cout << "Enter name or phone to search: ";
    cin.ignore();
    getline(cin, query);

    string qlow = toLowerStr(query);
    bool found = false;
    for (const auto &c : contacts) {
        if (toLowerStr(c.name).find(qlow) != string::npos ||
            toLowerStr(c.phone).find(qlow) != string::npos) {
            c.display();
            found = true;
        }
    }
    if (!found)
        cout << "No matching contact found.\n";
}

// Delete contact (case-insensitive)
void deleteContact(vector<Contact> &contacts, const string &filename) {
    string query;
    cout << "Enter name of contact to delete: ";
    cin.ignore();
    getline(cin, query);

    string qlow = toLowerStr(query);
    auto it = remove_if(contacts.begin(), contacts.end(),
                        [&](const Contact &c) { return toLowerStr(c.name) == qlow; });

    if (it != contacts.end()) {
        contacts.erase(it, contacts.end());
        saveToFile(contacts, filename);
        cout << " Contact deleted and saved successfully!\n";
    } else {
        cout << " Contact not found.\n";
    }
}

// Edit contact (case-insensitive)
void editContact(vector<Contact> &contacts, const string &filename) {
    string query;
    cout << "Enter name of contact to edit: ";
    cin.ignore();
    getline(cin, query);

    string qlow = toLowerStr(query);
    for (auto &c : contacts) {
        if (toLowerStr(c.name) == qlow) {
            cout << "\nEditing contact: " << c.name << endl;
            cout << "1. Edit Name\n";
            cout << "2. Edit Phone\n";
            cout << "3. Edit Email\n";
            cout << "4. Edit All\n";
            cout << "Enter your choice: ";

            int ch;
            cin >> ch;
            cin.ignore();

            switch (ch) {
                case 1: {
                    string newName;
                    cout << "Enter new name: ";
                    getline(cin, newName);
                    if (!newName.empty()) c.name = newName;
                    break;
                }
                case 2: {
                    string newPhone;
                    cout << "Enter new phone: ";
                    getline(cin, newPhone);
                    if (!newPhone.empty()) c.phone = newPhone;
                    break;
                }
                case 3: {
                    string newEmail;
                    cout << "Enter new email: ";
                    getline(cin, newEmail);
                    if (!newEmail.empty()) c.email = newEmail;
                    break;
                }
                case 4: {
                    string newName, newPhone, newEmail;
                    cout << "Enter new name: ";
                    getline(cin, newName);
                    cout << "Enter new phone: ";
                    getline(cin, newPhone);
                    cout << "Enter new email: ";
                    getline(cin, newEmail);
                    if (!newName.empty()) c.name = newName;
                    if (!newPhone.empty()) c.phone = newPhone;
                    if (!newEmail.empty()) c.email = newEmail;
                    break;
                }
                default:
                    cout << "Invalid choice.\n";
                    return;
            }

            saveToFile(contacts, filename);
            cout << " Contact updated and saved successfully!\n";
            return;
        }
    }
    cout << " Contact not found.\n";
}

// Sort contacts by name or phone
void sortContacts(vector<Contact> &contacts, const string &filename) {
    if (contacts.empty()) {
        cout << "No contacts to sort.\n";
        return;
    }

    int choice;
    cout << "\nSort Contacts By:\n";
    cout << "1. Name\n";
    cout << "2. Phone Number\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            sort(contacts.begin(), contacts.end(),
                 [](const Contact &a, const Contact &b) {
                     return toLowerStr(a.name) < toLowerStr(b.name);
                 });
            cout << "\n Contacts sorted alphabetically by name!\n";
            break;

        case 2:
            sort(contacts.begin(), contacts.end(),
                 [](const Contact &a, const Contact &b) {
                     return a.phone < b.phone;
                 });
            cout << "\n Contacts sorted numerically by phone number!\n";
            break;

        default:
            cout << " Invalid choice.\n";
            return;
    }

    saveToFile(contacts, filename);

    // Display sorted contacts
    cout << "\n----- Sorted Contact List -----\n";
    cout << left << setw(20) << "Name"
         << setw(15) << "Phone"
         << setw(25) << "Email" << endl;
    cout << string(60, '-') << endl;
    for (auto &c : contacts)
        c.display();
}

int main() {
    string filename = "contacts.txt";
    vector<Contact> contacts = loadFromFile(filename);

    int choice;
    do {
        cout << "\n===== SmartContact Contact Management =====\n";
        cout << "1. Add Contact\n";
        cout << "2. View All Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Edit Contact\n";
        cout << "5. Delete Contact\n";
        cout << "6. Sort Contacts\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addContact(contacts, filename); break;
            case 2: viewContacts(contacts); break;
            case 3: searchContact(contacts); break;
            case 4: editContact(contacts, filename); break;
            case 5: deleteContact(contacts, filename); break;
            case 6: sortContacts(contacts, filename); break;
            case 7:
                cout << " Exiting... All changes are already saved.\n";
                break;
            default:
                cout << " Invalid choice. Try again.\n";
        }
    } while (choice != 7);

    return 0;
}
