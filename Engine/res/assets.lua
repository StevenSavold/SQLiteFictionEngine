-- AUTHOR: Steven Savold
-- Basic implementation for the verbs defined in molly's mansion game

function get(item_id)
    -- #TODO: implement me!
end

function look(item_id)
	-- #TODO: implement me!
end

function examine(item_id)
	-- #TODO: implement me!
end

function drop(item_id)
	-- #TODO: implement me!
end

function n(item_id)
	local curr_room = get_current_room();
	local n, s, e, w, u, d = get_room_transitions(curr_room);

	-- This function moves the player north, 
	-- so update the players room to the n variable
	if player_transition_to(n) then
		print_location_text()
	else
		print("You cannot move that way!!")
	end
end

function s(item_id)
	local curr_room = get_current_room();
	local n, s, e, w, u, d = get_room_transitions(curr_room);

	-- This function moves the player south, 
	-- so update the players room to the s variable
	if player_transition_to(s) then
		print_location_text()
	else
		print("You cannot move that way!!")
	end
end

function e(item_id)
	local curr_room = get_current_room();
	local n, s, e, w, u, d = get_room_transitions(curr_room);

	-- This function moves the player east, 
	-- so update the players room to the e variable
	if player_transition_to(e) then
		print_location_text()
	else
		print("You cannot move that way!!")
	end
end

function w(item_id)
	local curr_room = get_current_room();
	local n, s, e, w, u, d = get_room_transitions(curr_room);

	-- This function moves the player west, 
	-- so update the players room to the w variable
	if player_transition_to(w) then
		print_location_text()
	else
		print("You cannot move that way!!")
	end
end

function u(item_id)
	local curr_room = get_current_room();
	local n, s, e, w, u, d = get_room_transitions(curr_room);

	-- This function moves the player up, 
	-- so update the players room to the u variable
	if player_transition_to(u) then
		print_location_text()
	else
		print("You cannot move that way!!")
	end
end

function d(item_id)
	local curr_room = get_current_room();
	local n, s, e, w, u, d = get_room_transitions(curr_room);

	-- This function moves the player down, 
	-- so update the players room to the d variable
	if player_transition_to(d) then
		print_location_text()
	else
		print("You cannot move that way!!")
	end
end



-- AUTHOR DEFINED INITALIZATION --

-- the ID of the starting room
local START_ROOM_ID = 2

-- This works as a rudementary way of checking if this is a new game
if get_current_room() == START_ROOM_ID then
	print_offscreen_text()
	print()
end 

-- print the players location on startup
print_location_text()