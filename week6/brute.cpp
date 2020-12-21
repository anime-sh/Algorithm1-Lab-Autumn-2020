#include <bits/stdc++.h> 
using namespace std; 
vector<int> arr;
void findWays( int i, int n) 
{ 
    if (n == 0) 
        return;
  for (int j = i; j <= n; j++) { 
        arr.push_back(j); 
        findWays(j, n - j); 
        arr.pop_back(); 
    } 
} 
int main() 
{ 
    int n,C; 
    cin>>n>>C;
    float e[n][C];
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<C;j++)
        {
            cin>>e[i][j];
        }
    }

    findWays(1, C-n); 
  
    return 0; 
} 
