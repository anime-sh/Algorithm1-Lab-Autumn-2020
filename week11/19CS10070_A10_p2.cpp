// Animesh Jha
// 19CS10070
#include <iostream>
#include <fstream>
#include <assert.h>
std::ifstream fin;
std::ofstream fout;
struct hashset
{
    int *data;
    hashset(int K)
    {
        data = new int[K];
        for (int i = 0; i < K; i++)
        {
            data[i] = -1;
        }
    }
};
struct _hashing
{
    int key;
    struct _hashing *next;
    hashset *vals;
    _hashing(int t = -1)
    {
        key = t;
        next = NULL;
        vals = NULL;
    }
};
using hash = _hashing;
int key(int i, int K)
{
    return ((i % K) + K) % K;
}
int main()
{
    fin.open("input.txt");
    fout.open("output.txt");
    int K, N;
    fin >> K >> N;
    char type;
    int counter_party, portfolio;
    hash *p = new hash[K];
    hash *c = new hash[K];
    for (int i = 0; i < K; i++)
    {
        p[i].key = -1e9; //sentinel val which never appears as a port number
        c[i].key = -1e9; //sentinel val which never appears as a counter party  number
        p[i].next = NULL;
        c[i].next = NULL;
    }
    for (int i = 0; i < N; i++)
    {
        fin >> type;
        if (type == '+')
        {
            fin >> counter_party >> portfolio;
            //insert into c
            int idx = key(counter_party, K);
            hash *chain = c + idx;
            while (chain->next != NULL and chain->next->key != counter_party)
            {
                chain = (chain->next);
            }
            if (chain->next != NULL)
            {
                if (chain->next->key != counter_party)
                {
                    std::cerr << "insert into c error\n";
                }
                else
                {
                    if ((chain->next)->vals->data[key(portfolio, K)] != -1)
                    {
                        //repeated element
                        continue;
                    }
                    else
                    {
                        // std::cerr<<counter_party<<" "<<portfolio<<std::endl;
                        (chain->next)->vals->data[key(portfolio, K)] = portfolio;
                    }
                }
            }
            else
            {
                chain->next = new _hashing(counter_party);
                chain->next->vals = new hashset(K);
                (chain->next)->vals->data[key(portfolio, K)] = portfolio;
            }
            //insert into p
            idx = key(portfolio, K);
            chain = p + idx;
            while (chain->next != NULL and chain->next->key != portfolio)
            {
                chain = chain->next;
            }
            if (chain->next != NULL)
            {
                if (chain->next->key != portfolio)
                {
                    std::cerr << "insert into p error\n";
                    // assert(0);
                }
                else
                {
                    if ((chain->next)->vals->data[key(counter_party, K)] != -1)
                    {
                        //repeated element
                        continue;
                    }
                    else
                    {
                        // std::cerr<<counter_party<<" "<<portfolio<<std::endl;
                        (chain->next)->vals->data[key(counter_party, K)] = counter_party;
                    }
                }
            }
            else
            {
                chain->next = new _hashing(portfolio);
                chain->next->vals = new hashset(K);
                (chain->next)->vals->data[key(counter_party, K)] = counter_party;
            }
        }
        else if (type == '-')
        {
            fin >> counter_party;
            int idx = key(counter_party, K);
            hash *chain = c + idx;
            hash *temp = chain->next;
            while (temp != NULL and temp->key != counter_party)
            {
                chain = temp;
                temp = temp->next;
            }
            if (temp == NULL)
            {
                std::cerr << "Counter party to be deleted doesnt exist in the tables" << std::endl;
            }
            else
            {
                chain->next = temp->next; //deleted
                int* table = temp->vals->data;
                for (int i1 = 0; i1 < K; i1++)
                {
                    if (~table[i1])
                    {
                        //find and deleted table[i] from p
                        int lo_idx = key(table[i1], K);
                        hash *lo_chain = p + lo_idx;
                        while (lo_chain != NULL and lo_chain->key != table[i1])
                        {

                            lo_chain = lo_chain->next;
                        }
                        if (lo_chain == NULL or lo_chain->key != table[i1])
                        {
                            std::cerr << "Yeh kya hogaya ?!!" << std::endl;
                        }
                        else
                        {
                            lo_chain->vals->data[key(counter_party, K)] = -1;
                        }
                    }
                }
            }
        }
        else
        {
            std::cerr << "Error: operator type\n";
            std::cerr << type << std::endl;
        }
    }
    for (int j = 0; j < K; j++)
    {
        bool print = false;
        hash* chain = p[j].next;
        while (chain)
        {
            hashset* start = chain->vals;
            for (int i = 0; i < K; i++)
            {
                if (~(start->data[i]))
                {
                    fout << "p " << j << " " << chain->key << " " << start->data[i] << "\n";
                    print = true;
                }
            }
            chain = chain->next;
        }
        if (!print)
            fout << "p " << j << " -1 -1\n";
    }
    for (int j = 0; j < K; j++)
    {
        hash* chain = c[j].next;
        bool print = false;
        while (chain)
        {
            hashset* start = chain->vals;
            for (int i = 0; i < K; i++)
            {
                if (~(start->data[i]))
                {
                    fout << "c " << j << " " << chain->key << " " << start->data[i] << "\n";
                    print = true;
                }
            }
            chain = chain->next;
        }
        if (!print)
            fout << "c " << j << " -1 -1\n";
    }

    fin.close();
    fout.close();

    return 0;
}