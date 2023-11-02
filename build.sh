cmake -B build -S .
result=$(cmake --build build) && mv build/todo_bot . && ./build/todo_bot
