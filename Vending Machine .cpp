#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
const string ADMIN_PASSWORD = "admin123";
// structure to hold item details
struct Item
{
    string name;
    double price;
    int quantity;
};
// ---------- function prototypes ----------
void showMenu(Item items[], int size);
bool isValidChoice(int choice, int size);
double takePayment(double price);
void buyItem(Item items[], int size, double &totalSales);
void adminPanel(Item items[], int size, double &totalSales);
void restockItem(Item items[], int size);
void salesReport(Item items[], int size, double totalSales);
void logSale(string itemName, double price);
int getIntInput(string prompt);
int main()
{
    Item items[6] = {
        {"Chips",        50.0, 5},
        {"Cold Drink",   80.0, 3},
        {"Chocolate",    60.0, 4},
        {"Biscuits",     30.0, 6},
        {"Juice",        70.0, 2},
        {"Water Bottle", 20.0, 8}
    };

    int size = 6;
    double totalSales = 0.0;
    int mainChoice;

    cout << "\n***********************************\n";
    cout << "   WELCOME TO SMART VENDING MACHINE\n";
    cout << "***********************************\n";

    do
    {
        cout << "\n--------- MAIN MENU ---------\n";
        cout << "1. Show Items\n";
        cout << "2. Buy Item\n";
        cout << "3. Admin Panel\n";
        cout << "4. Exit\n";

        mainChoice = getIntInput("Enter your choice: ");

        switch (mainChoice)
        {
        case 1:
            showMenu(items, size);
            break;

        case 2:
            buyItem(items, size, totalSales);
            break;

        case 3:
            adminPanel(items, size, totalSales);
            break;

        case 4:
            cout << "\nThank you for using the vending machine. Goodbye!\n";
            break;

        default:
            cout << "Invalid option, please try again.\n";
        }

    } while (mainChoice != 4);

    return 0;
}
// takes an integer input safely (handles wrong input like letters)
int getIntInput(string prompt)
{
    int value;
    cout << prompt;
    cin >> value;

    while (cin.fail())
    {
        cin.clear();               // clear the error flag
        cin.ignore(1000, '\n');    // ignore the wrong input
        cout << "Invalid input, please enter a number: ";
        cin >> value;
    }
    return value;
}
// display all items nicely in a table
void showMenu(Item items[], int size)
{
    cout << "\n===================================================\n";
    cout << left << setw(6) << "Code" << setw(15) << "Item"
         << setw(10) << "Price" << setw(10) << "Stock" << "\n";
    cout << "---------------------------------------------------\n";

    for (int i = 0; i < size; i++)
    {
        cout << left << setw(6) << i + 1 << setw(15) << items[i].name
             << "Rs. " << setw(6) << items[i].price;

        if (items[i].quantity == 0)
            cout << "OUT OF STOCK";
        else
            cout << items[i].quantity;

        cout << "\n";
    }
    cout << "===================================================\n";
}

bool isValidChoice(int choice, int size)
{
    return (choice >= 1 && choice <= size);
}
// handles cash collection, keeps asking until enough money inserted
double takePayment(double price)
{
    double totalPaid = 0, amount;

    while (totalPaid < price)
    {
        cout << "Remaining amount to insert: Rs. " << (price - totalPaid) << "\n";
        cout << "Insert amount: Rs. ";
        cin >> amount;

        if (cin.fail() || amount <= 0)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid amount, try again.\n";
            continue;
        }

        totalPaid += amount;
    }
    return totalPaid;
}
// saves purchase record to a text file (basic file handling)
void logSale(string itemName, double price)
{
    ofstream file("sales_log.txt", ios::app);
    if (file.is_open())
    {
        file << itemName << " sold for Rs. " << price << endl;
        file.close();
    }
}
// buying process, allows more than one item in same session
void buyItem(Item items[], int size, double &totalSales)
{
    char again = 'y';

    while (again == 'y' || again == 'Y')
    {
        showMenu(items, size);
        int choice = getIntInput("Enter the code of item you want to buy (0 to cancel): ");

        if (choice == 0)
        {
            cout << "Purchase cancelled.\n";
            return;
        }

        if (!isValidChoice(choice, size))
        {
            cout << "Invalid item code.\n";
            continue;
        }

        int index = choice - 1;

        if (items[index].quantity <= 0)
        {
            cout << "Sorry, " << items[index].name << " is out of stock.\n";
            continue;
        }

        cout << "\nYou selected: " << items[index].name
             << " (Rs. " << items[index].price << ")\n";

        double paid = takePayment(items[index].price);
        double change = paid - items[index].price;

        items[index].quantity--;
        totalSales += items[index].price;
        logSale(items[index].name, items[index].price);

        cout << "\nDispensing " << items[index].name << "...\n";
        if (change > 0)
            cout << "Please collect your change: Rs. " << change << "\n";

        cout << "Enjoy! Here is your receipt:\n";
        cout << "-----------------------------\n";
        cout << "Item: " << items[index].name << "\n";
        cout << "Price: Rs. " << items[index].price << "\n";
        cout << "Paid: Rs. " << paid << "\n";
        cout << "Change: Rs. " << change << "\n";
        cout << "-----------------------------\n";

        cout << "\nDo you want to buy another item? (y/n): ";
        cin >> again;
    }
}
// admin section protected by password
void adminPanel(Item items[], int size, double &totalSales)
{
    string password;
    cout << "\nEnter admin password: ";
    cin >> password;

    if (password != ADMIN_PASSWORD)
    {
        cout << "Incorrect password. Access denied.\n";
        return;
    }

    int adminChoice;
    do
    {
        cout << "\n------ ADMIN PANEL ------\n";
        cout << "1. Restock Item\n";
        cout << "2. View Sales Report\n";
        cout << "3. Back to Main Menu\n";

        adminChoice = getIntInput("Enter your choice: ");

        switch (adminChoice)
        {
        case 1:
            restockItem(items, size);
            break;
        case 2:
            salesReport(items, size, totalSales);
            break;
        case 3:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (adminChoice != 3);
}

void restockItem(Item items[], int size)
{
    showMenu(items, size);
    int choice = getIntInput("Enter item code to restock: ");

    if (!isValidChoice(choice, size))
    {
        cout << "Invalid item code.\n";
        return;
    }

    int qty = getIntInput("Enter quantity to add: ");

    if (qty <= 0)
    {
        cout << "Quantity must be positive.\n";
        return;
    }

    items[choice - 1].quantity += qty;
    cout << items[choice - 1].name << " restocked successfully. "
         << "New stock: " << items[choice - 1].quantity << "\n";
}
// shows total sales and highlights low stock items
void salesReport(Item items[], int size, double totalSales)
{
    cout << "\n============ SALES REPORT ============\n";
    cout << "Total Sales So Far: Rs. " << totalSales << "\n";
    cout << "----------------------------------------\n";
    cout << "Low Stock Items (less than 3 left):\n";

    bool anyLow = false;
    for (int i = 0; i < size; i++)
    {
        if (items[i].quantity < 3)
        {
            cout << "- " << items[i].name << " (only " << items[i].quantity << " left)\n";
            anyLow = true;
        }
    }

    if (!anyLow)
        cout << "None. All items sufficiently stocked.\n";

    cout << "========================================\n";
}
