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

void saveToFile(const QVector<QPair<double, double>>& data, const QString& filename) {
    // Open the file for writing
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing!";
        return;
    }

    QTextStream out(&file);

    // Write each pair in the format "x y"
    for (const auto& pair : data) {
        out << pair.first << " " << pair.second << "\n";
    }

    file.close();
    qDebug() << "Data written to" << filename;
}

// Function to detect peaks in yValues and return their corresponding (x, y) pairs
QVector<QPair<double, double>> detectPeaks(const QVector<double> &xValues, const QVector<double> &yValues) {
    QVector<QPair<double, double>> peaks;

    for (int i = 1; i < yValues.size() - 1; ++i) {
        // Check if it's a local maximum and if y-value is greater than 2000
        if (yValues[i] > yValues[i - 1] && yValues[i] > yValues[i + 1] && yValues[i] > 3000) {
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
    QVector<double> xValues, yValues, xValuesa, yValuesa;
    QString filePath = "slit_fechada_montagem.txt"; // Change this to your actual file path
    QString filePatha = "ref_peaks_1s_5ac_2000.txt"; // Change this to your actual file pat

    if (!readData(filePath, xValues, yValues)) {
        return -1;
    }
    if (!readData(filePatha, xValuesa, yValuesa)) {
        return -1;
    }

    QVector<QPair<double, double>> peaks = detectPeaks(xValues, yValues);
    QVector<QPair<double, double>> error;

    if (peaks.isEmpty()) {
        qDebug() << "No peaks found with y-value greater than 2000.";
    } else {
        qDebug() << "Detected peaks with y > 2000 (x, y):";
        int i = 0;
        for (const auto &peak : peaks) {
            qDebug() << "x:" << peak.first << ", y:" << peak.second << i;
            qDebug() << yValuesa[i];
            error.append(qMakePair(i+1, peak.first - yValuesa[i]));
            i++;
        }
    }
    // Filepath to save data
    QString filename = "output.txt";

    // Call the function to save the data
    saveToFile(error, filename);
    qDebug() << error;
    return a.exec();
}
