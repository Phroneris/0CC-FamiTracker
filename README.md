
# 日本語化したよ

0CC-FamiTrackerが好きなので勝手に日本語化してみた。  
現在の日本語版のバージョンは**0.3.15.3-JP001**。


## 概要

かつて私が公開した[0CC-FamiTracker v0.3.11r1の日本語化パッチ](http://phroneris.com/text/20150911.shtml "自サイトの配布ページ")の翻訳を元に、それを更に改善する感じで作っている。  
あのパッチ作成はResource Hackerだけでなくバイナリエディタまで使ってexeファイルをちまちま改造していく作業がつらかったんだけど、ある日「OSSなんだから元のソースを日本語化しちゃえばいいじゃん！」と気付いたのでこちらを作った。

やってる変更はほとんど単純な文字列の置換とちょっとしたスペースの調整のみだけど、何せ私はC++なんて触ったことがないので、この日本語化によってどんな不具合が増えているかわからない。仮にわかったとしても、私の手には負えないかもしれない。  
FamiTrackerはGPL 2.0ライセンスなので改めて書く必要はないことだけど、もし使う人がいるなら、どうかどんなトラブルが起きても自己責任で使ってほしい。  
一応、私が見つけられた範囲のトラブルは下の[Known issues](#known-issues "既知の問題点の項目")の項目に書き留めてある。  

GPLといえば、日本語化のための変更を加えた行やブロックには`/// jp`というコメントを添えてそれを示している。


## エフェクトについて

0CC-FamiTrackerには、独自のエフェクトや本家FamiTrackerのベータ版のエフェクトなどが搭載されている。  
独自の方については`0CC-readme.txt`に説明が書かれてるけど、全部英語でつらいので、これも日本語化してまとめてみた。  
→ [エフェクトとアルペジオ（日訳）.md](エフェクトとアルペジオ（日訳）.md "別ファイルへのリンク")


## レジストリについて

私はこれにとりかかるまで「インストーラーのないソフト」＝「拡張子関連付けをしない限り、レジストリに影響を残さないソフト」だとばかり思ってたんだけど、どうも最近のソフトはそうでもないらしい。  
本家FamiTrackerや0CC-FamiTrackerは、インストーラーがないにもかかわらず、最新版であればファイル拡張子が自動で関連付けされる。すごい。  
処理のタイミングとしては、起動時に関連付けが行われ、保存時にファイルアイコンの登録と拡張子表示の省略登録が行われ、終了時に環境設定の保存が行われる…という感じに見える。

私が見つけられる限り、0CC-FT 0.3.15.3では以下のレジストリが自動登録される（`★`はユーザー識別子）。  
なお、下位リストとして書いたものは連動しているレジストリなので、親玉を消すと勝手に消える。
```
  拡張子の関連付け：
  HKEY_CLASSES_ROOT\.0cc
    - HKEY_CURRENT_USER\Software\Classes\.0cc
    - HKEY_USERS\★\Software\Classes\.0cc
    - HKEY_USERS\★_Classes\.0cc

  関連付けの詳細：
  HKEY_CLASSES_ROOT\0CCFamiTracker.Document
    - HKEY_CURRENT_USER\Software\Classes\0CCFamiTracker.Document
    - HKEY_USERS\★\Software\Classes\0CCFamiTracker.Document
    - HKEY_USERS\★_Classes\0CCFamiTracker.Document

  環境設定：
  HKEY_CURRENT_USER\Software\0CC-FamiTracker
    - HKEY_USERS\★\Software\0CC-FamiTracker
```

<!-- 以下、個人的なぼやき：

  レジストリ自動登録、INIファイルも手動関連付けも要らないので導入時には確かにお手軽で便利だけど、
  消したり保管したり移動したりしたい時には結局それなりの知識と調査が必要になるので、むしろめんどくさい気がするなぁ。
  その辺をお手軽に処理する機能も一緒に実装されてるなら別だけど、それはソフトによってまちまちだろうから、
  どのみち調べるか試すかする必要が出てくるし。

  でもまあ、少なくとも私の使っているWindows7には
  【exeファイルのあるパスを変更すると同じexeファイルを二度と関連付けできなくなる】という忌まわしい不具合があって
  （「ファイルを開くプログラムの選択」で参照から選択しても一覧に表示されなくなっちゃう）、
  それを解決するためには結局レジストリをいじる羽目になるんだよね。
    ※ このようにいじるのだ→ https://www.pasoble.jp/windows/7/08843.html
  せっかくのポータブルexeなんだから気軽に場所を変えたいこともあるのに。
  で、起動のたびにレジストリが自動修正されるなら、晴れてこの不具合とは無縁になるわけだ。うーんやはり便利なのかも。

  …いや、でもINIファイルが無いことに初めて気付いた時は「どこに設定が保存されてんだこれ！？」って純粋にビビったな。
  ぶっちゃけ気味が悪かった。
  このソフトがたまたまOSSだったので、後から多少なりともタネが理解できたけど。

  ということで個人的な総評は「そうと知った上で使えば便利」。わぁ無難。

-->


## ビルドに関する注意点（開発者向け）

+ 日本語化されたリソースファイル（`*.rc`）やレジストリファイル（`*.reg`）は、文字エンコーディングがUTF-16 Little-Endianでないといけないっぽい。Visual Studio 2017上のエンコーディング選択肢は`Unicode - Codepage 1200`となる。

  - Big-Endianのリソースファイルだとエラーが出た。

+ UTF-16のファイルはそのままだとGitでバイナリファイル扱いされるらしいので、Gitへ渡す（add）時にはUTF-8に変換し、Gitから受け取る（checkout）時にはUTF-16に変換するようなフィルターをかけてやると楽になる。  
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
  5. 日本語化リポジトリをチェックアウトし直す。

  備考：
  - `filter`ではなく`diff`を指定するやり方がよく見つかるけど、これだと差分閲覧時の表示だけがエンコードされて実際のファイル受け渡しはバイナリになるので気に食わなかった。
  - nkfの代わりにiconvでもできなくはないけど、ファイル先頭にBOM差分が生じたり、事故ってファイルが崩壊したりするので気に食わなかった。
    入力を自動判別してくれるnkf最高。環境の共有しやすさとか知らん。みんな頑張ってnkfと上記設定を導入してほしい。

+ 日本語化されたその他のソースファイルは、文字エンコーディングがUTF-8でないといけない。BOMは無くても良く、その場合のVisual Studio 2017上のエンコーディング選択肢は`Unicode (UTF-8 without signature) - Codepage 65001`となる。  
  いや本来はBOMが必要らしいんだけど、私は以下の手順でBOMなしファイルをどうにかしている。
  
  1. ソリューションエクスプローラーで0CC-FamiTrackerを右クリック→「プロパティ」を開く。
  2. 構成とプラットフォームに「すべての構成」「すべてのプラットフォーム」を選択しておく。
  3. 「構成プロパティ」→「C/C++」→「コマンド ライン」のメニューを表示する。
  4. 「追加のオプション」の欄に`/source-charset:utf-8`というコンパイラオプションを追加して「OK」。
  
  備考：
  - `resource.h`だけは、BOMなしだとたまにShift_JISに戻っちゃうのでBOMをつけている。

+ 事前にVisual Studioの「C++に関するXPサポート」とDirectXの「DirectX SDK June 2010」の導入が要る。  
  `$(DXSDK_DIR)`へのパスも、通ってなければ通す（手動でやったかどうか覚えてない）。

+ 現状、ビルドに使用したソースは0.3.15.3の公式リリース版（master）ではなく、それをちょっとだけいじったもの。

  - ほんとは公式版と完全に同じ状態で翻訳したかったけど、そのままビルドしようとすると`Error C3861 'ReadGlobalMemory': identifier not found`と言われて失敗し、`Source/Clipboard.h`の一部の行を移動させたら成功したのでそれで良しとした。


## Known issues

+ 0CCFT never open help files
+ Some strings get garbled if they are localized
  - Marked with `/// jp-garbling` in source codes
+ There are some unexpected note mappings to right-side keys on my (Japanese) keyboard
  ```
    |key(shift)| expected |  actual  |
    |----------+----------+----------|
    |  ; (+)   |  D#4     |  F#5     | *   Z=C3
    |  \ (_)   |  F4      |  (none)  | **  has the same char as "\ (|)" but works differently 
    |  ] (})   |  F#4     |  G5      |
    |  @ (`)   |  F5      |  (none)  |
    |  ^ (~)   |  F#5     |  (none)  |
    |  [ ({)   |  G5      |  F5      |
    |  \ (|)   |  G#5     |  (none)  | *** note release by default
  ```
+ `V`xx effect of N163 shows a hint for one of VRC7
+ I couldn't confirm how the instrument recorder works (so I didn't localize it)
+ `Page Down` and `Scroll Lock` become invisible on the shortcut config list
+ Shortcuts are stored with localized names in the registry
+ `/Unregister` command causes `Unhandled exception C0000005` and crash, and no registry items are removed
+ `ID_CLONE_SEQUENCE` (in `IDR_SEQUENCE_POPUP`) shows `IDS_DISABLE_SAVE` string on the resource IDE
+ These don't show menu prompts on the status bar: `IDR_FRAME_POPUP`, `IDR_PATTERN_HEADER_POPUP`, `IDR_SAMPLE_EDITOR_POPUP`, `IDR_SAMPLE_WND_POPUP`, `IDR_SAMPLES_POPUP` and `IDR_SEQUENCE_POPUP`
+ [Non-issue]: Some contents of `ID_FRAME_*` are duplicated to ones of `ID_EDIT_*`
+ [Non-issue]: There are many unused strings in the string table
  - Marked some of them as beginning with `[*未使用*]`
+ [Non-issue]: I wonder why the actual configuration dialog is a little wider than the one previewed from the resource IDE


## 参考にしたもの

+ How to localize an RC file? — Internationalization Cookbook  
  https://mihai-nita.net/2007/05/03/how-to-localize-an-rc-file/
+ レジストリの直接編集によるファイルの拡張子と関連づけ - Glamenv-Septzen.net  
  https://www.glamenv-septzen.net/view/14
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


<!-- 以下、多言語化についてのぼやき：

  …なんかFamiTracker.cppのLoadLocalization()という箇所を見ると、
  language.dllというものを用意してやることでお手軽に翻訳できるようにする機能が、
  本家FamiTrackerの頃から搭載されているらしい。  
  しかし、本格実装の要望があって開発者も興味を持っているものの、そこまでには至っていないっぽい。残念。
   → Language pack - FamiTracker http://forums.famitracker.com/viewtopic.php?t=380
  
  更には別のローカライズエンジンを作る動きもあったようだけど、よくわからない。残念。
   → Famitracker Localization engine - FamiTracker http://forums.famitracker.com/viewtopic.php?p=9456

-->



​

以下は、日本語化元であるオリジナルの0CC-FamiTrackerのREADME。

-----

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
