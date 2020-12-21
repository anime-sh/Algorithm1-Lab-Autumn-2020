#include <iostream>
#include <fstream>
#include <assert.h>
std::ifstream fin;
std::ofstream fout;
struct field
{
	int data;
	field *next;
	field(int t = -1)
	{
		data = t;
		next = NULL;
	}
};
struct _hashing
{
	int key;
	struct _hashing *next;
	field *vals;
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
					// assert(0);
				}
				else
				{
					//check for repetition here
					bool repeat = false;
					hash *head_of_counterparty = chain->next;
					field *old = head_of_counterparty->vals;
					while (old != NULL)
					{
						if (old->data == portfolio)
						{
							repeat = true;
							break;
						}
						old = old->next;
					}
					if (repeat)
						continue;
					head_of_counterparty->vals = new field(portfolio);
					head_of_counterparty->vals->next = head_of_counterparty->vals;
				}
			}
			else
			{
				chain->next = new _hashing(counter_party);
				hash *head_of_counterparty = chain->next;
				field *old = head_of_counterparty->vals;
				head_of_counterparty->vals = new field(portfolio);
				head_of_counterparty->vals->next = old;
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
					hash *head_of_portfolio = chain->next;
					field *old = head_of_portfolio->vals;
					head_of_portfolio->vals = new field(counter_party);
					head_of_portfolio->vals->next = old;
				}
			}
			else
			{
				chain->next = new _hashing(portfolio);
				hash *head_of_portfolio = chain->next;
				field *old = head_of_portfolio->vals;
				head_of_portfolio->vals = new field(counter_party);
				head_of_portfolio->vals->next = old;
			}
		}
		else if (type == '-')
		{
			fin >> counter_party;
			int idx = key(counter_party, K);
			//delete counter party i from C and then get list of portfolios from which i has to be deleted
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
				chain->next = temp->next;  //deleted
				field *ports = temp->vals; //pointer to LL of linked lists to be deleted
				while (ports != NULL)
				{
					//Find ports->data
					int lo_idx = key(ports->data, K);
					hash *lo_chain1 = p + lo_idx;
					hash *lo_chain2 = lo_chain1->next;
					while (lo_chain2 != NULL and lo_chain2->key != ports->data)
					{
						lo_chain1 = lo_chain1;
						lo_chain2 = lo_chain2->next;
					}
					if (lo_chain2 == NULL or lo_chain2->key != ports->data)
					{
						std::cerr << "Yeh kya hogaya ?!!>" << std::endl;
					}
					else
					{
						field *lo_temp = lo_chain2->vals;
						if (lo_temp->data == counter_party)
						{
							std::cerr << "Completely deleted  " << lo_chain2->key << std::endl;
							lo_chain2->vals = lo_temp->next;
							if (lo_chain2->vals == NULL) //portfolio completely deleted
							{
								lo_chain1->next = lo_chain2->next;
							}
						}
						else
						{
							field *prev = lo_chain2->vals;
							lo_temp = lo_temp->next;
							while (lo_temp != NULL and lo_temp->data != counter_party)
							{
								prev = lo_temp;
								lo_temp = lo_temp->next;
							}
							if (lo_temp == NULL or lo_temp->data != counter_party)
							{
								std::cerr << "Yeh kya hogaya 2.0 ??" << std::endl;
							}
							else
							{
								prev->next = lo_temp->next; //next
							}
						}
					}
					//traverse
					ports = ports->next;
				}
			}
		}
		else
		{
			std::cerr << "Error: operator type\n";
			std::cerr << type << std::endl;
			// assert(0);
		}
	}
	/******DEBUG**********/
	// fout << "Printing P\n";
	// for (int i = 0; i < K; i++)
	// {
	// 	fout << "Index " << i << "\n";
	// 	auto chain = p[i].next;
	// 	while (chain != NULL)
	// 	{
	// 		fout << "Portfolio = " << chain->key;
	// 		fout << ", counter parties = ";
	// 		auto start = chain->vals;
	// 		while (start)
	// 		{
	// 			fout << start->data << " ";
	// 			start = start->next;
	// 		}
	// 		fout << "........";
	// 		chain = chain->next;
	// 	}
	// 	fout << "\n";
	// }
	// fout << "Printing C\n";
	// for (int i = 0; i < K; i++)
	// {
	// 	fout << "Index " << i << "\n";
	// 	auto chain = c[i].next;
	// 	while (chain != NULL)
	// 	{
	// 		fout << "Counter Party = " << chain->key;
	// 		fout << ", portfolios = ";
	// 		auto start = chain->vals;
	// 		while (start)
	// 		{
	// 			fout << start->data << " ";
	// 			start = start->next;
	// 		}
	// 		fout << "........";
	// 		chain = chain->next;
	// 	}
	// 	fout << "\n";
	// }
	/******DEBUG**********/
	for (int i = 0; i < K; i++)
	{
		auto chain = p[i].next;
		if (!chain)
			fout << "p " << i << " -1 -1\n";
		while (chain)
		{
			auto start = chain->vals;
			if (start == NULL)
			{
				fout << "p " << i << " " << chain->key << " -1\n";
			}
			while (start)
			{
				fout << "p " << i << " " << chain->key << " " << start->data << "\n";
				start = start->next;
			}
			chain = chain->next;
		}
	}
	for (int i = 0; i < K; i++)
	{
		auto chain = c[i].next;
		if (!chain)
			fout << "c " << i << " -1 -1\n";
		while (chain)
		{
			auto start = chain->vals;
			if (start == NULL)
			{
				fout << "c " << i << " " << chain->key << " -1\n";
			}
			while (start)
			{
				fout << "c " << i << " " << chain->key << " " << start->data << "\n";
				start = start->next;
			}
			chain = chain->next;
		}
	}

	fin.close();
	fout.close();
	return 0;
}