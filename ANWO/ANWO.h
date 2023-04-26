#pragma once

#include "Order.h"

class ANWO {
 public:
  ANWO() {}

  void get_orders();

  void send_orders();

  void buy_products();

  void get_products();

  void reduce_prices();

 private:
  std::map<Product, std::vector<Batch>> good_products, bad_products;
  std::map<Product, int> to_buy, already_buied_;
  std::vector<Order> orders_;
  std::vector<Transport> transports_;
  std::vector<Order> delay_;
};

std::string work(int days_num, int customer_num, int product_num, int added_percent,
          int reduced_percent);