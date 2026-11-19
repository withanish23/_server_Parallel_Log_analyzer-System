# Parallel Server Log Analysis Using C++ and OpenMP

## 1. Project overview

This project reads a server log file and counts entries by severity level. It implements the same analysis in two ways:

- **Sequential analysis:** processes log entries one at a time using a normal loop.
- **Parallel analysis:** processes log entries using an OpenMP parallel loop and reduction variables.

The two results are compared to check that parallel execution produces the same counts as sequential execution. A separate benchmark measures and compares their execution times.

The project demonstrates C++17, file handling, vectors, functions, classes, loops, OpenMP parallelism, reduction, testing and performance measurement.

## 2. Scope

The program recognises four severity levels: `INFO`, `WARNING`, `ERROR` and `CRITICAL`. Any other level is counted as `UNKNOWN`.

The project does not include anomaly detection, machine learning, a graphical interface, a database, networking or real-time monitoring.

## 3. Project structure

```text
ParallelProgramming_Retake_AnishGautam/
|
|-- data/
|   |-- server.log
|   |-- large_server.log
|
|-- include/
|   |-- LogReader.h
|   |-- LogRecord.h
|   |-- SequentialAnalyzer.h
|   |-- ParallelAnalyzer.h
|
|-- src/
|   |-- main.cpp
|   |-- LogReader.cpp
|   |-- SequentialAnalyzer.cpp
|   |-- ParallelAnalyzer.cpp
|
|-- tests/
|   |-- test_log_reader.cpp
|   |-- test_sequential.cpp
|   |-- test_parallel.cpp
|   |-- test_benchmark.cpp
|   |-- generate_logs.cpp
|
|-- results/
|-- build/
|-- .gitignore
|-- README.md
```

## 4. Log file format

Each valid line follows this format:

```text
YYYY-MM-DD HH:MM:SS LEVEL message
```

Example:

```text
2026-11-13 10:30:15 INFO Server started successfully
2026-11-13 10:31:05 WARNING High memory usage detected
2026-11-13 10:31:20 ERROR Database connection failed
2026-11-13 10:36:10 CRITICAL Server response timeout
```

The reader skips lines that do not contain a date, time and level. It stores the timestamp, level and message in a `LogRecord`.

## 5. Requirements

- C++17-compatible compiler
- OpenMP support
- GCC with OpenMP (for example, MSYS2 UCRT64 GCC on Windows)
- Visual Studio Code is optional

## 6. Build and run (MSYS2 UCRT64 terminal)

Open the project root folder in an MSYS2 UCRT64 terminal.

Create the build directory:

```bash
mkdir -p build
```

Build the main application:

```bash
g++ -std=c++17 -O2 -fopenmp -Iinclude src/main.cpp src/LogReader.cpp src/SequentialAnalyzer.cpp src/ParallelAnalyzer.cpp -o build/log_analyzer.exe
```

Run it with the sample file:

```bash
./build/log_analyzer.exe
```

A different log file can be supplied as the first argument:

```bash
./build/log_analyzer.exe data/large_server.log
```

## 7. Build and run the tests

Build and run the log reader test:

```bash
g++ -std=c++17 -O2 -fopenmp -Iinclude tests/test_log_reader.cpp src/LogReader.cpp -o build/test_log_reader.exe
./build/test_log_reader.exe
```

Build and run the sequential analysis test:

```bash
g++ -std=c++17 -O2 -fopenmp -Iinclude tests/test_sequential.cpp src/LogReader.cpp src/SequentialAnalyzer.cpp -o build/test_sequential.exe
./build/test_sequential.exe
```

Build and run the parallel analysis test:

```bash
g++ -std=c++17 -O2 -fopenmp -Iinclude tests/test_parallel.cpp src/LogReader.cpp src/SequentialAnalyzer.cpp src/ParallelAnalyzer.cpp -o build/test_parallel.exe
./build/test_parallel.exe
```

Build and run the benchmark:

```bash
g++ -std=c++17 -O2 -fopenmp -Iinclude tests/test_benchmark.cpp src/LogReader.cpp src/SequentialAnalyzer.cpp src/ParallelAnalyzer.cpp -o build/test_benchmark.exe
./build/test_benchmark.exe
```

Each test returns exit code `0` when successful and a non-zero exit code when it fails.

## 8. Generate the benchmark dataset

The supplied `data/large_server.log` contains 1,000,000 entries. To regenerate it:

```bash
g++ -std=c++17 -O2 tests/generate_logs.cpp -o build/generate_logs.exe
./build/generate_logs.exe
```

A different positive number of entries can be supplied:

```bash
./build/generate_logs.exe 500000
```

Regenerating the file replaces the existing large dataset.

## 9. How the parallel implementation works

The sequential analyzer loops through the vector and increments the matching counter for each log entry.

The parallel analyzer uses:

```cpp
#pragma omp parallel for reduction(+:info, warning, error, critical, unknown)
```

OpenMP divides loop iterations among threads. Each thread updates its own private copy of the reduction variables. At the end of the parallel region, OpenMP combines the partial counts. This avoids multiple threads updating the same counter at the same time.

The total number of entries is the vector size. The five severity counters should add up to the total number of entries.

## 10. Benchmark methodology

The benchmark:

1. Loads the large log file before timing begins.
2. Performs one warm-up run for each implementation.
3. Runs each implementation ten times.
4. Measures only the analysis functions using `std::chrono::steady_clock`.
5. Calculates the average execution time.
6. Compares the final sequential and parallel results.

File reading is excluded so the benchmark focuses on the analysis work.

**Speedup**

```text
Speedup = Sequential average time / Parallel average time
```

**Parallel efficiency**

```text
Efficiency = (Speedup / Number of threads) * 100
```

A speedup greater than 1 means the parallel implementation was faster in that measurement. Actual results depend on the computer, compiler, thread availability and system load. Parallel execution is not guaranteed to be faster for every dataset.

## 11. Important implementation files

- `LogRecord.h`: defines the structure used to store one log entry.
- `LogReader.h` and `LogReader.cpp`: read and parse the log file.
- `SequentialAnalyzer.h` and `SequentialAnalyzer.cpp`: perform sequential counting.
- `ParallelAnalyzer.h` and `ParallelAnalyzer.cpp`: perform counting with OpenMP.
- `main.cpp`: runs both implementations and compares their results.
- `tests/`: contains separate checks and the benchmark program.

## 12. Limitations

The reader expects the documented whitespace-separated log format. It skips malformed lines. Severity names are case-sensitive. The counters use integer values, which are sufficient for the supplied one-million-entry dataset. Benchmark results are specific to the machine and conditions used for the run.
