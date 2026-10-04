# Online Shopping Portal using C++

## Project Description

The Online Shopping Portal is a console-based application developed using C++. The project demonstrates important Object-Oriented Programming concepts such as encapsulation, inheritance, abstraction, polymorphism, constructors, exception handling, STL, composition, and aggregation.

## Features

* Customer registration
* Customer login
* Admin login
* Product management
* Product search
* Product categories
* Shopping cart
* Add products to cart
* Remove products from cart
* Checkout
* Multiple payment methods
* Credit Card payment
* UPI payment
* Cash on Delivery
* Order generation
* Order history
* Stock management

## OOP Concepts Used

### Encapsulation

Product, Cart, Order, User, and Customer data are encapsulated using private data members and public member functions.

### Inheritance

Customer and Admin inherit from the User class.

### Abstraction

The Payment class is an abstract class containing the pure virtual function `pay()`.

### Polymorphism

CreditCardPayment, UPIPayment, and CashOnDelivery override the `pay()` function.

### Constructors

Constructors are used to initialize Product, Customer, Order, and Payment objects.

### Exception Handling

Exceptions are used to handle invalid quantities, insufficient stock, and invalid products.

### STL

The `vector` container is used to store products, customers, cart items, and orders.

## Technologies Used

* C++
* Object-Oriented Programming
* STL
* File Handling
* Exception Handling

## Default Login Credentials

### Customer

Username:
`milton`

Password:
`1234`

### Admin

Username:
`admin`

Password:
`admin123`

## How to Run

Compile the program using:

```bash
g++ OnlineShoppingPortal.cpp -o OnlineShoppingPortal
```

Run:

### Windows

```bash
OnlineShoppingPortal.exe
```

### Linux / macOS

```bash
./OnlineShoppingPortal
```

## Project Structure

```text
OnlineShoppingPortal/
│
├── OnlineShoppingPortal.cpp
└── README.md
```

## Future Enhancements

* Database integration
* GUI interface
* Online payment gateway
* Product images
* Customer address management
* Invoice generation
* File/database-based persistent storage
* Product reviews and ratings
* Discount and coupon system
