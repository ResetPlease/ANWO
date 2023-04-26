#include "Customer.h"

Customer::Customer(int name, std::vector<Product>& products)
    : needed_({}), on_hand_({}), name_(name) {
  for (int i = 0; i < products.size(); ++i) {
    needed_.push_back(Bundle(products[i], dis(gen) % 10, 0));
    on_hand_.push_back(Bundle(products[i], 0, 0));
  }
}

Customer::Customer(const Customer& other)
    : needed_(other.needed_), on_hand_(other.on_hand_) {}

Customer::Customer(Customer&& other) noexcept
    : needed_(std::exchange(other.needed_, std::vector<Bundle>())),
      on_hand_(std::exchange(other.on_hand_, std::vector<Bundle>())) {}

Bundle Customer::gen_order(
    std::map<Product, std::vector<Batch>>& bad_products) {
  std::sort(needed_.begin(), needed_.end());
  std::sort(on_hand_.begin(), on_hand_.end());
  for (int i = 0; i < needed_.size(); ++i) {
    int amount = 0;
    for (auto& batch : bad_products[needed_[i]]) {
      amount += batch.total_quantity();
    }
    int wanted = want_to_buy(needed_[i].quantity(), amount);
    if (on_hand_[i].quantity() < wanted) {
      return Bundle(dynamic_cast<Product&>(needed_[i]),
                    wanted - on_hand_[i].quantity(), 0);
    }
  }
  return Bundle();
}

int Customer::want_to_buy(int needed, int amount) {
  if (needed == 0) return 0;
  return needed * (1 + 1.0 * amount / needed) * (dis(gen) % 5 + 1);
}

bool Customer::operator==(Customer other) {
  return needed_ == other.needed_ && on_hand_ == other.on_hand_;
}

void Customer::get_order(Bundle order) {
  for (int i = 0; i < needed_.size(); ++i) {
    if (Product(order) == Product(needed_[i])) {
      on_hand_[i].add_quantity(order.quantity());
      break;
    }
  }
}

void Customer::use_product() {
  bool works = true;
  for (int i = 0; i < needed_.size(); ++i) {
    if (on_hand_[i].quantity() < needed_[i].quantity()) {
      works = false;
    }
  }
  if (!works) return;
  for (int i = 0; i < needed_.size(); ++i) {
    on_hand_[i].add_quantity(-needed_[i].quantity());
  }
}

int Customer::name() { return name_; }