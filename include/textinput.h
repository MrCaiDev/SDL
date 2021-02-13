#ifndef textinput_h_
#define textinput_h_

#include <string>
#include <SDL.h>

//Importable text wrapper class
class TextInput
{
private:
	bool changetext;
public:
	std::string text;
	TextInput();
	~TextInput();
	void HandleEvent(SDL_Event event);
	std::string GetContent();
	bool Changed();
	void ResetChange();
	int Length();
};

/*
 * \brief Create an importable text.
 */
TextInput::TextInput()
{
	text = " ";
	changetext = false;
}

/*
 * \brief Deallocate an importable text.
 */
TextInput::~TextInput()
{
	text = " ";
	changetext = false;
}

/*
 * \brief Change the content of text according to events.
 * \param event If not NULL, the next event is removed from the queue and stored in that area.
 */
void TextInput::HandleEvent(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_KEYDOWN:
			//Handle deleting.
			if (event.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
			{
				text.pop_back();
				changetext = true;
			}
			//Handle copying.
			else if (event.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL))
				SDL_SetClipboardText(text.c_str());
			//Handle pasting.
			else if (event.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL))
			{
				text += SDL_GetClipboardText();
				changetext = true;
			}
			break;

		case SDL_TEXTINPUT:
			//Handle characters input.
			if (!((SDL_GetModState() & KMOD_CTRL) && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V')))
			{
				text += event.text.text;
				changetext = true;
			}
			break;
		default:
			break;
	}
}

/*
 * \brief Get the content of text.
 * \return The content of text.
 */
inline std::string TextInput::GetContent()
{
	return text;
}

/*
 * \brief Determine if the content has been changed.
 * \return 1 if changed, or 0 if unchanged.
 */
inline bool TextInput::Changed()
{
	return changetext;
}

/*
 * \brief View the current text as unchanged.
 */
inline void TextInput::ResetChange()
{
	changetext = false;
}

/*
 * \brief Get the length of text.
 * \return The length of text.
 */
inline int TextInput::Length()
{
	return (int)text.length();
}


#endif // !textinput_h_