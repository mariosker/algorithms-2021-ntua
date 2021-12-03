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

struct Sum_details {
  int s;
  int f;
  int dif;

  Sum_details(int s, int f) {
    this->s = s;
    this->f = f;
    this->dif = f - s + 1;
  }
};
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
  FenwickTree* tenants;
  vector<vector<Sum_details>> k_sums;

  void set(int buildings_, int buyer_count_) {
    buildings = buildings_;
    buyer_count = buyer_count_;
    tenants = new FenwickTree(buildings_);
  }

  void fill_K_sums() {
    int s;
    k_sums.resize(buyer_count + 1);
    for (int i = 0; i < buildings; i++) {
      for (int j = i; j < buildings; j++) {
        s = tenants->sum(i, j);
        if (s <= buyer_count) {
          k_sums[s].push_back(Sum_details(i, j));
        }
      }
    }

    for (int i = 0; i < buyer_count; i++) {
      sort(k_sums[i].begin(), k_sums[i].end(),
           [](Sum_details const& stud1, Sum_details const& stud2) -> bool {
             return stud1.dif < stud2.dif;
           });
    }
  }

  bool are_nonoverlapping(const Sum_details& a, const Sum_details& b) {
    return (a.f < b.s || b.f < a.s);
  }

  int find_min() {
    int min_length = INT_MAX;
    int s1;
    int s2;
    int index1;
    int index2;
    for (int i = 1; i < buyer_count / 2; i++) {
      int s1 = k_sums[i].size();
      int s2 = k_sums[buyer_count - i].size();
      if (s1 && s2) {
        index1 = 0;
        index2 = 0;

        while (index1 < s1 - 1 || index2 < s2 - 1) {
          if (are_nonoverlapping(k_sums[i][index1], k_sums[buyer_count - i][index2])) {
            min_length =
                min(min_length, k_sums[i][index1].dif + k_sums[buyer_count - i][index2].dif);
            break;
          } else {
            if (k_sums[i][index1].dif < k_sums[buyer_count - i][index2].dif) {
              if (index2 < s2 - 1)
                index2++;
              else if (index1 < s1 - 1)
                index1++;
            } else if (k_sums[i][index1].dif >= k_sums[buyer_count - i][index2].dif) {
              if (index1 < s1 - 1)
                index1++;
              else if (index2 < s2 - 1)
                index2++;
            }
          }
        }
      }
    }
    if (k_sums[buyer_count].size()) min_length = min(min_length, k_sums[buyer_count][0].dif);
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
    city.fill_K_sums();
    distance = city.find_min();
  }
  cout << distance << endl;
  return 0;
}
