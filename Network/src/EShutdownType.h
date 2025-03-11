#pragma once
#include "WinSock2.h"
namespace networking {
	enum EShutdownType
	{
		Receive = SD_RECEIVE,
		Send = SD_SEND,
		Both = SD_BOTH
	};

}