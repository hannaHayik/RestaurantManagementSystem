#include "../include/Restaurant.h"
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>

using namespace std;

//________Copy Assignment Operator_________
Restaurant &Restaurant::operator=(const Restaurant &other)
{
    if (&other == this)
        return *this;
    this->cusCount = other.cusCount;
    this->menu.clear();
    for (size_t i = 0; i < this->tables.size(); i++)
        delete this->tables[i];
    for (size_t i = 0; i < this->actionsLog.size(); i++)
        delete this->actionsLog[i];
    this->tables.clear();
    this->actionsLog.clear();
    for (size_t i = 0; i < other.menu.size(); i++)
    {
        Dish toPush(other.menu[i].getId(), other.menu[i].getName(), other.menu[i].getPrice(), other.menu[i].getType());
        this->menu.push_back(toPush);
    }
    for (size_t i = 0; i < other.tables.size(); i++)
    {
        Table *tb = new Table(*other.tables[i]);
        this->tables.push_back(tb);
    }
    for (size_t i = 0; i < other.actionsLog.size(); i++)
        this->actionsLog.push_back(other.actionsLog[i]->clone());
    return (*this);
}

//________Move Assignment Operator_________
Restaurant &Restaurant::operator=(Restaurant &&other)
{
    if (&other == this)
        return *this;
    this->cusCount = other.cusCount;
    this->menu.clear();
    for (size_t i = 0; i < this->tables.size(); i++)
        delete this->tables[i];
    for (size_t i = 0; i < this->actionsLog.size(); i++)
        delete this->actionsLog[i];
    this->tables.clear();
    this->actionsLog.clear();
    for (size_t i = 0; i < other.menu.size(); i++)
    {
        Dish toPush(other.menu[i].getId(), other.menu[i].getName(), other.menu[i].getPrice(), other.menu[i].getType());
        this->menu.push_back(toPush);
    }
    this->actionsLog = other.actionsLog;
    this->tables = other.tables;
    other.menu.clear();
    other.actionsLog.clear();
    other.tables.clear();
    return (*this);
}

//________Empty Constructor_________
Restaurant::Restaurant() {}

//________Constructor_________
Restaurant::Restaurant(const std::string &configFilePath)
{
    int state = -1;
    int id = 0;
    // int number_of_tables = -1; No Need
    vector<string> splits;
    std::ifstream fd(configFilePath);
    if (fd.is_open())
    {
        std::string line;
        while (getline(fd, line))
        {
            line = line.substr(0, line.size() - 1);
            if (line.empty())
            {
                continue;
            }
            else if (line.compare("#number of tables") == 0)
            {
                state = 1;
            }
            else if (line.compare("#tables description") == 0)
            {
                state = 2;
            }
            else if (line.compare("#Menu") == 0)
            {
                state = 3;
            }
            else if (line[0] == '#')
            {
                continue;
            }
            else
            {
                switch (state)
                {
                case 1:
                    // number_of_tables = std::stoi(line);  No Need
                    break;
                case 2:
                    splits = split(line, ',');
                    for (size_t i = 0; i < splits.size(); i++)
                    {
                        tables.push_back(new Table(std::stoi(splits[i])));
                    }
                    break;
                case 3:
                    splits = split(line, ',');
                    menu.push_back(Dish(id++, splits[0], std::stoi(splits[2]), returnDishType(splits[1])));
                    break;
                default:
                    break;
                }
            }
        }
        fd.close();
    }
}

//________Move Constructor_________
Restaurant::Restaurant(Restaurant &&other)
{
    this->actionsLog = other.actionsLog;
    this->open = other.open;
    this->tables = other.tables;
    for (size_t i = 0; i < other.menu.size(); i++)
    {
        Dish toPush(other.menu[i].getId(), other.menu[i].getName(), other.menu[i].getPrice(), other.menu[i].getType());
        this->menu.push_back(toPush);
    }
    other.menu.clear();
    other.actionsLog.clear();
    other.tables.clear();
}
//________Copy Constructor_________
Restaurant::Restaurant(Restaurant const &other)
{
    for (size_t i = 0; i < other.tables.size(); i++)
        this->tables.push_back(new Table(*(other.tables[i])));
    this->cusCount = other.cusCount;
    for (size_t i = 0; i < other.menu.size(); i++)
    {
        Dish toPush(other.menu[i].getId(), other.menu[i].getName(), other.menu[i].getPrice(), other.menu[i].getType());
        this->menu.push_back(toPush);
    }
    for (size_t i = 0; i < other.actionsLog.size(); i++)
        this->actionsLog.push_back(other.actionsLog[i]->clone());
}

//________Destructor_________
Restaurant::~Restaurant()
{
    for (size_t i = 0; i < tables.size(); i++)
        delete tables[i];

    for (size_t i = 0; i < this->actionsLog.size(); i++)
        delete actionsLog[i];
}

void Restaurant::start()
{
    this->cusCount = 0;
    std::cout << "Restaurant is now open!" << std::endl;
    string line;
    std::vector<string> tmp;
    bool boost = true;
    do
    {
        std::getline(std::cin, line);
        tmp = this->split(line, ' ');
        if (tmp[0] == "open")
        {
            std::string custoToParse;
            for (size_t i = 2; i < tmp.size(); i++)
                custoToParse += tmp[i];
            int tableCapa = stoi(tmp[1]);
            line = line.substr(7);
            tmp = this->split(line, ' ');
            line.clear();
            std::vector<std::string> tmp2;
            std::vector<std::string> tmp3;
            for (size_t i = 0; i < tmp.size(); i++)
            {
                tmp3 = this->split(tmp[i], ',');
                tmp2.push_back(tmp3[0]);
                tmp2.push_back(tmp3[1]);
            }
            std::vector<Customer *> customers = this->initCustomers(tmp2);
            OpenTable *newActivity = new OpenTable(tableCapa, customers);
            newActivity->act(*this);
            if (newActivity->getStatus() == ActionStatus::ERROR)
            {                                                 //Deleting Customers if table can't be opened, but log prints customer[i].getName
                for (size_t i = 0; i < customers.size(); i++) // while it's deleted causing segemenation fault
                    delete customers[i];
            }
            line.clear();
            tmp.clear();
        }
        else if (tmp[0] == "order")
        {
            Order *s1 = new Order(stoi(tmp[1]));
            s1->act(*this);
        }
        else if (tmp[0] == "status")
        {
            PrintTableStatus *s1 = new PrintTableStatus(stoi(tmp[1]));
            s1->act(*this);
        }
        else if (tmp[0] == "move")
        {
            MoveCustomer *s1 = new MoveCustomer(stoi(tmp[1]), stoi(tmp[2]), stoi(tmp[3]));
            s1->act(*this);
        }
        else if (tmp[0] == "log")
        {
            PrintActionsLog *s1 = new PrintActionsLog();
            s1->act(*this);
        }
        else if (tmp[0] == "backup")
        {
            BackupRestaurant *s1 = new BackupRestaurant();
            s1->act(*this);
        }
        else if (tmp[0] == "restore")
        {
            RestoreResturant *s1 = new RestoreResturant();
            s1->act(*this);
        }
        else if (tmp[0] == "closeall")
        {
            CloseAll *s1 = new CloseAll();
            s1->act(*this);
            boost = false;
        }
        else if (tmp[0] == "close")
        {
            Close *s1 = new Close(stoi(tmp[1]));
            s1->act(*this);
        }
        else if (tmp[0] == "menu")
        {
            PrintMenu *s1 = new PrintMenu();
            s1->act(*this);
        }
    } while (boost);
}
std::vector<Customer *> Restaurant::initCustomers(std::vector<std::string> customerString)
{
    std::vector<Customer *> toReturn;
    for (size_t i = 0; i < customerString.size(); i = i + 2)
    {
        if (customerString[i + 1] == "veg")
            toReturn.push_back(new VegetarianCustomer(customerString[i], this->cusCount));
        if (customerString[i + 1] == "spc")
            toReturn.push_back(new SpicyCustomer(customerString[i], this->cusCount));
        if (customerString[i + 1] == "chp")
            toReturn.push_back(new CheapCustomer(customerString[i], this->cusCount));
        if (customerString[i + 1] == "alc")
            toReturn.push_back(new AlchoholicCustomer(customerString[i], this->cusCount));
        this->cusCount++;
    }
    return toReturn;
}

std::vector<string> Restaurant::split(const std::string &str, const char &by)
{
    vector<string> ret;
    string tmp;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == by)
        {
            ret.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp.push_back(str[i]);
        }
    }
    ret.push_back(tmp);
    return ret;
}

DishType Restaurant::returnDishType(std::string str)
{
    if (str == "VEG")
        return DishType::VEG;
    if (str == "SPC")
        return DishType::SPC;
    if (str == "BVG")
        return DishType::BVG;
    return DishType::ALC;
}

std::vector<Table *> Restaurant::GetTables()
{
    return this->tables;
}

int Restaurant::getNumOfTables() const
{
    return tables.size();
}

Table *Restaurant::getTable(int ind)
{
    return tables[ind];
}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const
{
    return actionsLog;
}

std::vector<Dish> &Restaurant::getMenu()
{
    return menu;
}

void Restaurant::addActionToLog(BaseAction *actionToAdd)
{
    this->actionsLog.push_back(actionToAdd);
}
