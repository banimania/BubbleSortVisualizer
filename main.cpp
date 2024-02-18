#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <tuple>
#include <string>

const int S_WIDTH = 1000, S_HEIGHT = 720;
int comparisons = 0, accesses = 0;
int current;
int current2;
SDL_Rect getRect(int* array, int arraySize, int element) {
  SDL_Rect rect;
  rect.h = -array[element] * 5;
  rect.w = 10;
  rect.y = S_HEIGHT;
  rect.x = element * 10;
  return rect;
}

void drawArray(int* array, int arraySize, SDL_Renderer* renderer) {  
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  SDL_Rect rect;
  for (int i = 0; i < arraySize; i++) {
    rect = getRect(array, arraySize, i);
    if (current == i || current2 == i) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    if (current == i || current2 == i) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  }
}

std::tuple<int, int> sortArray(int* array, int arraySize, int k, int l) {
  int temp;
  for (int i = k; i < arraySize; i++) {
    for (int j = l; j < arraySize; j++) {
      comparisons++;
      accesses += 2;
      current = i;
      current2 = j;
      if (array[j] < array[i]) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        accesses += 4;
        return std::make_tuple(i, j);
      }
    }
    return std::make_tuple(i + 1, i + 2);
  }
  return std::make_tuple(-1, -1);
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Visualization", 0, 0, S_WIDTH, S_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int arraySize = 100;
  int* array = new int[arraySize];
  for (int i = 0; i < arraySize; i++) {
    array[i] = i;
  }
  std::random_shuffle(array, array + arraySize);

  int k = 0, l = 1;
  bool quit = false;
  while (!quit) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT) {
      quit = true;
    } else if (e.type == SDL_KEYDOWN) {
      std::tuple<int, int> kl = sortArray(array, arraySize, k, l);
      k = std::get<0>(kl);
      l = std::get<1>(kl);
      if (k == -1) quit = true;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawArray(array, arraySize, renderer);

    SDL_RenderPresent(renderer);

  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
