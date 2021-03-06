/* SemanticAnalyzer_Condition.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "SemanticAnalyzer.h"
#include "GlobalScope.h"
#include "SymbolRegistry.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "ast/NodeSerializer.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "TypeBuilder.h"
#include "ScopedNodes.h"
#include "FunctionIterator.h"
#include <cassert>
#include <algorithm>


USE_SWALLOW_NS
using namespace std;

void SemanticAnalyzer::visitIf(const IfStatementPtr& node)
{

}

static void checkExhausiveSwitch(SemanticAnalyzer* sa, const SwitchCasePtr& node)
{
    TypePtr conditionType = node->getControlExpression()->getType();
    //only check for enum and bool type
    TypePtr Bool = sa->getSymbolRegistry()->getGlobalScope()->Bool;
    if(conditionType->getCategory() != Type::Enum && conditionType != Bool)
        return;
    //do not check if it already has a default case
    if(node->getDefaultCase())
        return;
    if(conditionType->getCategory() == Type::Enum)
    {
        if(node->numCases() != conditionType->numEnumCases())
        {
            sa->error(node, Errors::E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE);
        }
    }
    else if(conditionType == Bool)
    {
        if(node->numCases() != 2)
        {
            sa->error(node, Errors::E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE);
        }
    }
}

void SemanticAnalyzer::visitSwitchCase(const SwitchCasePtr& node)
{
    node->getControlExpression()->accept(this);
    TypePtr conditionType = node->getControlExpression()->getType();
    assert(conditionType != nullptr);
    StackedValueGuard<TypePtr> contextualType(this->t_hint);
    contextualType.set(conditionType);

    checkExhausiveSwitch(this, node);

    for(const CaseStatementPtr& c : *node)
    {
        CodeBlockPtr statements = c->getCodeBlock();
        if(statements->numStatements() == 0)
        {
            error(node, Errors::E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0, L"case");
            return;
        }
        c->accept(this);
    }
    if(node->getDefaultCase())
    {

        if(node->getDefaultCase()->getCodeBlock()->numStatements() == 0)
        {
            error(node, Errors::E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0, L"default");
            return;
        }
        node->getDefaultCase()->accept(this);
    }
}

void SemanticAnalyzer::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
    //enum-case is similar to member access but also provided associated-value-enum binding's unpacking
    if(!t_hint)
    {
        //invalid contextual type
        error(node, Errors::E_NO_CONTEXTUAL_TYPE_TO_ACCESS_MEMBER_A_1, node->getName());
        return;
    }
    const EnumCase* ec = t_hint->getEnumCase(node->getName());
    if (ec == nullptr)
    {
        error(node, Errors::E_DOES_NOT_HAVE_A_MEMBER_2, t_hint->toString(), node->getName());
        return;
    }
    node->setType(t_hint);
    //TODO check for associated values for unpacking

}


void SemanticAnalyzer::visitCase(const CaseStatementPtr& node)
{
    assert(t_hint != nullptr);
    ScopedCodeBlockPtr codeBlock = static_pointer_cast<ScopedCodeBlock>(node->getCodeBlock());
    for(const CaseStatement::Condition& cond : node->getConditions())
    {
        //the pattern should be evaluated to the contextual type where the condition must evaluate to BooleanLiteralConvertible
        cond.condition->accept(this);
        TypePtr patternType = cond.condition->getType();
        assert(patternType != nullptr);
        if(!patternType->canAssignTo(t_hint))
        {
            error(cond.condition, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, patternType->toString(), t_hint->toString());
            break;
        }
        if(cond.guard)
        {
            cond.guard->accept(this);
            TypePtr whereType = cond.guard->getType();
            assert(whereType != nullptr);
            TypePtr p_BooleanType = symbolRegistry->getGlobalScope()->BooleanType;
            if(!whereType->canAssignTo(p_BooleanType))
            {
                error(cond.guard, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, whereType->toString(), p_BooleanType->toString());
                break;
            }
        }
        //create symbols that used for unpacking associated values
        if(t_hint->getCategory() == Type::Enum)
        {
            PatternAccessibility accessibility = AccessibilityUndefined;
            TuplePtr tuple = nullptr;
            EnumCasePatternPtr enumCase = dynamic_pointer_cast<EnumCasePattern>(cond.condition);
            if(!enumCase)
            {
                if (ValueBindingPatternPtr binding = dynamic_pointer_cast<ValueBindingPattern>(cond.condition))
                {
                    enumCase = dynamic_pointer_cast<EnumCasePattern>(binding->getBinding());
                    accessibility = binding->isReadOnly() ? AccessibilityConstant : AccessibilityVariable;
                }
            }
            assert(enumCase != nullptr);
            const EnumCase* ec = t_hint->getEnumCase(enumCase->getName());
            if(ec && ec->type != symbolRegistry->getGlobalScope()->Void)
            {
                vector<TupleExtractionResult> results;
                vector<int> indices;
                this->expandTuple(results, indices, cond.condition, L"#0", ec->type, accessibility);
                for (auto var : results)
                {
                    //register symbol
                    const wstring &name = var.name->getIdentifier();
                    SymbolPlaceHolderPtr sym(new SymbolPlaceHolder(name, var.type, SymbolPlaceHolder::R_LOCAL_VARIABLE, SymbolFlagInitialized | SymbolFlagReadable));
                    codeBlock->getScope()->addSymbol(sym);
                }
            }
        }
    }
    node->getCodeBlock()->accept(this);
}
