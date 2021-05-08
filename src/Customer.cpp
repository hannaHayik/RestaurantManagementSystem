#include "../include/Customer.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
//________Copy Constructor_________
Customer::Customer(const Customer &other) : name(other.name), id(other.id) {}
//________Constructor_________
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

std::string Customer::getName() const
{
    return this->name;
}

int Customer::getId() const
{
    return this->id;
}

Customer *Customer::clone()
{
    return nullptr;
}

//________Constructor_________
VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu)
{
    std::vector<int> food;
    int idOfMostExpNoAlco = VegetarianCustomer::MostExpBvgNoAlco(menu); //Find the most expensive drink not including alcoholic
    int idOfVegWithMinId = VegetarianCustomer::VegDishWithMinId(menu);  //Finds the vegeterian dish with lowest id in the menu
    if (idOfMostExpNoAlco != -1 && idOfVegWithMinId != -1)
    {
        food.push_back(idOfVegWithMinId);
        food.push_back(idOfMostExpNoAlco);
    }
    return food;
}

std::string VegetarianCustomer::toString() const
{
    return "veg";
}

VegetarianCustomer *VegetarianCustomer::clone()
{
    return (new VegetarianCustomer(*this));
}

//________Constructor_________
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu)
{
    if (!count)
    {
        int min = -1;
        for (size_t i = 0; i < menu.size(); i++)
        {
            if (min == -1 || (min != -1 && menu[min].getPrice() > menu[i].getPrice()))
            {
                min = i;
            }
        }
        count++;
        return {min};
    }

    return {};
}

std::string CheapCustomer::toString() const
{
    return "chp";
}

CheapCustomer *CheapCustomer::clone()
{
    return (new CheapCustomer(*this));
}

//________Constructor_________
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id) {}
//________Copy Constructor_________
SpicyCustomer::SpicyCustomer(const SpicyCustomer &other) : Customer(other.getName(), other.getId()), count(other.count) {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu)
{
    std::vector<int> orderingList;
    int mostExpnsv = SpicyCustomer::MostExpSpicyDish(menu); //Finds the most expensive spicy dish in the menu
    if (mostExpnsv == -1 && this->count == 0)
        return orderingList;
    if ((this->count == 0) && (mostExpnsv != -1))
    {
        orderingList.push_back(mostExpnsv);
        this->count++;
        return orderingList;
    }
    return this->orderPlus(menu); //If the counter is >=1, means not his first order so we call orderPlus
}

std::vector<int> SpicyCustomer::orderPlus(const std::vector<Dish> &menu)
{
    bool flag2 = false;
    int chpstDrink;
    std::vector<int> orderingList;
    std::vector<Dish> dishesToCompare;

    for (size_t i = 0; (i < menu.size() && !flag2); i++) //Finds a first element to compare others with
        if (menu[i].getType() == DishType::BVG)
        {
            chpstDrink = i;
            flag2 = true;
        }

    for (size_t i = 0; i < menu.size(); i++) //Finds the cheapest beverage
        if (menu[i].getType() == DishType::BVG)
            if (menu[i].getPrice() < menu[chpstDrink].getPrice())
                chpstDrink = i;

    if (flag2) //If found then we compare IDs for beverages with same price
    {
        for (size_t i = 0; i < menu.size(); i++)
            if (menu[i].getPrice() == menu[chpstDrink].getPrice())
                dishesToCompare.push_back(menu[i]);
        chpstDrink = Customer::idCompare(dishesToCompare);
    }
    if (flag2)
        orderingList.push_back(chpstDrink);
    this->count++;
    return orderingList;
}

std::string SpicyCustomer::toString() const
{
    return "spc";
}

SpicyCustomer *SpicyCustomer::clone()
{
    return (new SpicyCustomer(*this));
}

//________Constructor_________
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name, id), lastId(-1) {}
//________Copy Constructor_________
AlchoholicCustomer::AlchoholicCustomer(const AlchoholicCustomer &other) : Customer(other.getName(), other.getId()), count(other.count), lastId(-1) {}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu)
{
    int chosen = -1;
    int lastAlc;
    std::vector<Dish> dishes = selectionSort(menu);
    for (size_t i = 0; i < dishes.size(); i++)
    {
        if (menu[i].getType() == DishType::ALC)
        {
            lastAlc = i;
            if (chosen == -1 || (chosen != -1 && menu[chosen].getPrice() > menu[i].getPrice()))
            {
                if ((lastId != -1 && menu[lastId].getId() < menu[i].getId()) || lastId == -1)
                    chosen = i;
            }
        }
    }

    if (lastId == lastAlc)
        return {};

    lastId = chosen;

    if (lastId == -1)
    {
    };

    return {lastId};
}

std::string AlchoholicCustomer::toString() const
{
    return "alc";
}

AlchoholicCustomer *AlchoholicCustomer::clone()
{
    return (new AlchoholicCustomer(*this));
}

//_________Helping Functions________
int Customer::idCompare(const std::vector<Dish> &SamePriceMenu) //Retruns the id of the dish with smallest id
{
    int valueToReturn = SamePriceMenu[0].getId();
    for (size_t i = 0; i < SamePriceMenu.size(); i++)
        if (SamePriceMenu[i].getId() < valueToReturn)
            valueToReturn = SamePriceMenu[i].getId();
    return valueToReturn;
}
std::vector<Dish> Customer::selectionSort(std::vector<Dish> dishes) //An upgraded SelectionSort to match our needs
{
    size_t i, j, min_idx;

    std::vector<int> indces;
    for (i = 0; i < dishes.size(); i++)
    {
        indces.push_back(i);
    }

    for (i = 0; i < dishes.size(); i++)
    {
        min_idx = i;
        for (j = i + 1; j < dishes.size(); j++)
            if (dishes[indces[j]] < dishes[indces[min_idx]])
                min_idx = j;

        int tmp = indces[min_idx];
        indces[min_idx] = indces[i];
        indces[i] = tmp;
    }
    std::vector<Dish> ans;
    for (i = 0; i < dishes.size(); i++)
    {
        Dish from = dishes[indces[i]];
        Dish to = Dish(from);
        ans.push_back(to);
    }
    return ans;
}

int VegetarianCustomer::VegDishWithMinId(const std::vector<Dish> &menu)
{
    bool flagFirstDish = false;
    int dishindex = -1;
    for (size_t i = 0; (i < menu.size() && !flagFirstDish); i++)
    {
        if (menu[i].getType() == DishType::VEG)
        {
            dishindex = i;
            flagFirstDish = true;
        }
    }
    for (size_t i = 0; i < menu.size(); i++)
    {
        if (menu[i].getType() == DishType::VEG)
        {
            if (menu[i].getId() < menu[dishindex].getId())
                dishindex = i;
        }
    }
    if (dishindex == -1)
        return -1;
    else
        return menu[dishindex].getId();
}

int VegetarianCustomer::MostExpBvgNoAlco(const std::vector<Dish> &menu)
{
    bool flagFirstBVG = false;
    int bevergeindex = -1;
    std::vector<Dish> bevegToCompare;
    for (size_t i = 0; (i < menu.size() && !flagFirstBVG); i++)
    {
        if (menu[i].getType() == DishType::BVG)
        {
            bevergeindex = i;
            flagFirstBVG = true;
        }
    }

    for (size_t i = 0; (i < menu.size() && flagFirstBVG); i++)
    {
        if (menu[i].getType() == DishType::BVG)
        {
            if (menu[i].getPrice() > menu[bevergeindex].getPrice())
                bevergeindex = i;
        }
    }

    if (flagFirstBVG)
    {
        for (size_t i = 0; i < menu.size(); i++)
            if (menu[i].getPrice() == menu[bevergeindex].getPrice() && menu[i].getType() == DishType::BVG)
                bevegToCompare.push_back(menu[i]);
        bevergeindex = Customer::idCompare(bevegToCompare);
    }

    if (bevergeindex == -1)
        return -1;
    else
        return menu[bevergeindex].getId();
}

int SpicyCustomer::MostExpSpicyDish(const std::vector<Dish> &menu)
{
    bool flag = false;
    std::vector<Dish> dishesToCompare;
    int mostExpnsv = -1;

    for (size_t i = 0; (i < menu.size() && !flag); i++)
        if (menu[i].getType() == DishType::SPC)
        {
            mostExpnsv = i;
            flag = true;
        }

    for (size_t i = 0; i < menu.size(); i++)
    {
        if (menu[i].getType() == DishType::SPC)
            if (menu[i].getPrice() > menu[mostExpnsv].getPrice())
                mostExpnsv = i;
    }

    if (flag)
    {
        for (size_t i = 0; i < menu.size(); i++)
            if (menu[i].getPrice() == menu[mostExpnsv].getPrice() && (menu[i].getType() == DishType::SPC))
                dishesToCompare.push_back(menu[i]);
        mostExpnsv = Customer::idCompare(dishesToCompare);
    }

    if (mostExpnsv == -1)
        return -1;
    return menu[mostExpnsv].getId();
}
