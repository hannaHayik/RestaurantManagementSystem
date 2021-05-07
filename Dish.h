#ifndef DISH_H_
#define DISH_H_

#include <string>

enum DishType
{
  VEG,
  SPC,
  BVG,
  ALC
};

class Dish
{
public:
  Dish(const Dish &other);
  Dish(int d_id, std::string d_name, int d_price, DishType d_type);
  int getId() const;
  std::string getName() const;
  int getPrice() const;
  DishType getType() const;
  bool operator<(const Dish &other);
  bool operator==(const Dish &other);
  void setActive(bool toSet);
  bool getActive() const;
  std::string getTypeInString();

private:
  const int id;
  const std::string name;
  const int price;
  const DishType type;
  bool active;
};

#endif