source "${TEST_LIB}/funcs.bash"

fd_check='
{
    if (/==[0-9]*==\s*Open file descriptor/) {
        fd=$0;
        getline;
        if (! /inherited/) {
            print fd
        }
    }
}'

test_start "Memory Leak Check"

if ! ( which valgrind &> /dev/null ); then
    # valgrind is not installed.
    # (Install with `pacman -Sy valgrind`)
    test_end 1
fi

leak_output=$(valgrind \
    --trace-children=no \
    --child-silent-after-fork=yes \
    --leak-check=full \
    --track-fds=yes \
    --track-origins=yes \
    ./inspector -o -p "${TEST_DIR}/inputs/fakeproc" 2>&1)

echo "${leak_output}"

# Check for open FDs
awk "${fd_check}" <<< "${leak_output}" \
    | grep -i '==[0-9]*==.*file descriptor' && test_end 1

# Make sure no leaks are possible
grep -i '==[0-9]*==.*no leaks are possible' \
    <<< "${leak_output}" || test_end 1

# If none of the conditions were triggered above, the test passes.
test_end 0
