#ifndef STREET_H
#define STREET_H

#include "TrafficObject.h"

// forward declaration to avoid include cycle
class Intersection;

class Street : public TrafficObject, public std::enable_shared_from_this<Street>
{
public:
    // constructor / destructor
    Street();

    // getters / setters
    double get_length() { return length_; }
    void set_in_intersection(std::shared_ptr<Intersection> in);
    void set_out_intersection(std::shared_ptr<Intersection> out);
    std::shared_ptr<Intersection> get_out_intersection() { return inter_out_; }
    std::shared_ptr<Intersection> get_in_intersection() { return inter_in_; }

    // typical behaviour methods

    // miscellaneous
    std::shared_ptr<Street> get_shared_this() { return shared_from_this(); }

private:
    double length_;                                      // length of this street in m
    std::shared_ptr<Intersection> inter_in_, inter_out_; // intersections from which a vehicle can enter (one-way streets is always from 'in' to 'out')
};

#endif
