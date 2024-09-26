A Tool for extracting the plain text decryption key from Unity games using EasySave3.

(Please note that this was written with major help from ai, as i have not touched c in years)


What is EasySave3?
(from the Unity Asset Store)
- Easy Save lets you save almost anything with ease across platforms, along with features such as encryption, compression, cloud storage, spreadsheets, backups, and much more.


Many Developers using EasySave3 use the built in encryption function to stop people from tampering with their save files. Thats why i have made this tool to extract the *Default* encryption key from the assets of a Unity game.
What this tool does can be easily prevented by Developers, like not saving the key locally or not storing it as plain text. The Developers could also manually change the default key in the code which this Tool cannot detect.


On what unity Games will this game work on?
- Every Unity Game (Android and PC) as long as you have access to the unencrypted assets of the game.


Usage:
(For PC Unity Games)
search.exe {game_Data Folder)

(For Android Unity Games)
search.exe {apk file of the game}



How to build it:
gcc -o search.exe search.c
