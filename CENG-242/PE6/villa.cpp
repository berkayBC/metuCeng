#include <iostream>
#include "villa.h"
#include "owner.h"

using namespace std;

Villa::Villa(const string &property_name, int area, Owner *owner, int number_of_floors, bool having_garden)
{
    this->property_name = property_name;
    this->area = area;
    this->number_of_floors = number_of_floors;
    this->having_garden = having_garden;
    
    if (owner != NULL) {
        this->owner = owner;
        owner->add_property(this);
    }
    else this->owner = NULL;
}

float Villa::valuate()
{
    float value;
    if (this->having_garden) {
        value = this->area * this->number_of_floors * 20;
    }
    else value = this->area * this->number_of_floors * 10;
    
    return value;
}



