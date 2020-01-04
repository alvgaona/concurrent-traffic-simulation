#include <iostream>
#include <thread>
#include <vector>
#include <memory>

#include "../include/Graphics.h"
#include "../include/Intersection.h"
#include "../include/Street.h"
#include "../include/Vehicle.h"

// Paris
void createTrafficObjects_Paris(
    std::vector<std::shared_ptr<Street>> &streets,
    std::vector<std::shared_ptr<Intersection>> &intersections,
    std::vector<std::shared_ptr<Vehicle>> &vehicles,
    std::string &filename,
    int nVehicles)
{
    // assign filename of corresponding city map
    filename = "../data/paris.jpg";

    // init traffic objects
    int nIntersections = 9;
    for (auto ni = 0; ni < nIntersections; ni++)
    {
        intersections.push_back(std::make_shared<Intersection>());
    }

    // position intersections in pixel coordinates (counter-clockwise)
    intersections.at(0)->set_position(385, 270);
    intersections.at(1)->set_position(1240, 80);
    intersections.at(2)->set_position(1625, 75);
    intersections.at(3)->set_position(2110, 75);
    intersections.at(4)->set_position(2840, 175);
    intersections.at(5)->set_position(3070, 680);
    intersections.at(6)->set_position(2800, 1400);
    intersections.at(7)->set_position(400, 1100);
    intersections.at(8)->set_position(1700, 900); // central plaza

    // create streets and connect traffic objects
    int nStreets = 8;
    for (auto ns = 0; ns < nStreets; ns++)
    {
        streets.push_back(std::make_shared<Street>());
        streets.at(ns)->set_in_intersection(intersections.at(ns));
        streets.at(ns)->set_out_intersection(intersections.at(8));
    }

    // add vehicles to streets
    for (auto nv = 0; nv < nVehicles; nv++)
    {
        vehicles.push_back(std::make_shared<Vehicle>());
        vehicles.at(nv)->set_current_street(streets.at(nv));
        vehicles.at(nv)->set_current_destination(intersections.at(8));
    }
}

// NYC
void createTrafficObjects_NYC(std::vector<std::shared_ptr<Street>> &streets, std::vector<std::shared_ptr<Intersection>> &intersections, std::vector<std::shared_ptr<Vehicle>> &vehicles, std::string &filename, int nVehicles)
{
    // assign filename of corresponding city map
    filename = "../data/nyc.jpg";

    // init traffic objects
    int nIntersections = 6;
    for (auto ni = 0; ni < nIntersections; ni++)
    {
        intersections.push_back(std::make_shared<Intersection>());
    }

    // position intersections in pixel coordinates
    intersections.at(0)->set_position(1430, 625);
    intersections.at(1)->set_position(2575, 1260);
    intersections.at(2)->set_position(2200, 1950);
    intersections.at(3)->set_position(1000, 1350);
    intersections.at(4)->set_position(400, 1000);
    intersections.at(5)->set_position(750, 250);

    // create streets and connect traffic objects
    int nStreets = 7;
    for (auto ns = 0; ns < nStreets; ns++)
    {
        streets.push_back(std::make_shared<Street>());
    }

    streets.at(0)->set_in_intersection(intersections.at(0));
    streets.at(0)->set_out_intersection(intersections.at(1));

    streets.at(1)->set_in_intersection(intersections.at(1));
    streets.at(1)->set_out_intersection(intersections.at(2));

    streets.at(2)->set_in_intersection(intersections.at(2));
    streets.at(2)->set_out_intersection(intersections.at(3));

    streets.at(3)->set_in_intersection(intersections.at(3));
    streets.at(3)->set_out_intersection(intersections.at(4));

    streets.at(4)->set_in_intersection(intersections.at(4));
    streets.at(4)->set_out_intersection(intersections.at(5));

    streets.at(5)->set_in_intersection(intersections.at(5));
    streets.at(5)->set_out_intersection(intersections.at(0));

    streets.at(6)->set_in_intersection(intersections.at(0));
    streets.at(6)->set_out_intersection(intersections.at(3));

    // add vehicles to streets
    for (auto nv = 0; nv < nVehicles; nv++)
    {
        vehicles.push_back(std::make_shared<Vehicle>());
        vehicles.at(nv)->set_current_street(streets.at(nv));
        vehicles.at(nv)->set_current_destination(intersections.at(nv));
    }
}

int main()
{
    /* PART 1 : Set up traffic objects */

    // create and connect intersections and streets
    std::vector<std::shared_ptr<Street>> streets;
    std::vector<std::shared_ptr<Intersection>> intersections;
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::string backgroundImg;
    int nVehicles = 6;
    createTrafficObjects_Paris(streets, intersections, vehicles, backgroundImg, nVehicles);

    /* PART 2 : simulate traffic objects */

    // simulate intersection
    std::for_each(intersections.begin(), intersections.end(), [](std::shared_ptr<Intersection> &i) {
        i->simulate();
    });

    // simulate vehicles
    std::for_each(vehicles.begin(), vehicles.end(), [](std::shared_ptr<Vehicle> &v) {
        v->simulate();
    });

    /* PART 3 : Launch visualization */

    // add all objects into common vector
    std::vector<std::shared_ptr<TrafficObject>> trafficObjects;
    std::for_each(intersections.begin(), intersections.end(), [&trafficObjects](std::shared_ptr<Intersection> &intersection) {
        std::shared_ptr<TrafficObject> trafficObject = std::dynamic_pointer_cast<TrafficObject>(intersection);
        trafficObjects.push_back(trafficObject);
    });

    std::for_each(vehicles.begin(), vehicles.end(), [&trafficObjects](std::shared_ptr<Vehicle> &vehicles) {
        std::shared_ptr<TrafficObject> trafficObject = std::dynamic_pointer_cast<TrafficObject>(vehicles);
        trafficObjects.push_back(trafficObject);
    });

    // draw all objects in vector
    auto graphics = new Graphics();
    graphics->set_bg_file_name(backgroundImg);
    graphics->set_traffic_objects(trafficObjects);
    graphics->simulate();
}
