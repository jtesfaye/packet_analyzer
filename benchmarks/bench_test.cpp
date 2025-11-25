//
// Created by jeremiah tesfaye on 11/14/25.
//

#include <deque>
#include <benchmark/benchmark.h>
#include <vector>
#include <util/SparsePacketBuffer.h>

static void BM_mybuffer(benchmark::State& state) {

    for (auto _ : state) {

        SparsePacketBuffer<std::string> buffer(100);

        for (int i = 0; i < 100; i++) {
            buffer.add(i, std::move(std::to_string(i)));
        }
        benchmark::DoNotOptimize(buffer);
    }
}

static void BM_mybuffer_m(benchmark::State& state) {

    for (auto _ : state) {

        SparsePacketBuffer<std::string> buffer(100);

        for (int i = 0; i < 100; i++) {
            buffer.add(i, std::to_string(i));
        }
        benchmark::DoNotOptimize(buffer);
    }
}

static void BM_vector(benchmark::State& state) {

    for (auto _ : state) {
        std::vector<std::string> v;
        v.resize(100);
        for (int i = 0; i < 100; i++) {
            v[i] = std::to_string(i);
        }

        benchmark::DoNotOptimize(v);
    }
}

static void BM_deque(benchmark::State& state) {

    for (auto _ : state) {
        std::deque<std::string> v;
        v.resize(100);
        for (int i = 0; i < 100; i++) {
            v[i] = std::move(std::to_string(i));
        }

        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(BM_mybuffer_m);
BENCHMARK(BM_mybuffer);
BENCHMARK(BM_vector);
BENCHMARK(BM_deque);

BENCHMARK_MAIN();
