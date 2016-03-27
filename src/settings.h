/**************************************************************************
 *  Spivak Karaoke PLayer - a free, cross-platform desktop karaoke player *
 *  Copyright (C) 2015-2016 George Yunaev, support@ulduzsoft.com          *
 *                                                                        *
 *  This program is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *																	      *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 **************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFont>
#include <QColor>

class Settings
{
    public:
        Settings();

        enum BackgroundType
        {
            BACKGROUND_TYPE_NONE = 2,
            BACKGROUND_TYPE_COLOR = 4,
            BACKGROUND_TYPE_IMAGE = 6,
            BACKGROUND_TYPE_VIDEO = 8
        };

        QString cacheDir;

        // Player parameters
        QColor      playerLyricsTextBeforeColor;
        QColor      playerLyricsTextAfterColor;
        QColor      playerLyricsTextSpotColor;
        QFont       playerLyricsFont;
        int         playerLyricsFontMaxSize;
        int         playerLyricsFontFitLines;
        bool        playerLyricsTextEachCharacter;
        bool        playerUseBuiltinMidiSynth;
        bool        playerCDGbackgroundTransparent;
        bool        playerIgnoreBackgroundFromFormats;

        int         playerRenderFPS;
        int         playerMusicLyricDelay;      // delay in milliseconds between lyrics and music for this hardware
        int         playerVolumeStep;           // how much change the volume when +/- is pressed


        // Queue parameters
        bool        queueAddNewSingersNext; // if true, new singers are added right after the current singer; otherwise at the end.
        QString     queueFilename;  // if defined, we save queue into file
        bool        queueSaveOnExit;

        // Notification
        QColor      notificationTopColor;
        QColor      notificationCenterColor;


        // Player background
        BackgroundType  playerBackgroundType;
        QColor          playerBackgroundColor;
        QStringList     playerBackgroundObjects;    // images or videos
        unsigned int    playerBackgroundTransitionDelay; // images only, in seconds. 0 - no transitions/slideshow

        // Music background
        QStringList     musicCollections;
        bool            musicCollectionSortedOrder;
        int             musicCollectionCrossfadeTime;

        // Songs database
        QString         songdbFilename;

        // LIRC path
        bool            lircEnabled;
        QString         lircDevicePath;
        QString         lircMappingFile;

        // HTTP port
        bool            httpEnabled;
        bool            httpEnableAddQueue;
        unsigned int    httpListenPort;
        QString         httpDocumentRoot;

        // If true, we should start in a full screen mode (but this doesn't mean NOW is a fullscreen mode)
        bool            startInFullscreen;

        // If true, first-time wizard has been shown already
        bool            firstTimeWizardShown;

        // Replaces the database path according to rules
        QString         replacePath( const QString& origpath );

    public:
        // Load and save settings
        void            load();
        void            save();

    private:
        QString         m_appDataPath;

        // If the song path replacement is set (songPathReplacementFrom is not empty), this would replace '^from' to '^to'
        QString         songPathReplacementFrom;
        QString         songPathReplacementTo;
};

extern Settings * pSettings;


#endif // SETTINGS_H