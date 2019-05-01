-- AUTHOR: Steven Savold
-- Basic implementation for the verbs defined in molly's mansion game

function get(item_id)
    -- #TODO: implement me!

    -- update items holder to the player, 
    -- IFF the item is gettable and the items holder is the players holder
end

function look(item_id)
    -- #TODO: implement me!
    
    -- print the desc of the item IFF, the items holder is the player, or the players holder and is_viewed is true
    -- otherwise if the its holder is the player or the players holder and it has not been viewed, print the first_time_desc
    -- else error
end

function examine(item_id)
    -- #TODO: implement me!
    
    -- print the first_time_desc always if the items holder is the player or the players holder
end

function drop(item_id)
    -- #TODO: implement me!
    
    -- if the items holder is the player, update the items holder to the players holder
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