#pragma once

#include "Product.h"

class Bundle : public Product {
 public:
  Bundle() : Product(), quantity_(0), price_(0) {}

  Bundle(const Product& product, int quantity, int price)
      : Product(product), quantity_(quantity), price_(price) {}

  Bundle(const Bundle& other)
      : Product(other), quantity_(other.quantity_), price_(other.price_) {}

  Bundle(Bundle&& other) noexcept
      : Product(other), quantity_(other.quantity_), price_(other.price_) {}

  int quantity() const { return quantity_; }

  void Swap(Bundle other) {
    Product::Swap(other);
    std::swap(quantity_, other.quantity_);
    std::swap(price_, other.price_);
  }

  Bundle& operator=(Bundle other) {
    Swap(other);
    return *this;
  }

  bool operator==(Bundle other) const {
    return Product::operator==(other) && quantity_ == other.quantity_ &&
           price_ == other.price_;
  }

  void add_quantity(int x) {
    quantity_ += x;
    return;
  }

  int price() const { return price_; }

  void set_price(int x) {
    price_ = x;
    return;
  }

  friend bool operator<(Bundle lhs, Bundle rhs) {
    return lhs.quantity_ < rhs.quantity_;
  }

 protected:
  int quantity_, price_;
};
