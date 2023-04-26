#include "ANWO.h"

int main() {
  int days_num, customer_num, product_num, added_percent, reduced_percent;
  fin >> days_num >> customer_num >> product_num >> added_percent >>
      reduced_percent;
  fout << work(days_num, customer_num, product_num, added_percent,
       reduced_percent);
}