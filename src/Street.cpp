#include <iostream>
#include "../include/Vehicle.h"
#include "../include/Intersection.h"
#include "../include/Street.h"


Street::Street()
{
    type_ = ObjectType::kObjectStreet;
    length_ = 1000.0; // in m
}

void Street::set_in_intersection(std::shared_ptr<Intersection> in)
{
    inter_in_ = in;
    in->add_street(get_shared_this()); // add this street to list of streets connected to the intersection
}

void Street::set_out_intersection(std::shared_ptr<Intersection> out)
{
    inter_out_ = out;
    out->add_street(get_shared_this()); // add this street to list of streets connected to the intersection
}
