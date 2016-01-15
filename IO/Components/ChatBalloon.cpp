/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                               //
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
#include "ChatBalloon.h"
#include "Program\Constants.h"
#include "nlnx\nx.hpp"
#include "nlnx\node.hpp"

namespace IO
{
	// How long a line stays on screen. 4 seconds.
	const int16_t DURATION = 4000;

	ChatBalloon::ChatBalloon(int8_t type)
	{
		string typestr;
		if (type < 0)
		{
			switch (type)
			{
			case -1:
				typestr = "dead";
				break;
			}
		}
		else
		{
			typestr = std::to_string(type);
		}

		using nl::node;
		node src = nl::nx::ui["ChatBalloon.img"][typestr];

		arrow = Texture(src["arrow"]);
		center = Texture(src["c"]);
		east = Texture(src["e"]);
		northeast = Texture(src["ne"]);
		north = Texture(src["n"]);
		northwest = Texture(src["nw"]);
		west = Texture(src["w"]);
		southwest = Texture(src["sw"]);
		south = Texture(src["s"]);
		southeast = Texture(src["se"]);

		textlabel = Text(Text::A11M, Text::CENTER, Text::BLACK);

		xtile = north.getdimensions().x();
		ytile = west.getdimensions().y();
	}

	ChatBalloon::ChatBalloon() : ChatBalloon(0) {}

	ChatBalloon::~ChatBalloon() {}

	void ChatBalloon::settext(string text)
	{
		textlabel.settext(text, 80);

		int16_t numhor = static_cast<int16_t>(textlabel.getdimensions().x() / xtile) + 2;
		int16_t numver = static_cast<int16_t>(textlabel.getdimensions().y() / ytile) + 1;

		width = numhor * xtile;
		height = numver * ytile;

		duration = DURATION;
	}

	void ChatBalloon::draw(vector2d<int16_t> position) const
	{
		if (duration == 0)
			return;

		position.shifty(-85);

		int16_t numhor = (width / xtile);
		int16_t numver = (height / ytile);
		int16_t left = position.x() - width / 2;
		int16_t top = position.y() - height;

		northwest.draw(vector2d<int16_t>(left, top));
		southwest.draw(vector2d<int16_t>(left, top + height));

		for (int8_t i = 0; i < numver; i++)
		{
			int16_t yshift = (ytile * i);
			west.draw(vector2d<int16_t>(left, top + yshift));

			for (int8_t j = 0; j < numhor; j++)
			{
				int16_t xshift = (xtile * j);
				center.draw(vector2d<int16_t>(left + xshift, top + yshift));
			}

			east.draw(vector2d<int16_t>(left + width, top + yshift));
		}

		for (int8_t i = 0; i < numhor; i++)
		{
			int16_t xshift = (xtile * i);
			north.draw(vector2d<int16_t>(left + xshift, top));
			south.draw(vector2d<int16_t>(left + xshift, top + height));
		}

		northeast.draw(vector2d<int16_t>(left + width, top));
		southeast.draw(vector2d<int16_t>(left + width, top + height));

		arrow.draw(position);

		textlabel.draw(vector2d<int16_t>(position.x(), top - 4));
	}

	void ChatBalloon::update()
	{
		duration -= Constants::TIMESTEP;
		if (duration < 0)
			duration = 0;
	}

	void ChatBalloon::expire()
	{
		duration = 0;
	}
}