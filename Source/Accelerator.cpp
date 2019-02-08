/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2018 HertzDevil
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.  To obtain a
** copy of the GNU Library General Public License, write to the Free
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#include "Accelerator.h"
#include "FamiTrackerEnv.h"		// // //
#include "../resource.h"		// // //
#include "SettingsService.h"		// // // stOldSettingContext

/*
 * This class is used to translate key shortcuts -> command messages
 *
 * Translation is now done using win32 functions
 */

// List of modifier strings
const LPCWSTR CAccelerator::MOD_NAMES[] = {		// // //
	L"なし",	/// jp
	L"Alt",
	L"Ctrl",
	L"Ctrl+Alt",
	L"Shift",
	L"Shift+Alt",
	L"Shift+Ctrl",
	L"Shift+Ctrl+Alt",
};

// Default shortcut table
const std::vector<stAccelEntry> CAccelerator::DEFAULT_TABLE {	/// jp
	{L"入力オクターブを下げる",				0,				VK_DIVIDE,		ID_CMD_OCTAVE_PREVIOUS},
	{L"入力オクターブを上げる",				0,				VK_MULTIPLY,	ID_CMD_OCTAVE_NEXT},
	{L"再生/停止",					0,				VK_RETURN,		ID_TRACKER_TOGGLE_PLAY},
	{L"再生",							0,				0,				ID_TRACKER_PLAY},
	{L"最初から再生",				0,				VK_F5,			ID_TRACKER_PLAY_START},
	{L"カーソル位置から再生",				0,				VK_F7,			ID_TRACKER_PLAY_CURSOR},
	{L"Play from row marker",			MOD_CONTROL,	VK_F7,			ID_TRACKER_PLAY_MARKER, L"Play from bookmark"},		// // // 050B
	{L"パターンをループ再生",			0,				VK_F6,			ID_TRACKER_PLAYPATTERN},
	{L"行を再生",						MOD_CONTROL,	VK_RETURN,		ID_TRACKER_PLAYROW},
	{L"停止",							0,				VK_F8,			ID_TRACKER_STOP},
	{L"編集を有効化/無効化",			0,				VK_SPACE,		ID_TRACKER_EDIT},
	{L"Set row marker",					MOD_CONTROL,	'B',			ID_TRACKER_SET_MARKER, L"Set row marker"},		// // // 050B
	{L"ミックス貼り付け",					MOD_CONTROL,	'M',			ID_EDIT_PASTEMIX},
	{L"上書き貼り付け",			0,				0,				ID_EDIT_PASTEOVERWRITE},			// // //
	{L"挿入貼り付け",				0,				0,				ID_EDIT_PASTEINSERT},				// // //
	{L"全選択",						MOD_CONTROL,	'A',			ID_EDIT_SELECTALL},
	{L"選択解除",						0,				VK_ESCAPE,		ID_SELECT_NONE},					// // //
	{L"行を選択",						0,				0,				ID_SELECT_ROW},						// // //
	{L"カラムを選択",					0,				0,				ID_SELECT_COLUMN},					// // //
	{L"パターンを選択",					0,				0,				ID_SELECT_PATTERN},					// // //
	{L"フレームを選択",					0,				0,				ID_SELECT_FRAME},					// // //
	{L"チャンネルを選択",					0,				0,				ID_SELECT_CHANNEL},					// // //
	{L"トラックを選択",					0,				0,				ID_SELECT_TRACK},					// // //
	{L"エディタのフォーカスを切り替え",			0,				0,				ID_SELECT_OTHER},					// // //
	{L"行まで移動",						MOD_ALT,		'G',			ID_EDIT_GOTO},						// // //
	{L"チャンネルをミュート",					MOD_ALT,		VK_F9,			ID_TRACKER_TOGGLECHANNEL},
	{L"チャンネルをソロ",					MOD_ALT,		VK_F10,			ID_TRACKER_SOLOCHANNEL},
	{L"Toggle chip",					MOD_CONTROL|MOD_ALT, VK_F9,		ID_TRACKER_TOGGLECHIP},				// // //
	{L"Solo chip",						MOD_CONTROL|MOD_ALT, VK_F10,	ID_TRACKER_SOLOCHIP},				// // //
	{L"Record to instrument",			0,				0,				ID_TRACKER_RECORDTOINST},			// // //
	{L"数値の補間",					MOD_CONTROL,	'G',			ID_EDIT_INTERPOLATE},
	{L"次のフレームへ",				MOD_CONTROL,	VK_RIGHT,		ID_NEXT_FRAME},
	{L"前のフレームへ",			MOD_CONTROL,	VK_LEFT,		ID_PREV_FRAME},
	{L"ブックマークを切り替え",				MOD_CONTROL,	'K',			ID_BOOKMARKS_TOGGLE},				// // //
	{L"次のブックマークへ",					MOD_CONTROL,	VK_NEXT,		ID_BOOKMARKS_NEXT},					// // //
	{L"前のブックマークへ",				MOD_CONTROL,	VK_PRIOR,		ID_BOOKMARKS_PREVIOUS},				// // //
	{L"半音下げる",		MOD_CONTROL,	VK_F1,			ID_TRANSPOSE_DECREASENOTE},
	{L"半音上げる",		MOD_CONTROL,	VK_F2,			ID_TRANSPOSE_INCREASENOTE},
	{L"1 オクターブ下げる",	MOD_CONTROL,	VK_F3,			ID_TRANSPOSE_DECREASEOCTAVE},
	{L"1 オクターブ上げる",	MOD_CONTROL,	VK_F4,			ID_TRANSPOSE_INCREASEOCTAVE},
	{L"パターン番号を増加",				0,				VK_ADD,			IDC_FRAME_INC},
	{L"パターン番号を減少",				0,				VK_SUBTRACT,	IDC_FRAME_DEC},
	{L"次の音色へ",				MOD_CONTROL,	VK_DOWN,		ID_CMD_NEXT_INSTRUMENT},
	{L"前の音色へ",			MOD_CONTROL,	VK_UP,			ID_CMD_PREV_INSTRUMENT},
	{L"Type instrument number",			0,				0,				ID_CMD_INST_NUM},					// // //
	{L"音色マスク",				MOD_ALT,		'T',			ID_EDIT_INSTRUMENTMASK},
	{L"音量マスク",					MOD_ALT,		'V',			ID_EDIT_VOLUMEMASK},				// // //
	{L"音色を編集",				MOD_CONTROL,	'I',			ID_INSTRUMENT_EDIT},
	{L"ステップ量を増加",				MOD_CONTROL,	VK_ADD,			ID_CMD_INCREASESTEPSIZE},
	{L"ステップ量を減少",				MOD_CONTROL,	VK_SUBTRACT,	ID_CMD_DECREASESTEPSIZE},
	{L"自動スクロール",					0,				VK_SCROLL,		IDC_FOLLOW_TOGGLE},
	{L"現在のフレームを挿入",				MOD_CONTROL,	'D',			ID_MODULE_DUPLICATEFRAME},
	{L"フレームの挿入",					0,				0,				ID_MODULE_INSERTFRAME},
	{L"フレームの削除",					0,				0,				ID_MODULE_REMOVEFRAME},
	{L"反転",						MOD_CONTROL,	'R',			ID_EDIT_REVERSE},
	{L"フレームエディタにフォーカス",			0,				VK_F3,			ID_FOCUS_FRAME_EDITOR},
	{L"パターンエディタにフォーカス",			0,				VK_F2,			ID_FOCUS_PATTERN_EDITOR},
	{L"1 ステップ上へ",				MOD_ALT,		VK_UP,			ID_CMD_STEP_UP},
	{L"1 ステップ下へ",				MOD_ALT,		VK_DOWN,		ID_CMD_STEP_DOWN},
	{L"音色の切り替え",				MOD_ALT,		'S',			ID_EDIT_REPLACEINSTRUMENT},
	{L"コントロールパネルを表示/非表示",			0,				0,				ID_VIEW_CONTROLPANEL},
	{L"エフェクトリストの表示",			0,				0,				ID_HELP_EFFECTTABLE},
	{L"ブロックの先頭を選択",				MOD_ALT,		'B',			ID_BLOCK_START},
	{L"ブロックの末尾を選択",				MOD_ALT,		'E',			ID_BLOCK_END},
	{L"行の設定を取得",			0,				0,				ID_POPUP_PICKUPROW},
	{L"次の楽曲へ",						0,				0,				ID_NEXT_SONG},
	{L"前の楽曲へ",					0,				0,				ID_PREV_SONG},
	{L"間隔を 2 倍に",				0,				0,				ID_EDIT_EXPANDPATTERNS},
	{L"間隔を半分に",				0,				0,				ID_EDIT_SHRINKPATTERNS},
	{L"間隔を編集",				0,				0,				ID_EDIT_STRETCHPATTERNS},			// // //
	{L"フレームの複製挿入",					0,				0,				ID_MODULE_DUPLICATEFRAMEPATTERNS, L"フレームの複製挿入"},		// // //
	{L"パターンの複製",					MOD_ALT,		'D',			ID_MODULE_DUPLICATECURRENTPATTERN, L"パターンの複製"},	// // //
	{L"値の減少（小）",		MOD_SHIFT,		VK_F1,			ID_DECREASEVALUES},
	{L"値の増加（小）",		MOD_SHIFT,		VK_F2,			ID_INCREASEVALUES},
	{L"値の減少（大）",			MOD_SHIFT,		VK_F3,			ID_DECREASEVALUESCOARSE},			// // //
	{L"値の増加（大）",			MOD_SHIFT,		VK_F4,			ID_INCREASEVALUESCOARSE},			// // //
	{L"検索・置換タブを表示/非表示",		MOD_CONTROL,	'F',			ID_EDIT_FIND_TOGGLE},				// // //
	{L"次を検索",						0,				0,				ID_FIND_NEXT},						// // //
	{L"前を検索",					0,				0,				ID_FIND_PREVIOUS},					// // //
	{L"チャンネルステータスの呼び出し",			0,				0,				ID_RECALL_CHANNEL_STATE},			// // //
	{L"圧縮表示",					0,				0,				IDC_COMPACT_TOGGLE},				// // //
};

const int CAccelerator::ACCEL_COUNT = DEFAULT_TABLE.size();

// Registry key
const LPCWSTR CAccelerator::SHORTCUTS_SECTION = L"Shortcuts";		// // //

namespace {

// Translate internal modifier -> windows modifier
BYTE GetMod(int Mod) {
	return ((Mod & MOD_CONTROL) ? FCONTROL : 0) | ((Mod & MOD_SHIFT) ? FSHIFT : 0) | ((Mod & MOD_ALT) ? FALT : 0);
}

} // namespace

// Class instance functions

CAccelerator::CAccelerator() :
	m_pEntriesTable(ACCEL_COUNT),		// // //
	m_pAccelTable(ACCEL_COUNT)		// // //
{
	TRACE(L"Accelerator: Accelerator table contains %d items\n", ACCEL_COUNT);
}

CAccelerator::~CAccelerator()
{
	ASSERT(m_hAccel == nullptr);
}

LPCWSTR CAccelerator::GetItemName(int Item) const
{
	return m_pEntriesTable[Item].name;
}

int CAccelerator::GetItemKey(int Item) const
{
	return m_pEntriesTable[Item].key;
}

int CAccelerator::GetItemMod(int Item) const
{
	return m_pEntriesTable[Item].mod;
}

int CAccelerator::GetDefaultKey(int Item) const
{
	return DEFAULT_TABLE[Item].key;
}

int CAccelerator::GetDefaultMod(int Item) const
{
	return DEFAULT_TABLE[Item].mod;
}

LPCWSTR CAccelerator::GetItemModName(int Item) const
{
	return MOD_NAMES[m_pEntriesTable[Item].mod];
}

LPCWSTR CAccelerator::GetItemKeyName(int Item) const
{
	if (m_pEntriesTable[Item].key > 0) {
		return GetVKeyName(m_pEntriesTable[Item].key);
	}

	return L"なし";	/// jp
}

CStringW CAccelerator::GetVKeyName(int virtualKey) const {		// // //
	unsigned int scanCode = MapVirtualKeyW(virtualKey, MAPVK_VK_TO_VSC);

	// because MapVirtualKey strips the extended bit for some keys
	switch (virtualKey) {
		case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
		case VK_PRIOR: case VK_NEXT: // page up and page down
		case VK_END: case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_DIVIDE:	 // numpad slash
		case VK_NUMLOCK:
			scanCode |= 0x100; // set extended bit
			break;
	}

	WCHAR keyName[50] = { };

	if (GetKeyNameTextW(scanCode << 16, keyName, std::size(keyName)) != 0)
		return keyName;

	return L"";
}

void CAccelerator::StoreShortcut(int Item, int Key, int Mod)
{
	m_pEntriesTable[Item].key = Key;
	m_pEntriesTable[Item].mod = Mod;
}

CStringW CAccelerator::GetShortcutString(int id) const {		// // //
	for (const auto &x : m_pEntriesTable) {		// // //
		if (x.id == id) {
			CStringW KeyName = GetVKeyName(x.key);
			if (KeyName.GetLength() > 1)
				KeyName = KeyName.Mid(0, 1).MakeUpper() + KeyName.Mid(1, KeyName.GetLength() - 1).MakeLower();
			if (x.mod > 0)
				return FormattedW(L"\t%s+%s", MOD_NAMES[x.mod], (LPCWSTR)KeyName);
			else
				return FormattedW(L"\t%s", (LPCWSTR)KeyName);
		}
	}

	return L"";
}

bool CAccelerator::IsKeyUsed(int nChar) const		// // //
{
	return m_iUsedKeys.count(nChar & 0xFF) > 0;
}

// Registry storage/loading

void CAccelerator::SaveShortcuts(CSettings *pSettings) const
{
	// Save values
	auto *App = FTEnv.GetMainApp();		// // //
	for (const auto &x : m_pEntriesTable)		// // //
		App->WriteProfileInt(SHORTCUTS_SECTION, x.name, (x.mod << 8) | x.key);
}

void CAccelerator::LoadShortcuts(CSettings *pSettings)
{
	auto *App = FTEnv.GetMainApp();		// // //

	// Set up names and default values
	LoadDefaults();

	m_iUsedKeys.clear();		// // //

	// Load custom values, if exists
	/*
		location priorities:
		1. HKCU/SOFTWARE/0CC-FamiTracker
		1. HKCU/SOFTWARE/0CC-FamiTracker, original key
		2. HKCU/SOFTWARE/FamiTracker
		3. HKCU/SOFTWARE/FamiTracker, original key (using stAccelEntry::orig_name)
		4. default value
	*/
	for (auto &x : m_pEntriesTable) {		// // //
		int Setting = (x.mod << 8) | x.key;
		{		// // //
			stOldSettingContext s;
			if (x.orig_name != nullptr)		// // //
				Setting = App->GetProfileIntW(SHORTCUTS_SECTION, x.orig_name, Setting);
			Setting = App->GetProfileIntW(SHORTCUTS_SECTION, x.name, Setting);
		}
		if (x.orig_name != nullptr)		// // //
			Setting = App->GetProfileIntW(SHORTCUTS_SECTION, x.orig_name, Setting);
		Setting = App->GetProfileIntW(SHORTCUTS_SECTION, x.name, Setting);

		x.key = Setting & 0xFF;
		x.mod = Setting >> 8;
		if (!x.mod && x.key)		// // //
			m_iUsedKeys.insert(x.key);
	}
}

void CAccelerator::LoadDefaults()
{
	m_pEntriesTable = DEFAULT_TABLE;		// // //
}

void CAccelerator::Setup()
{
	// Translate key table -> windows accelerator table

	if (m_hAccel) {
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}

	m_pAccelTable.clear();		// // //
	for (const auto &x : m_pEntriesTable)
		if (x.key) {
			ACCEL a;
			a.fVirt = FVIRTKEY | GetMod(x.mod);
			a.key = x.key;
			a.cmd = x.id;
			m_pAccelTable.push_back(a);
		}
	m_hAccel = CreateAcceleratorTableW(m_pAccelTable.data(), ACCEL_COUNT);
}

BOOL CAccelerator::Translate(HWND hWnd, MSG *pMsg)
{
	if (m_hAdditionalAccel) {
		if (TranslateAcceleratorW(hWnd, m_hAdditionalAccel, pMsg)) {
			return TRUE;
		}
	}

	if (m_hAccel) {
		if (TranslateAcceleratorW(hWnd, m_hAccel, pMsg)) {
			return TRUE;
		}
	}

	return FALSE;
}

void CAccelerator::Shutdown()
{
	if (m_hAccel) {
		DestroyAcceleratorTable(m_hAccel);
		m_hAccel = NULL;
	}
}

void CAccelerator::SetAccelerator(HACCEL hAccel)
{
	m_hAdditionalAccel = hAccel;
}
