#pragma once

#include "allLib.h"

class Product {
 public:
  Product() : expiration_date_(0), amount_(0), name_(-1) {}

  Product(int name)
      : expiration_date_(dis(gen) % 3 + 2),
        amount_(dis(gen) % 10 + 1),
        name_(name) {}

  Product(const Product& other)
      : expiration_date_(other.expiration_date_),
        amount_(other.amount_),
        name_(other.name_) {}

  Product(Product&& other) noexcept
      : expiration_date_(other.expiration_date_),
        amount_(other.amount_),
        name_(other.name_) {}

  void Swap(Product other) {
    std::swap(expiration_date_, other.expiration_date_);
    std::swap(amount_, other.amount_);
    std::swap(name_, other.name_);
  }

  Product& operator=(Product other) {
    Swap(other);
    return *this;
  }

  bool operator==(Product other) const {
    return expiration_date_ == other.expiration_date_ &&
           amount_ == other.amount_ && name_ == other.name_;
  }

  friend bool operator<(Product lhs, Product rhs) { return lhs.name_ < rhs.name_; }

  int name() const { return name_; }

 protected:
  int name_;
  int expiration_date_;
  int amount_;
};

