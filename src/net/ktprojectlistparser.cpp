/***************************************************************************
 *   Project TUPI: Magia 2D                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez / xtingray                                          *
 *                                                                         *
 *   KTooN's versions:                                                     * 
 *                                                                         *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       *
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Gustav Gonzalez - http://www.maefloresta.com       *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktprojectlistparser.h"

struct KTProjectListParser::Private
{
    QList<KTProjectListParser::ProjectInfo> projectsInfo;
};

KTProjectListParser::KTProjectListParser() : KTXmlParserBase(), k( new Private())
{
}

KTProjectListParser::~KTProjectListParser()
{
}

bool KTProjectListParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "server_projectlist") {
        if (tag == "project") {
            ProjectInfo info;
            info.file = atts.value("filename");
            info.name = atts.value("name");
            info.author = atts.value("author");
            info.description = atts.value("description");
            info.date = atts.value("date");
            k->projectsInfo << info;
        }
    }

    return true;
}

bool KTProjectListParser::endTag(const QString &tag)
{
    return true;
}

void KTProjectListParser::text(const QString &text)
{
    
}

QList<KTProjectListParser::ProjectInfo> KTProjectListParser::projectsInfo()
{
    return k->projectsInfo;
}

int KTProjectListParser::listSize()
{
    return k->projectsInfo.count();
}
