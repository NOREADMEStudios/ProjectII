#ifndef __LABEL_H__
#define __LABEL_H__

#include "InterfaceElement.h"
#include "SDL_ttf\include\SDL_ttf.h"
#include "SDL\include\SDL_pixels.h"

typedef _TTF_Font Font;

class Label :
	public InterfaceElement
{
public:
	enum Alignment {
		CENTERED,
		LEFT,
		RIGHT
	};

	enum RenderMode {
		SOLID,
		BLENDED,
		SHADED
	};

	enum FontColor {
		COLOR_TRANSPARENT = -1,
		COLOR_WHITE,
		COLOR_BLACK,
		COLOR_GREY,
		COLOR_RED,
		COLOR_BLUE,
		COLOR_YELLOW,
		COLOR_GREEN,
		COLOR_BROWN,
		COLOR_ORANGE,
		COLOR_CYAN,
		COLOR_PURPLE,
		COLOR_PINK
	};

	Label();
	Label(int x, int y, const char* font_path, int pSize, RenderMode mode);
	~Label();

	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	bool RenderFont();

	void setAlignment(Alignment alignment);
	Alignment getAlignment() const;
	void setColor(SDL_Color fg, SDL_Color bg = { 0,0,0,0 });
	void getColor(SDL_Color* fg, SDL_Color* bg = nullptr);
	void setString(const char* string, ...);
	void setString(p2SString string);
	void getString(const char* string) const;
	void getString(p2SString& string) const;
	void setFont(Font* font); // Use at your own risk, you are accountable for the memory management of the font
	void setFont(const char* font_path, int pSize = -1); // Recommended since it uses the fonts module and doesn't require external memory management
	Font* getFont();
	void setSize(int pSize);
	int getSize() const;
	void setRenderMode(RenderMode mode);
	RenderMode getRenderMode();

protected:
	Font* font = nullptr;
	SDL_Color color_fg, color_bg;
	p2SString string;
	Alignment alignment;
	bool text_changed = false, font_changed = false;
	RenderMode render_mode = SOLID;
	const char* path = nullptr;
	int psize = 0;
};

#endif