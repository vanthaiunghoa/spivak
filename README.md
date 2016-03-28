# Spivak - free cross-platform Karaoke Player

Spivak is a free, cross-platform (Linux/Windows/OS X) Karaoke player based on GStreamer and Qt5. It supports a wide range of Karaoke formats, with the goal of playing all more or less widespread Karaoke formats on all popular platforms. It also has strong support for foreign languages, so playing Karaoke in Japanese, Russian or Hindu is fully supported.

Currently Spivak is in alpha stage, but already has impressive set of features, and is stable enough to survive a party without a single crash or hang.

Spivak is licensed under GNU GPL version 3, and is written by George Yunaev.

## Features

- Supports the most popular Karaoke lyric formats: MIDI/KAR, KaraFun, CDG, LRC (v1 and v2), Encore! Lyric, KOK, Ultrastar (TXT);
- Automatically detects the lyric text encoding, no manual encoding needed;
- Supports music/video media in many different formats, as long as they are supported by your GStreamer installation (and if any format is not supported, you can install GStreamer plugin to add support);
- Supports ZIP archives containing a music file + lyric file, for all supported format (i.e. MP3+LRC is also supported);
- Supports video karaoke, such as in AVI, MKV or Flash formats;
- Supports Karaoke collections, and is able to scan the Karaoke songs into the built-in song database with search capabilities;
- Has built-in Web interface allowing users to search for songs, browse the song database and even queue the songs themselves;
- Has a smart singer queue management, ensuring fair rotation - when the same guest queues multiple songs, they are spread to ensure everyone has opportunity to sing;
- Has a built-in MIDI software syntheser with embedded wavetable, so plays MIDI files even on OS without MIDI support and without any external files such as soundfonts;
- Supports animated images and video background for karaoke songs, including CD+G songs;
- Supports LIRC for remotely controlling the player;
- Supports background music - can play regular music files when no Karaoke is played, will automatically pause it when Karaoke is chosen, and will resume the music automatically once the queue is empty;
- Has comprehensive karaoke queue management interface, as well as background music management interface;

The player has been tested primarily on Linux, but has been briefly tested on Windows and Mac OS X.

## Compiling

Currently the only way to test the player is to build it from source, which should be easy on Linux, but may be cumbersome on other OSes. You would need the following libraries:

- Qt 5.5 (might work on prior versions too, but will NOT work on Qt4);
- GStreamer 1.0 (will NOT work on 0.10) and related libraries, notably Glib;
- uchardet2 (for automatic charset decoding);
- libzip (for handling ZIP and old KFN files);
- libsonivox (to render MIDI in software) - you can get it from here: https://github.com/gyunaev/libsonivox ;
- libcld2 (to build Karaoke language detector plugin; optional);
- sqlite3 (for karaoke database);

Please do not build with google breakpad, as this would enable automatic crash reporting which is useless for me because I do not have your symbols file.

To build, please comment out the line "CONFIG += GOOGLEBREAKPAD" in src/src.pro, and then build as usual:

    qmake (or qmake-qt5)
    make

Copy the player executable from src/spivak somewhere, and copy all the plugins into the "plugins" subdirectory where spivak executable is located.

## Contacts

At this moment please use gyunaev@ulduzsoft.com to send the bugs and feature requests. Please only do so if you're willing to actively participate in bugfixing, such as testing proposed patches, and are able to compile the player yourself from the source code. If you cannot, please wait until the first beta is available, when the official builds would be available.
