//
// Created by jeremiah tesfaye on 11/24/25.
//

#include <QVBoxLayout>
#include <view/ThroughputChart.h>

ThroughputChart::ThroughputChart(QWidget *parent)
: QWidget(parent)
, series(new QLineSeries(this))
, chart(new QChart())
, chart_view(new QChartView(chart, this))
, timer(new QTimer(this)) {

    chart->addSeries(series);
    x_seconds = new QValueAxis();
    x_seconds->setRange(0, max_points);
    x_seconds->setLabelFormat("%d");
    x_seconds->setTitleText("Sample");

    y_bytes = new QValueAxis();
    y_bytes->setRange(0, 1000);
    y_bytes->setLabelFormat("%d");
    y_bytes->setTitleText("Bytes/s");

    chart->addAxis(x_seconds, Qt::AlignBottom);
    chart->addAxis(y_bytes, Qt::AlignLeft);
    series->attachAxis(x_seconds);
    series->attachAxis(y_bytes);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(chart_view);
    setLayout(layout);

    connect(timer, &QTimer::timeout, this, &ThroughputChart::appendValue);

    timer->start(50);

}

void ThroughputChart::addValue(double bytes_per_second) {
    latest_value = bytes_per_second;
}

void ThroughputChart::appendValue() {

    if (series->count() > max_points) {
        series->removePoints(0, series->count() - max_points);
    }

    series->append(xIndex - max_points, xIndex);
}


