
# Build library
git submodule update --recursive --init

cmake -S . -B build
cmake --build build

# Build Examples

You will need to change this line to the name of an example.cpp

```
add_executable(SOGLR_Examples [example-name.cpp])
```

```shell
cd examples/
cmake -S . -B build
cmake --build build
./build/SOGLR_Examples
```