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
#include <iomanip>

#include "utility.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <asio.hpp>
