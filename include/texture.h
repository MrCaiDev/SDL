#ifndef texture_h_
#define texture_h_

#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <collision.h>
#include <error.h>

//Texture wrapper class
class Texture
{
protected:
	SDL_Renderer* rend;
	SDL_Texture* texture;
	int w, h;
public:
	Texture();
	~Texture();
	void CreateFromImage(SDL_Renderer* renderer, const char* file);
	void CreateFromImage(SDL_Renderer* renderer, const char* file, SDL_Color color);
	void CreateFromText(SDL_Renderer* renderer, std::string message, const char* file, SDL_Color color, int size);
	void SetColor(Uint8 r, Uint8 g, Uint8 b);
	void SetBlend(SDL_BlendMode blendmode);
	void SetAlpha(Uint8 alpha);
	std::vector<SDL_Rect> Cut(int m, int n);
	void Clear(SDL_Point point, SDL_Rect* clip = NULL);
	void RenderEx(SDL_Point point, double angle, SDL_Point center, SDL_RendererFlip flip, SDL_Rect* clip = NULL);
	void RenderStretched(SDL_Rect viewport, SDL_Rect* clip = NULL);
	int GetWidth();
	int GetHeight();
	void free();
};

/*
 * \brief Create an empty texture.
 */
Texture::Texture()
{
	rend = NULL;
	texture = NULL;
	w = 0;
	h = 0;
}

/*
 * \brief Deallocate the texture.
 */
Texture::~Texture()
{
	free();
}

/*
 * \brief Load an image directly into a texture.
 * \param renderer The renderer which should copy parts of a texture.
 * \param file The path of the source image.
 */
void Texture::CreateFromImage(SDL_Renderer* renderer, const char* file)
{
	free();
	rend = renderer;
	//Create the texture.
	texture = IMG_LoadTexture(rend, file);
	if (texture == NULL)
		SDL_ReportError("IMG_LoadTexture");
	else
		//Get the width and height of the texture.
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

/*
 * \brief Load an image into a texture, and make a color transparent.
 * \param renderer The renderer which should copy parts of a texture.
 * \param file_image The path of the source image.
 * \param color The color to be made transparent.
 */
void Texture::CreateFromImage(SDL_Renderer* renderer, const char* file, SDL_Color color)
{
	free();
	rend = renderer;
	//Load the image into a surface.
	SDL_Surface* surface = IMG_Load(file);
	if (surface == NULL)
		SDL_ReportError("IMG_Load");
	else
	{
		//Make the target color transparent.
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, color.r, color.g, color.b));
		//Create the texture from surface.
		texture = SDL_CreateTextureFromSurface(rend, surface);
		//Free the surface.
		SDL_FreeSurface(surface);
		surface = NULL;
		if (texture == NULL)
			SDL_ReportError("SDL_CreateTextureFromSurface");
		else
			//Get the width and height of the texture.
			SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	}
}

/*
 * \brief Create a texture from a string of message.
 * \param renderer The renderer which should copy parts of a texture.
 * \param message The string of source message.
 * \param file The path of the source font.
 * \param color The color of text.
 * \param size The size of text.
 */
void Texture::CreateFromText(SDL_Renderer* renderer, std::string message, const char* file, SDL_Color color, int size)
{
	free();
	rend = renderer;
	//Open the file of font.
	TTF_Font* font = TTF_OpenFont(file, size);
	if (font == NULL)
		SDL_ReportError("TTF_OpenFont");
	else
	{
		//Load the message into a surface.
		SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), color);
		//Close the file of font.
		TTF_CloseFont(font);
		if (surface == NULL)
			SDL_ReportError("TTF_RenderText");
		else
		{
			//Create the texture from surface.
			texture = SDL_CreateTextureFromSurface(rend, surface);
			//Free the surface.
			SDL_FreeSurface(surface);
			surface = NULL;
			if (texture == NULL)
				SDL_ReportError("SDL_CreateTextureFromSurface");
			else
				//Get the width and height of the texture.
				SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		}
	}
}

/*
 * \brief Set an additional color value used in render copy operations.
 * \param r The red color value multiplied into copy operations.
 * \param g The green color value multiplied into copy operations.
 * \param b The blue color value multiplied into copy operations.
 */
void Texture::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	if (SDL_SetTextureColorMod(texture, r, g, b) != 0)
		SDL_ReportError("SDL_SetTextureColorMod");
}

/*
 * \brief Set the blend mode of the texture.
 * \param blendmode The blend mode to use for texture blending.
 */
void Texture::SetBlend(SDL_BlendMode blendmode)
{
	if (SDL_SetTextureBlendMode(texture, blendmode) != 0)
		SDL_ReportError("SDL_SetTextureBlendMode");
}

/*
 * \brief Set the transparency of the texture.
 * \param alpha The alpha value multiplied into copy operations.
 */
void Texture::SetAlpha(Uint8 alpha)
{
	if (SDL_SetTextureAlphaMod(texture, alpha) != 0)
		SDL_ReportError("SDL_SetTextureAlphaMod");
}

/*
 * \brief Cut the texture into (m) rows and (n) columns.
 * \param m The target row number. (Y axis)
 * \param n The target column number. (X axis)
 * \return a vector containing rectangles showing the position and size of each clip.
 */
std::vector<SDL_Rect> Texture::Cut(int m, int n)
{
	//Get the width and height of a single clip.
	int single_w = w / n, single_h = h / m;
	//Start cutting.
	int number = m * n;
	int op = 0; //The serial number of the operating clip.
	std::vector<SDL_Rect> clips(number);
	for (int row = 0; row < m; row++)
	{
		for (int column = 0; column < n; column++)
		{
			op = row * n + column;
			clips[op].x = column * single_w;
			clips[op].y = row * single_h;
			clips[op].w = single_w;
			clips[op].h = single_h;
		}
	}
	return clips;
}

/*
 * \brief Copy a portion of the texture to the renderer.
 * \param point The destination coordinate to copy the texture.
 * \param clip A pointer to the portion of source texture, or NULL for the entire texture.
 */
void Texture::Clear(SDL_Point point, SDL_Rect* clip)
{
	SDL_Rect viewport = { point.x,point.y,w,h };
	if (clip != NULL)
	{
		viewport.w = clip->w;
		viewport.h = clip->h;
	}
	SDL_RenderCopy(rend, texture, clip, &viewport);
}

/*
 * \brief Copy a portion of the texture to the renderer, while rotating or flipping the portion at the same time.
 * \param point The destination coordinate to copy the texture.
 * \param angle An angle in degrees that indicates the rotation that will be applied to viewport, rotating it in a clockwise direction.
 * \param center The rotating center.
 * \param flip A way in which flipping actions should be performed on the texture.
 * \param clip A pointer to the portion of source texture, or NULL for the entire texture.
 */
void Texture::RenderEx(SDL_Point point, double angle, SDL_Point center, SDL_RendererFlip flip, SDL_Rect* clip)
{
	SDL_Rect viewport = { point.x,point.y,w,h };
	if (clip != NULL)
	{
		viewport.w = clip->w;
		viewport.h = clip->h;
	}
	SDL_RenderCopyEx(rend, texture, clip, &viewport, angle, &center, flip);
}

/*
 * \brief Copy a stretched portion of the texture to the renderer.
 * \param viewport The destination coordinate and size to copy the texture.
 * \param clip A pointer to the portion of source texture, or NULL for the entire texture.
 */
void Texture::RenderStretched(SDL_Rect viewport, SDL_Rect* clip)
{
	SDL_RenderCopy(rend, texture, clip, &viewport);
}

/*
 * \brief Get the width of a texture.
 * \return The width of a texture.
 */
inline int Texture::GetWidth()
{
	return w;
}

/*
 * \brief Get the height of a texture.
 * \return The height of a texture.
 */
inline int Texture::GetHeight()
{
	return h;
}

/*
 * \brief Deallocate the texture.
 */
void Texture::free()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		rend = NULL;
		w = 0;
		h = 0;
	}
}

//Texture derived class
class MovableTexture:public Texture
{
private:
	int x, y;
	SDL_Rect range;
	std::vector<SDL_Rect> boxes;
	std::vector<SDL_Point> delta;
	int velocity_x, velocity_y;
	void MoveBoxes();
public:
	MovableTexture();
	~MovableTexture();
	void CreateFromTexture(Texture texture, SDL_Point point, SDL_Rect range, std::vector<SDL_Rect> boxes);
	void HandleEvent(SDL_Event event);
	void Move();
	void CameraFollow(SDL_Rect& Camera);
	void Show();
	void Show(SDL_Rect& camera);
};

/*
 * \brief Create an empty movable texture.
 */
MovableTexture::MovableTexture():Texture()
{
	x = 0;
	y = 0;
	velocity_x = 0;
	velocity_y = 0;
}

/*
 * \brief Deallocate a movable texture.
 */
MovableTexture::~MovableTexture()
{
	x = 0;
	y = 0;
	range = { 0,0,0,0 };
	std::vector<SDL_Rect>().swap(boxes);
	std::vector<SDL_Point>().swap(delta);
	velocity_x = 0;
	velocity_y = 0;
}

/*
 * \brief Create a movable texture.
 * \param texture The source texture.
 * \param point The destination coordinate to copy the texture the first time.
 * \param range The scope of activity of the texture.
 * \param boxes The collision boxes of the texture.
 */
void MovableTexture::CreateFromTexture(Texture texture, SDL_Point point, SDL_Rect range, std::vector<SDL_Rect> boxes)
{
	//Save the initial position.
	x = point.x;
	y = point.y;
	//Save the range and the movable collision boxes.
	this->range = range;
	for (int i = 0; i < boxes.size(); i++)
	{
		delta.push_back({ boxes[i].x,boxes[i].y });
		this->boxes.push_back({ point.x + boxes[i].x,point.y + boxes[i].y ,boxes[i].w,boxes[i].h });
	}
}

/*
 * \brief Move the collision boxes.
 */
void MovableTexture::MoveBoxes()
{
	for (int i = 0; i < boxes.size(); i++)
	{
		boxes[i].x = x + delta[i].x;
		boxes[i].y = y + delta[i].y;
	}
}

/*
 * \brief Handle movement events.
 * \param event If not NULL, the next event is removed from the queue and stored in that area.
 */
void MovableTexture::HandleEvent(SDL_Event event)
{
	int vx = w / 10;
	int vy = h / 10;
	//If a key was pressed, add to the velocity on the corresponding direction.
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
		switch (event.key.keysym.sym)
		{
			case SDLK_w:
				velocity_y -= vy;
				break;
			case SDLK_s:
				velocity_y += vy;
				break;
			case SDLK_a:
				velocity_x -= vx;
				break;
			case SDLK_d:
				velocity_x += vx;
				break;
		}
	//If a key was released, restore the velocity on the corresponding direction.
	else if (event.type == SDL_KEYUP && event.key.repeat == 0)
		switch (event.key.keysym.sym)
		{
			case SDLK_w:
				velocity_y += vy;
				break;
			case SDLK_s:
				velocity_y -= vy;
				break;
			case SDLK_a:
				velocity_x += vx;
				break;
			case SDLK_d:
				velocity_x -= vx;
				break;
		}
}

/*
 * \brief Move the texture according to its velocity.
 */
void MovableTexture::Move()
{
	//Move in X direction.
	x += velocity_x;
	MoveBoxes();
	if (InsideCollided(boxes, range))
	{
		x -= velocity_x;
		MoveBoxes();
	}
	//Move in Y direction.
	y += velocity_y;
	MoveBoxes();
	if (InsideCollided(boxes, range))
	{
		y -= velocity_y;
		MoveBoxes();
	}
}

/*
 * \brief Make a camera follow the moving texture, placing the texture at the center of camera.
 * \param camera The camera which should shoot the texture.
 */
void MovableTexture::CameraFollow(SDL_Rect& camera)
{
	//Move the camera in X direction.
	camera.x = x + w / 2 - camera.w / 2;
	if (camera.x < 0)
		camera.x = 0;
	else if (camera.x + camera.w > range.x + range.w)
		camera.x = range.x + range.w - camera.w;
	//Move the camera in Y direction.
	camera.y = y + h / 2 - camera.h / 2;
	if (camera.y < 0)
		camera.y = 0;
	else if (camera.y + camera.h > range.y + range.h)
		camera.y = range.y + range.h - camera.h;
}

/*
 * \brief Show the texture on a renderer.
 */
void MovableTexture::Show()
{
	Clear({ x,y });
}

/*
 * \brief Show the texture on a renderer in front of a camera.
 */
void MovableTexture::Show(SDL_Rect& camera)
{
	Clear({ x - camera.x,y - camera.y });
}


#endif // !texture_h_