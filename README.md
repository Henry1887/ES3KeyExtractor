# Unity EasySave3 Decryption Key Extractor

A tool for extracting the plaintext decryption key from Unity games using EasySave3.

> Note: This tool was written with major help from AI, as I have not touched C in years.

## What is EasySave3?
*(from the Unity Asset Store)*

EasySave lets you save almost anything with ease across platforms, along with features such as encryption, compression, cloud storage, spreadsheets, backups, and much more.

Many developers using EasySave3 utilize the built-in encryption function to prevent people from tampering with their save files. That's why this tool was created—to extract the default encryption key from the assets of a Unity game.

> Important: Developers can easily prevent what this tool does by:
- Not saving the key locally.
- Not storing it as plaintext.
- Manually changing the default key in the code (which this tool cannot detect).

## Supported Games
This tool works on **every Unity game** (PC and Android), as long as you have access to the unencrypted assets of the game.

## Usage

For **PC Unity Games**:
`search.exe {game_Data folder}`

For **Android Unity Games**:
`search.exe {APK file of the game}`

## How to Build

To compile the tool, use the following command:
`gcc -o search.exe search.c`
