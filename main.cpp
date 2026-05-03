#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>

using namespace std;

// Book availability status
enum Status {
    AVAILABLE,
    BORROWED
};

// Structure that represents one book record
struct Book {
    int id;            // unique book ID
    string title;     // book title
    char category;    // category A / B / C
    double price;     // book price
    Status status;    // availability status
};

// Library class that manages all books
class Library {
private:
    vector<Book> books; // dynamic list of books

    // Reads only positive integer values from user
    int readPositiveInt(const string& msg) {
        string input;

        while (true) {
            cout << msg;
            cin >> input;

            bool ok = true;

            // Check if input contains only digits
            for (char c : input) {
                if (!isdigit(c)) {
                    ok = false;
                    break;
                }
            }

            if (!ok) {
                cout << "Invalid input! Only positive numbers allowed.\n";
                continue;
            }

            int value = stoi(input);

            if (value <= 0) {
                cout << "Value must be greater than 0!\n";
                continue;
            }

            return value;
        }
    }

    // Checks if book ID already exists
    bool isUnique(int id) {
        for (const auto& book : books) {
            if (book.id == id) return false;
        }
        return true;
    }

public:
    // Constructor loads data from file when program starts
    Library() {
        loadFromFile();
    }

    // Destructor saves data to file when program ends
    ~Library() {
        saveToFile();
    }

    // Add new book to library
    void addBook() {
        Book b;

        // Get unique ID
        while (true) {
            b.id = readPositiveInt("Enter Book ID: ");

            if (!isUnique(b.id)) {
                cout << "Duplicate ID!\n";
                continue;
            }

            break;
        }

        cin.ignore();

        // Input book title
        cout << "Enter Title: ";
        getline(cin, b.title);

        // Input and validate category
        while (true) {
            cout << "Enter Category (A/B/C): ";
            cin >> b.category;

            b.category = toupper(b.category);

            if (b.category == 'A' || b.category == 'B' || b.category == 'C') {
                break;
            }

            cout << "Invalid category!\n";
        }

        // Input and validate price
        while (true) {
            cout << "Enter Price: ";
            cin >> b.price;

            if (!cin.fail() && b.price >= 0) break;

            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid price!\n";
        }

        // Default status when adding book
        b.status = AVAILABLE;

        books.push_back(b);

        cout << "Book added successfully!\n";
    }

    // Display all books
    void viewBooks() {
        if (books.empty()) {
            cout << "No books available.\n";
            return;
        }

        for (const auto& b : books) {
            cout << "\nTitle: " << b.title;
            cout << "\nPrice: " << b.price;

            // Convert enum to readable text
            cout << "\nStatus: "
                 << (b.status == AVAILABLE ? "Available" : "Borrowed")
                 << "\n";
        }
    }

    // Search book by ID
    void searchBook() {
        int id = readPositiveInt("Enter ID to search: ");

        for (const auto& b : books) {
            if (b.id == id) {
                cout << "\nFound Book:";
                cout << "\nTitle: " << b.title;
                cout << "\nCategory: " << b.category;
                cout << "\nPrice: " << b.price;

                // Show status
                cout << "\nStatus: "
                     << (b.status == AVAILABLE ? "Available" : "Borrowed")
                     << "\n";
                return;
            }
        }

        cout << "Book not found.\n";
    }

    // Update book price and status
    void updateBook() {
        int id = readPositiveInt("Enter Book ID to update: ");

        for (auto& b : books) {
            if (b.id == id) {

                // Update price
                while (true) {
                    cout << "Enter new price: ";
                    cin >> b.price;

                    if (!cin.fail() && b.price >= 0) break;

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid price!\n";
                }

                // Update status
                int choice = readPositiveInt("1. Available\n2. Borrowed\nChoose status: ");

                b.status = (choice == 1) ? AVAILABLE : BORROWED;

                cout << "Book updated!\n";
                return;
            }
        }

        cout << "Book not found.\n";
    }

    // Delete book by ID
    void deleteBook() {
        int id = readPositiveInt("Enter Book ID to delete: ");

        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                books.erase(books.begin() + i);
                cout << "Book deleted!\n";
                return;
            }
        }

        cout << "Book not found.\n";
    }

    // Save books to file (persistent storage)
    void saveToFile() {
        ofstream file("library.txt");

        for (const auto& b : books) {
            file << b.id << "|"
                 << b.title << "|"
                 << b.category << "|"
                 << b.price << "|"
                 << (int)b.status << "\n";
        }
    }

    // Load books from file on program start
    void loadFromFile() {
        ifstream file("library.txt");
        if (!file.is_open()) return;

        Book b;
        string line;

        while (getline(file, line)) {

            size_t pos;

            // Parse ID
            pos = line.find('|');
            b.id = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            // Parse title
            pos = line.find('|');
            b.title = line.substr(0, pos);
            line.erase(0, pos + 1);

            // Parse category
            pos = line.find('|');
            b.category = line.substr(0, pos)[0];
            line.erase(0, pos + 1);

            // Parse price
            pos = line.find('|');
            b.price = stod(line.substr(0, pos));
            line.erase(0, pos + 1);

            // Parse status
            b.status = (Status)stoi(line);

            books.push_back(b);
        }
    }
};

// Main program menu
int main() {
    Library lib;
    int choice;

    while (true) {
        cout << "\n===== Library Menu =====\n";
        cout << "1. Add Book\n";
        cout << "2. View Books\n";
        cout << "3. Search Book\n";
        cout << "4. Update Book\n";
        cout << "5. Delete Book\n";
        cout << "6. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
        case 1: lib.addBook(); break;
        case 2: lib.viewBooks(); break;
        case 3: lib.searchBook(); break;
        case 4: lib.updateBook(); break;
        case 5: lib.deleteBook(); break;
        case 6:
            cout << "Saving and exiting...\n";
            return 0;
        default:
            cout << "Invalid choice!\n";
        }
    }
}