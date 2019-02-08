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
現在のバージョンはv0.3.15.3-JP001。

かつて公開した[0CC-FamiTracker v0.3.11r1の日本語化パッチ](http://phroneris.com/text/20150911.shtml "自サイトの配布ページ")の翻訳を元に、それを更に改善する感じで作っている。  
あのパッチ作成はResource Hackerだけでなくバイナリエディタまで使ってexeファイルをちまちま改造していく作業がつらかったんだけど、ある日「OSSなんだから元のソースを日本語化しちゃえばいいじゃん！」と気付いたのでこれを作り始めた。

やってる変更はほとんど単純な文字列の置換のみだけど、何せ私はC++なんて触ったことがないので、この日本語化によってどんな不具合が増えているかわからない。仮にわかったとしても、私の手には負えないかもしれない。  
FamiTrackerはGPL 2.0ライセンスなので改めて書く必要はないことだけど、もし使う人がいるなら、どうかどんなトラブルが起きても自己責任で使ってほしい。

GPLといえば、日本語化のための変更を加えた行やブロックには、`/// jp`というコメントを添えてそれを示している。

## ビルドに関する注意点

+ 日本語化されたリソースファイル（`*.rc`）は、文字エンコーディングがUTF-16でないといけない。  
  エンディアンの違いやBOMの必要性についてはよくわからないが、私はVisual Studio 2017の表記でいうところの`Unicode - Codepage 1200`という[エンコードを選んで保存している](https://nekko1119.hatenablog.com/entry/2017/03/28/003348 "参考記事")。
  - 同じUTF-16でも、`Unicode (Big-Endian) - Codepage 1201`だとエラーが出る。よくわからない。
+ UTF-16のリソースファイルをGitで管理するために、Gitへ渡す時（`add`）にはUTF-8に変換し、Gitから受け取る時（`checkout`）にはUTF-16に変換するような[フィルターをかけてやる](http://r-h.hatenablog.com/entry/2014/02/03/201805 "参考記事")と楽になる。  
  私は以下の手順でどうにかしている。
  1. [nkf v2.1.1](https://www.vector.co.jp/soft/win95/util/se295331.html "nkf v2.1.1 配布サイト")の`nkf.exe`をGitの`usr/bin`に置く。
  2. `.gitattributes`に`0CC-FamiTracker.rc filter=rc-utf16lebom`という行を追加する（このファイルは既にコミットに含んでいるので、このリポジトリでわざわざやる必要はない）。
  3. `.git/config`に以下の行を追加する。
     ```
     [filter "rc-utf16"]
     	smudge = nkf -w16Lxu
     	clean = nkf -w80xu
     ```
	 - nkfの代わりにiconvでもできなくはないが、ファイル先頭にBOM差分が生じたり、事故ってファイルが崩壊したりするのでつらい。
+ 日本語化されたその他のソースファイルは、文字エンコーディングがUTF-8でないといけない。  
  BOMは無くても良い。本来は必要らしいんだけど、[コンパイラオプションでどうにかしている](http://tech.hikware.com/article/20171020a.html "参考記事")。
+ バージョンを`0.3.15.3-JP001`と銘打ってはいるが、実際にビルドしたソースは0.3.15.3の公式リリース版（master）ではなく、そこから多少進んだ開発版（dev）。  
  だって、公式版をビルドしようとすると`Error C3861 'ReadGlobalMemory': identifier not found`って言われてビルドできないんだもん…。

## 多言語化についてのぼやき

…なんか`FamiTracker.cpp`の`LoadLocalization()`という箇所を見ると、`language.dll`というものを用意してやることでお手軽に翻訳できるようにする機能が本家FamiTrackerの頃から搭載されているらしい。  
しかし、[要望があって開発者も興味を持っているものの、本格実装には至っていないっぽい](http://forums.famitracker.com/viewtopic.php?t=380 "FamiTracker公式フォーラムのトピック")。残念。

更には[別のローカライズエンジンを作る動きもあったようだけど](http://forums.famitracker.com/viewtopic.php?p=9456 "FamiTracker公式フォーラムのトピック")、よくわからない。残念。


