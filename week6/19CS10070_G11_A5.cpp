// Name: Animesh Jha
// RollNumber: 19CS10070
// Subproblem: Let dp[i][j] be equal to the maximum probablity of success possible for the reactions 1...i using exactly j units of catalyst
// let e(k,p) denote the probability of success of the reaction k if p units of catalyst are used in the reaction
// Recursive Formulation:
//                       if i=0 set dp[i][j] to 1
//                       else if j<=0 dp[i][j]=0
//                       other wise dp[i][j]=max(dp[i-1][j-x]+e[i][x]) overall x in 1...j

#include <fstream>
#include <iostream>
#include <assert.h>
using namespace std;

int main()
{
    ifstream fin;
    ofstream fout;
    fin.open("input.txt");
    fout.open("output.txt");
    if (!fout)
    {
        cerr << "ERR: output.txt not created" << endl;
        return 0;
    }
    if (!fin)
    {
        cerr << "ERR: input.txt not created" << endl;
        return 0;
    }

    int n, C;
    fin >> n;
    fin >> C;

    double **e = new double *[n + 1]; //creation of e it is of size (n+1)*(C+1) to allow 1 based indexing
    for (int i = 0; i <= n; i++)
        e[i] = new double[C + 1];
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            fin >> e[i][j];
            assert(e[i][j] > 0);
        }
    }
    double **dp = new double *[n + 1]; //creation of table (n+1)*(C+1) to allow 1 based indexing see above for definition
    for (int i = 0; i <= n; i++)
        dp[i] = new double[C + 1];
    for (int i = 0; i <= n; i++) //initialisation
        for (int j = 0; j <= C; j++)
            dp[i][j] = 0;
    for (int i = 0; i <= C; i++) //setting the base case of i=0
        dp[0][i] = 1;

    int **par = new int *[n + 1]; //creation of table (n+1)*(C+1) to recover solution
    for (int i = 0; i <= n; i++)
        par[i] = new int[C + 1];
    for (int i = 0; i <= n; i++) //initialisation
        for (int j = 0; j <= C; j++)
            par[i][j] = -1;

    cout << endl;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= C; j++)
        {
            for (int x = 1; x <= j; x++)
            {
                double lo = dp[i - 1][j - x] * e[i][x];
                if (lo > dp[i][j])
                {
                    dp[i][j] = lo;
                    par[i][j] = x;
                }
            }
        }
    }

    fout << dp[n][C] << endl;
    int *solution = new int[n + 1];
    int j = n;
    int nlo = n;
    int clo = C;
    while (j > 0)
    {
        assert(nlo > 0 and clo > 0);
        solution[j] = par[nlo][clo];
        nlo--;
        clo -= solution[j];
        j--;
    }
    for (int i = 1; i <= n; i++)
    {
        fout << "reaction " << i << " : " << solution[i] << endl;
    }
    return 0;
}