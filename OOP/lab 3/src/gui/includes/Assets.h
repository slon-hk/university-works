#ifndef OOP_PROJECT_ASSETS_H
#define OOP_PROJECT_ASSETS_H

#include "SDL.h"

#include <map>
#include <string>

// Загрузчик и кеш текстур. Каждая картинка грузится с диска один раз,
// все текстуры освобождаются в деструкторе.
class Assets {
private:
	SDL_Renderer* renderer_;
	std::string root_; // папка с ассетами, относительно которой задаются пути
	std::map<std::string, SDL_Texture*> textures_;

public:
	Assets(SDL_Renderer* renderer, const std::string& root);
	~Assets();

	// текстура по относительному пути; nullptr, если файл не загрузился
	SDL_Texture* get(const std::string& path);
	// число кадров в горизонтальной ленте анимации из квадратных кадров
	int get_frame_count(SDL_Texture* texture) const;
};

#endif //OOP_PROJECT_ASSETS_H
