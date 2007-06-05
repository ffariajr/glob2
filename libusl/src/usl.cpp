#include "lexer.h"
#include "tree.h"
#include "code.h"
#include "memory.h"
#include "interpreter.h"
#include <cassert>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <ext/functional>
#include <utility>
#include <stack>

using namespace std;

struct Integer: Value
{
	int value;
	static struct IntegerAdd: NativeCode::Operation
	{
		IntegerAdd():
			NativeCode::Operation(&integerPrototype, "Integer::+", false)
		{}
		
		Value* execute(Thread* thread, Value* receiver, Value* argument)
		{
			Integer* thisInt = dynamic_cast<Integer*>(receiver);
			Integer* thatInt = dynamic_cast<Integer*>(argument);
			
			assert(thisInt);
			assert(thatInt);
			
			return new Integer(thread->heap, thisInt->value + thatInt->value);
		}
	} integerAdd;
	
	static struct IntegerPrototype: Prototype
	{
		IntegerPrototype():
			Prototype(0)
		{
			methods["+"] = &integerAdd;
		}
	} integerPrototype;
	
	Integer(Heap* heap, int value):
		Value(heap, &integerPrototype),
		value(value)
	{
		this->value = value;
	}
	
	virtual void dumpSpecific(std::ostream& stream) const { stream << "= " << value; }
};

Integer::IntegerPrototype Integer::integerPrototype;
Integer::IntegerAdd Integer::integerAdd;


struct DefRefCode: Code
{
	DefRefCode(size_t depth, ScopePrototype* method):
		depth(depth),
		method(method)
	{}
	
	void execute(Thread* thread)
	{
		Thread::Frame& frame = thread->frames.back();
		Value* receiver = frame.scope;
		for(size_t i = 0; i < depth; ++i)
		{
			Scope* outer = dynamic_cast<Scope*>(receiver);
			assert(outer); // Should not fail if the parser is bug-free
			receiver = outer->outer;
		}
		Function* function = new Function(thread->heap, receiver, method);
		frame.stack.push_back(function);
	}
	
	size_t depth;
	ScopePrototype* method;
};

struct DefLookupNode: ExpressionNode
{
	DefLookupNode(ScopePrototype* scope, const string& name):
		scope(scope),
		name(name)
	{}
	
	void generate(ScopePrototype* scope)
	{
		// TODO: this should be done in a compiler pass between parsing and code generation
		Prototype* prototype = scope;
		size_t depth = 0;
		while (true)
		{
			ScopePrototype* method = prototype->lookup(name);
			if (method != 0)
			{
				scope->body.push_back(new DefRefCode(depth, method));
				return;
			}
			ScopePrototype* s = dynamic_cast<ScopePrototype*>(prototype);
			assert(s); // TODO: throw a method not found exception
			prototype = s->outer;
			++depth;
		}
	}
	
	ScopePrototype* scope;
	string name;
};

struct Parser: Lexer
{
	Parser(const char* src, Heap* heap):
		Lexer(src),
		heap(heap)
	{}
	
	BlockNode* parse(ScopePrototype* scope)
	{
		return statements(scope);
	}
	
	BlockNode* statements(ScopePrototype* scope)
	{
		newlines();
		auto_ptr<BlockNode> block(new BlockNode());
		
		ScopePrototype* thisMethod = new ScopePrototype(heap, scope);
		scope->methods["this"] = thisMethod;
		block->statements.push_back(new DefNode(thisMethod, new ParentNode()));
		
		while (true)
		{
			switch (tokenType())
			{
			case END:
			case RBRACE:
				BlockNode::Statements& statements = block->statements;
				if (!statements.empty() && dynamic_cast<ExpressionNode*>(statements.back()) != 0)
				{
					block->value = statements.back();
					statements.pop_back();
				}
				else
				{
					block->value = new ConstNode(&nil);
				}
				return block.release();
			}
			block->statements.push_back(statement(scope));
			newlines();
		}
	}
	
	Node* statement(ScopePrototype* scope)
	{
		switch (tokenType())
		{
		case VAL:
			{
				next();
				string name = identifier();
				accept(ASSIGN);
				newlines();
				size_t index = scope->locals.size();
				scope->locals.push_back(name);
				return new ValNode(index, expression(scope));
			}
		case DEF:
			{
				next();
				string name = identifier();
				accept(ASSIGN);
				newlines();
				ScopePrototype* method = new ScopePrototype(heap, scope);
				scope->methods[name] = method;
				return new DefNode(method, expression(method));
			}
		default:
			return expression(scope);
		}
	}
	
	ExpressionNode* expression(ScopePrototype* scope)
	{
		auto_ptr<ExpressionNode> node(simple(scope));
		while (true)
		{
			switch (tokenType())
			{
			case ID:
				node.reset(selectAndApply(node, identifier(), scope));
				break;
			case LPAR:
			case LBRACE:
				node.reset(selectAndApply(node, "!", scope));
				break;
			default:
				return node.release();
			}
		}
	}
	
	ApplyNode* selectAndApply(auto_ptr<ExpressionNode> receiver, const string& method, ScopePrototype* scope)
	{
		ExpressionNode* argument = lazyExpr(scope);
		return new ApplyNode(new SelectNode(receiver.release(), method), argument);
	}
	
	ExpressionNode *expressions(ScopePrototype* scope)
	{
		newlines();
		auto_ptr<TupleNode> tuple(new TupleNode());
		while (true)
		{
			switch (tokenType())
			{
			case ID:
			case NUM:
			case LPAR:
			case LBRACE:
				tuple->expressions.push_back(expression(scope));
				newlines();
				break;
			
			case COMMA:
				next();
				newlines();
				break;
			
			case RPAR:
				next();
				switch (tuple->expressions.size())
				{
				case 0:
					return new ConstNode(&nil);
				case 1:
					{
						ExpressionNode* expr = tuple->expressions[0];
						tuple->expressions.clear();
						return expr;
					}
				default:
					return tuple.release();
				}
			
			default:
				assert(false);
			}
		}
	}
	
	ExpressionNode* simple(ScopePrototype* scope)
	{
		switch (tokenType())
		{
		case ID:
			{
				string name(identifier());
				
				ScopePrototype* s = scope;
				size_t depth = 0;
				do
				{
					ScopePrototype::Locals& locals = s->locals;
					size_t index = find(locals.begin(), locals.end(), name) - locals.begin();
					if (index < locals.size())
						return new ValRefNode(depth, index);
					s = dynamic_cast<ScopePrototype*>(s->outer);
					++depth;
				}
				while (s != 0);
				
				return new DefLookupNode(scope, name);
			}
		case NUM:
			{
				string str = token.string();
				next();
				int value = atoi(str.c_str());
				return new ConstNode(new Integer(heap, value));
			}
		case LPAR:
			{
				next();
				return expressions(scope);
			}
		case LBRACE:
			{
				next();
				auto_ptr<ScopePrototype> block(new ScopePrototype(heap, scope));
				auto_ptr<ExpressionNode> body(statements(block.get()));
				accept(RBRACE);
				return new ApplyNode(new DefRefNode(block.release(), body.release()), new ConstNode(&nil));
			}
		default:
			return new ConstNode(&nil);
		}
	}
	
	ExpressionNode* lazyExpr(ScopePrototype* scope)
	{
		auto_ptr<ScopePrototype> lazy(new ScopePrototype(heap, scope));
		ExpressionNode* expr = simple(lazy.get());
		return new DefRefNode(lazy.release(), expr);
	}
	
	void newlines()
	{
		while (tokenType() == NL)
		{
			next();
		}
	}
	
	string identifier()
	{
		if (tokenType() == ID)
		{
			string id = token.string();
			next();
			return id;
		}
		else
		{
			assert(false);
		}
	}
	
	void accept(TokenType type)
	{
		assert(tokenType() == type);
		next();
	}
	
	Heap* heap;
};

int main(int argc, char** argv)
{
	Heap heap;
	ScopePrototype* root = new ScopePrototype(&heap, 0);
	
	Parser parser("def z = 2\nval x = {{21} + {21}}\nx", &heap);
	Node* node = parser.parse(root);
	node->generate(root);
	
	Thread thread(&heap);
	thread.frames.push_back(Thread::Frame(new Scope(&heap, root, 0)));
	
	while (thread.frames.front().nextInstr < root->body.size())
	{
		Thread::Frame& frame = thread.frames.back();
		Code* code = frame.scope->def()->body[frame.nextInstr++];
		code->execute(&thread);
		code->dump(cout);
		cout << ", frames: " << thread.frames.size();
		cout << ", stack size: " << thread.frames.back().stack.size();
		cout << ", locals: " << thread.frames.back().scope->locals.size();
		cout << endl;
	}
	cout << "result: ";
	thread.frames.back().stack.back()->dump(cout);
	cout << endl;
	
	thread.frames.pop_back();
	
	cout << "heap size: " << heap.values.size() << "\n";
	cout << "garbage collecting\n";
	heap.garbageCollect(&thread);
	cout << "heap size: " << heap.values.size() << "\n";
}
