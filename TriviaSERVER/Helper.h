#pragma once

#include <vector>
#include <string>
#include <WinSock2.h>
#include "Codes.h"

// Numbers
#define ZERO    0
#define ONE     1
#define TWO     2
#define THREE   3

// For conveting into bytes
#define SHIFT_24    24
#define SHIFT_16    16
#define SHIFT_8      8
#define BYTE_MASK   0xFF

class Helper
{
public:
	static int getMessageTypeCode(const SOCKET sc);
	static int getIntPartFromSocket(const SOCKET sc, const int bytesNum);
	static std::string getStringPartFromSocket(SOCKET sc, const int bytesNum);
	static void sendData(const SOCKET sc, const std::string message);
	static void send_update_message_to_client(const SOCKET sc, const std::string& file_content, const std::string& second_username, const std::string& all_users);
	static std::string getPaddedNumber(const int num, const int digits);
	static int ByteToInt(std::vector<unsigned char> buffer);
	static void send_Msg(SOCKET client_socket, std::vector<unsigned char> buffer);

private:
	static std::string getPartFromSocket(const SOCKET sc, const int bytesNum);
	static std::string getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags);

};


#ifdef _DEBUG // vs add this define in debug mode
#include <stdio.h>
// Q: why do we need traces ?
// A: traces are a nice and easy way to detect bugs without even debugging
// or to understand what happened in case we miss the bug in the first time
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
// for convenient reasons we did the traces in stdout
// at general we would do this in the error stream like that
// #define TRACE(msg, ...) fprintf(stderr, msg "\n", __VA_ARGS__);

#else // we want nothing to be printed in release version
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
#define TRACE(msg, ...) // do nothing
#endif