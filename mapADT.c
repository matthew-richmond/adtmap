/*
* @author Matthew Richmond
* Implementation of mapADT header file
*
*/

#include <stdlib.h>
#include <stdbool.h>
typedef struct mapelm {
        void* key;
        void* value;
}*element; //works smoother when this is a pointer for some reason.....

typedef struct mapADT {
        int size;
        void* compfunc;
        element *data;
}*MapADT; //unsure why it's to be made a pointer, but it was done in the header file.


#define _MAP_IMPL_
#include "mapADT.h"

/*
* Function which creates an empty map, and then returns it.
*/
MapADT map_create(bool (*equals)(const void* a, const void* b)){
	MapADT map;
	map = (MapADT) malloc(sizeof(MapADT));
	map->size = 0;
	map->compfunc = equals;
	return map;
}

//Function that rewinds the map to the first index.
static void  map_rewind(MapADT map){
        for(int i = 0; i<map->size; i++){
                map->data--;
        }
}
//function that frees the space in memory that the map and all its contents contain.
void map_destroy(MapADT map){
	int elms = map->size;
	for(int i = 0; i<elms; i++){
		element elm = *map->data;
		free(elm);
	}
	free(map->data);
	free(map);
}
//frees the memory of the contents of the map.
void map_clear(MapADT map){
	int elms = map->size;
	for(int i = 0; i<elms; i++){
		element elm = *map->data;
		free(elm);
	}
	map->size = 0;
}


//Tests if the map contains a given key
bool map_contains(MapADT map, void* key){
	int elms = map->size;
	bool res = false;
	bool (*equals) (const void* a, void* b);
	equals = map->compfunc;
	for(int i = 0; i<elms; i++){
		if(equals(key,(*map->data)->key)){
				res = true;
			}
		map->data ++;
	}
	map_rewind(map);
	return res;
}

//Puts a key value pair in the map, checking if the key is already in the map
// and appends the value of that key if this is true;
void* map_put(MapADT map, void *key, void *value){
	bool (*equals) (const void* a, void* b);
	equals = map->compfunc;
	element nelm;
	nelm = malloc(sizeof(element));
	nelm->key = key;
	nelm->value = value;
	if(map->size == 0){
		map->data = malloc(sizeof(element));
		map->data = &nelm;
		return value;
		map->size = map->size+1;
	}
	else if(map_contains(map, nelm->key)){
		void* exists = NULL;
		for(int i=0; i<map->size; i++){
			if(equals((*map->data)->key,key)){
				exists = &((*map->data)->value);
				(*map->data)->value = value;
				free(equals);
			}
			map->data ++;
		}
		map_rewind(map);
		return exists;
	}
	else if(map->size > 0){
		element* newdat = (element*) realloc(map->data, sizeof(map->data) + sizeof(element));
		for(int i = 0; i<map->size; i++){
			newdat = map->data;
			newdat++;
			map->data++;
		}		
		newdat++;
		newdat = &nelm;
		free(map->data);
		map->data = newdat;
		map->size = map->size +1;
		map_rewind(map);
		return value;
	}
	free(equals);
	return value;
}

//retrieves the value of a key from the map
const void* map_get(MapADT map, void *key){
	int elms = map->size;
	const void* val;
	bool (*equals) (const void* a, void* b);
	equals = map->compfunc;
	for(int i = 0; i<elms; i++){
		if(equals(key,(*map->data)->key)){
			val =  (*map->data)->value;
		}
		map->data++;
	}
	map_rewind(map);
	return val;
}

//deletes a key/value pair from the map.

void* map_delete(MapADT map, void *key){
	int elms = map->size;
	void *delval = NULL;
	bool (*equals) (const void* a, void* b);
	equals = map->compfunc;
	if(!map_contains(map,key)){
		return NULL;
	}
	for(int i = 0; i<elms; i++){
		if(equals(key,(*map->data)->key)){
			element delelm = *map->data;
			delval = delelm->value;
		}
	}
	element *newdata;
	newdata = malloc(sizeof(map->data) - sizeof(element));
	for(int i =0; i<elms; i++){
		if(!equals(key,(*map->data)->key)){
			newdata = map->data;
			newdata++;
		}
		map->data++;
	}
	map_rewind(map);
	map->size--;
	map->data = realloc(map->data,sizeof(map->data) - sizeof(element));
	map->data = newdata;
	return delval;
}	
//checks if the map is empty.		
bool map_empty(MapADT map){
	if(map->size == 0){return true;}
	else{return false;}
}
