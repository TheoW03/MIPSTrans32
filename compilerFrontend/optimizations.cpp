#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
#include "../MipsTarget/MipsTarget.h"
// #include "../compilerFrontend/parser.h"

using namespace std;

/**
 * @brief does the optimization where it has
 *
 * a = 1 + 1;
 * it checks if its "pure" else it sums it for u
 *
 * @param op
 * @return int
 */
int check_if_pureExpression(Node *op)
{
    if (op == nullptr)
    {
        return 0;
    }
    if (instanceof <varaibleNode *>(op))
    {

        cout << "var \n";
        return 1;
    }
    int a = check_if_pureExpression(op->left);
    if (a == 0)
    {
        check_if_pureExpression(op->right);
    }
}

float constant_prop_float(Node *op)
{
    if (op == nullptr)
    {
        return 1;
    }
    // FloatNode *pd2 = dynamic_cast<FloatNode *>(op);

    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);

        return (int)stoi(pd->num);
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        return (float)stof(pd->num) / OFFSET;
    }
    if (instanceof <operatorNode *>(op))
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            return constant_prop_float(op->right) + constant_prop_float(op->left);
        }
        if (t == type::SUBTRACT)
        {
            return constant_prop_float(op->right) - constant_prop_float(op->left);
        }
        if (t == type::MULTIPLY)
        {
            return constant_prop_float(op->right) * constant_prop_float(op->left);
        }
        if (t == type::DIVISION)
        {
            return constant_prop_float(op->right) / constant_prop_float(op->left);
        }
        if (t == type::MOD)
        {
            return ((int)constant_prop_float(op->right) % (int)constant_prop_float(op->left));
        }
    }
}
int constant_prop_integer(Node *op)
{
    if (op == nullptr)
    {
        return 1;
    }

    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);

        return (int)stoi(pd->num);
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        return (int)stoi(pd->num) / OFFSET;
    }

    if (instanceof <operatorNode *>(op))
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            return constant_prop_integer(op->right) + constant_prop_integer(op->left);
        }
        if (t == type::SUBTRACT)
        {
            return constant_prop_integer(op->right) - constant_prop_integer(op->left);
        }
        if (t == type::MULTIPLY)
        {
            return constant_prop_integer(op->right) * constant_prop_integer(op->left);
        }
        if (t == type::DIVISION)
        {
            return constant_prop_integer(op->right) / constant_prop_integer(op->left);
        }
        if (t == type::MOD)
        {
            return ((int)constant_prop_integer(op->right) % (int)constant_prop_integer(op->left));
        }
    }
    return 0;
}