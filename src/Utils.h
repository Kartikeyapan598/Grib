#pragma once

#include <QWidget>
#include <QMessageBox>


inline void CreateMessageBoxInfo(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::information(parent, title, mssg);
    return;
}

inline void CreateMessageBoxWarn(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::warning(parent, title, mssg);
    return;
}

inline void CreateMessageBoxCrit(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::critical(parent, title, mssg, QMessageBox::Ok, QMessageBox::NoButton);
    return;
}

inline void CreateMessageBoxAbout(QWidget* parent, QString title, QString mssg)
{
    QMessageBox::about(parent, title, mssg);
    return;
}

inline QMessageBox::StandardButton MultipleButtonBox(QWidget* parent, QString title, QString mssg, QMessageBox var)
{
    return QMessageBox::question(parent, "Standard Button", "question",  QMessageBox::Yes
                                                                        | QMessageBox::No
                                                                        | QMessageBox::Close,
                                                                        QMessageBox::NoButton);

}

static int randomBetween(int low, int high)
{
    return (rand() % ((high + 1) - low) + low);
}

inline float toMerc (float latInDeg)
{
    float rad = latInDeg * M_PI /180.0;
    return log (tan (M_PI / 4 + rad / 2)) * 180 / M_PI;
}

inline float fromMerc (float deg)
{
    float rad = deg * M_PI /180.0;
    return atan (sinh (rad)) * 180 / M_PI;
}

inline void fillColorMap (std::unordered_map<int, QColor> &m,
                                                     std::vector<int> &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if (i == v.size() - 1) {
            break;
        }
        int k1 = v.at(i);
        int k2 = v.at(i + 1);
        for (int j = k1 + 1; j < k2; j++)
        {
            int ra = m.at(k1).red(), ga = m.at(k1).green(), ba = m.at(k1).blue();
            int rb = m.at(k2).red(), gb = m.at(k2).green(), bb = m.at(k2).blue();

            int rj = (rb - ra > 0) ? ra + (rb - ra)*(j - k1)/(k2 - k1) : ra - (ra - rb)*(j - k1)/(k2 - k1);
            int gj = (gb - ga > 0) ? ga + (gb - ga)*(j - k1)/(k2 - k1) : ga - (ga - gb)*(j - k1)/(k2 - k1);
            int bj = (bb - ba > 0) ? ba + (bb - ba)*(j - k1)/(k2 - k1) : ba - (ba - bb)*(j - k1)/(k2 - k1);

            m[j] = QColor(rj, gj, bj);
        }
    }
}

inline QPointF barb(double ang, double len, bool smallBarb = false)
{
    double barbLen = smallBarb ? len / 4 : len / 2;
    double phi = M_PI / 4;
    double bdx = barbLen * qCos(ang + phi);
    double bdy = barbLen * qSin(ang + phi);
    return QPointF(-bdx, -bdy);
}

inline QPair<QPointF, QPointF> triangle(double ang, double len)
{
    double hyp = len / 2;
    double phi = M_PI / 4;
    double bdx1 = hyp * qSin(phi) * qSin(ang);
    double bdy1 = hyp * qSin(phi) * qCos(ang);
    double bdx2 = hyp * qCos(phi) * qCos(ang);
    double bdy2 = hyp * qCos(phi) * qSin(ang);
    QPair<QPointF, QPointF> pair;
    pair.first = QPointF(bdx1, -bdy1);
    pair.second = QPointF(bdx2, bdy2);
    return pair;
}
