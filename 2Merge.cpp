#include<iostream>
#include<omp.h>  // Include OpenMP for parallel programming support

using namespace std;

// Function to merge two sorted subarrays into one sorted array
void merge(int low, int high, int mid, int arr[]){
    int n1 = mid - low + 1;  // Size of the left subarray
    int n2 = high - mid;     // Size of the right subarray

    // Temporary arrays to hold the left and right subarrays
    int left[n1];
    int right[n2];

    // Copy data to temporary arrays
    for(int i = 0; i < n1; i++){
        left[i] = arr[low + i];  // Copying elements from the left part of the array
    }

    for(int i = 0; i < n2; i++){
        right[i] = arr[mid + i + 1];  // Copying elements from the right part of the array
    }

    int i = 0, j = 0, k = low;
    
    // Merging the two subarrays back into the original array
    while(i < n1 && j < n2){
        if(left[i] <= right[j]){  // If the left element is smaller or equal, add it to the array
            arr[k] = left[i];
            i++;
        } else {  // If the right element is smaller, add it to the array
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // If there are remaining elements in the left subarray, add them to the array
    while(i < n1){
        arr[k] = left[i];
        i++;
        k++;
    }

    // If there are remaining elements in the right subarray, add them to the array
    while(j < n2){
        arr[k] = right[j];
        j++;
        k++;
    }
}

// Parallel Merge Sort function using OpenMP
void parallelSort(int arr[], int low, int high){
    if(low < high){
        int mid = (low + high) / 2;  // Find the middle of the array

        // Parallelize the recursive sorting of the two subarrays
        #pragma omp parallel sections
        {
            // Parallel section for sorting the left subarray
            #pragma omp section
            {
                parallelSort(arr, low, mid);
            }

            // Parallel section for sorting the right subarray
            #pragma omp section
            {
                parallelSort(arr, mid + 1, high);
            }
        }

        // After both subarrays are sorted, merge them
        merge(low, high, mid, arr);
    }
}

int main(){
    // Initialize the array
    int arr[5] = {5, 1, 6, 3, 7};
    
    // Perform parallel merge sort on the array
    parallelSort(arr, 0, 4);
    
    // Print the sorted array
    for(int i = 0; i < 5; i++){
        cout << arr[i] << " ";  // Output each element in the sorted array
    }

    return 0;
}
