#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H
#include "SemanticNodeVisitor.h"
#include "Type.h"

SWIFT_NS_BEGIN

class SymbolRegistry;
class CompilerResults;
class TypeDeclaration;
class Expression;
class Pattern;
class SemanticAnalyzer : public SemanticNodeVisitor
{
public:
    SemanticAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitAssignment(const AssignmentPtr& node) override;
    virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    virtual void visitValueBindings(const ValueBindingsPtr& node) override;
    virtual void visitTypeAlias(const TypeAliasPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitFunction(const FunctionDefPtr& node) override;
    virtual void visitSubscript(const SubscriptDefPtr& node);
    virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitIdentifier(const IdentifierPtr& id) override;
    virtual void visitEnumCasePattern(const EnumCasePatternPtr& node) override;
    virtual void visitParameter(const ParameterPtr& node) override;
    virtual void visitDeinit(const DeinitializerDefPtr& node) override;
    virtual void visitInit(const InitializerDefPtr& node) override;
    virtual void visitParameters(const ParametersPtr& node) override;
public:

    virtual void visitFunctionCall(const FunctionCallPtr& node) override;
    virtual void visitOperator(const OperatorDefPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitString(const StringLiteralPtr& node) override;
    virtual void visitInteger(const IntegerLiteralPtr& node) override;
    virtual void visitFloat(const FloatLiteralPtr& node) override;
    virtual void visitConditionalOperator(const ConditionalOperatorPtr& node) override;
    virtual void visitBinaryOperator(const BinaryOperatorPtr& node) override;
    virtual void visitUnaryOperator(const UnaryOperatorPtr& node) override;
    virtual void visitTuple(const TuplePtr& node) override;
    //virtual void visitIdentifier(const IdentifierPtr& node) override;
    virtual void visitCompileConstant(const CompileConstantPtr& node) override;
    virtual void visitMemberAccess(const MemberAccessPtr& node) override;
    //virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitReturn(const ReturnStatementPtr& node) override;
    virtual void visitParenthesizedExpression(const ParenthesizedExpressionPtr& node) override;
    virtual void visitArrayLiteral(const ArrayLiteralPtr& node) override;
    virtual void visitDictionaryLiteral(const DictionaryLiteralPtr& node) override;
    //virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    //virtual void visitSubscript(const SubscriptDefPtr& node) override;
    virtual void visitSubscriptAccess(const SubscriptAccessPtr& node) override;
    //virtual void visitStruct(const StructDefPtr& node) override;
public:
    /*
    virtual void visitValueBindings(const ValueBindingsPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    */
private:
    void visitAccessor(const CodeBlockPtr& accessor, const ParametersPtr& params, const SymbolPtr& setter);
    TypePtr defineType(const std::shared_ptr<TypeDeclaration>& node, Type::Category category);
    //Register all symbols in the pattern
    void registerPattern(const PatternPtr& pattern);
    //Verify each symbol in the tuple is initialized
    void verifyTuplePattern(const PatternPtr& pattern);

    //set or reset flag in all identifiers in given pattern
    void setFlag(const PatternPtr& pattern, bool add, int flag);
    FunctionSymbolPtr createFunctionSymbol(const FunctionDefPtr& func, const GenericDefinitionPtr& generic);
    TypePtr createFunctionType(const std::vector<ParametersPtr>::const_iterator& begin, const std::vector<ParametersPtr>::const_iterator& end, const TypePtr& retType, const GenericDefinitionPtr& generic);

    /**
    * Prepare parameters as symbols in given code block
    */
    void prepareParameters(SymbolScope* scope, const ParametersPtr& params);
    void registerSymbol(const SymbolPlaceHolderPtr& symbol);
    GenericDefinitionPtr prepareGenericTypes(const GenericParametersDefPtr& params);

    void visitFunctionCall(const IdentifierPtr& name, const FunctionCallPtr& node);
    void visitFunctionCall(const SymbolPtr& func, const FunctionCallPtr& node);
private:
    void checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer);
    TypePtr evaluateType(const ExpressionPtr& expr);

    /**
    * Calculates the fit score of arguments on given function
    * @return -1 if the type is not matched
    */
    float calculateFitScore(const TypePtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors);

    bool checkArgument(const TypePtr& funcType, const Type::Parameter& parameter, const std::pair<std::wstring, ExpressionPtr>& argument, bool variadic, float& score, bool supressErrors);
    TypePtr getExpressionType(const ExpressionPtr& expr, const TypePtr& hint, float& score);

    /**
    * Return a function that matches the given argument
    * This will always returns a matched function, if no functions matched it will throw exception and abort the process
    */
    FunctionSymbolPtr getOverloadedFunction(const NodePtr& node, const FunctionOverloadedSymbolPtr& funcs, const ParenthesizedExpressionPtr& arguments);
    /**
    * Check if the given expression can be converted to given type
    */
    bool canConvertTo(const ExpressionPtr&, const TypePtr& type);

    bool isInteger(const TypePtr& type);
    bool isNumber(const TypePtr& type);
    bool isFloat(const TypePtr& type);


    /**
    * Verify if the specified type conform to the given protocol
    */
    void verifyProtocolConform(const TypePtr& type);
    void verifyProtocolConform(const TypePtr& type, const TypePtr& protocol);
    void verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected);
private:
    //hint for parsing Array/tuple/dictionary literal
    TypePtr t_hint;
private:
    TypePtr currentType;
};




SWIFT_NS_END




#endif//SEMANTIC_ANALYZER_H