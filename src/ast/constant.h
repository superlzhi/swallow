#ifndef CONSTANT_H
#define CONSTANT_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern;
class Expression;
class Constant : public Declaration
{
public:
    Constant();
    ~Constant();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
    
    void add(Pattern* pattern, Expression* initializer);
    int numPairs();
    std::pair<Pattern*, Expression*> getPair(int i);
public:
    virtual void serialize(std::wostream& out);
public://Node
    virtual void accept(NodeVisitor* visitor);
public:
    std::vector<std::pair<Pattern*, Expression*> > pairs;
};

SWIFT_NS_END

#endif//CONSTANT_H
