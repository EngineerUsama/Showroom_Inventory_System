#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <iomanip>      //These are the preprocessor directive, where the program includes the necessary libraries.

                    
using namespace std;

struct Car              //structure used to store the make, model, year, and availability of a car.
{
    string make;
    string model;
    int year;
    bool available;
};

// Global variables
vector<Car> cars;           //used to store all the cars in the system.
mutex mtx;                  //used to ensure thread safety when accessing the "cars" vector.

// Function to add a new car to the system
void addCar()
{
    Car newCar;
    cout << "Enter make: ";
    cin >> newCar.make;
    cout << "Enter model name: ";
    cin >> newCar.model;
    cout << "Enter year: ";
    cin >> newCar.year;
    newCar.available = true;
    lock_guard<mutex> lock(mtx);    //used to ensure thread safety
    cars.push_back(newCar);
    cout << "Car added successfully." << endl;
}

// Function to update an existing car in the system
void updateCar()
{
    int index;
    cout << "Enter the index of the car to update: ";
    cin >> index;
    if (index >= 0 && index < cars.size())
    {
        cout << "Enter make: ";
        cin >> cars[index].make;
        cout << "Enter model name: ";
        cin >> cars[index].model;
        cout << "Enter year: ";
        cin >> cars[index].year;
        lock_guard<mutex> lock(mtx);        //used to ensure thread safety
        cout << "Car updated successfully." << endl;
    }
    else
    {
        cout << "Invalid index." << endl;
    }
}

// Function to delete an existing car from the system
void deleteCar()
{
    int index;
    cout << "Enter the index of the car to delete: ";
    cin >> index;
    if (index >= 0 && index < cars.size())
    {
        lock_guard<mutex> lock(mtx);        //used to ensure thread safety
        cars.erase(cars.begin() + index);
        cout << "Car deleted successfully." << endl;
    }
    else
    {
        cout << "Invalid index." << endl;
    }
}

// Function to list all cars in the system

//The function uses a lock_guard to lock the "mtx" mutex before accessing the "cars" vector, 
//and then iterates through the vector, printing the make, model, and year of each car.

void listCars()
{
    lock_guard<mutex> lock(mtx);        //used to ensure thread safety
    cout << left << setw(10) << "Index" << setw(20) << "Make" << setw(20) << "Model Name" << setw(10) << "Year" << endl;
    for (int i = 0; i < cars.size(); i++)
    {
        cout << left << setw(10) << i << setw(20) << cars[i].make << setw(20) << cars[i].model << setw(10) << cars[i].year << endl;
    }
}

//This function is used to save all the cars present in the "cars" vector to a file called "cars.txt". 
//The function uses a lock_guard to lock the "mtx" mutex before accessing the "cars" vector,
//and then iterates through the vector, writing the make, model, year, and availability of each car to the file.
void saveCars()
{
    lock_guard<mutex> lock(mtx);        //used to ensure thread safety
    ofstream file("cars.txt");
    for (int i = 0; i < cars.size(); i++)
    {
        file << endl << cars[i].make << "," << cars[i].model << "," << cars[i].year << "," << cars[i].available << endl;
    }
    file.close();
}

//This is the "loadCars" function, which is used to load all the cars from the "cars.txt" file into the "cars" vector. 
//The function uses an "ifstream" object to open the "cars.txt" file, 
//and then reads the file line by line using a "while" loop and the "getline" function.

//For each line, the function creates a new "Car" object called "newCar", 
//and then uses the "substr" and "find" functions to parse the make, model, year, and availability from the line.

//Finally, the function uses a lock_guard to lock the "mtx" mutex before accessing the "cars" vector and push the newCar object to the cars vector.
//After reading all lines, the function closes the file.

void loadCars()
{

    ifstream file("cars.txt");
    string line;
    while (getline(file, line))
    {
        Car newCar;
        int i = 0;
        int j = line.find(",");
        newCar.make = line.substr(i, j - i);
        i = j + 1;
        j = line.find(",", i);
        newCar.model = line.substr(i, j - i);
        i = j + 1;
        j = line.find(",", i);
        newCar.year = stoi(line.substr(i, j - i));      //stoi function to convert the year and availability to integers.
        i = j + 1;
        newCar.available = stoi(line.substr(i));
        lock_guard<mutex> lock(mtx);        //used to ensure thread safety
        cars.push_back(newCar);
    }
    file.close();
}

void displayHeader()                // display function used for formatted output
{
    cout << endl
        << endl;
    cout << "\t\tShowroom Inventory System\t\t" << endl;
    cout << "\t\t=======================================" << endl;
    cout << "\t\tAdd, Update, Delete and List Cars" << endl;
    cout << "\t\t=======================================" << endl;
    cout << endl;
}

void displayFooter()                // display function used for formatted output
{
    cout << endl;
    cout << "\t\tThank you for using the Showroom Inventory System" << endl;
    cout << "\t\t============================================" << endl;
    cout << endl;
}

int main()
{
    // Load cars from file on program start
    displayHeader();
    thread t1(loadCars);
    t1.join();
    cout << "\t\tConcepts Used in this Project." << endl;
    cout << "\t\t1. Mutex or Mutual Exclusion." << endl;
    cout << "\t\t2. Threads or Threading." << endl;

    while (true)                            // infinite loop for creating a menu
    {
        int choice;

        cout << "1. Add car" << endl;
        cout << "2. Update car" << endl;
        cout << "3. Delete car" << endl;
        cout << "4. List cars" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)                 //conditional statement to choose any 1 from the menu
        {
        case 1:
        {
            thread t(addCar);
            t.join();
            system("PAUSE");
            break;
        }
        case 2:
        {
            thread t(updateCar);
            t.join();
            system("PAUSE");
            break;
        }
        case 3:
        {
            thread t(deleteCar);
            t.join();
            system("PAUSE");
            break;
        }
        case 4:
        {
            thread t(listCars);
            t.join();
            system("PAUSE");
            break;
        }
        case 5:
        {
            thread t(saveCars);
            t.join();
            displayFooter();
            system("PAUSE");
            return 0;
        }
        default:
        {
            cout << "Invalid choice." << endl;
            system("PAUSE");
            break;
        }
        }
        system("cls");
    }
    return 0;
}