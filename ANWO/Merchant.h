#include "Info.h"

class Merchant {
 public:
  Merchant() {}
  Transport create_transport(Product product, int quantity) {
    Bundle bundle(product, dis(gen) % 10 + 1, info.get_price(product));
    int l = 0, r = INF;
    while (l + 1 < r) {
      int m = (l + r) / 2;
      if (m * bundle.quantity() < quantity)
        l = m;
      else
        r = m;
    }
    Batch batch(bundle, r, info.day());
    Transport transport(dis(gen) % 5 + 1, batch);
    return transport;
  }
};
