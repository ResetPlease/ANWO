#pragma once

#include "Bundle.h"

class Batch : public Bundle {
 public:
  Batch() : Bundle(), bundle_amount_(0), day_of_production_(0) {}

  Batch(const Bundle& bundle, int quantity, int day_of_production)
      : Bundle(bundle),
        bundle_amount_(quantity),
        day_of_production_(day_of_production) {}

  Batch(const Batch& other)
      : Bundle(other),
        bundle_amount_(other.quantity_),
        day_of_production_(other.day_of_production_) {}

  Batch(Batch&& other) noexcept
      : Bundle(other),
        bundle_amount_(other.quantity_),
        day_of_production_(other.day_of_production_) {}

  void Swap(Batch other) {
    Bundle::Swap(other);
    std::swap(quantity_, other.quantity_);
    std::swap(day_of_production_, other.day_of_production_);
  }

  Batch& operator=(Batch other) {
    Swap(other);
    return *this;
  }

  bool operator==(Batch other) const {
    return Bundle::operator==(other) && quantity_ == other.quantity_ &&
           day_of_production_ == other.day_of_production_;
  }

  int total_quantity() { return bundle_amount_ * quantity_; }

  int left_to_live(int cur_day) {
    return day_of_production_ + expiration_date_ - cur_day;
  }

  int bundle_amount() { return bundle_amount_; }

  void add_bundle_amount(int x) {
    bundle_amount_ += x;
    return;
  }

  friend bool operator<(Batch lhs, Batch rhs) {
    return lhs.day_of_production_ < rhs.day_of_production_;
  }

 protected:
  int bundle_amount_, day_of_production_;
};
