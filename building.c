
#include "defs.h"

/*
	Note: While this function is provided, you will likely want
	to modify it to suit your solution. This has been provided
	to give you a starting point to lay out the room connectivity,
	but it is entirely possible that you will want to use different
	data structures to represent the rooms and their connectivity,
	or different function names.

	Not following this function's structure exactly will not result
	in a deduction of points.

	The map that this is recreating is visible in the assignment
	specification, and also available by the original creator here:
	https://steamcommunity.com/sharedfiles/filedetails/?id=2251267947
    
*/

void populateRooms(BuildingType* building) {
	// First, create each room. Perhaps you want to include more data 
	// in the init parameters?
	RoomType* van = calloc(1, sizeof(RoomType));
	initRoom(van, "Van");
	RoomType* hallway = calloc(1, sizeof(RoomType));
	initRoom(hallway, "Hallway");
	RoomType* master_bedroom = calloc(1, sizeof(RoomType));
	initRoom(master_bedroom, "Master Bedroom");
	RoomType* boys_bedroom = calloc(1, sizeof(RoomType));
	initRoom(boys_bedroom, "Boy's Bedroom");
	RoomType* bathroom = calloc(1, sizeof(RoomType));
	initRoom(bathroom, "Bathroom");
	RoomType* basement = calloc(1, sizeof(RoomType));
	initRoom(basement, "Basement");
	RoomType* basement_hallway = calloc(1, sizeof(RoomType));
	initRoom(basement_hallway, "Basement Hallway");
	RoomType* right_storage_room = calloc(1, sizeof(RoomType));
	initRoom(right_storage_room, "Right Storage Room");
	RoomType* left_storage_room = calloc(1, sizeof(RoomType));
	initRoom(left_storage_room, "Left Storage Room");
	RoomType* kitchen = calloc(1, sizeof(RoomType));
	initRoom(kitchen, "Kitchen");
	RoomType* living_room = calloc(1, sizeof(RoomType));
	initRoom(living_room, "Living Room");
	RoomType* garage = calloc(1, sizeof(RoomType));
	initRoom(garage, "Garage");
	RoomType* utility_room = calloc(1, sizeof(RoomType));
	initRoom(utility_room, "Utility Room");

	// Now create a linked list of rooms using RoomNode in the Building
	RoomNode* van_node = calloc(1, sizeof(RoomNode));
	van_node->data = van;
	RoomNode* hallway_node = calloc(1, sizeof(RoomNode));
	hallway_node->data = hallway;
	RoomNode* master_bedroom_node = calloc(1, sizeof(RoomNode));
	master_bedroom_node->data = master_bedroom;
	RoomNode* boys_bedroom_node = calloc(1, sizeof(RoomNode));
	boys_bedroom_node->data = boys_bedroom;
	RoomNode* bathroom_node = calloc(1, sizeof(RoomNode));
	bathroom_node->data = bathroom;
	RoomNode* basement_node = calloc(1, sizeof(RoomNode));
	basement_node->data = basement;
	RoomNode* basement_hallway_node = calloc(1, sizeof(RoomNode));
	basement_hallway_node->data = basement_hallway;
	RoomNode* right_storage_room_node = calloc(1, sizeof(RoomNode));
	right_storage_room_node->data = right_storage_room;
	RoomNode* left_storage_room_node = calloc(1, sizeof(RoomNode));
	left_storage_room_node->data = left_storage_room;
	RoomNode* kitchen_node = calloc(1, sizeof(RoomNode));
	kitchen_node->data = kitchen;
	RoomNode* living_room_node = calloc(1, sizeof(RoomNode));
	living_room_node->data = living_room;
	RoomNode* garage_node = calloc(1, sizeof(RoomNode));
	garage_node->data = garage;
	RoomNode* utility_room_node = calloc(1, sizeof(RoomNode));
	utility_room_node->data = utility_room;

	// Building->rooms might be a linked list structre, or maybe just a node.
	initRoomList(building->rooms);

	appendRoom(building->rooms, van_node);
	appendRoom(building->rooms, hallway_node);
	appendRoom(building->rooms, master_bedroom_node);
	appendRoom(building->rooms, boys_bedroom_node);
	appendRoom(building->rooms, bathroom_node);
	appendRoom(building->rooms, basement_node);
	appendRoom(building->rooms, basement_hallway_node);
	appendRoom(building->rooms, right_storage_room_node);
	appendRoom(building->rooms, left_storage_room_node);
	appendRoom(building->rooms, kitchen_node);
	appendRoom(building->rooms, living_room_node);
	appendRoom(building->rooms, garage_node);
	appendRoom(building->rooms, utility_room_node);

	// Now connect the rooms. It is possible you do not need a separate
	// function for this, but it is provided to give you a starting point.
	connectRooms(van, hallway);
	connectRooms(hallway, master_bedroom);	
	connectRooms(hallway, boys_bedroom);	
	connectRooms(hallway, bathroom);	
	connectRooms(hallway, kitchen);	
	connectRooms(hallway, basement);	
	connectRooms(basement, basement_hallway);
	connectRooms(basement_hallway, right_storage_room);	
	connectRooms(basement_hallway, left_storage_room);
	connectRooms(kitchen, living_room);	
	connectRooms(kitchen, garage);	
	connectRooms(garage, utility_room);
	
	
}

/*
	Function: initBuilding
	 Purpose: initializes the properties of the given building
	      in: building to be initialized
*/
void initBuilding(BuildingType *b){
	b->ghost = NULL;
	b->rooms = malloc(sizeof(RoomListType));
}

/*
	Function: cleanup
	 Purpose: deallocate memory of everything in the building
	      in: building list to be cleaned
*/
void cleanup(BuildingType *b){
	//clean up the rooms
	cleanupRooms(b->rooms);	
	
	//clean up the hunters
	for (int x=0; x<MAX_HUNTERS; x++){
		cleanupHunter(b->allHunters[x]);
	}
}






















