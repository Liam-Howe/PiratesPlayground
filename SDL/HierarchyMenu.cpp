#include "stdafx.h"
#include "HierarchyMenu.h"
#include <SDL_image.h>
#include <iostream>

HierarchyMenu * HierarchyMenu::m_inst = nullptr;
HierarchyMenu * HierarchyMenu::Instance()
{
	if (!m_inst)
	{
		m_inst = new HierarchyMenu();
	}
	return m_inst;
}

HierarchyMenu::HierarchyMenu()
	: m_selectedPrimaryButton(0)
	, m_selectedSecondaryButton(0)
	, m_viewingSecondary(false)
	, m_btnClicked(false)
{

}


HierarchyMenu::~HierarchyMenu()
{
}


void HierarchyMenu::Update()
{
	mouseHighlight();
	if (m_keyTimer > 0)
	{
		m_keyTimer--;
	}
}

void HierarchyMenu::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, m_backgroundTexture, &m_backgroundSource, &m_backgroundPos);
	//SDL_RenderCopy(renderer, m_menuTitle, NULL, &m_menuTitlePos);

	for (int i = 0; i < m_primaryButtons.size(); ++i)
	{
		SDL_RenderCopy(renderer, m_primaryButtons.at(i)->getTexture(), &m_primaryButtons.at(i)->getSourcePosition(), &m_primaryButtons.at(i)->getPosition());

		if (m_viewingSecondary == true)
		{
			for (int j = 0; j < m_secondaryButtons.at(m_selectedPrimaryButton).size(); j++)
			{
				SDL_RenderCopy(renderer, m_secondaryButtons.at(m_selectedPrimaryButton).at(j)->getTexture(), &m_secondaryButtons.at(m_selectedPrimaryButton).at(j)->getSourcePosition(), &m_secondaryButtons.at(m_selectedPrimaryButton).at(j)->getPosition());
			}
		}
		
	}
	// Highlight Button
	if (m_viewingSecondary == true)
	{
		if (m_selectedSecondaryButton >= m_secondaryButtons.at(m_selectedPrimaryButton).size())
		{
			m_selectedSecondaryButton = 0;
		}
		if (m_selectedPrimaryButton == m_secondaryButtons.at(m_selectedPrimaryButton).at(m_selectedSecondaryButton)->getID())
		{
			m_cursorPos = m_secondaryButtons.at(m_selectedPrimaryButton).at(m_selectedSecondaryButton)->getPosition();
		}
	}
	else
	{
		if (m_selectedPrimaryButton == m_primaryButtons.at(m_selectedPrimaryButton)->getID())
		{
			m_cursorPos = m_primaryButtons.at(m_selectedPrimaryButton)->getPosition();
		}
	}
	SDL_RenderCopy(renderer, m_cursor, &m_cursorSource, &m_cursorPos);


}

void HierarchyMenu::CreateBackgroundTexture(SDL_Rect pos, SDL_Rect sourcePos, SDL_Texture *tex)
{
	m_backgroundTexture = tex;
	m_backgroundPos = pos;
	m_backgroundSource = sourcePos;
}

void HierarchyMenu::CreateMenuTitle(std::string path, std::string message, int fontSize, SDL_Color color, SDL_Rect pos, SDL_Renderer * renderer)
{
	m_menuTitlePos = pos;
	//m_menuTitle = AssetLoader::loadFont(path, message, fontSize, color, m_menuTitlePos, renderer);
}

void HierarchyMenu::AddPrimaryButton(SDL_Rect pos, SDL_Rect sourcePos, int id, SDL_Texture * tex)
{
	SDL_Texture* tempTex = tex;
	m_primaryButtons.push_back(new Button(pos, tempTex, sourcePos, id));
	m_secondaryButtons.push_back(std::vector<Button*>());
}

void HierarchyMenu::AddSecondaryButton(SDL_Texture * tex, SDL_Rect pos, SDL_Rect sourcePos, int id, int index)
{
	SDL_Texture* tempTex = tex;
	m_secondaryButtons.at(id).push_back(new Button(pos, tempTex, sourcePos, id, index));

}

void HierarchyMenu::ChangeSelectedButton(int dir)
{
	if (m_viewingSecondary == false)
	{
		int numPrimaryButton = m_primaryButtons.size();

		m_selectedPrimaryButton += dir;

		if (m_selectedPrimaryButton >= numPrimaryButton)
		{
			m_selectedPrimaryButton = 0;
		}
		if (m_selectedPrimaryButton < 0)
		{
			m_selectedPrimaryButton = numPrimaryButton - 1;
		}
	}
	else
	{
		int numSecondaryButton = m_secondaryButtons.at(m_selectedPrimaryButton).size();

		m_selectedSecondaryButton += dir;

		if (m_selectedSecondaryButton >= numSecondaryButton)
		{
			m_selectedSecondaryButton = 0;
		}
		if (m_selectedSecondaryButton < 0)
		{
			m_selectedSecondaryButton = numSecondaryButton - 1;
		}
	}
}

void HierarchyMenu::SelectButton()
{
	//m_btnClicked = true;
	if (m_secondaryButtons.at(m_selectedPrimaryButton).size() == 0)
	{
		m_btnClicked = true;
	}
	else if (m_viewingSecondary == true)
	{
		m_btnClicked = true;
	}
	else
	{
		m_viewingSecondary = true;
	}

}

void HierarchyMenu::ReturnToPrimaryList()
{
	if (m_viewingSecondary)
	{
		m_viewingSecondary = !m_viewingSecondary;
		m_selectedSecondaryButton = 0;
	}
}

void HierarchyMenu::SetButtonHighlight(SDL_Rect pos, SDL_Rect sourcePos, SDL_Texture * tex)
{
	m_cursorPos = pos;
	m_cursorSource = sourcePos;
	SDL_Texture* tempTex = tex;
	m_cursor = tempTex;
}
void HierarchyMenu::mouseDown()
{
	SDL_Point mousePos = getMousePoint();

	SDL_Rect mouseRect = { mousePos.x, mousePos.y, 1, 1 };

	for (int i = 0; i < m_primaryButtons.size(); i++)
	{
		if (SDL_HasIntersection(&mouseRect, &m_primaryButtons.at(i)->getPosition()))
		{
			if (m_viewingSecondary == true && i == m_selectedPrimaryButton)
			{
				ReturnToPrimaryList();
			}
			else if (m_viewingSecondary == true && i != m_selectedPrimaryButton)
			{
				m_selectedPrimaryButton = i;
			}
			else
			{
				SelectButton();
			}
				
		}
	}
	for (int j = 0; j < m_secondaryButtons.at(m_selectedPrimaryButton).size(); j++)
	{
		if(SDL_HasIntersection(&mouseRect, &m_secondaryButtons.at(m_selectedPrimaryButton).at(j)->getPosition()))
		{
			SelectButton();
		}
	}
}

void HierarchyMenu::mouseHighlight()
{
	SDL_Point mousePos = getMousePoint();

	SDL_Rect mouseRect = { mousePos.x, mousePos.y, 1, 1 };

	for (int i = 0; i < m_primaryButtons.size(); i++)
	{
		if (SDL_HasIntersection(&mouseRect, &m_primaryButtons.at(i)->getPosition()))
		{
			if (!m_viewingSecondary)
			{
				m_selectedPrimaryButton = i;
			}
		}
	}

	for (int j = 0; j < m_secondaryButtons.at(m_selectedPrimaryButton).size(); j++)
	{
		if (SDL_HasIntersection(&mouseRect, &m_secondaryButtons.at(m_selectedPrimaryButton).at(j)->getPosition()))
		{
			m_selectedSecondaryButton = j;
		}
	}
}

void HierarchyMenu::ClickShortcut(int shortcut)
{
	if (m_keyTimer <= 0)
	{
		m_keyTimer = 1000;
		if (!m_viewingSecondary)
		{
			m_selectedPrimaryButton = shortcut;
			SelectButton();
		}
		else
		{
			m_selectedSecondaryButton = shortcut;
			SelectButton();
		}
	}
}

void HierarchyMenu::ClearMenu()
{
	for (size_t i = 0; i < m_primaryButtons.size(); i++)
	{
		delete m_primaryButtons[i];
		m_primaryButtons[i] = nullptr;
	}
	m_primaryButtons.clear();
	m_secondaryButtons.clear();

	m_selectedPrimaryButton = 0;
	m_selectedPrimaryButton = 0;
	m_btnClicked = false;
	m_viewingSecondary = false;
	m_backgroundTexture = nullptr;
}

SDL_Point* HierarchyMenu::getButtonClicked()
{
	if (m_btnClicked)
	{
		m_btnClicked = false;
		SDL_Point* temp = new SDL_Point{ m_selectedPrimaryButton, m_selectedSecondaryButton };
		return temp;
	}
	else
	{
		return nullptr;
	}
}

SDL_Point HierarchyMenu::getMousePoint()
{
	int x;
	int y;
	SDL_GetMouseState(&x, &y);

	return SDL_Point{ x, y };
}
