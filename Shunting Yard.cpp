// Shunting Yard.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Rewrites expression using the shunting yard algorithm in infix, prefix, or postfix form 
// Author: Pranav Sharma
// Date: 12/03/20

#include <iostream>
#include <cstring>
#include <stack>
using namespace std;
#include "BNode.h"
// basic struct for node thats used for stack and queue to create the postfix expression
struct Node {
    char data;
    Node* next;
};
// functions used in the program
void printList(Node* start, char output[], Node*& front, Node*& rear);
bool isOperator(char c);
void createPost(char* input, Node*& top, Node* &front, Node* &rear);
int getPrecedence(char c);
void infix(BNode* head);
void prefix(BNode* head);
void postfix(BNode* head);
BNode* constructTree(char output[]);


// stack functions
void push(char value, Node*& top);
void pop(Node*& top);
int peek(Node*& top);


// queue functions
void enqueue(char value, Node*& front, Node*& rear);
void dequeue(Node*& front, Node*& rear);

int main()
{
    // top of the stack
    Node* top = NULL;
    // pointers in the queue
    Node* front = NULL;
    Node* rear = NULL;

    // variables used to capture input and output(postfix expression)
    char* input = new char();
    char output[100];
    memset(output, '\0', 100);
    int ans = 0;
    cout << "In this program we use binary trees and the shunting yard algorithm to evaluate a mathematical expression\n";
    cout << "Enter a mathematical expresion using spacing between each character\n";
    cin.getline(input, 80, '\n');
    // once expression is captured, its created into postfix form
    createPost(input, top, front, rear);
    cout << "Great! The postfix notation for this equation is:\n";
    // prints the output
    printList(front, output, front, rear);
    cout << output;
    bool playing = true;
    // creates a binary expression tree based on the postfix expression
    BNode* head = constructTree(output);

    // prints different forms while user still wants to 
    while (playing) {
        cout << "\nWhat would you like to convert this into? Type 1, 2, 3, or 4 (1 - Infix, 2 - prefix, 3 - postfix, 4 - QUIT)\n ";
        cin.getline(input, 80, '\n');
        if (strcmp(input, "1") == 0) {
            infix(head);
        }
        else if (strcmp(input, "2") == 0) {
            prefix(head);
        }
        else if (strcmp(input, "3") == 0) {
            postfix(head);
        }
        else {
            cout << "Alrighty! See you!\n";
            playing = false;
        }
    }

}
// takes the head of the binary expression tree and prints the postfix expression
void postfix(BNode* head) {
    if (head) {
        postfix(head->left);
        postfix(head->right);
        cout << head->data << " ";
    }
}
// takes the head of the binary expression tree and prints the prefix expression
void prefix(BNode* head) {
    if (head) {
        cout << head->data << " ";
        prefix(head->left);
        prefix(head->right);
    }
}
// takes the head of the binary expression tree and prints the infix expression
void infix(BNode* head) {
    if (head) {
        infix(head->left);
        cout << head->data << " ";
        infix(head->right);
    }
}
// creates the binary expression tree based on the output, got idea from geeksforgeeks.org
BNode* constructTree(char output[]) {
    stack<BNode*> stack;// used a preexisting stack for this
    BNode* head;
    BNode* right;
    BNode* left;

    // assigns each char int he output
    for (int i = 0; i < strlen(output); i++) {
        char c = output[i];
        if (c != ' ') {
            // if the char is not an operator it gets pushed ot the stack
            if (!isOperator(c)) {
                head = new BNode(c);
                stack.push(head);
            }
            // if char is an operator its put into the binary expression tree making all the parent nodes operators
            else {
                // tree branch is created
                head = new BNode(c);
                right = stack.top();
                stack.pop();
                left = stack.top();
                stack.pop();

                // assigns node
                head->right = right;
                head->left = left;

                // adds the head to the stack
                stack.push(head);

            }
        }
    }
    head = stack.top();
    stack.pop();
    // returns the head of the binary tree
    return head;
}
// creates the postfix expression from the initial infix given
void createPost(char* input, Node* &top, Node* &front, Node* &rear) { // shunting yard algorithm found on wikipedia
    for (int c = 0; input[c] != '\0'; c++) {
        char i = input[c];
        if (i != ' ') {
            if (isOperator(i)) {// if char is an operator it gets pushed into the queue
                if (top != NULL) {
                    // orders precedence of the operators
                    while (top != NULL && getPrecedence(top->data) > getPrecedence(i) || getPrecedence(top->data) == getPrecedence(i) && top->data != '(') {
                        enqueue(top->data, front, rear);
                        pop(top);
                        if (top == NULL) {
                            break;
                        }
                    }
                }
                push(i, top);
            }
            else if (i == '(') {// left parenthesis go into the stack
                push(i, top);
            }
            else if (i == ')') {// right paranethesis empties into stack until left parenthesis is found
                while (top->data != '(') {
                    enqueue(top->data, front, rear);
                    pop(top);
                }
                if (top->data == '(') {
                    pop(top);
                } 
            }
            //numbers are put into the queue
            else {
                enqueue(i, front, rear);
            }
        }
    }
    while (top != NULL) {// push anything left to the queue
        enqueue(top->data, front, rear);
        pop(top);
    }

}
// returns the precedence of each operator
int getPrecedence(char c) {
    if (c == '^') {
        return 4;
    }
    else if (c == '*' || c == '/') {
        return 3;
    }
    else if (c == '+' || c == '-') {
        return 2;
    }
    else {
        return 0;
    }
}
// checks if a character is an operator 
bool isOperator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
        return true;
    }
    else { return false; }
}
// prints the queue while assigning it to an output cstring
void printList(Node* start, char output[], Node* &front, Node* &rear) {
    Node* ptr = front;
    int i = 0;
    while (ptr != NULL) {
        output[i] = ptr->data;
        output[i + 1] = ' ';
        i += 2;
        ptr = ptr->next;
        dequeue(front, rear);
    }
    
}
// pushes a char to the stack
void push(char value, Node* &top) {
    Node* node = new Node();
    node->data = value;
    node->next = top;
    top = node;
}
// removes the top of the stack
void pop(Node* &top) {
    Node* temp = top;
    if (temp == NULL) {
        cout << "Stack is empty :(";
    }
    else {
        top = top->next;
        delete temp;
    }
}
// returns the top of the stack
int peek(Node* &top) {
    return top->data;
}

// adds to the queue
void enqueue(char value, Node* &front, Node* &rear) {
    Node* node = new Node();
    node->data = value;

    if (rear == NULL) {
        front = node;
        rear = node;
    }
    else {
        rear->next = node;
        rear = node;
    }
}
// removes the firs element of the queue
void dequeue(Node*& front, Node*& rear) {
    if (front == NULL) {
        return;
    }
    Node* temp = front;
    front = front->next;
    if (front == NULL) {
        rear = NULL;
    }
    delete temp;
}