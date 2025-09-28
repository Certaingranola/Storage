#include <iostream>
#include <cstring>
#include <conio.h>
#include <windows.h>

using namespace std;

// ========== ENUMS ==========
enum Colors {
    BLACK = 0, DARK_BLUE = 1, DARK_GREEN = 2, DARK_CYAN = 3,
    DARK_RED = 4, DARK_MAGENTA = 5, DARK_YELLOW = 6, GREY = 7,
    DARK_GREY = 8, BLUE = 9, GREEN = 10, CYAN = 11,
    RED = 12, MAGENTA = 13, YELLOW = 14, WHITE = 15
};

// ========== UTILITY ==========
void setConsoleColor(Colors textColor, Colors bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// Product structure to store product details.
struct Product {
    char name[50];
    char manufacturer[50];
    double price;
    char category[30];
    char arrivalDate[11];
    char expirationDate[11];
};

// Pointer for the dynamic array and variables for size tracking.
Product* warehouse = nullptr;
int productCount = 0;
int warehouseCapacity = 0;

// Function to resize the dynamic array.
void resizeWarehouse(int newCapacity) {
    Product* newWarehouse = new Product[newCapacity];
    for (int i = 0; i < productCount; ++i) {
        newWarehouse[i] = warehouse[i];
    }
    delete[] warehouse;
    warehouse = newWarehouse;
    warehouseCapacity = newCapacity;
}

// Add a product to the dynamic array.
void addProduct(const Product& p) {
    if (productCount == warehouseCapacity) {
        if (warehouseCapacity == 0) {
            resizeWarehouse(1);
        }
        else {
            resizeWarehouse(warehouseCapacity * 2);
        }
    }
    warehouse[productCount++] = p;
    cout << "Product added.\n";
}

// Remove a product by name.
void removeProduct(const char* name) {
    for (int i = 0; i < productCount; ++i) {
        if (strcmp(warehouse[i].name, name) == 0) {
            for (int j = i; j < productCount - 1; ++j)
                warehouse[j] = warehouse[j + 1];
            --productCount;
            cout << "Product removed.\n";
            return;
        }
    }
    cout << "Product not found.\n";
}

// Replace a product by name.
void replaceProduct(const char* name, const Product& newProduct) {
    for (int i = 0; i < productCount; ++i) {
        if (strcmp(warehouse[i].name, name) == 0) {
            warehouse[i] = newProduct;
            cout << "Product replaced.\n";
            return;
        }
    }
    cout << "Product not found.\n";
}

// Function to display all products in the warehouse.
void displayAllProducts() {
    if (productCount == 0) {
        cout << "Warehouse is empty.\n";
        return;
    }
    cout << "\n------ WAREHOUSE INVENTORY ------\n";
    for (int i = 0; i < productCount; ++i) {
        cout << "---------------------------------\n";
        cout << "Name: " << warehouse[i].name << endl;
        cout << "Manufacturer: " << warehouse[i].manufacturer << endl;
        cout << "Price: " << warehouse[i].price << " UAH" << endl;
        cout << "Category: " << warehouse[i].category << endl;
        cout << "Arrival Date: " << warehouse[i].arrivalDate << endl;
        cout << "Expiration Date: " << warehouse[i].expirationDate << endl;
    }
    cout << "---------------------------------\n";
}

// Search for a product by name.
void searchByName(const char* name) {
    for (int i = 0; i < productCount; ++i)
        if (strcmp(warehouse[i].name, name) == 0)
            cout << warehouse[i].name << " | " << warehouse[i].price << " UAH\n";
}

// Search for a product by manufacturer.
void searchByManufacturer(const char* manufacturer) {
    for (int i = 0; i < productCount; ++i)
        if (strcmp(warehouse[i].manufacturer, manufacturer) == 0)
            cout << warehouse[i].name << " | " << warehouse[i].manufacturer << "\n";
}

// Search for products by maximum price.
void searchByPrice(double maxPrice) {
    for (int i = 0; i < productCount; ++i)
        if (warehouse[i].price <= maxPrice)
            cout << warehouse[i].name << " | " << warehouse[i].price << " UAH\n";
}

// Search for products by category.
void searchByCategory(const char* category) {
    for (int i = 0; i < productCount; ++i)
        if (strcmp(warehouse[i].category, category) == 0)
            cout << warehouse[i].name << " | " << warehouse[i].category << "\n";
}

// Search for products by arrival date.
void searchByArrivalDate(const char* date) {
    for (int i = 0; i < productCount; ++i)
        if (strcmp(warehouse[i].arrivalDate, date) == 0)
            cout << warehouse[i].name << " | " << warehouse[i].arrivalDate << "\n";
}

// Search for products by expiration date.
void searchByExpirationDate(const char* date) {
    for (int i = 0; i < productCount; ++i)
        if (strcmp(warehouse[i].expirationDate, date) == 0)
            cout << warehouse[i].name << " | " << warehouse[i].expirationDate << "\n";
}

// Sort products by price using bubble sort.
void sortByPrice() {
    for (int i = 0; i < productCount - 1; ++i)
        for (int j = i + 1; j < productCount; ++j)
            if (warehouse[i].price > warehouse[j].price) {
                Product temp = warehouse[i];
                warehouse[i] = warehouse[j];
                warehouse[j] = temp;
            }
}

// Sort products by category using bubble sort.
void sortByCategory() {
    for (int i = 0; i < productCount - 1; ++i)
        for (int j = i + 1; j < productCount; ++j)
            if (strcmp(warehouse[i].category, warehouse[j].category) > 0) {
                Product temp = warehouse[i];
                warehouse[i] = warehouse[j];
                warehouse[j] = temp;
            }
}

// ========== MENU ==========
int showMenu(const string items[], int itemCount, Colors activeText, Colors activeBg, Colors normalText, Colors normalBg) {
    int selected = 0;
    const int ENTER = 13;
    const int ESCAPE = 27;
    const int UP = 72;
    const int DOWN = 80;

    while (true) {
        system("cls");
        cout << "\n------ WAREHOUSE MENU ------\n";
        for (int i = 0; i < itemCount; ++i) {
            if (i == selected) {
                setConsoleColor(activeText, activeBg);
                cout << " > " << items[i];
            }
            else {
                setConsoleColor(normalText, normalBg);
                cout << "   " << items[i];
            }
            setConsoleColor(WHITE, BLACK);
            cout << endl;
        }

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == UP) {
                selected = (selected == 0) ? itemCount - 1 : selected - 1;
            }
            else if (key == DOWN) {
                selected = (selected == itemCount - 1) ? 0 : selected + 1;
            }
        }
        else if (key == ENTER) {
            return selected;
        }
        else if (key == ESCAPE) {
            return -1;
        }
    }
}

// ========== MAIN ==========
int main() {
    const int itemCount = 13;
    string mainMenu[itemCount] = {
        "Add product",
        "Remove product",
        "Replace product",
        "Search by name",
        "Search by manufacturer",
        "Search by price",
        "Search by category",
        "Search by arrival date",
        "Search by expiration date",
        "Sort by price",
        "Sort by category",
        "Display all products",
        "Exit"
    };

    while (true) {
        int choice = showMenu(mainMenu, itemCount, BLACK, YELLOW, WHITE, BLACK);

        if (choice == -1 || choice == 12) {
            cout << "Exiting program.\n";
            delete[] warehouse;
            return 0;
        }

        system("cls");
        cout << "You selected: " << mainMenu[choice] << endl;

        Product temp;
        char buffer[50];
        double price;

        switch (choice) {
        case 0:
            cout << "Name: "; cin.getline(temp.name, 50);
            cout << "Manufacturer: "; cin.getline(temp.manufacturer, 50);
            cout << "Price: "; cin >> temp.price; cin.ignore();
            cout << "Category: "; cin.getline(temp.category, 30);
            cout << "Arrival date (YYYY-MM-DD): "; cin.getline(temp.arrivalDate, 11);
            cout << "Expiration date (YYYY-MM-DD): "; cin.getline(temp.expirationDate, 11);
            addProduct(temp);
            break;
        case 1:
            cout << "Enter product name to remove: "; cin.getline(buffer, 50);
            removeProduct(buffer);
            break;
        case 2:
            cout << "Enter product name to replace: "; cin.getline(buffer, 50);
            cout << "New product data:\n";
            cout << "Name: "; cin.getline(temp.name, 50);
            cout << "Manufacturer: "; cin.getline(temp.manufacturer, 50);
            cout << "Price: "; cin >> temp.price; cin.ignore();
            cout << "Category: "; cin.getline(temp.category, 30);
            cout << "Arrival date: "; cin.getline(temp.arrivalDate, 11);
            cout << "Expiration date: "; cin.getline(temp.expirationDate, 11);
            replaceProduct(buffer, temp);
            break;
        case 3:
            cout << "Enter product name: "; cin.getline(buffer, 50);
            searchByName(buffer);
            break;
        case 4:
            cout << "Enter manufacturer: "; cin.getline(buffer, 50);
            searchByManufacturer(buffer);
            break;
        case 5:
            cout << "Enter max price: "; cin >> price; cin.ignore();
            searchByPrice(price);
            break;
        case 6:
            cout << "Enter category: "; cin.getline(buffer, 30);
            searchByCategory(buffer);
            break;
        case 7:
            cout << "Enter arrival date: "; cin.getline(buffer, 11);
            searchByArrivalDate(buffer);
            break;
        case 8:
            cout << "Enter expiration date: "; cin.getline(buffer, 11);
            searchByExpirationDate(buffer);
            break;
        case 9:
            sortByPrice();
            cout << "Sorted by price.\n";
            break;
        case 10:
            sortByCategory();
            cout << "Sorted by category.\n";
            break;
        case 11:
            displayAllProducts();
            break;
        }

        cout << "\nPress any key to continue...";
        _getch();
    }

    return 0;
}