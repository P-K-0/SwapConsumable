
#pragma warning(push)

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "xbyak/xbyak.h"

#ifdef NDEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif

#pragma warning(pop)

namespace logger = SKSE::log;

using namespace std::literals;

#include <regex>

#include "version.h"
#include "boost/filesystem.hpp"
#include "SimpleIni.h"
