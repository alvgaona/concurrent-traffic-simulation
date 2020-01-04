#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <thread>

#include "../include/Intersection.h"
#include "../include/Street.h"
#include "../include/Vehicle.h"

/* Implementation of class "WaitingVehicles" */

int WaitingVehicles::get_size() {
  std::lock_guard<std::mutex> lock(mutex_);

  return vehicles_.size();
}

void WaitingVehicles::push_back(std::shared_ptr<Vehicle> vehicle,
                               std::promise<void> &&promise) {
  std::lock_guard<std::mutex> lock(mutex_);

  vehicles_.push_back(vehicle);
  promises_.push_back(std::move(promise));
}

void WaitingVehicles::permit_entry_to_first_in_queue() {
  std::lock_guard<std::mutex> lock(mutex_);

  // get entries from the front of both queues
  auto firstPromise = promises_.begin();
  auto firstVehicle = vehicles_.begin();

  // fulfill promise and send signal back that permission to enter has been
  // granted
  firstPromise->set_value();

  // remove front elements from both queues
  vehicles_.erase(firstVehicle);
  promises_.erase(firstPromise);
}

/* Implementation of class "Intersection" */

Intersection::Intersection() {
  type_ = ObjectType::kObjectIntersection;
  is_blocked_ = false;
}

void Intersection::add_street(std::shared_ptr<Street> street) {
  streets_.push_back(street);
}

std::vector<std::shared_ptr<Street>>
Intersection::query_streets(const std::shared_ptr<Street> &incoming) {
  // store all outgoing streets in a vector ...
  std::vector<std::shared_ptr<Street>> outgoings;
  for (auto it : streets_) {
    if (incoming->get_id() !=
        it->get_id()) // ... except the street making the inquiry
    {
      outgoings.push_back(it);
    }
  }

  return outgoings;
}

// adds a new vehicle to the queue and returns once the vehicle is allowed to
// enter
void Intersection::add_vehicles_to_queue(const std::shared_ptr<Vehicle>& vehicle) {
  std::unique_lock<std::mutex> lck(mutex_);
  std::cout << "Intersection #" << id_
            << "::add_vehicles_to_queue: thread id = " << std::this_thread::get_id()
            << std::endl;
  lck.unlock();

  // add new vehicle to the end of the waiting line
  std::promise<void> prmsVehicleAllowedToEnter;
  std::future<void> ftrVehicleAllowedToEnter =
      prmsVehicleAllowedToEnter.get_future();
  waiting_vehicles_.push_back(vehicle, std::move(prmsVehicleAllowedToEnter));

  // wait until the vehicle is allowed to enter
  ftrVehicleAllowedToEnter.wait();
  lck.lock();
  std::cout << "Intersection #" << id_ << ": Vehicle #" << vehicle->get_id()
            << " is granted entry." << std::endl;

  if (traffic_light_.get_current_phase() == TrafficLightPhase::kRed) {
    traffic_light_.wait_for_green();
  }

  lck.unlock();
}

void Intersection::vehicle_has_left(const std::shared_ptr<Vehicle>& vehicle) {
  // std::cout << "Intersection #" << id_ << ": Vehicle #" << vehicle->get_id()
  // << " has left." << std::endl;

  // unblock queue processing
  this->set_is_blocked(false);
}

void Intersection::set_is_blocked(bool is_blocked) {
  is_blocked_ = is_blocked;
  // std::cout << "Intersection #" << id_ << " is_blocked=" << is_blocked <<
  // std::endl;
}

// virtual function which is executed in a thread
void Intersection::simulate() // using threads_ + promises/futures + exceptions
{
  traffic_light_.simulate();

  // launch vehicle queue processing in a thread
  threads_.emplace_back(std::thread(&Intersection::process_vechile_queue, this));
}

void Intersection::process_vechile_queue() {
  // print id of the current thread
  // std::cout << "Intersection #" << id_ << "::process_vechile_queue: thread id =
  // " << std::this_thread::get_id() << std::endl;

  // continuously process the vehicle queue
  while (true) {
    // sleep at every iteration to reduce CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // only proceed when at least one vehicle is waiting in the queue
    if (waiting_vehicles_.get_size() > 0 && !is_blocked_) {
      // set intersection to "blocked" to prevent other vehicles from entering
      this->set_is_blocked(true);

      // permit entry to first vehicle in the queue (FIFO)
      waiting_vehicles_.permit_entry_to_first_in_queue();
    }
  }
}

bool Intersection::traffic_light_is_green() { return traffic_light_.get_current_phase() == TrafficLightPhase::kGreen; }
