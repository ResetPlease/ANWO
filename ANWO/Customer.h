#pragma once

#include "Batch.h"


class Customer {
 public:
  Customer() = default;

  Customer(int name, std::vector<Product>& products);

  Customer(const Customer& other);

  Customer(Customer&& other) noexcept;

  Bundle gen_order(std::map<Product, std::vector<Batch>>& bad_products);

  void get_order(Bundle order);

  int want_to_buy(int needed, int amount);

  bool operator==(Customer other);

  void use_product();

  int name();
  
 protected:
  int name_;
  std::vector<Bundle> needed_;
  std::vector<Bundle> on_hand_;
};
