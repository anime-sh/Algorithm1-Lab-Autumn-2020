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

typedef struct _heap
{
    job list[MAX_SIZE];
    int numJobs;
} heap;
int priority(job a, job b) // returns true if a should be the parent of b
{
    if (a.remLength < b.remLength)
        return 1;
    else if (a.remLength == b.remLength and a.jobId < b.jobId)
        return 1;
    else
        return 0;
}
void initHeap(heap *H) // initializes the heap pointed to by H (just sets numJobs to 0)
{
    H->numJobs = 0;
}
void fixDown(heap *H, int i) //fixes heap property below
{
    int mx_idx = i;
    int l_idx = 2 * i;
    int r_idx = l_idx + 1;

    // if (l_idx <= H->numJobs and H->list[l_idx].remLength < H->list[mx_idx].remLength)
    //     mx_idx = l_idx;
    // if (r_idx <= H->numJobs and H->list[r_idx].remLength < H->list[mx_idx].remLength)
    //     mx_idx = r_idx;

    if (l_idx <= H->numJobs and priority(H->list[l_idx], H->list[mx_idx]))
        mx_idx = l_idx;
    if (r_idx <= H->numJobs and priority(H->list[r_idx], H->list[mx_idx]))
        mx_idx = r_idx;

    if (i ^ mx_idx)
    {
        job temp = H->list[mx_idx];
        H->list[mx_idx] = H->list[i];
        H->list[i] = temp;
        fixDown(H, mx_idx);
    }
}
void insertJob(heap *H, job j) // inserts the job j in the heap pointed to by H
{
    assert(H->numJobs < MAX_SIZE);
    H->numJobs++;
    int i = H->numJobs;
    H->list[i] = j;

    // while (i > 1 and H->list[i / 2].remLength > H->list[i].remLength)
    while (i > 1 and priority(H->list[i], H->list[i / 2]))
    {
        job temp = H->list[i / 2];
        H->list[i / 2] = H->list[i];
        H->list[i] = temp;
        i >>= 1;
    }
}
int extractMinJob(heap *H, job *j) // If the heap is empty, returns -1; Otherwise deletes the minimum element from the heap, sets *j to it, and returns 0
{
    if (H->numJobs)
    {
        *j = H->list[1];
        H->list[1] = H->list[H->numJobs];
        H->numJobs--;
        fixDown(H, 1);
        return 0;
    }
    else
    {
        return -1;
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
void scheduler(job jobList[], int n)
{
    counting_sort(jobList, n);
    int time = 0;
    int turn = 0;
    int k = 0;
    heap H;
    initHeap(&H);
    cout << "Jobs scheduled at each timestep are:" << endl;
    while (!(k >= n and H.numJobs == 0))
    {
        while (k < n and jobList[k].startTime <= time)
        {
            insertJob(&H, jobList[k]);
            k++;
        }
        // cerr<<"Heap status @ time = "<<time<<endl;
        // for(int i=1;i<=H.numJobs;i++)
        //     cerr<<H.list[i].jobId<<" "<<H.list[i].startTime<<"  "<<H.list[i].remLength<<endl;
        if (H.numJobs)
        {
            job mn;
            int x = extractMinJob(&H, &mn);
            assert(!x);
            // cerr<<"Min selected = "<<mn.jobId<<"  "<<mn.remLength<<endl;
            if (mn.remLength == mn.jobLength) //job starting for the first time
            {
                turn += (time - mn.startTime);
            }
            cout << mn.jobId << " ";
            mn.remLength--;
            if (mn.remLength > 0)
            {
                insertJob(&H, mn);
            }
        }
        else
        {
            cout << "-1 ";
        }
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
    scheduler(jobList, n);
    return 0;
}