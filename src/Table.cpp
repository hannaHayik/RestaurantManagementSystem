#include "../include/Table.h"
#include <iostream>
#include <vector>
#include <string>

//________Constructor_________
Table::Table(int t_capacity) : capacity(t_capacity), open(false), customersList(), orderList() {}

//________Copy Assignment Operator_________
Table &Table::operator=(const Table &other) 
{
    if (&other == this)
        return *this;

    this->customersList.clear();
    this->orderList.clear();
    this->capacity = other.capacity;
    for (size_t i = 0; i < other.orderList.size(); i++)
    {
        Dish toPush(other.orderList[i].second.getId(), other.orderList[i].second.getName(), other.orderList[i].second.getPrice(), other.orderList[i].second.getType());
        this->orderList.push_back(OrderPair(other.orderList[i].first, toPush));
    }
    this->customersList = other.customersList;
    this->open = other.open;
    return *this;
}

//________Move Assignment Operator_________
Table &Table::operator=(Table &&other)
{
    if (&other == this)
        return *this;

    this->customersList.clear();
    this->orderList.clear();
    this->capacity = other.capacity;
    for (size_t i = 0; i < other.orderList.size(); i++)
    {
        Dish toPush(other.orderList[i].second.getId(), other.orderList[i].second.getName(), other.orderList[i].second.getPrice(), other.orderList[i].second.getType());
        this->orderList.push_back(OrderPair(other.orderList[i].first, toPush));
    }
    this->customersList = other.customersList;
    this->open = other.open;
    other.customersList.clear();
    other.orderList.clear();
    return *this;
}
//________Copy Constructor_________
Table::Table(const Table &other) : capacity(other.getCapacity()), open(other.open), customersList(), orderList()
{
    for (size_t i = 0; i < other.orderList.size(); i++)
    {
        Dish toPush(other.orderList[i].second);
        this->orderList.push_back(OrderPair(other.orderList[i].first, toPush));
    }
    for (size_t i = 0; i < other.customersList.size(); i++)
        this->customersList.push_back(other.customersList[i]->clone());
}
//________Move Constructor_________
Table::Table(Table &&other)
{
    this->customersList.clear();
    this->orderList.clear();
    this->open = other.open;
    this->capacity = other.capacity;
    this->customersList = other.customersList;
    for (size_t i = 0; i < other.orderList.size(); i++)
    {
        Dish toPush(other.orderList[i].second);
        this->orderList.push_back(OrderPair(other.orderList[i].first, toPush));
    }
    other.customersList.clear();
    other.orderList.clear();
}

//________Destructor________
Table::~Table()
{
    for (size_t i = 0; i < this->customersList.size(); i++)
    {
        delete this->customersList[i];
    }
}
void Table::removeCustomer(int id) //Finds the customer and removes it from the customers vector
{
    int tmp = -1;
    for (size_t i = 0; i < this->customersList.size(); i++)
        if (this->customersList[i]->getId() == id)
            tmp = i;
    if (tmp != -1)
        this->customersList.erase(this->customersList.begin() + tmp);
}

void Table::addCustomer(Customer *customer)
{
    this->customersList.push_back(customer);
}

Customer *Table::getCustomer(int id)
{
    int index = this->GetCustomerIndexInList(id);
    if (index != -1)
    {
        return this->customersList[index];
    }
    return nullptr; 
}

int Table::GetCustomerIndexInList(int id)
{
    bool flag = false;
    int index = -1;
    for (size_t i = 0; (i < this->customersList.size() && !flag); i++)
    {
        if (this->customersList[i]->getId() == id)
        {
            index = i;
            flag = true;
        }
    }
    return index;
}

std::vector<Customer *> &Table::getCustomers()
{
    return this->customersList;
}

std::vector<OrderPair> &Table::getOrders()
{
    return this->orderList;
}

void Table::order(const std::vector<Dish> &menu) //Runs every customer's Order function and prints every one's order
{
    for (size_t i = 0; i < this->customersList.size(); i++)
    {
        std::vector<int> tmpVector = this->customersList[i]->order(menu);
        for (size_t j = 0; j < tmpVector.size(); j++)
            for (size_t t = 0; t < menu.size(); t++)
                if (tmpVector[j] == menu[t].getId())
                {
                    this->orderList.push_back(OrderPair(this->customersList[i]->getId(), menu[t]));
                    std::cout << this->customersList[i]->getName() + " ordered " + menu[t].getName() << std::endl;
                }
    }
}

void Table::openTable()
{
    this->open = true;
}

void Table::closeTable() //Function changes the table status and frees the memory used by the object
{
    this->open = false;
    for (size_t i = 0; i < this->customersList.size(); i++)
        delete customersList[i];
    this->customersList.clear();
    this->orderList.clear();
}
int Table::getBill()
{
    int bill = 0;
    for (size_t i = 0; i < this->orderList.size(); i++)
        if (this->orderList[i].second.getActive() != false)
            bill = bill + this->orderList[i].second.getPrice();
    return bill;
}

bool Table::isOpen()
{
    return this->open == true;
}

int Table::getCapacity() const
{
    return this->capacity;
}
