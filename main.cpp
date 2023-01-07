#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

using namespace std;

const int PRODUCT_NAME_LENGTH = 32;
const char SAVE_FILE_PATH [13] = "saveFile.bin";

class Product {
    //Individual products and functions that work with individual products
    char productName[PRODUCT_NAME_LENGTH];
    float productPrice;
    int productLeftAmount;
    int productSoldAmount;

public:
    Product() {

    }

    Product(char name[PRODUCT_NAME_LENGTH], float price, int leftAmount, int soldAmount) {
        //TODO: Maybe passing a shorter array could lead to accessing restricted memory?
        memcpy(this->productName, name, 32 * sizeof(char));
        this->productPrice = price;
        this->productLeftAmount = leftAmount;
        this->productSoldAmount = soldAmount;
    }

    ~Product() {

    }

    string getProductName() {
        return this->productName;
    }

    float getProductPrice() {
        return this->productPrice;
    } 

    int getProductLeft() {
        return this->productLeftAmount;
    }

    int getProductSold() {
        return this->productSoldAmount;
    }

    void productPrint() {
        cout  << '\n' << left 
            << setw(15) << "Name: "  << productName << '\n' 
            << setw(15) << "Price: " << productPrice << '\n'
            << setw(15) << "In stock: " << productLeftAmount << '\n'
            << setw(15) << "Total sold: " << productSoldAmount << "\n\n";
    }
};

class ProductsContainer {
    //Product storage and functions related to work with all products
    vector <Product> products;

public:
    ProductsContainer() {

    }
    ~ProductsContainer() {

    }

    bool saveToFile() {
        ofstream destination(SAVE_FILE_PATH, ios::binary);

        for (auto prod: products) {
            destination.write((char*)&prod, sizeof(Product));
        }

        destination.close();

        return 1;
    }

    bool readFromFile() {
        //TODO: Return 0 if file not found
        Product tempProduct;
        ifstream source (SAVE_FILE_PATH, ios::binary);

        while (source.read((char*)&tempProduct, sizeof(Product))){
            this->products.push_back(tempProduct);
        }

        source.close();

        return 1;
    }

    void printAllProducts() {
        cout << "\nProdukti:\n";
        for (auto prod: products) {
            prod.productPrint();
        }
    }

    void addProduct(Product prod) {
        this->products.push_back(prod);
    }

    void inputProduct() {
        string nameInput;
        float price;
        int stock;
        int sold;

        cout << "Input product name: ";
        cin.clear();
	    cin.ignore();
        getline(cin, nameInput);
        if (nameInput.length() > PRODUCT_NAME_LENGTH - 1) {
            nameInput.resize(32);
            cout << "ERROR: Product name length exceeds " << PRODUCT_NAME_LENGTH-1 << " characters, "
                << "truncated to: " << nameInput << "\n"; 
        }
        char name[PRODUCT_NAME_LENGTH] = "";
        strcpy(name, nameInput.c_str());

        cout << "Input product price: ";
        cin >> price;
        while (!cin.good()) {
            cin.clear();
	        cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: bad input.\n Please input a valid number: ";
            cin >> price;
        }

        cout << "Input product in stock: ";
        cin >> stock;
        while (!cin.good()) {
            cin.clear();
	        cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: bad input.\n Please input a valid number: ";
            cin >> stock;
        }

        cout << "Input product sold: ";
        cin >> sold;
        while (!cin.good()) {
            cin.clear();
	        cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: bad input.\n Please input a valid number: ";
            cin >> sold;
        }

        cout << "\nProduct added successfully\n\n";

        products.push_back(Product(name, price, stock, sold));



    }
    
};

int main() {
    bool writeMode = 0;
    ProductsContainer products;
    cout << "Write mode? (1 yes, 0 no): ";
    cin >> writeMode;
    if (writeMode) {
        /*
        char name1[32] = "Prod 1";
        char name2[32] = "Prod 2";
        char name3[32] = "Prod 3";
        char name4[32] = "Prod 4";

        Product product1(name1, 12.2, 13, 25);
        Product product2(name2, 25.0, 15, 32);
        Product product3(name3, 137.99, 23, 35);
        Product product4(name4, 64, 132, 52);

        products.addProduct(product1);
        products.addProduct(product2);
        products.addProduct(product3);
        products.addProduct(product4);
        */

        products.inputProduct();
        products.inputProduct();
        
        products.printAllProducts();

        products.saveToFile();
    } else {

        products.readFromFile();

        products.printAllProducts();
    }
}