#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std;

// ========== Bubble Sort Implementation ==========

// Sequential bubble sort
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

// ========== Merge Sort Implementation ==========

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

// ========== Utility Functions ==========

// Function to print array
void printArray(const vector<int>& arr, int max_elements = 10) {
    int n = arr.size();
    int elements_to_print = min(max_elements, n);
    
    for (int i = 0; i < elements_to_print; i++) {
        cout << arr[i] << " ";
    }
    
    if (n > elements_to_print) {
        cout << "... (" << (n - elements_to_print) << " more elements)";
    }
    cout << endl;
}

// Function to verify sort is correct
bool verifySorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i-1] > arr[i]) {
            return false;
        }
    }
    return true;
}

// Function to generate random array
vector<int> generateRandomArray(int size, int min_val = 0, int max_val = 1000) {
    vector<int> arr(size);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(min_val, max_val);
    
    for (int i = 0; i < size; i++) {
        arr[i] = distr(gen);
    }
    
    return arr;
}

// ========== Main Function ==========

int main() {
    int choice, n;
    bool use_random_data = false;
    
    cout << "===== Parallel Sorting Algorithms =====" << endl;
    cout << "1. Bubble Sort" << endl;
    cout << "2. Merge Sort" << endl;
    cout << "3. Both (with performance comparison)" << endl;
    cout << "Enter your choice (1-3): ";
    cin >> choice;
    
    cout << "Enter the number of elements: ";
    cin >> n;
    
    cout << "Use random data? (1 for yes, 0 for no): ";
    cin >> use_random_data;
    
    vector<int> arr;
    
    if (use_random_data) {
        arr = generateRandomArray(n);
        cout << "Generated random array with " << n << " elements" << endl;
    } else {
        arr.resize(n);
        cout << "Enter " << n << " integers:" << endl;
        for (int i = 0; i < n; i++) {
            cin >> arr[i];
        }
    }
    
    cout << "\nOriginal array: ";
    printArray(arr);
    
    if (choice == 1 || choice == 3) {
        cout << "\n===== Bubble Sort =====" << endl;
        
        // For Bubble Sort
        vector<int> bubble_arr_copy = arr;
        vector<int> bubble_arr = arr;
        
        // Parallel Bubble Sort
        auto start = chrono::high_resolution_clock::now();
        parallelBubbleSort(bubble_arr);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> parallel_bubble_time = end - start;
        
        cout << "Sorted array (parallel bubble): ";
        printArray(bubble_arr);
        cout << "Parallel Bubble Sort execution time: " << fixed << setprecision(6) 
             << parallel_bubble_time.count() << " seconds" << endl;
        cout << "Correctly sorted: " << (verifySorted(bubble_arr) ? "Yes" : "No") << endl;
        
        // Sequential Bubble Sort
        start = chrono::high_resolution_clock::now();
        sequentialBubbleSort(bubble_arr_copy);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> sequential_bubble_time = end - start;
        
        cout << "Sequential Bubble Sort execution time: " << fixed << setprecision(6) 
             << sequential_bubble_time.count() << " seconds" << endl;
        
        if (parallel_bubble_time.count() > 0) {
            cout << "Bubble Sort Speedup: " << fixed << setprecision(2) 
                 << (sequential_bubble_time.count() / parallel_bubble_time.count()) << "x" << endl;
        } else {
            cout << "Bubble Sort Speedup: N/A (time too small to measure)" << endl;
        }
    }
    
    if (choice == 2 || choice == 3) {
        cout << "\n===== Merge Sort =====" << endl;
        
        // For Merge Sort
        vector<int> merge_arr_copy = arr;
        vector<int> merge_arr = arr;
        
        // Parallel Merge Sort
        auto start = chrono::high_resolution_clock::now();
        parallelMergeSortWrapper(merge_arr);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> parallel_merge_time = end - start;
        
        cout << "Sorted array (parallel merge): ";
        printArray(merge_arr);
        cout << "Parallel Merge Sort execution time: " << fixed << setprecision(6) 
             << parallel_merge_time.count() << " seconds" << endl;
        cout << "Correctly sorted: " << (verifySorted(merge_arr) ? "Yes" : "No") << endl;
        
        // Sequential Merge Sort
        start = chrono::high_resolution_clock::now();
        sequentialMergeSort(merge_arr_copy, 0, merge_arr_copy.size() - 1);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> sequential_merge_time = end - start;
        
        cout << "Sequential Merge Sort execution time: " << fixed << setprecision(6) 
             << sequential_merge_time.count() << " seconds" << endl;
        
        if (parallel_merge_time.count() > 0) {
            cout << "Merge Sort Speedup: " << fixed << setprecision(2) 
                 << (sequential_merge_time.count() / parallel_merge_time.count()) << "x" << endl;
        } else {
            cout << "Merge Sort Speedup: N/A (time too small to measure)" << endl;
        }
    }
    
    if (choice == 3) {
        cout << "\n===== Performance Comparison =====" << endl;
        cout << "For array size " << n << ":" << endl;
        
        // Calculate best algorithm based on performance
        auto bubble_time = chrono::duration<double>(0);
        auto merge_time = chrono::duration<double>(0);
        
        // Time parallel bubble sort
        vector<int> test_arr = arr;
        auto start = chrono::high_resolution_clock::now();
        parallelBubbleSort(test_arr);
        auto end = chrono::high_resolution_clock::now();
        bubble_time = end - start;
        
        // Time parallel merge sort
        test_arr = arr;
        start = chrono::high_resolution_clock::now();
        parallelMergeSortWrapper(test_arr);
        end = chrono::high_resolution_clock::now();
        merge_time = end - start;
        
        cout << "Parallel Bubble Sort: " << fixed << setprecision(6) << bubble_time.count() << " seconds" << endl;
        cout << "Parallel Merge Sort:  " << fixed << setprecision(6) << merge_time.count() << " seconds" << endl;
        
        if (bubble_time.count() < merge_time.count()) {
            cout << "Bubble Sort was faster by " << fixed << setprecision(2) 
                 << (merge_time.count() / bubble_time.count()) << "x" << endl;
        } else if (merge_time.count() < bubble_time.count()) {
            cout << "Merge Sort was faster by " << fixed << setprecision(2) 
                 << (bubble_time.count() / merge_time.count()) << "x" << endl;
        } else {
            cout << "Both algorithms performed similarly" << endl;
        }
        
        cout << "\nNOTE: For larger arrays, Merge Sort is generally more efficient than Bubble Sort," << endl;
        cout << "      but for very small arrays, Bubble Sort might be faster due to less overhead." << endl;
    }
    
    return 0;
} 