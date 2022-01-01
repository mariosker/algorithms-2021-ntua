#include <bits/stdc++.h>

using namespace std;

int threshold;

// binary search find the first bigger element from key and replace it
// example array 2 5 7 10 14 search for 6. it will return 7
// we use it for lis(longest increasing sequence)
// Binary search (note boundaries in the caller)
int upper_bound(vector<int>& v, int l, int r, int key) {
  while (r - l > 1) {
    int m = l + (r - l) / 2;
    if (v[m] >= key)
      r = m;
    else
      l = m;
  }
  return r;
}

// binary search find the next first smaller element from key and replace it
// example array 2 5 7 10 14 search for 6. it will return 5
// we use it for lds(longest decreasing sequence)
int lower_bound(vector<int>& v, int l, int r, int key) {
  int ans = -1;
  while (l <= r) {
    int m = (l + r) / 2;
    if (v[m] == key) return m;
    if (v[m] >= key)
      l = m + 1;
    else {
      ans = m;
      r = m - 1;
    }
  }
  return ans;
}

// find the longest increasing sequence if we add K to part of the array
int longest_incr_subsequence(vector<int>& v) {
  int max_forward;
  if (v.size() == 0) return 0;

  // initialize tail for lis(tail is the last number in an increasing seaquence )
  // for example
  // tail[0] last number in increasing seaquence with length = 1
  // tail[5] last number in increasing seaquence with length = 5
  // tail[11] last number in increasing seaquence with length = 12
  vector<int> tail(v.size(), 0);

  // we need 2 posisiton
  //      one for latest change and
  //      another for max up until this point
  vector<vector<int> > tail_forward_max_number(
      v.size(), vector<int>(2, -1));  // new tail [i][0] / tail for maximum length until this point [i][1]  (lis)
  vector<vector<int> > tail_forward_max_length(
      v.size(), vector<int>(2, -1));  // length for latest change [i][0] / max length up until this point [i][1]  (lis)

  vector<vector<int> > tail_backward_max_number(
      v.size(), vector<int>(2, -1));  // new tail [i][0] / tail for maximum length until this point [i][1]  (lds)
  vector<vector<int> > tail_backward_max_length(
      v.size(), vector<int>(2, -1));  // length for latest change [i][0] / max length up until this point [i][1]  (lds)

  int length_forward = 1;   // length of first element for lis
  int length_backward = 1;  // length of first element for lss

  int forward_pos;
  int backward_pos;

  // initialize first position with 1st element
  tail[0] = v[0];
  tail_forward_max_number[0][0] = v[0];
  tail_forward_max_number[0][1] = v[0];
  tail_forward_max_length[0][0] = length_forward;
  tail_forward_max_length[0][1] = length_forward;

  // find lis(longest increasing sequence)
  for (size_t i = 1; i < v.size(); i++) {
    // if v[i] is smaller than the smallest element yet tail[0] replace it
    if (v[i] < tail[0]) {
      tail[0] = v[i];

      tail_forward_max_number[i][0] = v[i];
      tail_forward_max_length[i][0] = 1;
    }
    // v[i] extends largest subsequence
    else if (v[i] > tail[length_forward - 1]) {
      tail[length_forward++] = v[i];

      tail_forward_max_number[i][0] = v[i];
      tail_forward_max_length[i][0] = length_forward;
    }
    // find if we can reduce some tail
    else {
      forward_pos = upper_bound(tail, -1, length_forward - 1, v[i]);

      tail[forward_pos] = v[i];

      tail_forward_max_number[i][0] = v[i];
      tail_forward_max_length[i][0] = forward_pos + 1;
    }

    // tail for maximum length until this point [i][1]  (lis)
    tail_forward_max_number[i][1] = tail[length_forward - 1];
    // max length up until this point [i][1]  (lis)
    tail_forward_max_length[i][1] = length_forward;
  }
  // max_forward: length of lis
  max_forward = length_forward;

  // initialize tail_2 for lds(tail is the last number in an decreasing seaquence )
  // for example
  // tail[0] last number in decreasing seaquence with length = 1
  // tail[5] last number in decreasing seaquence with length = 5
  // tail[11] last number in decreasing seaquence with length = 12
  vector<int> tail_2(v.size(), 0);

  // initialize first position with (n-1)th element
  tail_2[0] = v[v.size() - 1] + threshold;
  tail_backward_max_number[0][0] = tail_2[0];
  tail_backward_max_number[0][1] = tail_2[0];
  tail_backward_max_length[0][0] = length_backward;
  tail_backward_max_length[0][1] = length_backward;

  // find lds(longest decreasing sequence) starting from n-2
  for (int i = v.size() - 2; i != -1; i--) {
    // if v[i] is bigger than the biggest element yet tail[0] replace it
    if (v[i] + threshold > tail_2[0]) {
      tail_2[0] = v[i] + threshold;

      tail_backward_max_number[v.size() - 1 - i][0] = v[i] + threshold;
      tail_backward_max_length[v.size() - 1 - i][0] = 1;
    }
    // v[i] extends largest subsequence
    else if (v[i] + threshold < tail_2[length_backward - 1]) {
      tail_2[length_backward++] = v[i] + threshold;

      tail_backward_max_number[v.size() - 1 - i][0] = v[i] + threshold;
      tail_backward_max_length[v.size() - 1 - i][0] = length_backward;
    }
    // find if we can increase of some tail
    else {
      backward_pos = lower_bound(tail_2, 0, length_backward, v[i] + threshold);

      tail_2[backward_pos] = v[i] + threshold;

      tail_backward_max_number[v.size() - 1 - i][0] = v[i] + threshold;
      tail_backward_max_length[v.size() - 1 - i][0] = backward_pos + 1;
    }
    // tail for maximum length until this point [i][1]  (lds)
    tail_backward_max_number[v.size() - 1 - i][1] = tail_2[length_backward - 1];
    // max length up until this point [i][1]  (lds)
    tail_backward_max_length[v.size() - 1 - i][1] = length_backward;
  }

  int max = INT_MIN;
  int comb = 0;
  long int forw = 0;
  long int backw = 0;
  long int forw_max = 0;
  long int backw_max = 0;

  // check each combination [0,0]-[1,n-2], [0,1]-[2,n-2], [0,2]-[3,n-2], [3,0]-[4,n-2],
  // [4,0]-[5,n-2],,.....,[0,n-3]-[n-2,n-2]
  for (size_t k = 0; k < v.size() - 1; k++) {
    forw = tail_forward_max_number[k][0];
    backw = tail_backward_max_number[v.size() - k - 2][0];

    forw_max = tail_forward_max_number[k][1];
    backw_max = tail_backward_max_number[v.size() - k - 2][1];

    // chech if we maintain increasing sequence
    if (forw < backw) {
      comb = tail_forward_max_length[k][0] + tail_backward_max_length[v.size() - k - 2][0];
      if (comb > max) {
        max = comb;
      }
    }
    comb = 0;

    // chech if we maintain increasing sequence
    if (forw_max < backw_max) {
      comb = tail_forward_max_length[k][1] + tail_backward_max_length[v.size() - k - 2][1];
      if (comb > max) {
        max = comb;
      }
    }
    comb = 0;
  }

  // check if lis of whole array is longer than the result we got by combining lis and lds
  if (max_forward > max) return max_forward;

  return max;
}

// main for manual input
int main() {
  int res;
  size_t years_count;
  vector<int> annual_salaries;

  cin >> years_count >> threshold;

  annual_salaries.resize(years_count, 0);
  for (size_t i = 0; i < years_count; ++i) cin >> annual_salaries[i];

  res = longest_incr_subsequence(annual_salaries);

  cout << res << endl;

  return 0;
}
