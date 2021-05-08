#include "../include/Action.h"
#include "../include/Restaurant.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

//________Constructor_________
BaseAction::BaseAction() : errorMsg(), status(ActionStatus::PENDING) {}
//________Copy Constructor_________
BaseAction::BaseAction(const BaseAction &other) : errorMsg(other.errorMsg), status(other.status) {}
//________Copy Assignment Operator_________
BaseAction &BaseAction::operator=(const BaseAction &other)
{
        if (&other == this)
                return *this;
        this->errorMsg = other.errorMsg;
        this->status = other.status;
        return (*this);
}

ActionStatus BaseAction::getStatus() const
{
        return this->status;
}

void BaseAction::complete()
{
        this->status = ActionStatus::COMPLETED;
}

std::string BaseAction::getErrorMsg() const
{
        return this->errorMsg;
}

void BaseAction::error(std::string errorMsg)
{
        this->errorMsg = errorMsg;
        this->status = ActionStatus::ERROR;
}

BaseAction *BaseAction::clone()
{
        return nullptr;
}

void BaseAction::setStatus(ActionStatus sts)
{
        this->status = sts;
}

//Functions act calls the complete() function if they succeed, else they update the error message with the right message.
//act functions also call addActionToLog function in restaurant so we can print the actions log later.
//toStrings is done according to the examples in the website, they check the ActionStatus first and returns an appropriate string.

//________Constructor_________
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id), customers(customersList) {}
//________Move Constructor_________
OpenTable::OpenTable(OpenTable &&other) : tableId(other.tableId), customers(other.customers), toStringVector(other.toStringVector)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
        other.customers.clear();
        other.toStringVector.clear();
}
//________Copy Constructor_________
OpenTable::OpenTable(const OpenTable &other) : tableId(other.tableId), customers(other.customers)
{
        this->toStringVector = other.toStringVector;
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}
//________Destructor_________
OpenTable::~OpenTable() {}

void OpenTable::act(Restaurant &restaurant)
{
        for (size_t i = 0; i < customers.size(); i++)
        {
                this->toStringVector.push_back(customers[i]->getName());
                this->toStringVector.push_back(customers[i]->toString());
        }
        if ((restaurant.getTable(tableId) == nullptr) || (restaurant.getTable(tableId)->isOpen() == true))
        {
                std::cout << "Error: Table does not exist or is already open" << std::endl;
                this->error("Error: Table does not exist or is already open");
        }
        else
        {
                restaurant.getTable(tableId)->openTable();
                for (size_t i = 0; i < this->customers.size(); i++)
                        restaurant.getTable(tableId)->addCustomer(this->customers[i]);
                this->complete();
        }
        restaurant.addActionToLog(this);
}

std::string OpenTable::toString() const
{
        std::string s = "open " + std::to_string(this->tableId) + " ";
        for (size_t i = 0; i < this->toStringVector.size() - 1; i = i + 2)
                s = s + this->toStringVector[i] + "," + this->toStringVector[i + 1] + " ";
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + "Completed";
        else
                s = s + this->getErrorMsg();
        return s;
}

OpenTable *OpenTable::clone()
{
        return new OpenTable(*this);
}

//________Constructor_________
Order::Order(int id) : tableId(id) {}
//________Copy Constructor_________
Order::Order(const Order &other) : tableId(other.tableId)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}
void Order::act(Restaurant &restaurant)
{
        if ((restaurant.getTable(tableId) == nullptr) || (restaurant.getTable(tableId)->isOpen() == false))
        {
                std::cout << "Table does not exist or is closed" << std::endl;
                this->error("Table does not exist or is closed");
        }
        else
        {
                restaurant.getTable(this->tableId)->order(restaurant.getMenu());
                this->complete();
        }
        restaurant.addActionToLog(this);
}

std::string Order::toString() const
{
        std::string s = "order " + std::to_string(this->tableId);
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + " Completed";
        else
                s = s + " " + this->getErrorMsg();
        return s;
}

Order *Order::clone()
{
        return new Order(*this);
}

//________Constructor_________
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTable(src), dstTable(dst), id(customerId) {}
//________Copy Constructor_________
MoveCustomer::MoveCustomer(const MoveCustomer &other) : srcTable(other.srcTable), dstTable(other.dstTable), id(other.id)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}
//Checks if everything as asked (source table is opened, customer exists,...) and move him to the other table with his orders
void MoveCustomer::act(Restaurant &restaurant)
{
        Table *source = restaurant.getTable(this->srcTable);
        Table *destnation = restaurant.getTable(this->dstTable);
        Customer *tmpCustomer = restaurant.getTable(this->srcTable)->getCustomer(this->id);
        std::vector<OrderPair> vecToReturn;
        if (((size_t)(destnation->getCapacity()) > destnation->getCustomers().size()) &&
            (source != nullptr) && (destnation != nullptr) && (source->isOpen()) && (destnation->isOpen()) && (tmpCustomer != nullptr))
        {

                this->complete();
                destnation->addCustomer(tmpCustomer);
                source->removeCustomer(tmpCustomer->getId());
                for (size_t i = 0; i < source->getOrders().size(); i++)
                        if (source->getOrders()[i].first == this->id)
                        {
                                destnation->getOrders().push_back(source->getOrders()[i]);
                                source->getOrders()[i].second.setActive(false);
                        }
        }
        else
                this->error("Error:Cannot move customer");
        if (source->getCustomers().size() == 0)
                source->closeTable();
        restaurant.addActionToLog(this);
}

std::string MoveCustomer::toString() const
{
        std::string s = "move " + std::to_string(this->srcTable);
        s += " " + std::to_string(this->dstTable);
        s += " " + std::to_string(this->id);
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + " Completed";
        else
                s = s + " " + this->getErrorMsg();
        return s;
}

MoveCustomer *MoveCustomer::clone()
{
        return new MoveCustomer(*this);
}

//________Constructor_________
Close::Close(int id) : tableId(id) {}
//________Copy Constructor_________
Close::Close(const Close &other) : tableId(other.tableId)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}
void Close::act(Restaurant &restaurant)
{
        if ((restaurant.getTable(this->tableId) == nullptr) || (restaurant.getTable(this->tableId)->isOpen() == false))
                this->error("Table does not exist or is not open");
        else
        {
                std::cout << "Table " << this->tableId << " was closed."
                          << " Bill " << restaurant.getTable(this->tableId)->getBill() << "NIS" << std::endl;
                restaurant.getTable(this->tableId)->closeTable();
                this->complete();
        }
        restaurant.addActionToLog(this);
}
std::string Close::toString() const
{
        std::string s = "close " + std::to_string(this->tableId);
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + " Completed";
        else
                s = s + " " + this->getErrorMsg();
        return s;
}

Close *Close::clone()
{
        return new Close(*this);
}

//________Constructor_________
PrintMenu::PrintMenu() {}
//________Copy Constructor_________
PrintMenu::PrintMenu(const PrintMenu &other)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}

void PrintMenu::act(Restaurant &restaurant)
{
        this->complete();
        for (size_t i = 0; i < restaurant.getMenu().size(); i++)
        {
                std::cout << restaurant.getMenu()[i].getName() << " " << restaurant.getMenu()[i].getTypeInString() << " " << restaurant.getMenu()[i].getPrice() << "NIS" << std::endl;
        }
        restaurant.addActionToLog(this);
}
std::string PrintMenu::toString() const
{
        std::string s = "menu ";
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + " Completed";
        else
                s = s + " " + this->getErrorMsg();
        return s;
}

PrintMenu *PrintMenu::clone()
{
        return new PrintMenu(*this);
}

//________Constructor_________
CloseAll::CloseAll() {}

void CloseAll::act(Restaurant &restaurant)
{
        this->complete();
        for (size_t i = 0; i < restaurant.GetTables().size(); i++)
        {
                if (restaurant.getTable(i)->isOpen() == true)
                {
                        std::cout << "Table " << i << " was closed."
                                  << "Bill " << restaurant.getTable(i)->getBill() << "NIS" << std::endl;
                        restaurant.getTable(i)->closeTable();
                }
        }
        restaurant.addActionToLog(this);
}
std::string CloseAll::toString() const
{
        return "restuarant closed";
}

CloseAll *CloseAll::clone()
{
        return new CloseAll(*this);
}

//________Constructor_________
PrintTableStatus::PrintTableStatus(int id) : tableId(id) {}
//________Copy Constructor_________
PrintTableStatus::PrintTableStatus(const PrintTableStatus &other) : tableId(other.tableId)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}
void PrintTableStatus::act(Restaurant &restaurant)
{
        if (restaurant.getTable(tableId)->isOpen())
        {
                std::cout << "Table " << tableId << " status: open" << std::endl;
                std::cout << "Customers:" << std::endl;
                for (size_t i = 0; i < restaurant.getTable(tableId)->getCustomers().size(); i++)
                {
                        std::cout << restaurant.getTable(tableId)->getCustomers()[i]->getId() << " " << restaurant.getTable(tableId)->getCustomers()[i]->getName() << std::endl;
                }
                std::cout << "Orders:" << std::endl;
                for (size_t i = 0; i < restaurant.getTable(tableId)->getOrders().size(); i++)
                {
                        if (restaurant.getTable(tableId)->getOrders()[i].second.getActive() != false)
                                std::cout << restaurant.getTable(tableId)->getOrders()[i].second.getName() << " " << restaurant.getTable(tableId)->getOrders()[i].second.getPrice() << "NIS " << restaurant.getTable(tableId)->getOrders()[i].first << std::endl;
                }
                std::cout << "Current Bill: " << restaurant.getTable(tableId)->getBill() << "NIS" << std::endl;
        }
        else
                std::cout << "Table " << tableId << " status: closed" << std::endl;
        this->complete();
        restaurant.addActionToLog(this);
}

std::string PrintTableStatus::toString() const
{
        std::string s = "status " + std::to_string(this->tableId);
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + " Completed";
        else
                s = s + " " + this->getErrorMsg();
        return s;
}

PrintTableStatus *PrintTableStatus::clone()
{
        return new PrintTableStatus(*this);
}

//________Constructor_________
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant) //Prints every action's toString to the screen
{
        for (size_t i = 0; i < restaurant.getActionsLog().size(); i++)
                if (restaurant.getActionsLog()[i]->toString().compare("log") != 0)
                        std::cout << restaurant.getActionsLog()[i]->toString() << std::endl;
        restaurant.addActionToLog(this);
}

std::string PrintActionsLog::toString() const
{
        return "log";
}

PrintActionsLog *PrintActionsLog::clone()
{
        return new PrintActionsLog(*this);
}

//________Constructor_________
BackupRestaurant::BackupRestaurant() {}
//________Copy Constructor_________
BackupRestaurant::BackupRestaurant(const BackupRestaurant &other)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}

void BackupRestaurant::act(Restaurant &restaurant)
{
        if (backup != nullptr)
        {
                *backup = restaurant;
        }
        else
        {
                backup = new Restaurant(restaurant);
        }
        this->complete();
        restaurant.addActionToLog(this);
}

std::string BackupRestaurant::toString() const
{
        std::string s = "backup ";
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + " Completed";
        else
                s = s + " " + this->getErrorMsg();
        return s;
}

BackupRestaurant *BackupRestaurant::clone()
{
        return new BackupRestaurant(*this);
}

//________Constructor_________
RestoreResturant::RestoreResturant() {}
//________Copy Constructor_________
RestoreResturant::RestoreResturant(const RestoreResturant &other)
{
        if (other.getStatus() == ActionStatus::COMPLETED)
                this->setStatus(ActionStatus::COMPLETED);
        else if (other.getStatus() == ActionStatus::ERROR)
                this->error(other.getErrorMsg());
        else
                this->setStatus(ActionStatus::PENDING);
}

void RestoreResturant::act(Restaurant &restaurant)
{
        if (backup == nullptr)
        {
                this->error("No backup available");
                std::cout << "No backup available" << std::endl;
        }
        else
        {
                restaurant = *backup;
                this->complete();
        }
        restaurant.addActionToLog(this);
}

std::string RestoreResturant::toString() const
{
        std::string s = "restore ";
        if (this->getStatus() == ActionStatus::COMPLETED)
                s = s + "Completed";
        else
                s = s + " " + this->getErrorMsg();
        return s;
}

RestoreResturant *RestoreResturant::clone()
{
        return new RestoreResturant(*this);
}
