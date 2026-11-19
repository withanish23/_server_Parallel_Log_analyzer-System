# Benchmark Results

The benchmark measures only the sequential and parallel analysis functions. Reading the log file is completed before timing. Each implementation receives one warm-up run and then ten timed runs. Four OpenMP threads are configured.

## Previously recorded Windows run

The following values were verified during the project work on 17 November 2026:

| Item | Result |
|---|---:|
| Compiler | GCC 16.2.0 (MSYS2 UCRT64) |
| Dataset | 1,000,000 log entries |
| Threads | 4 |
| Timed repetitions | 10 |
| Sequential average | 4.30925 ms |
| Parallel average | 3.03283 ms |
| Speedup | 1.42087x |
| Parallel efficiency | 35.5217% |
| Sequential/parallel results | Identical |

## Additional validation run

The project was also compiled and run in the available Linux validation environment on 19 November 2026. This is a separate machine/compiler run and should not be mixed with the Windows result above.

| Item | Result |
|---|---:|
| Compiler | GCC 14.2.0 (Linux) |
| Dataset | 1,000,000 log entries |
| Threads | 4 |
| Timed repetitions | 10 |
| Sequential average | 6.01910 ms |
| Parallel average | 4.76634 ms |
| Speedup | 1.26283x |
| Parallel efficiency | 31.57086% |
| Sequential/parallel results | Identical |

## Interpretation

Both recorded runs produced a speedup greater than 1, meaning the parallel implementation was faster in those measurements. The measured speedup is below four times because parallel execution has overhead and the workload, hardware and memory system limit scaling.

Benchmark values depend on the computer, compiler, runtime, thread availability and background system activity. For a final submission, run the benchmark on the computer used for the project demonstration and report that machine's output with its compiler and thread settings.
