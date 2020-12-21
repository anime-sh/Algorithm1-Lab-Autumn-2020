// Animesh Jha
// 19CS10070
// time complexity of original part O(lg(max intial elevation)*(N^2))
// algo used: binary search over time to check for connectivity
// least time dijkstra (Bonus part): instead of distance, minimise the maxtime of the nodes on a path used to reach a node U
//                                   so instead of (dis[v]+weight<dis[u]) dis[u]=dis[v]+weight , we will set mn_time(u)=max(grid(u),mn_time(v)), where grid(u) is the inital elevation of u
#include <iostream>
#include <assert.h>
using namespace std;
const int INF = 1e9;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};
struct coord
{
    int x, y;
};
void dfs(int x, int y, int **grid, int **vis, coord **par, int n, int time, int px = -1, int py = -1)
{
    if (grid[x][y] > time)
        return;
    vis[x][y] = 1;
    par[x][y].x = px;
    par[x][y].y = py;
    for (int i = 0; i < 4; i++)
    {
        int ux = x + dx[i];
        int uy = y + dy[i];
        if (ux < 0 or ux >= n or uy < 0 or uy >= n)
            continue;
        if (vis[ux][uy])
            continue;
        if (grid[ux][uy] <= time)
            dfs(ux, uy, grid, vis, par, n, time, x, y);
    }
}
void leastTimeDijkstra(int Sx, int Sy, int Fx, int Fy, int **grid, int **vis, coord **par, int n)
{
    int **mn_time = new int *[n];
    for (int i = 0; i < n; i++)
    {
        mn_time[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            mn_time[i][j] = INF;
            vis[i][j] = 0;
            // par[i][j].x = -1;
            // par[i][j].y = -1;
        }
    }
    mn_time[Sx][Sy] = grid[Sx][Sy];
    while (!vis[Fx][Fy])
    {
        coord v = {-1, -1};
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (!vis[i][j] and (v.x == -1 or mn_time[v.x][v.y] > mn_time[i][j]))
                {
                    v.x = i;
                    v.y = j;
                }
            }
        }
        // cerr << v.x << " " << v.y << endl;
        if (mn_time[v.x][v.y] == INF)
            break;
        vis[v.x][v.y] = 1;
        for (int i = 0; i < 4; i++)
        {
            int ux = v.x + dx[i];
            int uy = v.y + dy[i];
            if (ux < 0 or ux >= n or uy < 0 or uy >= n)
                continue;
            int nmn = mn_time[v.x][v.y];
            if (nmn < grid[ux][uy])
                nmn = grid[ux][uy];
            if (nmn < mn_time[ux][uy])
            {
                mn_time[ux][uy] = nmn;
            }
        }
    }
    cout << mn_time[Fx][Fy] << endl;
}
int main()
{
    int n;
    cin >> n;
    int **grid = new int *[n];
    coord **par = new coord *[n];
    int **vis = new int *[n];
    int mxtime = -INF;
    for (int i = 0; i < n; i++)
    {
        grid[i] = new int[n];
        par[i] = new coord[n];
        vis[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            cin >> grid[i][j];
            if (grid[i][j] > mxtime)
                mxtime = grid[i][j];
        }
    }
    int Sx, Sy, Fx, Fy;
    cin >> Sx >> Sy >> Fx >> Fy;
    int lo = 0;
    int hi = mxtime + 1;
    int ans = hi;
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                vis[i][j] = 0;
                par[i][j].x = -1;
                par[i][j].y = -1;
            }
        }
        dfs(Sx, Sy, grid, vis, par, n, mid);
        // cerr << mid << endl;
        // cerr << "vis log" << endl;
        // for (int i = 0; i < n; i++)
        // {
        //     for (int j = 0; j < n; j++)
        //         cerr << vis[i][j] << " ";
        //     cerr << endl;
        // }
        if (vis[Fx][Fy])
        {
            ans = (ans > mid) ? mid : ans;
            hi = mid - 1;
        }
        else
        {
            lo = mid + 1;
        }
    }
    cout << "Minimum time taken is: " << ans << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            vis[i][j] = 0;
            par[i][j].x = -1;
            par[i][j].y = -1;
        }
    }
    dfs(Sx, Sy, grid, vis, par, n, ans); //to trace the path
    // cerr << "par log" << endl;
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //         cerr <<"( " <<par[i][j].x << ", "<<par[i][j].y<<") ";
    //     cerr << endl;
    // }
    int tx = Fx;
    int ty = Fy;
    int num = 0;
    while (tx != -1 and ty != -1) //while parents not {-1,-1}
    {
        // cerr << tx << " " << ty << endl;
        num++;
        int temp = tx;
        tx = par[tx][ty].x;
        ty = par[temp][ty].y;
    }
    coord *path = new coord[num];
    int z = num - 1;
    tx = Fx;
    ty = Fy;
    while (tx != -1 and ty != -1 and z >= 0)
    {
        path[z].x = tx;
        path[z].y = ty;
        z--;
        int temp = tx;
        tx = par[tx][ty].x;
        ty = par[temp][ty].y;
    }
    cout << "The path taken to reach ( " << Fx << ", " << Fy << ") from ( " << Sx << ", " << Sy << " ) is: " << endl;
    for (int i = 0; i < num; i++)
    {
        cout << "( " << path[i].x << " , " << path[i].y << "), ";
    }
    cout << endl;
    cout << "Number of blocks: " << num << endl;
    cout << "leastTimeDijkstra: " << endl;
    leastTimeDijkstra(Sx, Sy, Fx, Fy, grid, vis, par, n);
    return 0;
}