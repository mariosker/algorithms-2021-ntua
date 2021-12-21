#include <bits/stdc++.h>
using namespace std;

typedef array<int, 2> offer;
vector<vector<vector<offer>>> merchants;
vector<vector<vector<vector<int>>>> memoization;

int count_sets;
int merchants_max_offers;
int merchant0_full_sets;
int merchant1_full_sets;
int merchant2_full_sets;

void get_input();
bool comp_offer_by_cost(offer i, offer j);

int solve();
int aux_solve(int merchant, int n_offers);
unsigned int get_min_cost(int merchant, int n_offers, int product, int offer);

int get_max_offers();
int get_merchant_full_sets(int merchant);

int main() {
  merchants.resize(3);  // 3 products
  for (int i = 0; i < 3; ++i) {
    (merchants[i]).resize(3);  // 3 merchants
  }

  get_input();

  merchant0_full_sets = get_merchant_full_sets(0);
  merchant1_full_sets = get_merchant_full_sets(1);
  merchant2_full_sets = get_merchant_full_sets(2);

  if (count_sets > merchant0_full_sets + merchant1_full_sets + merchant2_full_sets) {
    cout << "-1" << endl;
    return 0;
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      sort((merchants[i][j]).begin(), (merchants[i][j]).end(), comp_offer_by_cost);
    }
  }

  merchants_max_offers = get_max_offers();
  memoization.resize(3);
  for (int i = 0; i < 3; ++i) {
    memoization[i].resize(3);
    for (int j = 0; j < 3; ++j) {
      memoization[i][j].resize(count_sets + 1);
      for (int k = 0; k < count_sets + 1; ++k) {
        memoization[i][j][k].resize(merchants_max_offers, -1);
      }
    }
  }

  cout << solve() << endl;
  return 0;
}

void get_input() {
  ios_base::sync_with_stdio(false);

  short count_deals;
  char id[2];
  int quantity;
  int cost;

  cin >> count_sets >> count_deals;

  for (short i = 0; i < count_deals; ++i) {
    cin >> id >> quantity >> cost;

    if (id[1] == 'A') merchants[0][id[0] - '0' - 1].push_back(offer{quantity, cost});
    if (id[1] == 'B') merchants[1][id[0] - '0' - 1].push_back(offer{quantity, cost});
    if (id[1] == 'C') merchants[2][id[0] - '0' - 1].push_back(offer{quantity, cost});
  }
}

bool comp_offer_by_cost(offer i, offer j) {
  if (i[1] == j[1]) {
    return i[0] > j[0];
  }
  return i[1] < j[1];
}

int get_merchant_full_sets(int merchant) {
  int total_products_A = 0;
  int total_products_B = 0;
  int total_products_C = 0;

  for (offer it : merchants[0][merchant]) total_products_A += it[0];

  for (offer it : merchants[1][merchant]) total_products_B += it[0];

  for (offer it : merchants[2][merchant]) total_products_C += it[0];

  return (min(total_products_A, min(total_products_B, total_products_C)));
}

int get_max_offers() {
  int max_offers = 0;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      max_offers = max(max_offers, (int)merchants[i][j].size());
    }
  }
  return max_offers;
}

int solve() {
  int temp_cost;
  int min_cost = numeric_limits<int>::max();

  int aux_merchant0;
  int aux_merchant1;
  int aux_merchant2;

  if (count_sets > merchant0_full_sets + merchant1_full_sets + merchant2_full_sets) return -1;

  for (int i = 0; i <= min(merchant0_full_sets, count_sets); ++i) {
    for (int j = 0; j <= min(merchant1_full_sets, count_sets - i); ++j) {
      if (merchant2_full_sets < count_sets - i - j) continue;

      aux_merchant0 = aux_solve(0, i);
      aux_merchant1 = aux_solve(1, j);
      aux_merchant2 = aux_solve(2, count_sets - i - j);

      temp_cost = aux_merchant0 + aux_merchant1 + aux_merchant2;
      min_cost = min(temp_cost, min_cost);
    }
  }

  return min_cost;
}

int aux_solve(int merchant, int n_offers) {
  if (n_offers <= 0) return 0;

  int min_cost_A = get_min_cost(merchant, n_offers, 0, 0);
  int min_cost_B = get_min_cost(merchant, n_offers, 1, 0);
  int min_cost_C = get_min_cost(merchant, n_offers, 2, 0);

  return min_cost_A + min_cost_B + min_cost_C;
}

unsigned int get_min_cost(int merchant, int quantity, int product, int offer) {
  if (quantity <= 0) return 0;

  if (offer < merchants_max_offers && memoization[product][merchant][quantity][offer] != -1)
    return memoization[product][merchant][quantity][offer];

  int temp_cost = numeric_limits<int>::max();

  if (offer >= (int)((merchants[product][merchant]).size()))  // size <= M -> int
    return numeric_limits<int>::max();

  if (merchants[product][merchant][offer][0] >= count_sets)
    return memoization[product][merchant][quantity][offer] = merchants[product][merchant][offer][1];

  temp_cost = min(get_min_cost(merchant, quantity - (merchants[product][merchant][offer][0]), product, offer + 1) +
                      (merchants[product][merchant][offer][1]),
                  get_min_cost(merchant, quantity, product, offer + 1));
  return memoization[product][merchant][quantity][offer] = temp_cost;

  return numeric_limits<int>::max();
}
