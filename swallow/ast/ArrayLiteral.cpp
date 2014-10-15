#include "ArrayLiteral.h"
#include "NodeVisitor.h"
USE_SWIFT_NS

ArrayLiteral::ArrayLiteral()
    :Expression(NodeType::ArrayLiteral)
{

}

ArrayLiteral::~ArrayLiteral()
{
//    SafeDeleteAll(elements);
}
void ArrayLiteral::push(const ExpressionPtr& item)
{
	elements.push_back(item);
}
ExpressionPtr ArrayLiteral::getElement(int i)
{
    return elements[i];
}
int ArrayLiteral::numElements()const
{
    return elements.size();
}

void ArrayLiteral::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitArrayLiteral);
}
std::vector<ExpressionPtr>::iterator ArrayLiteral::begin()
{
    return elements.begin();
}
std::vector<ExpressionPtr>::iterator ArrayLiteral::end()
{
    return elements.end();
}