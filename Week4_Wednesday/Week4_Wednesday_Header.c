#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "Data.bin"
#pragma warning (disable:4996)

typedef struct s_item{
	int key;
	int val;
	struct s_item* next;
	struct s_item* prev;
}t_Item;
//sample for creating instance of typedef vs struct
struct s_item s;
t_Item s1;

typedef struct {
	int version;
	int serialNum;
	int  ItemsCount;
	char reserve[100];
}t_headerFile;

t_Item* NewItem(int, int);

t_Item *Head = NULL, *Tail = NULL;

int main1()
{
	int num;
	scanf("%d", &num);
	t_Item* item = (t_Item*)malloc(sizeof(t_Item) * num);
	//create 10 items
	for (int i = 0; i < 10; i++)
	{
		NewItem((i+1) * 5, (i+1));
	}
	t_headerFile fileHeader;
	fileHeader.version = 1;
	fileHeader.ItemsCount = 10;
	fileHeader.serialNum = 12345;

	//save in file
	FILE* f = fopen(FILE_NAME, "w");
	if (!f)
	{
		printf("File error");
		return 1;
	}
	fwrite(&fileHeader, sizeof(t_headerFile), 1, f);
	t_Item* curr = Head;
	while (curr)
	{
		fwrite(curr, sizeof(t_Item), 1, f);
		curr = curr->next;
	}
	fclose(f);
	//free memory
	curr = Head;
	while (curr)
	{
		curr = curr->next;
		free(Head);
		Head = curr;
	}

	//read from file
	f = fopen(FILE_NAME, "r");
	if (!f)
	{
		printf("File error");
		return 1;
	}
	t_headerFile headerOfFile;
	int read = fread(&headerOfFile, sizeof(t_headerFile), 1, f);
	if (read == 0)
	{
		printf("File is empty");
		return 1;
	}
	//build the list
	Head = Tail = NULL;

	for (int i = 0; i < headerOfFile.ItemsCount; i++)
	{
		t_Item* curr = (t_Item*)malloc(sizeof(t_Item));
		read = fread(curr, sizeof(t_Item), 1, f);
		NewItem(curr->val, curr->key);
	}
	//free memory
	curr = Head;
	while (curr)
	{
		curr = curr->next;
		free(Head);
		Head = curr;
	}
	fclose(f);
	return 0;
}

t_Item* NewItem(int val, int key)
{
	t_Item* ret = (t_Item*)malloc(sizeof(t_Item));
	ret->key = key;
	ret->val = val;
	ret->next = NULL;
	ret->prev = NULL;

	if (!Head)
	{
		Head = ret;
		Tail = ret;
	}
	else {
		ret->prev = Tail;
		Tail->next = ret;
		Tail = ret;
	}
	return ret;
}