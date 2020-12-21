// Animesh Jha
// 19CS10070
#include <iostream>
#include <assert.h>
using namespace std;
#define MAX_SIZE 100
// #warning doesnt handle tie breaks via jobId implement it!!!
typedef struct _job
{
    int jobId;
    int startTime;
    int jobLength;
    int remLength;
} job;
typedef struct _jobPair
{
    int jobid_from;
    int jobid_to;
} jobpair;
typedef struct _heap
{
    job list[MAX_SIZE];
    int idx[MAX_SIZE];
    int numJobs;
} newheap;
int priority(job a, job b) // returns true if a should be the parent of b
{
    if (a.remLength < b.remLength)
        return 1;
    else if (a.remLength == b.remLength and a.jobId < b.jobId)
        return 1;
    else
        return 0;
}
void initHeap(newheap *H) // initializes the heap pointed to by H (just sets numJobs to 0)
{
    H->numJobs = 0;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        H->idx[i] = -1;
    }
}
void fixDown(newheap *H, int i) //fixes heap property below
{
    int mx_idx = i;
    int l_idx = 2 * i;
    int r_idx = l_idx + 1;
    if (l_idx <= H->numJobs and priority(H->list[l_idx], H->list[mx_idx]))
        mx_idx = l_idx;
    if (r_idx <= H->numJobs and priority(H->list[r_idx], H->list[mx_idx]))
        mx_idx = r_idx;

    if (i ^ mx_idx)
    {
        job temp = H->list[mx_idx];
        H->list[mx_idx] = H->list[i];
        H->list[i] = temp;
        H->idx[H->list[i].jobId] = i;
        H->idx[H->list[mx_idx].jobId] = mx_idx;
        fixDown(H, mx_idx);
    }
}
void insertJob(newheap *H, job j) // inserts the job j in the heap pointed to by H
{
    assert(H->numJobs < MAX_SIZE);
    H->numJobs++;
    int i = H->numJobs;
    H->list[i] = j;
    H->idx[j.jobId] = i;
    while (i > 1 and priority(H->list[i], H->list[i / 2]))
    {
        job temp = H->list[i / 2];
        H->list[i / 2] = H->list[i];
        H->list[i] = temp;
        H->idx[H->list[i].jobId] = i;
        H->idx[H->list[i / 2].jobId] = i / 2;
        i >>= 1;
    }
}
int extractMinJob(newheap *H, job *j) // If the heap is empty, returns -1; Otherwise deletes the minimum element from the heap, sets *j to it, and returns 0
{
    if (H->numJobs)
    {
        *j = H->list[1];
        H->list[1] = H->list[H->numJobs];
        H->idx[j->jobId] = -1;
        H->numJobs--;
        fixDown(H, 1);
        return 0;
    }
    else
    {
        return -1;
    }
}
void decreaseKey(newheap *H, int i)
{
    H->list[i].remLength /= 2;
    H->list[i].jobLength /= 2;
    while (i > 1 and priority(H->list[i], H->list[i / 2]))
    {
        job temp = H->list[i / 2];
        H->list[i / 2] = H->list[i];
        H->list[i] = temp;
        H->idx[H->list[i].jobId] = i;
        H->idx[H->list[i / 2].jobId] = i / 2;
        i >>= 1;
    }
}
void counting_sort(job jobList[], int n)
{
    job *temp = new job[n];
    int mx = -1e9;
    for (int i = 0; i < n; i++)
        mx = (mx < jobList[i].startTime) ? jobList[i].startTime : mx;
    // cerr << "max: " << mx << endl;
    int *ct = new int[1 + mx];
    for (int i = 0; i <= mx; i++)
        ct[i] = 0;
    for (int i = 0; i < n; i++)
    {
        ct[jobList[i].startTime]++;
    }
    for (int i = 1; i <= mx; i++)
    {
        ct[i] += ct[i - 1];
    }
    for (int i = 0; i < n; i++)
    {
        temp[ct[jobList[i].startTime] - 1] = jobList[i];
        ct[jobList[i].startTime]--;
    }
    for (int i = 0; i < n; i++)
    {
        jobList[i] = temp[i];
        // cerr << jobList[i].jobId << " " << jobList[i].startTime << " " << jobList[i].jobLength << " " << jobList[i].remLength << endl;
    }
}
void newscheduler(job jobList[], jobpair pairList[], int n, int m)
{
    counting_sort(jobList, n);
    /*******turn pairlist into an adjlist******/
    int *ct = new int[n + 1];
    int *to_k = new int[n + 1];
    for (int i = 0; i <= n; i++)
    {
        ct[i] = 0;
        to_k[i] = 0;
    }
    for (int i = 0; i < m; i++)
    {
        ct[pairList[i].jobid_from]++;
    }
    int **adjList = new int *[n + 1];
    for (int i = 0; i <= n; i++)
    {
        adjList[i] = new int[ct[i]];
    }
    for (int i = 0; i < m; i++)
    {
        int u = pairList[i].jobid_from;
        int v = pairList[i].jobid_to;
        // cerr<<u<<" "<<v<<" "<<to_k[u]<<endl;
        adjList[u][to_k[u]] = v;
        to_k[u]++;
        assert(to_k[u] <= ct[u]);
    }
    /******************************************/
    int *started = new int[n + 1];
    int *future = new int[n + 1];
    for (int i = 1; i <= n; i++)
    {
        started[i] = 0;
        future[i] = 0;
    }
    int time = 0;
    int turn = 0;
    int k = 0;
    newheap H;
    initHeap(&H);
    cout << "Jobs scheduled at each timestep are:" << endl;
    while (!(k >= n and H.numJobs == 0))
    {
        while (k < n and jobList[k].startTime <= time)
        {
            int local = 0;
            while (local < future[jobList[k].jobId])
            {
                jobList[k].jobLength /= 2;
                jobList[k].remLength /= 2;
                local++;
            }
            insertJob(&H, jobList[k]);
            k++;
        }
        if (H.numJobs)
        {
            job mn;
            int x = extractMinJob(&H, &mn);
            assert(!x);
            if (!started[mn.jobId]) //job starting for the first time
            {
                turn += (time - mn.startTime);
                started[mn.jobId] = 1;
            }
            cout << mn.jobId << " ";
            mn.remLength--;
            if (mn.remLength > 0)
                insertJob(&H, mn);
            else if (!mn.remLength)
            {
                for (int i = to_k[mn.jobId] - 1; i >= 0; i--)
                {
                    if (!started[adjList[mn.jobId][i]])
                    {
                        if (~H.idx[adjList[mn.jobId][i]])
                            decreaseKey(&H, H.idx[adjList[mn.jobId][i]]);
                        else
                            future[adjList[mn.jobId][i]]++;
                    }
                }
            }
        }
        else
            cout << "-1 ";
        time++;
    }
    cout << endl;
    cout << "Average turn around time was: " << static_cast<double>(turn) / n << endl;
}
int main()
{
    int n;
    cout << "Enter no. of jobs (n):";
    cin >> n;
    cout << "Enter the jobs:" << endl;
    job *jobList = new job[n];
    for (int i = 0; i < n; i++)
    {
        cin >> jobList[i].jobId >> jobList[i].startTime >> jobList[i].jobLength;
        jobList[i].remLength = jobList[i].jobLength;
    }
    int m;
    cout << "Enter the number of dependency pairs\n";
    cin >> m;
    jobpair *pairList = new jobpair[m];
    cout << "Enter the dependency pairs" << endl;
    for (int i = 0; i < m; i++)
    {
        cin >> pairList[i].jobid_from >> pairList[i].jobid_to;
    }
    newscheduler(jobList, pairList, n, m);
    return 0;
}