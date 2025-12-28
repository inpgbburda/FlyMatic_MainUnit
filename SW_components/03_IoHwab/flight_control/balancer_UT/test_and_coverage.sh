

#!/usr/bin/env bash
set -euo pipefail

# Coverage generator for flight_control sources (recursively).
# - Auto-discovers C/C++ sources in the parent directory.
# - Excludes mocks, stubs, unit-test folders, and run_all_tests.cpp.
# - Builds UTs, runs them to produce .gcda, then emits HTML/XML/TXT under
#   builds_UT/coverage (ignored by git).
#
# Usage:
#   ./test_and_coverage.sh
#
# Requirements:
#   - gcovr (install via: python3 -m pip install gcovr)
#   - gcov (GCC). On Windows, install via Cygwin packages: gcc-core, gcc-g++
#
# Notes:
#   - Designed to handle deep Cygwin paths; runs gcov on all filtered .gcno
#     files found under builds_UT.
#   - Source filter root: the parent flight_control directory.

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../../../.." && pwd)"
build_dir="$repo_root/SW_components/03_IoHwab/flight_control/balancer_UT/builds_UT"
coverage_dir="$build_dir/coverage"
# Detect source directory one level above (flight_control)
source_dir="$(cd "$script_dir/.." && pwd)"

mkdir -p "$coverage_dir"

if ! command -v gcovr >/dev/null 2>&1; then
  echo "gcovr not found. Install with: python3 -m pip install gcovr" >&2
  exit 1
fi

if ! command -v gcov >/dev/null 2>&1; then
  echo "gcov not found. Install GCC/gcov in Cygwin (select gcc-core/gcc-g++)" >&2
  exit 1
fi

###############################################################################
# 1) Build unit tests
###############################################################################
echo "[1/4] Building unit tests in: $build_dir"
(
  cd "$build_dir"
  make all
)

###############################################################################
# 2) Run unit tests to produce .gcda data
###############################################################################
echo "[2/4] Running unit tests..."
(
  cd "$build_dir"
  if [ -x ./balancer_UT.exe ]; then
    ./balancer_UT.exe || true
  else
    echo "Warning: balancer_UT.exe not found in $build_dir" >&2
  fi
)

###############################################################################
# 3) Generate .gcov files for all relevant .gcno (handles deep Cygwin paths)
###############################################################################
echo "[3/4] Generating .gcov files via gcov..."
mapfile -t gcno_files < <(find "$build_dir" -type f -name '*.gcno' | grep -E -v '/mocks/|/stubs/|/_UT/|run_all_tests\.cpp')
if [ "${#gcno_files[@]}" -gt 0 ]; then
  (
    cd "$build_dir"
    for f in "${gcno_files[@]}"; do
      gcov -b -c "$f" || echo "Warning: gcov failed on $f" >&2
    done
  )
else
  echo "Warning: No .gcno files found under $build_dir (filtered)" >&2
fi

###############################################################################
# 4) Generate HTML/XML/TXT reports via gcovr
###############################################################################
echo "[4/4] Generating gcovr reports..."
common_opts=(
  -r "$repo_root"
  --object-directory "$build_dir"
  # Include only sources under the parent flight_control directory
  --filter "$source_dir"
  # Exclude auxiliary sources
  --exclude ".*mocks/.*"
  --exclude ".*stubs/.*"
  --exclude ".*_UT/.*"
  --exclude ".*run_all_tests\\.cpp$"
)

# HTML report (with per-file details)
gcovr "${common_opts[@]}" --html --html-details -o "$coverage_dir/index.html"

# XML report (Cobertura)
gcovr "${common_opts[@]}" --xml -o "$coverage_dir/coverage.xml"

# Text summary
gcovr "${common_opts[@]}" --print-summary > "$coverage_dir/coverage.txt"

echo "Coverage reports written to: $coverage_dir"
echo " - HTML: $coverage_dir/index.html"
echo " - XML:  $coverage_dir/coverage.xml"
echo " - TXT:  $coverage_dir/coverage.txt"
