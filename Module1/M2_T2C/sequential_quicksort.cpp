#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = (low - 1);
        
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        
        int pi = i + 1;
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    vector<int> arr = {178, 223, 121, 987, 122, 212, 563, 12, 123};
    int n = arr.size();
    
    cout << "Sequential QuickSort:" << endl;
    
    auto start = high_resolution_clock::now();
    quickSort(arr, 0, n - 1);
    auto stop = high_resolution_clock::now();
    
    cout << "Sorted array: ";
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << endl;
    
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken: " << duration.count() << " microseconds" << endl;
    
    return 0;
}
