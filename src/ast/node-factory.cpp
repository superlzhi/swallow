#include "node-factory.h"
#include "ast/ast.h"
USE_SWIFT_NS;

CommentNode* NodeFactory::createComment(const std::wstring& comment)
{
    return new CommentNode(comment);
}
BooleanLiteral* NodeFactory::createBoolean(bool value)
{
    return new BooleanLiteral(value);
}
IntegerLiteral* NodeFactory::createInteger(const std::wstring& value)
{
    return new IntegerLiteral(value);
}
FloatLiteral* NodeFactory::createFloat(const std::wstring& value)
{
    return new FloatLiteral(value);
}
StringLiteral* NodeFactory::createString(const std::wstring& value)
{
    return new StringLiteral(value);
}
UnaryOperator* NodeFactory::createUnary(const std::wstring& op, OperatorType::T type)
{
    UnaryOperator* ret = new UnaryOperator(op, type);
    return ret;
}
Identifier* NodeFactory::createIdentifier(const std::wstring& op)
{
    return new Identifier(op);
}
InOutParameter* NodeFactory::createInOutParameter(Identifier* identifier)
{
    InOutParameter* ret = new InOutParameter(identifier);
    return ret;
}
BinaryOperator* NodeFactory::createBinary(const std::wstring& op, Associativity::T associativity, int precedence)
{
    BinaryOperator* ret = new BinaryOperator(op, associativity, precedence);
    return ret;
}

ArrayLiteral* NodeFactory::createArrayLiteral()
{
    return new ArrayLiteral();
}
DictionaryLiteral* NodeFactory::createDictionaryLiteral()
{
    return new DictionaryLiteral();
}
CompileConstant* NodeFactory::createCompilecConstant(const std::wstring& name, const std::wstring& value)
{
    return new CompileConstant(name, value);
}

MemberAccess* NodeFactory::createMemberAccess(ExpressionNode* self, Identifier* field)
{
    return new MemberAccess(self, field);
}
Subscript* NodeFactory::createSubscript(ExpressionNode* self, ExpressionNode* index)
{
    return new Subscript(self, index);
}

TypeCheck* NodeFactory::createTypeCheck(ExpressionNode* expr, TypeNode* type)
{
    return new TypeCheck(expr, type);
}
TypeCast* NodeFactory::createTypeCast(ExpressionNode* expr, TypeNode* type)
{
    return new TypeCast(expr, type);
}
Assignment* NodeFactory::createAssignment(ExpressionNode* lhs, ExpressionNode* rhs)
{
    return new Assignment(lhs, rhs);
}
ConditionalOperator* NodeFactory::createConditionalOperator(ExpressionNode* cond, ExpressionNode* trueExpr, ExpressionNode* falseExpr)
{
    return new ConditionalOperator(cond, trueExpr, falseExpr);
}
ParenthesizedExpression* NodeFactory::createParenthesizedExpression()
{
    return new ParenthesizedExpression();
}

Initializer* NodeFactory::createInitializer(ExpressionNode* expr)
{
    return new Initializer(expr);
}
SelfExpression* NodeFactory::createSelfExpression(ExpressionNode* expr)
{
    return new SelfExpression(expr);
}
DynamicType* NodeFactory::createDynamicType(ExpressionNode* expr)
{
    return new DynamicType(expr);
}
ForcedValue* NodeFactory::createForcedValue(ExpressionNode* expr)
{
    return new ForcedValue(expr);
}
OptionalChaining* NodeFactory::createOptionalChaining(ExpressionNode* expr)
{
    return new OptionalChaining(expr);
}
FunctionCall* NodeFactory::createFunctionCall()
{
    return new FunctionCall();
}