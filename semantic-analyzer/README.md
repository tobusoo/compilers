# COOL semantic analyzer

## Build
```bash
cmake -B build -S . -G Ninja
cmake --build build
```

## Run
```bash
./build/semantic-analyzer cool-examples/arithmetic1.cl
```

## TODO
- [ ] Refactoring the project and configuring cmake
- [ ] Semantic analysis of all transferred files, not just the last one