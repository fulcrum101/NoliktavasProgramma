#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <limits>
#include <string>
#include <vector>
#define MAX 10000000

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
    ProductsContainer() {

    }
    ~ProductsContainer() {

    }

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

        while (source.read((char*)&tempProduct, sizeof(Product))){
            this->products.push_back(tempProduct);
        }

        source.close();
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

//Add product function

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
            cout << "Current price: " << products[existingProductIndex].getProductPrice() << endl;
            cout << "Modify product price by: ";
            cin >> price;
            while (!cin.good()) {
                cin.clear();
	            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: bad input.\n Please input a valid number: ";
                cin >> price;
            }

            cout << "Current stock: " << products[existingProductIndex].getProductLeft() << endl;
            cout << "Modify product in stock by: ";
            cin >> stock;
            while (!cin.good()) {
                cin.clear();
	            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: bad input.\n Please input a valid number: ";
                cin >> stock;
            }

            cout << "Current sold: " << products[existingProductIndex].getProductSold() << endl;
            cout << "Modify product sold by: ";
            cin >> sold;
            while (!cin.good()) {
                cin.clear();
	            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: bad input.\n Please input a valid number: ";
                cin >> sold;
            }

            products[existingProductIndex].setProductPrice(products[existingProductIndex].getProductPrice() + price);
            products[existingProductIndex].setProductSold(products[existingProductIndex].getProductSold() + sold);
            products[existingProductIndex].setProductLeft(products[existingProductIndex].getProductLeft() + stock);

            cout << "\nProduct info updated successfully\n\n";
        } else {
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
    }

//Top 3 most expensive products

    void topExpensive() {
        int topCount = 3;
        if (products.size() < 3) {
            topCount = products.size();
        }
        Product * topExpen = new Product[topCount];
        
        for (int i = 0; i < topCount; i++) {
            topExpen[i] = products[i];
        }
        sort(topExpen, topExpen+3, comparePrice);

        for (int i = 3; i < products.size(); i++) {
            if (products[i].getProductPrice() > topExpen[topCount-1].getProductPrice()) {
                topExpen[topCount-1] = products[i];
                sort(topExpen, topExpen+3, comparePrice);
            }
        }

        cout << "\nTop " << topCount << " most expensive:\n\n";
        for (int i = 0; i < topCount; i++) {
            topExpen[i].productPrint();
        }

    delete [] topExpen;
    }

//Top 3 cheapest products

    void topCheapest()
    {

        float frstmin, secmin, trdmin = MAX;
        string frstnamec, secnamec, trdnamec;

        for (auto prod1: products)
        {

            if (prod1.getProductPrice() < frstmin)
            {
                trdmin = secmin;
                secmin = frstmin;
                frstmin = prod1.getProductPrice();
                frstnamec = prod1.getProductName();
            }
 
            else if (prod1.getProductPrice() < secmin)
            {
                trdmin = secmin;
                secmin = prod1.getProductPrice();
                secnamec = prod1.getProductName();
            }

            else if (prod1.getProductPrice() < trdmin)
            {
                trdmin = prod1.getProductPrice();
                trdnamec = prod1.getProductName();
            }
        }

        cout << endl << "Three cheapest products (In ascending order by price): " << endl; 
        cout << frstnamec << ": " << frstmin << " Euro" << endl;
        cout << secnamec << ": " << secmin << " Euro" << endl;
        cout << trdnamec << ": " << trdmin << " Euro" << endl;
    }

//Top 3 most earning products

    void topMostEarned()
    {
        float frstmax, secmax, trdmax = 0;
        string frstnamee, secnamee, trdnamee;
        float earned;

        for(auto prod2: products)
        {
            earned = prod2.getProductPrice() * prod2.getProductSold();

            if (earned > frstmax)
            {
                trdmax = secmax;
                secmax = frstmax;
                frstmax = earned;
                frstnamee = prod2.getProductName();
            }

            else if (earned > secmax && earned != frstmax)
            {
                trdmax = secmax;
                secmax = earned;
                secnamee = prod2.getProductName();
            }

            else if (earned > trdmax && earned != secmax)
                trdmax = earned;
                trdnamee = prod2.getProductName();
            }

        cout << endl << "Three most earning products: " << endl;
        cout << frstnamee << ": " << frstmax << " Euro" << endl;
        cout << secnamee << ": " << secmax << " Euro" << endl;
        cout << trdnamee << ": " << trdmax << " Euro" << endl;
    }   

//Top 3 least earning products

    void topLeastEarned() {
        int topCount = 3;
        if (products.size() < 3) {
            topCount = products.size();
        }
        Product * leastEarned = new Product[topCount];
        
        for (int i = 0; i < topCount; i++) {
            leastEarned[i] = products[i];
        }
        sort(leastEarned, leastEarned+3, compareEarnings);

        for (int i = 3; i < products.size(); i++) {
            if (products[i].getProductSold() * products[i].getProductPrice()  < leastEarned[0].getProductSold() * leastEarned[0].getProductPrice()) {
                leastEarned[0] = products[i];
                sort(leastEarned, leastEarned+3, compareEarnings);
            }
        }

        cout << "\nTop " << topCount << " least earning products:\n\n";
        for (int i = topCount-1; i >= 0; i--) {
            leastEarned[i].productPrint();
            cout << " Earned: " << leastEarned[i].getProductSold() * leastEarned[i].getProductPrice() << endl;
        }

        delete [] leastEarned;
    }

//Top 3 most sold products

    void topMostSold() {
        int topCount = 3;
        if (products.size() < 3) {
            topCount = products.size();
        }
        Product * topSold = new Product[topCount];
        
        for (int i = 0; i < topCount; i++) {
            topSold[i] = products[i];
        }
        sort(topSold, topSold+3, compareSold);

        for (int i = 3; i < products.size(); i++) {
            if (products[i].getProductSold() > topSold[topCount-1].getProductSold()) {
                topSold[topCount-1] = products[i];
                sort(topSold, topSold+3, compareSold);
            }
        }

        cout << "\nTop " << topCount << " most sold products:\n\n";
        for (int i = 0; i < topCount; i++) {
            topSold[i].productPrint();
        }

        delete [] topSold;
    }

//Top 3 least sold products

    void topLeastSold()
    {

        float frstmins, secmins, trdmins = MAX;
        string frstnames, secnames, trdnames;

        for (auto prod3: products)
        {

            if (prod3.getProductSold() < frstmins)
            {
                trdmins = secmins;
                secmins = frstmins;
                frstmins = prod3.getProductSold();
                frstnames = prod3.getProductName();
            }
 
            else if (prod3.getProductPrice() < secmins)
            {
                trdmins = secmins;
                secmins = prod3.getProductSold();
                secnames = prod3.getProductName();
            }

            else if (prod3.getProductSold() < trdmins)
            {
                trdmins = prod3.getProductSold();
                trdnames = prod3.getProductName();
            }
        }

        cout << endl << "Three least sold products (In ascending order): " << endl; 
        cout << frstnames << ": " << frstmins << " (Amount of sales)" << endl;
        cout << secnames << ": " << secmins << " (Amount of sales)" << endl;
        cout << trdnames << ": " << trdmins << " (Amount of sales)" << endl;
    } 

//Sell product/-s from stock

    void productSelling()
    {
        string sell;
        int count_sale, count_stock, amount;
        cout << "\nType the product you want to sell: ";
        getline(cin >> ws, sell);
        cout << "\nType how many products you want to sell (in numbers): ";
        cin >> amount;

        for (auto prod: products)
        {
            if (sell == prod.getProductName())
            {
                count_sale = prod.getProductSold();
                count_stock = prod.getProductLeft();
                prod.setProductSold(count_sale + amount);
                prod.setProductLeft(count_stock - amount);
                cout << "\nProduct successfully sold!" << endl;
                cout << "\nAmount of products sold after the sale: " << prod.getProductSold() << endl;
                cout << "\nProducts left in stock: " << prod.getProductLeft() << endl;
            }

            else
            {
                continue;
            } 
        }
    }

//Function to show info about a single product

    void oneProductData()
    {
        string singleprod;
        cout << "Choose the product you want to display: ";
        getline(cin >> ws, singleprod);

        for (auto prod: products)
        {
            if (singleprod == prod.getProductName())
            {
                cout << "\nProduct data: " << endl;
                cout << "\nName: " << prod.getProductName() << endl;
                cout << "Price: " << prod.getProductPrice() << endl;
                cout << "Amount left: " << prod.getProductLeft() << endl;
                cout << "Amount sold: " << prod.getProductSold() << endl;
            }

            else
            {
                continue;
            } 
        }
    }

//Menu shown to the user

    void menu()
    {
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

//Main function prototype

int main()
{
    ProductsContainer products;
    products.readFromFile();
    int option;

    while (option != 11)
    {
        products.menu();
        cin >> option;
        cout << "\nChosen Option: " << option << endl;
        enum choices {One = 1, Two = 2, Three = 3, Four = 4, Five = 5, Six = 6, Seven = 7, Eight = 8, Nine = 9, Ten = 10, Eleven = 11};

        switch(option)
        {
            case One:
                //Function to input product
                products.inputProduct();
                break;
            case Two:
                //Function to print all products
                products.printAllProducts();
                break;
            case Three:
                //Function to sell a product
                products.productSelling();
                break;
            case Four:
                //Function to show data about a single product
                products.oneProductData();
                break;
            case Five:
                //Function to show Top 3 most sold products
                products.topMostSold();
                break;
            case Six:
                //Function to show Top 3 least sold products
                products.topLeastSold();
                break;
            case Seven:
                //Function to show Top 3 most earning products
                products.topMostEarned();
                break;
            case Eight:
                //Function to show Top 3 least earning products
                products.topLeastEarned();
                break;
            case Nine:
                //Function to show Top 3 most expensive products
                products.topExpensive();
                break;
            case Ten:
                //Function to show Top 3 cheapest products
                products.topCheapest();
                break;
            case Eleven:
                //End program
                break;
            default:
                cout << "Error: Choose one of the options!" << endl;
        }

        products.saveToFile();
    }
    return 0;
}