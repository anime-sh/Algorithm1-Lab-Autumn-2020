#include <iostream>
#include<stdlib.h>
// using std::cerr;
using std::cin;
using std::cout;
using std::endl;

struct node
{
	int data;
	struct node *add;
};
typedef struct node node;

void traverse_from_front_to_end(node *head)
{
	cout << "Doubly linked list traversed from front to end: ";
	node *prev = NULL;
	node *next;
	while (head != NULL)
	{
		cout << head->data;
		next = (node *)((unsigned long long)prev ^ (unsigned long long)head->add);
		prev = head;
		head = next;
		if (head != NULL)
		{
			cout << ", ";
		}
	}
	cout << endl;
}

void traverse_from_end_to_front(node *tail)
{
	cout << "Doubly linked list traversed from end to front: ";
	node *next = NULL;
	node *prev;
	while (tail != NULL)
	{
		cout << tail->data;
		prev = (node *)((unsigned long long)next ^ (unsigned long long)tail->add);
		next = tail;
		tail = prev;
		if (tail != NULL)
		{
			cout << ", ";
		}
	}
	cout << endl;
}

void reverse(node **head, node **tail) //swaping head and tail should suffice as (next ^ prev) = (prev ^ next)
{
	node *temp = *head;
	*head = *tail;
	*tail = temp;
}

void alternate(node **head, node **tail)   //function inserts the tail node between two nodes denoted by left and leftnext and then updates tail, set left=leftnext and continues
{
	node *left = *head;
	node *leftprev = NULL;
	while (left != NULL)
	{
		// cerr<<left->data<<" ";
		node *leftnext = (node *)((unsigned long long)leftprev ^ (unsigned long long)left->add);	//element after left
		node *tailprev = (node *)((unsigned long long)(NULL) ^ (unsigned long long)((*tail)->add)); //element one before the tail

		if (leftnext == NULL || leftnext == (*tail)) //left is the last element or the second last element
		{
			break;
		}

		// cerr<<"a";
		leftnext->add = (node *)((unsigned long long)(leftnext->add) ^ (unsigned long long)left); //delink left and leftnext

		// cerr<<"b";
		left->add = (node *)((unsigned long long)leftprev ^ (unsigned long long)(*tail)); //update xor of left (make a connection with present tail)
		(*tail)->add = (node *)((unsigned long long)left ^ (unsigned long long)leftnext); //update xor of tail (insert between left and leftnext)

		// cerr<<"c";
		tailprev->add = (node *)((unsigned long long)(tailprev->add) ^ (unsigned long long)(*tail)); //remove (tail) as the next of tail prev
		leftnext->add = (node *)((unsigned long long)leftnext->add ^ (unsigned long long)(*tail));	 //make the link (tail ) and leftnext;

		// cerr<<"addr: "<<left<<" "<<leftnext<<" "<<*tail<<" "<<tailprev<<endl;

		leftprev = (*tail); //update leftprev
		left = leftnext;	//update left (skip by 1 types)
		*tail = tailprev;	//update tail

		// cerr<<"done"<<endl;
	}
}

int main()
{
	node *head, *tail;
	head = tail = NULL;
	int n;
	cout << "n = ";
	cin >> n;
	cout << "\nEnter " << n << " integers between -100 and 100:\n";
	for (int i = 0; i < n; i++)
	{
		node *p = (node *)calloc(sizeof(node), 1);
		cin >> p->data;
		// cerr<<i<<" "<<p<<endl;
		if (i == 0)
		{
			p->add = NULL;
			head = p;
			tail = p;
		}
		else
		{
			p->add = (node *)((unsigned long long)tail ^ (unsigned long long)NULL);
			tail->add = (node *)((unsigned long long)tail->add ^ (unsigned long long)p);
			tail = p;
		}
	}

	traverse_from_front_to_end(head);
	traverse_from_end_to_front(tail);
	
	reverse(&head, &tail);	
	cout << "Reversed ";
	traverse_from_front_to_end(head);
	// traverse_from_end_to_front(tail);
	
	alternate(&head, &tail);
	cout<<"Alternated ";
	traverse_from_front_to_end(head);
	// traverse_from_end_to_front(tail);

	return 0;
}