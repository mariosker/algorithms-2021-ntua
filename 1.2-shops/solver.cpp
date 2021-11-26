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
  vector<int> bit;  // binary indexed tree
  int n;

  FenwickTree(int n) {
    this->n = n;
    bit.assign(n, 0);
  }

  FenwickTree(vector<int> a) : FenwickTree(a.size()) {
    for (size_t i = 0; i < a.size(); i++) add(i, a[i]);
  }

  int sum(int r) {
    int ret = 0;
    for (; r >= 0; r = (r & (r + 1)) - 1) ret += bit[r];
    return ret;
  }

  int sum(int l, int r) { return sum(r) - sum(l - 1); }

  void add(int idx, int delta) {
    for (; idx < n; idx = idx | (idx + 1)) bit[idx] += delta;
  }
};

class City {
 public:
  int buildings;
  int buyer_count;
  FenwickTree *tenants;
  vector<vector<int>> caching;

  void set(int buildings_, int buyer_count_) {
    buildings = buildings_;
    buyer_count = buyer_count_;
    tenants = new FenwickTree(buildings_);
    caching.resize(buildings, vector<int>(buildings, -1));
  }
  City(){};
  ~City() { delete tenants; };
};

bool get_input(City &city) {
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

int min_length_subarray_with_target_sum(int idx_s, City &city, int target_sum) {
  int min_length = INT_MAX;
  int subaray_sum = 0;

  for (int i = idx_s; i < city.buildings; i++) {
    for (int j = i; j < city.buildings; j++) {
      if (city.caching[i][j] == -1) {
        subaray_sum = city.tenants->sum(i, j);
        city.caching[i][j] = subaray_sum;
      } else {
        subaray_sum = city.caching[i][j];
      }

      if (subaray_sum > target_sum) {
        break;
      } else if (subaray_sum == target_sum) {
        min_length = min(min_length, j - i + 1);
      }
    }
  }
  return min_length;
}

int min_length_subarrays(City &city) {
  int min_length = INT_MAX;

  int subaray_sum = 0;

  for (int i = 0; i < city.buildings; i++) {
    for (int j = i; j < city.buildings; j++) {
      if (city.caching[i][j] == -1) {
        subaray_sum = city.tenants->sum(i, j);
        city.caching[i][j] = subaray_sum;
      } else {
        subaray_sum = city.caching[i][j];
      }
      if (subaray_sum > city.buyer_count) {
        break;
      } else if (subaray_sum == city.buyer_count) {
        min_length = min(min_length, j - i + 1);
      } else {
        int res = min_length_subarray_with_target_sum(
            j + 1, city, city.buyer_count - subaray_sum);
        if (res != INT_MAX) min_length = min(min_length, j - i + 1 + res);
      }
    }
  }
  return min_length;
}

int main() {
  City city;
  int distance;
  if (get_input(city))
    distance = 1;
  else
    distance = min_length_subarrays(city);
  cout << (distance != INT_MAX ? distance : -1) << endl;
}
