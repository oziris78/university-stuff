
/*
 * Veri Yapilari dersi - Proje
 * Oguzhan Topaloglu, C19052025
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// -------------------------  STRUCTS  ------------------------- //


typedef struct node {
    char vertex;            // node's name as a letter such as 'A' or 'K'
    int weight;             // cost/length of the edge
    struct node* next;      // next node of the node linkedlist
} Node;


typedef struct graph {
    int vertexCount;        // how many nodes are there? => 3, 4, ...
    Node** adjLists;        // (A, D), (A, B), (B, D), (C, D)
} Graph;


typedef struct polygon {
    int totalLength;        // sum of all weights
    int cornerCount;        // 3-gon => cornerCount = 3 etc
    char* path;             // "A, D, B, E" etc.
    struct polygon* next;   // next node of the polygon linkedlist
} Polygon;


// -------------------------  FUNC DEFINITIONS  ------------------------- //


// Functions that are sequentially called from main()
Graph* newGraph(const char*);
Polygon* determinePolygons(Graph*);
void organizePolygons(Polygon*);
void printResults(Graph*, Polygon*);
void freeEverything(Graph*, Polygon*);

// Helper functions that are called from other functions
void DFS(Graph*, Polygon**, char, char, char*, int*, int, int);
void insertToPolygonLinkedlist(Polygon**, Polygon*);
Polygon* newPolygon(char*, int, int);
char* cpyStr1End(const char*, int, int);
int compareChar(const void*, const void*);


// -------------------------  FUNCS  ------------------------- //


/**
 * @brief Creates a new graph based on the given txt
 * 
 * @param path The file path containing the graph data.
 * @return Graph* A pointer to the newly created graph.
 */
Graph* newGraph(const char* path) {
    // Open the file for reading
    FILE *file = fopen(path, "r");
    if (file == NULL) exit(-1);

    // Determine the vertex count
    char src, dest, biggestLetter = 'A';
    int i, weight;
    while (fscanf(file, " %c %c %d", &src, &dest, &weight) == 3) {
        if(src > biggestLetter) biggestLetter = src;
        if(dest > biggestLetter) biggestLetter = dest;
    }
    int vertexCount = biggestLetter - 'A' + 1;

    // Initialize the graph
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->vertexCount = vertexCount;
    graph->adjLists = (Node**) malloc(graph->vertexCount * sizeof(Node*));

    for (i = 0; i < graph->vertexCount; i++) {
        graph->adjLists[i] = NULL;
    }

    // Add all the edges
    rewind(file);
    while (fscanf(file, " %c %c %d", &src, &dest, &weight) == 3) {
        // Add one edge from src to dest with weight
        const int srcArrIndex = src - 'A';  // 0 for 'A', 1 for 'B', ...
        Node* n1 = (Node*) malloc(sizeof(Node));
        n1->vertex = dest;
        n1->weight = weight;
        n1->next = graph->adjLists[srcArrIndex];
        graph->adjLists[srcArrIndex] = n1;

        const int destArrIndex = dest - 'A'; 
        Node* n2 = (Node*) malloc(sizeof(Node));
        n2->vertex = src;
        n2->weight = weight;
        n2->next = graph->adjLists[destArrIndex];
        graph->adjLists[destArrIndex] = n2;
    }

    fclose(file);
    return graph;
}



/**
 * @brief Determines all polygons in the graph.
 * 
 * @param graph The graph to analyze.
 * @return Polygon* A pointer to the head of the linked list containing all polygons.
 */
Polygon* determinePolygons(Graph* graph) {
    Polygon* polygon = NULL;

    int i, path_index = 0;
    char* path = (char*) calloc(graph->vertexCount, sizeof(char));
    int* visited = (int*) calloc(graph->vertexCount, sizeof(int));

    for (i = 0; i < graph->vertexCount; i++) {
        // Convert vertex index to ascii char
        char c = 65 + i;
        // Perform DFS from each vertex
        DFS(graph, &polygon, c, c, path, visited, 0, 0);
        // Reset visited status for next DFS
        visited[i] = 0;
    }

    free(visited);
    free(path);

    return polygon;
}



/**
 * @brief Organizes the polygons within the linked list in a specific order.
 * 
 * This function sorts the polygons alphabetically based on their paths (ABCD and ADCB etc) 
 * and eliminates duplicate polygons from the list.
 * 
 * @param polygonHead A pointer to the head of the polygon linked list.
 */
void organizePolygons(Polygon* polygonHead) {
    int i;

    // Sort the polygons in alphabetical order
    Polygon* polygon = polygonHead; 
    while(polygon != NULL) {
        // Find the index of the smallest character in the path
        int minIndex = 0;
        for(i = 1; i < polygon->cornerCount; i++) {
            if(polygon->path[i] < polygon->path[minIndex]) {
                minIndex = i;
            }
        }
        
        // If the smallest character is not at the beginning, rotate the path
        if(minIndex != 0) {
            char* newPath = (char*) malloc(polygon->cornerCount * sizeof(char));
            int j = 0;
            for(i = minIndex; i < polygon->cornerCount; i++, j++) {
                newPath[j] = polygon->path[i];
            }
            for(i = 0; i < minIndex; i++, j++) {
                newPath[j] = polygon->path[i];
            }

            free(polygon->path);
            polygon->path = newPath;
        }

        polygon = polygon->next;
    }

    // Rotate AECB into ABCE
    polygon = polygonHead; 
    while(polygon != NULL) {
        int length = polygon->cornerCount;
        // Create a copy of the path excluding the first character
        char* str1 = cpyStr1End(polygon->path, length, 0);  // copy of path[1:end]
        // Create a reversed copy of the path excluding the first character
        char* str2 = cpyStr1End(polygon->path, length, 1);  // reverse of path[1:end]

        // If the reversed copy is alphabetically smaller than the original copy, reverse the path
        if(strcmp(str1, str2) > 0) { // reverse is better
            for(i = 1; i < length; i++) {
                polygon->path[i] = str2[i-1];
            }
            free(str1);
            free(str2);
        }

        polygon = polygon->next;
    }

    // Remove the duplicate polygons
    polygon = polygonHead;
    while(polygon != NULL && polygon->next != NULL) {
        Polygon* tmp = polygon;
        while(tmp->next != NULL) {
            // Check if the next polygon has the same length and corner count as the current polygon
            int sameLen = tmp->next->totalLength == polygon->totalLength;
            int sameCornerCount = tmp->next->cornerCount == polygon->cornerCount;
            
            if(sameLen && sameCornerCount) {
                // Now check if the two paths are also the same
                char* path1 = tmp->next->path;
                char* path2 = polygon->path;
                int len1 = tmp->next->cornerCount;
                int len2 = polygon->cornerCount;

                char* sortedPath1 = (char*) malloc(len1 * sizeof(char));
                char* sortedPath2 = (char*) malloc(len2 * sizeof(char));

                memcpy(sortedPath1, path1, len1);
                memcpy(sortedPath2, path2, len2);

                qsort(sortedPath1, len1, sizeof(char), compareChar);
                qsort(sortedPath2, len2, sizeof(char), compareChar);

                int areSameVertices = (len1 == len2) ? 
                    (memcmp(sortedPath1, sortedPath2, len1) == 0) : 0; 

                free(sortedPath1);
                free(sortedPath2);

                // If they are the same, remove the next polygon
                if(areSameVertices) {
                    Polygon* temp = tmp->next;
                    tmp->next = tmp->next->next;
                    free(temp->path);
                    free(temp);
                }
                else {
                    tmp = tmp->next;
                }
            } 
            else {
                tmp = tmp->next;
            }
        }

        polygon = polygon->next;
    }

}



/**
 * @brief Prints the results of the polygon analysis according to the project's PDF file.
 * 
 * This function calculates and displays the total count of all polygons.
 * It also displays the count of each type of polygon (3-gon, 4-gon, etc.).
 * It also prints details of each polygon, including their vertex counts, paths and total lengths.
 * 
 * @param graph The graph being analyzed.
 * @param polygon A pointer to the head of the polygon linked list.
 */
void printResults(Graph* graph, Polygon* polygon) {
    int i, polygonCount = 0, ngonCountsLen = graph->vertexCount-3+1;
    // Array for storing the counts of each type of polygon
    int* ngonCounts = (int*) calloc(ngonCountsLen, sizeof(int)); // [3gon, 4gon, 5gon, ...]

    // Count the total number of polygons and the number of each type of polygon and print them according to the PDF format
    Polygon* head = polygon;
    while(head != NULL) {
        polygonCount++;
        ngonCounts[head->cornerCount - 3]++;
        head = head->next;
    }
    printf("Sekil sayisi: %d\n", polygonCount);
    for(i = 0; i < ngonCountsLen; i++) {
        printf("%d'gen sayisi: %d\n", i+3, ngonCounts[i]);
    }
    printf("\n");

    // Print the details of each type of polygon (3gon, 4gon, ...)
    int ngonCount;
    for(ngonCount = 3; ngonCount <= graph->vertexCount; ngonCount++) {
        int* printed = calloc(polygonCount, sizeof(int));
        
        int areAllPrinted = 0;
        int leftNum = 1;

        // While there are still polygons of the current type that haven't been printed
        while(areAllPrinted == 0) {
            areAllPrinted = 1;
            int j;
            for(j = 0; j < polygonCount; j++) {
                Polygon* head = polygon;
                int index = 0;
                while(head != NULL) {
                    // If the current polygon is of the current type and hasn't been printed, set areAllPrinted to 0
                    if(head->cornerCount == ngonCount && printed[index] == 0) {
                        areAllPrinted = 0;
                    }
                    head = head->next;
                    index++;
                }
            }
            
            // Try to find the alphabetically smallest, UNPRINTED polygon of the current type
            Polygon* minPoly = NULL;
            int minPolyIndex = 0;

            Polygon* head = polygon;
            int index = 0;
            while(head != NULL) {
                if(head->cornerCount == ngonCount && printed[index] == 0 && (minPoly == NULL || strcmp(head->path, minPoly->path) < 0)) {
                    minPoly = head;
                    minPolyIndex = index;
                }
                head = head->next;
                index++;
            }

            // If such unprinted polygon was found, print it.
            if(minPoly != NULL) {
                printf("%d. %d'gen: ", leftNum++, minPoly->cornerCount);
                for(j = 0; j < minPoly->cornerCount; j++) {
                    printf("%c ", minPoly->path[j]);
                }
                printf("%c ", minPoly->path[0]);
                printf("Uzunluk: %d\n", minPoly->totalLength);
                printed[minPolyIndex] = 1;
            }
        }

        // Free the printed array for this iteration of ngonCounts
        free(printed);
        printf("\n");
    }

    free(ngonCounts);
}



/**
 * @brief Frees everything related to the graph and the polygon linkedlist.
 * 
 * This function deallocates the memory used by the graph and polygon linked list,
 * including nodes and paths.
 * 
 * @param graph A pointer to the graph structure.
 * @param polygons A pointer to the head of the polygon linked list.
 */
void freeEverything(Graph* graph, Polygon* polygons) {
    // Freeing the graph
    int i;
    for(i = 0; i < graph->vertexCount; i++) {
        Node* temp = graph->adjLists[i];
        while(temp != NULL) {
            Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjLists);
    free(graph);

    // Freeing the polygons
    Polygon* tempPoly = polygons;
    while(tempPoly != NULL) {
        Polygon* nextPoly = tempPoly->next;
        free(tempPoly->path);
        free(tempPoly);
        tempPoly = nextPoly;
    }
}


// -------------------------  MAIN  ------------------------- //


int main() {
    const char* filePath = "./Sample2.txt";
    printf("%s icin analiz:\n", filePath);

    Graph* graph = newGraph(filePath);
    Polygon* polygons = determinePolygons(graph);
    organizePolygons(polygons); // sort & remove duplicate paths
    printResults(graph, polygons);
    freeEverything(graph, polygons);

    return 0;
}


// -------------------------  HELPERS  ------------------------- //



/**
 * @brief Performs DFS (depth first search) to find all possible polygons starting from a given vertex.
 * 
 * This function implements DFS to traverse the graph and find all possible polygons 
 * starting from the specified vertex. It constructs polygons with unique vertex paths and calculates 
 * their total lengths. The resulting polygons are added to the polygon linked list.
 * 
 * Please note that this function will add the same polygons in different forms (for example ABEDC 
 * and ACDEB are the same polygons). These duplicate polygons will be removed from the list in the
 * organizePolygons(Polygon*) function.
 * 
 * @param g A pointer to the graph structure.
 * @param p A pointer to the head of the polygon linked list.
 * @param vert The current vertex being visited.
 * @param start The starting vertex of the current DFS traversal.
 * @param path An array storing the current vertex path/sequence.
 * @param visited An array indicating visited vertices.
 * @param pathIndex The index of the current vertex in the path array.
 * @param polyLen The length of the current polygon.
 */
void DFS(Graph* g, Polygon** p, char vert, char start, char* path, int* visited, int pathIndex, int polyLen) {
    int vertexIndex = vert - 'A';

    // Add current vertex to path and mark as visited
    path[pathIndex++] = vert;
    visited[vertexIndex] = 1;

    // Loop through all vertices connected to the current vertex
    Node* temp = g->adjLists[vertexIndex];
    while (temp != NULL) {
        char connectedVertex = temp->vertex;
        char connectedVertexIndex = connectedVertex - 'A';

        // If vertex hasnt been visited, perform DFS from that vertex
        if (visited[connectedVertexIndex] == 0) {
            polyLen += temp->weight;
            DFS(g, p, connectedVertex, start, path, visited, pathIndex, polyLen);
            polyLen -= temp->weight; // sub weight after DFS call to backtrack
        }
        // If we've returned to the start vertex and have more than 2 vertices in path, we've found a polygon (paths with len=2 are just edges, for example ABA, CDC, EAE :P)
        else if (connectedVertex == start && pathIndex > 2) {
            Polygon* polygon = newPolygon(path, pathIndex, polyLen + temp->weight);
            insertToPolygonLinkedlist(p, polygon);
        }

        temp = temp->next;
    }
    
    // Decrement pathIndex and make current vertex unvisited to backtrack
    pathIndex--;
    visited[vertexIndex] = 0;
}



/**
 * @brief Inserts a new polygon into the polygon linkedlist.
 * 
 * @param p A double pointer to the head of the linked list of polygons.
 * @param polygon A pointer to the polygon to be inserted into the linkedlist.
 */
void insertToPolygonLinkedlist(Polygon** p, Polygon* polygon) {
    // If the list is empty, the new polygon becomes the head of the list
    if(*p == NULL) {
        *p = polygon;
    }
    // Otherwise, add the new polygon to the end of the list.
    else {
        Polygon* lastPoly = *p;
        while(lastPoly->next != NULL) {
            lastPoly = lastPoly->next;
        }
        lastPoly->next = polygon;
    }
}



/**
 * @brief Creates a new polygon with the given path, corner count, and total length.
 * 
 * @param path A pointer to a character array representing the path of the polygon.
 * @param cornerCount The number of corners in the polygon.
 * @param totalLength The total length of the polygon.
 * @return Polygon* A pointer to the newly created polygon.
 */
Polygon* newPolygon(char* path, int cornerCount, int totalLength) {
    Polygon* polygon = (Polygon*) malloc(sizeof(Polygon));
    polygon->totalLength = totalLength;
    polygon->cornerCount = cornerCount;

    polygon->path = malloc(polygon->cornerCount * sizeof(char));
    int i;
    for(i = 0; i < polygon->cornerCount; i++) {
        polygon->path[i] = path[i];
    }
    return polygon;
}



/**
 * @brief Copies str[1:end] and returns it. This string will be needed to freed by the user later on.
 * 
 * @param str The input string to be copied.
 * @param length The length of the input string.
 * @param isReversed Flag indicating whether to reverse the copied part.
 * @return char* A pointer to the copied string.
 */
char* cpyStr1End(const char* str, int length, int isReversed) {
    char* string = (char*) malloc(length * sizeof(char));

    int i, index = 0;
    // Add [1, end] range in normal or reversed order into string
    for(i = 1; i < length; i++) {
        int strIndex = isReversed ? length - i : i;
        string[index++] = str[strIndex];
    }
    string[index] = '\0'; // null-terminate the string
    
    return string;
}



/**
 * @brief Compares two characters for sorting purposes.
 * 
 * This function compares two characters for sorting purposes.
 * This function is used in the qsort function to sort an array of chars.
 * 
 * @param a Pointer to the first character.
 * @param b Pointer to the second character.
 * @return int A negative value if the first character is less than the second, 
 *             zero if they are equal, and a positive value otherwise.
 */
int compareChar(const void* a, const void* b) {
    return *(char*)a - *(char*)b;
}


