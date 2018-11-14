#include <iostream>
#include <fstream>
#include <deque> 
using namespace std;

using intq = deque<int>;

void printKMax(int arr[], int n, int k){
    intq q(k);
    
    // Go over 1st k elements. Use queue to store largest index in window.
    int i;
    for (i = 0; i < k; ++i)
    {
        while (!q.empty() && arr[i] >= arr[q.back()])
            q.pop_back(); // drop indexes with smaller values
        q.push_back(i);
    }
    
    for (; i < n; ++i)
    {
        std::cout << arr[q.front()] << " ";
        
        while (!q.empty() && q.front() <= i - k)
            q.pop_front();
        while (!q.empty() && arr[i] >= arr[q.back()])
            q.pop_back();
        q.push_back(i);
    }
    std::cout << arr[q.front()] << "\n";
}

int main(int, char** argv){
    std::ifstream fin(argv[1]);
  
	int t;
	fin >> t;
	while(t>0) {
		int n,k;
    	fin >> n >> k;
    	int i;
    	int arr[n];
    	for(i=0;i<n;i++)
      		fin >> arr[i];
    	printKMax(arr, n, k);
    	t--;
  	}
  	return 0;
}
