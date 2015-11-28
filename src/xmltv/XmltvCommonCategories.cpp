/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QObject>

#include "XmltvCommon.h"

QMap<QString, QString> Tano::Xmltv::categoryMap()
{
    QMap<QString, QString> catMap;

    catMap["movie/drama"] = QObject::tr("Movie/Drama");
    catMap["detective/thriller"] = QObject::tr("Detective/Thriller");
    catMap["adventure/western/war"] = QObject::tr("Adventure/Western/War");
    catMap["sciencefiction/fantasy/horror"] = QObject::tr("Science Fiction/Fantasy/Horror");
    catMap["comedy"] = QObject::tr("Comedy");
    catMap["soap/melodrama/folkloric"] = QObject::tr("Soap/Melodrama/Folkloric");
    catMap["romance"] = QObject::tr("Romance");
    catMap["serious/classical/religious/historicalmovie/drama"] = QObject::tr("Serious/Classical/Religious/Historical Movie/Drama");
    catMap["adultmovie/drama"] = QObject::tr("Adult Movie/Drama");

    catMap["news/currentaffairs"] = QObject::tr("News/Current affairs");
    catMap["news/weatherreport"] = QObject::tr("News/Weather report");
    catMap["newsmagazine"] = QObject::tr("News magazine");
    catMap["documentary"] = QObject::tr("Documentary");
    catMap["discussion/interview/debate"] = QObject::tr("Discussion/Interview/Debate");

    catMap["show/gameshow"] = QObject::tr("Show/Game show");
    catMap["gameshow/quiz/contest"] = QObject::tr("Game show/Quiz/Contest");
    catMap["varietyshow"] = QObject::tr("Variety show");
    catMap["talkshow"] = QObject::tr("Talk show");

    catMap["sports"] = QObject::tr("Sports");
    catMap["specialevents"] = QObject::tr("Special events");
    catMap["sportsmagazines"] = QObject::tr("Sports magazines");
    catMap["football/soccer"] = QObject::tr("Football/Soccer");
    catMap["tennis/squash"] = QObject::tr("Tennis/Squash");
    catMap["teamsports"] = QObject::tr("Team sports");
    catMap["athletics"] = QObject::tr("Athletics");
    catMap["motorsport"] = QObject::tr("Motor sport");
    catMap["watersport"] = QObject::tr("Water sport");

    catMap["children's/youthprogrammes"] = QObject::tr("Children's/Youth programmes");
    catMap["pre-schoolchildren'sprogrammes"] = QObject::tr("Pre-school children's programmes");
    catMap["entertainmentprogrammesfor6to14"] = QObject::tr("Entertainment programmes for 6 to 14");
    catMap["entertainmentprogrammesfor10to 16"] = QObject::tr("Entertainment programmes for 10 to 16");
    catMap["informational/educational/schoolprogrammes"] = QObject::tr("Informational/Educational/School programmes");
    catMap["cartoons/puppets"] = QObject::tr("Cartoons/Puppets");

    catMap["music/ballet/dance"] = QObject::tr("Music/Ballet/Dance");
    catMap["rock/pop"] = QObject::tr("Rock/Pop");
    catMap["seriousmusic/classicalmusic"] = QObject::tr("Serious music/Classical music");
    catMap["folk/traditionalmusic"] = QObject::tr("Folk/Traditional music");
    catMap["jazz"] = QObject::tr("Jazz");
    catMap["musical/opera"] = QObject::tr("Musical/Opera");

    catMap["arts/culture"] = QObject::tr("Arts/Culture");
    catMap["performingarts"] = QObject::tr("Performing arts");
    catMap["finearts"] = QObject::tr("Fine arts");
    catMap["religion"] = QObject::tr("Religion");
    catMap["popularculture/traditionalarts"] = QObject::tr("Popular culture/Traditional arts");
    catMap["literature"] = QObject::tr("Literature");
    catMap["film/cinema"] = QObject::tr("Film/Cinema");
    catMap["experimentalfilm/video"] = QObject::tr("Experimental film/video");
    catMap["broadcasting/press"] = QObject::tr("Broadcasting/Press");

    catMap["social/politicalissues/economics"] = QObject::tr("Social/Political issues/Economics");
    catMap["magazines/reports/documentary"] = QObject::tr("Magazines/Reports/Documentary");
    catMap["economics/socialadvisory"] = QObject::tr("Economics/Social advisory");
    catMap["remarkablepeople"] = QObject::tr("Remarkable people");

    catMap["education/science/factualtopics"] = QObject::tr("Education/Science/Factual topics");
    catMap["nature/animals/environment"] = QObject::tr("Nature/Animals/Environment");
    catMap["technology/naturalsciences"] = QObject::tr("Technology/Natural sciences");
    catMap["medicine/physiology/psychology"] = QObject::tr("Medicine/Physiology/Psychology");
    catMap["foreigncountries/expeditions"] = QObject::tr("Foreign countries/Expeditions");
    catMap["social/spiritualsciences"] = QObject::tr("Social/Spiritual sciences");
    catMap["furthereducation"] = QObject::tr("Further education");
    catMap["languages"] = QObject::tr("Languages");

    catMap["leisurehobbies"] = QObject::tr("Leisure hobbies");
    catMap["tourism/travel"] = QObject::tr("Tourism/Travel");
    catMap["handicraft"] = QObject::tr("Handicraft");
    catMap["motoring"] = QObject::tr("Motoring");
    catMap["fitnessandhealth"] = QObject::tr("Fitness and Health");
    catMap["cooking"] = QObject::tr("Cooking");
    catMap["advertisement/shopping"] = QObject::tr("Advertisement/Shopping");
    catMap["gardening"] = QObject::tr("Gardening");

    return catMap;
}

QString Tano::Xmltv::mapCategory(const QString &cat)
{
    QString source = cat;
    source = source.replace(" ", "");
    source = source.replace(QRegExp("(\\(.*\\))"), "");
    source = source.toLower();

    return categoryMap()[source];
}
