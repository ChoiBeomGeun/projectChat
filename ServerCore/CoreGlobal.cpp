#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"
#include "DebugManager.h"
#include "MemoryManager.h"
#include <d3d9.h>
#include "SocketUtils.h"

ThreadManager * GThreadManager = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;
MemoryManager* GMemoryManager = nullptr;
SendBufferManager* GSendBufferManager = nullptr;
//DebugManager* GDebugManager = nullptr;



class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemoryManager = new MemoryManager();
		GDeadLockProfiler = new DeadLockProfiler();
		GSendBufferManager = new SendBufferManager();
	//	GDebugManager = new DebugManager(true);

		SocketUtils::Init();



	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GDeadLockProfiler;
		delete GMemoryManager;
		delete GSendBufferManager;
		SocketUtils::Clear();
	}
} GCoreGlobal ;
