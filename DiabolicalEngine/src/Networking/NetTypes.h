#pragma once


class NetBuffer
{
public:
	NetBuffer(int NewSize) { Buffer = new char[NewSize]; Size = NewSize; }
	~NetBuffer() { delete[] Buffer; }
	int Size = 0;
	char* Buffer = nullptr;
};