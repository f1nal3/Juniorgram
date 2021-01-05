#pragma once

#include <iostream>
#include <cstdint>
#include <chrono>
#include <string>
#include <memory>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>
#include <algorithm>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE

#include <asio.hpp>
