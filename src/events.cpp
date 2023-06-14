
#include "events.h"
#include "settings.h"

namespace Events {

	void UpdateListItem(RE::TESObjectREFR* refr, RE::TESForm* form)
	{
		using fn = decltype(&UpdateListItem);
		REL::Relocation<fn> fnUpdateListItem(REL::RelocationID(51911, 52849, 0));
		fnUpdateListItem(refr, form);
	}

	void Hook::Drink(RE::Actor* a_this, RE::AlchemyItem* a_item, RE::ExtraDataList* a_extraList)
	{
		a_DrinkPotion(a_this, a_item, a_extraList);

		if (!a_this || a_this->IsDead() || !a_this->Is3DLoaded() || !a_item)
			return;

		auto& settings = Settings::Manager::GetSingleton();

		if (!settings.IsEnabled(a_this))
			return;

		auto& items = settings.GetItems();

		auto it = items.find(a_item->formID);

		if (it != items.end()) {

			auto frmObj = RE::TESForm::LookupByID(it->second.swap);

			if (frmObj) {

				auto obj = frmObj->As<RE::TESBoundObject>();

				if (obj) 
					a_this->AddObjectToContainer(obj, nullptr, it->second.count, nullptr);

				if (a_this == RE::PlayerCharacter::GetSingleton()) {

					UpdateListItem(a_this, nullptr);
					UpdateListItem(a_this, nullptr);
				}
			}
		}
	}

	void Hook::Init() noexcept
	{
		static bool registered{};

		if (registered)
			return;

		struct Code_DrinkPotion : 
			Xbyak::CodeGenerator {

			Code_DrinkPotion(REL::RelocationID address)
			{
				Xbyak::Label label;

				mov(ptr[rsp + 8], rbx);

				jmp(ptr[rip + label]);

				L(label);

				dq(address.address() + 5);
			}
		};

		auto& trampoline = SKSE::GetTrampoline();

		SKSE::AllocTrampoline(64);

		Code_DrinkPotion code{ DrinkPotionID };

		code.ready();

		a_DrinkPotion = (DrinkPotionFn)trampoline.allocate(code);

		trampoline.write_branch<5>(DrinkPotionID.address(), Drink);

		registered = true;
	
		logger::info("Registered successfully!");
	}

	DrinkPotionFn Hook::a_DrinkPotion;

	Hook Hook::instance;
}