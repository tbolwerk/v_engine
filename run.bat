@call "%VS140COMNTOOLS%VsDevCmd.bat"
if not exists build
(
	mkdir build
)
cmake -S . -B build
make -C build
START build\v_engine.exe