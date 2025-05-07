#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;

// Sequential bubble sort for comparison
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel bubble sort using OpenMP
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    
    for (int i = 0; i < n-1; i++) {
        // Odd-even sort method for parallelization
        #pragma omp parallel for
        for (int j = 1; j < n-1; j += 2) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
        
        #pragma omp parallel for
        for (int j = 0; j < n-1; j += 2) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
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
    
    // Measure parallel bubble sort time
    auto start = chrono::high_resolution_clock::now();
    parallelBubbleSort(arr);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> parallel_time = end - start;
    
    cout << "Sorted array (parallel): ";
    printArray(arr);
    cout << "Parallel Bubble Sort execution time: " << parallel_time.count() << " seconds" << endl;
    
    // Measure sequential bubble sort time
    start = chrono::high_resolution_clock::now();
    sequentialBubbleSort(arr_copy);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> sequential_time = end - start;
    
    cout << "Sequential Bubble Sort execution time: " << sequential_time.count() << " seconds" << endl;
    cout << "Speedup: " << sequential_time.count() / parallel_time.count() << "x" << endl;
    
    return 0;
} 