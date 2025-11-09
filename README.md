# College Recommender CLI

This repository contains a C++ command-line tool that ranks colleges based on weighted criteria. It ingests a CSV file of college statistics, applies a configurable set of filters, computes weighted scores, and prints the top recommendations using either a heap-based ranking or a hash map search.

## Prerequisites
- A C++17-compatible compiler (e.g., `g++` 9 or newer or MSVC 2019+)
- CMake 3.26 or newer
- A CSV data set containing the expected fields (sample files are included in the `data/` directory)

## Building the project
From the root of the repository run the following command:
cmake --build build

This produces the executable `build/Project2` (or `Project2.exe` on Windows).

## Running the recommender
Run the python file and input the information needed.

It expects 12 arguments in addition to the program name, which you cn add in the frontend. The arguments are:
| Argument | Description |
| --- | --- |
| `csv-path` | Path to the CSV file with college data. The repository ships example files under `data/`. |
| `state` | Two-letter state filter (e.g., `FL`). Use `all` or an empty string to disable. |
| `control` | Institution control filter (`Public`, `Private`, etc.). Use `all` to disable. |
| `max_tuition` | Maximum tuition (integer). Use `0` to disable. |
| `min_acceptance` | Minimum acceptance rate (0–100). Use `0` to disable. |
| `min_sat` | Minimum SAT score (integer). Use `0` to disable. |
| `undergrad_size` | Minimum undergraduate population (integer). Use `0` to disable. |
| `w_tuition` | Weight to apply to tuition in the ranking score (float). |
| `w_acceptance` | Weight for acceptance rate (float). |
| `w_sat` | Weight for SAT score (float). |
| `w_undergrad` | Weight for undergraduate population (float). |
| `algorithm` | Ranking algorithm to use: `heap` (available) or `hashmap` (planned). |

## Project structure
- `src/` – C++ source files and headers.
- `data/` – Example CSV files and data-preparation scripts.
- `resources/` – Miscellaneous project documentation.