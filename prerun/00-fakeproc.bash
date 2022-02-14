
if [[ ! -e "${TEST_DIR}/inputs/fakeproc" ]]; then
    echo "Extracting test proc file system..."
    tar -C "${TEST_DIR}/inputs" -xf "${TEST_DIR}/inputs/fakeproc.tar.gz"
fi

