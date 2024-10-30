#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDebug>

// Function to read data from a .txt file and store it in two vectors
bool readData(const QString &filePath, QVector<double> &xValues, QVector<double> &yValues) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for reading:" << filePath;
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        double x, y;
        in >> x >> y;
        xValues.append(x);
        yValues.append(y);
    }

    return true;
}

// Function to detect peaks in yValues and return their corresponding (x, y) pairs
QVector<QPair<double, double>> detectPeaks(const QVector<double> &xValues, const QVector<double> &yValues) {
    QVector<QPair<double, double>> peaks;

    for (int i = 1; i < yValues.size() - 1; ++i) {
        // Check if it's a local maximum and if y-value is greater than 2000
        if (yValues[i] > yValues[i - 1] && yValues[i] > yValues[i + 1] && yValues[i] > 2000) {
            peaks.append(qMakePair(xValues[i], yValues[i]));
        }
    }

    return peaks;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QVector<double> xValues, yValues;
    QString filePath = "5.txt"; // Change this to your actual file path

    if (!readData(filePath, xValues, yValues)) {
        return -1;
    }

    QVector<QPair<double, double>> peaks = detectPeaks(xValues, yValues);

    if (peaks.isEmpty()) {
        qDebug() << "No peaks found with y-value greater than 2000.";
    } else {
        qDebug() << "Detected peaks with y > 2000 (x, y):";
        for (const auto &peak : peaks) {
            qDebug() << "x:" << peak.first << ", y:" << peak.second;
        }
    }
    return a.exec();
}
