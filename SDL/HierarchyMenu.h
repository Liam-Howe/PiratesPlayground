#pragma once
#include <SDL.h>
#include "Button.h"
#include <vector>
#include <map>

class HierarchyMenu
{
public:

	static HierarchyMenu * Instance();

	void Update();
	void Draw(SDL_Renderer * renderer);

	void CreateBackgroundTexture(SDL_Rect pos, SDL_Rect sourcePos, SDL_Texture* tex);

	void CreateMenuTitle(std::string path, std::string message, int fontSize, SDL_Color color, SDL_Rect pos, SDL_Renderer* renderer);

	void AddPrimaryButton(SDL_Rect pos, SDL_Rect sourcePos, int id, SDL_Texture * tex);

	void AddSecondaryButton(SDL_Texture * tex, SDL_Rect pos, SDL_Rect sourcePos, int id, int index);
		
	void ChangeSelectedButton(int dir);
	void SelectButton();
	void ReturnToPrimaryList();

	void SetButtonHighlight(SDL_Rect pos, SDL_Rect sourcePos, SDL_Texture * tex);

	void mouseDown();
	void mouseHighlight();

	void ClickShortcut(int shortcut);

	void ClearMenu();

	SDL_Point* getButtonClicked();
	SDL_Point getMousePoint();

private:
	HierarchyMenu();
	~HierarchyMenu();
	static HierarchyMenu *m_inst;

	// Background Variables
	SDL_Texture* m_backgroundTexture;
	SDL_Rect m_backgroundPos;
	SDL_Rect m_backgroundSource;

	// Menu Title
	SDL_Texture* m_menuTitle;
	SDL_Rect m_menuTitlePos;

	// Primary Buttons
	std::vector<Button*> m_primaryButtons;

	// Secondary Buttons
	std::vector<std::vector<Button*>> m_secondaryButtons;

	// Button Selecting
	bool m_btnClicked;
	int m_selectedPrimaryButton;
	int m_selectedSecondaryButton;
	bool m_viewingSecondary;
	SDL_Texture* m_cursor;
	SDL_Rect m_cursorPos;
	SDL_Rect m_cursorSource;

	// Mouse Stuff
	SDL_Rect m_mouseRect;

	// Shortcuts
	std::vector<int> m_shortcuts;
	float m_keyTimer;
};

