#ifndef ACTION_H_
#define ACTION_H_
#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus
{
  PENDING,
  COMPLETED,
  ERROR
};

//Forward declaration
class Restaurant;

class BaseAction
{
public:
  BaseAction &operator=(const BaseAction &other);
  BaseAction(const BaseAction &other);
  BaseAction();
  virtual ~BaseAction(){}
  ActionStatus getStatus() const;
  virtual void act(Restaurant &restaurant) = 0;
  virtual std::string toString() const = 0;
  virtual BaseAction* clone();
  void setStatus(ActionStatus sts);
protected:
  void complete();
  void error(std::string errorMsg);
  std::string getErrorMsg() const;

private:
  std::string errorMsg;
  ActionStatus status;
};

class OpenTable : public BaseAction
{
public:
  OpenTable(int id, std::vector<Customer *> &customersList);
  void act(Restaurant &restaurant);
  std::string toString() const;
  virtual ~OpenTable();
  OpenTable(const OpenTable &other);
  OpenTable(OpenTable &&other);
  OpenTable* clone();

private:
  const int tableId;
  std::vector<Customer *> customers;
  std::vector<std::string> toStringVector;
};

class Order : public BaseAction
{
public:
  Order(const Order &other);
  Order(int id);
  void act(Restaurant &restaurant);
  std::string toString() const;
  Order* clone();

private:
  const int tableId;
};

class MoveCustomer : public BaseAction
{
public:
  MoveCustomer(const MoveCustomer &other);
  MoveCustomer(int src, int dst, int customerId);
  void act(Restaurant &restaurant);
  std::string toString() const;
  MoveCustomer* clone();

private:
  const int srcTable;
  const int dstTable;
  const int id;
};

class Close : public BaseAction
{
public:
  Close(const Close &other);
  Close(int id);
  void act(Restaurant &restaurant);
  std::string toString() const;
  Close* clone();

private:
  const int tableId;
};

class CloseAll : public BaseAction
{
public:
  CloseAll();
  void act(Restaurant &restaurant);
  std::string toString() const;
  CloseAll* clone();

private:
};

class PrintMenu : public BaseAction
{
public:
  PrintMenu();
  PrintMenu(const PrintMenu &other);
  void act(Restaurant &restaurant);
  std::string toString() const;
  PrintMenu* clone();

private:
};

class PrintTableStatus : public BaseAction
{
public:
  PrintTableStatus(const PrintTableStatus &other);
  PrintTableStatus(int id);
  void act(Restaurant &restaurant);
  std::string toString() const;
  PrintTableStatus* clone();

private:
  const int tableId;
};

class PrintActionsLog : public BaseAction
{
public:
  PrintActionsLog();
  void act(Restaurant &restaurant);
  std::string toString() const;
  PrintActionsLog* clone();

private:
};

class BackupRestaurant : public BaseAction
{
public:
  BackupRestaurant(const BackupRestaurant &other);
  BackupRestaurant();
  void act(Restaurant &restaurant);
  std::string toString() const;
  BackupRestaurant* clone();

private:
};

class RestoreResturant : public BaseAction
{
public:
  RestoreResturant &operator=(const RestoreResturant &other);
  RestoreResturant(const RestoreResturant &other);
  RestoreResturant();
  void act(Restaurant &restaurant);
  std::string toString() const;
  RestoreResturant* clone();
};

#endif