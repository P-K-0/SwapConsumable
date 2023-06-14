
#pragma once

namespace Settings {

	constexpr auto IniFile = "Data\\SKSE\\Plugins\\SwapConsumable.ini";

	struct Item {

		RE::FormID base{};
		RE::FormID swap{};
		std::int32_t count{};
	};

	class Manager {

	public:

		[[nodiscard]] static Manager& GetSingleton() noexcept { return instance; }

		void Read() noexcept;
		void EnumFiles() noexcept;

		[[nodiscard]] auto& GetItems() noexcept { return items; }

		[[nodiscard]] bool IsEnabled(RE::Actor*) noexcept;

	private:

		void ReadFile(const std::string&) noexcept;

		[[nodiscard]] Item Parse(const std::string&) noexcept;
		[[nodiscard]] RE::FormID AsID(const std::string&) noexcept;

		std::unordered_map<RE::FormID, Item> items;

		bool bEnablePC{ true };
		bool bEnableNPC{ false };
	
		static Manager instance;
	};
}
