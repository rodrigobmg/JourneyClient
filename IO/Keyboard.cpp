/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015-2016 Daniel Allendorf                                   //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "Keyboard.h"

#include "glfw3.h"

namespace jrc
{
	constexpr int32_t Keytable[90] =
	{
		0, 0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, // number keys, up to key 0
		0, 0, 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', 0, 0, 0, //first letter row, up to key 28
		GLFW_KEY_LEFT_CONTROL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, 0, 0,  //second row, up to 41
		GLFW_KEY_LEFT_SHIFT, 0, 'Y', 'X', 'C', 'V', 'B', 'N', 'M', 0, 0, 0, 0, 0, GLFW_KEY_LEFT_ALT, GLFW_KEY_SPACE, 0, //third row, up to 58
		GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12, //up to 70
		GLFW_KEY_HOME, 0, GLFW_KEY_PAGE_UP, 0, 0, 0, 0, 0, GLFW_KEY_END, 0, GLFW_KEY_PAGE_DOWN, GLFW_KEY_INSERT, GLFW_KEY_DELETE, 0, 0, 0, 0, 0, 0
	};

	Keyboard::Keyboard()
	{
		keymap[GLFW_KEY_LEFT] = { KeyType::ACTION, KeyAction::LEFT };
		keymap[GLFW_KEY_RIGHT] = { KeyType::ACTION, KeyAction::RIGHT };
		keymap[GLFW_KEY_UP] = { KeyType::ACTION, KeyAction::UP };
		keymap[GLFW_KEY_DOWN] = { KeyType::ACTION, KeyAction::DOWN };

		textactions[GLFW_KEY_BACKSPACE] = KeyAction::BACK;
		textactions[GLFW_KEY_ENTER] = KeyAction::RETURN;
		textactions[GLFW_KEY_SPACE] = KeyAction::SPACE;
		textactions[GLFW_KEY_TAB] = KeyAction::TAB;
	}

	int32_t Keyboard::shiftcode() const
	{
		return GLFW_KEY_LEFT_SHIFT;
	}

	int32_t Keyboard::ctrlcode() const
	{
		return GLFW_KEY_LEFT_CONTROL;
	}

	KeyAction::Id Keyboard::get_ctrl_action(int32_t keycode) const
	{
		switch (keycode)
		{
		case GLFW_KEY_C:
			return KeyAction::COPY;
		case GLFW_KEY_V:
			return KeyAction::PASTE;
		default:
			return KeyAction::NOACTION;
		}
	}

	void Keyboard::assign(uint8_t key, uint8_t tid, int32_t action)
	{
		if (KeyType::Id type = KeyType::typebyid(tid))
		{
			Mapping mapping{ type, action };
			keymap[Keytable[key]] = mapping;
			maplekeys[key] = mapping;
		}
	}

	Keyboard::Mapping Keyboard::get_text_mapping(int32_t keycode, bool shift) const
	{
		if (textactions.count(keycode))
		{
			return{ KeyType::ACTION, textactions.at(keycode) };
		}
		else if (keycode > 47 && keycode < 65)
		{
			return{ KeyType::NUMBER, keycode - (shift ? 15 : 0) };
		}
		else if (keycode > 64 && keycode < 91)
		{
			return{ KeyType::LETTER, keycode + (shift ? 0 : 32) };
		}
		else
		{
			switch (keycode)
			{
			case GLFW_KEY_LEFT:
			case GLFW_KEY_RIGHT:
			case GLFW_KEY_UP:
			case GLFW_KEY_DOWN:
				return keymap.at(keycode);
			default:
				return{ KeyType::NONE, 0 };
			}
		}
	}

	Keyboard::Mapping Keyboard::get_mapping(int32_t keycode) const
	{
		auto iter = keymap.find(keycode);
		if (iter == keymap.end())
			return{};

		return iter->second;
	}
}
