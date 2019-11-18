//Compiling Instrc : g++ -Wall -std=c++0x <File name>.cpp
//Include this header file in the circularbuffer cpp file

//Complexity : O(n)
//As Acquire is of O(n) and the functions called inside is also O(n) but they are all called sequentially.

#pragma once
#include<iostream>
 
using namespace std;

// Please design and implement the following class. 
// Please describe the complexity of your solution.

// CircularBufferManager manages a circular buffer.

class CircularBufferManager
{
private:
        // Each node will have the information about each element of circular data
        // Circular manager will be a linked list 
        // If used then 1 and if free it is 0
	struct node{ 
		bool used;
		int size;
		node *next;
		int priority;
	};
	void *push(int size, int p); // Insert node in Linked list 
	void pop(node*, node*);  // Delete node in Linked list
	void Merge();   // Merge linked list if two successive nodes are unused
	void *fit(int size);  // First fit algorithem
	void display();  //Display the linked list
	void split(node* , int); // Split if the size is more then it is required
	int memused;  //Total memory used
	int cbsize;   //Total memory

    void *ptr;  //Pointer to circular buffer
    node *head = nullptr;  //head pointer of Linked list
    node *tail = nullptr;  //Tail pointer of Linked List
    void (*revoke)(void* pb);  //Pointer to revoke function
    
public:
	// Constructor of the class.
	// Parameters:
	// pb: pointer to the circular buffer allocated by the caller.
	// cb: size in bytes of said circular buffer.
	// revoke: a callback which Acquire() may call when it has to revoke an allocated chunk of memory.
	CircularBufferManager(void* pb, int size, void (*revoke)(void* pb));

	// Acquire() allocates the requested chunck of memory. It should return the allocated memory or nullptr.
	// Allocation strategy is first-fit.
	//
	// Bonus: If there isn't sufficient free memory, then Acquire() is to revoke memory chunks by calling revoke().
	// Acquire may revoke only memory chuncks with lower priority than the current request priority.
	//
	// Parameters:
	// size: The requested size.
	// priority: priority of the request.
	void* Acquire(int size, int priority);

	// Free() is called to free allocated memory. Free() must protect itself from corrupts requests, i.e. nonexistent or bad pb.
	bool Free(void* pb);
};
// Parameters:
// pb: pointer to the circular buffer allocated by the caller.
// cb: size in bytes of said circular buffer.
// revoke: a callback which Acquire() may call when it has to revoke an allocated chunk of memory.
CircularBufferManager::CircularBufferManager(void* pb, int size, void (*revoke)(void* pb)){
    ptr = pb;  //pointer to circular buffer
    cbsize = size; //Circular buffer size
    this->revoke = revoke; // pointing to call back function
    memused = 0;  // Initialized used memory 
}
//Split() called when we need less memory then what the node has
// Parameters:
// block: pointer to the circular buffer manager Linked list node after which one more node will be created with the extra size
// size: size in bytes which needs to be allocated to the new node
void CircularBufferManager::split(node *block, int size){
    cout<<"In the split"<<endl;
    node *temp, *newnode;
    temp = block->next;
    newnode = (node*)malloc(sizeof(node));
    newnode->size = size;
    newnode->next = temp;
    newnode->used = 0;
    block->next = newnode;
    if(newnode->next == NULL)   // If the new node is allocated in the end then tail will be updated
        tail = newnode;
}
// Push() is called when we need to enter new node in the linked list 
// Parameters:
// p : priority of the block
// size: size in bytes which needs to be allocated to the new node

void *CircularBufferManager::push(int size, int p){
    cout<<"I am in push"<<endl;
    node* temp;
    int im = memused;
    if(memused + size <= cbsize){ //Check if size is less then the size of the circular buffer
       memused = size + memused;
       temp  = (node*)malloc(sizeof(node));
       temp->size = size;
       temp->used = 1;  
       temp->priority = p;
       if(head == NULL){
	       head = temp;
	       tail = temp;
       }
       else {
	       tail->next = temp;
	       tail = temp;
	       tail->next = nullptr;
       }
       return((void*)((static_cast<char*>(ptr)) + im)); //return the address to the circular buffer where value can be allocated

    }
    return nullptr; // If no space is ledt then return null pointer
}

// Pop() will delete the next linked list by pointing the nect of current block to the next of the next node
// Parameters:
// curr_blk : pointer to the curent block
// next_blk: pointer to next block which will be removed
void CircularBufferManager::pop(node *curr_blk, node *next_blk){
    curr_blk->next = next_blk->next;
    free(next_blk);
}

//Fit() Implements first fit algorithm 
//Parameters:
//size: number of bytes you need to allocate 
void* CircularBufferManager::fit(int size){
    cout<< "I am in fit"<<endl;
    node *block = head;
    int mem;
    while(block != nullptr){
        if((block->size >= size) && (block->used == 0)){// Look for the block size which is unused and size is greater then you need
        	//found the block
            if(block->size > size){ // If size is greater
        	split(block, block->size - size);// then split it
                block->size = size;
            }
            block->used = 1;
            return((void*)((static_cast<char*>(ptr)) + mem )); // return the pointer to the cb
        }
        else{
        	//Did not found the block
            mem = block->size + mem;
            block = block->next;
        }
    }
    return nullptr;// If nothing available then return NULL
}

//Merge is called when two consecutive block are unused they are merged in one
void CircularBufferManager::Merge(){
	cout << "inside merge" << endl;
	node *curr_blk, *next_blk;
	curr_blk = head;
	next_blk = head->next;
	while(next_blk != nullptr){
        if((curr_blk->used == 0) && (next_blk->used == 0)){
         	curr_blk->size = curr_blk->size + next_blk->size; 
         	pop(curr_blk, next_blk);

        }
        curr_blk = next_blk;
       	next_blk = next_blk->next;
    }
}

// Display the linked list of the circular buffer manager
void CircularBufferManager::display(){
	cout<< "In Display"<<endl;
	node* block;
	block = head;
	int s_no = 0;
	while(block != nullptr){
		cout<<"Node number: "<< s_no<<"\t";
		cout<< "Used: "<< block->used<<"\t";
        cout<<"Priority: "<<block->priority<<"\t";
		cout<<"size"<<block->size<<endl;
		block = block->next;
		s_no++;
	}
}

// Acquire() allocates the requested chunck of memory. It should return the allocated memory or nullptr.
// Allocation strategy is first-fit.
//
// Bonus: If there isn't sufficient free memory, then Acquire() is to revoke memory chunks by calling revoke().
// Acquire may revoke only memory chuncks with lower priority than the current request priority.
//
// Parameters:
// size: The requested size.
// priority: priority of the request
void* CircularBufferManager::Acquire(int size, int priority){
    cout << "In acquire" << endl;
    node *block;
    void *b;
    b = fit(size);  // First call First fir algorithm to check if any free space is available
    int flag = 0;
    if (b == nullptr) // If not then see if still the space is available in the circular buffer
        b = push(size, priority);
    //Bonus
    if(b == nullptr){  // If it is full then remove the block with low priority
        block = head;
        while(block !=nullptr && (flag == 0)){
            if(block->priority < priority){ //If found the block with lower priority
                if(block->size > size){ // If the size is greater then what is needed
                    split( block, block->size - size);
                    block->size = size;
                    revoke(ptr);
                    b = (void*)ptr;
                    flag = 1;
                }
                else if(block->size == size){ // If size is exact
                    block->priority = priority;
                	revoke(ptr); 
                	b = (void*)ptr;
                    flag = 1;
                }
                else{ // If the size is less 
                    block = block->next;
                    b = nullptr;
                }
            }
            else{
                block = block->next; // Shift to next block
            } 
        }
            Merge(); 
    }
    //display(); //for debug : will display the cb manager
    if(b == nullptr){
        cout << "Cannot allocate it"<< endl;
    }
    return(b);

}

// Free() is called to free allocated memory. Free() must protect itself from corrupts requests, i.e. nonexistent or bad pb.

bool CircularBufferManager::Free(void* pb){
    cout << "In Free" << endl;
    node *curr, *prev;
    int mem = 0;
    curr = head;
    prev = NULL;

    while(curr != NULL){
        if((void*)((static_cast<char*>(ptr)) + mem ) == pb){
            if(curr == head){ // If only one node in the linked list
                tail = nullptr;
                head = nullptr;
                memused = memused - curr->size;
                free(curr);
                display();
                return true;
            }
            prev->next = curr->next;
            curr->next = NULL;
            curr->used = 0;
            tail = prev;
            memused = memused - curr->size;
            cout << memused << endl;
            free(curr);
            Merge();
            //display(); //For debug
            return true;
        }
        else{
            prev = curr;
            mem = mem + curr->size;
            curr = curr->next;
        }
        
        
    }
    cout<< "Bad request";
    return false;

}

