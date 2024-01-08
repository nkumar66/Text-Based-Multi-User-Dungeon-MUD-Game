#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//extra
#define MAX_INVENTORY_SIZE 10000 
#define DISPLAY_SIZE 10

typedef struct  Room {
   int id;
   char *name;
   char *description;
   int south, north, east, west;
   int object;
   bool starting;
}Room;

typedef struct Object {
    int id;
    char *name;
   char *description;
}Object;

struct Room *rooms;
struct Object *objects;




int extractInt(char * line){

    char *copy = strdup(line);
    char *token = NULL;

    token = strtok(copy, ":");
    token = strtok(NULL, ":");

    token[strlen(token) -1] =0;
    int extractInt = -1;
    sscanf(token, "%d", &extractInt);

    free(copy);

    return extractInt;

}

char * extractString(char * line){

    char *copy = strdup(line);
    char *token = NULL;
    token = strtok(copy, ":");
    token = strtok(NULL, ":");
    token[strlen(token) -1] =0;
    token ++;
    token ++;
    token[strlen(token) -1] =0;

    char *output = strdup(token);
    free(copy);
    return output;
}



bool loadData() {

    FILE *file = fopen("world.json","r");
    if(!file) {
        printf("Not found or Failed to open file");
        return false;
    }

    char *line = NULL;
    size_t read;
    size_t len;

    while( (read = getline(&line, &len, file)) != -1) {
        if(line[read -1]== '\n') {
            line[read -1] = 0;
            read --;
        }
    Room r;
      
    if (strstr(line, "\"id\"")) {
        r.id = extractInt(line);
    }

   if (strstr(line, "\"name\"")) {
      r.name =extractString(line);
   }

   if (strstr(line, "\"description\"")) {
      r.description =extractString(line);
   }

   if (strstr(line, "\"north\"")) {
      r.north = extractInt(line);
   }

   if (strstr(line, "\"south\"")) {
        r.south = extractInt(line);
   }

    if (strstr(line, "\"east\"")) {
        r.east = extractInt(line);
   }

   if (strstr(line, "\"west\"")) {
       r.west = extractInt(line);
   }
    if (strstr(line, "\"object\"")) {
       r.object = extractInt(line);
   }
    if (strstr(line, "\"starting\"")) {
        r.starting =extractString(line);
   }
 
   if (strstr(line, "}")) {
     rooms[r.id] = r;
       
   }
  if (strstr(line, "\"objects\"")) {
       break;
    }
  
   

    }

     while( (read = getline(&line, &len, file)) != -1) {
        if(line[read -1]== '\n') {
            line[read -1] = 0;
            read --;
        }

Object obj;
 
      if (strstr(line, "]")) {
       break;
   }
if (strstr(line, "\"id\"")) {
      obj.id = extractInt(line);
   }

   if (strstr(line, "\"name\"")) {
     obj.name =extractString(line);
   }

   if (strstr(line, "\"description\"")) {
      obj.description =extractString(line);
   }


   if (strstr(line, "}")) {
     objects[obj.id] = obj;

   }

    }
    return true;
    }


    int findbyId(int id){
        for(int i =0; i <1000; i++){
            if(objects[i].id == id){
                return i;
            }
        }
      return -1;  
    }
    
    void showRoom(int current) {
        int objIndex = findbyId(rooms[current].object);
     
        if (rooms[current].name == NULL) {
            printf("You can't walk there.\n");
        } 

        printf("#%d : %s\n", current, rooms[current].name);
        printf("\n");
        printf("%s\n", rooms[current].description);
        printf("\n");

        if (objects[objIndex].id != -1 && objects[objIndex].name != NULL) {
            printf("You see a %s\n",  objects[objIndex].name);
            printf("\n");
        }

    printf("[ [n]orth, [s]outh, [l]ook, [g]et, [i]nventory, [d]rop, [q]uit ]\n");
    return;
    }
  

    void gameLoop(){
        
        char input[100];
        int currentRoom;
        int inventory[MAX_INVENTORY_SIZE];
        int inventoryCount = 0;
        
       

    
 
        for (int i = 0; i < 1000; i++){
            if (rooms[i].starting){
                currentRoom = i;
                break;
            }

        }

      while(true){
          showRoom(currentRoom);


          printf("> ");


         fgets(input, sizeof(input), stdin);
        
        if(input[strlen(input) -1]== '\n') {
          input[strlen(input) -1] = 0;
        }


  if (strcmp(input, "quit") == 0 || strcmp(input, "q") == 0) {
            printf("Thanks for playing...\n");
             break;
         }
    if (strcmp(input, "look")== 0 || strcmp(input, "l")== 0){
              printf("h");
             }

         if (strcmp(input, "get") == 0 || strcmp(input, "g") == 0) {
         int objIndex = findbyId(rooms[currentRoom].object);

    if (objIndex != -1) {

        if (inventoryCount < MAX_INVENTORY_SIZE) {
           
             int objectID = rooms[currentRoom].object; 
           
              if (objectID >= 0 && objectID < 10000) { 
                  inventory[inventoryCount] = objectID;
                 inventoryCount++;
                printf("You picked up %s\n", objects[objIndex].name);
                  rooms[currentRoom].object = -1;
            } else {
                  printf("Invalid object ID found in the room.\n");
              }
          } else {
             printf("Your inventory is full.\n");
         }
      } else {
         printf("There's nothing to pick up here.\n");
     }
  }


          if (strcmp(input, "drop") == 0 || strcmp(input, "d") == 0) {
    if (inventoryCount == 0) {
        printf("Nothing in your backpack to drop\n");
    } else {
        printf("Inventory:\n");
        for (int i = 0; i < inventoryCount; ++i) {
            if (inventory[i] >= 0 && inventory[i] < MAX_INVENTORY_SIZE) {
                int objIndex = inventory[i];
                if (objects[objIndex].id != -1 && objects[objIndex].name != NULL) {
                    printf("%d. %s\n", i + 1, objects[objIndex].name);
                }
            }
        }
        printf("Type the ID or name of the item you want to drop ");
        int dropID;
        scanf("%d", &dropID);

        getchar();

        if (dropID >= 1 && dropID <= inventoryCount) {
            int dropIndex = dropID - 1;
            int dropItemID = inventory[dropIndex];
            printf("You dropped %s.\n", objects[dropItemID].name);

            rooms[currentRoom].object = objects[dropItemID].id;

            for (int i = dropIndex; i < inventoryCount - 1; ++i) {
                inventory[i] = inventory[i + 1];
            }
            inventoryCount--;
        } else {
            printf("Invalid item ID.\n");
        }

     
        showRoom(currentRoom);
    }
}


                  

  if (strcmp(input, "inventory") == 0 || strcmp(input, "i") == 0) {
   if (inventoryCount == 0) {
       printf("nothing in your backpack\n");
     } else {
       printf("Inventory:\n");
        for (int i = 0; i < inventoryCount; ++i) {
            if (inventory[i] >= 0 && inventory[i] < MAX_INVENTORY_SIZE) {
            int objIndex = inventory[i];
               if (objects[objIndex].id != -1 && objects[objIndex].name != NULL) {
                   printf("%s\n", objects[objIndex].name);
               }
            }
       }
     }
 }
                                     
       
         if (strcmp(input, "north")== 0 || strcmp(input, "n")== 0){
             if(rooms[currentRoom].north != -1){
                  currentRoom = rooms[currentRoom].north;
             }
    
         }
           if (strcmp(input, "south")== 0 || strcmp(input, "s")== 0){
             if(rooms[currentRoom].south != -1){
                  currentRoom = rooms[currentRoom].south;
             }
    
         }
        if (strcmp(input, "west")== 0 || strcmp(input, "w")== 0){
             if(rooms[currentRoom].west != -1){
                  currentRoom = rooms[currentRoom].west;
             }
    
         }

      if (strcmp(input, "east")== 0 || strcmp(input, "e")== 0){
             if(rooms[currentRoom].east != -1){
                  currentRoom = rooms[currentRoom].east;
             }
    
         }
      }
      }


int main()
{
  
  int *inventory = malloc(sizeof(int) * 1000000);
   rooms = malloc(sizeof(Room) * 10000);
 objects = malloc(sizeof(Object) * 10000);
   for (int i = 0; i < 10000; ++i) {
        objects[i].id = -1;
        objects[i].name = NULL;
        objects[i].description = NULL;
    }
    
    
    if (loadData()) {
        gameLoop(rooms, objects);
   }else{
     printf("Failed to load data. Exiting\n");
    return 99;
   }

  
free(inventory);
   free(rooms);
   free(objects); 
   return 0;
}

