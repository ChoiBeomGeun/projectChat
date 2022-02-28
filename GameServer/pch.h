#pragma once

#define WIN32_LEAN_AND_MEAN   // 불필요한 windows 헤더 삭제

#ifdef  _DEBUG
#pragma comment(lib,"Debug\\ServerCore.lib")

#else
#pragma comment(lib,"Release\\ServerCore.lib")
#endif //  _DEBUG


#include "CorePch.h"

