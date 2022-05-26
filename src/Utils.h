#pragma once

#include <QWidget>
#include <QMessageBox>
#include <cmath>
#include <QtMath>

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

inline void fillColorMap (std::unordered_map<int, QColor> &m, std::vector<int> &v)
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

static void interpolationHelper (std::vector<std::vector<double>> &refinedData, int d, int i, int j, double a00, double a10, double a01, double a11)
{
    if (d == 1)
    {
        refinedData[i][j] = a00;
        refinedData[i][j + d] = a01;
        refinedData[i + d][j] = a10;
        refinedData[i + d][j + d] = a11;
        return;
    }

    double a0 = (a00 + a10 + a01 + a11) / 4.0;
    double a1 = (a00 + a10) / 2.0;
    double a2 = (a00 + a01) / 2.0;
    double a3 = (a11 + a01) / 2.0;
    double a4 = (a11 + a10) / 2.0;

    interpolationHelper(refinedData, d / 2, i, j, a00, a1, a2, a0);
    interpolationHelper(refinedData, d / 2, i + d / 2, j, a1, a10, a0, a4);
    interpolationHelper(refinedData, d / 2, i, j + d / 2, a2, a0, a01, a3);
    interpolationHelper(refinedData, d / 2, i + d / 2, j + d / 2, a0, a4, a3, a11);
}


static std::vector<std::vector<double>> getInterpolated2dArray (std::vector<std::vector<double>> &data, int res)
{
    int d = res * 2;
    int rows = (data.size() - 1) * d + 1;
    int cols = (data[0].size() - 1) * d + 1;

    std::vector<std::vector<double>> refinedArray (rows, std::vector<double>(cols, 0.0));
//    std::vector<std::vector<double>> refinedArray (rows, std::vector<double>(cols, -1));

    for (size_t j = 0; j < data[0].size() - 1; j++)
    {
        for (size_t i = 0; i < data.size() - 1; i++)
        {
            double a00 = data[i][j];
            double a10 = data[i + 1][j];
            double a01 = data[i][j + 1];
            double a11 = data[i + 1][j + 1];
            interpolationHelper(refinedArray, d, i * d, j * d, a00, a10, a01, a11);
        }
    }

    return refinedArray;
}

inline QVector<QLineF> getIsoLines (bool b1, bool b2, bool b3, bool b4, bool bx, QPointF p1, QPointF p2, QPointF p3, QPointF p4, double pr1, double pr2, double pr3, double pr4, double pr)
{
    double dy = (p3 - p1).y();
    double dx = (p2 - p1).x();

    double dpx1 = pr2 - pr1;
    double dpx2 = pr4 - pr3;
    double dpy1 = pr3 - pr1;
    double dpy2 = pr4 - pr2;

    QVector<QLineF> lines;


/* Grid -

   p3 .. p4
   .. px ..
   p1 .. p2

*/


/* Case 0 - no lines

      0    0

      0    0
*/

/* Case 1

      0    0

      1    0
*/
    if (b1 == 1 && b2 == 0 && b3 == 0 && b4 == 0)
    {
        QPointF p, q;

        p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
        q = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 2

      0    0

      0    1
*/
    else if (b1 == 0 && b2 == 1 && b3 == 0 && b4 == 0)
    {
        QPointF p, q;

        p = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());
        q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 3

      0    0

      1    1
*/
    else if (b1 == 1 && b2 == 1 && b3 == 0 && b4 == 0)
    {
        QPointF p, q;

        p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
        q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 4

      0    1

      0    0
*/
    else if (b1 == 0 && b2 == 0 && b3 == 0 && b4 == 1)
    {
        QPointF p, q;

        p = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());
        q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 5 - saddle case

      0    1

      1    0
*/
    else if (b1 == 1 && b2 == 0 && b3 == 0 && b4 == 1)
    {
        if (bx == 1)
        {
        /*
              0     1
                 1
              1     0
        */
            QPointF p, q;

            p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
            q = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());

            QLineF line (p, q);
            lines.push_back(line);
            qInfo() << line;

            p = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());
            q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

            line = QLineF(p, q);

            lines.push_back(line);
        }
        else
        {
        /*
              0     1
                 0
              1     0
        */
            QPointF p, q;

            p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
            q = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());

            QLineF line (p, q);
            lines.push_back(line);
            qInfo() << line;

            p = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());
            q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

            line = QLineF(p, q);

            lines.push_back(line);
        }
    }

/* Case 6

      0    1

      0    1
*/
    else if (b1 == 0 && b2 == 1 && b3 == 0 && b4 == 1)
    {
        QPointF p, q;

        p = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());
        q = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());

        QLineF line (p, q);
        lines.push_back(line);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 7

      0    1

      1    1
*/
    else if (b1 == 1 && b2 == 1 && b3 == 0 && b4 == 1)
    {
        QPointF p, q;

        p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
        q = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());

        QLineF line (p, q);
        lines.push_back(line);
        qInfo() << line;
        lines.push_back(line);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 8

      1    0

      0    0
*/
    else if (b1 == 0 && b2 == 0 && b3 == 1 && b4 == 0)
    {
        QPointF p, q;

        p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
        q = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());

        QLineF line (p, q);
        lines.push_back(line);
        qInfo() << line;
        lines.push_back(line);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 9

       1    0

       1    0
*/
    else if (b1 == 1 && b2 == 0 && b3 == 1 && b4 == 0)
    {
        QPointF p, q;

        p = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());
        q = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());

        QLineF line (p, q);
        lines.push_back(line);
        qInfo() << line;
        lines.push_back(line);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 10 - saddle case

      1    0

      0    1
*/
    else if (b1 == 0 && b2 == 1 && b3 == 1 && b4 == 0)
    {
        if (bx == 1)
        {
        /*
              1     0
                 1
              0     1
        */
            QPointF p, q;

            p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
            q = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());

            QLineF line (p, q);
            lines.push_back(line);
            qInfo() << line;

            p = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());
            q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

            line = QLineF(p, q);

            lines.push_back(line);
        }
        else
        {
        /*
              1     0
                 0
              0     1
        */
            QPointF p, q;

            p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
            q = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());

            QLineF line (p, q);
            lines.push_back(line);
            qInfo() << line;

            p = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());
            q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

            line = QLineF(p, q);

            lines.push_back(line);
        }
    }

/* Case 11

       1    0

       1    1
*/
    else if (b1 == 1 && b2 == 1 && b3 == 1 && b4 == 0)
    {
        QPointF p, q;

        p = QPointF(p1.x() + (pr - pr3) * (dx / dpx2), p3.y());
        q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 12

      1    1

      0    0
*/
    else if (b1 == 0 && b2 == 0 && b3 == 1 && b4 == 1)
    {
        QPointF p, q;

        p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
        q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 13

       1    1

       1    0
*/
    else if (b1 == 1 && b2 == 0 && b3 == 1 && b4 == 1)
    {
        QPointF p, q;

        p = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());
        q = QPointF(p2.x(), p1.y() + (pr - pr2) * (dy / dpy2));

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 14

      1    1

      0    1
*/
    else if (b1 == 0 && b2 == 1 && b3 == 1 && b4 == 1)
    {
        QPointF p, q;

        p = QPointF(p1.x(), p1.y() + (pr - pr1) * (dy / dpy1));
        q = QPointF(p1.x() + (pr - pr1) * (dx / dpx1), p1.y());

        QLineF line (p, q);
        qInfo() << line;
        lines.push_back(line);
    }

/* Case 15 - no lines

      1    1

      1    1
*/

    return lines;
}
