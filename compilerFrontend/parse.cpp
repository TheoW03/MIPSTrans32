#include <iostream>
#include <string>
#include <vector>
#include "../compilerFrontend/Lexxer.h"

using namespace std;

#pragma region Node

enum status
{
    N_NULL,
    NUM,
    OPERATOR,
    NODE
};
struct Node
{
    virtual ~Node();

    struct Node *left;
    struct Node *right;
    int value;
    status s = status::NODE;
};
struct varaibleNode : public Node
{
    Node *expression;
    Tokens *varailbe;
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
struct varNode : public Node
{
    struct Tokens *name;
    struct Node *value;
};
struct FunctionNode : public Node
{
    struct Tokens *nameOfFunction;
    vector<Tokens *> params;
    vector<Node *> statements;
};
#pragma endregion
Node *expression(vector<Tokens> &tokens);
bool isNull(Node *n)
{
    return n->s == status::N_NULL;
}
void setNull(Node *n)
{
    n->s = status::N_NULL;
}
// Node parse(vector<Tokens> tokens){
//     return expression(tokens);
// }
Tokens *current = new Tokens;
Tokens *matchAndRemove(vector<Tokens> &tokens, type typeT)
{
    if (tokens.empty())
    {
        return nullptr;
    }
    cout << "passed: " + tokens[0].dictionary[typeT];
    cout << "\n";
    cout << "id: " + tokens[0].dictionary[tokens[0].id];
    cout << "\n";
    cout << "buf: " + tokens[0].buffer + " \n";
    if (tokens[0].id == typeT)
    {

        Tokens *t = new Tokens(tokens[0]);
        current = t;
        tokens.erase(tokens.begin());
        return t;
    }
    // cout << "unmatched \n";
    return nullptr;
}

Node *factor(vector<Tokens> &tokens)
{
    Tokens *a = new Tokens;
    a = (matchAndRemove(tokens, type::NUMBER) != nullptr) ? current : (matchAndRemove(tokens, type::OP_PARENTHISIS) != nullptr) ? current
                                                                  : (matchAndRemove(tokens, type::WORD) != nullptr)             ? current
                                                                                                                                : nullptr;
    type id;
    if (a != nullptr)
    {
        id = a->id;
    }
    else
    {
        return nullptr;
    }
    if (id == type::NUMBER)
    {
        NumNode *numN = new NumNode;
        string b = a->buffer;
        // cout << "b: " + b << endl;
        numN->num = b;
        // cout << "numN: "+numN->num << endl;
        delete a;
        return numN;
    }
    else if (id == type::OP_PARENTHISIS)
    {
        Node *exp = expression(tokens);
        matchAndRemove(tokens, type::CL_PARENTHISIS);
        cout << "op \n";

        return exp;
    }
    else if (id == type::WORD)
    {
        // NumNode *numN = new NumNode;
        // string b = a->buffer;
        // // cout << "b: " + b << endl;
        // numN->num = b;
        // // cout << "numN: "+numN->num << endl;
        // delete a;
        // return numN;
        return nullptr;
    }

    else
    {
        return nullptr;
    }
    // do stuff
}

Node *term(vector<Tokens> &tokens)
{

    // n.value = 0;
    Node *n;
    Node *opNode;
    opNode = factor(tokens);
    Tokens *op = (matchAndRemove(tokens, type::MULTIPLY) != nullptr) ? current : (matchAndRemove(tokens, type::DIVISION) != nullptr) ? current
                                                                             : (matchAndRemove(tokens, type::MOD) != nullptr)        ? current
                                                                                                                                     : nullptr; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::MULTIPLY) != nullptr) ? current : (matchAndRemove(tokens, type::DIVISION) != nullptr) ? current
                                                                                 : (matchAndRemove(tokens, type::MOD) != nullptr)        ? current
                                                                                                                                         : nullptr; // n.value = 0;
            }
            if (op == nullptr)
            {
                return opNode;
            }
            Node *right = factor(tokens);
            operatorNode *n = new operatorNode;
            n->left = opNode;
            n->right = right;
            n->token = op;
            opNode = n;
            node = opNode;
        }
    }
    return opNode;
    // do stuff
}

Node *expression(vector<Tokens> &tokens)
{
    Node *n;
    Node *opNode = term(tokens);
    Tokens *op = (matchAndRemove(tokens, type::ADDITION) != nullptr) ? current : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
                                                                                                                                     : NULL; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::ADDITION) != nullptr) ? current : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
                                                                                                                                         : NULL; // n.value = 0;
            }
            if (op == nullptr)
            {
                if (opNode == nullptr)
                {
                    // cout << "opNode nulll \n";
                }
                return opNode;
            }
            Node *right = term(tokens);
            operatorNode *n = new operatorNode;
            n->left = opNode;
            n->right = right;
            n->token = op;
            opNode = n;
            node = opNode;
        }
    }
    if (opNode == nullptr)
    {
        // cout << "opNode nulll \n";
    }
    return opNode;
    // do stuff
}

// will parse functions
Node *handleFunctions(vector<Tokens> &tokens)
{
    FunctionNode *f = new FunctionNode;
    matchAndRemove(tokens, type::FUNCTION);
    Tokens *name = matchAndRemove(tokens, type::WORD);
    matchAndRemove(tokens, type::OP_PARENTHISIS);

    vector<Tokens *> vars;
    while (matchAndRemove(tokens, type::CL_PARENTHISIS) == nullptr)
    {
        Tokens *var = matchAndRemove(tokens, type::WORD);
        matchAndRemove(tokens, type::COMMA);
        vars.push_back(var);
    }
    f->nameOfFunction = name;
    f->params = vars;
    return f;
}
void printParams(vector<Tokens *> a)
{
    cout << "params" << endl;
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i]->dictionary[a[i]->id] + "(" + a[i]->buffer + ") \n";
    }
}
Node *testParse(vector<Tokens> &tokens)
{
    FunctionNode *f = static_cast<FunctionNode *>(handleFunctions(tokens));
    printParams(f->params);
    return f;
}
void RemoveEOLS(vector<Tokens> &list)
{
    while (true)
    {
        Tokens *e = matchAndRemove(list, type::END_OF_LINE);

        if (e == nullptr)
        {
            return;
        }
    }
}
Node *parseVar(vector<Tokens> &tokens, Tokens *name)
{
    matchAndRemove(tokens, type::EQUALS);
    varaibleNode *n;
    n->expression = expression(tokens);
    RemoveEOLS(tokens);
    n->varailbe = name;
    return n;
}

/**
 * @brief function stuff
 *
 * @param tokens
 * @return Node*
 */

Node *functionParse(vector<Tokens> &tokens)
{
    printList(tokens);
    FunctionNode *f;
    vector<Node *> states;
    if (matchAndRemove(tokens, type::FUNCTION) != nullptr)
    {
        Node *func = handleFunctions(tokens);
        while (matchAndRemove(tokens, type::END) != nullptr)
        {
            Tokens *a = matchAndRemove(tokens, type::WORD);
            if (a != nullptr)
            {
                Node *var = parseVar(tokens, a);
                states.push_back(var);
            }
        }
        FunctionNode *pd = dynamic_cast<FunctionNode *>(func);
        if (pd != nullptr)
        {
            pd->statements = states;
        }
        return pd;
    }
    return nullptr;
}

/**
 * meant for testing Recursion stuff
 */
Node *parse(vector<Tokens> &tokens)
{

    Tokens *var = matchAndRemove(tokens, type::WORD);
    matchAndRemove(tokens, type::EQUALS);
    Node *a = expression(tokens);
    varaibleNode *c = new varaibleNode;
    c->expression = a;
    c->varailbe = var;

    if (a == nullptr)
    {
        // cout << "null \n";
        return nullptr;
    }
    return a;
}