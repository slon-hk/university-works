#include "includes/Assets.h"

#include <iostream>

using namespace std;

Assets::Assets(SDL_Renderer* renderer, const string& root) : renderer_(renderer), root_(root) {
}

Assets::~Assets() {
	for (map<string, SDL_Texture*>::iterator it = textures_.begin(); it != textures_.end(); ++it) {
		if (it->second != nullptr) {
			SDL_DestroyTexture(it->second);
		}
	}
}

SDL_Texture* Assets::get(const string& path) {
	map<string, SDL_Texture*>::iterator found = textures_.find(path);
	if (found != textures_.end()) {
		return found->second;
	}

	SDL_Texture* texture = nullptr;
	string full_path = root_ + "/" + path;
	SDL_Surface* surface = SDL_LoadPNG(full_path.c_str());
	if (surface == nullptr) {
		cout << "Не удалось загрузить " << full_path << ": " << SDL_GetError() << endl;
	} else {
		texture = SDL_CreateTextureFromSurface(renderer_, surface);
		SDL_DestroySurface(surface);
	}

	// кешируем даже nullptr, чтобы не пытаться загружать битый файл каждый кадр
	textures_[path] = texture;
	return texture;
}

int Assets::get_frame_count(SDL_Texture* texture) const {
	if (texture == nullptr) {
		return 1;
	}
	float width = 0;
	float height = 0;
	SDL_GetTextureSize(texture, &width, &height);
	if (height <= 0) {
		return 1;
	}
	// кадры квадратные и идут в ряд, поэтому их число = ширина / высота
	int count = (int) (width / height);
	return count > 0 ? count : 1;
}
