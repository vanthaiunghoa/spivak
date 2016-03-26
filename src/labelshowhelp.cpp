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

#include <QWhatsThis>

#include "labelshowhelp.h"

LabelShowHelp::LabelShowHelp( QWidget *parent )
    : QLabel(parent)
{
    connect( this, SIGNAL(linkActivated(QString)), this, SLOT(clicked()) );
}

void LabelShowHelp::clicked()
{
    // Link is clicked - do we have a buddy?
    QString text = buddy()->whatsThis();

    QWhatsThis::showText ( ((QWidget*)parent())->mapToGlobal( buddy()->pos() ), text, this );
}
