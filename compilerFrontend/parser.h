#include <iostream>
#include <string>
#include <vector>
#include "../compilerFrontend/Lexxer.h"
using namespace std;

#define OFFSET 65536

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
    Node() : left(nullptr), right(nullptr) {}
};
struct VaraibleReference : public Node
{
    Node *expression;
    Tokens *varailbe;
};

struct funcCallNode : public Node
{
    Tokens *funcCall;
    vector<Node *> params;
};
struct NullStruct : public Node
{
};

struct NumNode : public Node
{
    string num;
};
struct FloatNode : public Node
{
    string num;
};

struct IntegerNode : public Node
{
    string num;
};
struct stringNode : public Node
{
    string stringBuffer;
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
    Tokens *typeOfVar;

    int size;
};
struct FunctionNode : public Node
{
    struct Tokens *nameOfFunction;
    vector<varaibleNode *> params;
    vector<Node *> statements;
};
#endif // NODE_H

Node *parse(vector<Tokens> &tokens);

Node *term(vector<Tokens> &tokens);
Node *factor(vector<Tokens> &tokens);

Node *expression(vector<Tokens> &tokens);

Tokens *matchAndRemove(vector<Tokens> &tokens, type type, string caller);
bool isNull(Node *n);
Node *handleFunctions(vector<Tokens> &tokens);
void printParams(vector<Tokens *> a);
Node *testParse(vector<Tokens> &tokens);

Node *functionParse(vector<Tokens> &tokens);
Node *parseVar(vector<Tokens> &tokens, Tokens *name);
Node *handleCalls(vector<Tokens> &tokens, Tokens *checkIfFunct);
Node *handleSatements(vector<Tokens> &tokens);
Node *testExpressionParse(vector<Tokens> &tokens);