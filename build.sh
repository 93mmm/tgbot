cmake -B build -S .
cmake --build build && mv build/todo_bot environment && cd environment && ./todo_bot
