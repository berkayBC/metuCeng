#include <iostream>
#include "apartment.h"
#include "owner.h"

using namespace std;

Apartment::Apartment(const string &property_name, int area, Owner *owner, int floor, bool having_elevator)
{
    this->property_name = property_name;
    this->area = area;
    this->floor = floor;
    this->having_elevator = having_elevator;
    
    if (owner != NULL) {
        this->owner = owner;
        owner->add_property(this);
    }
    else this->owner = NULL;
}

float Apartment::valuate()
{
    float value;
    if (this->having_elevator) {
        value = this->area * 10 * this->floor * 1.5 / 10;
    }
    else value = this->area * 10 * this->floor / 10;
    
    return value;
}