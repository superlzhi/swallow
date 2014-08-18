#include "Attribute.h"
USE_SWIFT_NS


Attribute::Attribute()
    :Node(NodeType::Attribute)
{
}

void Attribute::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& Attribute::getName()const
{
    return name;
}
void Attribute::addBalancedToken(const std::wstring& token)
{
    balancedTokens.push_back(token);
}