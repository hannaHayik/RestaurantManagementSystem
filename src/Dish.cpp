#include "../include/Dish.h"
//________Constructor_________
Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(d_name), price(d_price), type(d_type), active(true) {}
//________Copy Constructor_________
Dish::Dish(const Dish &other) : id(other.id), name(other.name), price(other.price), type(other.type), active(other.getActive()) {}

int Dish::getId() const
{
    return this->id;
}

std::string Dish::getName() const
{
    return this->name;
}
int Dish::getPrice() const
{
    return this->price;
}

DishType Dish::getType() const
{
    return this->type;
}

std::string Dish::getTypeInString()
{
    if (this->getType() == DishType::VEG)
        return "VEG";
    if (this->getType() == DishType::SPC)
        return "SPC";
    if (this->getType() == DishType::BVG)
        return "BVG";
    return "ALC";
}

bool Dish::operator<(const Dish &other)
{
    if (price < other.getPrice())
        return true;
    return price == other.getPrice() && id < other.getId();
}

bool Dish::operator==(const Dish &other)
{
    return this->id == other.getId();
}

void Dish::setActive(bool toSet)
{
    this->active = toSet;
}

bool Dish::getActive() const
{
    return this->active;
}
