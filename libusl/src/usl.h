#ifndef USL_H
#define USL_H

#include "memory.h"
#include "debug.h"

#include <istream>

struct Thread;
struct Scope;

struct Usl
{
	Usl();
	virtual ~Usl() {}
	
	void includeScript(const std::string& name, std::istream& source);
	void createThread(const std::string& name, std::istream& source);
	
	virtual std::ifstream* openFile(const std::string& name);
	
	typedef std::map<std::string, Value*> Cache;
	typedef std::map<std::string, Value*> RuntimeValues;
	typedef std::vector<Thread> Threads;
	
	DebugInfo debug;
	Heap heap;
	Scope* root;
	Cache cache;
	RuntimeValues runtimeValues;
	Threads threads;
	
	/// Run one thread (round-robin over all threads) for a maximum of steps bytecodes executions
	size_t run(size_t steps);
	
private:
	Scope* compile(const std::string& name, std::istream& source);
	Thread* createThread(Scope* scope);
	friend struct Load;
};

#endif // ndef USL_H
