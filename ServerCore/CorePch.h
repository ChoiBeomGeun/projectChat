#pragma once

#include "CoreGlobal.h"
#include "Container.h"
#include "PacketHeader.h"

#include <windows.h>
#include <iostream>
using namespace std;

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
