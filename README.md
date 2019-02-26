# 0CC-FamiTracker

0CC-FamiTracker is a modified version of FamiTracker that incorporates various bug fixes and new features which work in exported NSFs as well. The name "0CC" comes from the author's favourite arpeggio effect command. The current version includes:

- Partial FamiTracker 0.5.0 beta support
- Sound engine extensions:
   - Ad-hoc multichip NSF export
   - Echo buffer access
   - Polyphonic note preview
- New effects:
   - Hardware volume envelope effects
   - Delayed channel effects
   - FDS automatic FM effects
   - N163 wave buffer access effect
- Instrument extensions:
   - Arpeggio schemes
   - Instrument recorder
   - Compatible sequence instruments
- Interface extensions:
   - Find / replace tab
   - Transpose dialog
   - Split keyboard
- Extra module contents:
   - Detune settings
   - Groove settings
   - Bookmark manager
   - Linear pitch mode

See the change log for the full list of changes made in 0CC-FamiTracker.

This program and its source code are licensed under the GNU General Public License Version 2. Differences to the original FamiTracker source are marked with "// // //"; those to the ASM source with ";;; ;; ;" and "; ;; ;;;".

The current build is based on the version 0.5.0 beta 5 release of the official FamiTracker. 0CC-FamiTracker will be ported to newer official releases once they become available; features added in 0CC-FamiTracker may not have identical behaviour as the corresponding features on the official branch.

# Links

- http://hertzdevil.info/programs/  
  The download site for all versions of 0CC-Famitracker.
- http://0cc-famitracker.tumblr.com/  
  The official development log of 0CC-FamiTracker.
- http://github.com/HertzDevil/0CC-FamiTracker  
  The Git source repository for the tracker (this page).
- http://github.com/HertzDevil/0CC-FT-NSF-Driver  
  The Git source repository for the NSF driver.

---

# 日本語化

森の子リスのミーコの大冒険 (Phroneris) が勝手に日本語化してみた。  
現在のバージョンはv0.3.15.4-JPpre001。

## 概要

かつて私が公開した[0CC-FamiTracker v0.3.11r1の日本語化パッチ](http://phroneris.com/text/20150911.shtml "自サイトの配布ページ")の翻訳を元に、それを更に改善する感じで作っている。  
あのパッチ作成はResource Hackerだけでなくバイナリエディタまで使ってexeファイルをちまちま改造していく作業がつらかったんだけど、ある日「OSSなんだから元のソースを日本語化しちゃえばいいじゃん！」と気付いたのでこれを作り始めた。

やってる変更はほとんど単純な文字列の置換とちょっとしたスペースの調整のみだけど、何せ私はC++なんて触ったことがないので、この日本語化によってどんな不具合が増えているかわからない。仮にわかったとしても、私の手には負えないかもしれない。  
FamiTrackerはGPL 2.0ライセンスなので改めて書く必要はないことだけど、もし使う人がいるなら、どうかどんなトラブルが起きても自己責任で使ってほしい。

GPLといえば、日本語化のための変更を加えた行やブロックには`/// jp`というコメントを添えてそれを示している。

## エフェクトについて

[エフェクトとアルペジオ（日訳）](エフェクトとアルペジオ（日訳）.md "別ファイルへのリンク")に日本語でまとめてみた。

## ビルドに関する注意点

+ 日本語化されたリソースファイル（`*.rc`）やレジストリファイル（`*.reg`）は、文字エンコーディングがUTF-16 Little-Endianでないといけないっぽい。Visual Studio 2017上のエンコーディング選択肢は`Unicode - Codepage 1200`となる。
  - Big-Endianのリソースファイルだとエラーが出た。
+ UTF-16のファイルをGitで管理するために、Gitへ渡す（add）時にはUTF-8に変換し、Gitから受け取る（checkout）時にはUTF-16に変換するようなフィルターをかけてやると楽になる。  
  私は以下の手順でどうにかしている。
  1. [nkf v2.1.1](https://www.vector.co.jp/soft/win95/util/se295331.html "nkf v2.1.1 配布サイト")の`nkf.exe`をGitの`usr/bin`に置く。
  2. 日本語化リポジトリをチェックアウトする。
  3. `.gitattributes`に以下の行を追加してコミットする（これは既に日本語化リポジトリに含まれているので、改めてやる必要はない）。
     ```
	 0CC-FamiTracker.rc filter=utf16lebom
	 0CC-FamiTracker.reg filter=utf16lebom
	 ```
  4. `.git/config`に以下の行を追加する。
     ```
     [filter "utf16lebom"]
     	smudge = nkf -w16Lxu
     	clean = nkf -w80xu
     ```
	 - nkfの代わりにiconvでもできなくはないが、ファイル先頭にBOM差分が生じたり、事故ってファイルが崩壊したりするのでつらい。
  5. 日本語化リポジトリをチェックアウトし直す。
+ 日本語化されたその他のソースファイルは、文字エンコーディングがUTF-8でないといけない。BOMは無くても良く、その場合のVisual Studio 2017上のエンコーディング選択肢は`Unicode (UTF-8 without signature) - Codepage 65001`となる。
  - いや、本来はBOMが必要らしいんだけど、コンパイラオプションでどうにかしている。
  - `resource.h`だけは、たまにShift_JISに戻っちゃうのでBOMをつけてみている。
+ 事前にVisual Studioの「C++に関するXPサポート」とDirectXの「DirectX SDK June 2010」の導入が要る。  
  手動でやったかどうか覚えてないけど、必要なら`$(DXSDK_DIR)`へのパスも通す。
+ 現状、ビルドに使用したソースは0.3.15.3の公式リリース版（master）ではなく、そこから多少進んだ開発版（dev）。
  - ほんとは公式版を使いたかったけど、ビルドしようとすると`Error C3861 'ReadGlobalMemory': identifier not found`って言われてビルドできなかったので…。

## 参考にしたもの

+ How to localize an RC file? — Internationalization Cookbook  
  https://mihai-nita.net/2007/05/03/how-to-localize-an-rc-file/
+ gitの便利なfilterまとめ - webネタ  
  http://r-h.hatenablog.com/entry/2014/02/03/201805
+ Visual Studio 2017でファイルのエンコードを指定して保存する方法 - C++と色々  
  https://nekko1119.hatenablog.com/entry/2017/03/28/003348
+ Visual Studio で UTF-8 で C++ を書いたら心が折れそうになった件 - Hikware.Tech  
  http://tech.hikware.com/article/20171020a.html
+ visual studio - How to install build tools for v141_xp for VC 2017? - Stack Overflow  
  https://stackoverflow.com/questions/49516896/how-to-install-build-tools-for-v141-xp-for-vc-2017
+ DirectX SDKインストール時に”S1023エラー”が出る時の対処法  
  http://nanoappli.com/blog/archives/4739

## Known issues

+ 0CCFT never open help files
+ Some strings get garbled if they are localized
  - Marked with `/// jp-garbling` in source codes
+ Shortcuts are stored with localized names in the registry
+ `ID_CLONE_SEQUENCE` (in `IDR_SEQUENCE_POPUP`) shows `IDS_DISABLE_SAVE` string on the resource IDE
+ `IDR_FRAME_POPUP`, `IDR_PATTERN_HEADER_POPUP`, `IDR_SAMPLE_EDITOR_POPUP`, `IDR_SAMPLE_WND_POPUP`, `IDR_SAMPLES_POPUP` and `IDR_SEQUENCE_POPUP` don't show menu prompts on the status bar
+ [WIP-reg]: Save a `*.0cc` file -> close -> double-click the file -> 0CCFT crashes
  - It's OK to open it from 0CCFT
+ [Non-issue]: Some contents of `ID_FRAME_*` are duplicated to ones of `ID_EDIT_*`
+ [Non-issue]: There are many strings unused in the string table
  - Marked some of them as beginning with `[*未使用*]`
+ [Non-issue]: I wonder why the actual configuration dialog is a little wider than the one previewed from the resource IDE

\* WIP-reg issues occur of course because I tried building the beta sources as Release without `WIP` definition in `version.h`

## 多言語化についてのぼやき

…なんか`FamiTracker.cpp`の`LoadLocalization()`という箇所を見ると、`language.dll`というものを用意してやることでお手軽に翻訳できるようにする機能が本家FamiTrackerの頃から搭載されているらしい。  
しかし、[要望があって開発者も興味を持っているものの、本格実装には至っていないっぽい](http://forums.famitracker.com/viewtopic.php?t=380 "FamiTracker公式フォーラムのトピック")。残念。

更には[別のローカライズエンジンを作る動きもあったようだけど](http://forums.famitracker.com/viewtopic.php?p=9456 "FamiTracker公式フォーラムのトピック")、よくわからない。残念。


