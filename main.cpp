#include <algorithm>
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
        memcpy(this->productName, name, PRODUCT_NAME_LENGTH * sizeof(char));
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

    void setProductPrice(float price) {
        this->productPrice = price;
    } 

    void setProductLeft(int left) {
        this->productLeftAmount = left;
    }

    void setProductSold(int sold) {
        this->productSoldAmount = sold;
    }

    void productPrint() {
        cout  << '\n' << left 
            << setw(15) << "Name: "  << productName << '\n' 
            << setw(15) << "Price: " << productPrice << '\n'
            << setw(15) << "In stock: " << productLeftAmount << '\n'
            << setw(15) << "Total sold: " << productSoldAmount << "\n\n";
    }
};

bool compareSold(Product firstProduct, Product secondProduct) {
    return firstProduct.getProductSold() > secondProduct.getProductSold();
}

bool compareEarnings(Product firstProduct, Product secondProduct) {
    return firstProduct.getProductSold() * firstProduct.getProductPrice() > secondProduct.getProductSold() * secondProduct.getProductPrice();
}

bool comparePrice(Product firstProduct, Product secondProduct) {
    return firstProduct.getProductPrice() > secondProduct.getProductPrice();
}
    
class ProductsContainer {
    //Product storage and functions related to work with all products
    vector <Product> products;

public:
    //Task functions
    ProductsContainer() {

    }
	
    ~ProductsContainer() {

    }

    //File reading and saving functions
    void saveToFile() {
        ofstream destination(SAVE_FILE_PATH, ios::binary);

        for (auto prod: products) {
            destination.write((char*)&prod, sizeof(Product));
        }

        destination.close();
    }

    void readFromFile() {
        Product tempProduct;
        ifstream source (SAVE_FILE_PATH, ios::binary);

        if (!source.is_open()) {
            ofstream destination(SAVE_FILE_PATH, ios::binary);
            destination.close();
            source.open(SAVE_FILE_PATH, ios::binary);
        }

        while (source.read((char*)&tempProduct, sizeof(Product))) {
            this->products.push_back(tempProduct);
        }

        source.close();
    }

    //Input product function (Task 1/Option 1)
    void inputProduct() {
        bool productExists = 0;
        int existingProductIndex = -1;
        string nameInput;
        float price;
        int stock;
        int sold;

        cout << "Input product name: ";
        cin.clear();
        cin.ignore();
        getline(cin, nameInput);
	    
        if (nameInput.length() > PRODUCT_NAME_LENGTH - 1) {
            nameInput.resize(PRODUCT_NAME_LENGTH);
            cout << "ERROR: Product name length exceeds " << PRODUCT_NAME_LENGTH-1 << " characters, "
                << "truncated to: " << nameInput << "\n"; 
        }
	    
        char name[PRODUCT_NAME_LENGTH] = "";
        strcpy(name, nameInput.c_str());

        for (int i = 0; i < products.size(); i++) {
            if (products[i].getProductName() == name) {
                existingProductIndex = i;
                productExists = 1;
                break;
            }
        }

        if (productExists) {
            cout << "Product \"" << name << "\" already exists, entering moddifying mode" << endl;
            cout << "Current stock: " << products[existingProductIndex].getProductLeft() << endl;
            cout << "Modify product in stock by: ";
            cin >> stock;
		
            while (!cin.good() || stock < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: bad input.\n Please input a valid number: ";
                cin >> stock;
            }

            products[existingProductIndex].setProductLeft(products[existingProductIndex].getProductLeft() + stock);

            cout << "\nProduct info updated successfully\n\n";
        } else {
            cout << "Input product price: ";
            cin >> price;
		
            while (!cin.good() || price < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: bad input.\n Please input a valid number: ";
                cin >> price;
            }

            cout << "Input product in stock: ";
            cin >> stock;
		
            while (!cin.good() || stock < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: bad input.\n Please input a valid number: ";
                cin >> stock;
            }

            sold = 0;
            products.push_back(Product(name, price, stock, sold));
            cout << "\nProduct added successfully\n\n";
        }
    }

    //Print all products (Task 2/Option 2)
    void printAllProducts() {
        cout << "\nProdukti:\n";
        for (auto prod: products) {
            prod.productPrint();
        }
    }

    //Sell product/-s from stock (Task 3/Option 3)
    void productSelling() {
        bool productFound = 0;
        string sell;
        int count_sale, count_stock, amount;
	    
        cout << "\nType the product you want to sell: ";
        getline(cin >> ws, sell);

        for (auto &prod: products) {
            if (sell == prod.getProductName()) {
                productFound = 1;

                cout << "\nType how many products you want to sell (in numbers): ";
                cin >> amount;

                count_sale = prod.getProductSold();
                count_stock = prod.getProductLeft();
		    
                if (count_stock - amount >= 0) {
                    prod.setProductSold(count_sale + amount);
                    prod.setProductLeft(count_stock - amount);
                    cout << "\nProduct successfully sold!" << endl;
                    cout << "\nAmount of products sold after the sale: " << prod.getProductSold() << endl;
                    cout << "\nProducts left in stock: " << prod.getProductLeft() << endl;
                } else {
                    cout << "Error: Not enough product in stock to sell, action canceled!\n\n";
                }   
            } else {
                continue;
            }
        }

        if (!productFound) {
            cout << "Product \"" << sell << "\" was not found!\n\n";
        }
    }

    //Function to show info about a single product (Task 4/Option 4)
    void oneProductData() {
        bool productFound = 0;
        string singleprod;
	    
        cout << "\nChoose the product you want to display: ";
        getline(cin >> ws, singleprod);

        for (auto prod: products) {
            if (singleprod == prod.getProductName()) {
                productFound = 1;

                cout << "\nProduct data: " << endl;
                cout << "\nName: " << prod.getProductName() << endl;
                cout << "Price: " << prod.getProductPrice() << endl;
                cout << "Amount left: " << prod.getProductLeft() << endl;
                cout << "Amount sold: " << prod.getProductSold() << endl;
            } else {
                continue;
            } 
        }

        if (!productFound) {
            cout << "Product \"" << singleprod << "\" was not found!\n\n";
        }
    }

    //Top 3 most sold products (Task 5/Option 5)
    void topMostSold() {
        vector<Product> tempProducts = this->products;
        sort(tempProducts.begin(), tempProducts.end(), compareSold);

        for (int i = 0; i < 3 && i < tempProducts.size(); i++) {
            cout << "\nTop " << i + 1 << " of most sold: \n";
            tempProducts[i].productPrint();
        }
    }

    //Top 3 least sold products (Task 6/Option 6)
    void topLeastSold() {
        vector<Product> tempProducts = this -> products;
        sort(tempProducts.begin(), tempProducts.end(), compareSold);
        reverse(tempProducts.begin(), tempProducts.end());

        for (int i = 0; i < 3 && i < tempProducts.size(); i++) {
            cout << "\nTop " << i + 1 << " of least sold: \n";
            tempProducts[i].productPrint();
        }
    }

    //Top 3 most earning products (Task 7/Option 7)
    void topMostEarned() {
        vector<Product> tempProducts = this -> products;
        sort(tempProducts.begin(), tempProducts.end(), compareEarnings);

        for (int i = 0; i < 3 && i < tempProducts.size(); i++) {
            cout << "\nTop " << i + 1 << " of most earned: \n";
            tempProducts[i].productPrint();
        }
    }

    //Top 3 least earning products (Task 8/Option 8)
    void topLeastEarned() {
        vector<Product> tempProducts = this->products;
        sort(tempProducts.begin(), tempProducts.end(), compareEarnings);
        reverse(tempProducts.begin(), tempProducts.end());

        for (int i = 0; i < 3 && i < tempProducts.size(); i++) {
            cout << "\nTop " << i + 1 << " of least earning: \n";
            tempProducts[i].productPrint();
        }
    }

    //Top 3 most expensive products (Task 9/Option 9)
    void topExpensive() {
        vector<Product> tempProducts = this->products;
        sort(tempProducts.begin(), tempProducts.end(), comparePrice);

        for (int i = 0; i < 3 && i < tempProducts.size(); i++) {
            cout << "\nTop " << i + 1 << " of most expensive: \n";
            tempProducts[i].productPrint();
        }
    }

    //Top 3 cheapest products (Task 10/Option 10)
    void topCheapest() {
        vector<Product> tempProducts = this -> products;
        sort(tempProducts.begin(), tempProducts.end(), comparePrice);
        reverse(tempProducts.begin(), tempProducts.end());

        for (int i = 0; i < 3 && i < tempProducts.size(); i++) {
            cout << "\nTop " << i + 1 << " of cheapest: \n";
            tempProducts[i].productPrint();
        }
    }

    //Menu shown to the user
    void menu() {
        cout << "\nOption 1: Add a product/-s to stock" << endl;
        cout << "Option 2: Show all the products in stock" << endl;
        cout << "Option 3: Sell a product/-s" << endl;
        cout << "Option 4: Data about a product" << endl;
        cout << "Option 5: Top 3 most sold products" << endl;
        cout << "Option 6: Top 3 least sold products" << endl;
        cout << "Option 7: Top 3 most earning products" << endl;
        cout << "Option 8: Top 3 least earning products" << endl;
        cout << "Option 9: Top 3 most expensive products" << endl;
        cout << "Option 10: Top 3 cheapest products" << endl;
        cout << "Option 11: End program" << endl;
        cout << "Choose an option (by typing a number): ";
    }
};

//Main function
int main() {
    ProductsContainer products;
    products.readFromFile();
    int option;

    while (option != 11) {
        products.menu();
        cin >> option;
        cout << "\nChosen Option: " << option << endl;
        enum choices {One = 1, Two = 2, Three = 3, Four = 4, Five = 5, Six = 6, Seven = 7, Eight = 8, Nine = 9, Ten = 10, Eleven = 11};

        switch(option) {
            case One:
                //Function to input product (Line 131)
                products.inputProduct();
                break;
            case Two:
                //Function to print all products (Line 232)
                products.printAllProducts();
                break;
            case Three:
                //Function to sell a product (Line 241)
                products.productSelling();
                break;
            case Four:
                //Function to show data about a single product (Line 272)
                products.oneProductData();
                break;
            case Five:
                //Function to show Top 3 most sold products (Line 298)
                products.topMostSold();
                break;
            case Six:
                //Function to show Top 3 least sold products (Line 327)
                products.topLeastSold();
                break;
            case Seven:
                //Function to show Top 3 most earning products (Line 351)
                products.topMostEarned();
                break;
            case Eight:
                //Function to show Top 3 least earning products (Line 374)
                products.topLeastEarned();
                break;
            case Nine:
                //Function to show Top 3 most expensive products (Line 404)
                products.topExpensive();
                break;
            case Ten:
                //Function to show Top 3 cheapest products (Line 433)
                products.topCheapest();
                break;
            case Eleven:
                //End program
                break;
            default:
                cout << "Error: Choose one of the options!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    products.saveToFile();
    return 0;
}
