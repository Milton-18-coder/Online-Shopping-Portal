#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

using namespace std;

/* =========================================================
   PRODUCT CLASS
   ========================================================= */

class Product
{
private:
    int productId;
    string name;
    string category;
    double price;
    int stock;

public:

    Product()
    {
        productId = 0;
        name = "";
        category = "";
        price = 0;
        stock = 0;
    }

    Product(int id, string n, string c, double p, int s)
    {
        productId = id;
        name = n;
        category = c;
        price = p;
        stock = s;
    }

    // Getters
    int getId() const
    {
        return productId;
    }

    string getName() const
    {
        return name;
    }

    string getCategory() const
    {
        return category;
    }

    double getPrice() const
    {
        return price;
    }

    int getStock() const
    {
        return stock;
    }

    // Setter
    void setStock(int s)
    {
        stock = s;
    }

    void display() const
    {
        cout << left
             << setw(8) << productId
             << setw(25) << name
             << setw(18) << category
             << setw(12) << fixed << setprecision(2) << price
             << setw(8) << stock
             << endl;
    }
};


/* =========================================================
   ABSTRACT PAYMENT CLASS
   ========================================================= */

class Payment
{
protected:
    double amount;

public:

    Payment(double a)
    {
        amount = a;
    }

    virtual void pay() = 0;

    virtual ~Payment()
    {
    }
};


/* =========================================================
   CREDIT CARD PAYMENT
   ========================================================= */

class CreditCardPayment : public Payment
{
private:
    string cardNumber;

public:

    CreditCardPayment(double a, string card)
        : Payment(a)
    {
        cardNumber = card;
    }

    void pay() override
    {
        cout << "\nPayment Method : Credit Card" << endl;
        cout << "Amount         : Rs. "
             << fixed << setprecision(2)
             << amount << endl;

        cout << "Card Number    : **** **** **** "
             << cardNumber.substr(cardNumber.length() - 4)
             << endl;

        cout << "Payment Successful!" << endl;
    }
};


/* =========================================================
   UPI PAYMENT
   ========================================================= */

class UPIPayment : public Payment
{
private:
    string upiId;

public:

    UPIPayment(double a, string upi)
        : Payment(a)
    {
        upiId = upi;
    }

    void pay() override
    {
        cout << "\nPayment Method : UPI" << endl;
        cout << "UPI ID         : " << upiId << endl;
        cout << "Amount         : Rs. "
             << fixed << setprecision(2)
             << amount << endl;

        cout << "Payment Successful!" << endl;
    }
};


/* =========================================================
   CASH ON DELIVERY
   ========================================================= */

class CashOnDelivery : public Payment
{
public:

    CashOnDelivery(double a)
        : Payment(a)
    {
    }

    void pay() override
    {
        cout << "\nPayment Method : Cash on Delivery" << endl;

        cout << "Amount to Pay  : Rs. "
             << fixed << setprecision(2)
             << amount << endl;

        cout << "Order placed successfully!" << endl;
    }
};


/* =========================================================
   CART ITEM
   ========================================================= */

class CartItem
{
private:
    Product product;
    int quantity;

public:

    CartItem(Product p, int q)
    {
        product = p;
        quantity = q;
    }

    Product getProduct() const
    {
        return product;
    }

    int getQuantity() const
    {
        return quantity;
    }

    double getTotal() const
    {
        return product.getPrice() * quantity;
    }

    void increaseQuantity(int q)
    {
        quantity += q;
    }
};


/* =========================================================
   CART CLASS
   ========================================================= */

class Cart
{
private:
    vector<CartItem> items;

public:

    void addProduct(Product product, int quantity)
    {
        if (quantity <= 0)
        {
            throw invalid_argument("Quantity must be greater than zero.");
        }

        if (quantity > product.getStock())
        {
            throw runtime_error("Insufficient stock.");
        }

        for (auto &item : items)
        {
            if (item.getProduct().getId() == product.getId())
            {
                if (item.getQuantity() + quantity > product.getStock())
                {
                    throw runtime_error("Requested quantity exceeds stock.");
                }

                item.increaseQuantity(quantity);

                cout << "Product quantity updated in cart.\n";
                return;
            }
        }

        items.push_back(CartItem(product, quantity));

        cout << "Product added to cart successfully.\n";
    }


    void removeProduct(int productId)
    {
        auto it = remove_if(
            items.begin(),
            items.end(),
            [productId](const CartItem &item)
            {
                return item.getProduct().getId() == productId;
            }
        );

        if (it != items.end())
        {
            items.erase(it, items.end());

            cout << "Product removed from cart.\n";
        }
        else
        {
            cout << "Product not found in cart.\n";
        }
    }


    void displayCart() const
    {
        if (items.empty())
        {
            cout << "\nCart is empty.\n";
            return;
        }

        cout << "\n================ YOUR CART ================\n";

        cout << left
             << setw(8) << "ID"
             << setw(25) << "Product"
             << setw(12) << "Price"
             << setw(10) << "Quantity"
             << setw(12) << "Total"
             << endl;

        cout << string(67, '-') << endl;

        double grandTotal = 0;

        for (const auto &item : items)
        {
            cout << left
                 << setw(8) << item.getProduct().getId()
                 << setw(25) << item.getProduct().getName()
                 << setw(12) << item.getProduct().getPrice()
                 << setw(10) << item.getQuantity()
                 << setw(12) << item.getTotal()
                 << endl;

            grandTotal += item.getTotal();
        }

        cout << string(67, '-') << endl;

        cout << "Grand Total : Rs. "
             << fixed << setprecision(2)
             << grandTotal << endl;
    }


    double getTotal() const
    {
        double total = 0;

        for (const auto &item : items)
        {
            total += item.getTotal();
        }

        return total;
    }


    bool empty() const
    {
        return items.empty();
    }


    vector<CartItem> getItems() const
    {
        return items;
    }


    void clear()
    {
        items.clear();
    }
};


/* =========================================================
   ORDER CLASS
   ========================================================= */

class Order
{
private:
    int orderId;
    vector<CartItem> items;
    double totalAmount;
    string paymentMethod;

public:

    Order(
        int id,
        vector<CartItem> orderItems,
        double total,
        string method
    )
    {
        orderId = id;
        items = orderItems;
        totalAmount = total;
        paymentMethod = method;
    }


    void displayOrder() const
    {
        cout << "\n========================================\n";

        cout << "Order ID       : " << orderId << endl;

        cout << "Payment Method : "
             << paymentMethod << endl;

        cout << "\nProducts:\n";

        for (const auto &item : items)
        {
            cout << "- "
                 << item.getProduct().getName()
                 << " x "
                 << item.getQuantity()
                 << " = Rs. "
                 << item.getTotal()
                 << endl;
        }

        cout << "\nTotal Amount   : Rs. "
             << fixed << setprecision(2)
             << totalAmount
             << endl;

        cout << "========================================\n";
    }

    int getOrderId() const
    {
        return orderId;
    }
};


/* =========================================================
   USER CLASS
   ========================================================= */

class User
{
protected:
    string username;
    string password;

public:

    User(string u, string p)
    {
        username = u;
        password = p;
    }

    virtual void showRole() const = 0;

    string getUsername() const
    {
        return username;
    }

    bool login(string u, string p) const
    {
        return username == u && password == p;
    }

    virtual ~User()
    {
    }
};


/* =========================================================
   CUSTOMER CLASS
   ========================================================= */

class Customer : public User
{
private:
    Cart cart;
    vector<Order> orders;

public:

    Customer(string u, string p)
        : User(u, p)
    {
    }


    void showRole() const override
    {
        cout << "Role : Customer\n";
    }


    Cart& getCart()
    {
        return cart;
    }


    void addOrder(Order order)
    {
        orders.push_back(order);
    }


    void showOrderHistory() const
    {
        if (orders.empty())
        {
            cout << "\nNo orders found.\n";
            return;
        }

        cout << "\n========== ORDER HISTORY ==========\n";

        for (const auto &order : orders)
        {
            order.displayOrder();
        }
    }
};


/* =========================================================
   ADMIN CLASS
   ========================================================= */

class Admin : public User
{
public:

    Admin(string u, string p)
        : User(u, p)
    {
    }

    void showRole() const override
    {
        cout << "Role : Administrator\n";
    }
};


/* =========================================================
   SHOPPING PORTAL CLASS
   ========================================================= */

class ShoppingPortal
{
private:
    vector<Product> products;

    vector<Customer> customers;

    Admin admin;

    int nextOrderId;


public:

    ShoppingPortal()
        : admin("admin", "admin123")
    {
        nextOrderId = 1001;

        loadProducts();

        customers.push_back(
            Customer("milton", "1234")
        );
    }


    /* =====================================================
       PRODUCT MANAGEMENT
       ===================================================== */

    void loadProducts()
    {
        products.push_back(
            Product(
                101,
                "Laptop",
                "Electronics",
                55000,
                10
            )
        );

        products.push_back(
            Product(
                102,
                "Smartphone",
                "Electronics",
                25000,
                15
            )
        );

        products.push_back(
            Product(
                103,
                "Headphones",
                "Electronics",
                2500,
                20
            )
        );

        products.push_back(
            Product(
                104,
                "T-Shirt",
                "Fashion",
                799,
                30
            )
        );

        products.push_back(
            Product(
                105,
                "Jeans",
                "Fashion",
                1499,
                25
            )
        );

        products.push_back(
            Product(
                106,
                "Backpack",
                "Accessories",
                1200,
                18
            )
        );

        products.push_back(
            Product(
                107,
                "Running Shoes",
                "Footwear",
                2999,
                12
            )
        );
    }


    void displayProducts()
    {
        cout << "\n================ PRODUCTS ================\n";

        cout << left
             << setw(8) << "ID"
             << setw(25) << "Name"
             << setw(18) << "Category"
             << setw(12) << "Price"
             << setw(8) << "Stock"
             << endl;

        cout << string(71, '-') << endl;

        for (const auto &product : products)
        {
            product.display();
        }
    }


    Product* findProduct(int id)
    {
        for (auto &product : products)
        {
            if (product.getId() == id)
            {
                return &product;
            }
        }

        return nullptr;
    }


    void searchProduct()
    {
        string keyword;

        cout << "\nEnter product name to search: ";

        cin.ignore();
        getline(cin, keyword);

        bool found = false;

        for (const auto &product : products)
        {
            string name = product.getName();

            transform(
                name.begin(),
                name.end(),
                name.begin(),
                ::tolower
            );

            string search = keyword;

            transform(
                search.begin(),
                search.end(),
                search.begin(),
                ::tolower
            );

            if (name.find(search) != string::npos)
            {
                product.display();

                found = true;
            }
        }

        if (!found)
        {
            cout << "No products found.\n";
        }
    }


    void addProduct()
    {
        int id;
        string name;
        string category;
        double price;
        int stock;

        cout << "\nEnter Product ID: ";
        cin >> id;

        cout << "Enter Product Name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter Category: ";
        getline(cin, category);

        cout << "Enter Price: ";
        cin >> price;

        cout << "Enter Stock: ";
        cin >> stock;

        products.push_back(
            Product(
                id,
                name,
                category,
                price,
                stock
            )
        );

        cout << "\nProduct added successfully!\n";
    }


    void removeProduct()
    {
        int id;

        cout << "\nEnter Product ID to remove: ";
        cin >> id;

        auto it = remove_if(
            products.begin(),
            products.end(),
            [id](const Product &p)
            {
                return p.getId() == id;
            }
        );

        if (it != products.end())
        {
            products.erase(it, products.end());

            cout << "Product removed successfully.\n";
        }
        else
        {
            cout << "Product not found.\n";
        }
    }


    /* =====================================================
       CUSTOMER LOGIN
       ===================================================== */

    Customer* customerLogin()
    {
        string username;
        string password;

        cout << "\n========== CUSTOMER LOGIN ==========\n";

        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;

        for (auto &customer : customers)
        {
            if (customer.login(username, password))
            {
                cout << "\nLogin successful!\n";

                return &customer;
            }
        }

        cout << "\nInvalid username or password.\n";

        return nullptr;
    }


    /* =====================================================
       CUSTOMER REGISTRATION
       ===================================================== */

    void registerCustomer()
    {
        string username;
        string password;

        cout << "\n========== REGISTER ==========\n";

        cout << "Enter username: ";
        cin >> username;

        for (const auto &customer : customers)
        {
            if (customer.getUsername() == username)
            {
                cout << "Username already exists.\n";
                return;
            }
        }

        cout << "Enter password: ";
        cin >> password;

        customers.push_back(
            Customer(username, password)
        );

        cout << "\nRegistration successful!\n";
    }


    /* =====================================================
       SHOPPING MENU
       ===================================================== */

    void customerMenu(Customer &customer)
    {
        int choice;

        do
        {
            cout << "\n\n====================================\n";
            cout << "        CUSTOMER DASHBOARD\n";
            cout << "====================================\n";

            cout << "Welcome, "
                 << customer.getUsername()
                 << "!\n\n";

            cout << "1. View Products\n";
            cout << "2. Search Product\n";
            cout << "3. Add Product to Cart\n";
            cout << "4. View Cart\n";
            cout << "5. Remove Product from Cart\n";
            cout << "6. Checkout\n";
            cout << "7. Order History\n";
            cout << "8. Logout\n";

            cout << "\nEnter choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    displayProducts();
                    break;

                case 2:
                    searchProduct();
                    break;

                case 3:
                {
                    int id;
                    int quantity;

                    displayProducts();

                    cout << "\nEnter Product ID: ";
                    cin >> id;

                    cout << "Enter Quantity: ";
                    cin >> quantity;

                    Product *product = findProduct(id);

                    try
                    {
                        if (product == nullptr)
                        {
                            throw runtime_error(
                                "Product not found."
                            );
                        }

                        customer.getCart().addProduct(
                            *product,
                            quantity
                        );
                    }
                    catch (const exception &e)
                    {
                        cout << "Error: "
                             << e.what()
                             << endl;
                    }

                    break;
                }


                case 4:

                    customer.getCart().displayCart();

                    break;


                case 5:
                {
                    int id;

                    cout << "\nEnter Product ID: ";
                    cin >> id;

                    customer.getCart().removeProduct(id);

                    break;
                }


                case 6:

                    checkout(customer);

                    break;


                case 7:

                    customer.showOrderHistory();

                    break;


                case 8:

                    cout << "\nLogging out...\n";

                    break;


                default:

                    cout << "\nInvalid choice.\n";
            }

        }
        while (choice != 8);
    }


    /* =====================================================
       CHECKOUT
       ===================================================== */

    void checkout(Customer &customer)
    {
        if (customer.getCart().empty())
        {
            cout << "\nYour cart is empty.\n";
            return;
        }

        customer.getCart().displayCart();

        double total =
            customer.getCart().getTotal();

        int choice;

        cout << "\n========== PAYMENT ==========\n";

        cout << "1. Credit Card\n";
        cout << "2. UPI\n";
        cout << "3. Cash on Delivery\n";

        cout << "\nSelect payment method: ";
        cin >> choice;

        Payment *payment = nullptr;

        string paymentMethod;

        if (choice == 1)
        {
            string card;

            cout << "Enter card number: ";
            cin >> card;

            if (card.length() < 4)
            {
                cout << "Invalid card number.\n";
                return;
            }

            payment =
                new CreditCardPayment(
                    total,
                    card
                );

            paymentMethod = "Credit Card";
        }

        else if (choice == 2)
        {
            string upi;

            cout << "Enter UPI ID: ";
            cin >> upi;

            payment =
                new UPIPayment(
                    total,
                    upi
                );

            paymentMethod = "UPI";
        }

        else if (choice == 3)
        {
            payment =
                new CashOnDelivery(
                    total
                );

            paymentMethod = "Cash on Delivery";
        }

        else
        {
            cout << "Invalid payment method.\n";
            return;
        }


        payment->pay();

        delete payment;


        Order order(
            nextOrderId++,
            customer.getCart().getItems(),
            total,
            paymentMethod
        );

        customer.addOrder(order);

        // Update stock
        for (const auto &item :
             customer.getCart().getItems())
        {
            Product *product =
                findProduct(
                    item.getProduct().getId()
                );

            if (product != nullptr)
            {
                product->setStock(
                    product->getStock()
                    -
                    item.getQuantity()
                );
            }
        }

        customer.getCart().clear();

        cout << "\nOrder placed successfully!\n";

        cout << "Your Order ID is: "
             << order.getOrderId()
             << endl;
    }


    /* =====================================================
       ADMIN LOGIN
       ===================================================== */

    bool adminLogin()
    {
        string username;
        string password;

        cout << "\n========== ADMIN LOGIN ==========\n";

        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;

        if (admin.login(username, password))
        {
            cout << "\nAdmin login successful!\n";

            return true;
        }

        cout << "\nInvalid admin credentials.\n";

        return false;
    }


    /* =====================================================
       ADMIN MENU
       ===================================================== */

    void adminMenu()
    {
        int choice;

        do
        {
            cout << "\n====================================\n";
            cout << "          ADMIN DASHBOARD\n";
            cout << "====================================\n";

            cout << "1. View Products\n";
            cout << "2. Add Product\n";
            cout << "3. Remove Product\n";
            cout << "4. Logout\n";

            cout << "\nEnter choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:

                    displayProducts();

                    break;

                case 2:

                    addProduct();

                    break;

                case 3:

                    removeProduct();

                    break;

                case 4:

                    cout << "\nLogging out...\n";

                    break;

                default:

                    cout << "\nInvalid choice.\n";
            }

        }
        while (choice != 4);
    }


    /* =====================================================
       MAIN MENU
       ===================================================== */

    void run()
    {
        int choice;

        do
        {
            cout << "\n\n";
            cout << "==============================================\n";
            cout << "          ONLINE SHOPPING PORTAL\n";
            cout << "==============================================\n";

            cout << "1. Customer Login\n";
            cout << "2. Customer Registration\n";
            cout << "3. Admin Login\n";
            cout << "4. Exit\n";

            cout << "\nEnter choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                {
                    Customer *customer =
                        customerLogin();

                    if (customer != nullptr)
                    {
                        customerMenu(*customer);
                    }

                    break;
                }

                case 2:

                    registerCustomer();

                    break;

                case 3:

                    if (adminLogin())
                    {
                        adminMenu();
                    }

                    break;

                case 4:

                    cout << "\nThank you for using "
                         << "Online Shopping Portal!\n";

                    break;

                default:

                    cout << "\nInvalid choice.\n";
            }

        }
        while (choice != 4);
    }
};


/* =========================================================
   MAIN FUNCTION
   ========================================================= */

int main()
{
    ShoppingPortal portal;

    portal.run();

    return 0;
}
