#include<stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct Items
{
	char name[30];
	char description[50];
	int additionalHelat;
	int additionalAttack;
	struct Items* next;
}Items;

typedef struct Creature{
	char name[30];
	int health;
	int attack;
}Creature;

typedef struct Room {
	int roomNumber;
	Creature* creature;
	Items* item;
	struct Room* nextRoom;
	struct Room* prevRoom;
}Room;


typedef struct Player {
	char name[30];
	int healt;
	int attack;
	Items* item;
}Player;


void attachRooms(Room* rooms[], int length) {
	for (int i = 0; i < length-1; i++)
	{
		rooms[i]->nextRoom = rooms[i + 1];
		rooms[i + 1]->prevRoom = rooms[i];
	}
}
void attachItems(Items* items[], int length) {
	for (int i = 0; i < length-1; i++)
	{
		items[i]->next = items[i + 1];
	}
}
void war(Creature* creature, Player* player1) {
	if (creature == NULL) {
		printf("There is no monster.");
		return;
	}
	printf("You ara against %s\n", creature->name);
	while (creature->health>0 && player1->healt>0) {
		printf("You strike first... You gave the %d damega\n", player1->attack);
		creature->health -= player1->attack;
		if (creature->health <= 0) {
			printf("You killed the monster.\n");
			return;
		}
		printf("It attack you and you get %d damega\n", creature->attack);
		player1->healt -= creature->attack;
		if (player1->healt < 0) {
			printf("You died\n");
			exit(1);
		}
	}
}
void pickUp(Room* room,Player* p1) {
	if (room->item == NULL) {
		printf("There is no item.\n");
		return;
	}
	Items* addItem = room->item;
	room->item = NULL;
	addItem->next = p1->item;
	p1->item = addItem;
	p1->attack += addItem->additionalAttack;
	p1->healt += addItem->additionalHelat;
	printf("You picked up %s\n", addItem->description);

}
void lookUp(int i) {
	FILE* file = fopen("Room.txt", "r");
	if (file == NULL) {
		printf("There is nothing.\n");
		return;
	}
	char line[100];
	int lineNumber = 1;
	while (fgets(line, sizeof(line), file) != NULL) {
		if (lineNumber == i) {
			printf("%s\n", line);
			break;
		}
		lineNumber++;
	}
	fclose(file);
}
void saveGame(Room* room,Player* p1) {
	FILE* file = fopen("saves.txt", "w");
	if (file == NULL)
	{
		printf("Error!");
	}
	fprintf(file,"Charecter healt %d\n", p1->healt);
	fprintf(file,"Charecter attack power %d\n", p1->attack);
	fprintf(file,"Current room number: %d\n", room->roomNumber);
	
	fclose(file);
	printf("Save completed.");
}
int loadGame(Room* room, Player* p1) {
	FILE* file = fopen("saves.txt", "r");
	if (file == NULL)
	{
		printf("Error!");
	}
	fscanf_s(file,"Charecter healt %d\n", &p1->healt);
	fscanf_s(file,"Charecter attack power %d\n", &p1->attack);
	
	int a;
	fscanf_s(file, "Current room number: %d\n",&a);
	fclose(file);
	printf("Load completed.\n");
	return a-1;
}
void exitGame() {
	printf("Your game is closed");
	exit(1);
}





int main() {
	Creature goblin = { "goblin",50,5 };
	Creature ork = { "ork",100,10 };
	Creature golem = { "golem",100,20 };
	Creature dragon = { "dragon",300,50 };
	Items item1 = { "Sword","Old and rusty sword",0,25 };
	Items item2 = { "Bread","Restore some healt",25,0 };
	Items item3 = { "Book","Old book about elixir of life",0,0 };
	Items item4 = { "Armor","Gives additional protection",50,0 };
	Items item5 = { "Golden Sword","Gives additional Attack",0,100 };
	Items item6 = { "Elixir","Gives immortality",0,0 };
	Items item7 = { "Potion","Restor your full health",200,0 };
	Items* items[] = { &item1,&item2,&item3,&item4,&item5,&item6,&item7 };
	attachItems(items, 7);

	Room rooms[14];
	for (int i = 0; i < 14; i++)
	{
		rooms[i].roomNumber = i + 1;
		rooms[i].creature = NULL;
		rooms[i].item = NULL;
		rooms[i].nextRoom = NULL;
		rooms[i].prevRoom = NULL;

	}
	rooms[2].creature = &goblin;
	rooms[5].creature = &ork;
	rooms[7].creature = &golem;
	rooms[11].creature = &dragon;
	
	rooms[1].item = &item1;
	rooms[3].item = &item2;
	rooms[6].item = &item4;
	rooms[7].item = &item7;
	rooms[8].item = &item5;
	rooms[9].item = &item3;
	rooms[12].item = &item6;

	Room* roomPtr[14];
	for (int i = 0; i < 14; i++)
	{
		roomPtr[i] = &rooms[i];
	}
	attachRooms(roomPtr, 14);
	char name[30];
	printf("Enter your name: ");
	scanf_s("%s", name, sizeof(name));
	Player p1 = { " ",100,50,NULL};
	strcpy_s(p1.name, sizeof(p1.name), name);
	printf("Enter your command:\n");
	char command[100];
	int i = 0;
	int loop = 0;
	while (loop==0) {
		rooms[i];
		//printf("Player health %d\n", p1.attack);
		scanf_s("%s", command,sizeof(command));

		if (strcmp(command,"MoveForward")==0) {
			if (i < 13) {
				i++;
				printf("You are at room %d\n", i + 1);
			}
			else
			{
				printf("You completed the game.");
			}
		}
		else if (strcmp(command, "MoveBack")==0) {
			if (i == 0) {
				printf("You cant go back");
			}
			else
			{
				i--;
				printf("You are at room %d\n", i + 1);
			}
		}
		else if (strcmp(command, "Pickup")==0) {
			pickUp(&rooms[i], &p1);
			rooms[i].item = NULL;
		}
		else if (strcmp(command, "LookUp")==0) {
			lookUp(i+1);
		}
		else if (strcmp(command, "Attack")==0) {
			war(rooms[i].creature, &p1);
			rooms[i].creature = NULL;
		}
		else if (strcmp(command, "Save")==0) {
			saveGame(&rooms[i], &p1);
		}
		else if (strcmp(command, "Load")==0) {
			i=loadGame(& rooms[i], &p1);

		}
		else if (strcmp(command, "Exit")==0) {
			exitGame();
		}
		else
		{
			printf("Invalid move!\n");
		}
	}

	return 0;
}