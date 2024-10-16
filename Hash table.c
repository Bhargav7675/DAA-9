#include <stdio.h>
#include <stdlib.h>

// Node for doubly linked list
typedef struct Node {
    int key;
    int value;
    struct Node* prev;
    struct Node* next;
} Node;

// Hash table
typedef struct HashTable {
    Node** table;
    int size;
    int count;
} HashTable;

// Function to create a new node
Node* createNode(int key, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Hash function using multiplication method
int hashFunction(int key, int size) {
    float A = 0.6180339887; // (sqrt(5) - 1) / 2
    return (int)(size * ((key * A) - (int)(key * A)));
}

// Create hash table
HashTable* createHashTable(int initialSize) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = initialSize;
    ht->count = 0;
    ht->table = (Node**)calloc(ht->size, sizeof(Node*));
    return ht;
}

// Insert key-value pair
void insert(HashTable* ht, int key, int value) {
    int index = hashFunction(key, ht->size);

    Node* newNode = createNode(key, value);
    if (ht->table[index] == NULL) {
        ht->table[index] = newNode;
    } else {
        Node* head = ht->table[index];
        while (head->next != NULL) {
            head = head->next;
        }
        head->next = newNode;
        newNode->prev = head;
    }
    ht->count++;

    // Resize if necessary
    if (ht->count > ht->size) {
        // Double the size
        int newSize = ht->size * 2;
        Node** newTable = (Node**)calloc(newSize, sizeof(Node*));
        
        // Rehashing
        for (int i = 0; i < ht->size; i++) {
            Node* current = ht->table[i];
            while (current != NULL) {
                Node* next = current->next; // Store next node
                int newIndex = hashFunction(current->key, newSize);
                
                current->next = newTable[newIndex]; // Insert into new table
                newTable[newIndex] = current;
                current->prev = NULL; // Reset prev to NULL for new table
                current = next; // Move to the next node
            }
        }
        free(ht->table);
        ht->table = newTable;
        ht->size = newSize;
    }
}

// Remove key
void removeKey(HashTable* ht, int key) {
    int index = hashFunction(key, ht->size);
    Node* current = ht->table[index];
    
    while (current != NULL) {
        if (current->key == key) {
            // Remove node from linked list
            if (current->prev) current->prev->next = current->next;
            if (current->next) current->next->prev = current->prev;
            if (current == ht->table[index]) ht->table[index] = current->next; // Update head if needed
            
            free(current);
            ht->count--;

            // Shrink if necessary
            if (ht->count < ht->size / 4 && ht->size > 1) {
                int newSize = ht->size / 2;
                Node** newTable = (Node**)calloc(newSize, sizeof(Node*));
                
                // Rehashing
                for (int i = 0; i < ht->size; i++) {
                    Node* curr = ht->table[i];
                    while (curr != NULL) {
                        Node* next = curr->next; // Store next node
                        int newIndex = hashFunction(curr->key, newSize);
                        
                        curr->next = newTable[newIndex]; // Insert into new table
                        newTable[newIndex] = curr;
                        curr->prev = NULL; // Reset prev to NULL for new table
                        curr = next; // Move to the next node
                    }
                }
                free(ht->table);
                ht->table = newTable;
                ht->size = newSize;
            }
            return;
        }
        current = current->next;
    }
}

// Search for a key
int search(HashTable* ht, int key) {
    int index = hashFunction(key, ht->size);
    Node* current = ht->table[index];
    
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return -1; // Key not found
}

// Free hash table
void freeHashTable(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        Node* current = ht->table[i];
        while (current != NULL) {
            Node* toDelete = current;
            current = current->next;
            free(toDelete);
        }
    }
    free(ht->table);
    free(ht);
}

// Main function for testing
int main() {
    HashTable* ht = createHashTable(4);
    
    insert(ht, 1, 10);
    insert(ht, 2, 20);
    insert(ht, 3, 30);
    insert(ht, 4, 40);
    
    printf("Value for key 2: %d\n", search(ht, 2));
    
    removeKey(ht, 2);
    printf("Value for key 2 after removal: %d\n", search(ht, 2));
    
    freeHashTable(ht);
    return 0;
}
