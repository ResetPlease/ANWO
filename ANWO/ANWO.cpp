#include "ANWO.h"

void ANWO::get_orders() {
  for (auto& customer : info.customers()) {
    Bundle order = customer->gen_order(bad_products);
    if (order.name() == -1) continue;
    info.stats_get_order(customer->name());
    orders_.push_back(Order(order, customer));
  }
  info.stats_warehouse_amounts(good_products, bad_products);
}

void ANWO::send_orders() {
  sort(orders_.begin(), orders_.end());

  int income = 0;
  for (auto& order : orders_) {
    Bundle to_send = Bundle(order.bundle(), 0, 0);

    std::vector<Batch>& bads = bad_products[order.bundle()];
    std::sort(bads.begin(), bads.end());
    while (to_send.quantity() < order.bundle().quantity() && bads.size() > 0) {
      while (to_send.quantity() < order.bundle().quantity() &&
             bads[0].bundle_amount() > 0) {
        to_send.add_quantity(bads[0].quantity());
        bads[0].add_bundle_amount(-1);
        income += bads[0].price();
      }
      if (bads[0].bundle_amount() == 0) {
        bads.erase(bads.begin());
      }
    }

    std::vector<Batch>& goods = good_products[order.bundle()];
    std::sort(goods.begin(), goods.end());
    while (to_send.quantity() < order.bundle().quantity() && goods.size() > 0) {
      while (to_send.quantity() < order.bundle().quantity() &&
             goods[0].bundle_amount() > 0) {
        to_send.add_quantity(goods[0].quantity());
        goods[0].add_bundle_amount(-1);
        income += goods[0].price();
      }
      if (goods[0].bundle_amount() == 0) {
        goods.erase(goods.begin());
      }
    }

    int not_enough = order.bundle().quantity() - to_send.quantity();
    int buy =
        ((not_enough > 0 ? not_enough : 0) + order.bundle().quantity() / 1000);
    to_buy[order.bundle()] += buy;
    if (to_send.quantity() > 0) {
      info.stats_send_order(order.customer()->name(), order.bundle().name(),
                            order.bundle().quantity());
    }
    order.customer()->get_order(to_send);
  }
  orders_.clear();
  info.add_income(income);
}

void ANWO::buy_products() {
  Merchant merch;
  int lost = 0;
  for (auto& transp : transports_) {
    Batch batch = transp.get_batch();
    already_buied_[batch] += batch.total_quantity();
  }
  for (auto& prod : to_buy) {
    prod.second -= already_buied_[prod.first];
    if (prod.second <= 0) continue;
    info.stats_buy_product(prod.first.name(), prod.second);
    transports_.push_back(merch.create_transport(prod.first, prod.second));
    Batch batch = transports_.rbegin()->get_batch();
    lost += batch.price() * batch.bundle_amount();
  }
  info.add_lost(lost);
  to_buy.clear();
  already_buied_.clear();
}

void ANWO::get_products() {
  std::sort(transports_.begin(), transports_.end());
  int i = 0;
  while (i < transports_.size()) {
    transports_[i].reduce_days(1);
    if (transports_[i].get_days() == 0) {
      Batch batch = transports_[i].get_batch();
      info.stats_get_product(batch.name(), batch.total_quantity());
      batch.set_price(batch.price() * (100 + info.added_percent()) / 100);
      good_products[batch].push_back(batch);
      transports_.erase(transports_.begin() + i);
      continue;
    }
    ++i;
  }
  return;
}

void ANWO::reduce_prices() {
  for (auto& prod : good_products) {
    int i = 0;
    while (i < prod.second.size()) {
      if (prod.second[i].left_to_live(info.day()) <= 2) {
        Batch batch = prod.second[i];
        info.stats_bad(prod.first.name(), batch.total_quantity());
        prod.second.erase(prod.second.begin() + i);
        batch.set_price(batch.price() * (100 - info.reduced_percent()) / 100);
        bad_products[batch].push_back(batch);
        continue;
      }
      ++i;
    }
  }
  for (auto& prod : bad_products) {
    int i = 0;
    while (i < prod.second.size()) {
      if (prod.second[i].left_to_live(info.day()) == 0) {
        info.stats_trash(prod.first.name(), prod.second[i].total_quantity());
        prod.second.erase(prod.second.begin() + i);
        continue;
      }
      ++i;
    }
  }
}

std::string work(int days_num, int customer_num, int product_num, int added_percent,
          int reduced_percent) {
  info.set_params(days_num, customer_num, product_num, added_percent,
                  reduced_percent);
  ANWO warehouse;
  for (; info.day() <= info.days_number(); info.update_day()) {
    info.out_stats();
    warehouse.reduce_prices();
    warehouse.buy_products();
    warehouse.get_products();
    warehouse.send_orders();
    warehouse.get_orders();
  }
  return out;
}