#include <bits/stdc++.h>

int N, K;

using namespace std;

int main() {
  cin >> N;
  cin >> K;

  int *num_of_people_in_each_apartment =
      new int[N];  // pinakas me 8eseis N(diamerismata) kai se ka8e 8esh exoume ton ari8mo ton katikon tou diamerismato
  int *sum_up_to_point_for_sum_array = new int[N];  // a8roisma mexri ena simio me 8eseis N gia pinaka sum
  for (int i = 0; i < N; i++) {
    sum_up_to_point_for_sum_array[i] = 0;
  }

  int number;
  int *sum_of_each_subarray = new int[K + 1];  // pinakas me mege8os array poy exei sum tin 8esi tou (sum=i)
  for (int i = 0; i < K + 1; i++) {
    sum_of_each_subarray[i] = 2 * N;  // arxikopoiv olew tiw 8eseiw me length megalitero apo ton arxiko pinaka
  }
  sum_of_each_subarray[0] = 0;

  int count = 0;
  for (int i = 0; i < N; i++) {
    cin >> number;
    num_of_people_in_each_apartment[count] = number;  // eisagvgi ari8mon katoikon se kaue diamerisma
    count = count + 1;
    if (K == 0 && number == 0) {
      cout << 1 << endl;
      return 0;
    }
  }

  // LARGE_INTEGER t1, t2, tc;
  // QueryPerformanceFrequency(&tc);
  // QueryPerformanceCounter(&t1);
  // num_of_people_in_each_apartment  atoma ana diamerisma
  // sum_up_to_point_i                sum anuropon mexri ena simio
  // sum_of_each_subarray             pinakas me mikoi gia sums
  int min_answer = INT_MAX;
  for (int i = 0; i < N; i++) {
    int temp = 0;
    for (int j = 0; j < i + 1; j++) {
      temp = num_of_people_in_each_apartment[i] + sum_up_to_point_for_sum_array[j];
      if (temp <= K) {
        sum_up_to_point_for_sum_array[j] = temp;

        // isvw prepei na tsekarv oti den einai kainourio
        if (sum_of_each_subarray[temp] > i - j + 1) {
          sum_of_each_subarray[temp] = i - j + 1;
        }
      }
    }

    int temp_no_memmory = 0;
    for (int k = i + 1; k < N; k++) {
      int temp_answer = 0;
      int search_k = 0;
      temp_no_memmory = temp_no_memmory + num_of_people_in_each_apartment[k];
      if (temp_no_memmory <= K) {
        search_k = K - temp_no_memmory;
        if (search_k > -1 && search_k <= K &&
            sum_of_each_subarray[search_k] <
                2 * N) {  // isvw auto sum_of_each_subarray[search_k]<2*N na prepei an to balv se ujexoristo if apo kato
          temp_answer = sum_of_each_subarray[search_k] + k - i;
          if (temp_answer < min_answer) {
            min_answer = temp_answer;
          }
        }

      }
      // mallon akpou mpainei length=k-i+1
      else {
        k = N;
        // kane kati an jeperasei to auroisma to K
      }
    }
  }

  if (min_answer == INT_MAX) min_answer = -1;
  cout << min_answer << endl;
  return 0;
}
