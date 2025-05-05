#include<iostream>
#include<omp.h>  // Include OpenMP for parallel programming support

using namespace std;

// Function to perform Bubble Sort in a sequential manner
void bubble(int array[], int n){
    for (int i = 0; i < n - 1; i++){  // Outer loop for each pass of bubble sort
        for (int j = 0; j < n - i - 1; j++){  // Inner loop for comparing adjacent elements
            if (array[j] > array[j + 1]) // If elements are in the wrong order, swap them
                swap(array[j], array[j + 1]);
        }
    }
}

// Function to perform a parallel version of Bubble Sort using OpenMP
void pBubble(int array[], int n){
    // Sort odd indexed numbers using parallelism
    for(int i = 0; i < n; ++i){    
        #pragma omp for  // Parallelize this loop
        for (int j = 1; j < n; j += 2){  // Loop through odd-indexed elements
            if (array[j] < array[j - 1])  // If odd-indexed element is smaller than previous one, swap
                swap(array[j], array[j - 1]);
        }

    // Synchronize threads after sorting odd indexed numbers
    #pragma omp barrier  // Ensure all threads finish odd-index sorting before proceeding

    // Sort even indexed numbers using parallelism
    #pragma omp for  // Parallelize this loop
    for (int j = 2; j < n; j += 2){  // Loop through even-indexed elements
        if (array[j] < array[j - 1])  // If even-indexed element is smaller than previous one, swap
            swap(array[j], array[j - 1]);
    }
  }
}

// Helper function to print the array elements
void printArray(int arr[], int n){
    for(int i = 0; i < n; i++) 
        cout << arr[i] << " ";  // Print each element in the array
    cout << "\n";  // Print a newline at the end
}

int main(){
    // Set up variables
    int n = 10;  // Define the size of the array
    int arr[n];  // Declare array arr
    int brr[n];  // Declare array brr (though it isn't used in the code)
    double start_time, end_time;  // Variables to measure execution time

    // Initialize the array with numbers from n to 1 (reverse order)
    for(int i = 0, j = n; i < n; i++, j--) 
        arr[i] = j;
    
    // Measure time taken for the sequential bubble sort
    start_time = omp_get_wtime();  // Start time
    bubble(arr, n);  // Perform sequential bubble sort
    end_time = omp_get_wtime();  // End time
    cout << "Sequential Bubble Sort took : " << end_time - start_time << " seconds.\n";
    printArray(arr, n);  // Print the sorted array
    
    // Reset the array to original reverse order for parallel sorting
    for(int i = 0, j = n; i < n; i++, j--) 
        arr[i] = j;
    
    // Measure time taken for the parallel bubble sort
    start_time = omp_get_wtime();  // Start time
    pBubble(arr, n);  // Perform parallel bubble sort
    end_time = omp_get_wtime();  // End time
    cout << "Parallel Bubble Sort took : " << end_time - start_time << " seconds.\n";
    printArray(arr, n);  // Print the sorted array
}
