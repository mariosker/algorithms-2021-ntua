/**
 * @file solver.cpp
 * @author Marios Kerasiotis (marioskerasiotis+git_algo@gmail.com)
 * @brief Best way to sell to exactly K people.
 * @version 0.1
 * @date 2021-11-23
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <bits/stdc++.h>
using namespace std;

struct FenwickTree {
  vector<long int> bit;  // binary indexed tree
  long int n;

  FenwickTree(long int n) {
    this->n = n;
    bit.assign(n, 0);
  }

  FenwickTree(vector<long int> a) : FenwickTree(a.size()) {
    for (size_t i = 0; i < a.size(); i++) add(i, a[i]);
  }

  long int sum(long int r) {
    long int ret = 0;
    for (; r >= 0; r = (r & (r + 1)) - 1) ret += bit[r];
    return ret;
  }

  long int sum(long int l, long int r) { return sum(r) - sum(l - 1); }

  void add(long int idx, long int delta) {
    for (; idx < n; idx = idx | (idx + 1)) bit[idx] += delta;
  }
};

class City {
 public:
  int buildings;
  int buyer_count;
  FenwickTree* tenants;
  vector<int> K;

  void set(int buildings_, int buyer_count_) {
    buildings = buildings_;
    buyer_count = buyer_count_;
    tenants = new FenwickTree(buildings_);
    K.resize(buyer_count + 1, INT_MAX);
  }

  int check_sum(int y, long int target) {
    long int current_sum;

    for (int j = y; j < buildings; j++) {
      current_sum = tenants->sum(y, j);
      if (current_sum <= buyer_count)
        K[current_sum] = min(K[current_sum], j - y + 1);
      else
        break;
    }

    return (K[target] != INT_MAX) ? K[target] : -1;
  }

  int find_min() {
    int min_length = INT_MAX;
    int right_length = 0;
    long int current_sum = 0;

    for (int y = buildings - 1; y >= 0; y--) {
      for (int x = 0; x <= y; x++) {
        current_sum = tenants->sum(x, y);
        if (current_sum <= buyer_count) K[current_sum] = min(K[current_sum], y - x + 1);

        if (current_sum == buyer_count)
          min_length = min(min_length, y - x + 1);
        else if (current_sum < buyer_count) {
          right_length = check_sum(y + 1, buyer_count - current_sum);
          if (right_length != -1) min_length = min(min_length, right_length + y - x + 1);
        }
      }
    }

    return (min_length != INT_MAX) ? min_length : -1;
  }

  City(){};

  ~City() { delete tenants; };
};

bool get_input(City& city) {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int buildings;
  int buyer_count;
  int tenant_count;
  cin >> buildings >> buyer_count;
  city.set(buildings, buyer_count);

  for (int i = 0; i < buildings; i++) {
    cin >> tenant_count;
    if (tenant_count == buyer_count) return true;
    city.tenants->add(i, tenant_count);
  }
  return false;
}

int main() {
  City city;
  int distance;
  if (get_input(city))
    distance = 1;
  else {
    distance = city.find_min();
  }
  cout << distance << endl;
  return 0;
}
