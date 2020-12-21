// Name:Animesh Jha
// Roll Number: 19CS10070
#include <iostream>
#include <assert.h>
using namespace std;

typedef struct _node
{
    int rank;
    struct _node *parent;
} node;
struct coord
{
    int x;
    int y;
};

void makeset(node &cell)
{
    cell.parent = &cell;
    cell.rank = 0;
}
node *findset(node &v)
{
    if (v.parent == &v)
        return v.parent;
    else
    {
        node *root = findset((*(v.parent)));
        v.parent = root;
        return root;
    }
}
void mergeset(node &a, node &b)
{
    node *para = findset(a);
    node *parb = findset(b);
    if (para == parb)
        return;
    if (para->rank < parb->rank)
    {
        para->parent = parb;
    }
    else if (para->rank > parb->rank)
    {
        parb->parent = para;
    }
    else
    {
        parb->parent = para;
        para->rank++;
    }
}
int rand(int a, int b)
{
    return a + rand() % (b - a + 1);
}
void printGrid(int **hwall, int **vwall, int m, int n)
{
    //print top row
    for (int i = 0; i < n; i++)
    {
        if (!i)
        {
            cout << "+   ";
        }
        else
        {
            cout << "+---";
        }
    }
    cout << "+\n";
    //print bulk
    for (int i = 0; i < m - 1; i++)
    {
        cout << "|   ";
        for (int j = 0; j < n - 1; j++)
        {
            cout << ((vwall[i][j]) ? "|" : " ") << "   ";
        }
        cout << "|\n";
        for (int j = 0; j < n; j++)
        {
            cout << "+";
            cout << ((hwall[i][j]) ? "---" : "   ");
        }
        cout << "+\n";
    }
    //print last interior vertical
    cout << "|   ";
    for (int j = 0; j < n - 1; j++)
    {
        cout << ((vwall[m - 1][j]) ? "|" : " ") << "   ";
    }
    cout << " \n";
    //print last horizontal
    for (int i = 0; i < n; i++)
    {
        cout << "+---";
    }
    cout << "+\n";
}
void findtreasure(int m, int n, coord *H, coord *V, int **hwall, int **vwall, node **grid)
{
    int i = 0;
    int j = 0;
    for (int k = 0; k < m * n; k++)
    {
        if (i >= (m - 1) * n)
        {
            // move to suitable vwall[j]
            //union due to vwall[j]
            //j++
            while (j < (n - 1) * m)
            {
                coord local = V[j];
                if (findset(grid[local.x][local.y]) != findset(grid[local.x][local.y + 1]))
                {
                    vwall[local.x][local.y] = 0;
                    mergeset(grid[local.x][local.y], grid[local.x][local.y + 1]);
                    j++;
                    break;
                }
                else
                {
                    j++;
                }
            }
        }
        else if (j >= (n - 1) * m)
        {
            // move to suitable hwall[i]
            //union due to hwall[i]
            //i++
            while (i < (m - 1) * n)
            {
                coord local = H[i];
                if (findset(grid[local.x][local.y]) != findset(grid[local.x + 1][local.y]))
                {
                    hwall[local.x][local.y] = 0;
                    mergeset(grid[local.x][local.y], grid[local.x + 1][local.y]);
                    i++;
                    break;
                }
                else
                {
                    i++;
                }
            }
        }
        else
        {
            int type = rand(0, 1);
            if (type == 0)
            {
                // move to suitable hwall[i]
                //union due to hwall[i]
                //i++
                while (i < (m - 1) * n)
                {
                    coord local = H[i];
                    if (findset(grid[local.x][local.y]) != findset(grid[local.x + 1][local.y]))
                    {
                        hwall[local.x][local.y] = 0;
                        mergeset(grid[local.x][local.y], grid[local.x + 1][local.y]);
                        i++;
                        break;
                    }
                    else
                    {
                        i++;
                    }
                }
            }
            else
            {
                // move to suitable vwall[j]
                //union due to vwall[j]
                //j++
                while (j < (n - 1) * m)
                {
                    coord local = V[j];
                    if (findset(grid[local.x][local.y]) != findset(grid[local.x][local.y + 1]))
                    {
                        vwall[local.x][local.y] = 0;
                        mergeset(grid[local.x][local.y], grid[local.x][local.y + 1]);
                        j++;
                        break;
                    }
                    else
                    {
                        j++;
                    }
                }
            }
        }
    }
}
int main(int argc, char **argv)
{
    srand((unsigned int)time(NULL));
    int m, n;
    if (argc == 1)
    {
        m = n = 10;
    }
    else
    {
        m = atoi(argv[1]);
        n = atoi(argv[2]);
    }

    node **grid = new node *[m];
    for (int i = 0; i < m; i++)
    {
        grid[i] = new node[n];
        for (int j = 0; j < n; j++)
        {
            makeset(grid[i][j]);
        }
    }

    int **hwall = new int *[m - 1]; //horizontal walls ka situation 0 means no wall 1 means wall
    for (int i = 0; i < m - 1; i++)
    {
        hwall[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            hwall[i][j] = 0;
        }
    }
    int **vwall = new int *[m]; //vertical walls ka situation
    for (int i = 0; i < m; i++)
    {
        vwall[i] = new int[n - 1];
        for (int j = 0; j < n - 1; j++)
        {
            vwall[i][j] = 0;
        }
    }

    int **inihwall = new int *[m - 1]; //for printing initial grid waste of space tbh
    for (int i = 0; i < m - 1; i++)
    {
        inihwall[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            inihwall[i][j] = 1;
        }
    }
    int **inivwall = new int *[m]; //same as inihwall
    for (int i = 0; i < m; i++)
    {
        inivwall[i] = new int[n - 1];
        for (int j = 0; j < n - 1; j++)
        {
            inivwall[i][j] = 1;
        }
    }

    coord *H = new coord[(m - 1) * n];
    for (int i = 0; i < (m - 1) * n; i++)
    {
        int x, y;
        do
        {
            x = rand(0, m - 2);
            y = rand(0, n - 1);
        } while (hwall[x][y]);
        hwall[x][y] = 1;
        H[i].x = x;
        H[i].y = y;
    }
    coord *V = new coord[(n - 1) * m];
    for (int i = 0; i < (n - 1) * m; i++)
    {
        int x, y;
        do
        {
            x = rand(0, m - 1);
            y = rand(0, n - 2);
        } while (vwall[x][y]);
        vwall[x][y] = 1;
        V[i].x = x;
        V[i].y = y;
    }
    findtreasure(m, n, H, V, hwall, vwall, grid);
    if (findset(grid[0][0]) == findset(grid[m - 1][n - 1]))
    {
        cout << "The final chamber can be reached from the start chamber." << endl;
    }
    else
    {
        cout << "The final chamber can NOT be reached from the start chamber." << endl;
    }
    cout << "Initial Grid" << endl;
    printGrid(inihwall, inivwall, m, n);
    cout << "Final Grid" << endl;
    printGrid(hwall, vwall, m, n);
    return 0;
}