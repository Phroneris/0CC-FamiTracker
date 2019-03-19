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

#include "ConfigGeneral.h"
#include "FamiTracker.h"
#include "MainFrm.h"		// // //
#include "FamiTrackerView.h"		// // //
#include "Settings.h"
#include "ft0cc/enum_traits.h"		// // //

const LPCWSTR CConfigGeneral::CONFIG_STR[] = {		// // //	/// jp
	L"カーソル回り込み（横）",
	L"カーソル飛び越し（縦）",
	L"フリーカーソルモード",
	L"WAVE ファイルをプレビューする",
	L"キーリピート",
	L"行数を 16 進数で表示する",
	L"前後のフレームをプレビュー表示する",
	L"音停止時に DPCM をリセットしない",
	L"カーソル移動はステップを無視する",
	L"Delete キーで行詰めする",
	L"ファイルをバックアップする",
	L"同時起動を禁止する",
	L"行全体をプレビュー再生する",
	L"ダブルクリックで選択しない",
	L"値の増減を回り込ませる",
	L"低音量を切り捨てる",
	L"旧版 FDS 音量テーブルを使用する",
	L"チャンネルステータスを遡上取得する",
	L"フレーム外まで貼り付ける",
	L"省略された行を表示する",
	L"テンキーで 16 進数を入力する",
	L"フレーム外まで選択する",
	L"起動時に最新版を確認する",
};

const LPCWSTR CConfigGeneral::CONFIG_DESC[] = {		// // //	/// jp
	L"パターンエディタで、カーソルが左右端を回り込んで移動できるようにします。",
	L"パターンエディタで、カーソルがフレームを越えて移動できるようにします。",
	L"カーソルを強制的に画面の中央に位置させる機能をオフにします。",
	L"サンプルをモジュールに読み込む際に、ファイルを開くダイアログで WAVE ファイルや DPCM ファイルをプレビュー再生します。",
	L"パターンエディタで、キーリピート（キー押しっ放しによる連続入力）を有効にします。",
	L"ステータスバーにおける行やフレームの数表示を 16 進数にします。",
	L"パターンエディタで、前後のフレームを薄くプレビュー表示します。",
	L"DPCM サンプルのプレビュー後、DPCM チャンネルをリセットしません。編集時の不快なポップノイズをなくします。",
	L"ステップ設定をノート入力にのみ反映させ、カーソル移動はステップ数 1 で行います。",
	L"Delete キーで行を丸ごと削除し、上に詰めます。Shift+Delete キーと同じ効果になります。",
	L"モジュールの保存時に既存ファイルの複製バックアップを作成します。",
	L"0CC-FamiTracker を単一起動でのみ実行します。",
	L"パターンエディタで、ノート挿入時に全チャンネルをプレビュー再生します。",
	L"パターンエディタで、ダブルクリックによる全選択機能をオフにします。",
	L"Shift+マウスホイールで値を変更する際、上下限に達した値を回り込ませます。",
	L"Axy エフェクトや 7xy エフェクトにおいて、1 未満の音量を切り捨てます。デフォルトでは 1 に切り上げられています。",
	L"FDS 音量テーブルについて、NSF 書き出しに用いられているものより正確な音量テーブルを採用します。",
	L"現在位置より前のチャンネルステータスを再現して再生します。1 行だけ再生する場合はこの限りではありません。",
	L"データの貼り付けをフレーム外にまで及ばせます。",
	L"Bxx、Cxx、Dxx エフェクトで省略される行を表示します。",
	L"テンキーの特殊キーを 16 進数入力に割り当てます。ショートカットキーと重複しない必要があります。",
	L"フレームを越えたパターン選択を有効にします。",
	L"インターネット接続が可能な場合、ソフト起動時に 0CC-FamiTracker の最新版を確認します。",
};

// CConfigGeneral dialog

IMPLEMENT_DYNAMIC(CConfigGeneral, CPropertyPage)
CConfigGeneral::CConfigGeneral()
	: CPropertyPage(CConfigGeneral::IDD)
{
}

CConfigGeneral::~CConfigGeneral()
{
}

void CConfigGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigGeneral, CPropertyPage)
	ON_CBN_EDITUPDATE(IDC_PAGELENGTH, OnCbnEditupdatePagelength)
	ON_CBN_SELENDOK(IDC_PAGELENGTH, OnCbnSelendokPagelength)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnCbnSelchangeComboStyle)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CONFIG_LIST, OnLvnItemchangedConfigList)
END_MESSAGE_MAP()


// CConfigGeneral message handlers

BOOL CConfigGeneral::OnSetActive()
{
	SetDlgItemInt(IDC_PAGELENGTH, m_iPageStepSize, FALSE);
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_STYLE))->SetCurSel(m_iEditStyle);		// // //

	return CPropertyPage::OnSetActive();
}

void CConfigGeneral::OnOK()
{
	CPropertyPage::OnOK();
}

BOOL CConfigGeneral::OnApply()
{
	// Translate page length
	BOOL Trans;

	m_iPageStepSize = GetDlgItemInt(IDC_PAGELENGTH, &Trans, FALSE);

	if (Trans == FALSE)
		m_iPageStepSize = 4;
	else if (m_iPageStepSize > 256 /*MAX_PATTERN_LENGTH*/)
		m_iPageStepSize = 256 /*MAX_PATTERN_LENGTH*/;

	if (m_bCheckVersion && !theApp.GetSettings()->General.bCheckVersion)		// // //
		theApp.CheckNewVersion(false);

	auto *pSettings = theApp.GetSettings();		// // //
	pSettings->General.bWrapCursor			= m_bWrapCursor;
	pSettings->General.bWrapFrames			= m_bWrapFrames;
	pSettings->General.bFreeCursorEdit		= m_bFreeCursorEdit;
	pSettings->General.bWavePreview			= m_bPreviewWAV;
	pSettings->General.bKeyRepeat			= m_bKeyRepeat;
	pSettings->General.bRowInHex			= m_bRowInHex;
	pSettings->General.iEditStyle			= enum_cast<edit_style_t>(m_iEditStyle);
	pSettings->General.bFramePreview		= m_bFramePreview;
	pSettings->General.bNoDPCMReset			= m_bNoDPCMReset;
	pSettings->General.bNoStepMove			= m_bNoStepMove;
	pSettings->General.iPageStepSize		= m_iPageStepSize;
	pSettings->General.bPullUpDelete		= m_bPullUpDelete;
	pSettings->General.bBackups				= m_bBackups;
	pSettings->General.bSingleInstance		= m_bSingleInstance;
	pSettings->General.bPreviewFullRow		= m_bPreviewFullRow;
	pSettings->General.bDblClickSelect		= m_bDisableDblClick;
	// // //
	pSettings->General.bWrapPatternValue	= m_bWrapPatternValue;
	pSettings->General.bCutVolume			= m_bCutVolume;
	pSettings->General.bFDSOldVolume		= m_bFDSOldVolume;
	pSettings->General.bRetrieveChanState	= m_bRetrieveChanState;
	pSettings->General.bOverflowPaste		= m_bOverflowPaste;
	pSettings->General.bShowSkippedRows		= m_bShowSkippedRows;
	pSettings->General.bHexKeypad			= m_bHexKeypad;
	pSettings->General.bMultiFrameSel		= m_bMultiFrameSel;
	pSettings->General.bCheckVersion		= m_bCheckVersion;

	pSettings->Keys.iKeyNoteCut				= m_iKeyNoteCut;
	pSettings->Keys.iKeyNoteRelease			= m_iKeyNoteRelease;
	pSettings->Keys.iKeyClear				= m_iKeyClear;
	pSettings->Keys.iKeyRepeat				= m_iKeyRepeat;
	pSettings->Keys.iKeyEchoBuffer			= m_iKeyEchoBuffer;		// // //

	theApp.GetMainFrame()->GetTrackerView()->UpdateSongView();		// // //

	return CPropertyPage::OnApply();
}

BOOL CConfigGeneral::OnInitDialog()
{
	WCHAR Text[64] = { };

	CPropertyPage::OnInitDialog();

	const auto *pSettings = theApp.GetSettings();		// // //

	m_bWrapCursor			= pSettings->General.bWrapCursor;
	m_bWrapFrames			= pSettings->General.bWrapFrames;
	m_bFreeCursorEdit		= pSettings->General.bFreeCursorEdit;
	m_bPreviewWAV			= pSettings->General.bWavePreview;
	m_bKeyRepeat			= pSettings->General.bKeyRepeat;
	m_bRowInHex				= pSettings->General.bRowInHex;
	m_iEditStyle			= value_cast(pSettings->General.iEditStyle);
	m_bFramePreview			= pSettings->General.bFramePreview;
	m_bNoDPCMReset			= pSettings->General.bNoDPCMReset;
	m_bNoStepMove			= pSettings->General.bNoStepMove;
	m_iPageStepSize			= pSettings->General.iPageStepSize;
	m_bPullUpDelete			= pSettings->General.bPullUpDelete;
	m_bBackups				= pSettings->General.bBackups;
	m_bSingleInstance		= pSettings->General.bSingleInstance;
	m_bPreviewFullRow		= pSettings->General.bPreviewFullRow;
	m_bDisableDblClick		= pSettings->General.bDblClickSelect;
	// // //
	m_bWrapPatternValue		= pSettings->General.bWrapPatternValue;
	m_bCutVolume			= pSettings->General.bCutVolume;
	m_bFDSOldVolume			= pSettings->General.bFDSOldVolume;
	m_bRetrieveChanState	= pSettings->General.bRetrieveChanState;
	m_bOverflowPaste		= pSettings->General.bOverflowPaste;
	m_bShowSkippedRows		= pSettings->General.bShowSkippedRows;
	m_bHexKeypad			= pSettings->General.bHexKeypad;
	m_bMultiFrameSel		= pSettings->General.bMultiFrameSel;
	m_bCheckVersion			= pSettings->General.bCheckVersion;

	m_iKeyNoteCut			= pSettings->Keys.iKeyNoteCut;
	m_iKeyNoteRelease		= pSettings->Keys.iKeyNoteRelease;
	m_iKeyClear				= pSettings->Keys.iKeyClear;
	m_iKeyRepeat			= pSettings->Keys.iKeyRepeat;
	m_iKeyEchoBuffer		= pSettings->Keys.iKeyEchoBuffer;		// // //

	GetKeyNameTextW(MapVirtualKey(m_iKeyNoteCut, MAPVK_VK_TO_VSC) << 16, Text, std::size(Text));
	SetDlgItemTextW(IDC_KEY_NOTE_CUT, Text);
	GetKeyNameTextW(MapVirtualKey(m_iKeyNoteRelease, MAPVK_VK_TO_VSC) << 16, Text, std::size(Text));
	SetDlgItemTextW(IDC_KEY_NOTE_RELEASE, Text);
	GetKeyNameTextW(MapVirtualKey(m_iKeyClear, MAPVK_VK_TO_VSC) << 16, Text, std::size(Text));
	SetDlgItemTextW(IDC_KEY_CLEAR, Text);
	GetKeyNameTextW(MapVirtualKey(m_iKeyRepeat, MAPVK_VK_TO_VSC) << 16, Text, std::size(Text));
	SetDlgItemTextW(IDC_KEY_REPEAT, Text);
	GetKeyNameTextW(MapVirtualKey(m_iKeyEchoBuffer, MAPVK_VK_TO_VSC) << 16, Text, std::size(Text));		// // //
	SetDlgItemTextW(IDC_KEY_ECHO_BUFFER, Text);

	EnableToolTips(TRUE);

	m_wndToolTip.Create(this, TTS_ALWAYSTIP);
	m_wndToolTip.Activate(TRUE);

	CWnd *pWndChild = GetWindow(GW_CHILD);
	CStringW strToolTip;

	while (pWndChild) {
		int nID = pWndChild->GetDlgCtrlID();
		if (strToolTip.LoadStringW(nID)) {
			m_wndToolTip.AddTool(pWndChild, strToolTip);
		}
		pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
	}

	const bool CONFIG_BOOL[SETTINGS_BOOL_COUNT] = {		// // //
		m_bWrapCursor,
		m_bWrapFrames,
		m_bFreeCursorEdit,
		m_bPreviewWAV,
		m_bKeyRepeat,
		m_bRowInHex,
		m_bFramePreview,
		m_bNoDPCMReset,
		m_bNoStepMove,
		m_bPullUpDelete,
		m_bBackups,
		m_bSingleInstance,
		m_bPreviewFullRow,
		m_bDisableDblClick,
		m_bWrapPatternValue,
		m_bCutVolume,
		m_bFDSOldVolume,
		m_bRetrieveChanState,
		m_bOverflowPaste,
		m_bShowSkippedRows,
		m_bHexKeypad,
		m_bMultiFrameSel,
		m_bCheckVersion,
	};

	CListCtrl *pList = static_cast<CListCtrl*>(GetDlgItem(IDC_CONFIG_LIST));
	CRect r;		// // //
	pList->GetClientRect(&r);
	pList->DeleteAllItems();
	pList->InsertColumn(0, L"", LVCFMT_LEFT, 20);
	pList->InsertColumn(1, L"Option", LVCFMT_LEFT, r.Width() - 20 - ::GetSystemMetrics(SM_CXHSCROLL));
	pList->SendMessageW(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	pList->SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	for (int i = SETTINGS_BOOL_COUNT - 1; i > -1; --i) {
		pList->InsertItem(0, L"", 0);
		pList->SetCheck(0, CONFIG_BOOL[i]);
		pList->SetItemText(0, 1, CONFIG_STR[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigGeneral::OnCbnEditupdatePagelength()
{
	SetModified();
}

void CConfigGeneral::OnCbnSelendokPagelength()
{
	SetModified();
}

void CConfigGeneral::OnCbnSelchangeComboStyle()		// // //
{
	m_iEditStyle = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_STYLE))->GetCurSel();
	SetModified();
}

void CConfigGeneral::OnLvnItemchangedConfigList(NMHDR *pNMHDR, LRESULT *pResult)		// // //
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CListCtrl *pList = static_cast<CListCtrl*>(GetDlgItem(IDC_CONFIG_LIST));

	auto mask = pNMLV->uNewState & pNMLV->uOldState;		// // // wine compatibility
	pNMLV->uNewState &= ~mask;
	pNMLV->uOldState &= ~mask;

	using T = bool (CConfigGeneral::*);
	const T CONFIG_BOOL[SETTINGS_BOOL_COUNT] = {		// // //
		&CConfigGeneral::m_bWrapCursor,
		&CConfigGeneral::m_bWrapFrames,
		&CConfigGeneral::m_bFreeCursorEdit,
		&CConfigGeneral::m_bPreviewWAV,
		&CConfigGeneral::m_bKeyRepeat,
		&CConfigGeneral::m_bRowInHex,
		&CConfigGeneral::m_bFramePreview,
		&CConfigGeneral::m_bNoDPCMReset,
		&CConfigGeneral::m_bNoStepMove,
		&CConfigGeneral::m_bPullUpDelete,
		&CConfigGeneral::m_bBackups,
		&CConfigGeneral::m_bSingleInstance,
		&CConfigGeneral::m_bPreviewFullRow,
		&CConfigGeneral::m_bDisableDblClick,
		&CConfigGeneral::m_bWrapPatternValue,
		&CConfigGeneral::m_bCutVolume,
		&CConfigGeneral::m_bFDSOldVolume,
		&CConfigGeneral::m_bRetrieveChanState,
		&CConfigGeneral::m_bOverflowPaste,
		&CConfigGeneral::m_bShowSkippedRows,
		&CConfigGeneral::m_bHexKeypad,
		&CConfigGeneral::m_bMultiFrameSel,
		&CConfigGeneral::m_bCheckVersion,
	};

	if (pNMLV->uChanged & LVIF_STATE) {
		if (pNMLV->uNewState & LVNI_SELECTED || pNMLV->uNewState & 0x3000) {
			SetDlgItemTextW(IDC_EDIT_CONFIG_DESC, FormattedW(L"解説: %s", (LPCWSTR)CONFIG_DESC[pNMLV->iItem]));	/// jp

			if (pNMLV->iItem >= 0 && pNMLV->iItem < SETTINGS_BOOL_COUNT)
				pList->SetItemState(pNMLV->iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}

		if (pNMLV->uNewState & 0x3000) {
			SetModified();
			for (int i = 0; i < SETTINGS_BOOL_COUNT; ++i)
				this->*(CONFIG_BOOL[i]) = pList->GetCheck(i) != 0;
		}
	}

	*pResult = 0;
}

BOOL CConfigGeneral::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		WCHAR Text[64] = { };
		int id = GetFocus()->GetDlgCtrlID();
		int key = pMsg->wParam;

		if (key == 27)		// ESC
			key = 0;

		switch (id) {
			case IDC_KEY_NOTE_CUT:
				m_iKeyNoteCut = key;
				break;
			case IDC_KEY_NOTE_RELEASE:
				m_iKeyNoteRelease = key;
				break;
			case IDC_KEY_CLEAR:
				m_iKeyClear = key;
				break;
			case IDC_KEY_REPEAT:
				m_iKeyRepeat = key;
				break;
			case IDC_KEY_ECHO_BUFFER:		// // //
				m_iKeyEchoBuffer = key;
				break;
			default:
				return CPropertyPage::PreTranslateMessage(pMsg);
		}

		GetKeyNameTextW(key ? pMsg->lParam : 0, Text, std::size(Text));
		SetDlgItemTextW(id, Text);

		SetModified();

		return TRUE;
	}

	m_wndToolTip.RelayEvent(pMsg);

	return CPropertyPage::PreTranslateMessage(pMsg);
}
