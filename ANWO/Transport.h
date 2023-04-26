#pragma once

#include "Customer.h"

class Transport {
 public:
  Transport() = default;

  Transport(int days, Batch batch) : days_(days), batch_(batch) {}

  Transport(const Transport& other)
      : days_(other.days_), batch_(other.batch_) {}

  /*Transport(Transport&& other) noexcept
      : days_(std::exchange(other.days_, 0)),
        batch_(std::exchange(other.batch_, Batch())) {}
        */
  Batch get_batch() { return batch_; }

  int get_days() { return days_; }
  void reduce_days(int x) {
    days_ -= x;
    return;
  }

  bool operator<(Transport other) const { return days_ < other.days_; }
  bool operator==(Transport other) const {
    return batch_ == other.batch_ && days_ == other.days_;
  }

 protected:
  int days_;
  Batch batch_;
};
