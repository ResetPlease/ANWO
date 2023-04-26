#pragma once

#include "Merchant.h"

class Order {
 public:
  Order() : bundle_(Bundle()), customer_(NULL) {}

  Order(const Order& other)
      : bundle_(other.bundle_), customer_(other.customer_) {}

  Order(Order&& other)
      : bundle_(std::exchange(other.bundle_, Bundle())),
        customer_(std::exchange(other.customer_, nullptr)) {}

  Order(Bundle& bundle, Customer* customer)
      : bundle_(bundle), customer_(customer) {}

  friend bool operator<(Order lhs, Order rhs) {
    if (lhs.bundle_.name() == rhs.bundle_.name()) {
      return lhs.bundle_.quantity() < rhs.bundle_.quantity();
    }
    return lhs.bundle_.name() < rhs.bundle_.name();
  }

  bool operator==(Order& other) {
    return bundle_ == other.bundle_ && *customer_ == *other.customer_;
  }

  void Swap(Order other) {
    bundle_.Swap(other.bundle_);
    std::swap(customer_, other.customer_);
  }

  Order& operator=(Order other) {
    Swap(other);
    return *this;
  }

  Bundle& bundle() { return bundle_; }

  Customer* customer() { return customer_; }

 protected:
  Bundle bundle_;
  Customer* customer_;
};
