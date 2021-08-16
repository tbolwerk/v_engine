mkdir -p build
cmake -S . -B build
make -C build
sh ./build/v_engine
