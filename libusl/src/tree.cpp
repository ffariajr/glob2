#include "tree.h"
#include "code.h"
#include <memory>

using namespace std;

void DefRefNode::generate(ScopePrototype* scope)
{
	value->generate(this->scope);
	this->scope->body.push_back(new ReturnCode());
	scope->body.push_back(new ScopeCode());
	scope->body.push_back(new DefRefCode(this->scope));
}

void ConstNode::generate(ScopePrototype* scope)
{
	scope->body.push_back(new ConstCode(value));
}

void ValRefNode::generate(ScopePrototype* scope)
{
	scope->body.push_back(new ValRefCode(depth, index));
}

void SelectNode::generate(ScopePrototype* scope)
{
	receiver->generate(scope);
	scope->body.push_back(new SelectCode(name));
}

void ApplyNode::generate(ScopePrototype* scope)
{
	receiver->generate(scope);
	argument->generate(scope);
	scope->body.push_back(new ApplyCode());
}

void ValNode::generate(ScopePrototype* scope)
{
	value->generate(scope);
	scope->body.push_back(new ValCode());
}

void BlockNode::generate(ScopePrototype* scope)
{
	
	for (Statements::const_iterator it = statements.begin(); it != statements.end(); ++it)
	{
		Node* statement = *it;
		statement->generate(scope);
		if (dynamic_cast<ExpressionNode*>(statement) != 0)
		{
			// if the statement is an expression, its result is ignored
			scope->body.push_back(new PopCode());
		}
	}
	value->generate(scope);
}

void DefNode::generate(ScopePrototype* scope)
{
	scope = this->scope;
	body->generate(scope);
	scope->body.push_back(new ReturnCode());
}

void ScopeNode::generate(ScopePrototype* scope)
{
	scope->body.push_back(new ScopeCode());
}

void ParentNode::generate(ScopePrototype* scope)
{
	this->scope->generate(scope);
	scope->body.push_back(new ParentCode());
}

void TupleNode::generate(ScopePrototype* scope)
{
	for (Expressions::const_iterator it = expressions.begin(); it != expressions.end(); ++it)
	{
		Node* expression = *it;
		expression->generate(scope);
	}
	scope->body.push_back(new TupleCode(expressions.size()));
}

void DefLookupNode::generate(ScopePrototype* scope)
{
	auto_ptr<ScopeNode> receiver(new ScopeNode());
	ScopePrototype* method;
	
	// TODO: this should be done in a compiler pass between parsing and code generation
	Prototype* prototype = scope;
	while (true)
	{
		method = prototype->lookup(name);
		if (method != 0)
			break;
		
		ScopePrototype* s = dynamic_cast<ScopePrototype*>(prototype);
		assert(s != 0); // TODO: throw a method not found exception
		receiver.reset(new ParentNode(receiver.release()));
		prototype = s->outer;
	}
	
	receiver->generate(scope);
	scope->body.push_back(new DefRefCode(method));
}
