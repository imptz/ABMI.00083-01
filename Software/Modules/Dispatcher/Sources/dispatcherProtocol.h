#ifndef DISPATCHER_PROTOCOL_H
#define DISPATCHER_PROTOCOL_H

#pragma pack(push, 1)

struct DispatcherPacketHeader{
	unsigned int messageNumber;
	unsigned int hash;
};

#pragma pack(pop)

#endif
