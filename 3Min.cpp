#include <iostream>     // For standard input/output
#include <omp.h>        // For OpenMP parallelization
#include <vector>       // For using std::vector
#include <limits>       // For numeric limits like INT_MAX, INT_MIN

using namespace std;

int main() {
    const int N = 10000;               // Size of the data array
    vector<int> data(N);               // Vector to hold N integers

    // Initialize the data with values from 0 to 999 repeatedly
    for (int i = 0; i < N; ++i)
        data[i] = i % 1000;

    // Initialize min, max, and sum
    int min_val = numeric_limits<int>::max();   // Start with maximum possible value for min
    int max_val = numeric_limits<int>::min();   // Start with minimum possible value for max
    long long sum = 0;                          // To store the sum of all values

    // Parallel loop using OpenMP with reduction operations for min, max, and sum
    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < N; ++i) {
        int val = data[i];

        // These conditions are technically redundant due to the reduction,
        // but kept for clarity. The reduction itself ensures correctness.
        if (val < min_val) min_val = val;
        if (val > max_val) max_val = val;

        sum += val;  // Accumulate sum
        // cout << val << " "; // Optional: Print values (commented out for performance)
    }

    // Calculate average by dividing sum by number of elements
    double avg = (double)sum / N;

    // Print results
    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;

    return 0; // Exit program
}
