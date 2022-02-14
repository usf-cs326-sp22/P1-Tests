source "${TEST_LIB}/funcs.bash"

test_start "README Check" \
    "Ensures the readme has been filled out."

grep 'TODO' "${TEST_DIR}/../README.md" || test_end 0

test_end 1 # If the grep above found any TODOs, the test case fails.
