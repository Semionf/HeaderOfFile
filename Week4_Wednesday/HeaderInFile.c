#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)

#define VERSION 1

typedef struct Item {
	int key;
	int val;
	struct Item* next;
	struct Item* prev;
}Item;

typedef struct fileHeader {
	int version;
	int serialNum;
	int  ItemsCount;
	char reserve[100];
}fileHeader;

struct Item* add(int, int);
Item* search(int);
void update(int, int);
void saveInFile();
void deleteNode(int);
void load();
void freeList();
void printList();

Item* head = NULL, *tail = NULL;
fileHeader header;

int main()
{
	header.ItemsCount = 0;
	header.version = VERSION;
	header.serialNum = 1111;
	char ch = 0;
	int key, value, num;
	while (ch != 9)
	{
		printf("Press 'A' for Add, 'F' for search, 'U' for update, 'S' for save in file, 'D' for delete, 'L' for Load, 'P' for Print List:");
		num = scanf(" %c", &ch);
		switch (ch)
		{
		case 'A':
			printf("Enter Key and Value: ");
			num = scanf("%d%d",&key, &value);
			add(key, value);
			break;
		case 'F':
			printf("Enter Key to search for Value: ");
			num = scanf("%d", &key);
			Item* node = search(key);
			printf("The value is: %d\n", node->val);
			break;
		case 'U':
			printf("Enter Key and value to update value for key: ");
			num = scanf("%d%d", &key, &value);
			update(key, value);
			break;
		case 'S':
			saveInFile();
			freeList();
			header.ItemsCount = 0;
			break;
		case 'D':
			printf("Enter Key to delete from List: ");
			num = scanf("%d", &key);
			deleteNode(key);
			header.ItemsCount--;
			break;
		case 'L':
			load();
			break;
		case 'P':
			printList();
		case 9:
			break;
		default:
			printf("Wrong input, please try again.\n");
			break;
		}
	}
	freeList();
}

struct Item* add(int key, int val)
{
	Item* curr = head;
	while (curr)
	{
		if (curr->key == key)
		{
			update(key, val);
			return head;
		}
	}
	Item* newNode = (Item*)malloc(sizeof(Item));
	if (!newNode)
	{
		printf("Allocation Error");
		exit(1);
	}
	newNode->next = newNode->prev = NULL;
	newNode->key = key;
	newNode->val = val;
	header.ItemsCount++;
	if (!head)
	{
		head = tail = newNode;
	}
	else {
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
	return newNode;
}

Item* search(int key) {
	Item* curr = head;
	while (curr)
	{
		if (curr->key == key)
		{
			return curr;
		}
		curr = curr->next;
	}
	printf("***.\n");
	return NULL;
}

void update(int key, int value) {
	Item* curr = head;
	while (curr)
	{
		if (curr->key == key)
		{
			curr->val = value;
			printf("The Key has updated for the value: %d\n", value);
			return;
		}
		curr = curr->next;
	}
	printf("No such Key.\n");
}

void saveInFile()
{
	char fileName[100];
	printf("Enter file name: ");
	scanf("%s", fileName);
	if (!head)
	{
		printf("The List is empty \n");
		return;
	}
	FILE* f = fopen(fileName, "w");
	Item* curr = head;
	if (!f)
	{
		printf("File Error");
		exit(1);
	}

	fwrite(&header, sizeof(fileHeader), 1, f);
	for (int i = 0; i < header.ItemsCount; i++)
	{
		fwrite(curr, sizeof(Item), 1, f);
		curr = curr->next;
	}
	fclose(f);
}

void deleteNode(int key) {
	Item* curr = search(key);
	while (curr)
	{
		if (head->key == key && tail->key == key)
		{
			free(curr);
			head = tail = NULL;
			return;
		}
		if (head->key == key)
		{
			head = head->next;
			head->prev = NULL;
			free(curr);
			return;
		}
		if (tail->key == key)
		{
			tail = tail->prev;
			tail->next = NULL;
			free(curr);
			return;
		}
		if (curr->key == key)
		{
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			free(curr);
			return;
		}
		curr = curr->next;
	}
	printf("No such key.\n");
	return;
}

void load()
{
	char fileName[100];
	printf("Enter file name: ");
	scanf("%s", fileName);
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		printf("File Error");
		exit(1);
	}
	fread(&header, sizeof(fileHeader), 1, f);
	if (!header.ItemsCount)
	{
		printf("File is empty.\n");
		return;
	}
	for (int i = 0; i < header.ItemsCount; i++)
	{
		Item* curr = (Item*)malloc(sizeof(Item));
		if (!curr)
		{
			printf("Allocation Error");
			exit(1);
		}
		fread(curr, sizeof(Item), 1, f);
		add(curr->key, curr->val);
	}
	fclose(f);
	printf("Linked list was uploaded.\n");
}

void freeList()
{
	Item* curr = head;
	while (head)
	{
		curr = head;
		head = head->next;
		free(curr);
	}
}

void printList()
{
	if (!head)
	{
		printf("The List is empty \n");
		return;
	}
	printf("The List is: \n");
	Item* curr = head;
	while (curr)
	{
		printf("The key is %d and Value is %d\n", curr->key, curr->val);
		curr = curr->next;
	}
}