#include "Label.h"
#include "j1Fonts.h"
#include "SDL\include\SDL.h"


Label::Label() : InterfaceElement()
{
}

Label::Label(int x, int y, const char* font_path, int pSize, RenderMode mode) : InterfaceElement()
{
	rel_pos.x = x;
	rel_pos.y = y;
	rect.w = 0;
	rect.h = 0;
	render_mode = mode;
	psize = pSize;
	path = font_path;
	type = LABEL;
	Font* aux = App->font->Load(font_path, pSize);
	if (aux != nullptr)
		setFont(aux);
	else LOG("Could not load font: %s", font_path);
}

Label::~Label()
{
}


bool Label::Start()
{
	return true;
}

bool Label::PreUpdate()
{
	bool ret = InterfaceElement::PreUpdate();

	if (font != nullptr && text_changed && ret) {
		if (font_changed) {
			App->font->Unload(font);
			font = nullptr;
			font = App->font->Load(path, psize);
			if (font == nullptr) {
				ret = false;
				LOG("Error changing font: %s", TTF_GetError());
			}
			font_changed = false;
		}

		ret = RenderFont();
		text_changed = false;
	}

	return ret;
}

bool Label::Update(float dt)
{
	if (font != nullptr) {
		int d_x = 0, d_y = 0;
		/*switch (alignment)
		{
		case Label::CENTERED:
			d_x = -rect.w / 2;
			d_y = -rect.h / 2;
			break;
		case Label::LEFT:
			break;
		case Label::RIGHT:
			d_x = -rect.w;
			d_y = -rect.h;
			break;
		default:
			break;
		}*/

		int dx = 0, dy = 0, dw = 0, dh = 0;

		SDL_Rect actual_anim_rect;

		if (parent != nullptr) {
			dx = result_rect.x - rect.x;
			dy = result_rect.y - rect.y;
			dw = result_rect.w;
			dh = result_rect.h;

			actual_anim_rect.w = dw;
			actual_anim_rect.h = dh;
			actual_anim_rect.x = dx;
			actual_anim_rect.y = dy;
		}
		else
		{
			actual_anim_rect = rect;
			actual_anim_rect.x = 0;
			actual_anim_rect.y = 0;
		}

		App->render->BlitGui(tex, result_rect.x + d_x, result_rect.y + d_y, &actual_anim_rect);
	}

	bool ret = InterfaceElement::Update(dt);
	return ret;
}

bool Label::CleanUp()
{
	if (tex != nullptr) {
		SDL_DestroyTexture(tex);
		tex = nullptr;
	}
	App->font->Unload(font);

	InterfaceElement::CleanUp();
	return true;
}

bool Label::RenderFont()
{
	bool ret = true;
	if (tex != nullptr) {
		SDL_DestroyTexture(tex);
		tex = nullptr;
	}

	tex = App->font->Print(string.GetString(), color_fg, font, render_mode, color_bg);

	if (tex == nullptr)
		ret = false;
	else
		SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);

	result_rect = content_rect = rect;

	return ret;
}

void Label::setAlignment(Label::Alignment alignment)
{
	this->alignment = alignment;
}

Label::Alignment Label::getAlignment() const
{
	return alignment;
}

void Label::setColor(SDL_Color fg, SDL_Color bg)
{
	/*switch (fg)
	{
	case Label::COLOR_WHITE:
		color_fg = { 255, 255, 255, 255 };
		break;
	case Label::COLOR_BLACK:
		color_fg = { 0, 0, 0, 255 };
		break;
	case Label::COLOR_GREY:
		color_fg = { 128, 128, 128, 255 };
		break;
	case Label::COLOR_RED:
		color_fg = { 255, 0, 0, 255 };
		break;
	case Label::COLOR_BLUE:
		color_fg = { 0, 0, 255, 255 };
		break;
	case Label::COLOR_YELLOW:
		color_fg = { 0, 255, 255, 255 };
		break;
	case Label::COLOR_GREEN:
		color_fg = { 0, 255, 0, 255 };
		break;
	case Label::COLOR_BROWN:
		color_fg = { 102, 51, 0, 255 };
		break;
	case Label::COLOR_ORANGE:
		color_fg = { 255, 128, 0, 255 };
		break;
	case Label::COLOR_CYAN:
		color_fg = { 0, 255, 255, 255 };
		break;
	case Label::COLOR_PURPLE:
		color_fg = { 128, 0, 255, 255 };
		break;
	case Label::COLOR_PINK:
		color_fg = { 0, 255, 128, 255 };
		break;
	default:
		break;
	}*/
	color_fg = fg;
	color_bg = bg;
	text_changed = true;
}

void Label::getColor(SDL_Color * fg, SDL_Color * bg)
{
	if (fg != nullptr)
		fg = &color_fg;
	if (bg != nullptr)
		bg = &color_bg;
}

void Label::setString(const char* format, ...)
{
	if (format != NULL)
	{
		static va_list  ap;
		static char tmp[TMP_STRING_SIZE];

		va_start(ap, format);
		int res = vsnprintf_s(tmp, TMP_STRING_SIZE, format, ap);
		va_end(ap);

		if (res > 0)
			string.create(tmp);

		text_changed = true;
	}
}

void Label::setString(p2SString string)
{
	this->string = string;
	text_changed = true;
}

void Label::getString(const char* string) const
{
	string = this->string.GetString();
}

void Label::getString(p2SString& string) const
{
	string = this->string;
}

void Label::setFont(Font* font)
{
	App->font->Unload(this->font);
	this->font = font;
	text_changed = true;
}

void Label::setFont(const char* font_path, int pSize)
{
	path = font_path;
	if (pSize > -1)
		psize = pSize;
	text_changed = true;
	font_changed = true;
}

Font* Label::getFont()
{
	return font;
}

void Label::setSize(int pSize)
{
	psize = pSize;
	text_changed = true;
	font_changed = true;
}

int Label::getSize() const
{
	return psize;
}

void Label::setRenderMode(RenderMode mode)
{
	render_mode = mode;
}

Label::RenderMode Label::getRenderMode()
{
	return render_mode;
}
