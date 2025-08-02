#include "../include/utils.h"


Mix_Chunk *loadSound(const char *path) {
  Mix_Chunk *sound = Mix_LoadWAV(path);
  if (sound == NULL) {
    printf("Erro ao carregar som: %s\n", Mix_GetError());
  }
  return sound;
}


