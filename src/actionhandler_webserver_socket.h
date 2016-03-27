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

#ifndef ACTIONHANDLER_WEBSERVER_SOCKET_H
#define ACTIONHANDLER_WEBSERVER_SOCKET_H

#include <QObject>
#include <QTcpSocket>

class QTcpSocket;

class ActionHandler_WebServer_Socket : public QObject
{
    Q_OBJECT

    public:
        ActionHandler_WebServer_Socket( QTcpSocket * httpsock );
        ~ActionHandler_WebServer_Socket();

    signals:
        // This runs in a different thread, so QueuedConnection is must
        void    queueAdd( QString singer, int id );

    private slots:
        void    readyRead();

    private:
        // Sends an error code, and closes the socket
        void    sendError( int code );

    private:
        bool    search( QJsonDocument& document );
        bool    addsong( QJsonDocument& document);
        bool    listqueue( QJsonDocument& document );
        bool    listDatabase( QJsonDocument& document );

        void    sendData( const QByteArray& data, const QByteArray &type = "application/json" );

        QTcpSocket *    m_httpsock;

        // Http request body with headers
        QByteArray      m_httpRequest;

        // Parsing header results. If url is non-empty, header is parsed
        QString         m_url;
        QString         m_cookie;
        QString         m_method;
        unsigned int    m_contentLength;

};

#endif // ACTIONHANDLER_WEBSERVER_SOCKET_H