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

#include "midisyntheser.h"
#include "logger.h"


// EAS file i/o callbacks
int MIDISyntheser::EAS_FILE_readAt( void *handle, void *buf, int offset, int size )
{
    MIDISyntheser * self = (MIDISyntheser *) handle;

    if ( offset > self->m_midiData.size() )
        return -1;

    int available = qMin( self->m_midiData.size() - offset, size );

    memcpy( buf, self->m_midiData.constData() + offset, available );
    return available;
}

int MIDISyntheser::EAS_FILE_size( void *handle )
{
    MIDISyntheser * self = (MIDISyntheser *) handle;

    return self->m_midiData.size();
}

static inline void StoreDwordBE( void * ptr, unsigned int value )
{
    unsigned char * p = (unsigned char*) ptr;

    p[0] = (value >> 24) & 0xFF;
    p[1] = (value >> 16) & 0xFF;
    p[2] = (value >> 8) & 0xFF;
    p[3] = value & 0xFF;
}

MIDISyntheser::MIDISyntheser(QObject *parent)
    : QIODevice( parent ), m_audioBuffer(16384, 0)
{
    m_easHandle = 0;
    m_easData = 0;
}

bool MIDISyntheser::open(const QByteArray &midiData)
{
    m_midiData = midiData;

    // Init the EAS library
    EAS_RESULT result = EAS_Init( &m_easData );

    if ( result  != EAS_SUCCESS )
    {
        Logger::error( "MIDISyntheser: cannot initialize EAS library: error %d", result );
        return false;
    }

    // Call EAS library to open file
    m_easFileIO.handle = this;
    m_easFileIO.size = EAS_FILE_size;
    m_easFileIO.readAt = EAS_FILE_readAt;

    if ( (result = EAS_OpenFile(m_easData, &m_easFileIO, &m_easHandle)) != EAS_SUCCESS )
    {
        Logger::error( "MIDISyntheser: cannot open MIDI file with EAS library: error %d", result );
        return false;
    }

    // Get the library configuration. In our case it is static, but still
    const S_EAS_LIB_CONFIG *pLibConfig = EAS_Config();
    m_mixBufferSize = pLibConfig->mixBufferSize;

    // Prepare to play the file
    if ( (result = EAS_Prepare(m_easData, m_easHandle)) != EAS_SUCCESS )
    {
        Logger::error( "MIDISyntheser: cannot prepare MIDI file with EAS library: error %d", result );
        return false;
    }

    // Get the play length so we can fill up our fake WAV header
    EAS_I32 playTime;

    if ( (result = EAS_ParseMetaData( m_easData, m_easHandle, &playTime) ) != EAS_SUCCESS )
    {
        Logger::error( "MIDISyntheser: cannot get MIDI length: error %d", result );
        return false;
    }

    // We know the total time, and we play 44100*2*2 samples per second, so we can calculate the total size
    m_totalSize = (int) (((qint64) playTime * 44100 * 4) / 1000);

    Logger::error( "MIDISyntheser: MIDI length %ld msec, matching WAV size %d bytes", playTime, m_totalSize );

    m_audioAvailable = 0;
    m_currentPosition = 0;
    m_audioEnded = false;

    // Open the QIODevice unbuffered so we wouldn't have to deal with QIODevice's own buffer
    QIODevice::open( QIODevice::ReadOnly | QIODevice::Unbuffered );

    // Now render a bit of data so we have something to present
    fillBuffer();

    // and we're good to go
    return true;
}

void MIDISyntheser::close()
{
    QIODevice::close();

    EAS_CloseFile( m_easData, m_easHandle );
    EAS_Shutdown( m_easData );
}

qint64 MIDISyntheser::pos() const
{
    return m_currentPosition;
}

bool MIDISyntheser::seek(qint64 pos)
{
    // What is requested seek position in the MIDI stream?
    int timing = (pos / (44100 * 4)) * 1000;

    qDebug("seek %ld -> %d", pos, timing );

    if ( (EAS_Locate( m_easData, m_easHandle, timing, true )) != EAS_SUCCESS )
    {
        Logger::error( "MIDISyntheser: seek to %ld (%ld) failed", (long) pos, timing );
        return false;
    }

    m_currentPosition = pos;

    // Invalidate the buffer so we start rendering again
    m_audioAvailable = 0;

    return true;
}

qint64 MIDISyntheser::size() const
{
    return m_totalSize;
}

bool MIDISyntheser::isSequential()  const
{
    return false;
}

bool MIDISyntheser::atEnd() const
{
    return m_audioEnded;
}

qint64 MIDISyntheser::bytesAvailable() const
{
    return m_audioAvailable;
}

qint64 MIDISyntheser::readData(char *data, qint64 maxSize)
{
    qint64 copied = 0;

    while ( copied < maxSize )
    {
        // Do we have data in our buffer?
        if ( m_audioAvailable > 0 )
        {
            int amount = qMin( (unsigned int) (maxSize - copied), m_audioAvailable );
            memcpy( data + copied, m_audioBuffer.data(), amount );

            m_audioAvailable -= amount;
            copied += amount;

            // Move the rest of the data, if we still have something
            if ( m_audioAvailable > 0 )
                memmove( m_audioBuffer.data(), m_audioBuffer.data() + amount, m_audioAvailable );

            continue;
        }

        // Do we have more data?
        if ( m_audioEnded )
            break;

        if ( !fillBuffer() )
            return -1;
    }

    m_currentPosition += copied;
    return copied;
}

qint64 MIDISyntheser::writeData(const char *, qint64)
{
    return -1;
}

bool MIDISyntheser::fillBuffer()
{
    // Each sample is 8 bit 2 channels, so 2 bytes per sample
    EAS_I32 count;
    EAS_STATE state;

    while ( m_audioAvailable + m_mixBufferSize * 4 < (unsigned int) m_audioBuffer.size() )
    {
        if ( EAS_Render(m_easData, (EAS_PCM*) (m_audioBuffer.data() + m_audioAvailable), m_mixBufferSize, &count) != EAS_SUCCESS )
        {
            Logger::error( "MIDISyntheser: rendering failed" );
            m_audioEnded = true;
            return false;
        }

        m_audioAvailable += count * 4;

        if ( EAS_State(m_easData, m_easHandle, &state) != EAS_SUCCESS )
        {
            Logger::error( "MIDISyntheser: failed to get state" );
            m_audioEnded = true;
            return false;
        }

        // Is playback complete?
        if ( state == EAS_STATE_STOPPED || state == EAS_STATE_ERROR )
        {
            m_audioEnded = true;
            break;
        }
    }

    return true;
}