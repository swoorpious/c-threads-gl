# c-threads-gl

Practicing C, threading and OpenGL.

### What's in here

- `thread1.c` — basic pthreads
- `producer_consumer.c` — producer/consumer pattern with pthreads  
- `four_shaders.c` — offscreen OpenGL rendering across 4 windows with pthreads, each running a different shader

### Dependencies

- GLFW
- glad
- pthreads

### Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```
