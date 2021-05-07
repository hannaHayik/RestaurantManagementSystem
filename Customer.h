#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer
{
public:
  Customer(const Customer &other);
  virtual ~Customer() {}
  Customer(std::string c_name, int c_id);
  virtual std::vector<int> order(const std::vector<Dish> &menu) = 0;
  virtual std::string toString() const = 0;
  std::string getName() const;
  int getId() const;
  int idCompare(const std::vector<Dish> &SameIDmenu);
  std::vector<Dish> selectionSort(std::vector<Dish> dishes);
  virtual Customer *clone();

private:
  const std::string name;
  const int id;
};

class VegetarianCustomer : public Customer
{
public:
  VegetarianCustomer(std::string name, int id);
  virtual ~VegetarianCustomer() {}
  std::vector<int> order(const std::vector<Dish> &menu);
  std::string toString() const;
  int VegDishWithMinId(const std::vector<Dish> &menu);
  int MostExpBvgNoAlco(const std::vector<Dish> &menu);
  VegetarianCustomer *clone();

private:
};

class CheapCustomer : public Customer
{
public:
  CheapCustomer(std::string name, int id);
  virtual ~CheapCustomer() {}
  std::vector<int> order(const std::vector<Dish> &menu);
  std::string toString() const;
  CheapCustomer *clone();

private:
  int count = 0;
};

class SpicyCustomer : public Customer
{
public:
  SpicyCustomer(std::string name, int id);
  SpicyCustomer(const SpicyCustomer &other);
  virtual ~SpicyCustomer() {}
  std::vector<int> order(const std::vector<Dish> &menu);
  std::string toString() const;
  std::vector<int> orderPlus(const std::vector<Dish> &menu);
  int MostExpSpicyDish(const std::vector<Dish> &menu);
  SpicyCustomer *clone();

private:
  int count = 0;
};

class AlchoholicCustomer : public Customer
{
public:
  AlchoholicCustomer(const AlchoholicCustomer &other);
  AlchoholicCustomer(std::string name, int id);
  virtual ~AlchoholicCustomer() {}
  std::vector<int> order(const std::vector<Dish> &menu);
  std::string toString() const;
  AlchoholicCustomer *clone();

private:
  int count = 0;
  int lastId;
};

#endif