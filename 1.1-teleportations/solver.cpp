/**
 * @file solver.cpp
 * @author Marios Kerasiotis
 * @brief Solution for the teleportation problem in O((N+M)logM)
 * @version 1.2
 * @date 2021-11-18
 *
 * @copyright Copyright (c) 2021
 *
 */
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Holds data for each portal
 *
 */
class Portal {
 public:
  int entry_universe;
  int exit_universe;
  int portal_size;

  /**
   * @brief Construct a new Portal object
   *
   */
  Portal() {
    entry_universe = 0;
    exit_universe = 0;
    portal_size = 0;
  }

  /**
   * @brief Set the portal object
   *
   * @param entry_universe_ where the portal begins
   * @param exit_universe_ where the portal ends
   * @param portal_size_ the size of the portal
   */
  void set_portal(int entry_universe_, int exit_universe_, int portal_size_) {
    entry_universe = entry_universe_;
    exit_universe = exit_universe_;
    portal_size = portal_size_;
  }

  /**
   * @brief print the portal structure
   *
   */
  void print_portal() {
    cout << "(" << entry_universe << ", " << exit_universe << ", "
         << portal_size << "), ";
  }
};

/**
 * @brief Multiverse object containing portals and universe order
 *
 */
class Multiverse {
 public:
  int universe_count;
  int portal_count;
  vector<int> universes;
  vector<Portal> portals;

  /**
   * @brief Construct a new Multiverse object
   *
   */
  Multiverse() {
    universe_count = 0;
    portal_count = 0;
  }

  /**
   * @brief Set the universes object
   *
   * @param universe_count_ number of universes
   */
  void set_universes(int universe_count_) {
    universe_count = universe_count_;
    universes.resize(universe_count);
  }

  /**
   * @brief Set the portals object
   *
   * @param portal_count_ number of portals
   */
  void set_portals(int portal_count_) {
    portal_count = portal_count_;
    portals.resize(portal_count);
  }

  /**
   * @brief
   *
   * @param a Portal a
   * @param b Portal b
   * @return true if portal a has bigger size than b
   * @return false if portal b has bigger size than a
   */
  static bool compare_portal_size(const Portal &a, const Portal &b) {
    return (a.portal_size < b.portal_size);
  }
  /**
   * @brief sort portals based on their size
   *
   */
  void sort_portals() {
    sort(portals.begin(), portals.end(), compare_portal_size);
  }
};

/**
 * @brief Disjoint Set
 *
 */
class Dsu {
  vector<int> parent;
  vector<int> size;

 public:
  /**
   * @brief Construct a new Dsu object
   *
   * @param array_size number of nodes for the dsu
   */
  Dsu(int array_size) {
    parent.resize(array_size);
    size.resize(array_size);

    for (int i = 0; i < array_size; i++) {
      parent[i] = i;
      size[i] = 1;
    }
  }

  /**
   * @brief Destroy the Dsu object
   *
   */
  ~Dsu() {
    vector<int>().swap(parent);
    vector<int>().swap(size);
  }

  /**
   * @brief finds head of the set that node belongs to
   *
   * @param v number of node a
   * @return int number of set head node
   */
  int find_set(int v) {
    if (v == parent[v]) return v;
    return parent[v] = find_set(parent[v]);
  }

  /**
   * @brief Connect two sets using ranking
   *
   * @param a number of node a
   * @param b number of node b
   */
  void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      if (size[a] < size[b]) swap(a, b);
      parent[b] = a;
      size[a] += size[b];
    }
  }

  /**
   * @brief checks if two nodes are in the same set
   *
   * @param a number of node a
   * @param b number of node b
   * @return true if two nodes are in the same set
   * @return false if two nodes are not in the same set
   */
  bool are_connected(int a, int b) { return find_set(a) == find_set(b); }
};

/**
 * @brief Get the necessary input
 *
 * @param multiverse Empty Multiverse object
 */
void get_input(Multiverse &multiverse) {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int universe, universe_count, portal_count;
  int entry_universe, exit_universe, portal_size;

  cin >> universe_count >> portal_count;

  multiverse.set_universes(universe_count);
  multiverse.set_portals(portal_count);

  for (int i = 0; i < multiverse.universe_count; i++) {
    cin >> universe;
    multiverse.universes[i] = --universe;
  }

  for (int i = 0; i < multiverse.portal_count; i++) {
    cin >> entry_universe >> exit_universe >> portal_size;
    multiverse.portals[i].set_portal(--entry_universe, --exit_universe,
                                     portal_size);
  }
}

/**
 * @brief Checks if the portals are connected with certain weight and above
 *
 * @param multiverse Multiverse object that holds portals and universes
 * @param weight_index index of the minimum weight to scan portal connectivity
 * @return true if portals are connected
 * @return false if portals are not connected
 */
bool check_weight(Multiverse &multiverse, int weight_index) {
  Dsu dsu(multiverse.universe_count);

  for (int i = weight_index; i < multiverse.portal_count; i++) {
    dsu.union_sets(multiverse.portals[i].entry_universe,
                   multiverse.portals[i].exit_universe);
  }

  for (int i = 0; i < multiverse.universe_count; i++) {
    if (!dsu.are_connected(i, multiverse.universes[i])) return false;
  }

  return true;
}

/**
 * @brief Find the biggest size portal where all universes connect with each
 * other.
 *
 * @param multiverse
 * @return int
 */
int right_bound(Multiverse &multiverse) {
  int left = 0;
  int right = multiverse.portal_count;

  while (left < right) {
    int mid = left + (right - left) / 2;
    if (!check_weight(multiverse, mid)) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }
  return right - 1;
}

int main() {
  struct Multiverse multiverse;
  get_input(multiverse);

  multiverse.sort_portals();

  const int solution = right_bound(multiverse);
  cout << multiverse.portals[solution].portal_size << endl;

  return 0;
}
