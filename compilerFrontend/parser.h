#include <iostream>
#include <string>
#include <vector>
#include "../compilerFrontend/Lexxer.h"
using namespace std;

#ifndef STAT_H
#define STAT_H
enum status
{
    N_NULL,
    NUM,
    OPERATOR
};
#endif
#ifndef NODE_H
#define NODE_H

struct Node
{
    struct Node *left;
    struct Node *right;
    status s;
    virtual ~Node();
};

struct NullStruct : public Node
{
};

struct NumNode : public Node
{
    string num;
};

struct operatorNode : public Node
{
    struct Tokens *token;
};
struct StatementNode : public Node
{
    struct Node *expression;
    struct Tokens *nameOfVar;
};
struct varaibleNode : public Node
{
    Node *expression;
    Tokens *varailbe;
};
struct FunctionNode:public Node
{
    struct Tokens* nameOfFunction;
    vector<Tokens *> params;
    vector<Node *> statements;
};
#endif // NODE_H

Node *parse(vector<Tokens> &tokens);

Node *term(vector<Tokens> &tokens);
Node *factor(vector<Tokens> &tokens);

Node *expression(vector<Tokens> &tokens);

Tokens *matchAndRemove(vector<Tokens> &tokens, type type);
bool isNull(Node *n);
Node* handleFunctions(vector<Tokens> &tokens);
void printParams(vector<Tokens*> a);
Node *testParse(vector<Tokens> &tokens);
Node* functionParse(vector<Tokens> &tokens);
Node *parseVar(vector<Tokens> &tokens, Tokens *name);