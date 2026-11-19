# Development of a Parallel Server Log Analysis System Using C++ and OpenMP

## 1. Introduction

Server log files contain records about events that happen while a server is running. Reading and counting these records can be done sequentially or by using parallel processing. This project implements both approaches and compares their correctness and execution time.

The main purpose of the project is to demonstrate concepts from the Parallel Programming course through a small and understandable C++ application.

## 2. Project objectives

The objectives are:

- Read and store records from a server log text file.
- Count records by severity level.
- Implement the same analysis using sequential programming.
- Implement the analysis using OpenMP parallel programming.
- Compare the outputs of both implementations.
- Measure execution time and calculate speedup and parallel efficiency.

## 3. Project scope

The application recognises `INFO`, `WARNING`, `ERROR` and `CRITICAL` severity levels. Any other level is counted as `UNKNOWN`.

The project focuses on log reading, counting, parallelisation, correctness testing and performance measurement. It does not implement anomaly detection, machine learning, a graphical user interface, a database, networking or real-time monitoring.

## 4. Tools and technologies

- Programming language: C++17
- Parallel programming library: OpenMP
- Compiler used for the recorded Windows benchmark: GCC 16.2.0, MSYS2 UCRT64
- Additional validation compiler: GCC 14.2.0 on Linux
- Timing: `std::chrono::steady_clock`
- Development environment: Visual Studio Code

## 5. System design and implementation

The project is divided into small modules.

### 5.1 LogRecord

`LogRecord` is a structure containing three string fields: timestamp, level and message. Each valid line read from the file is stored as one record.

### 5.2 LogReader

`LogReader` opens the requested text file and reads it line by line. It extracts the date, time and severity level, then stores the remaining text as the message. Incomplete lines are skipped. The reader also handles Windows CRLF line endings.

### 5.3 SequentialAnalyzer

The sequential analyzer processes the vector of log records using a normal `for` loop. For each record, it checks the severity level and increments the corresponding counter. It also counts the total number of entries.

### 5.4 ParallelAnalyzer

The parallel analyzer performs the same counting operation with an OpenMP `parallel for` directive.

The implementation uses the OpenMP reduction clause:

```cpp
#pragma omp parallel for reduction(+:info, warning, error, critical, unknown)
```

The loop iterations are distributed across the available threads. Each thread uses private copies of the reduction variables. OpenMP combines the partial values at the end of the parallel region. This avoids concurrent updates to the same shared counter.

### 5.5 Main application

The main application reads the sample log file, runs both analyzers, displays their results and checks whether all counters match. A different input file can be supplied as a command-line argument.

## 6. Testing

Separate test programs are provided for the log reader, sequential analyzer, parallel analyzer and benchmark.

The tests check:

- Correct loading and parsing of the sample log file.
- Skipping of incomplete lines.
- Retention of an unrecognised severity level.
- Correct counts for the sample dataset.
- Handling of unknown severity levels.
- Handling of empty input.
- Equality of sequential and parallel results.
- Successful processing of the large dataset.

All five validation programs (the reader test, sequential test, parallel test, main application and benchmark) compiled successfully with C++17 and OpenMP in the Linux validation environment. All completed successfully.

For the sample file, the expected results are 10 total entries: 4 INFO, 2 WARNING, 3 ERROR, 1 CRITICAL and 0 UNKNOWN.

For the supplied large dataset, both implementations counted 1,000,000 entries: 250,000 INFO, 250,000 WARNING, 250,000 ERROR, 250,000 CRITICAL and 0 UNKNOWN.

## 7. Performance measurement

The benchmark loads the dataset before starting the timer, so file-reading time is not included. It performs one warm-up run for each analyzer and then measures ten runs of each implementation using `std::chrono::steady_clock`.

The average execution time is calculated for both approaches.

Speedup is calculated as:

```text
Speedup = Sequential average time / Parallel average time
```

Parallel efficiency is calculated as:

```text
Efficiency = (Speedup / Number of threads) * 100
```

### 7.1 Previously recorded Windows benchmark

| Measurement | Result |
|---|---:|
| Dataset size | 1,000,000 entries |
| Threads | 4 |
| Repetitions | 10 |
| Sequential average | 4.30925 ms |
| Parallel average | 3.03283 ms |
| Speedup | 1.42087x |
| Parallel efficiency | 35.5217% |

The sequential and parallel results were identical.

### 7.2 Additional Linux validation benchmark

A separate validation run was performed with GCC 14.2.0 on Linux.

| Measurement | Result |
|---|---:|
| Dataset size | 1,000,000 entries |
| Threads | 4 |
| Repetitions | 10 |
| Sequential average | 6.01910 ms |
| Parallel average | 4.76634 ms |
| Speedup | 1.26283x |
| Parallel efficiency | 31.57086% |

The sequential and parallel results were identical in this run as well.

The two benchmark results are from different environments and should not be compared as if they were measured on the same computer. Execution time can change with hardware, compiler, thread availability and system load.

## 8. Discussion

The benchmark runs show that parallel analysis was faster than sequential analysis for the supplied one-million-entry dataset in both recorded environments.

The speedup is not equal to the number of threads. Parallel execution has overhead, and the work cannot always be divided and processed with perfect efficiency. The result demonstrates that parallel programming can improve execution time for this workload, but the improvement depends on the environment and workload size.

Correctness is important when parallelising a program. Comparing the counters from both implementations provides a direct check that parallel processing has not changed the result.

## 9. Limitations

The reader expects the documented whitespace-separated log format. Incomplete lines are skipped, and severity names are case-sensitive. The counters use integer types and are intended for the supplied dataset size. Performance measurements may vary between runs and computers.

## 10. Conclusion

The project demonstrates how the same task can be implemented using sequential and parallel programming in C++17. The sequential analyzer provides a simple baseline, while the parallel analyzer uses OpenMP reduction to count log entries across multiple threads.

Testing confirms that both approaches produce matching results for the supplied datasets and edge cases. The benchmark provides a practical comparison of execution time, speedup and parallel efficiency.

The project meets its intended educational purpose by demonstrating file handling, structured programming, loops, vectors, OpenMP parallel loops, reduction, correctness testing and performance measurement in a focused application.
