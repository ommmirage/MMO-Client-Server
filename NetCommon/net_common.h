#pragma once

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <deque>


#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
