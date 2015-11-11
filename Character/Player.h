/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
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
#pragma once
#include "Character\PlayableChar.h"
#include "Character\Charstats.h"
#include "Character\Look\CharLook.h"
#include "Character\Inventory\Inventory.h"
#include "Character\Skillbook.h"
#include "Character\Questlog.h"
#include "Character\Telerock.h"
#include "Character\Monsterbook.h"
#include "Character\PlayerStates.h"
#include "Gameplay\Physics\Physics.h"
#include "Gameplay\MovementInfo.h"
#include "Graphics\Textlabel.h"
#include "Util\rectangle2d.h"

namespace Character
{
	using::Util::rectangle2d;
	using::Graphics::Textlabel;
	using::Gameplay::Physics;
	using::Gameplay::PhysicsObject;
	using::Gameplay::MovementInfo;

	// A class that represents the player. Implements 'Mapobject' for on-map behaviour and 'Playable' for processing Keyactions.
	class Player : public PlayableChar
	{
	public:
		// Construct a player object with the given id, look and stats. Also initializes nametag, flip and stance.
		Player(int32_t, CharLook, Charstats);
		Player();
		// Initialize the player's states like standing, walking etc. After doing so the player will respond to key inputs.
		void initcontrol();
		// Sends a Keyaction to the player's state, to apply forces, change the state and other behaviour.
		// Parameters: Keyaction(maple key-code), bool(is key down)
		void sendaction(Keyaction, bool);
		// Set if the player and look should be flipped.
		// Parameters: bool(is facing right)
		void setflip(bool);
		// Set the current stance, which determines look and behaviour. Also resets the current animation.
		// Parameters: CharStance(new stance)
		void setstance(CharStance);
		// Can be called after sending player movement to the server to start a new stack.
		void clearmovement();
		// Recalculates the total stats from base stats, inventories and skills.
		// Parameters: bool(also recalculate inventory stats)
		void recalcstats(bool);
		// Draw the player's look and nametag using the specified position.
		// Parameters: vector2d<int>(camera position), float(interpolation value)
		void draw(vector2d<int32_t>, float) const;
		// Update the player's animation, physics and states.
		// Parameters: Physics&(engine to use)
		int8_t update(const Physics&);
		// Set the PhysicsObject's position as specified. Used for respawning and teleports.
		void setposition(int32_t, int32_t);
		// Returns the current walking force, calculated from the total ES_SPEED stat.
		float getwforce() const;
		// Returns the current jumping force, calculated from the total ES_JUMP stat.
		float getjforce() const;
		// Returns if the player is flipped, eg. facing right.
		float getflyforce() const;
		// Returns if the player is flipped, eg. facing right.
		bool getflip() const;
		// Returns the current stance.
		CharStance getstance() const;
		// Returns the object id, equivalent to the character id in the database.
		int32_t getoid() const;
		// Returns the layer of the current foothold. Used to determine drawing order of mapobjects.
		int8_t getlayer() const;
		// Returns if a Keyaction is currently active. Used by player states for some behaviour (ex. jumpdown).
		bool keydown(Keyaction) const;
		// Returns the current position as a point from the PhysicsObject's position.
		vector2d<int32_t> getposition() const;
		// Returns the collision rectangle.
		rectangle2d<int32_t> bounds() const;
		// References to various player properties.
		Charstats& getstats();
		CharLook& getlook();
		Inventory& getinvent();
		Skillbook& getskills();
		Questlog& getquests();
		Telerock& gettrock();
		Monsterbook& getmonsterbook();
		PhysicsObject& getphobj();
		// Obtain a reference to this object's movement info so it can be sent to the server.
		const MovementInfo& getmovement() const;
	private:
		CharLook look;
		Charstats stats;
		int32_t cid;
		CharStance stance;
		bool flip;
		Inventory inventory;
		Skillbook skillbook;
		Questlog questlog;
		Telerock telerock;
		Monsterbook monsterbook;
		Textlabel name;
		Ptrmap<CharStance, PlayerState> plstates;
		map<Keyaction, bool> keysdown;
		PhysicsObject phobj;
		MovementInfo movementinfo;
	};
}
