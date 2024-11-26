#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unordered_set>
#include <vector>
#include <windows.h>
#include <filesystem>

using namespace RE;
namespace fs = std::filesystem;

PlayerCharacter* p = nullptr;
BSScript::IVirtualMachine* vm = nullptr;
TESDataHandler* DH = nullptr;
FavoritesManager* favoritesManager = nullptr;

float getCount;
float scrapCount;

bool bLootRunning = false;
BGSStoryManagerQuestNode* questNode = nullptr;
TESQuest* lootQuest = nullptr;
Actor* scrapActor = nullptr;

BGSKeyword* FeaturedItem = nullptr;
BGSKeyword* UnscrappableObject = nullptr;
BGSKeyword* workshopKeyword = nullptr;

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
TESGlobal* gPipboyComponentSYNC = nullptr;
BGSEquipSlot* GrenadeSlot = nullptr;

BGSListForm* componentMaterialList = nullptr;
std::vector<TESForm*> componentMaterials;

BGSListForm* componentScrapList = nullptr;
BGSListForm* componentFilterList = nullptr;

TESObjectREFR* componentFilterCont_Added = nullptr;
TESObjectREFR* componentFilterCont_Removed = nullptr;

std::unordered_set<TESForm*> compoSkip_unorderdSet;
std::unordered_set<TESObjectREFR*> vendorContainers;

std::unordered_set<TESForm*> workshop_unorderdSet;
std::unordered_set<TESForm*> fruit_unorderdSet;
std::unordered_set<TESForm*> meat_unorderdSet;
std::unordered_set<TESForm*> chem_unorderdSet;

BGSKeyword* Alcohol = nullptr;
TESGlobal* gLootAlcohol = nullptr;

TESGlobal* gLootAmmo = nullptr;
TESGlobal* gLootNuka = nullptr;
BGSKeyword* ObjectTypeNukaCola = nullptr;
TESGlobal* gLootMeat = nullptr;
BGSSoundDescriptorForm* meatCheck = nullptr;
TESGlobal* gLootIngredient = nullptr;
BGSKeyword* FruitOrVegetable = nullptr;
TESGlobal* gLootRestore = nullptr;
TESGlobal* gLootFood = nullptr;
BGSKeyword* ObjectTypeFood = nullptr;
BGSKeyword* ObjectTypeChem = nullptr;
EffectSetting* DamageRadiationChem = nullptr;
EffectSetting* RestoreHealthFood = nullptr;
TESGlobal* gLootRestoreAmount = nullptr;
TESGlobal* gLoodRadMult = nullptr;

BGSMessage* getMessage = nullptr;
BGSMessage* sendMessage = nullptr;
BGSMessage* twoMessage = nullptr;

bool runingScriptToggle;

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

bool GetQuestObject(TESObjectREFR* t)
{
	using func_t = decltype(&GetQuestObject);
	REL::Relocation<func_t> func{ REL::ID(608378) };
	return func(t);
}

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

void AddItem(BGSInventoryList* list, TESBoundObject* bound, uint32_t count, ExtraDataList* extra, uint16_t c)
{
	using func_t = decltype(&AddItem);
	REL::Relocation<func_t> func{ REL::ID(98986) };
	return func(list, bound, count, extra, c);
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

void RemoveAllItems(BSScript::IVirtualMachine* vm, uint32_t i, TESObjectREFR* del, TESObjectREFR* send, bool bMove)
{
	using func_t = decltype(&RemoveAllItems);
	REL::Relocation<func_t> func{ REL::ID(534058) };
	return func(vm, i, del, send, bMove);
}

void RemoveAllScriptAddedForms(BGSListForm* list)
{
	using func_t = decltype(&RemoveAllScriptAddedForms);
	REL::Relocation<func_t> func{ REL::ID(812852) };
	return func(list);
}

bool IsComponentFavorite(FavoritesManager* fm, TESBoundObject* bound)
{
	using func_t = decltype(&IsComponentFavorite);
	REL::Relocation<func_t> func{ REL::ID(352046) };
	return func(fm, bound);
}

void ToggleComponentFavorite(FavoritesManager* fm, TESBoundObject* bound)
{
	using func_t = decltype(&ToggleComponentFavorite);
	REL::Relocation<func_t> func{ REL::ID(1084401) };
	return func(fm, bound);
}


	struct contLootStruct  // 상자템 마지막 정리를 위해 구조체 배열에 저장했다 처리함
{
	BGSInventoryItem item;
	bool bScrap;
};

// 파일에서 폼을 불러옴
TESForm* getFormformFile(std::string tempLine)
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

// 필터 가구를 열때 txt 목록 아이템을 상자에 넣음
bool FillContainerfromFile(std::monostate, bool bGet)
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

// 인벤토리 리스트를 못불러오면 만드는 함수
BGSInventoryList* initialize_FilterInvList(TESObjectREFR* filterContainer)
{
	TESObjectCONT* filterCONT = (TESObjectCONT*)(filterContainer->data.objectReference);

	if (!filterCONT) {
		return nullptr;
	}

	TESContainer* insertContainer = filterCONT;
	filterContainer->CreateInventoryList(insertContainer);                   // 인벤토리 리스트를 다시 만듬
	BGSInventoryList* invList = componentFilterCont_Removed->inventoryList;  // 변수에 생성한 인벤토리 리스트를 다시 할당

	if (!invList) {
		return nullptr;
	}

	return invList;
}

// MCM 잡동사니 재료 필터
void setComponentFilter(std::monostate)
{
	compoSkip_unorderdSet.clear();  // 먼저 예외 배열을 비움
	std::unordered_set<TESForm*> tempCompoFormSet;

	// 잡동사니 필터용 폼리스트에 물건을 확인하고 컴포넌트를 추출해서 배열에 저장
	if (!componentFilterList->arrayOfForms.empty()) {
		for (TESForm* scrapForm : componentFilterList->arrayOfForms) {
			//TESBoundObject* obj = bItem.object;
			if (!scrapForm) {
				continue;
			}

			TESObjectMISC* tempMisc = (TESObjectMISC*)scrapForm;
			if (!tempMisc) {
				continue;
			}

			BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* checkCompo = tempMisc->componentData;
			bool isCompoValid = checkCompo && !checkCompo->empty();

			if (isCompoValid) {
				auto& tuple = (*checkCompo)[0];
				TESForm* compoForm = tuple.first;
				if (compoForm) {
					tempCompoFormSet.insert(compoForm);
				}
			}
		}
	}

	if (tempCompoFormSet.empty()) {
		return;  // 잡동사니 필터용 배열이 비었으면 예외 아이템 배열을 만들 필요가 없으므로 종료함
	}

	auto& miscForms = DH->GetFormArray<RE::TESObjectMISC>();

	for (RE::TESObjectMISC* misc : miscForms) {
		if (misc) {
			BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* checkCompo = misc->componentData;
			bool isCompoValid = checkCompo && !checkCompo->empty();

			if (isCompoValid) {
				bool isSkipMisc = true;

				for (int i = 0; i < checkCompo->size(); ++i) {
					auto& tuple = (*checkCompo)[i];
					TESForm* compoForm = tuple.first;

					if (tempCompoFormSet.find(compoForm) == tempCompoFormSet.end()) {
						isSkipMisc = false;  // 스킵재료와 잡동사니의 재료를 비교해 다른게 있으면 스킵용 배열에 넣지 않음
						break;
					}
				}

				if (isSkipMisc) {
					compoSkip_unorderdSet.insert(misc);
				}
			}
		}
	}
}

// 글로벌 값이 켜져있다면 구성요소 필터를 설정할때
void processingTagSYNC_ToFilter()
{
	//logger::info("구성요소 필터에서 동기화");

	runingScriptToggle = true;  // 토글을 직접할때만 이벤트를 받게 하기 위해 변수 조절

	//RemoveAllScriptAddedForms(componentFilterList); // 구성요소 제외 폼리스트를 삭제함

	for (int i = 0; i < componentMaterials.size(); ++i) {
		TESBoundObject* tempBound = (TESBoundObject*)componentMaterials[i];  // 구성요소 자체를 변수에 넣음
		if (tempBound) {
			TESForm* scrapComponent = componentScrapList->arrayOfForms[i];  // 구성요소의 스크랩재료를 변수에 넣음

			bool existRemoveList = false;
			for (TESForm* form : componentFilterList->arrayOfForms) { // 구성요소 제외 폼리스트에 해당 스크랩 재료가 있는지 확인함
				if (form == scrapComponent) {
					existRemoveList = true;
					break;
				}
			}

			if (IsComponentFavorite(favoritesManager, tempBound)) {        // 구성요소가 핍보이 구성요소 태그에 등록되어있다면
				if (existRemoveList) {           // 있으면 없어야 하고 없으면 있어야함
					//logger::info("토글해서 필터를 끔");
					ToggleComponentFavorite(favoritesManager, tempBound);  // 두 값은 달라야함. 태그에 등록되어있는걸 취소시킴
				}
			} else {														// 구성요소가 핍보이 구성요소 태그에 등록되어있지 않다면
				if (!existRemoveList) {                                     // 있으면 없어야 하고 없으면 있어야함
					//logger::info("토글해서 필터를 켬");
					ToggleComponentFavorite(favoritesManager, tempBound);  // 두 값은 달라야함. 태그에 등록되어있는걸 취소시킴
				}
			}
		}
	}

	runingScriptToggle = false;

	//setComponentFilter(std::monostate{});  // componentFilterList 폼리스트에 컴포넌트 추가가 끝났으니 필터 배열에 misc 아이템을 주입
}

// 핍보이에서 구성요소를 태그할떄 실행
void processingTagSYNC_ToPipboy()
{
	//logger::info("핍보이에서 동기화");

	RemoveAllScriptAddedForms(componentFilterList); // 구성요소 필터 제거용 폼리스트를 삭제함

	for (int i = 0; i < componentMaterials.size(); ++i) {
		TESBoundObject* tempBound = (TESBoundObject*)componentMaterials[i]; // 구성요소 자체를 변수에 넣음
		if (tempBound) {
			if (!IsComponentFavorite(favoritesManager, tempBound)) {
				TESForm* scrapComponent = componentScrapList->arrayOfForms[i]; // 구성요소의 스크랩재료를 변수에 넣음

				if (scrapComponent) {
					componentFilterList->AddScriptAddedForm(scrapComponent); // 스크랩 재료를 스킵 폼리스트에 삽입
					//logger::info("스킵 폼리스트에 추가");
				}
			}
		}
	}

	setComponentFilter(std::monostate{});  // componentFilterList 폼리스트에 컴포넌트 추가가 끝났으니 필터 배열에 misc 아이템을 주입
}


// 구성재료 필터 선택 해제 상자를 열때 실행
void fillContainer_ComponentFilter_Removed(std::monostate)
{
	BGSInventoryList* removeContainerInvList = componentFilterCont_Removed->inventoryList;
	if (!removeContainerInvList) {  // 상자 초기화가 안됐으면 인벤토리 리스트 만들어서 할당함
		removeContainerInvList = initialize_FilterInvList(componentFilterCont_Removed);

		if (!removeContainerInvList) {
			return;
		}
	}

	RemoveAllItems(vm, 0, componentFilterCont_Removed, nullptr, false);

	if (componentFilterList->arrayOfForms.empty()) {
		return;
	}

	for (TESForm* scrapForm : componentFilterList->arrayOfForms) {
		if (scrapForm) {
			FormOrInventoryObj tempObj;
			tempObj.form = scrapForm;
			AddItemVM(vm, 0, componentFilterCont_Removed, tempObj, 1, false);
		}
	}
}

// 구성재료 필터 선택 해제 상자를 닫을때 실행
void fillContainer_ComponentFilter_Removed_Close(std::monostate)
{
	RemoveAllScriptAddedForms(componentFilterList);

	BGSInventoryList* removedContainerInvList = componentFilterCont_Removed->inventoryList;  // 필터 removed 상자의 리스트를 가져옴. 직전에 확인 했으므로 null체크 x
	BSTArray<BGSInventoryItem> boxList = removedContainerInvList->data;

	if (boxList.empty()) {
		compoSkip_unorderdSet.clear(); // removed 박스가 비었으면 아이템을 다 빼서 필터 건게 없으니 필터배열을 지우고 종료
		if (gPipboyComponentSYNC->value == 1) { // 구성요소 태그 동기화를 켰으면 배열을 처리함
			processingTagSYNC_ToFilter();
		}
			
		return;
	}

	for (BGSInventoryItem bItem : boxList) {
		TESBoundObject* obj = bItem.object;
		if (!obj) {
			continue;
		}

		componentFilterList->AddScriptAddedForm(obj);
	}

	if (gPipboyComponentSYNC->value == 1) {  // 구성요소 태그 동기화를 켰으면 배열을 처리함
		processingTagSYNC_ToFilter();
	}

	setComponentFilter(std::monostate{});  // componentFilterList 폼리스트에 컴포넌트 추가가 끝났으니 필터 배열에 misc 아이템을 주입
}

// 구성재료 필터 선택 상자를 열때 실행
void fillContainer_ComponentFilter_Added(std::monostate)
{
	RemoveAllItems(vm, 0, componentFilterCont_Added, nullptr, false);

	std::unordered_set<TESForm*> checkComponent_unorderdSet; // 확인용 임시 배열

	for (TESForm* component : componentScrapList->arrayOfForms) {
		if (component) {
			checkComponent_unorderdSet.insert(component); // 확인용 임시배열에 스크랩 재료를 옮김
		}
	}

	BGSInventoryList* addedContainerInvList = componentFilterCont_Added->inventoryList;
	if (!addedContainerInvList) {
		addedContainerInvList = initialize_FilterInvList(componentFilterCont_Added);

		if (!addedContainerInvList) {
			return;
		}
	}

	// 잡동사니 필터용 폼리스트에 재료를 확인하고 배열에 저장
	if (!componentFilterList->arrayOfForms.empty()) {
		for (TESForm* form : componentFilterList->arrayOfForms) {
			checkComponent_unorderdSet.erase(form);  // 폼리스트에 있는 재료면 삭제함
		}
	}

	for (TESForm* form : checkComponent_unorderdSet) {  // 모든 구성요소가 있는 배열에서 필터용 removed박스에 있는 재료를 삭제하고 남은 재료를 박스에 추가
		FormOrInventoryObj tempObj;
		tempObj.form = form;
		AddItemVM(vm, 0, componentFilterCont_Added, tempObj, 1, true);  // VM함수로 바로 처리가 안되기 때문에 파피루스에서 다음 함수 호출
	}
}

// 구성재료 필터 선택 상자를 닫을때 실행
void fillContainer_ComponentFilter_Added_to_Removed(std::monostate)
{
	BGSInventoryList* addedContainerInvList = componentFilterCont_Added->inventoryList;  // 필터 added 상자의 리스트를 가져옴. 직전에 확인 했으므로 null체크 x
	BSTArray<BGSInventoryItem> boxList = addedContainerInvList->data;

	std::unordered_set<TESForm*> checkComponent_unorderdSet;

	for (TESForm* component : componentScrapList->arrayOfForms) {
		if (component) {
			checkComponent_unorderdSet.insert(component);  // 먼저 스크랩 재료 폼리스트에서 전체 재료를 임시 배열에 모두 저장함
		}
	}

	for (BGSInventoryItem bItem : boxList) {
		TESBoundObject* obj = bItem.object;
		if (!obj) {
			continue;
		}

		checkComponent_unorderdSet.erase(obj);
	}

	if (checkComponent_unorderdSet.empty()) {
		compoSkip_unorderdSet.clear();  // 다 지워졌으면 뺀게 없으므로 배열을 지우고 종료함
		return;
	}

	RemoveAllScriptAddedForms(componentFilterList);

	for (TESForm* form : checkComponent_unorderdSet) {  // 모든 구성요소가 있는 배열에서 필터용 added 박스에 있는 재료를 삭제하고 남은 재료를 removed 박스에 추가
		TESBoundObject* bound = (TESBoundObject*)form;
		if (!bound) {
			continue;
		}

		componentFilterList->AddScriptAddedForm(bound);
	}

	if (gPipboyComponentSYNC->value == 1) {  // 구성요소 태그 동기화를 켰으면 배열을 처리함
		processingTagSYNC_ToFilter();
	}

	setComponentFilter(std::monostate{});  // componentFilterList 폼리스트에 컴포넌트 추가가 끝났으니 필터 배열에 misc 아이템을 주입
}

// 근처의 ref를 검색함
bool EnumReferencesCloseToRef(RE::TESDataHandler* SendHandler, RE::TESObjectREFR* TargetRef, float fDistance, RE::NiPoint3* tPoint, float fdistance02, bool(__fastcall*)(RE::TESObjectREFR* refr, void* acc), void* acc2)
{
	using func_t = decltype(&EnumReferencesCloseToRef);
	REL::Relocation<func_t> func{ REL::ID(1348560) };

	return func(
		SendHandler, TargetRef, fDistance, tPoint, fdistance02,
		[](TESObjectREFR* ref, void* acc) -> bool {
			((std::vector<TESObjectREFR*>*)acc)->push_back(ref);
			return false;
		},
		acc2);
}

// 무조건 인벤토리에 넣는 아이템을 txt에서 불러온걸 필터 벡터 배열에 주입
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

// 스킵할 아이템을 txt에서 불러온걸 필터 벡터 배열에 주입
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

// esp에 적은 필터와 txt 필터를 배열에 삽입
bool loadFilterSettingsFromFiles()
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

// 루팅 개별필터 가구를 닫을때 실행됨
bool FilterContainerSetting(std::monostate, bool bGet)
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

	if (!loadFilterSettingsFromFiles())  // 기본 필터와 txt 필터를 각 배열에 삽입
		return false;

	return true;
}

// 루팅 실행 코드
void runLooting_Slow(std::vector<TESObjectREFR*> refArray)
{
	FormOrInventoryObj tempObj;
	std::vector<TESObjectREFR*> alreadyLootBox;

	for (TESObjectREFR* ref : refArray) {
		if (ref != p && ref != nullptr) {
			if (!GetDisabled(ref) && !ref->IsCrimeToActivate() && !IsActivationBlocked(ref) && !GetQuestObject(ref)) {
				TESBoundObject* refBase = ref->data.objectReference;
				stl::enumeration<ENUM_FORM_ID, std::uint8_t> type = refBase->formType;

				if ((gLootActor->value == 1 && type == ENUM_FORM_ID::kNPC_ && ref->IsDead(ref)) || (type == ENUM_FORM_ID::kCONT && gLootBox->value == 1 && !isLocked(vm, 0, ref) && !HasKeywordVM(vm, 0, refBase, workshopKeyword) && (vendorContainers.find(ref) == vendorContainers.end()))) {
					BGSInventoryList* temp = ref->inventoryList;
					if (temp) {
						BSTArray<BGSInventoryItem> list = temp->data;
						size_t listSize = list.size();

						if ((type == ENUM_FORM_ID::kNPC_ && listSize > 0) || (type == ENUM_FORM_ID::kCONT && listSize > 0 && listSize < 30)) {
							if (std::find(alreadyLootBox.begin(), alreadyLootBox.end(), ref) != alreadyLootBox.end()) {
								continue;
							} else {
								alreadyLootBox.push_back(ref);
							}

							std::vector<contLootStruct> contStruct;  // 마지막 RemoveItemVM을 위해 구조체 배열에 저장

							for (BGSInventoryItem bItem : list) {
								if (!CanBePickedUp(&bItem)) {
									continue;
								}

								TESBoundObject* obj = bItem.object;

								if (!obj) {
									continue;
								}

								TESFullName* refBaseName = (TESFullName*)refBase;
								if (!refBaseName || TESFullName::GetFullName(*refBase, false).empty()) {
									continue;
								}

								stl::enumeration<ENUM_FORM_ID, std::uint8_t> objType = obj->formType;

								// 무기 슬롯이 GrenadeSlot 인지 확인
								if (objType == ENUM_FORM_ID::kWEAP) {
									TESObjectWEAP* weapon = (TESObjectWEAP*)obj;
									if (weapon->equipSlot && weapon->equipSlot == GrenadeSlot) {
										if (gLootFeaturedItem->value == 0 && HasKeywordVM(vm, 0, obj, FeaturedItem))
											continue;

										contStruct.push_back(contLootStruct(bItem, false));
										continue;
									}
									// 탄약. 기본 필터 검사만 함
								} else if (objType == ENUM_FORM_ID::kAMMO && gLootAmmo->value == 1) {
									if (!ammoGetList.empty()) {
										if (std::find(ammoGetList.begin(), ammoGetList.end(), obj) != ammoGetList.end()) {
											contStruct.push_back(contLootStruct(bItem, false));
											continue;
										}
									}

									if (!ammoSkipList.empty()) {
										if (std::find(ammoSkipList.begin(), ammoSkipList.end(), obj) != ammoSkipList.end()) {
											continue;
										}
									}
									contStruct.push_back(contLootStruct(bItem, false));
									continue;
									// 잡동사니. MCM 필터 확인과 스크랩 가능 잡동사니인지 확인함
								} else if (objType == ENUM_FORM_ID::kMISC) {
									if (!miscGetList.empty()) {
										if (std::find(miscGetList.begin(), miscGetList.end(), obj) != miscGetList.end()) {
											contStruct.push_back(contLootStruct(bItem, false));
											continue;
										}
									}

									if (gLootFeaturedItem->value == 0 && HasKeywordVM(vm, 0, obj, FeaturedItem)) {
										continue;
									}

									if (std::find(compoSkip_unorderdSet.begin(), compoSkip_unorderdSet.end(), obj) != compoSkip_unorderdSet.end()) {
										continue;
									}

									if (!miscSkipList.empty()) {
										if (std::find(miscSkipList.begin(), miscSkipList.end(), obj) != miscSkipList.end()) {
											continue;
										}
									}

									// 스크랩설정과 잡동사니의 스크랩 재료 정보를 확인하고 스크랩 NPC로 보냄
									if (gLootScrap->value == 0) {
										contStruct.push_back(contLootStruct(bItem, false));
										continue;
									} else {
										BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* checkCompo = ((TESObjectMISC*)obj)->componentData;
										bool isCompoValid = checkCompo && !checkCompo->empty();

										if (!isCompoValid || HasKeywordVM(vm, 0, obj, UnscrappableObject) || HasKeywordVM(vm, 0, obj, FeaturedItem)) {
											contStruct.push_back(contLootStruct(bItem, false));
											continue;
										} else {
											contStruct.push_back(contLootStruct(bItem, true));
											continue;
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

									if (HasKeywordVM(vm, 0, obj, ObjectTypeChem) || ((MagicItem*)obj)->IsMedicine()) {  // 플래그가 의약품인지 확인함
										contStruct.push_back(contLootStruct(bItem, false));
										continue;
									} else if (gLootNuka->value == 1 && HasKeywordVM(vm, 0, obj, ObjectTypeNukaCola)) {  // 누카콜라 키워드 확인
										contStruct.push_back(contLootStruct(bItem, false));
										continue;
									} else if (gLootAlcohol->value == 1 && HasKeywordVM(vm, 0, obj, Alcohol)) {  // 알코올 키워드 확인
										contStruct.push_back(contLootStruct(bItem, false));
										continue;
									} else if (gLootMeat->value == 1 && GetCraftingUseSound(obj) && GetCraftingUseSound(obj) == meatCheck) {
										contStruct.push_back(contLootStruct(bItem, false));  // 크래프팅 사운드가 고기소리인지 확인
										continue;
									} else if (gLootIngredient->value == 1 && HasKeywordVM(vm, 0, obj, FruitOrVegetable)) {
										contStruct.push_back(contLootStruct(bItem, false));  // 음식재료 키워드 확인
										continue;
									} else if (gLootRestore->value == 1) {
										BSTArray<EffectItem*> eList = ((MagicItem*)obj)->listOfEffects;
										if (!eList.empty()) {
											bool isLootAlch = false;
											float healAmount = 0;
											float radAmount = 0;

											for (EffectItem* effect : eList) {
												EffectSetting* checkEffect = effect->effectSetting;
												if (checkEffect == RestoreHealthFood) {
													isLootAlch = true;
													if (healAmount <= 0) {
														healAmount = effect->data.magnitude * effect->data.duration;
													} else {
														continue;
													}

													if (healAmount < gLootRestoreAmount->value) {  // 회복량 확인
														isLootAlch = false;                        // 회복량 낮으면 루팅안함
														break;
													}
												} else if (checkEffect == DamageRadiationChem) {
													if (gLootFood->value == 0) {
														isLootAlch = false;  // 방사능이 있고 mcm에서 안켰으면 루팅 안함
														break;
													} else {
														radAmount = effect->data.magnitude;
													}
												}
											}

											if (isLootAlch) {
												float radMult = 5 + (gLoodRadMult->value * 5);

												if (radAmount <= 0 || radMult >= 35) {
													contStruct.push_back(contLootStruct(bItem, false));
												} else if ((radAmount / healAmount) * 100 <= radMult) {
													contStruct.push_back(contLootStruct(bItem, false));
												}
											}
										}
									}
								}
							}

							int saveCount = contStruct.size();

							if (saveCount == 0)
								continue;

							for (int i = saveCount - 1; i >= 0; --i) {
								BGSInventoryItem bItem = contStruct[i].item;
								TESBoundObject* obj = bItem.object;

								if (!contStruct[i].bScrap) {  // 스크랩 상자로 보내는 작업인지 확인
									++getCount;
									tempObj.form = obj;
									RemoveItemVM(vm, 0, ref, tempObj, bItem.GetCount(), true, p);
								} else {
									++scrapCount;
									tempObj.form = obj;
									RemoveItemVM(vm, 0, ref, tempObj, bItem.GetCount(), true, scrapActor);
								}
							}
						}
					}
					//// 필드 아이템 처리부분.
				} else if (type == ENUM_FORM_ID::kWEAP) {
					TESFullName* refBaseName = (TESFullName*)refBase;
					if (!refBaseName || TESFullName::GetFullName(*refBase, false).empty()) {
						continue;
					}

					TESObjectWEAP* weapon = (TESObjectWEAP*)refBase;
					if (weapon->equipSlot && weapon->equipSlot == GrenadeSlot) {
						if (gLootFeaturedItem->value == 0 && ref->HasKeyword(FeaturedItem)) {
							continue;
						} else {
							++getCount;
							tempObj.form = ref;
							AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
							continue;
						}
					}
				} else if (type == ENUM_FORM_ID::kAMMO && gLootAmmo->value == 1) {
					TESFullName* refBaseName = (TESFullName*)refBase;
					if (!refBaseName || TESFullName::GetFullName(*refBase, false).empty()) {
						continue;
					}

					if (!ammoGetList.empty()) {
						if (std::find(ammoGetList.begin(), ammoGetList.end(), refBase) != ammoGetList.end()) {
							++getCount;
							tempObj.form = ref;
							AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
							continue;
						}
					}

					if (!ammoSkipList.empty()) {
						if (std::find(ammoSkipList.begin(), ammoSkipList.end(), refBase) != ammoSkipList.end()) {
							continue;
						}
					}

					++getCount;
					tempObj.form = ref;
					AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
					continue;
				} else if (type == ENUM_FORM_ID::kMISC) {
					TESFullName* refBaseName = (TESFullName*)refBase;
					if (!refBaseName || TESFullName::GetFullName(*refBase, false).empty()) {
						continue;
					}
						
					if (!miscGetList.empty()) {
						if (std::find(miscGetList.begin(), miscGetList.end(), refBase) != miscGetList.end()) {
							++getCount;
							tempObj.form = ref;
							AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
							continue;
						}
					}

					if (gLootFeaturedItem->value == 0 && ref->HasKeyword(FeaturedItem)) {
						continue;
					}

					if (compoSkip_unorderdSet.find(refBase) != compoSkip_unorderdSet.end()) {
						continue;
					}

					if (!miscSkipList.empty()) {
						if (std::find(miscSkipList.begin(), miscSkipList.end(), refBase) != miscSkipList.end()) {
							continue;
						}
					}

					if (gLootScrap->value == 0) {
						++getCount;
						tempObj.form = ref;
						AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
						continue;
					} else {
						BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* checkCompo = ((TESObjectMISC*)refBase)->componentData;
						bool isCompoValid = checkCompo && !checkCompo->empty();

						if (!isCompoValid || ref->HasKeyword(UnscrappableObject) || ref->HasKeyword(FeaturedItem)) {
							++getCount;
							tempObj.form = ref;
							AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
							continue;
						} else {
							++scrapCount;
							tempObj.form = ref;
							AddItemVM(vm, 0, scrapActor, tempObj, GetCount(ref), true);
							continue;
						}
					}
				} else if (type == ENUM_FORM_ID::kALCH) {
					TESFullName* refBaseName = (TESFullName*)refBase;
					if (!refBaseName || TESFullName::GetFullName(*refBase, false).empty()) {
						continue;
					}

					if (!alchGetList.empty()) {
						if (std::find(alchGetList.begin(), alchGetList.end(), refBase) != alchGetList.end()) {
							++getCount;
							tempObj.form = ref;
							AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
							continue;
						}
					}

					if (gLootFeaturedItem->value == 0 && ref->HasKeyword(FeaturedItem))
						continue;

					if (!alchSkipList.empty()) {
						if (std::find(alchSkipList.begin(), alchSkipList.end(), refBase) != alchSkipList.end()) {
							continue;
						}
					}

					if (ref->HasKeyword(ObjectTypeChem) || ((MagicItem*)refBase)->IsMedicine()) {
						++getCount;
						tempObj.form = ref;
						AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
						continue;
					} else if (gLootNuka->value == 1 && ref->HasKeyword(ObjectTypeNukaCola)) {
						++getCount;
						tempObj.form = ref;
						AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
						continue;
					} else if (gLootAlcohol->value == 1 && ref->HasKeyword(Alcohol)) {
						++getCount;
						tempObj.form = ref;
						AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
						continue;
					} else if (gLootMeat->value == 1 && GetCraftingUseSound(refBase) && GetCraftingUseSound(refBase) == meatCheck) {
						++getCount;
						tempObj.form = ref;
						AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
						continue;
					} else if (gLootIngredient->value == 1 && ref->HasKeyword(FruitOrVegetable)) {
						++getCount;
						tempObj.form = ref;
						AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
						continue;
					} else if (gLootRestore->value == 1) {
						BSTArray<EffectItem*> eList = ((MagicItem*)refBase)->listOfEffects;
						if (!eList.empty()) {
							bool isLootAlch = false;
							float healAmount = 0;
							float radAmount = 0;

							for (EffectItem* effect : eList) {
								EffectSetting* checkEffect = effect->effectSetting;
								if (checkEffect == RestoreHealthFood) {
									isLootAlch = true;
									if (healAmount <= 0) {
										healAmount = effect->data.magnitude * effect->data.duration;
									} else {
										continue;
									}

									if (healAmount < gLootRestoreAmount->value) {  // 회복량 확인
										isLootAlch = false;                        // 회복량 낮으면 루팅안함
										break;
									}
								} else if (checkEffect == DamageRadiationChem) {
									if (gLootFood->value == 0) {
										isLootAlch = false;  // 방사능이 있고 mcm에서 안켰으면 루팅 안함
										break;
									} else {
										radAmount = effect->data.magnitude;
									}
								}
							}

							if (isLootAlch) {
								float radMult = 5 + (gLoodRadMult->value * 5);

								if (radAmount <= 0 || radMult >= 35) {
									++getCount;
									tempObj.form = ref;
									AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
									continue;
								} else if ((radAmount / healAmount) * 100 <= radMult) {
									++getCount;
									tempObj.form = ref;
									AddItemVM(vm, 0, p, tempObj, GetCount(ref), true);
									continue;
								}
							}
						}
					}
				}
			}
		}
	}
}

// 스크랩박스에 있는 잡동사니를 스크랩함
uint32_t scrapMISC(std::monostate)
{
	BGSInventoryList* temp = scrapActor->inventoryList;

	uint32_t scrapCount = 0;

	BSTArray<BGSInventoryItem> miscList = temp->data;
	if (!miscList.empty()) {
		for (BGSInventoryItem bItem : miscList) {
			TESBoundObject* obj = bItem.object;

			stl::enumeration<ENUM_FORM_ID, std::uint8_t> type = obj->formType;

			if (obj->formType != ENUM_FORM_ID::kMISC) {
				continue;  // 잡동사니가 아니면 리턴
			}

			BSTArray<BSTTuple<TESForm*, BGSTypedFormValuePair::SharedVal>>* compoDatas = ((TESObjectMISC*)obj)->componentData;
			if (!compoDatas || compoDatas->empty()) {
				continue;  // 재료 배열이 없거나 비어있으면 리턴
			}

			for (int i = 0; i < compoDatas->size(); ++i) {
				auto& tuple = (*compoDatas)[i];

				TESForm* compoForm = tuple.first;
				uint32_t count = tuple.second.i;

				if (!compoForm) {
					continue;
				}

				FormOrInventoryObj tempObj;
				tempObj.form = compoForm;
				AddItemVM(vm, 0, p, tempObj, count, true);

				scrapCount += count;
			}
		}
	}

	return scrapCount;
}

// 게임 시작시 팩션에 있는 머천트 컨테이너를 검색해서 배열에 넣음
void saveVendorContainer()
{
	auto& AllFactionForms = DH->GetFormArray<RE::TESFaction>();

	for (RE::TESFaction* faction : AllFactionForms) {
		if (faction) {
			TESObjectREFR* vendorChest = faction->vendorData.merchantContainer;
			if (vendorChest) {
				vendorContainers.insert(vendorChest);
			}
		}
	}
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

	BSTArray<TESQuest*> childRen = questNode->children;
	if (!childRen.empty()) {
		int i = childRen.size() - 1;
		while (i >= 0) {
			RemoveChild(questNode, childRen[i], 0);
			--i;
		}
	}

	//auto readConfigStart = std::chrono::high_resolution_clock::now();

	std::vector<TESObjectREFR*> refArray;

	NiPoint3 point = p->GetPosition();
	EnumReferencesCloseToRef(DH, p, gLootRange->value, &point, gLootRange->value, nullptr, &refArray);

	runLooting_Slow(refArray);

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

	AddChild(questNode, lootQuest, 0);

	bLootRunning = false;
}

void createFileIfNotExist(const std::string& filePath)
{
	// 파일이 존재하지 않으면 생성
	if (!fs::exists(filePath)) {
		std::ofstream outFile(filePath);
	}
}

// 핍보이 잡동사니 탭에서 구성요소에 태그를 붙이거나 뗄때를 감지하고 호출됨
class MyEventSink : public BSTEventSink<FavoriteMgr_Events::ComponentFavoriteEvent> 
{
public:

	virtual BSEventNotifyControl ProcessEvent([[maybe_unused]] const FavoriteMgr_Events::ComponentFavoriteEvent& a_event, BSTEventSource<FavoriteMgr_Events::ComponentFavoriteEvent>*) override
	{
		if (gPipboyComponentSYNC->value == 1 && !runingScriptToggle) {
			//logger::info("눌렀음");
			processingTagSYNC_ToPipboy();
		}
		return BSEventNotifyControl::kContinue;
	}
};

// 핍보이 구성요소 태그 이벤트 등록 + 컴포넌트 폼리스트를 벡터배열에 저장함
void RegisterCustomSink()
{
	// public BSTEventSource< > 가 있는 싱글톤을 가져옴
	favoritesManager = RE::FavoritesManager::GetSingleton();

	// 이벤트 후킹용 클래스를 동적으로 생성
	MyEventSink* mySink = new MyEventSink();

	// 싱크를 FavoritesManager에 등록
	favoritesManager->RegisterSink(mySink);

	// 나중에 메모리를 해제할 때는 delete로 객체를 해제해야 합니다.
	// delete mySink;  // 적절한 시점에 메모리 해제

	// 폼리스트의 구성요소를 벡터 배열로 복사함
	for (TESForm* component : componentMaterialList->arrayOfForms) {
		if (component) {
			componentMaterials.push_back(component);
		}
	}
}

void setPipboyTagSYNC(std::monostate)
{
	processingTagSYNC_ToPipboy();
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

			gLootRange = (TESGlobal*)DH->LookupForm(0x801, "LootAroundJunk.esp");
			gLootYES = (TESGlobal*)DH->LookupForm(0x800, "LootAroundJunk.esp");
			gLootBox = (TESGlobal*)DH->LookupForm(0x802, "LootAroundJunk.esp");
			gLootActor = (TESGlobal*)DH->LookupForm(0x809, "LootAroundJunk.esp");
			gLootScrap = (TESGlobal*)DH->LookupForm(0x804, "LootAroundJunk.esp");
			gLootPrint = (TESGlobal*)DH->LookupForm(0x806, "LootAroundJunk.esp");
			gLootFeaturedItem = (TESGlobal*)DH->LookupForm(0x865, "LootAroundJunk.esp");
			gPipboyComponentSYNC = (TESGlobal*)DH->LookupForm(0x88A, "LootAroundJunk.esp");

			GrenadeSlot = (BGSEquipSlot*)DH->LookupForm(0x00046AAC, "Fallout4.esm");

			FeaturedItem = (BGSKeyword*)DH->LookupForm(0x01B3FAC, "Fallout4.esm");
			UnscrappableObject = (BGSKeyword*)DH->LookupForm(0x01CC46A, "Fallout4.esm");
			Alcohol = (BGSKeyword*)DH->LookupForm(0x0010C416, "Fallout4.esm");
			gLootAlcohol = (TESGlobal*)DH->LookupForm(0x830, "LootAroundJunk.esp");

			componentMaterialList = (BGSListForm*)DH->LookupForm(0x81A, "LootAroundJunk.esp");
			componentScrapList = (BGSListForm*)DH->LookupForm(0x81B, "LootAroundJunk.esp");
			componentFilterCont_Added = (TESObjectREFR*)DH->LookupForm(0x87E, "LootAroundJunk.esp");
			componentFilterCont_Removed = (TESObjectREFR*)DH->LookupForm(0x87F, "LootAroundJunk.esp");
			componentFilterList = (BGSListForm*)DH->LookupForm(0x888, "LootAroundJunk.esp");

			gLootAmmo = (TESGlobal*)DH->LookupForm(0x829, "LootAroundJunk.esp");
			gLootNuka = (TESGlobal*)DH->LookupForm(0x82C, "LootAroundJunk.esp");
			workshopKeyword = (BGSKeyword*)DH->LookupForm(0x054BA7, "Fallout4.esm");
			ObjectTypeNukaCola = (BGSKeyword*)DH->LookupForm(0x00249F30, "Fallout4.esm");
			gLootMeat = (TESGlobal*)DH->LookupForm(0x82D, "LootAroundJunk.esp");
			meatCheck = (BGSSoundDescriptorForm*)DH->LookupForm(0x11286F, "Fallout4.esm");
			gLootIngredient = (TESGlobal*)DH->LookupForm(0x82F, "LootAroundJunk.esp");
			FruitOrVegetable = (BGSKeyword*)DH->LookupForm(0x1C63E5, "Fallout4.esm");
			gLootFood = (TESGlobal*)DH->LookupForm(0x82E, "LootAroundJunk.esp");
			ObjectTypeFood = (BGSKeyword*)DH->LookupForm(0x00055ECC, "Fallout4.esm");
			ObjectTypeChem = (BGSKeyword*)DH->LookupForm(0x000F4AE7, "Fallout4.esm");
			gLootRestore = (TESGlobal*)DH->LookupForm(0x870, "LootAroundJunk.esp");
			gLootRestoreAmount = (TESGlobal*)DH->LookupForm(0x872, "LootAroundJunk.esp");
			gLoodRadMult = (TESGlobal*)DH->LookupForm(0x873, "LootAroundJunk.esp");
			DamageRadiationChem = (EffectSetting*)DH->LookupForm(0x024056, "Fallout4.esm");
			RestoreHealthFood = (EffectSetting*)DH->LookupForm(0x0397E, "Fallout4.esm");

			getMessage = (BGSMessage*)DH->LookupForm(0x0811, "LootAroundJunk.esp");
			sendMessage = (BGSMessage*)DH->LookupForm(0x0812, "LootAroundJunk.esp");
			twoMessage = (BGSMessage*)DH->LookupForm(0x0810, "LootAroundJunk.esp");

			loadFilterSettingsFromFiles();  // 필터를 txt 파일에서 불러옴

			// 필터 파일을 확인하고 없으면 만듬
			std::string getName = "Get.txt";
			std::string skipName = "Skip.txt";

			// Get.txt와 Skip.txt 경로 생성
			std::string getPath = lootDir + getName;
			std::string skipPath = lootDir + skipName;

			// 파일이 없으면 생성
			createFileIfNotExist(getPath);
			createFileIfNotExist(skipPath);

			RegisterCustomSink(); // 핍보이 구성요소 태그 이벤트 등록

			break;
		}
	case F4SE::MessagingInterface::kPostLoadGame:
		{
			setComponentFilter(std::monostate{});  // 잡동사니의 구성요소에 대한 필터를 removed 상자에 든 아이템으로 구성함
			break;
		}
	case F4SE::MessagingInterface::kNewGame:
		{
			setComponentFilter(std::monostate{});
			break;
		}
	}
}

bool RegisterPapyrusFunctions(RE::BSScript::IVirtualMachine* a_vm)
{
	vm = a_vm;

	//REL::IDDatabase::Offset2ID o2i;
	//logger::info("0x0x499f0: {}", o2i(0x499f0));

	//std::size_t offset = REL::IDDatabase::get().id2offset(1067039);
	//logger::info("Offset for ID 1067039: {}", offset);

	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "StartLoot"sv, StartLoot);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "FilterContainerSetting"sv, FilterContainerSetting);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "FillContainerfromFile"sv, FillContainerfromFile);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "setComponentFilter"sv, setComponentFilter);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "scrapMISC"sv, scrapMISC);

	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "fillContainer_ComponentFilter_Added"sv, fillContainer_ComponentFilter_Added);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "fillContainer_ComponentFilter_Added_to_Removed"sv, fillContainer_ComponentFilter_Added_to_Removed);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "fillContainer_ComponentFilter_Removed"sv, fillContainer_ComponentFilter_Removed);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "fillContainer_ComponentFilter_Removed_Close"sv, fillContainer_ComponentFilter_Removed_Close);
	a_vm->BindNativeMethod("LAJ_LootF4SE"sv, "setPipboyTagSYNC"sv, setPipboyTagSYNC);

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
