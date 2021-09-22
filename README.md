# esp32-i2s-dac-builtin-tone

micropythonに拡張C言語モジュールとして組み込み、ESP32の内臓DACを使って、GPIO25端子から、トーンを発生することができます。

```
import builtindac

builtindac.init()
builtindac.tone(440,500) #440Hz, 500ms
```


# micropython esp32 への組み込み

## 事前準備

次のツールをインストールします。

- [git](https://git-scm.com/)
- [Docker](https://www.docker.com/)
- [Visual Studio Code](https://code.visualstudio.com/)
- [Visual Studio Code Remote - Containers](https://code.visualstudio.com/docs/remote/containers)


## リポジトリ（ツール、ソースコード）の取得

- [jp-rad/vscode-env-remote-esp-idf](https://github.com/jp-rad/vscode-env-remote-esp-idf)
- [micropython/micropython](https://github.com/micropython/micropython)
- [jp-96/esp32-i2s-dac-builtin-tone](https://github.com/jp-96/esp32-i2s-dac-builtin-tone)


**手順**

[jp-rad/vscode-env-remote-esp-idf](https://github.com/jp-rad/vscode-env-remote-esp-idf)を取得します。

```
cd yourworkfolder
git clone https://github.com/jp-rad/vscode-env-remote-esp-idf.git
```

[micropython/micropython](https://github.com/micropython/micropython)と[jp-96/esp32-i2s-dac-builtin-tone](https://github.com/jp-96/esp32-i2s-dac-builtin-tone)を取得済みの`vscode-env-remote-esp-idf`にある`ws`フォルダ内に取得します。

```
cd vscode-env-remote-esp-idf
cd ws
git clone https://github.com/micropython/micropython.git
git clone https://github.com/jp-96/esp32-i2s-dac-builtin-tone.git
```

## 開発環境の起動

取得済みの`vscode-env-remote-esp-idf`をVisual Studio Codeで開きます。

```
cd yourworkfolder
code vscode-env-remote-esp-idf
```

左下の`Open a Remote Window`アイコンをクリックし、画面上部のドロップダウンリストから`Reopen in Container`を選択してDockerコンテナを起動します。  
*※初回の起動は、イメージファイル等をダウンロードするため、数分かかります。*

新しいターミナルを開き(`bash`)、[micropython/micropython](https://github.com/micropython/micropython)と[jp-96/esp32-i2s-dac-builtin-tone](https://github.com/jp-96/esp32-i2s-dac-builtin-tone)のソースコードを確認します。
```
cd /home/vscode/ws
ls
```

### micropythonのビルド

新しいターミナルを開き(`bash`)、ESP32向けの`micropython`をビルド(コンパイル)します。
```
cd /home/vscode/ws/micropython
make -C mpy-cross
cd ports/esp32
make submodules
make USER_C_MODULES=../../../../esp32-i2s-dac-builtin-tone/micropython.cmake
```

`build-GENERIC`フォルダに作成された`firmware.bin`ファイルがESP32用のmicropythonファームウェアですので、`esptool.py`等でESP32へ転送してください。

```
cd yourworkfolder
cd vscode-env-remote-esp-idf/ws/micropython/ports/esp32/build-GENERIC
esptool.py --port COM3 erase_flash
esptool.py --chip esp32 --port COM3 write_flash -z 0x1000 firmware.bin
```

### スタート練習装置のサンプル

`esp32-i2s-dac-builtin-tone/example`フォルダにトーンを使ったスタート練習装置のサンプルプログラムがあります。
`ampy`ツール等でESP32へpyファイルを書き込んでください。

```
cd yourworkfolder
cd vscode-env-remote-esp-idf/ws/esp32-i2s-dac-builtin-tone/example
cd firmware
esptool.py --port COM3 erase_flash
esptool.py --chip esp32 --port COM3 write_flash -z 0x1000 firmware.bin
cd ..
ampy -p COM3 put intervalstarter.py
ampy -p COM3 put main.py
```
