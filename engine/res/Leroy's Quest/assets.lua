-- AUTHOR: Steven Savold
-- Basic implementation for the verbs defined in molly's mansion game

function get(item_id)
    -- #TODO: implement me!

    -- update items holder to the player, 
    -- IFF the item is gettable and the items holder is the players holder

	local items_holder = get_holder(item_id)
	local players_holder = get_current_room()

	if is_gettable(item_id) and items_holder == players_holder then
		update_holder(item_id, get_player_id())
	else
		print("You cannot get what isnt there.")
	end

end

function look(item_id)
    
    -- print the desc of the item IFF, the items holder is the player, 
    --     or the players holder and is_viewed is true
    -- otherwise if the its holder is the player or the players holder
    --     and it has not been viewed, print the first_time_desc
    -- else error

    local items_holder = get_holder(item_id)
	local players_holder = get_current_room()
	
	-- if no item was specified, look at the room and exit
	if item_id == -1 then 
		look_at_room(players_holder)
		return
	end 

    if items_holder == players_holder or items_holder == get_player_id() then
        if is_viewed(item_id) then
            print_desc(item_id)
        else
            print_first_desc(item_id)
        end
    else
        print("You can't see anything like that here...")
    end

end

function look_at_room(room_id)
	print_desc(room_id)
end

function examine(item_id)
    
    -- print the first_time_desc always if the items holder is the player or 
    -- the players holder
    local items_holder = get_holder(item_id)
    local player_holder = get_current_room()

    if items_holder == player_holder or items_holder == get_player_id() then 
        print_first_desc(item_id)
    else 
        print("You can't see anything like that here...")
    end

end

function drop(item_id)
    
    -- if the items holder is the player, update the items holder to the players holder
    items_holder = get_holder(item_id)
    players_holder = get_current_room()

    if items_holder == get_player_id() then
        update_holder(item_id, players_holder)
    else
        print("You cannot drop what you don't have.")
    end

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

function help()
	print("Try some of these commands:")
	print("\t(n)orth, (s)outh, (e)ast, (w)est, (u)p, (d)own - to move in a direction")
	print("\tlook, look at - to get the description of something")
	print("\tget, take, pick up - to pick up an item from the room")
	print("\thelp - to print this text")
end

function exit()
	quit_game()
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