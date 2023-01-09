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
    return firstProduct.getProductSold() * firstProduct.getProductPrice() < secondProduct.getProductSold() * secondProduct.getProductPrice();
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

//Trīs dārgākie funkcija

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
            if (products[i].getProductSold() > topExpen[topCount-1].getProductSold()) {
                topExpen[topCount-1] = products[i];
                sort(topExpen, topExpen+3, comparePrice);
            }
        }

        cout << "\nTop " << topCount << " most expensive:\n\n";
        for (int i = 0; i < topCount; i++) {
            topExpen[i].productPrint();
        }

        //delete [] topSold;
    }

//Trīs lētākie funkcija

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

//Trīs pelnošākie produkti

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

//Trīs vis nepelnošākie produkti

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
            if (products[i].getProductSold() < leastEarned[0].getProductSold()) {
                leastEarned[0] = products[i];
                sort(leastEarned, leastEarned+3, compareEarnings);
            }
        }

        cout << "\nTop " << topCount << " most earning products:\n\n";
        for (int i = 0; i < topCount; i++) {
            leastEarned[i].productPrint();
            cout << " Earned: " << leastEarned[i].getProductSold() * leastEarned[i].getProductPrice() << endl;
        }

        //delete [] topSold;
    }

//Trīs visvairāk pārdotie produkti

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

        //delete [] topSold;
    }

//Trīs vismazāk pārdotie produkti

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

//Pārdod produktus

    void productSelling()
    {
        string sell;
        int count_sale, count_stock, amount;
        cout << "Type the product you want to sell: ";
        getline(cin >> ws, sell);
        cout << "Type how many products you want to sell (in numbers): ";
        cin >> amount;

        for (auto prod: products)
        {
            if (sell == prod.getProductName())
            {
                count_sale = prod.getProductSold();
                count_stock = prod.getProductLeft();
                prod.setProductSold(count_sale + amount);
                prod.setProductLeft(count_stock - amount);
                cout << "Product successfully sold!" << endl;
                cout << "Amount of products sold after the sale: " << prod.getProductSold() << endl;
                cout << "Products left in stock: " << prod.getProductLeft() << endl;
            }

            else
            {
                continue;
            } 
        }
    }

//Viens produkts dati

    void oneProductData()
    {
        string singleprod;
        cout << "Choose the product you want to display: ";
        getline(cin >> ws, singleprod);

        for (auto prod: products)
        {
            if (singleprod == prod.getProductName())
            {
                cout << "Product data: " << endl;
                cout << "Name: " << prod.getProductName() << endl;
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
        products.inputProduct();
        products.inputProduct();
        
        
        products.printAllProducts();

        products.saveToFile();
    } else {

        products.readFromFile();
        //products.printAllProducts();
        products.topMostSold();
        products.topExpensive();
        products.topLeastEarned();
        products.inputProduct();
        products.printAllProducts();
    }

    return 0;
}