#pragma once
#include "Transport.h"

class Info {
 public:
  Info() = default;
  Info(int days_num, int customer_num, int product_num, int added_percent,
       int reduced_percent);
  Info(const Info& other);
  Info(Info&& other);
  Info operator=(const Info& other);
  
  void set_params(int days_num, int customer_num, int product_num,
                  int added_percent, int reduced_percent);

  int day();
  int update_day();
  int days_number();
  std::vector<Customer*>& customers();
  void add_income(int x);
  void add_lost(int x);
  void out_stats();
  int get_price(Product prod);
  int added_percent();
  int reduced_percent();
  void stats_buy_product(int name, int quantity);
  void stats_get_product(int name, int quantity);
  void stats_send_order(int name_customer, int name_product, int quantity);
  void stats_get_order(int name);
  void stats_warehouse_amounts(
      std::map<Product, std::vector<Batch>>& good_products,
      std::map<Product, std::vector<Batch>>& bad_products);
  void stats_trash(int name, int quantity);
  void stats_bad(int name, int quantity);
 protected:
  int days_num_, customer_num_, products_num_, added_percent_, reduced_percent_;
  int day_;
  std::vector<Product> all_products_;
  std::map<Product, int> prices_;
  std::vector<Customer*> all_customers_;
  int total_income_;
  int total_lost_;
};

extern Info info;
