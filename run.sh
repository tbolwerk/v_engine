mkdir -p build
cmake -S . -B build
make -C build
cd build && ./v_engine
