// Conway's Game of Life - GPU Accelerated Implementation
// - Bitfield storage
// - GPU parallel computation with CUDA
// - GPU rendering

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <SFML/Graphics.hpp>
#include <cuda_gl_interop.h>

struct Config {
  int width = 256;
  int height = 256;
  int scale = 3;
  bool showFps = false;
};

void parseArgs(int argc, char** argv, Config& config) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-w" || arg == "--width") {
      if (i + 1 < argc) {
        config.width = std::atoi(argv[++i]);
      }
    }
    else if (arg == "-h" || arg == "--height") {
      if (i + 1 < argc) {
        config.height = std::atoi(argv[++i]);
      }
    }
    else if (arg == "-s" || arg == "--scale") {
      if (i + 1 < argc) {
        config.scale = std::atoi(argv[++i]);
      }
    }
    else if (arg == "--fps") {
      config.showFps = true;
    }
  }
}

#define CUDA_CHECK(call) { \
    cudaError_t err = call; \
    if (err != cudaSuccess) { \
        std::cerr << "CUDA error in " << __FILE__ << " at line " << __LINE__ << ": " \
                  << cudaGetErrorString(err) << std::endl; \
        exit(EXIT_FAILURE); \
    } \
}

bool checkGPUCapabilities() {
  int deviceCount = 0;
  CUDA_CHECK(cudaGetDeviceCount(&deviceCount));

  if (deviceCount == 0) {
    std::cerr << "No CUDA-capable GPU found." << std::endl;
    return false;
  }

  cudaDeviceProp deviceProp;
  CUDA_CHECK(cudaGetDeviceProperties(&deviceProp, 0));

  std::cout << "GPU Information:" << std::endl;
  std::cout << "  Device Name: " << deviceProp.name << std::endl;
  std::cout << "  Compute Capability: " << deviceProp.major << "." << deviceProp.minor << std::endl;
  std::cout << "  Total Global Memory: " << deviceProp.totalGlobalMem / (1024 * 1024) << " MB" << std::endl;
  std::cout << "  Multiprocessors: " << deviceProp.multiProcessorCount << std::endl;
  std::cout << "  Max Threads Per Block: " << deviceProp.maxThreadsPerBlock << std::endl;

  return true;
}

const int BLOCK_SIZE = 16;  // size of thread block (16x16 = 256 threads)
const int BITS_PER_ELEMENT = 32;  // 32-bit integers for storage

void calculateStorageRequirements(int width, int height, int& elementsPerRow, size_t& totalMemory) {
  elementsPerRow = (width + BITS_PER_ELEMENT - 1) / BITS_PER_ELEMENT;

  totalMemory = elementsPerRow * height * sizeof(unsigned int);

  std::cout << "Grid Storage Requirements:" << std::endl;
  std::cout << "  Grid Dimensions: " << width << " x " << height << std::endl;
  std::cout << "  Elements per Row: " << elementsPerRow << std::endl;
  std::cout << "  Total Memory: " << totalMemory << " bytes" << std::endl;
  std::cout << "  Memory Efficiency: " << (float)(width * height) / (float)(elementsPerRow * height * BITS_PER_ELEMENT) * 100.0f << "%" << std::endl;
}

__global__ void initializeRandom(unsigned int* state, int elementsPerRow, int width, int height, unsigned int seed) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x < elementsPerRow && y < height) {
    unsigned int value = 0;
    for (int bit = 0; bit < BITS_PER_ELEMENT; bit++) {
      int actualX = x * BITS_PER_ELEMENT + bit;
      if (actualX < width) {
        unsigned int hash = seed + actualX * 374761393 + y * 668265263;
        hash = (hash ^ (hash >> 13)) * 1274126177;
        hash = hash ^ (hash >> 16);

        if (hash & 1) {
          value |= (1 << bit);
        }
      }
    }

    state[y * elementsPerRow + x] = value;
  }
}

__global__ void clearGrid(unsigned int* state, int elementsPerRow, int height) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x < elementsPerRow && y < height) {
    state[y * elementsPerRow + x] = 0;
  }
}

__global__ void toggleCell(unsigned int* state, int elementsPerRow, int cellX, int cellY) {
  int elementX = cellX / BITS_PER_ELEMENT;
  int bitOffset = cellX % BITS_PER_ELEMENT;

  unsigned int mask = 1 << bitOffset;
  atomicXor(&state[cellY * elementsPerRow + elementX], mask);
}

__device__ int getCellState(const unsigned int* state, int elementsPerRow, int x, int y, int width, int height) {
  if (x < 0) x += width;
  if (y < 0) y += height;
  if (x >= width) x -= width;
  if (y >= height) y -= height;

  int elementX = x / BITS_PER_ELEMENT;
  int bitOffset = x % BITS_PER_ELEMENT;

  return (state[y * elementsPerRow + elementX] >> bitOffset) & 1;
}

__global__ void gameOfLifeStep(const unsigned int* currentState, unsigned int* nextState,
  int elementsPerRow, int width, int height) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x < width && y < height) {
    int neighbors = 0;
    neighbors += getCellState(currentState, elementsPerRow, x - 1, y - 1, width, height);
    neighbors += getCellState(currentState, elementsPerRow, x, y - 1, width, height);
    neighbors += getCellState(currentState, elementsPerRow, x + 1, y - 1, width, height);
    neighbors += getCellState(currentState, elementsPerRow, x - 1, y, width, height);
    neighbors += getCellState(currentState, elementsPerRow, x + 1, y, width, height);
    neighbors += getCellState(currentState, elementsPerRow, x - 1, y + 1, width, height);
    neighbors += getCellState(currentState, elementsPerRow, x, y + 1, width, height);
    neighbors += getCellState(currentState, elementsPerRow, x + 1, y + 1, width, height);

    int currentCellState = getCellState(currentState, elementsPerRow, x, y, width, height);

    int newState = 0;
    if (currentCellState == 1) {
      newState = (neighbors == 2 || neighbors == 3) ? 1 : 0;
    }
    else {
      newState = (neighbors == 3) ? 1 : 0;
    }

    int elementX = x / BITS_PER_ELEMENT;
    int bitOffset = x % BITS_PER_ELEMENT;

    unsigned int mask = 1 << bitOffset;
    if (newState) {
      atomicOr(&nextState[y * elementsPerRow + elementX], mask);
    }
    else {
      atomicAnd(&nextState[y * elementsPerRow + elementX], ~mask);
    }
  }
}

__global__ void renderGrid(const unsigned int* state, unsigned char* pixels,
  int elementsPerRow, int width, int height, int scale) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x < width && y < height) {
    int cellState = getCellState(state, elementsPerRow, x, y, width, height);

    unsigned char color = cellState ? 0 : 255;

    for (int dy = 0; dy < scale; dy++) {
      for (int dx = 0; dx < scale; dx++) {
        int pixelX = x * scale + dx;
        int pixelY = y * scale + dy;

        if (pixelX < width * scale && pixelY < height * scale) {
          int pixelPos = (pixelY * width * scale + pixelX) * 4;  // 4 bytes per pixel (RGBA)
          pixels[pixelPos] = color;     // R
          pixels[pixelPos + 1] = color; // G
          pixels[pixelPos + 2] = color; // B
          pixels[pixelPos + 3] = 255;   // A (always fully opaque)
        }
      }
    }
  }
}

int main(int argc, char** argv) {
  Config config;
  parseArgs(argc, argv, config);

  if (!checkGPUCapabilities()) {
    return EXIT_FAILURE;
  }

  int elementsPerRow = 0;
  size_t totalMemory = 0;
  calculateStorageRequirements(config.width, config.height, elementsPerRow, totalMemory);

  unsigned int* d_currentState = nullptr;
  unsigned int* d_nextState = nullptr;
  CUDA_CHECK(cudaMalloc(&d_currentState, totalMemory));
  CUDA_CHECK(cudaMalloc(&d_nextState, totalMemory));

  dim3 blockSize(BLOCK_SIZE, BLOCK_SIZE);
  dim3 gridSize((elementsPerRow + blockSize.x - 1) / blockSize.x,
    (config.height + blockSize.y - 1) / blockSize.y);

  unsigned int seed = static_cast<unsigned int>(time(nullptr));
  initializeRandom << <gridSize, blockSize >> > (d_currentState, elementsPerRow, config.width, config.height, seed);
  CUDA_CHECK(cudaGetLastError());

  clearGrid << <gridSize, blockSize >> > (d_nextState, elementsPerRow, config.height);
  CUDA_CHECK(cudaGetLastError());

  sf::RenderWindow window(sf::VideoMode(config.width * config.scale, config.height * config.scale),
    "Conway's Game of Life - GPU Accelerated");
  window.setFramerateLimit(60);

  sf::Texture texture;
  if (!texture.create(config.width * config.scale, config.height * config.scale)) {
    std::cerr << "Failed to create texture!" << std::endl;
    return EXIT_FAILURE;
  }
  sf::Sprite sprite(texture);

  size_t pixelDataSize = config.width * config.height * config.scale * config.scale * 4;  // RGBA
  unsigned char* d_pixels = nullptr;
  CUDA_CHECK(cudaMalloc(&d_pixels, pixelDataSize));

  // host buffer for pixel data
  std::vector<sf::Uint8> pixels(pixelDataSize);

  dim3 renderBlockSize(BLOCK_SIZE, BLOCK_SIZE);
  dim3 renderGridSize((config.width + renderBlockSize.x - 1) / renderBlockSize.x,
    (config.height + renderBlockSize.y - 1) / renderBlockSize.y);

  // simulation control
  bool paused = true;
  sf::Clock clock;
  sf::Clock fpsClock;
  int frameCount = 0;

  std::cout << "Controls:" << std::endl;
  std::cout << "  Left Mouse Button: Toggle cell state" << std::endl;
  std::cout << "  Space: Pause/Resume simulation" << std::endl;
  std::cout << "  R: Randomize grid" << std::endl;
  std::cout << "  C: Clear grid" << std::endl;
  std::cout << "  Escape: Exit" << std::endl;

  // main loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
        else if (event.key.code == sf::Keyboard::Space) {
          paused = !paused;
        }
        else if (event.key.code == sf::Keyboard::R) {
          seed = static_cast<unsigned int>(time(nullptr));
          initializeRandom << <gridSize, blockSize >> > (d_currentState, elementsPerRow, config.width, config.height, seed);
          CUDA_CHECK(cudaGetLastError());
        }
        else if (event.key.code == sf::Keyboard::C) {
          clearGrid << <gridSize, blockSize >> > (d_currentState, elementsPerRow, config.height);
          CUDA_CHECK(cudaGetLastError());
        }
      }
      else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          int cellX = event.mouseButton.x / config.scale;
          int cellY = event.mouseButton.y / config.scale;

          if (cellX >= 0 && cellX < config.width && cellY >= 0 && cellY < config.height) {
            toggleCell << <1, 1 >> > (d_currentState, elementsPerRow, cellX, cellY);
            CUDA_CHECK(cudaGetLastError());
          }
        }
      }
    }

    if (!paused) {
      gameOfLifeStep << <renderGridSize, renderBlockSize >> > (
        d_currentState, d_nextState, elementsPerRow, config.width, config.height);
      CUDA_CHECK(cudaGetLastError());

      std::swap(d_currentState, d_nextState);

      clearGrid << <gridSize, blockSize >> > (d_nextState, elementsPerRow, config.height);
      CUDA_CHECK(cudaGetLastError());
    }

    renderGrid << <renderGridSize, renderBlockSize >> > (
      d_currentState, d_pixels, elementsPerRow, config.width, config.height, config.scale);
    CUDA_CHECK(cudaGetLastError());

    CUDA_CHECK(cudaMemcpy(pixels.data(), d_pixels, pixelDataSize, cudaMemcpyDeviceToHost));

    texture.update(pixels.data());
    window.clear();
    window.draw(sprite);

    if (config.showFps) {
      frameCount++;
      if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        float fps = frameCount / fpsClock.getElapsedTime().asSeconds();
        window.setTitle("Conway's Game of Life - GPU Accelerated (FPS: " + std::to_string(static_cast<int>(fps)) + ")");
        frameCount = 0;
        fpsClock.restart();
      }
    }

    window.display();
  }

  CUDA_CHECK(cudaFree(d_currentState));
  CUDA_CHECK(cudaFree(d_nextState));
  CUDA_CHECK(cudaFree(d_pixels));

  return EXIT_SUCCESS;
}