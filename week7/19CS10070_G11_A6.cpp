// Name: Animesh Jha
// Roll Number: 19CS10070
//  logic: the function p(x) =  it is possible to schedule visits in a given order such that the minimum gap is >= x is a monotonic function
//         it is false for high values and true for low values, we can find the largest point where the function is true by binary search
//          do this for all visit combinations

#include <iostream>
#include <fstream>
// #include <assert.h>
using namespace std;
typedef struct
{
    int start;
    int end;
} interval;
// void merge(interval a[], int lo, int hi)
// {
//     interval temp[hi - lo + 1];
//     for (int i = lo; i <= hi; i++)
//     {
//         temp[i - lo] = a[i];
//     }
//     int mid = (lo + hi) / 2;
//     int i = 0;
//     int j = mid + 1 - lo;
//     int k = lo;
//     while (i <= mid - lo and j <= hi - lo)
//     {
//         if (temp[i].end > temp[j].end)
//         {
//             a[k++] = temp[i];
//             i++;
//         }
//         else if (temp[i].end == temp[j].end and temp[i].start >= temp[j].start)
//         {
//             a[k++] = temp[i];
//             i++;
//         }
//         else
//         {
//             a[k++] = temp[j];
//             j++;
//         }
//     }
//     while (i <= mid - lo)
//     {
//         a[k++] = temp[i++];
//     }
//     while (j <= hi - lo)
//     {
//         a[k++] = temp[j++];
//     }
// }
// void sort(interval a[], int lo, int hi)
// {
//     if (lo == hi)
//         return;
//     int mid = (lo + hi) / 2;
//     sort(a, lo, mid);
//     sort(a, mid + 1, hi);
//     merge(a, lo, hi);
// }
bool is_pos(interval schedule[], int length, int order[], int n)
{
    int prev = schedule[order[0]].start;
    for (int i = 1; i < n; i++)
    {
        int mn_next = schedule[order[i]].start;
        int mx_next = schedule[order[i]].end;
        if (mx_next - prev < length)
            return false;
        int next = (mn_next > prev + length) ? mn_next : prev + length;
        prev = next;
    }
    return true;
}
int permute(int order[], int n)
{
    int i = n - 1;
    while (i > 0 and order[i - 1] >= order[i])
    {
        i--;
    }
    if (i <= 0)
        return 0;
    int j = n - 1;
    while (order[j] <= order[i - 1])
    {
        j--;
    }
    int lo = order[i - 1];
    order[i - 1] = order[j];
    order[j] = lo;
    int k = n - 1;
    while (i < k)
    {
        int lo = order[i];
        order[i] = order[k];
        order[k] = lo;
        i++;
        k--;
    }
    return 1;
}
int *schedule_visits(interval schedule[], int n, int &ans)
{
    int *order = new int[n];
    for (int i = 0; i < n; i++)
    {
        order[i] = i;
    }
    int *best = new int[n];
    int mx_dist = 0;
    //check all perms
    do
    {
        // cerr<<"Order is: \n";
        // for(int i=0;i<n;i++)
        //     cerr<<order[i]<<" ";
        // cerr<<endl;
        int lo = 0;
        int hi = 1500*60;
        int local_ans = 0;
        while (lo <= hi) //binary search to find max gap
        {
            int mid = lo +(hi-lo)/ 2;
            if (is_pos(schedule, mid, order, n))
            {
                lo = mid + 1;
                if (local_ans < mid)
                {
                    local_ans = mid;
                }
            }
            else
            {
                hi = mid - 1;
            }
        }
        if (local_ans > mx_dist)
        {
            mx_dist = local_ans;
            for (int i = 0; i < n; i++)
            {
                best[i] = order[i];
            }
        }
    } while (permute(order, n));
    // cerr<<"Max gap is "<<mx_dist<<endl;
    // cerr<<"Best order is \n";
    // for(int i=0;i<n;i++)
    //     cerr<<best[i]<<" ";
    ans = mx_dist;
    return best;
}

int main()
{
    ifstream fin;
    fin.open("input1.txt");
    ofstream fout;
    fout.open("output1.txt");

    int n;
    fin >> n;
    interval *schedule = new interval[n];
    for (int i = 0; i < n; i++)
    {
        fin >> schedule[i].start >> schedule[i].end;
        schedule[i].start *= 60; //convert into seconds
        schedule[i].end *= 60;   //convert into seconds
        // cerr << schedule[i].start << " " << schedule[i].end << endl;
    }
    int ans = 0;
    int *order = schedule_visits(schedule, n, ans);
    fout << ans / 60 << ":";
    if ((ans % 60) / 10)
        fout << ans % 60 << endl;
    else
        fout << "0" << ans % 60 << endl;
    for (int i = 0; i < n; i++)
        fout << order[i] << " ";
    fout << endl;
    fout.close();
    fin.close();
    return 0;
}