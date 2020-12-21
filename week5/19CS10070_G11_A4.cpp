#include <iostream>
#include <fstream>
#include <string.h>
#include <assert.h>

using namespace std;
struct symbol
{
	char sym;
	int freq;
};
bool priority(char a, char b)
{
	int pa = 0;
	if (a >= '0' and a <= '9')
	{
		pa = 26 + a - '0';
	}
	else
	{
		pa = a - 'a';
	}
	int pb = 0;
	if (b >= '0' and b <= '9')
	{
		pb = 26 + b - '0';
	}
	else
	{
		pb = b - 'a';
	}
	return pa < pb;
}
class Node
{
public:
	char *sym;
	int freq;
	Node *next;
	Node *left;
	Node *right;

	Node(char s[], int f)
	{
		this->freq = f;

		int i = 0;
		while (s[i] != '\0')
			i++;
		this->sym = new char[i + 1];
		i = 0;
		while (s[i] != '\0')
		{
			this->sym[i] = s[i];
			i++;
		}
		this->sym[i] = '\0';

		this->next = NULL;
		this->left = NULL;
		this->right = NULL;
	}
	void insert_normal(Node *x)
	{
		Node *temp = this;
		while ((temp->next) != NULL)
		{
			temp = temp->next;
		}
		Node *n = temp->next;
		temp->next = x;
		x->next = n;
	}
	int pty(Node a) //returns true if Node a is of higher priority than *(this)
	{
		int self_len = 0;
		while (this->sym[self_len] != '\0')
			self_len++;
		int a_len = 0;
		while (a.sym[a_len] != '\0')
			a_len++;
		if (a_len < self_len)
			return 1;
		if (self_len > a_len)
			return 0;
		if (a_len == 1)
		{
			return priority(char(this->sym[0]), char(a.sym[0]));
		}
		// NXYZ form ke hai dono
		for (int i = 1; i < a_len; i++)
		{
			if (a.sym[i] > this->sym[i])
				return 0;
			else if (a.sym[i] < this->sym[i])
				return 1;
		}
		assert(0);
	}
	Node *insert(Node *supe)
	{
		Node *prev = this;
		Node *temp = this;
		while (temp != NULL && supe->freq > temp->freq)
		{
			prev = temp;
			temp = temp->next;
		}
		while (temp != NULL and (supe->freq) == (temp->freq) and !supe->pty(*temp))
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == this)
		{
			supe->next = this;
			return supe;
		}
		prev->next = supe;
		supe->next = temp;
		return this;
	}
	Node *get_super(int x)
	{
		Node *l1 = this;
		Node *r1 = this->next;
		// l1->next = NULL;
		// r1->next = NULL;
		int num_of_digits = 0;
		int cp = x;
		while (cp)
		{
			num_of_digits++;
			cp /= 10;
		}
		num_of_digits++;
		char *sym1 = new char[num_of_digits + 1];
		sym1[0] = 'N';
		sym1[num_of_digits--] = '\0';
		while (x)
		{
			assert(num_of_digits > 0);
			sym1[num_of_digits--] = '0' + (x % 10);
			x /= 10;
		}
		Node *supe = new Node(sym1, l1->freq + r1->freq);
		supe->left = l1;
		supe->right = r1;
		supe->next = NULL;
		return supe;
	}
	void print()
	{
		Node *temp = this;
		while (temp != NULL)
		{
			cout << temp->sym << " " << temp->freq << endl;
			temp = temp->next;
		}
	}
};

void build_freq(symbol freq[])
{
	fstream log;
	log.open("log.txt");
	int num;
	log >> num;
	char c[100];
	for (int i = 0; i < num; i++)
	{
		log >> c;
		int j = 0;
		while (c[j] != '\0')
		{
			char lo = c[j++];
			if (lo >= 'A' and lo <= 'Z') //convert to lower case
				lo = lo + 'a' - 'A';
			if (lo >= 'a' and lo <= 'z')
				freq[lo - 'a'].freq++;
			else if (lo >= '0' and lo <= '9')
				freq[26 + lo - '0'].freq++;
			else
				assert(0);
		}
	}
}

void sort(symbol freq[], int n = 36)
{
	symbol temp;
	for (int i = 0; i < n - 1; i++)
	{
		int mn_ind = i;
		for (int j = i + 1; j < n; j++)
		{
			if (freq[j].freq < freq[mn_ind].freq)
				mn_ind = j;
			else if (freq[j].freq == freq[mn_ind].freq and priority(freq[j].sym, freq[mn_ind].sym))
				mn_ind = j;
		}
		temp = freq[mn_ind];
		freq[mn_ind] = freq[i];
		freq[i] = temp;
	}
}
Node *to_linked_list(symbol freq[], int n = 36)
{
	char *lo = new char[2];
	lo[0] = freq[0].sym;
	lo[1] = '\0';
	Node *front = new Node(lo, freq[0].freq);

	for (int i = 1; i < n; i++)
	{
		char *temp = new char[2];
		temp[0] = freq[i].sym;
		temp[1] = '\0';
		Node *t = new Node(temp, freq[i].freq);
		front->insert_normal(t);
	}
	front->print();
	return front;
}
Node *build_huffman(Node *root)
{
	int x = 1;
	while (root->next != NULL)
	{
		Node *super = root->get_super(x++);
		root = root->next->next;
		if (root)
			root = root->insert(super);
		else
		{
			root = super;
			break;
		}
	}
	return root;
}
ofstream out;
void treetraversal(Node *root)
{
	if (root == NULL)
		return;
	out << root->sym << "(" << root->freq << "), ";
	treetraversal(root->left);
	treetraversal(root->right);
}
void processHC(Node *root, char path[], char HC[][500])
{
	if (root == NULL)
		return;
	if (root->left or root->right)
	{

		char p1[500];
		char p2[500];
		int i = 0;
		while (path[i] != '\0')
		{
			p1[i] = path[i];
			i++;
		}
		p1[i++] = '0';
		p1[i] = '\0';
		processHC(root->left, p1, HC);
		i = 0;
		while (path[i] != '\0')
		{
			p2[i] = path[i];
			i++;
		}
		p2[i++] = '1';
		p2[i] = '\0';
		processHC(root->right, p2, HC);
	}
	else
	{
		int pres;
		if (root->sym[0] <= 'z' && root->sym[0] >= 'a')
		{
			pres = root->sym[0] - 'a';
		}
		else
		{
			pres = root->sym[0] - '0' + 26;
		}
		int i = 0;
		while (path[i] != '\0')
		{
			HC[pres][i] = path[i];
			i++;
		}
		HC[pres][i]='\0';
	}
}
int main()
{
	symbol freq[36];
	out.open("output.txt");

	for (int i = 0; i < 26; i++)
	{
		freq[i].sym = static_cast<char>('a' + i);
		freq[i].freq = 0;
	}
	for (int i = 26; i < 36; i++)
	{
		freq[i].sym = static_cast<char>('0' + i - 26);
		freq[i].freq = 0;
	}
	build_freq(freq);
	out << "Frequency distribution: " << endl;
	for (int i = 0; i < 36; i++)
	{
		out << freq[i].sym << " = " << freq[i].freq << endl;
	}
	sort(freq);
	Node *front = to_linked_list(freq);
	char HCODES[36][500];
	char path[500];
	Node *root = build_huffman(front);
	treetraversal(root);
	out << endl;
	processHC(root, path, HCODES);
	for (int i = 0; i < 36; i++)
	{
		if (i < 26)
			out << char('a' + i) << " ";
		else
			out << i - 26 << " ";
		out << HCODES[i] << endl;
	}
	return 0;
}