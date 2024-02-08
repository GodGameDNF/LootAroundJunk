#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <windows.h>

using namespace RE;

PlayerCharacter* p = nullptr;
BSScript::IVirtualMachine* vm = nullptr;
TESDataHandler* DH = nullptr;

float getCount;
float scrapCount;

bool bLootRunning = false;
BGSStoryManagerQuestNode* questNode = nullptr;
TESQuest* lootQuest = nullptr;
Actor* scrapActor = nullptr;
TESGlobal* gAllGetCount = nullptr;

BGSKeyword* FeaturedItem = nullptr;
BGSKeyword* UnscrappableObject = nullptr;

std::string lootDir;
std::vector<TESForm*> getList;
std::vector<TESForm*> skipList;

std::vector<TESForm*> miscGetList;
std::vector<TESForm*> ammoGetList;
std::vector<TESForm*> alchGetList;

std::vector<TESForm*> miscSkipList;
std::vector<TESForm*> ammoSkipList;
std::vector<TESForm*> alchSkipList;

TESGlobal* gLootRange = nullptr;
TESGlobal* gLootYES = nullptr;
TESGlobal* gLootBox = nullptr;
TESGlobal* gLootActor = nullptr;
TESGlobal* gLootFeaturedItem = nullptr;
TESGlobal* gLootScrap = nullptr;
TESGlobal* gLootPrint = nullptr;
BGSEquipSlot* GrenadeSlot = nullptr;

std::vector<TESForm*> compoSkipList;
BGSListForm* steelList = nullptr;
BGSListForm* woodList = nullptr;
BGSListForm* glassList = nullptr;
BGSListForm* boneList = nullptr;

TESGlobal* gLootSteel = nullptr;
TESGlobal* gLootWood = nullptr;
TESGlobal* gLootGlass = nullptr;
TESGlobal* gLootBone = nullptr;

BGSKeyword* Alcohol = nullptr;
TESGlobal* gLootAlcohol = nullptr;

TESGlobal* gLootAmmo = nullptr;
TESGlobal* gLootNuka = nullptr;
BGSKeyword* ObjectTypeNukaCola = nullptr;
TESGlobal* gLootMeat = nullptr;
BGSSoundDescriptorForm* meatCheck = nullptr;
TESGlobal* gLootIngredient = nullptr;
BGSKeyword* FruitOrVegetable = nullptr;
TESGlobal* gLootFood = nullptr;
BGSKeyword* ObjectTypeFood = nullptr;
BGSKeyword* ObjectTypeChem = nullptr;
EffectSetting* DamageRadiationChem = nullptr;

BGSMessage* getMessage = nullptr;
BGSMessage* sendMessage = nullptr;
BGSMessage* twoMessage = nullptr;

struct RefrOrInventoryObj
{
	TESObjectREFR* _ref{ nullptr };        // 00
	TESObjectREFR* _container{ nullptr };  // 08
	uint16_t _uniqueID{ 0 };               // 10
};

struct FormOrInventoryObj
{
	TESForm* form{ nullptr };  // TESForm 포인터를 가리키는 포인터
	uint64_t second_arg{ 0 };  // unsigned 64비트 정수
};

BGSSoundDescriptorForm* GetCraftingUseSound(TESForm* t)
{
	using func_t = decltype(&GetCraftingUseSound);
	REL::Relocation<func_t> func{ REL::ID(278311) };
	return func(t);
}

bool isLocked(BSScript::IVirtualMachine* vm, uint32_t i, TESObjectREFR* t)
{
	using func_t = decltype(&isLocked);
	REL::Relocation<func_t> func{ REL::ID(1317816) };
	return func(vm, i, t);
}

bool HasKeywordVM(BSScript::IVirtualMachine* vm, uint32_t i, TESForm* target, BGSKeyword* k)
{
	using func_t = decltype(&HasKeywordVM);
	REL::Relocation<func_t> func{ REL::ID(1400368) };
	return func(vm, i, target, k);
}

void PickUpObject(Actor* pRef, TESObjectREFR* target, std::int32_t count, bool a_playPickUpSounds)
{
	using func_t = decltype(&PickUpObject);
	REL::Relocation<func_t> func{ REL::ID(1119564) };
	return func(pRef, target, count, a_playPickUpSounds);
}

void AddChild(BGSStoryManagerQuestNode* n, TESQuest* q, uint32_t index)
{
	using func_t = decltype(&AddChild);
	REL::Relocation<func_t> func{ REL::ID(105164) };
	return func(n, q, index);
}

void RemoveChild(BGSStoryManagerQuestNode* n, TESQuest* q, uint32_t index)
{
	using func_t = decltype(&RemoveChild);
	REL::Relocation<func_t> func{ REL::ID(1501255) };
	return func(n, q, index);
}

bool CanBePickedUp(BGSInventoryItem* send)
{
	using func_t = decltype(&CanBePickedUp);
	REL::Relocation<func_t> func{ REL::ID(1434670) };
	return func(send);
}

bool GetDisabled(TESObjectREFR* send)
{
	using func_t = decltype(&GetDisabled);
	REL::Relocation<func_t> func{ REL::ID(395330) };
	return func(send);
}

uint64_t GetCount(TESObjectREFR* send)
{
	using func_t = decltype(&GetCount);
	REL::Relocation<func_t> func{ REL::ID(50725) };
	return func(send);
}

void PlayerPickUpObject(PlayerCharacter* pRef, TESObjectREFR* target, std::int32_t count, bool bSound)
{
	using func_t = decltype(&PlayerPickUpObject);
	REL::Relocation<func_t> func{ REL::ID(1411483) };
	return func(pRef, target, count, bSound);
}

const char* GetModel(TESForm* send)
{
	using func_t = decltype(&GetModel);
	REL::Relocation<func_t> func{ REL::ID(1039522) };
	return func(send);
}

bool IsActivationBlocked(TESObjectREFR* t)
{
	using func_t = decltype(&IsActivationBlocked);
	REL::Relocation<func_t> func{ REL::ID(407609) };
	return func(t);
}

bool RemoveItemVM(BSScript::IVirtualMachine* vm, uint32_t i, TESObjectREFR* target, FormOrInventoryObj obj, uint32_t count, bool b1, TESObjectREFR* sender)
{
	using func_t = decltype(&RemoveItemVM);
	REL::Relocation<func_t> func{ REL::ID(492460) };
	return func(vm, i, target, obj, count, b1, sender);
}

bool AddItemVM(BSScript::IVirtualMachine* vm, uint32_t i, TESObjectREFR* target, FormOrInventoryObj obj, uint32_t count, bool b1)
{
	using func_t = decltype(&AddItemVM);
	REL::Relocation<func_t> func{ REL::ID(1212351) };
	return func(vm, i, target, obj, count, b1);
}

bool Show(BSScript::IVirtualMachine* vm, uint32_t i, BGSMessage* send, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9)
{
	using func_t = decltype(&Show);
	REL::Relocation<func_t> func{ REL::ID(175806) };
	return func(vm, i, send, f1, f2, f3, f4, f5, f6, f7, f8, f9);
}

void setMiscFilter(std::monostate) // MCM 잡동사니 재료 필터
{
	compoSkipList.clear();

	if (gLootSteel->value == 0) {
		for (TESForm* form : steelList->arrayOfForms) {
			compoSkipList.push_back(form);
		}
	};

	if (gLootWood->value == 0) {
		for (TESForm* form : woodList->arrayOfForms) {
			compoSkipList.push_back(form);
		}
	}

	if (gLootGlass->value == 0) {
		for (TESForm* form : glassList->arrayOfForms) {
			compoSkipList.push_back(form);
		}
	}

	if (gLootBone->value == 0) {
		for (TESForm* form : boneList->arrayOfForms) {
			compoSkipList.push_back(form);
		}
	}
}

struct contLootStruct // 상자템 마지막 정리를 위해 구조체 배열에 저장했다 처리함
{
	BGSInventoryItem item;
	bool bScrap;
};

bool EnumReferencesCloseToRef(RE::TESDataHandler* SendHandler, RE::TESObjectREFR* TargetRef, float fDistance, RE::NiPoint3* tPoint, float fdistance02, bool(__fastcall*)(RE::TESObjectREFR* refr, void* acc), void* acc2)
{
	using func_t = decltype(&EnumReferencesCloseToRef);
	REL::Relocation<func_t> func{ REL::ID(1348560) };

	return func(
		SendHandler, TargetRef, fDistance, tPoint, fdistance02,
		[](TESObjectREFR* ref, void* acc) -> bool {
			if (ref != p && ref != nullptr) {
				if (!GetDisabled(ref) && !ref->IsCrimeToActivate() && !IsActivationBlocked(ref)) {
					TESBoundObject* refBase = ref->data.objectReference;
					stl::enumeration<ENUM_FORM_ID, std::uint8_t> type = refBase->formType;

					if ((gLootActor->value == 1 && type == ENUM_FORM_ID::kNPC_ && ref->IsDead(ref)) || (gLootBox->value == 1 && type == ENUM_FORM_ID::kCONT && !isLocked(vm, 0, ref) && GetModel(refBase) != "Markers\\EditorMarkers\\ContainerMarker.nif")) {
						BGSInventoryList* temp = ref->inventoryList;
						if (temp) {
							BSTArray<BGSInventoryItem> list = temp->data;
							
							if ((type == ENUM_FORM_ID::kNPC_ && list.size() > 0) || (type == ENUM_FORM_ID::kCONT && list.size() > 0 && list.size() < 20)) {
								std::vector<contLootStruct> contStruct; // 마지막 RemoveItemVM을 위해 구조체 배열에 저장
								
								for (BGSInventoryItem bItem : list) {
									if (!CanBePickedUp(&bItem))
										continue;

									TESBoundObject* obj = bItem.object;

									if (!obj)
										continue;

									stl::enumeration<ENUM_FORM_ID, std::uint8_t> objType = obj->formType;

									// 무기 슬롯이 GrenadeSlot 인지 확인
									if (type == ENUM_FORM_ID::kWEAP) {
										TESObjectWEAP* weapon = (TESObjectWEAP*)refBase;
										if (weapon->equipSlot && weapon->equipSlot == GrenadeSlot) {
											if (gLootFeaturedItem->value == 0 && HasKeywordVM(vm, 0, obj, FeaturedItem))
												continue;

											contStruct.push_back(contLootStruct(bItem, false));
										}
									// 탄약. 기본 필터 검사만 함
									} else if (objType == ENUM_FORM_ID::kAMMO && gLootAmmo->value == 1) {
										if (!ammoGetList.empty()) {
											if (std::find(ammoGetList.begin(), ammoGetList.end(), obj) != ammoGetList.end()) {
												contStruct.push_back(contLootStruct(bItem, false));
												continue;
											}
										}

										if (gLootFeaturedItem->value == 0 && HasKeywordVM(vm, 0, obj, FeaturedItem))
											continue;

										if (!ammoSkipList.empty()) {
											if (std::find(ammoSkipList.begin(), ammoSkipList.end(), obj) != ammoSkipList.end()) {
												continue;
											}
										}
										contStruct.push_back(contLootStruct(bItem, false));
									// 잡동사니. MCM 필터 확인과 스크랩 가능 잡동사니인지 확인함
									} else if (objType == ENUM_FORM_ID::kMISC) {
										if (!miscGetList.empty()) {
											if (std::find(miscGetList.begin(), miscGetList.end(), obj) != miscGetList.end()) {
												contStruct.push_back(contLootStruct(bItem, false));
											}
										}

										if (gLootFeaturedItem->value == 0 && HasKeywordVM(vm, 0, obj, FeaturedItem))
											continue;

										if (!compoSkipList.empty()) {
											if (std::find(compoSkipList.begin(), compoSkipList.end(), obj) != compoSkipList.end()) {
												continue;
											}
										}

										if (!miscSkipList.empty()) {
											if (std::find(miscSkipList.begin(), miscSkipList.end(), obj) != miscSkipList.end()) {
												continue;
											}
										}

										// 스크랩설정과 잡동사니의 스크랩 재료 정보를 확인하고 스크랩 NPC로 보냄
										if (gLootScrap->value == 0) {
											contStruct.push_back(contLootStruct(bItem, false));
										} else {
											BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* checkCompo = ((TESObjectMISC*)obj)->componentData;

											if (HasKeywordVM(vm, 0, obj, UnscrappableObject) || HasKeywordVM(vm, 0, obj, FeaturedItem) || !checkCompo || checkCompo->empty()) {
												contStruct.push_back(contLootStruct(bItem, false));
											} else {
												contStruct.push_back(contLootStruct(bItem, true));
											}
										}
									// 소모품 확인
									} else if (objType == ENUM_FORM_ID::kALCH) {
										if (!alchGetList.empty()) {
											if (std::find(alchGetList.begin(), alchGetList.end(), obj) != alchGetList.end()) {
												contStruct.push_back(contLootStruct(bItem, false));
												continue;
											}
										}

										if (gLootFeaturedItem->value == 0 && HasKeywordVM(vm, 0, obj, FeaturedItem))
											continue;

										if (!alchSkipList.empty()) {
											if (std::find(alchSkipList.begin(), alchSkipList.end(), obj) != alchSkipList.end()) {
												continue;
											}
										}

										if (ref->HasKeyword(ObjectTypeChem) || ((MagicItem*)obj)->IsMedicine()) { // 플래그가 의약품인지 확인함
											contStruct.push_back(contLootStruct(bItem, false));
											continue;
										} else if (gLootNuka->value == 1 && HasKeywordVM(vm, 0, obj, ObjectTypeNukaCola)) { // 누카콜라 키워드 확인
											contStruct.push_back(contLootStruct(bItem, false));
											continue;
										} else if (gLootAlcohol->value == 1 && HasKeywordVM(vm, 0, obj, Alcohol)) { // 알코올 키워드 확인
											contStruct.push_back(contLootStruct(bItem, false));
											continue;
										} else if (gLootMeat->value == 1 && GetCraftingUseSound(obj) && GetCraftingUseSound(obj) == meatCheck) {
											contStruct.push_back(contLootStruct(bItem, false)); // 크래프팅 사운드가 고기소리인지 확인
											continue;
										} else if (gLootIngredient->value == 1 && HasKeywordVM(vm, 0, obj, FruitOrVegetable)) {
											contStruct.push_back(contLootStruct(bItem, false)); // 음식재료 키워드 확인
											continue;
										}
										// 음식 방사능 effect가 있는지 확인
										if (gLootFood->value == 0 && HasKeywordVM(vm, 0, obj, (ObjectTypeFood))) {
											BSTArray<EffectItem*> eList = ((MagicItem*)obj)->listOfEffects;
											if (!eList.empty()) {
												for (EffectItem* effect : eList) {
													if (effect->effectSetting == DamageRadiationChem) {
														continue;
													}
												}
											}
										}
										contStruct.push_back(contLootStruct(bItem, false));
									}
								}

								int saveCount = contStruct.size();

								if (saveCount == 0)
									return false;

								/// 상자를 연 상태에서 템창 정리를 위해 마지막은 파피루스 스크립트로 처리
								for (int i = saveCount - 1; i >= 0; --i) {
									BGSInventoryItem bItem = contStruct[i].item;
									TESBoundObject* obj = bItem.object;
									uint32_t iCount = bItem.GetCount();

									stl::enumeration<ENUM_FORM_ID, std::uint8_t> objType = obj->formType;

									if (!contStruct[i].bScrap) { // 스크랩 상자로 보내는 작업인지 확인
										if (i == 0) {
											FormOrInventoryObj tempObj;
											tempObj.form = obj;
											getCount++;
											RemoveItemVM(vm, 0, ref, tempObj, iCount, true, p);
										} else {
											TESObjectREFR::RemoveItemData* rData = new TESObjectREFR::RemoveItemData(obj, iCount);
											rData->reason = ITEM_REMOVE_REASON::KDropping;
											ObjectRefHandle dropRef = ref->RemoveItem(*rData);
											getCount++;
											PlayerPickUpObject(p, dropRef.get().get(), iCount, false);
											delete rData;
										}								
									} else {
										if (i == 0) {
											FormOrInventoryObj tempObj;
											tempObj.form = obj;
											scrapCount++;
											RemoveItemVM(vm, 0, ref, tempObj, iCount, true, scrapActor);
										} else {
											TESObjectREFR::RemoveItemData* rData = new TESObjectREFR::RemoveItemData(obj, iCount);
											rData->reason = ITEM_REMOVE_REASON::kStoreContainer;
											rData->a_otherContainer = scrapActor;
											scrapCount++;
											ref->RemoveItem(*rData);
											delete rData;
										}
									}
								}
							}
						}

					//// 필드 아이템 처리부분. 아이템습득 메세지 안나오게 버린후 주워먹기
					} else if (type == ENUM_FORM_ID::kWEAP) {
						TESObjectWEAP* weapon = (TESObjectWEAP*)refBase;
						if (weapon->equipSlot && weapon->equipSlot == GrenadeSlot) {
							if (gLootFeaturedItem->value == 0 && ref->HasKeyword(FeaturedItem))
								return false;

							PlayerPickUpObject(p, ref, GetCount(ref), false);
						}
					} else if (type == ENUM_FORM_ID::kAMMO && gLootAmmo->value == 1) {
						if (!ammoGetList.empty()) {
							if (std::find(ammoGetList.begin(), ammoGetList.end(), refBase) != ammoGetList.end()) {
								getCount++;
								PlayerPickUpObject(p, ref, GetCount(ref), false);
								return false;
							}
						}

						if (gLootFeaturedItem->value == 0 && ref->HasKeyword(FeaturedItem))
							return false;

						if (!ammoSkipList.empty()) {
							if (std::find(ammoSkipList.begin(), ammoSkipList.end(), refBase) != ammoSkipList.end()) {
								return false;
							}
						}
						PlayerPickUpObject(p, ref, GetCount(ref), false);
					} else if (type == ENUM_FORM_ID::kMISC) {
						if (!miscGetList.empty()) {
							if (std::find(miscGetList.begin(), miscGetList.end(), refBase) != miscGetList.end()) {
								getCount++;
								PlayerPickUpObject(p, ref, GetCount(ref), false);
								return false;
							}
						}

						if (gLootFeaturedItem->value == 0 && ref->HasKeyword(FeaturedItem))
							return false;

						if (!compoSkipList.empty()) {
							if (std::find(compoSkipList.begin(), compoSkipList.end(), refBase) != compoSkipList.end()) {
								return false;
							}
						}

						if (!miscSkipList.empty()) {
							if (std::find(miscSkipList.begin(), miscSkipList.end(), refBase) != miscSkipList.end()) {
								return false;
							}
						}

						if (gLootScrap->value == 0) {
							getCount++;
							PlayerPickUpObject(p, ref, GetCount(ref), false);
						} else {
							BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* checkCompo = ((TESObjectMISC*)refBase)->componentData;

							if (ref->HasKeyword(UnscrappableObject) || ref->HasKeyword(FeaturedItem) || !checkCompo || checkCompo->empty()) {
								getCount++;
								PlayerPickUpObject(p, ref, GetCount(ref), false);
							} else {
								scrapCount++;
								PickUpObject(scrapActor, ref, GetCount(ref), false);
							}
						}
					} else if (type == ENUM_FORM_ID::kALCH) {
						if (!alchGetList.empty()) {
							if (std::find(alchGetList.begin(), alchGetList.end(), refBase) != alchGetList.end()) {
								getCount++;
								PlayerPickUpObject(p, ref, GetCount(ref), false);
								return false;
							}
						}

						if (gLootFeaturedItem->value == 0 && ref->HasKeyword(FeaturedItem))
							return false;

						if (!alchSkipList.empty()) {
							if (std::find(alchSkipList.begin(), alchSkipList.end(), refBase) != alchSkipList.end()) {
								return false;
							}
						}

						if (ref->HasKeyword(ObjectTypeChem) || ((MagicItem*)refBase)->IsMedicine()) {
							getCount++;
							PlayerPickUpObject(p, ref, GetCount(ref), false);
							return false;
						} else if (gLootNuka->value == 1 && ref->HasKeyword(ObjectTypeNukaCola)) {
							getCount++;
							PlayerPickUpObject(p, ref, GetCount(ref), false);
							return false;
						} else if (gLootAlcohol->value == 1 && ref->HasKeyword(Alcohol)) {
							getCount++;
							PlayerPickUpObject(p, ref, GetCount(ref), false);
							return false;
						} else if (gLootMeat->value == 1 && GetCraftingUseSound(refBase) && GetCraftingUseSound(refBase) == meatCheck) {
							getCount++;
							PlayerPickUpObject(p, ref, GetCount(ref), false);
							return false;
						} else if (gLootIngredient->value == 1 && ref->HasKeyword(FruitOrVegetable)) {
							getCount++;
							PlayerPickUpObject(p, ref, GetCount(ref), false);
							return false;
						}
						if (gLootFood->value == 0 && ref->HasKeyword(ObjectTypeFood)) {
							BSTArray<EffectItem*> eList = ((MagicItem*)refBase)->listOfEffects;
							if (!eList.empty()) {
								for (EffectItem* effect : eList) {
									if (effect->effectSetting == DamageRadiationChem) {
										return false;
									}
								}
							}
						}
						getCount++;
						PlayerPickUpObject(p, ref, GetCount(ref), false);
					}
				}
			}
			return false;
		},
		acc2);
}

TESForm* getFormformFile(std::string tempLine) // txt 파일 라인 처리부분
{
	TESForm* form = nullptr;

	const int MIN_FORM_ID = 0;
	const int MAX_FORM_ID = 0xFFFFFF;  // 예시 범위
	size_t pos = tempLine.find("**");
	if (pos == std::string::npos) {
		return form;  // 문자열에 **가 없음
	}

	std::istringstream iss(tempLine);

	std::string espName = tempLine.substr(0, pos);  // ** 이전 부분
	std::string stringFormID = tempLine.substr(pos + 2);
	int formID;
	std::istringstream(stringFormID) >> std::hex >> formID;

	if (formID < MIN_FORM_ID || formID > MAX_FORM_ID) {
		return form;  // 받아온 값이 이상함
	}

	form = DH->LookupForm(formID, espName);

	return form;
}

bool FillContainerfromFile(std::monostate, bool bGet) // 필터 가구를 열때 txt 목록 아이템을 상자에 넣음
{
	std::string fileName;
	TESObjectREFR* filterBox = nullptr;

	if (bGet) {
		fileName = "Get.txt";
		filterBox = (TESObjectREFR*)DH->LookupForm(0x860, "LootAroundJunk.esp");
	} else {
		fileName = "Skip.txt";
		filterBox = (TESObjectREFR*)DH->LookupForm(0x861, "LootAroundJunk.esp");
	}

	std::string GetPath = lootDir + fileName;
	std::ifstream GetFile(GetPath);
	if (!GetFile) {
		return false;  // 파일을 열지 못한 경우
	}

	std::string tempLine;

	while (std::getline(GetFile, tempLine)) {
		TESForm* form = getFormformFile(tempLine);
		if (form) {
			FormOrInventoryObj tempObj;
			tempObj.form = form;
			AddItemVM(vm, 0, filterBox, tempObj, 1, true);
		}
	}

	GetFile.close();

	return true;
}

void injectList_Get(TESForm* form)
{
	stl::enumeration<ENUM_FORM_ID, std::uint8_t> type = form->formType;
	if (type == ENUM_FORM_ID::kMISC) {
		miscGetList.push_back(form);
	} else if (type == ENUM_FORM_ID::kAMMO) {
		ammoGetList.push_back(form);
	} else if (type == ENUM_FORM_ID::kALCH) {
		alchGetList.push_back(form);
	}
}

void injectList_Skip(TESForm* form)
{
	stl::enumeration<ENUM_FORM_ID, std::uint8_t> type = form->formType;
	if (type == ENUM_FORM_ID::kMISC) {
		miscSkipList.push_back(form);
	} else if (type == ENUM_FORM_ID::kAMMO) {
		ammoSkipList.push_back(form);
	} else if (type == ENUM_FORM_ID::kALCH) {
		alchSkipList.push_back(form);
	}
}

bool loadFilterSettingsFromFiles() // esp에 적은 필터와 txt 필터를 배열에 삽입
{
	miscGetList.clear();
	ammoGetList.clear();
	alchGetList.clear();

	miscSkipList.clear();
	ammoSkipList.clear();
	alchSkipList.clear();

	BGSListForm* yesLootList = (BGSListForm*)DH->LookupForm(0x867, "LootAroundJunk.esp");
	BGSListForm* noLootList = (BGSListForm*)DH->LookupForm(0x866, "LootAroundJunk.esp");

	for (TESForm* form : yesLootList->arrayOfForms) {
		if (form) {
			injectList_Get(form);
		}
	}

	for (TESForm* form : noLootList->arrayOfForms) {
		if (form) {
			injectList_Skip(form);
		}
	}

	std::string getName = "Get.txt";
	std::string GetPath = lootDir + getName;
	std::ifstream GetFile(GetPath);
	if (!GetFile) {
		return false;  // 파일을 열지 못한 경우
	}
	std::string tempLine;

	while (std::getline(GetFile, tempLine)) {
		TESForm* form = getFormformFile(tempLine);

		if (form) {
			injectList_Get(form);
		}
	}

	GetFile.close();

	std::string skipName = "Skip.txt";
	std::string skipPath = lootDir + skipName;
	std::ifstream SkipFile(skipPath);
	if (!SkipFile) {
		return false;  // 파일을 열지 못한 경우
	}
	std::string tempLine02;

	while (std::getline(SkipFile, tempLine02)) {
		TESForm* form = getFormformFile(tempLine02);

		if (form) {
			injectList_Skip(form);
		}
	}

	SkipFile.close();

	return true;
}

bool FilterContainerSetting(std::monostate, bool bGet) // 루팅 필터 가구를 닫을때 실행됨
{   
	std::string fileName;
	TESObjectREFR* filterBox = nullptr;

	if (bGet) {
		fileName = "Get.txt";
		filterBox = (TESObjectREFR*)DH->LookupForm(0x860, "LootAroundJunk.esp");
	} else {
		fileName = "Skip.txt";
		filterBox = (TESObjectREFR*)DH->LookupForm(0x861, "LootAroundJunk.esp");
	}

	if (!filterBox)
		return false;

	std::string filePath = lootDir + fileName;
	std::ofstream fileStream(filePath, std::ios::out);  // 파일 쓰기 모드로 열기

	if (!fileStream.is_open())
		return false;  // 파일을 열지 못한 경우 종료

	BGSInventoryList* temp = filterBox->inventoryList;
	if (!temp) {
		fileStream.flush();  // 리스트를 못받으면 파일 닫고 종료
		fileStream.close();
		return false;
	}

	std::vector<TESObjectREFR::RemoveItemData*> itemsToRemove;

	// 상자 내의 아이템에서 아이템 분류하고 메모장에 적기
	BSTArray<BGSInventoryItem> boxList = temp->data;
	if (!boxList.empty()) {
		for (BGSInventoryItem bItem : boxList) {
			TESBoundObject* obj = bItem.object;

			stl::enumeration<ENUM_FORM_ID, std::uint8_t> type = obj->formType;

			if (obj->formType != ENUM_FORM_ID::kMISC && obj->formType != ENUM_FORM_ID::kALCH && obj->formType != ENUM_FORM_ID::kAMMO) {
				continue;
			}

			// 멤버함수 썼더니 최종수정 모드가 뽑혀서 배열로 원본 esp 검색
			BSStaticArray<TESFile*> overwriteArray = obj->sourceFiles.array[0];
			TESFile* espFile = overwriteArray[0];
			bool bEsl = false;
			if (espFile->IsLight()) {
				bEsl = true;
			}

			std::string_view nameTemp = espFile->GetFilename();
			std::string espName = std::string(nameTemp);

			std::stringstream ss;
			ss << std::hex << obj->formID;
			std::string hexString = ss.str();

			// esl 플래그 확인하고 폼id 저장
			if (bEsl) {
				if (hexString.size() < 3) {
					hexString = std::string(3 - hexString.size(), '0') + hexString;
				}
				hexString = hexString.substr(hexString.size() - 3, 3);
			} else {
				if (hexString.size() < 6) {
					hexString = std::string(6 - hexString.size(), '0') + hexString;
				}
				hexString = hexString.substr(hexString.size() - 6, 6);
			}

			std::string slash = "**";
			std::string text = std::string(espName) + slash + hexString;
			fileStream << text << std::endl;

			TESObjectREFR::RemoveItemData* rData = new TESObjectREFR::RemoveItemData(obj, 1);
			rData->reason = ITEM_REMOVE_REASON::kNone;
			itemsToRemove.push_back(rData);
		}
	}

	// 삭제시 배열 꼬임을 막기 위해 임시 배열에 저장하고 한번에 삭제
	if (!itemsToRemove.empty()) {
		for (TESObjectREFR::RemoveItemData* rData : itemsToRemove) {
			ObjectRefHandle dropRef = filterBox->RemoveItem(*rData);
			delete rData;  // 메모리 해제
		}
		itemsToRemove.clear();
	}

	fileStream.flush();  // 버퍼를 비워서 파일에 쓰기 작업을 완료합니다.
	fileStream.close();  // 파일을 닫습니다.

	if (!loadFilterSettingsFromFiles()) // 기본 필터와 txt 필터를 각 배열에 삽입
		return false;

	return true;
}

void StartLoot(std::monostate)
{
	if (bLootRunning || gLootYES->value != 1) {
		return;
	} else {
		bLootRunning = true;
	}

	getCount = 0;
	scrapCount = 0;

	// 퀘스트 요청을 막기 위해 퀘스트 노드의 퀘스트를 잠시 제거
	BSTArray<TESQuest*> childRen = questNode->children;
	if (childRen.empty()) {
		int i = childRen.size() - 1;
		while (i >= 0) {
			RemoveChild(questNode, childRen[i], 0);
			--i;
		}
	}

	//auto readConfigStart = std::chrono::high_resolution_clock::now();

	NiPoint3 point = p->GetPosition();
	EnumReferencesCloseToRef(DH, p, gLootRange->value, &point, gLootRange->value, nullptr, nullptr);

	//auto readConfigEnd = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> readConfigDuration = readConfigEnd - readConfigStart;

	//logger::info("ReadConfig execution time: {} seconds", readConfigDuration.count());

	if (gLootPrint->value != 0) {
		if (getCount > 0 && scrapCount > 0) {
			Show(vm, 0, twoMessage, getCount, scrapCount, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		} else if (getCount > 0) {
			Show(vm, 0, getMessage, getCount, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		} else if (scrapCount > 0) {
			Show(vm, 0, sendMessage, scrapCount, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		}
	}

	gAllGetCount->value = gAllGetCount->value + getCount + scrapCount;

	AddChild(questNode, lootQuest, 0);
	bLootRunning = false;
	return;
}

void OnF4SEMessage(F4SE::MessagingInterface::Message* msg)
{
	switch (msg->type) {
	case F4SE::MessagingInterface::kGameDataReady:
		{
			p = PlayerCharacter::GetSingleton();
			DH = RE::TESDataHandler::GetSingleton();

			char resultBuf[256];
			uint32_t tInt = GetModuleFileNameA(GetModuleHandle(NULL), resultBuf, sizeof(resultBuf));

			// 실행 파일 경로를 구한 후 targetDirectory에 직접 할당
			lootDir = std::string(resultBuf, tInt);
			lootDir = lootDir.substr(0, lootDir.find_last_of('\\')) + "\\Data\\F4SE\\Plugins\\_LootingFilter\\";

			questNode = (BGSStoryManagerQuestNode*)DH->LookupForm(0x803, "LootAroundJunk.esp");
			lootQuest = (TESQuest*)DH->LookupForm(0x858, "LootAroundJunk.esp");
			scrapActor = (Actor*)DH->LookupForm(0x863, "LootAroundJunk.esp");
			gAllGetCount = (TESGlobal*)DH->LookupForm(0x868, "LootAroundJunk.esp");

			gLootRange = (TESGlobal*)DH->LookupForm(0x801, "LootAroundJunk.esp");
			gLootYES = (TESGlobal*)DH->LookupForm(0x800, "LootAroundJunk.esp");
			gLootBox = (TESGlobal*)DH->LookupForm(0x802, "LootAroundJunk.esp");
			gLootActor = (TESGlobal*)DH->LookupForm(0x809, "LootAroundJunk.esp");
			gLootScrap = (TESGlobal*)DH->LookupForm(0x804, "LootAroundJunk.esp");
			gLootPrint = (TESGlobal*)DH->LookupForm(0x806, "LootAroundJunk.esp");
			gLootFeaturedItem = (TESGlobal*)DH->LookupForm(0x865, "LootAroundJunk.esp");

			GrenadeSlot = (BGSEquipSlot*)DH->LookupForm(0x00046AAC, "Fallout4.esm");

			FeaturedItem = (BGSKeyword*)DH->LookupForm(0x01B3FAC, "Fallout4.esm");
			UnscrappableObject = (BGSKeyword*)DH->LookupForm(0x01CC46A, "Fallout4.esm");
			Alcohol = (BGSKeyword*)DH->LookupForm(0x0010C416, "Fallout4.esm");
			gLootAlcohol = (TESGlobal*)DH->LookupForm(0x830, "LootAroundJunk.esp");

			steelList = (BGSListForm*)DH->LookupForm(0x823, "LootAroundJunk.esp");
			woodList = (BGSListForm*)DH->LookupForm(0x824, "LootAroundJunk.esp");
			glassList = (BGSListForm*)DH->LookupForm(0x822, "LootAroundJunk.esp");
			boneList = (BGSListForm*)DH->LookupForm(0x81F, "LootAroundJunk.esp");

			gLootSteel = (TESGlobal*)DH->LookupForm(0x827, "LootAroundJunk.esp");
			gLootWood = (TESGlobal*)DH->LookupForm(0x828, "LootAroundJunk.esp");
			gLootGlass = (TESGlobal*)DH->LookupForm(0x826, "LootAroundJunk.esp");
			gLootBone = (TESGlobal*)DH->LookupForm(0x825, "LootAroundJunk.esp");

			gLootAmmo = (TESGlobal*)DH->LookupForm(0x829, "LootAroundJunk.esp");
			gLootNuka = (TESGlobal*)DH->LookupForm(0x82C, "LootAroundJunk.esp");
			ObjectTypeNukaCola = (BGSKeyword*)DH->LookupForm(0x00249F30, "Fallout4.esm");
			gLootMeat = (TESGlobal*)DH->LookupForm(0x82D, "LootAroundJunk.esp");
			meatCheck = (BGSSoundDescriptorForm*)DH->LookupForm(0x11286F, "Fallout4.esm");
			gLootIngredient = (TESGlobal*)DH->LookupForm(0x82F, "LootAroundJunk.esp");
			FruitOrVegetable = (BGSKeyword*)DH->LookupForm(0x1C63E5, "Fallout4.esm");
			gLootFood = (TESGlobal*)DH->LookupForm(0x82E, "LootAroundJunk.esp");
			ObjectTypeFood = (BGSKeyword*)DH->LookupForm(0x00055ECC, "Fallout4.esm");
			ObjectTypeChem = (BGSKeyword*)DH->LookupForm(0x000F4AE7, "Fallout4.esm");
			DamageRadiationChem = (EffectSetting*)DH->LookupForm(0x024056, "Fallout4.esm");

			getMessage = (BGSMessage*)DH->LookupForm(0x0811, "LootAroundJunk.esp");
			sendMessage = (BGSMessage*)DH->LookupForm(0x0812, "LootAroundJunk.esp");
			twoMessage = (BGSMessage*)DH->LookupForm(0x0810, "LootAroundJunk.esp");

			loadFilterSettingsFromFiles();
			break;
		}
	case F4SE::MessagingInterface::kPostLoadGame:
		{
			std::monostate a;
			setMiscFilter(a);
			break;
		}
	case F4SE::MessagingInterface::kNewGame:
		{
			std::monostate a;
			setMiscFilter(a);
			break;
		}
	}
}

bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* a_vm)
{
	vm = a_vm;

	REL::IDDatabase::Offset2ID o2i;
	logger::info("0x0x499f0: {}", o2i(0x499f0));

	//std::size_t offset = REL::IDDatabase::get().id2offset(1067039);
	//logger::info("Offset for ID 1067039: {}", offset);

	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "StartLoot"sv, StartLoot);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "FilterContainerSetting"sv, FilterContainerSetting);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "FillContainerfromFile"sv, FillContainerfromFile);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "setMiscFilter"sv, setMiscFilter);

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= fmt::format("{}.log", Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("Global Log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::trace);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%^%l%$] %v"s);

	logger::info("{} v{}", Version::PROJECT, Version::NAME);

	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_f4se->IsEditor()) {
		logger::critical("loaded in editor");
		return false;
	}

	const auto ver = a_f4se->RuntimeVersion();
	if (ver < F4SE::RUNTIME_1_10_162) {
		logger::critical("unsupported runtime v{}", ver.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);

	const F4SE::PapyrusInterface* papyrus = F4SE::GetPapyrusInterface();
	if (papyrus)
		papyrus->Register(RegisterPapyrusFunctions);

	const F4SE::MessagingInterface* message = F4SE::GetMessagingInterface();
	if (message)
		message->RegisterListener(OnF4SEMessage);

	return true;
}
