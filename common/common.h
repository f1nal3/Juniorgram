#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#endif

#include <asio.hpp>
