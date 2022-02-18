#include <iostream>
#include "property.h"
#include "owner.h"

using namespace std;

Property::Property()
{
}

Property::Property(const string &property_name, int area, Owner *owner)
{
    this->property_name = property_name;
    this->area = area;
    if (owner != NULL) {
        this->owner = owner;
        owner->add_property(this);
    }
    else this->owner = NULL;
}

void Property::set_owner(Owner *owner)
{
    this->owner = owner;
    if (owner != NULL) {
        owner->add_property(this);
    }
}

float Property::valuate()
{
    float value = 100;
    return value;
}

string &Property::get_name()
{
    return this->property_name;
}

void Property::print_info()
{
    if (this->owner != NULL) {
        cout << "" << this->property_name << " (" << this->area << " m2)" << " Owner: " << this->owner->get_name() << endl; 
    }
    else {
        cout << "" << this->property_name << " (" << this->area << " m2)" << " Owner: No owner" << endl; 
    }
}