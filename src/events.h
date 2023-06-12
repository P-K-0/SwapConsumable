#pragma once

namespace Events {

	using DrinkPotionFn = void(*)(RE::Actor*, RE::AlchemyItem*, RE::ExtraDataList*);

	class Hook {
	 
	public:

		[[nodiscard]] static Hook& GetSingleton() noexcept { return instance; }

		static void Drink(RE::Actor*, RE::AlchemyItem*, RE::ExtraDataList*);

		void Init() noexcept;

	private:

		Hook() noexcept {}
		~Hook() noexcept {}

		Hook(const Hook&) = delete;
		Hook(Hook&&) = delete;

		Hook& operator=(const Hook&) = delete;
		Hook& operator=(Hook&&) = delete;

		const REL::RelocationID DrinkPotionID = RELOCATION_ID(37797, 38746);

		static DrinkPotionFn a_DrinkPotion;

		static Hook instance;
	};
}