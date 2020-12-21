//Animesh Jha 19CS10070
#include <iostream>
// #include <assert.h>
using namespace std;
const int MX = 200;              //MX size of grid
const int dx[4] = {0, 1, 0, -1}; //to discover neighbours
const int dy[4] = {1, 0, -1, 0};
struct POINT
{
    int x, y;
};
using POINT = struct POINT;

//QUEUE DEFINITION STARTS
struct QUEUE
{
    POINT *Arr;
    int queue_size;
    int start_id;
    int end_id;
};
using QUEUE = struct QUEUE;
void init(QUEUE *qP, int size)
{
    (*qP).queue_size = size;
    (*qP).Arr = new POINT[size];
    (*qP).start_id = (*qP).end_id = -1;
}
int isempty(QUEUE qP)
{
    if (qP.start_id == -1)
    {
        // assert(qP.end_id == -1);
        return 1;
    }
    return 0;
}
void enqueue(QUEUE *qP, POINT p)
{
    (*qP).Arr[++(*qP).end_id] = p;
    if ((*qP).end_id == 0)
        (*qP).start_id = 0;
}
POINT dequeue(QUEUE *qP)
{
    POINT ret = (*qP).Arr[(*qP).start_id++];
    if ((*qP).start_id > (*qP).end_id)
    {
        (*qP).start_id = (*qP).end_id = -1;
    }
    return ret;
}
//QUEUE DEFINITION ENDS

//STACK DEFINTION STARTS
struct STACK
{
    POINT head;
    struct STACK *next;
};
using STACK = struct STACK;
void init(STACK *s)
{
    POINT Sentinel;
    Sentinel.x = -1;
    Sentinel.y = -1;
    s->head = Sentinel;
    s->next = NULL;
}
int isempty(STACK s)
{
    return (s.head.x == -1 && s.head.y == -1);
}
void push(STACK *s, POINT p)
{
    STACK *temp = new STACK;
    temp->head = s->head;
    temp->next = s->next;
    s->head = p;
    s->next = temp;
}
POINT pop(STACK *s)
{
    POINT ret = s->head;
    (*s) = (*(s->next));
    return ret;
}

// void STACKDEBUG()
// {
//     STACK s;
//     init(&s);
//     cerr<<&s<<" "<<s.head.x<<", "<<s.head.y<<" "<<s.next<<endl;
//     push(&s, {1, 2});
//     cerr<<&s<<" "<<s.head.x<<", "<<s.head.y<<" "<<s.next<<endl;
//     push(&s, {3, 4});
//     cerr<<&s<<" "<<s.head.x<<", "<<s.head.y<<" "<<s.next<<endl;
//     push(&s, {5, 6});
//     cerr<<&s<<" "<<s.head.x<<", "<<s.head.y<<" "<<s.next<<endl;
//     push(&s, {7, 8});
//     cerr<<&s<<" "<<s.head.x<<", "<<s.head.y<<" "<<s.next<<endl;
//     while (!isempty(s))
//     {
//         auto lo = pop(&s);
//         cerr << "deleted: " << lo.x << "," << lo.y << "\t";
//         cerr<<s.head.x<<", "<<s.head.y<<" "<<s.next<<endl;
//     }
//     cerr << "exit" << endl;
// }

//STACK DEFINTION ENDS

void printgrid(int **grid, int A, int B)
{
    cout << "Grid of stones is\n";
    for (int i = 0; i < A; i++)
    {
        for (int j = 0; j < B; j++)
        {
            cout << (grid[i][j] ? '-' : '*');
        }
        cout << endl;
    }
}
int strategy1(int **grid, int A, int B, POINT James, POINT Fly) //bfs from james to fly
{
    int **vis = new int *[A];
    for (int i = 0; i < A; i++)
    {
        vis[i] = new int[B];
        for (int j = 0; j < B; j++)
        {
            vis[i][j] = 0;
        }
    }
    QUEUE q;
    init(&q, A * B + 1);
    //go from James to Fly
    vis[James.x][James.y] = 1;
    enqueue(&q, James);
    while (!isempty(q))
    {
        POINT fr = dequeue(&q);
        for (int i = 0; i < 4; i++) //check all 4 neighbours
        {
            POINT u;
            u.x = fr.x + dx[i];
            u.y = fr.y + dy[i];
            if (u.x < 0 || u.y < 0 || u.x >= A || u.y >= B) //out of bounds
            {
                continue;
            }
            if (vis[u.x][u.y] == 1 || grid[u.x][u.y] == 1) // unstable rock or already visited
            {
                continue;
            }
            vis[u.x][u.y] = 1;
            enqueue(&q, u);
        }
    }
    return vis[Fly.x][Fly.y];
}
void strategy2(int **grid, int A, int B, POINT James, POINT Fly) //do a dfs from fly to james, easier to print the path this way
{
    int **vis = new int *[A];
    for (int i = 0; i < A; i++)
    {
        vis[i] = new int[B];
        for (int j = 0; j < B; j++)
        {
            vis[i][j] = 0;
        }
    }
    STACK s;
    init(&s);
    vis[Fly.x][Fly.y] = 1;
    push(&s, Fly);
    while (!isempty(s))
    {
        POINT v = pop(&s);
        if (v.x == James.x && v.y == James.y)
        {
            //print the path
            cout << "(" << v.x << ", " << v.y << "), ";
            while (s.next != NULL)
            {
                cout << "(" << s.head.x << ", " << s.head.y << "), ";
                s = *(s.next);
            }
            cout << endl;
            return;
        }
        for (int i = 0; i < 4; i++) //check the neighbours for atleast one unvisited node
        {
            POINT u;
            u.x = v.x + dx[i];
            u.y = v.y + dy[i];
            if (u.x < 0 || u.y < 0 || u.x >= A || u.y >= B) //out of bounds
            {
                continue;
            }
            if (vis[u.x][u.y] == 1 || grid[u.x][u.y] == 1) // unstable rock or already visited
            {
                continue;
            }
            push(&s, v);
            push(&s, u);
            vis[u.x][u.y] = 1;
            break;
        }
    }
}
int main()
{
    int A, B, N;
    cin >> A >> B >> N;
    int **grid = new int *[A];
    POINT *U_rocks = new POINT[N];
    POINT James, Fly;
    for (int i = 0; i < A; i++)
    {
        grid[i] = new int[B];
        for (int j = 0; j < B; j++)
        {
            grid[i][j] = 0; //0 means stable rock
        }
    }
    for (int i = 0; i < N; i++)
    {
        cin >> U_rocks[i].x;
    }
    for (int i = 0; i < N; i++)
    {
        cin >> U_rocks[i].y;
        grid[U_rocks[i].x][U_rocks[i].y] = 1;
    }
    cin >> James.x >> James.y >> Fly.x >> Fly.y;
    printgrid(grid, A, B);
    if (strategy1(grid, A, B, James, Fly))
    {
        cout << "Path exists\n";
        strategy2(grid, A, B, James, Fly);
    }
    else
    {
        cout << "No path exists\n";
    }
    // STACKDEBUG();
    return 0;
}