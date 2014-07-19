#ifndef PROGRAM_H
#define PROGRAM_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN

class Statement;
class Program : public Node
{
public:
    Program();
    ~Program();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);


    void addStatement(Statement* statement);
    int numStatements()const;
    Statement* getStatement(int n);
private:
    std::vector<Statement*> statements;
};

SWIFT_NS_END

#endif//PROGRAM_H
