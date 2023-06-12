
#include "settings.h"
#include "events.h"

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);

#ifndef NDEBUG
    auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
    auto path = logger::log_directory();
    if (!path) {
        return false;
    }

    *path /= fmt::format(FMT_STRING("{}.log"), Version::Project);
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

    auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
    log->set_level(spdlog::level::trace);
#else
    log->set_level(spdlog::level::info);
    log->flush_on(spdlog::level::info);
#endif

    spdlog::set_default_logger(std::move(log));
    spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

    logger::info(FMT_STRING("{} v {}.{}.{}"), Version::Project, Version::Major, Version::Minor, Version::Build);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* msg) {

        if (msg && msg->type == SKSE::MessagingInterface::kDataLoaded) {

            Settings::Manager::GetSingleton().Read();

            Events::Hook::GetSingleton().Init();
        }
    });

    return true;
}
