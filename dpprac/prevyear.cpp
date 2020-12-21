#include <iostream>
using namespace std;
int main()
{
    int n, k;
    cin >> n >> k;
    int dp[n + 1][k + 1] = {0};
    for (int j = 1; j <= k; j++)
    {
        for (int i = 0; i <= n; i++)
        {
            if (j == 1)
            {
                dp[i][j] = i;
            }
            else if (i < 2)
            {
                dp[i][j] = i;
            }
            else
            {
                int mn = 1e9;
                for (int x = 1; x <= i; x++)
                {
                    int lo = ((dp[x - 1][j - 1] > dp[i - x][j]) ? dp[x - 1][j - 1] : dp[i - x][j]);
                    mn = (mn > lo) ? lo : mn;
                }
                dp[i][j] = mn + 1;
            }
        }
    }
    cout << dp[n][k] << endl;
    return 0;
}