# QWERuler premium ファームウェア

QWERuler premium（QWERuler の黒×金 限定版）用の Vial 対応ファームウェア（QMK / Vial）のソースです。

- 対象: 2026年度駒場祭版・2026年度エンジニアフェスティバル版（回路・ファームウェアは共通です）
- マイコン: Waveshare RP2040-Zero（表面に実装、フラッシュ W25Q16JV 2MB）
- 裏面のオプション「直付け」RP2040 回路（W25Q16JV、12MHz、RESET / BOOT ボタン、GP25 に LED）でも、同じファームウェアがそのまま動きます
- 初期キーマップ: `Q` `W` `E` `R`（QWERuler rev2 と同じ）

> [!WARNING]
> QWERuler premium はキーを 1 本ずつマイコンに直結しており（ダイオードなし）、QWERuler rev2（2×2 マトリクス + ダイオード）とは使っているピンも配線方式も違います。
> **rev2 用のファームウェア（`qweruler_rev2_vial.uf2`）は QWERuler premium では正しく動きません。** 逆も同様です。

## ディレクトリ構成

```
firmware/qweruler_premium/
├── README.md            … このファイル
├── RELEASE_NOTES.md     … GitHub Release の本文（下書き）
└── keyboards/
    └── pentroniclab/
        └── qweruler_premium/   … vial-qmk の keyboards/ 以下にそのまま置ける形
            ├── keyboard.json   … キーボード定義（ピン、USB ID など）
            ├── config.h        … RESET 2 回押しでブートローダに入る設定
            ├── readme.md
            └── keymaps/
                ├── default/    … QMK 用の標準キーマップ（QWER）
                └── vial/       … Vial 用キーマップ（配布するファームウェアはこちら）
```

## ピンアサイン

キーはマトリクスを組まず、1 キーにつき GPIO を 1 本使う直結方式です（QMK の `matrix_pins.direct`）。
スイッチのもう一方の端子は GND につながっているので、押すと Low になります（アクティブ Low）。
プルアップは RP2040 の内蔵プルアップを使います（QMK の直結ピンの標準動作です）。

| キー（表から見て左から） | GPIO | もう一方の端子 | 初期キー | Vial 上の位置（行,列） |
|---|---|---|---|---|
| SW1 | GP8 | GND | `Q` | 0,0 |
| SW2 | GP7 | GND | `W` | 0,1 |
| SW3 | GP6 | GND | `E` | 0,2 |
| SW4 | GP5 | GND | `R` | 0,3 |

## 主な設定

| 項目 | 値 | 場所 |
|---|---|---|
| 製品名 / メーカー名 | `QWERuler premium` / `Pentronic Lab.` | `keyboard.json` |
| USB VID / PID | `0x5045` / `0x5052` | `keyboard.json`、`keymaps/vial/vial.json` |
| Vial キーボード UID | `0x55, 0x74, 0xCE, 0xFF, 0xEA, 0x6D, 0xD0, 0x0A` | `keymaps/vial/config.h` |
| Vial のロック解除 | SW1（左端）と SW4（右端）を同時に長押し | `keymaps/vial/config.h` |
| レイヤー数 | 4（レイヤー 0 が QWER、1〜3 は透過） | `keymaps/vial/` |
| ブートローダ | RESET を 500ms 以内に 2 回押す | `config.h` |

ブートローダ（`RPI-RP2` ドライブ）に入る方法は次のとおりです。

- RP2040-Zero の RESET ボタンを素早く 2 回押す
- BOOT ボタンを押したまま RESET ボタンを押して離す（RP2040-Zero・直付け回路とも）
- SW1（左端のキー）を押したまま USB を接続する（Bootmagic。Vial で変更したキーマップなどの設定は初期化されます）

### USB VID / PID について

`0x5045` / `0x5052` は仮に決めた値です（USB-IF から割り当てを受けた ID ではありません。QMK の自作キーボードでは一般的な運用です）。
GitHub のコード検索で確認した範囲では、qmk_firmware に同じ VID を使っているキーボードはありません。
変更する場合は、**`keyboard.json` の `usb.vid` / `usb.pid` と、`keymaps/vial/vial.json` の `vendorId` / `productId` を必ず両方とも同じ値に**書き換えてください。
Vial はキーボードを UID で識別するので、VID / PID を変えても Vial での認識には影響しません。

### Vial キーボード UID について

`VIAL_KEYBOARD_UID` はこの製品用にランダムに生成した 8 バイトの値です。
Vial はこの値でキーボードを見分けるため、**他の製品のファームウェアに流用しないでください**。
また、出荷後に変更すると Vial から別のキーボードとして扱われるので、原則として変更しないでください。

## CI でのビルド

`.github/workflows/qweruler_premium_firmware.yml` が GitHub Actions でファームウェアをビルドします。

- 実行されるタイミング
  - `firmware/qweruler_premium/` 以下かワークフローファイルを変更するプルリクエスト
  - 手動実行（Actions タブ → 「QWERuler premium firmware」→ 「Run workflow」）
  - `qweruler_premium` で始まるタグの push（このときだけ下書きリリースも作ります。後述）
- やっていること
  1. QMK 公式のビルド用コンテナ（`ghcr.io/qmk/qmk_cli`。vial-qmk 自身の CI と同じイメージを digest で固定）の中で、[vial-qmk](https://github.com/vial-kb/vial-qmk)（`vial` ブランチ）を shallow clone
  2. `make git-submodule` でサブモジュールを取得
  3. `firmware/qweruler_premium/keyboards/` の中身を vial-qmk の `keyboards/` にコピー
  4. `make pentroniclab/qweruler_premium:vial` でビルド
  5. できた `pentroniclab_qweruler_premium_vial.uf2` を `qweruler_premium_vial.uf2` に名前を変えて、Artifact（名前: `qweruler_premium_vial`）としてアップロード

ビルドしたファームウェアは、Actions の実行結果ページ下部の「Artifacts」から zip でダウンロードできます。

> [!NOTE]
> vial-qmk は常に `vial` ブランチの最新を使います。vial-qmk 側の更新でビルドが通らなくなった場合は、vial-qmk の `.github/workflows/ci.yml` で使われているコンテナイメージに合わせてワークフローの `container:` を更新してください。

## ローカルでのビルド

1. [QMK のビルド環境](https://docs.qmk.fm/newbs_getting_started)を用意します（Windows なら QMK MSYS、macOS / Linux なら `qmk` CLI）。
   `qmk setup` で qmk_firmware を clone する必要はありません。
2. vial-qmk を取得し、サブモジュールを取得します。

   ```sh
   git clone --depth 1 -b vial https://github.com/vial-kb/vial-qmk.git
   cd vial-qmk
   make git-submodule
   ```

3. このリポジトリのキーボード定義を vial-qmk にコピーします（パスは適宜読み替えてください）。

   ```sh
   cp -r /path/to/Pentronic-Lab/firmware/qweruler_premium/keyboards/. keyboards/
   ```

4. ビルドします。

   ```sh
   make pentroniclab/qweruler_premium:vial
   ```

   vial-qmk のフォルダ直下に `pentroniclab_qweruler_premium_vial.uf2` ができます。
   配布するときは `qweruler_premium_vial.uf2` に名前を変えてください。
   QMK 標準のキーマップ（Vial なし）は `make pentroniclab/qweruler_premium:default` でビルドできます。

## リリース手順

1. 必要に応じて `RELEASE_NOTES.md`（リリース本文）と、ワークフローの `RELEASE_NAME`（リリースのタイトル。現在は `QWERuler premium Firmware rev 1`）を更新し、`main` にマージします。
2. リリースしたいコミット（通常は `main` の最新）に `qweruler_premium` タグを付けて push します。

   ```sh
   git switch main
   git pull
   git tag qweruler_premium
   git push origin qweruler_premium
   ```

3. ワークフローがファームウェアをビルドし、**下書き（Draft）のリリース**を作ります。
   - タイトル: `QWERuler premium Firmware rev 1`
   - 添付ファイル: `qweruler_premium_vial.uf2`
   - 本文: `RELEASE_NOTES.md` の内容
4. メンテナが Releases ページで下書きを確認します。添付の `.uf2` を実機に書き込み、QWER と入力できること・Vial で認識されることを確かめてから、「Publish release」で公開してください。
   **公開するまでは一般の人からは見えません。**

取扱説明書からは `https://github.com/uNikks/Pentronic-Lab/releases/tag/qweruler_premium` にリンクする想定です。

ファームウェアを改版するときは、`RELEASE_NAME` と `RELEASE_NOTES.md` を更新したうえで、

- 古いリリースとタグを削除してから、同じ `qweruler_premium` タグを付け直して push する（取扱説明書のリンクを変えずに済みます）か、
- `qweruler_premium_rev1.1` のように `qweruler_premium` で始まる別のタグを push する

のどちらかにしてください。
