
#include "settings.h"

namespace Settings {

	void Manager::Read() noexcept	
	{
		EnumFiles();

		CSimpleIni ini;

		ini.SetUnicode();

		auto ret = ini.LoadFile("Data\\SKSE\\Plugins\\SwapConsumable.ini");

		if (ret == SI_OK) {

			bEnablePC = std::atoi(ini.GetValue("Main", "bEnablePC", "1"));
			bEnableNPC = std::atoi(ini.GetValue("Main", "bEnableNPC", "0"));
		}
	}

	void Manager::EnumFiles() noexcept
	{
		boost::filesystem::path path{ "Data\\SKSE\\Plugins\\SwapConsumable\\" };

		std::vector<std::string> files;

		for (auto it : boost::filesystem::directory_iterator{ path }) {

			if (!boost::filesystem::is_regular_file(it))
				continue;

			if (_strcmpi(it.path().extension().string().c_str(), ".ini") != 0)
				continue;

			files.push_back(it.path().lexically_normal().string());
		}

		if (files.empty())
			return;

		std::sort(files.begin(), files.end());

		files.erase(std::unique(files.begin(), files.end()), files.end());

		for (auto& file : files)
			ReadFile(file);
	}

	void Manager::ReadFile(const std::string& filename) noexcept
	{
		std::ifstream ifs{ filename };

		if (!ifs)
			return;

		for (std::string line; std::getline(ifs, line); ) {

			auto item = Parse(line);

			if (item.base == 0)
				continue;

			auto it = items.find(item.base);

			if (it != items.end()) {

				logger::info("file : {} the list already contains this ID : {}", filename, item.base);

				continue;
			}

			auto frm = RE::TESForm::LookupByID(item.base);

			if (!frm) {

				logger::info("invalid ID : {}", item.base);

				continue;
			}

			auto isValid = frm->Is(RE::FormType::AlchemyItem) | frm->Is(RE::FormType::Ingredient);

			if (!isValid) {

				logger::info("ID : {} the item must be consumable", item.base);

				continue;
			}

			items[item.base] = item;
		}
	}

	Item Manager::Parse(const std::string& line) noexcept
	{
		Item tmp{};

		std::regex reg{ R"((\S*\w*.\w*)\u007C(\w*)=(\S*\w*.\w*)\u007C(\w*)~(\d*))" };

		std::smatch matches;

		if (std::regex_search(line, matches, reg) && matches.size() >= 6) {

			auto dataHandler = RE::TESDataHandler::GetSingleton();

			if (dataHandler) {

				tmp.base = dataHandler->LookupFormID(AsID(matches[2].str()), matches[1].str());
				tmp.swap = dataHandler->LookupFormID(AsID(matches[4].str()), matches[3].str());
				tmp.count = std::atoi(matches[5].str().c_str());
			}
		}

		return tmp;
	}

	RE::FormID Manager::AsID(const std::string& str) noexcept
	{
		RE::FormID ret{};

		std::stringstream ss;

		ss << std::hex << str;
		ss >> ret;

		return ret;
	}

	bool Manager::IsEnabled(RE::Actor* actor) noexcept
	{
		if (actor == RE::PlayerCharacter::GetSingleton())
			return bEnablePC;

		return bEnableNPC;
	}

	Manager Manager::instance;
}