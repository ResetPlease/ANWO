#include "Info.h"
Info info;

void Info::set_params(int days_num, int customer_num, int product_num,
                      int added_percent, int reduced_percent) {
  days_num_ = days_num;
  customer_num_ = customer_num;
  products_num_ = product_num;
  added_percent_ = added_percent;
  reduced_percent_ = reduced_percent;

  day_ = 0;
  all_products_.clear();
  prices_.clear();
  all_customers_.clear();
  total_income_ = 0;
  total_lost_ = 0;
  out = "";

  std::vector<int> choice;
  for (int i = 0; i < products_num_; ++i) {
    choice.push_back(i);
  }
  for (int i = 0; i < gen() % 10000; ++i) {
    std::swap(choice[gen() % products_num_], choice[gen() % products_num_]);
  }
  for (int i = 0; i < products_num_; ++i) {
    all_products_.push_back(Product(choice[i]));
    prices_[all_products_[i]] = dis(gen) % 100;
  }
  for (int i = 0; i < customer_num_; ++i) {
    all_customers_.push_back(new Customer(i, all_products_));
  }
  return;
}

Info::Info(const Info& other)
    : days_num_(other.days_num_),
      customer_num_(other.customer_num_),
      products_num_(other.products_num_),
      added_percent_(other.added_percent_),
      reduced_percent_(other.reduced_percent_),
      day_(other.day_),
      all_products_(other.all_products_),
      prices_(other.prices_),
      all_customers_(other.all_customers_),
      total_income_(other.total_income_),
      total_lost_(other.total_lost_) {}

Info::Info(Info&& other)
    : days_num_(other.days_num_),
      customer_num_(other.customer_num_),
      products_num_(other.products_num_),
      added_percent_(other.added_percent_),
      reduced_percent_(other.reduced_percent_),
      day_(other.day_),
      all_products_(std::exchange(other.all_products_, std::vector<Product>())),
      prices_(std::exchange(other.prices_, std::map<Product, int>())),
      all_customers_(
          std::exchange(other.all_customers_, std::vector<Customer*>())),
      total_income_(other.total_income_),
      total_lost_(other.total_lost_) {}

Info::Info(int days_num, int customer_num, int product_num, int added_percent,
           int reduced_percent)
    : days_num_(days_num),
      customer_num_(customer_num),
      products_num_(products_num_),
      added_percent_(added_percent),
      reduced_percent_(reduced_percent),
      day_(0),
      total_income_(0),
      total_lost_(0) {
  std::vector<int> choice;
  for (int i = 0; i < products_num_; ++i) {
    choice.push_back(i);
  }
  for (int i = 0; i < gen() % 10000; ++i) {
    std::swap(choice[gen() % products_num_], choice[gen() % products_num_]);
  }
  for (int i = 0; i < products_num_; ++i) {
    all_products_.push_back(Product(choice[i]));
    prices_[all_products_[i]] = dis(gen) % 100;
  }
  for (int i = 0; i < customer_num_; ++i) {
    all_customers_.push_back(new Customer(i, all_products_));
  }
}

int Info::day() { return day_; }

int Info::update_day() {
  day_++;
  for (auto& customer : all_customers_) {
    customer->use_product();
  }
  return day_;
}

int Info::days_number() { return days_num_; }

std::vector<Customer*>& Info::customers() { return all_customers_; }

void Info::add_income(int x) {
  total_income_ += x;
  return;
}

int Info::get_price(Product prod) { return prices_[prod]; }

void Info::add_lost(int x) {
  total_lost_ += x;
  return;
}

void Info::out_stats() {
  out += "Day " + std::to_string(day_) + "\n";
  out += "Income " + std::to_string(total_income_) + "\n";
  out += "Lost " + std::to_string(total_lost_) + "\n";

  // fout << std::endl;
  // fout << "Day: " << day_ << std::endl;
  // fout << "Income: " << total_income_ << std::endl;
  // fout << "Lost: " << total_lost_ << std::endl;
  // fout << std::endl;
}

int Info::added_percent() { return added_percent_; }
int Info::reduced_percent() { return reduced_percent_; }
void Info::stats_send_order(int name_customer, int name_product, int quantity) {
  out += "Send " + std::to_string(name_customer) + " " +
         std::to_string(name_product) + " " + std::to_string(quantity) + "\n";
  // fout << "Send " << name << std::endl;
}
void Info::stats_buy_product(int name, int quantity) {
  out += "Buy " + std::to_string(name) + " " + std::to_string(quantity) + "\n";
  // fout << "Buy " << name << std::endl;
}
void Info::stats_get_product(int name, int quantity) {
  out += "Get " + std::to_string(name) + " " + std::to_string(quantity) + "\n";
  // fout << "Get " << name << std::endl;
}
void Info::stats_get_order(int name) {
  out +=
      "Order " + std::to_string(name) + "\n";
  // fout << "Order " << name << std::endl;
}
void Info::stats_warehouse_amounts(
    std::map<Product, std::vector<Batch>>& good_products,
    std::map<Product, std::vector<Batch>>& bad_products) {
  for (auto& prod : good_products) {
    int amount = 0;
    for (auto& i : prod.second) amount += i.total_quantity();
    out += "Good " + std::to_string(prod.first.name()) + " " +
           std::to_string(amount) + "\n";
    // fout << "Good " << prod.first.name() << ' ' << amount << std::endl;
  }
  for (auto& prod : bad_products) {
    int amount = 0;
    for (auto& i : prod.second) amount += i.total_quantity();
    out += "Bad " + std::to_string(prod.first.name()) + " " +
           std::to_string(amount) + "\n";
    // fout << "Bad " << prod.first.name() << ' ' << amount << std::endl;
  }
}
void Info::stats_trash(int name, int quantity) {
  out +=
      "Trash " + std::to_string(name) + " " + std::to_string(quantity) + "\n";
  // fout << "Trahs " << name << ' ' << quantity << std::endl;
}
void Info::stats_bad(int name, int quantity) {
  out +=
      "ToBad " + std::to_string(name) + " " + std::to_string(quantity) + "\n";
  // fout << "ToBad " << name << ' ' << quantity << std::endl;
}