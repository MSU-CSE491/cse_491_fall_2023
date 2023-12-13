emcc factorial.cpp -s WASM=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" -o factorial.js
