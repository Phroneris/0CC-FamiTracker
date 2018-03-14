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


#pragma once

#include "stdafx.h"		// // //
#include <memory>		// // //
#include <vector>		// // //

// CSettings command target

enum EDIT_STYLES {		// // // renamed
	EDIT_STYLE_FT2 = 0,		// FT2
	EDIT_STYLE_MPT = 1,		// ModPlug
	EDIT_STYLE_IT = 2,		// IT
};

enum WIN_STATES {
	STATE_NORMAL,
	STATE_MAXIMIZED,
};

enum PATHS {
	PATH_FTM,
	PATH_FTI,
	PATH_NSF,
	PATH_DMC,
	PATH_WAV,

	PATH_COUNT,
};

// // // helper class for loading settings from official famitracker
struct stOldSettingContext
{
	stOldSettingContext();
	~stOldSettingContext();
};

// Base class for settings, pure virtual
class CSettingBase {
public:
	CSettingBase(LPCWSTR pSection, LPCWSTR pEntry) : m_pSection(pSection), m_pEntry(pEntry) { }
	virtual ~CSettingBase() noexcept = default;		// // //
	virtual void Load() = 0;
	virtual void Save() = 0;
	virtual void Default() = 0;
	virtual void UpdateDefault(LPCWSTR pSection, LPCWSTR pEntry);		// // /
	LPCWSTR GetSection() const { return m_pSection; }
protected:
	LPCWSTR m_pSection;
	LPCWSTR m_pEntry;
	LPCWSTR m_pSectionSecond = nullptr;		// // //
	LPCWSTR m_pEntrySecond = nullptr;		// // //
};

// Templated setting class
template <class T>
class CSettingType : public CSettingBase {
public:
	CSettingType(LPCWSTR pSection, LPCWSTR pEntry, T defaultVal, T *pVar) : CSettingBase(pSection, pEntry), m_tDefaultValue(defaultVal), m_pVariable(pVar) { }
	void Load() override;
	void Save() override;
	void Default() override;
protected:
	T *m_pVariable;
	T m_tDefaultValue;
};

// Settings collection
class CSettings {
private:
	CSettings();
	CSettings(const CSettings &) = delete;		// // //
	CSettings(CSettings &&) = delete;

public:
	void	LoadSettings();
	void	SaveSettings();
	void	DefaultSettings();
	void	DeleteSettings();
	void	SetWindowPos(int Left, int Top, int Right, int Bottom, int State);

	const CStringW &GetPath(unsigned int PathType) const;		// // //
	void	SetPath(const CStringW &PathName, unsigned int PathType);

public:
	static CSettings &GetInstance();		// // //

public:
	// Local cache of all settings (all public)

	struct {
		bool	bWrapCursor;
		bool	bWrapFrames;
		bool	bFreeCursorEdit;
		bool	bWavePreview;
		bool	bKeyRepeat;
		bool	bRowInHex;
		bool	bFramePreview;
		int		iEditStyle;
		bool	bNoDPCMReset;
		bool	bNoStepMove;
		int		iPageStepSize;
		bool	bPullUpDelete;
		bool	bBackups;
		bool	bSingleInstance;
		bool	bPreviewFullRow;
		bool	bDblClickSelect;
		bool	bWrapPatternValue;		// // //
		bool	bCutVolume;
		bool	bFDSOldVolume;
		bool	bRetrieveChanState;
		bool	bOverflowPaste;
		bool	bShowSkippedRows;
		bool	bHexKeypad;
		bool	bMultiFrameSel;
		bool	bCheckVersion;		// // //
	} General;

	struct {
		int		iErrorLevel;
	} Version;		// // //

	struct {
		int		iDevice;
		int		iSampleRate;
		int		iSampleSize;
		int		iBufferLength;
		int		iBassFilter;
		int		iTrebleFilter;
		int		iTrebleDamping;
		int		iMixVolume;
	} Sound;

	struct {
		int		iMidiDevice;
		int		iMidiOutDevice;
		bool	bMidiMasterSync;
		bool	bMidiKeyRelease;
		bool	bMidiChannelMap;
		bool	bMidiVelocity;
		bool	bMidiArpeggio;
	} Midi;

	struct {
		int		iColBackground;
		int		iColBackgroundHilite;
		int		iColBackgroundHilite2;
		int		iColPatternText;
		int		iColPatternTextHilite;
		int		iColPatternTextHilite2;
		int		iColPatternInstrument;
		int		iColPatternVolume;
		int		iColPatternEffect;
		int		iColSelection;
		int		iColCursor;
		int		iColCurrentRowNormal;		// // //
		int		iColCurrentRowEdit;
		int		iColCurrentRowPlaying;

		CStringW	strFont;		// // //
		CStringW	strFrameFont;		// // // 050B
		int		iFontSize;
		bool	bPatternColor;
		bool	bDisplayFlats;
	} Appearance;

	struct {
		int		iLeft;
		int		iTop;
		int		iRight;
		int		iBottom;
		int		iState;
	} WindowPos;

	struct {
		int		iKeyNoteCut;
		int		iKeyNoteRelease;
		int		iKeyClear;
		int		iKeyRepeat;
		int		iKeyEchoBuffer;		// // //
	} Keys;

	struct {
		bool	bAverageBPM;
		bool	bChannelState;
		bool	bRegisterState;
	} Display;		// // // 050B

	// Other
	int SampleWinState;
	int FrameEditPos;
	int ControlPanelPos;		// // // 050B
	bool FollowMode;
	bool MeterDecayRate;		// // // 050B
	bool m_bNamcoMixing;		// // //

	struct {
		int		iLevelAPU1;
		int		iLevelAPU2;
		int		iLevelVRC6;
		int		iLevelVRC7;
		int		iLevelMMC5;
		int		iLevelFDS;
		int		iLevelN163;
		int		iLevelS5B;
	} ChipLevels;

	CStringW InstrumentMenuPath;

private:
	template<class T>
	CSettingBase *AddSetting(LPCWSTR pSection, LPCWSTR pEntry, T tDefault, T *pVariable);		// // //
	void SetupSettings();

private:
	static const int MAX_SETTINGS = 128;

private:
	std::vector<std::unique_ptr<CSettingBase>> m_pSettings;		// // //

private:
	// Paths
	CStringW Paths[PATH_COUNT];
};
