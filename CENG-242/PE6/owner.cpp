#include <iostream>
#include <string>
#include <vector>
#include "owner.h"

using namespace std;

Owner::Owner()
{
}

Owner::Owner(const string &name, float balance)
{
    this->name = name;
    this->balance = balance;
}

void Owner::print_info()
{
}

string &Owner::get_name()
{
    return this->name;
}

void Owner::add_property(Property *property)
{
    this->properties.push_back(property);
}

void Owner::buy(Property *property, Owner *seller)
{
    float value = property->valuate();
    cout << "[BUY] Property: " << property->get_name() << " Value: " << property->valuate() << "$"
         << " " << seller->get_name() << "--->" << this->get_name() << endl;

    bool have = false;
    unsigned int i;

    for (i = 0; i < seller->properties.size(); i++)
    {
        if (seller->properties[i]->get_name() == property->get_name())
        {
            have = true;
            break;
        }
    }

    if (this->balance < property->valuate())
    {
        cout << "[ERROR] Unaffordable  property" << endl;
    }
    else if (!have)
    {
        cout << "[ERROR] Transaction  on  unowned  property" << endl;
    }
    else
    {
        this->balance -= value;
        this->add_property(property);

        seller->properties.erase(seller->properties.begin() + i);
        seller->balance += value;
    }
}

void Owner::sell(Property *property, Owner *owner)
{
    cout << "[SELL] Property: " << property->get_name() << " Value: " << property->valuate() << "$"
         << " " << this->get_name() << "--->" << owner->get_name() << endl;

    bool have = false;
    unsigned int i;
    for (i = 0; i < this->properties.size(); i++)
    {
        if (this->properties[i]->get_name() == property->get_name())
        {
            have = true;
            break;
        }
    }

    if (owner->balance < property->valuate())
    {
        cout << "[ERROR] Unaffordable  property" << endl;
    }
    else if (!have)
    {
        cout << "[ERROR] Transaction  on  unowned  property" << endl;
    }
    else
    {
        owner->balance -= property->valuate();
        owner->add_property(property);

        this->properties.erase(this->properties.begin() + i);
        this->balance += property->valuate();
    }
}

void Owner::list_properties()
{
    cout << "Properties of " << this->name << ":" << endl;
    cout << "Balance: " << this->balance << "$" << endl;

    for (unsigned int i = 0; i < this->properties.size(); i++)
    {
        cout << "" << i + 1 << ". " << this->properties[i]->get_name() << endl;
    }
}