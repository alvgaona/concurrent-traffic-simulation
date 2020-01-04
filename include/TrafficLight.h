#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "TrafficObject.h"
#include <condition_variable>
#include <deque>
#include <mutex>

// forward declarations to avoid include cycle
class Vehicle;

template <class T> class MessageQueue {
public:
  void send(T &&phase);
  T receive();

private:
  std::deque<T> queue_;
  std::condition_variable condition_;
  std::mutex mutex_;
};

enum class TrafficLightPhase {
  kRed = 0,
  kGreen,
};

class TrafficLight : public TrafficObject {
public:
  // constructor / destructor
  TrafficLight();
  ~TrafficLight();
  TrafficLight(TrafficLight const &source) = delete;
  TrafficLight(TrafficLight &&source) noexcept = delete;

  // getters / setters
  void set_current_phase(const TrafficLightPhase phase);

  // operators overload
  TrafficLight &operator=(TrafficLight const &source) = delete;
  TrafficLight &operator=(TrafficLight &&source) noexcept = delete;

  void simulate();
  void wait_for_green();
  TrafficLightPhase get_current_phase();

private:
  void cycle_through_phases();

  std::mutex mutex_;
  TrafficLightPhase current_phase_;
  std::shared_ptr<MessageQueue<TrafficLightPhase >> msg_queue_;
};

#endif
