#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;

// Sequential merge operation
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Sequential merge sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        
        merge(arr, left, mid, right);
    }
}

// Parallel merge sort
void parallelMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    const int MAX_DEPTH = 3; // Control parallelism depth
    
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // Use parallelism for initial divisions
        if (depth < MAX_DEPTH) {
            #pragma omp task
            parallelMergeSort(arr, left, mid, depth + 1);
            
            #pragma omp task
            parallelMergeSort(arr, mid + 1, right, depth + 1);
            
            #pragma omp taskwait
        } else {
            // Switch to sequential for deeper recursion
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }
        
        merge(arr, left, mid, right);
    }
}

// Wrapper for parallel merge sort
void parallelMergeSortWrapper(vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSort(arr, 0, arr.size() - 1);
    }
}

// Function to print array
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;
    
    vector<int> arr(n);
    vector<int> arr_copy(n);
    
    cout << "Enter " << n << " integers:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        arr_copy[i] = arr[i];
    }
    
    cout << "Original array: ";
    printArray(arr);
    
    // Measure parallel merge sort time
    auto start = chrono::high_resolution_clock::now();
    parallelMergeSortWrapper(arr);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> parallel_time = end - start;
    
    cout << "Sorted array (parallel): ";
    printArray(arr);
    cout << "Parallel Merge Sort execution time: " << parallel_time.count() << " seconds" << endl;
    
    // Measure sequential merge sort time
    start = chrono::high_resolution_clock::now();
    sequentialMergeSort(arr_copy, 0, arr_copy.size() - 1);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> sequential_time = end - start;
    
    cout << "Sequential Merge Sort execution time: " << sequential_time.count() << " seconds" << endl;
    cout << "Speedup: " << sequential_time.count() / parallel_time.count() << "x" << endl;
    
    return 0;
} 