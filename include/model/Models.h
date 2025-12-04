//
// Created by jeremiah tesfaye on 12/1/25.
//

#ifndef MODELS_H
#define MODELS_H

#include <model/RowModel.h>
#include <model/DetailModel.h>
#include <view/ThroughputChart.h>

struct Models {

    RowModel* row_model;
    DetailModel* detail_model;
    ThroughputChart* throughput_chart;

    Models(RowModel* r, DetailModel* d, ThroughputChart* c)
    : row_model(r)
    , detail_model(d)
    , throughput_chart(c) {}

};


#endif //MODELS_H
