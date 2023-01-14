#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;


//room structures
typedef struct{
	char name[MAX_STR];
	struct RoomList *connectedRooms;
	struct EvidenceList *evidenceList;
	struct HType *hunters[MAX_HUNTERS];
	struct GhostType *ghost;
	sem_t mutex;
} RoomType;

typedef struct RNode{
	RoomType *data;
	struct RNode *next;
} RoomNode;

typedef struct RoomList{
	RoomNode *head;
} RoomListType;


//ghost structures
typedef struct{
	GhostClassType ghostType;
	RoomType *currRoom;
	int evidenceType[4];
	int boredom;
} GhostType;


//evidence structures
typedef struct{
	EvidenceClassType evidenceType;
	float value;
} EvidenceType;

typedef struct ENode{
	EvidenceType *data;
	struct ENode *next;
} EvidenceNode;

typedef struct EvidenceList{
	EvidenceNode *head;
} EvidenceListType;


//hunter structures
typedef struct HType{
	RoomType *currRoom;
	EvidenceClassType evidenceType;
	EvidenceListType *evidence;
	char name[MAX_STR];
	int fear;
	int boredom;
} HunterType;


//building structures
typedef struct{
	GhostType *ghost;
	HunterType *allHunters[MAX_HUNTERS];
	RoomListType *rooms;
} BuildingType;



//forward declarations
void initRoomList(RoomListType*);
void initRoom(RoomType*, char*);
void appendRoom(RoomListType*, RoomNode*);
void connectRooms(RoomType*, RoomType*);
int getRoomListSize(RoomListType*);	//HELPER FUNCTION
void cleanupRooms(RoomListType*);
void cleanupConnectedRooms(RoomListType*);

void initGhost(GhostType*, GhostClassType, RoomType*);
void ghostMove(GhostType*);		//move ghost's position
	//- moves the ghost to an adjacent room
	//- decided by RNG
	//- update appropriate pointers
	//- print the movement
void ghostLeaveEvidence(GhostType*);	//leave evidence at current room
	//- use RNG to decide which evidence to leave behind
	//- creates evidence structure and points it to current room
	//- print evidence left behind
void *ghostThread(void*);		//thread function for ghost
	//- encompasses all ghost actions and decision making
	//- if hunter is present, leave evidence or do nothing
	//- if not, leave evidence, do nothing, or move
	//- change boredom value appropriately
	//- print the decision

void initEvidenceList(EvidenceListType*);
void initEvidence(EvidenceType*, EvidenceClassType, float);
void appendEvidence(EvidenceListType*, EvidenceType*);
void deleteEvidence(EvidenceListType*, float);
int checkGhostly(EvidenceType*);	//HELPER FUNCTION
void cleanupEvidenceList(EvidenceListType*);

void initHunter(HunterType*, RoomType*, EvidenceClassType, char*);
void hunterMove(HunterType*);		//move the hunter's position
	//- moves the hunter to an adjacent room
	//- decided by RNG
	//- update appropriate pointers
	//- print the movement
void hunterCheckEvidence(HunterType*);	//check the current room for evidence
	//- check evidence list within room
	//- if evidence of same type as hunter exists, remove from room and add to hunter's list
	//- if not, create standard evidence of random value and append to hunter's list
	//- print result
void communicate(HunterType*);		//trade ghostly evidence with another hunter in current room
	//- get all ghostly evidence from other hunter
	//- print action
void *hunterThread(void *);		//thread function for hunters
	//- encompasses all hunter actions and calculations
	//- if another hunter is present, collect evidence, communicate, or move
	//- if not, collect evidence or move
	//- print action
void cleanupHunter(HunterType*);

void initBuilding(BuildingType*);
void cleanup(BuildingType*);

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void populateRooms(BuildingType*);   // Populates the building with sample data for rooms

























