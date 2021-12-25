#pragma once
#define NOMINMAX

/// Common includes
#include <Windows.h>
#include <algorithm>
#include <cstdint>
#include <cctype>
#include <functional>
#include <thread>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <string_view>
#include <deque>
#include <fstream>
#include <Psapi.h>
#include <sstream>
#include <libloaderapi.h>
#include <cmath>
#include <memory>
#include <stack>
#include <iostream>
#include <TlHelp32.h>
#include <any>
#include <array>
#include <filesystem>
#include <winternl.h>
#include <intrin.h>

/// Our stuff
#include "hash.hpp"
#include "machine_hash.hpp"

#include "injection.hpp"

#include "communication.hpp"

#include "console.hpp"

#include "security.hpp"

#include "json/json.hpp"
