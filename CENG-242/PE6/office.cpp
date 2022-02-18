#include <iostream>
#include "office.h"
#include "owner.h"

using namespace std;

Office::Office(const string &property_name, int area, Owner *owner, bool having_wifi, bool having_reception)
{
    this->property_name = property_name;
    this->area = area;
    this->having_wifi = having_wifi;
    this->having_reception = having_reception;
    
    if (owner != NULL) {
        this->owner = owner;
        owner->add_property(this);
    }
    else this->owner = NULL;
}

float Office::valuate()
{ 
    float value;
    if (this->having_wifi && this->having_reception) {
        value = this->area * 5 * 1.3 * 1.5;
    }
    else if (this->having_wifi) {
        value = this->area * 5 * 1.3;
    }
    else if (this->having_reception) {
        value = this->area * 5 * 1.5;
    }
    else value = this->area * 5;
    
    return value;
}