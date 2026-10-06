all: build_source

generate:
	cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON --preset debug

copy_compile_commands:
	cp build/compile_commands.json .

build_source:
	cmake --build ./build -j9
