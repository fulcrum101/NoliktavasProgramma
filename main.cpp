#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

const int PRODUCT_NAME_LENGTH = 32;
const char SAVE_FILE_PATH [13] = "saveFile.bin";

class Product {
    char productName[PRODUCT_NAME_LENGTH];
    float productPrice;
    int productLeftAmount;
    int productSoldAmount;

public:
    Product() {

    }

    Product(char name[PRODUCT_NAME_LENGTH], float price, int leftAmount, int soldAmount) {
        memcpy(this->productName, name, 32 * sizeof(char));
        this->productPrice = price;
        this->productLeftAmount = leftAmount;
        this->productSoldAmount = soldAmount;
    }

    ~Product() {

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
    }

    bool readFromFile() {
        Product tempProduct;
        ifstream source (SAVE_FILE_PATH, ios::binary);

        while (source.read((char*)&tempProduct, sizeof(Product))){
            this->products.push_back(tempProduct);
        }

        source.close();
    }

    void printAllProducts() {
        for (auto prod: products) {
            prod.productPrint();
        }
    }

    void addProduct(Product prod) {
        this->products.push_back(prod);
    }
};

int main() {
    bool WRITE_MODE = 0;

    if (WRITE_MODE) {
        char name1[32] = "Prod 1";
        char name2[32] = "Prod 2";
        char name3[32] = "Prod 3";
        char name4[32] = "Prod 4";

        Product product1(name1, 12.2, 13, 25);
        Product product2(name2, 25.0, 15, 32);
        Product product3(name3, 137.99, 23, 35);
        Product product4(name4, 64, 132, 52);

        ProductsContainer products;
        products.addProduct(product1);
        products.addProduct(product2);
        products.addProduct(product3);
        products.addProduct(product4);

        products.printAllProducts();

        products.saveToFile();
    } else {
        ProductsContainer products;

        products.readFromFile();

        products.printAllProducts();
    }
}