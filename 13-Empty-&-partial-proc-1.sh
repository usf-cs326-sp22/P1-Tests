source "${TEST_LIB}/funcs.bash"

test_start "Empty/partially-populated procfs" \
"Ensures the inspector can handle malformed, damaged, or missing proc entries without crashing"

./inspector -op ./tests/inputs/dummyproc1 || test_end 1

./inspector -op ./tests/inputs/dummyproc2 || test_end 1

test_end 0
