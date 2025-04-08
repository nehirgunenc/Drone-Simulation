#include "Queue.h"
#include <iostream>



// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here
    front = -1;
    rear = -1;
    count = 0;
}



// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province  
    
    if (count == MAX_QUEUE_SIZE) {  
        std::cerr << "Error: Queue is full. Can not enqueue\n";
        return;
    }

    rear = (rear + 1) % MAX_QUEUE_SIZE;  // Circular increase
    data[rear] = province;
    count++;

    if (front == -1) {  
        front = 0;
    }

}


// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here
    if (isEmpty()) {  
        std::cerr << "Error: Queue is empty. Can not dequeue\n";
        return -1;
    }

    int removedValue = data[front];
    front = (front + 1) % MAX_QUEUE_SIZE;  // Circular increase
    count--;

    if (count == 0) {  
        front = -1;
        rear = -1;
    }

    
    return removedValue;
}


// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    if(!isEmpty()){
        return data[front];
    }
    
    return -1;
    
}


// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    if(count==0){
        return true;
    }
    return false;
}


// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if (count == MAX_QUEUE_SIZE) {  
        std::cerr << "Error: Queue is full. Cannot enqueue priority item.\n";
        return;
    }

    
    if (isEmpty()) {
        enqueue(province);
        return;
    }

    // Add priority element to the beginning of the circular queue
    front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    data[front] = province;
    count++;
    
}
