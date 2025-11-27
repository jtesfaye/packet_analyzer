//
// Created by jeremiah tesfaye on 11/24/25.
//

#ifndef THROUGHPUTCHART_H
#define THROUGHPUTCHART_H
#include <QObject>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QTimer>

class ThroughputChart : public QWidget {
    Q_OBJECT

public:
    explicit ThroughputChart(QWidget *parent = nullptr);

public slots:
    void addValue(double bytes_per_second);

private slots:
    void appendValue();

private:

    QLineSeries *series;
    QChart *chart;
    QChartView *chart_view;
    QValueAxis *x_seconds;
    QValueAxis *y_bytes;
    QTimer *timer;
    double latest_value = 0.0;
    int xIndex = 0;
    const int max_points = 200;

};
#endif //THROUGHPUTCHART_H
