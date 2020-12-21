#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
using namespace std;
struct Stack
{
    char op;
    struct Stack *next;
};
using Stack = struct Stack;
Stack *createStack()
{
    Stack *top = (Stack *)calloc(1, sizeof(Stack));
    top=NULL;
    return top;
}
int isEmpty(Stack *st)
{
    if (st == NULL)
        return 1;
    return 0;
}
char peek(Stack *st)
{
    return st->op;
}
char pop(Stack **st)
{
    char ret = (*st)->op;
    (*st) = (*st)->next;
    return ret;
}
void push(Stack **st, char op)
{
    Stack *temp = (Stack *)calloc(1, sizeof(Stack));
    temp->op = op;
    temp->next = *st;
    (*st) = temp;
}
int prec(char op)
{
    if (op == '(' || op == ')')
        return 3;
    if (op == '%' || op == '/' || op == '*')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    assert(false);
}
int precedence(char op1, char op2)
{
    return (prec(op1) >= prec(op2)) ? 1 : 0;
}
int main()
{
    ofstream fout;
    ifstream fin;
    fin.open("input.txt");
    char c;
    Stack * top=createStack();
    while (fin)
    {      
        fout.open("part1-output.txt",ios::app);
        fin.read(&c,sizeof(c));
        cout<<c;  
        if (c == '\n')
        {
            // cerr<<"hi"<<endl;
            while (!isEmpty(top))
            {
                fout << pop(&top) << " ";
            }
            fout.write(&c,sizeof(c));
        }
        else
        {
            int x = 0;
            while (c == ' ')
                fin.read(&c,sizeof(c));
            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
            {
                while (!isEmpty(top) && peek(top) != '(')
                {
                    if (precedence(peek(top),c))
                    {
                        char temp=pop(&top);
                        fout.write(&temp,sizeof(temp));
                        temp=' ';
                        fout.write(&temp,sizeof(temp));
                    }
                    else
                    {
                        break;
                    }
                }
                push(&top, c);
            }
            else if (c == '(')
            {
                push(&top, c);
            }
            else if (c == ')')
            {
                while (peek(top) != '(')
                {
                    char temp=pop(&top);
                    fout.write(&temp,sizeof(temp));
                    temp=' ';
                    fout.write(&temp,sizeof(temp));
                }
                pop(&top);
            }
            else
            {
                while (c >= '0' && c <= '9')
                {
                    x = 10 * x + (c - '0');
                     fin.read(&c,sizeof(c));
                }
                fout.write((char*)&x,sizeof(x));
                if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '(' || c == ')')
                {

                }
            }
        }
        fout.close();

    }
    fin.close();
}