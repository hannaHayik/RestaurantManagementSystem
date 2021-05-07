#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"

extern Restaurant* backup;

class Restaurant
{
  public:
    Restaurant& operator=(const Restaurant &other);
    Restaurant& operator=(Restaurant &&other);//13.11
    Restaurant();
    Restaurant(const std::string &configFilePath);
    Restaurant(Restaurant const &other);
    Restaurant(Restaurant &&other);
    void start();
    int getNumOfTables() const;
    Table *getTable(int ind);
    const std::vector<BaseAction *> &getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish> &getMenu();
    std::vector<Table *> GetTables();
    DishType returnDishType(std::string str);
    void addActionToLog(BaseAction* actionToAdd);
    virtual ~Restaurant();
    std::vector<std::string> split(const std::string &str, const char &by);
    std::vector<Customer*> initCustomers(std::vector<std::string> customerString);
  private:
    bool open;
    std::vector<Table *> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction *> actionsLog;
    int cusCount=0;
};

#endif