#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- PROVIDED CODE, DON'T WORRY ABOUT THIS---

// A node in the linked list
typedef struct node {
  char *artist;
  char *songName;
  char *genre;
  struct node *next;
} Node;

// Prompt the user for a string safely, without buffer overflow
    void inputStringFromUser(char *prompt, char *s, int maxStrLength) {
    int i = 0;
    char c;

    printf("%s --> ", prompt);
    while (i < maxStrLength && (c = getchar()) != '\n')
        s[i++] = c;
    s[i] = '\0';
}

// Function to call when the user is trying to insert a song name that is already in the personal music library.
void songNameDuplicate(char *songName) {
  printf("\nA song with the name '%s' is already in the music library.\n"
         "No new song entered.\n",
         songName);
}

// Function to call when a song name was found in the personal music library.
void songNameFound(char *songName) {
  printf("\nThe song name '%s' was found in the music library.\n", songName);
}

// Function to call when a song name was not found in the personal music library.
void songNameNotFound(char *songName) {
  printf("\nThe song name '%s' was not found in the music library.\n",songName);
}

// Function to call when a song name that is to be deleted was found in the personal music library.
void songNameDeleted(char *songName) {
  printf("\nDeleting a song with name '%s' from the music library.\n",songName);
}

// Function to call when printing an empty music library.
void printMusicLibraryEmpty(void) { 
  printf("\nThe music library is empty.\n"); 
}

// Function to call to print a title when the entire music library is printed.
void printMusicLibraryTitle(void) { 
  printf("\nMy Personal Music Library: \n"); 
}

const int MAX_LENGTH = 1024;

// --- MY FUNCTIONS, ONLY FOCUS ON THIS ---

// Free Node
void freeNode(Node *ptr){
    free(ptr->songName);
    free(ptr->artist);
    free(ptr->genre);
    free(ptr);
}

// Function for inserting linked list (DONE)
Node *insertAlphabetically(Node *head, Node *newNode){
    // Make cur and pre pointers
    Node *cur=head,*pre=NULL;
    // while cur is not null and the first letter of the song in newnode is larger than the current node's song name
    while(cur!=NULL && strcmp(newNode->songName,cur->songName)>0){
        // pre = cur and cur = cur -> next
        pre = cur;
        cur = cur -> next;
    }
    // if cur is not null and newNode --> data == cur --> data 
    if(cur!=NULL && strcmp(newNode->songName,cur->songName)==0){
        // say no duplicate is allowed
        songNameDuplicate(newNode->songName);
        // free new node
        freeNode(newNode);
    }
    else{
        // sandwich new node between pre and cur
        newNode -> next = cur;
        if (pre == NULL) head = newNode;
        else (pre -> next = newNode);
    }
    // Return head 
    return head;
}

// Function for printing songs out (DONE)
void printList(Node *head){
    // Set head equal to pointer
    Node *cur = head;
    // If pointer is equal to null, say music library is empty
    if (cur == NULL){
        printMusicLibraryEmpty();
        return;
    }
    printMusicLibraryTitle();

    // While pointer isn't null
    while (cur != NULL){
        // Print the data of the node
        printf("\n%s\n",cur->songName);
        printf("%s\n",cur->artist);
        printf("%s\n",cur->genre);
        // Set pointer to next node
        cur = cur->next;
    }
}

// Function for searching for song (DONE)
Node *searchSong(Node *head, char *key){
    // If list head is null, return false
    if(head==NULL){
        songNameNotFound(key);
        return NULL;
    }
    // Make current node pointer and set = to head
    Node *cur = head;
    // While current --> name is not equal to key and current is not null
    while(cur!=NULL && strcmp(cur->songName,key)!=0){
        // current = current --> next
        cur = cur ->next;
    }
    // Check if valid
    if(cur == NULL){
        songNameNotFound(key);
        return NULL;
    }
    songNameFound(key);
    // Print information of current
    printf("\n%s\n",cur->songName);
    printf("%s\n",cur->artist);
    printf("%s\n",cur->genre);
    return cur;
}

// Function for finding and deleting a song (DONE)
Node *deleteSong(Node *head, char *key){
    // If list head is null, return false
    if(head == NULL){
        // Song name wasn't found
        songNameNotFound(key);
        return head;
    }
    // Make current node pointer and set = to head
    Node *cur = head;
    Node *pre=NULL;
    // While current --> name is not equal to key and current is not null
    while(cur!=NULL && strcmp(cur->songName,key)!=0){
        pre = cur;
        cur = cur -> next;
    }
    // Check if valid
    if(cur == NULL){
        songNameNotFound(key);
        return head;
    }

    // Make delete statement
    songNameDeleted(cur->songName);

    // Free the node in scenarios

    if(pre==NULL&&cur->next!=NULL){
        // Error here
        Node *temp = head;
        head=cur->next;
        freeNode(temp);
    }
    else if(pre==NULL&&cur->next==NULL){
        // Error here
        Node *temp = head;
        head=cur->next;
        freeNode(temp);
    }  
    else if(pre!=NULL&&cur->next==NULL){
        //pre->next=NULL;
        pre->next = cur->next;
        freeNode(cur);
        //do nothing because it is on tail end of list
    } 
    else if(pre!=NULL&&cur->next!=NULL) {
        pre -> next = cur -> next;
        freeNode(cur);
    }

    return head;
}

// Function for quitting program and feeding memory (DONE)
Node *deleteWholeList(Node *ptr){
    // If pointer is null, return
    if (ptr==NULL) return NULL;
    // Define temp pointer
    Node *temp;
    // While ptr is not null, temp = ptr, ptr to next, free temp
    while(ptr!=NULL){
        temp = ptr;
        ptr = ptr->next;
        songNameDeleted(temp->songName);
        freeNode(temp);
    }
    return ptr;
}

int main(void) {
    // Declare the head of the linked list.
    Node *listHead = NULL;

    // Announce the start of the program
    printf("Personal Music Library.\n\n");
    printf("%s", "Commands are I (insert), D (delete), S (search by song name),\n""P (print), Q (quit).\n");

    char response;
    char input[MAX_LENGTH + 1];
    do {
        inputStringFromUser("\nCommand", input, MAX_LENGTH);
        response = toupper(input[0]);

        if (response == 'I') {
            // Construct a new node
            Node *newNode=(Node*) malloc(sizeof(Node));
            newNode->next=NULL;

            // Prompt user for information and assign to node
            char *promptName = "Song name" ;
            char nameInput[MAX_LENGTH+1];
            inputStringFromUser(promptName,nameInput,MAX_LENGTH+1);
            newNode->songName = (char*)malloc(sizeof(nameInput));
            strcpy(newNode->songName,nameInput);

            char *promptArtist =  "Artist" ;
            char artistInput[MAX_LENGTH+1];
            inputStringFromUser(promptArtist,artistInput,MAX_LENGTH+1);
            newNode->artist = (char*)malloc(sizeof(artistInput));
            strcpy(newNode->artist,artistInput);

            char *promptGenre =  "Genre" ;
            char genreInput[MAX_LENGTH+1];
            inputStringFromUser(promptGenre,genreInput,MAX_LENGTH+1);
            newNode->genre = (char*)malloc(sizeof(genreInput));
            strcpy(newNode->genre,genreInput);

            // Pass node to function to insert alphabetically
            listHead=insertAlphabetically(listHead,newNode);
        }
        else if (response == 'D') {
            // Prompt user for song name
            char key[MAX_LENGTH+1];
            char *prompt = "\nEnter the name of the song to be deleted";
            inputStringFromUser(prompt,key,MAX_LENGTH+1);
            // Call function for deleting song name, pass in head and key
            listHead=deleteSong(listHead,key);

        }
        else if (response == 'S') {
            // Search for a song by its name.
            char key[MAX_LENGTH+1];
            // Prompt user for input
            char *prompt = "\nEnter the name of the song to search for";
            inputStringFromUser(prompt,key,MAX_LENGTH+1);
            // Call search function, pass in head and list
            searchSong(listHead,key);

        }
        else if (response == 'P') {
            // Print linked list by calling function and pass in head of list
            printList(listHead);

        }
        else if (response == 'Q') {
            ;
            // This condition does nothing, only here to make sure the loop stops
        }
        else {
            // do this if no command matched ...
            printf("\nInvalid command.\n");
        }
    } while (response != 'Q');

    // Call function to delete entire list, pass in list head
    listHead=deleteWholeList(listHead);
    // Call print list function to confirm deletion
    printList(listHead);
    return 0;
}
