#include <iostream>
#include <string>
#include <windows.h> 
#include <conio.h>   
#include <fstream>

using namespace std;

// --- DATA STRUCTURES & GLOBAL DEFINITIONS ---

const int MAIN_MENU_ITEMS = 8;

struct Product {
	string name;
	string manufacturer;
	double price;
	string group;
	string arrivalDate;
	string expirationDate;
};

Product** warehouse = nullptr;
int productCount = 0;
int warehouseCapacity = 0;
const string FILENAME = "warehouse_data.txt";


// --- UTILITY / CONSOLE FUNCTIONS ---

void gotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
	system("cls");
}

int handleMenuNavigation(int currentSelection, int maxItems) {
	int key = _getch();

	if (key == 13) {
		return -1;
	}

	if (key == 0 || key == 224) {
		key = _getch();
		if (key == 72) {
			currentSelection--;
		}
		else if (key == 80) {
			currentSelection++;
		}
		else {
			return currentSelection;
		}
	}
	else {
		return currentSelection;
	}

	if (currentSelection < 0) {
		currentSelection = maxItems - 1;
	}
	else if (currentSelection >= maxItems) {
		currentSelection = 0;
	}

	return currentSelection;
}

// --- UTILITY / DYNAMIC ARRAY MANAGEMENT ---

void resizeWarehouse(int newCapacity) {
	if (newCapacity == 0) {
		newCapacity = (warehouseCapacity == 0) ? 10 : warehouseCapacity * 2;
	}

	Product** newWarehouse = new Product * [newCapacity];

	for (int i = 0; i < newCapacity; ++i) {
		newWarehouse[i] = NULL;
	}

	for (int i = 0; i < warehouseCapacity; ++i) {
		newWarehouse[i] = warehouse[i];
	}

	if (warehouse != nullptr) {
		delete[] warehouse;
	}

	warehouse = newWarehouse;
	warehouseCapacity = newCapacity;
}

void initializeWarehouse() {
	if (warehouseCapacity == 0) {
		resizeWarehouse(10);
	}
}

void cleanupWarehouse() {
	if (warehouse != nullptr) {
		for (int i = 0; i < warehouseCapacity; ++i) {
			if (warehouse[i] != NULL) {
				delete warehouse[i];
			}
		}
		delete[] warehouse;
		warehouse = nullptr;
	}
	warehouseCapacity = 0;
	productCount = 0;
}


// --- UTILITY / FILE OPERATIONS ---

string productToString(const Product* p) {
	if (p == NULL) return "";
	return p->name + ";" + p->manufacturer + ";" + to_string(p->price) + ";" + p->group + ";" + p->arrivalDate + ";" + p->expirationDate;
}

Product* stringToProduct(const string& data) {
	Product* p = new Product;
	size_t pos = 0;
	size_t nextPos;
	string token;

	for (int i = 0; i < 6; ++i) {
		nextPos = data.find(';', pos);
		if (nextPos == string::npos) nextPos = data.length();
		token = data.substr(pos, nextPos - pos);
		pos = nextPos + 1;

		switch (i) {
		case 0: p->name = token; break;
		case 1: p->manufacturer = token; break;
		case 2:
			try {
				p->price = stod(token);
			}
			catch (...) {
				p->price = 0.0;
			}
			break;
		case 3: p->group = token; break;
		case 4: p->arrivalDate = token; break;
		case 5: p->expirationDate = token; break;
		}
	}
	return p;
}

void saveToFile() {
	ofstream outFile(FILENAME);
	if (outFile.is_open()) {
		int savedCount = 0;
		for (int i = 0; i < warehouseCapacity; ++i) {
			if (warehouse[i] != NULL) {
				outFile << productToString(warehouse[i]) << endl;
				savedCount++;
			}
		}
		outFile.close();
		cout << "\nData saved successfully to " << FILENAME << ". Total items saved: " << savedCount << endl;
	}
	else {
		cout << "\nUnable to open file for saving!" << endl;
	}
	cout << "\nPress any key to continue...";
	_getch();
}

void loadFromFile() {
	ifstream inFile(FILENAME);

	cleanupWarehouse();
	initializeWarehouse();

	if (inFile.is_open()) {
		string line;
		int i = 0;
		while (getline(inFile, line)) {
			if (!line.empty()) {
				if (i >= warehouseCapacity) {
					resizeWarehouse(0);
				}

				warehouse[i] = stringToProduct(line);
				productCount++;
				i++;
			}
		}
		inFile.close();
		cout << "\nData loaded successfully from " << FILENAME << ". Total items: " << productCount << endl;
	}
	else {
		cout << "\nFile " << FILENAME << " not found or unable to open. Starting with empty warehouse." << endl;
	}
	cout << "\nPress any key to continue...";
	_getch();
}

// --- WAREHOUSE FUNCTIONS / CRUD & DISPLAY ---

void displayProduct(const Product* p, int index) {
	if (p == NULL) return;
	cout << "Index: " << index << "\n";
	cout << "  Name:          " << p->name << "\n";
	cout << "  Manufacturer:  " << p->manufacturer << "\n";
	cout << "  Price:         " << p->price << "\n";
	cout << "  Group:         " << p->group << "\n";
	cout << "  Arrival Date:  " << p->arrivalDate << "\n";
	cout << "  Expiration Date: " << p->expirationDate << "\n";
	cout << "-----------------------------------\n";
}

void addProduct() {
	clearScreen();
	cout << "--- Add New Product ---\n";

	if (productCount >= warehouseCapacity) {
		resizeWarehouse(0);
		cout << "Warehouse capacity doubled to " << warehouseCapacity << " to accommodate new items.\n";
	}

	int index = -1;
	for (int i = 0; i < warehouseCapacity; ++i) {
		if (warehouse[i] == NULL) {
			index = i;
			break;
		}
	}

	Product* newP = new Product;

	cout << "Enter Product Name: ";
	getline(cin >> ws, newP->name);
	cout << "Enter Manufacturer: ";
	getline(cin >> ws, newP->manufacturer);
	cout << "Enter Price: ";
	while (!(cin >> newP->price) || newP->price < 0) {
		cout << "Invalid price. Enter Price: ";
		cin.clear();
		cin.ignore(10000, '\n');
	}
	cout << "Enter Group (e.g., 'Water', 'Canned Food'): ";
	getline(cin >> ws, newP->group);
	cout << "Enter Arrival Date (YYYY-MM-DD): ";
	getline(cin >> ws, newP->arrivalDate);
	cout << "Enter Expiration Date (YYYY-MM-DD): ";
	getline(cin >> ws, newP->expirationDate);

	warehouse[index] = newP;
	productCount++;

	cout << "\nProduct added successfully at index " << index << "!\n";
	cout << "\nPress any key to continue...";
	_getch();
}

void removeProduct() {
	clearScreen();
	cout << "--- Remove Product ---\n";
	if (productCount == 0) {
		cout << "Warehouse is empty!\n";
		cout << "\nPress any key to continue...";
		_getch();
		return;
	}

	int index;
	cout << "Enter Index of product to remove (0 to " << warehouseCapacity - 1 << "): ";
	while (!(cin >> index) || index < 0 || index >= warehouseCapacity || warehouse[index] == NULL) {
		cout << "Invalid index or product slot is empty. Enter valid index: ";
		cin.clear();
		cin.ignore(10000, '\n');
	}

	delete warehouse[index];
	warehouse[index] = NULL;
	productCount--;

	cout << "\nProduct at index " << index << " removed successfully.\n";
	cout << "\nPress any key to continue...";
	_getch();
}

void replaceProduct() {
	clearScreen();
	cout << "--- Replace Product ---\n";
	if (productCount == 0) {
		cout << "Warehouse is empty!\n";
		cout << "\nPress any key to continue...";
		_getch();
		return;
	}

	int index;
	cout << "Enter Index of product to replace (0 to " << warehouseCapacity - 1 << "): ";
	while (!(cin >> index) || index < 0 || index >= warehouseCapacity || warehouse[index] == NULL) {
		cout << "Invalid index or product slot is empty. Enter valid index: ";
		cin.clear();
		cin.ignore(10000, '\n');
	}

	cout << "\nCurrently replacing:\n";
	displayProduct(warehouse[index], index);

	delete warehouse[index];

	cout << "--- Enter NEW Product Data ---\n";

	Product* newP = new Product;

	cout << "Enter NEW Product Name: ";
	getline(cin >> ws, newP->name);
	cout << "Enter NEW Manufacturer: ";
	getline(cin >> ws, newP->manufacturer);
	cout << "Enter NEW Price: ";
	while (!(cin >> newP->price) || newP->price < 0) {
		cout << "Invalid price. Enter NEW Price: ";
		cin.clear();
		cin.ignore(10000, '\n');
	}
	cout << "Enter NEW Group: ";
	getline(cin >> ws, newP->group);
	cout << "Enter NEW Arrival Date (YYYY-MM-DD): ";
	getline(cin >> ws, newP->arrivalDate);
	cout << "Enter NEW Expiration Date (YYYY-MM-DD): ";
	getline(cin >> ws, newP->expirationDate);

	warehouse[index] = newP;

	cout << "\nProduct at index " << index << " replaced successfully.\n";
	cout << "\nPress any key to continue...";
	_getch();
}

void displayAllProducts() {
	clearScreen();
	cout << "--- All Products in Warehouse (" << productCount << " items) ---\n";

	if (productCount == 0) {
		cout << "Warehouse is currently empty.\n";
	}
	else {
		for (int i = 0; i < warehouseCapacity; ++i) {
			if (warehouse[i] != NULL) {
				displayProduct(warehouse[i], i);
			}
		}
	}
	cout << "\nPress any key to continue...";
	_getch();
}


// --- WAREHOUSE FUNCTIONS / SEARCH & SORT ---

void searchProductMenu() {
	clearScreen();
	cout << "--- Search Product ---\n";
	if (productCount == 0) {
		cout << "Warehouse is empty! No search possible.\n";
		cout << "\nPress any key to continue...";
		_getch();
		return;
	}

	int searchType = 0;
	string searchOptions[] = {
		"1. By Name",
		"2. By Manufacturer",
		"3. By Price",
		"4. By Group",
		"5. By Arrival Date",
		"6. By Expiration Date",
		"7. Back to Main Menu"
	};
	const int SEARCH_MENU_ITEMS = 7;
	int keyResult;

	while (true) {
		clearScreen();
		cout << "--- Select Search Criterion (Use Arrows, then Enter) ---\n";
		for (int i = 0; i < SEARCH_MENU_ITEMS; ++i) {
			gotoXY(2, 2 + i);
			if (i == searchType) {
				cout << "-> \x1b[33m" << searchOptions[i] << "\x1b[0m";
			}
			else {
				cout << "   " << searchOptions[i] << "   ";
			}
		}

		keyResult = handleMenuNavigation(searchType, SEARCH_MENU_ITEMS);
		if (keyResult == -1) {
			if (searchType == SEARCH_MENU_ITEMS - 1) return;
			break;
		}
		searchType = keyResult;
	}

	string term;
	double priceTerm;
	bool found = false;

	clearScreen();
	cout << "--- Search Results ---\n";

	if (searchType == 2) {
		cout << "Enter search price: ";
		while (!(cin >> priceTerm) || priceTerm < 0) {
			cout << "Invalid price. Enter search price: ";
			cin.clear();
			cin.ignore(10000, '\n');
		}
	}
	else {
		cout << "Enter search term for " << searchOptions[searchType] << ": ";
		getline(cin >> ws, term);
	}


	for (int i = 0; i < warehouseCapacity; ++i) {
		if (warehouse[i] == NULL) continue;

		bool match = false;
		switch (searchType) {
		case 0:
			if (warehouse[i]->name.find(term) != string::npos) match = true;
			break;
		case 1:
			if (warehouse[i]->manufacturer.find(term) != string::npos) match = true;
			break;
		case 2:
			if (warehouse[i]->price == priceTerm) match = true;
			break;
		case 3:
			if (warehouse[i]->group.find(term) != string::npos) match = true;
			break;
		case 4:
			if (warehouse[i]->arrivalDate == term) match = true;
			break;
		case 5:
			if (warehouse[i]->expirationDate == term) match = true;
			break;
		}

		if (match) {
			displayProduct(warehouse[i], i);
			found = true;
		}
	}

	if (!found) {
		cout << "No products found matching the criteria.\n";
	}

	cout << "\nPress any key to continue...";
	_getch();
}

void swapProductPointers(Product*& a, Product*& b) {
	Product* temp = a;
	a = b;
	b = temp;
}

void sortByPrice() {
	if (productCount <= 1) {
		clearScreen();
		cout << "Not enough items to sort.\n";
		cout << "\nPress any key to continue...";
		_getch();
		return;
	}

	for (int i = 0; i < warehouseCapacity - 1; ++i) {
		for (int j = 0; j < warehouseCapacity - 1 - i; ++j) {

			if (warehouse[j] != NULL && warehouse[j + 1] != NULL) {
				if (warehouse[j]->price > warehouse[j + 1]->price) {
					swapProductPointers(warehouse[j], warehouse[j + 1]);
				}
			}
			else if (warehouse[j] == NULL && warehouse[j + 1] != NULL) {
				swapProductPointers(warehouse[j], warehouse[j + 1]);
			}
		}
	}

	clearScreen();
	cout << "--- Products Sorted by Price (Ascending) ---\n";

	for (int i = 0; i < productCount; ++i) {
		if (warehouse[i] != NULL) {
			displayProduct(warehouse[i], i);
		}
	}

	cout << "\nPress any key to continue...";
	_getch();
}

void sortByGroup() {
	if (productCount <= 1) {
		clearScreen();
		cout << "Not enough items to sort.\n";
		cout << "\nPress any key to continue...";
		_getch();
		return;
	}

	for (int i = 0; i < warehouseCapacity - 1; ++i) {
		for (int j = 0; j < warehouseCapacity - 1 - i; ++j) {

			if (warehouse[j] != NULL && warehouse[j + 1] != NULL) {
				if (warehouse[j]->group > warehouse[j + 1]->group) {
					swapProductPointers(warehouse[j], warehouse[j + 1]);
				}
			}
			else if (warehouse[j] == NULL && warehouse[j + 1] != NULL) {
				swapProductPointers(warehouse[j], warehouse[j + 1]);
			}
		}
	}

	clearScreen();
	cout << "--- Products Sorted by Group (Alphabetical) ---\n";

	for (int i = 0; i < productCount; ++i) {
		if (warehouse[i] != NULL) {
			displayProduct(warehouse[i], i);
		}
	}

	cout << "\nPress any key to continue...";
	_getch();
}


// --- MAIN MENU LOGIC ---

void mainMenu() {
	initializeWarehouse();
	loadFromFile();

	int selection = 0;
	int keyResult;
	string menuOptions[] = {
		"1. Add Product",
		"2. Remove Product",
		"3. Swap Product",
		"4. Search Product",
		"5. Sort by Price",
		"6. Sort by Group",
		"7. View All Products",
		"8. Save & Exit"
	};

	while (true) {
		clearScreen();
		cout << "======================================\n";
		cout << "       WAREHOUSE MANAGEMENT SYSTEM\n";
		cout << "======================================\n\n";

		for (int i = 0; i < MAIN_MENU_ITEMS; ++i) {
			gotoXY(2, 5 + i);
			if (i == selection) {
				cout << "-> \x1b[33;1m" << menuOptions[i] << "\x1b[0m";
			}
			else {
				cout << "   " << menuOptions[i] << "   ";
			}
		}

		keyResult = handleMenuNavigation(selection, MAIN_MENU_ITEMS);

		if (keyResult == -1) {
			switch (selection) {
			case 0: addProduct(); break;
			case 1: removeProduct(); break;
			case 2: replaceProduct(); break;
			case 3: searchProductMenu(); break;
			case 4: sortByPrice(); break;
			case 5: sortByGroup(); break;
			case 6: displayAllProducts(); break;
			case 7:
				saveToFile();
				cleanupWarehouse();
				clearScreen();
				cout << "Thank you for using the Warehouse System. Goodbye!\n";
				return;
			}
		}
		else {
			selection = keyResult;
		}
	}
}

// --- MAIN FUNCTION ---

int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	mainMenu();

	return 0;
}