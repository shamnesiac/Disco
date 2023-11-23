/**
 * Group ID - 18
 * Member 1 Name - Anirudh Anand
 * Member 1 BITS ID - 2021B3A70981P
 * Member 2 Name - Arushi Gulati
 * Member 2 BITS ID - 2021B5A71704P
 * Member 3 Name - Jai Bothra
 * Member 3 BITS ID - 2021A7PS0015P
 */

// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Graph
{
    int n;                // Size of the graph
    int **adj;            // Adjacency matrix
    char **station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase
 * and creates the Graph structure from it.
 */
Graph *create_graph(char input_file_path[])
{
    FILE *f = fopen(input_file_path, "r");
    Graph *g = (Graph *)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    // Names of stations
    g->station_names = (char **)malloc(g->n * sizeof(char *));
    for (int i = 0; i < g->n; i++)
    {
        g->station_names[i] = (char *)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    // Adjacency matrix
    g->adj = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        g->adj[i] = calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for (int i = 0; i < edges; i++)
    {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

/**
 * Q.1
 * Return the number of junctions.
 */
int find_junctions(Graph *g)
{
    int noOfJunctions = 0;
    for (int i = 0; i < g->n; i++)
    {
        int noOfConnections = 0;
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j] == 1)
                noOfConnections++;
        }
        if (noOfConnections >= 4) // increments the number of junctions when the number of connections for an element is 4 or more
            noOfJunctions++;
    }
    return noOfJunctions;
}

/**
 * Q.2
 * Return true if the tour specified in the question is possible for this
 * graph, else return false.
 * Parameter SAME_STATION - If it is true, then question 2(a) must be solved.
 * If false, then question 2(b) must be solved.
 */
bool sheldons_tour(Graph *g, bool SAME_STATION)
{
    if (SAME_STATION) // tests if bool SAME_STATION is true (2(a))
    {
        for (int i = 0; i < g->n; i++)
        {
            int deg = 0;
            for (int j = 0; j < g->n; j++)
            {
                if (g->adj[i][j])
                    deg++;
            }
            if ((deg % 2) != 0)
                return false;
        }
    }
    else // tests if bool SAME_STATION is false (2(b))
    {
        int oddDegree = 0;
        for (int i = 0; i < g->n; i++)
        {
            int deg = 0;
            for (int j = 0; j < g->n; j++)
            {
                if (g->adj[i][j] == 1)
                    deg++;
            }
            if (deg % 2 != 0)
                oddDegree++;
        }

        if (oddDegree != 2)
            return false;
        else
            return true;
    }
}

/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required.
 */
int **warshall(Graph *g)
{
    // Do not modify or delete pre-filled code!
    int **closure = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        closure[i] = calloc(g->n, sizeof(int));
    }

    // Code goes here
    for (int i = 0; i < g->n; i++) // fills in existing connections
    {

        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j] == 1)
                closure[i][j] = 1;
            else
                closure[i][j] = 0;
        }
    }
    for (int k = 0; k < g->n; k++)
    {
        for (int i = 0; i < g->n; i++)
        {
            for (int j = 0; j < g->n; j++)
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]); // fills in the transitive connections using Warshall's Algorithm
        }
    }

    return closure; // Do not modify
}

/**
 * Q.3
 * Return the number of impossible pairs.
 */
int find_impossible_pairs(Graph *g)
{
    int **closure = warshall(g); // Do not modify
    int noOfcities = 0;
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (closure[i][j] == 0)
            {
                noOfcities++; // increases each impossible pair if the element cannot reach the other one by any means (using the transitive closure)
            }
        }
    }
    return noOfcities;
}

/**
 * Q.4
 * Return the number of vital train tracks.
 */
int find_vital_train_tracks(Graph *g)
{
    int **test = warshall(g);
    int testsum = 0;
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
            testsum = testsum + test[i][j]; // sum of elements in transitive closure of original case
    }
    int essential = 0;
    for (int a = 0; a < g->n; a++)
    {
        for (int b = (g->n - 1); b > a; b--) // this creates a case for each a, b connection being removed (testing)
        {
            if (g->adj[a][b] != 1) // if track doesn't exist in the original graph, ignore this case (no change made)
                continue;
            int **tempadj;     // making adjacent matrix for new case
            int **tempclosure; // making transitive closure for new case
            tempadj = (int **)malloc(g->n * sizeof(int *));
            for (int i = 0; i < g->n; i++)
                tempadj[i] = calloc(g->n, sizeof(int));
            tempclosure = (int **)malloc(g->n * sizeof(int *));
            for (int i = 0; i < g->n; i++)
                tempclosure[i] = calloc(g->n, sizeof(int));
            for (int i = 0; i < g->n; i++) // copying adjacent matrix after removing the track defined by (a,b)
            {
                for (int j = 0; j < g->n; j++)
                {
                    if (((i == a) && (j == b)) || ((i == b) && (j == a))) // removing the track defined by (a,b) (not copying from g->adj)
                        continue;
                    else
                        tempadj[i][j] = g->adj[i][j];
                }
            }
            for (int i = 0; i < g->n; i++) // Warshall's algorithm for new case
            {

                for (int j = 0; j < g->n; j++)
                {
                    if (tempadj[i][j] == 1)
                        tempclosure[i][j] = 1;
                    else
                        tempclosure[i][j] = 0;
                }
            }
            for (int k = 0; k < g->n; k++)
            {
                for (int i = 0; i < g->n; i++)
                {
                    for (int j = 0; j < g->n; j++)
                        tempclosure[i][j] = tempclosure[i][j] || (tempclosure[i][k] && tempclosure[k][j]);
                }
            }
            int tempsum = 0; // calculating the sum of the transitive closure of the new case
            for (int i = 0; i < g->n; i++)
            {
                for (int j = 0; j < g->n; j++)
                    tempsum = tempsum + tempclosure[i][j];
            }
            if (tempsum != testsum) // if there is a difference, the track is essential (add 1 more essential case)
                essential++;
        }
    }
    return essential;
}

/**
 * Q.5
 * Return upgrades, an array of size n.
 * upgrades[i] should be 0 if railway station i gets a restaurant
 * upgrades[i] should be 1 if railway station i gets a maintenance depot
 * If it is not possible to upgrade, then return "-1" in the entire array
 */
int *upgrade_railway_stations(Graph *g)
{
    int *upgrades = calloc(g->n, sizeof(int)); // Do not modify

    // Code goes here
    for (int i = 0; i < g->n; i++) // initializing all upgrades to -1
        upgrades[i] = -1;
    int queue[g->n];               // creating queue used in BFS
    bool visited[g->n];            // creating visited array
    for (int i = 0; i < g->n; i++) // initializing visited array to false
        visited[i] = false;
    int front = 0, rear = 0;
    queue[rear++] = 0;
    visited[0] = true;
    upgrades[0] = 0; // initializing first station upgrade to be 0
    int temp = 0;    // variable to store element to be dequeued
    int count = 1;   // tracking variable for number of visited stations
    while (front != rear)
    {
        temp = queue[front++];
        for (int i = 0; i < g->n; i++)
        {
            if (g->adj[temp][i]) // testing adjacency
            {
                if (!visited[i]) // testing if the station is not visited
                {
                    visited[i] = true;                      // visiting the station
                    queue[rear++] = i;                      // adding the station to the queue
                    upgrades[i] = (upgrades[temp] + 1) % 2; // giving the opposite upgrade of the station being dequeued (current station)
                    count++;                                // incrementing the number of visited stations
                    if (count == g->n)                      // terminating condition (if all stations have been visited)
                        front = rear;
                }
                if (visited[i] && (upgrades[i] == upgrades[temp])) // terminating condition (if impossibility is found)
                    front = rear;
            }
        }
    }
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j] && (upgrades[i] == upgrades[j]))
            {
                for (int k = 0; k < g->n; k++)
                    upgrades[k] = -1; //-1 array made if the optimal upgrade is impossible
                return upgrades;
            }
        }
    }
    return upgrades; // Do not modify
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
 */
int distance(Graph *g, int city_x, int city_y)
{
    if (city_x == city_y) // if both cities are the same, return distance as 0
        return 0;
    bool visited[g->n];            // array of visited stations (using modified BFS to compute distance)
    for (int i = 0; i < g->n; i++) // initially all stations are not visited
        visited[i] = false;
    int queue[g->n];         // creating a priority queue for storing the order of the tree (queue will never be larger than sum of nodes)
    int front = 0, rear = 0; // implementing the queue using front and rear variable
    visited[city_x] = true;  // source city is start of the tree
    queue[rear++] = city_x;  // queue the starting city at the beginning and move rear
    int distance = 1;        // since we will now queue the first connections, establish the distance of any next element as 1 from current
    int temp = city_x;
    int gen = 0, size = 1; // gen tracks the number of elements in a generation (distance) and size guides how long the head must travel before new generation
    while (front != rear)
    {
        temp = queue[front++]; // temporary variable takes position at head and head moves by 1 (dequeues front)
        size--;                // since head moves by 1, reduce size to keep track of the distance from the source (keeps track of dequeuing)
        for (int i = 0; i < g->n; i++)
        {
            if (g->adj[temp][i] && (i == city_y))
                return distance;                // if the next element is the destination, return current distance as distance
            if (g->adj[temp][i] && !visited[i]) // if city is not visited
            {
                visited[i] = true; // visit the city
                queue[rear++] = i; // add that city to the priority queue and move rear
                gen++;             // keeps track of the number of elements of new generation queued
            }
        }
        if (size == 0) // if the number of elements in the current generation are all traversed (tracked by the dequeuing of elements)
        {
            size = gen; // next generation size is the number of elements queued by rear
            gen = 0;    // since new generation, we need to once again start from size 0 of the generation
            distance++; // increase distance to reflect new generation of BFS
        }
    }
    return -1; // return -1 if both elements are not connected (distance never found even after the end of the queue)
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
 */
int railway_capital(Graph *g)
{
    int min = 1000000; // value set arbitarily large
    int capital = -1;  // capital undecided hence set to -1
    for (int i = 0; i < g->n; i++)
    {
        int sum = 0;
        for (int j = 0; j < g->n; j++)
            sum = sum + distance(g, i, j);
        if (sum < min) // smallest sum of distances is capital
        {
            capital = i;
            min = sum;
        }
    }
    return capital;
}

/**
 * Helper function for Q.8
 */
bool maharaja_express_tour(Graph *g, int source, int current_city, int previous_city, int *visited)
{
    if (g->adj[previous_city][current_city] == 1) // check if the cities are adjacent
    {
        if (visited[current_city] == 1) // check if current city has been visited
        {
            if (current_city == source) // if it is the source, the condition has been satisfied, so return true, else return false
                return true;
            else
                return false;
        }
        visited[current_city]++; // if not visited, then visit the current city
        for (int j = 0; j < g->n; j++)
        {
            if (j != current_city && j != previous_city && maharaja_express_tour(g, source, j, current_city, visited)) // recursively apply dfs using current city as new previous city for all other indexes
                return true;                                                                                           // if at any point the above conditions are satisfied, a recusrive path of true is taken until the main function returns true, else continue for other cases
        }
    }
    return false; // if the condition is never met, return false to close the recursive path
}

/**
 * Q.8
 * Return true if it is possible to have a Maharaja Express tour
 * starting from source city, else return false.
 */
bool maharaja_express(Graph *g, int source)
{
    int *visited = (int *)malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++)
    {
        visited[i] = 0;
    }
    // Hint: Call the helper function and pass the visited array created here.
    visited[source] = 1;
    for (int i = 0; i < g->n; i++) // using dfs
    {
        if (i != source && maharaja_express_tour(g, source, i, source, visited)) // if the current index is not the source but maharaja express tour is possible, return true
            return true;
    }
    return false; // if all indexes don't work with source, return false
}

int main()
{
    char input_file_path[100] = "testcase_2.txt"; // Can be modified
    Graph *g = create_graph(input_file_path);     // Do not modify

    // Code goes here
    printf("Question 1\n");
    int noOfJunctions = find_junctions(g);
    printf("Number of junctions = %d\n", noOfJunctions);

    printf("\nQuestion 2\n");
    if (!find_impossible_pairs(g))
    {
        if (sheldons_tour(g, true))
            printf("Sheldon's tour (ending in same city as start) = POSSIBLE\n");
        else
            printf("Sheldon's tour (ending in same city as start) = IMPOSSIBLE\n");

        if (sheldons_tour(g, false))
            printf("Sheldon's tour (ending in different city) = POSSIBLE\n");
        else
            printf("Sheldon's tour (ending in different city) = IMPOSSIBLE\n");
    }
    else
        printf("Not evaluated as graph is disconnected\n");

    printf("\nQuestion 3\n");
    int **c = warshall(g);
    printf("Transitive closure = \n");
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
            printf("%d ", c[i][j]);
        printf("\n");
    }
    printf("\n");
    int impossible = find_impossible_pairs(g);
    printf("Impossible to travel city pairs = %d\n", impossible);

    printf("\nQuestion 4\n");
    int vital = find_vital_train_tracks(g);
    printf("Number of vital train tracks = %d\n", vital);

    printf("\nQuestion 5\n");
    if (!find_impossible_pairs(g))
    {
        int *upgrade = upgrade_railway_stations(g);
        printf("Railway Station Upgrades :\n");
        for (int i = 0; i < g->n; i++)
        {
            if (upgrade[i] == -1)
            {
                printf("NOT POSSIBLE\n");
                break;
            }
            else if (upgrade[i] == 0)
                printf("%s = Restaurant\n", g->station_names[i]);
            else
                printf("%s = Maintenance depot\n", g->station_names[i]);
        }
    }
    else
        printf("Not evaluated as graph is disconnected\n");

    printf("\nQuestion 6\n");
    printf("Type in the indexes of the cities you want to calculate the distance between\n");
    int a, b;
    scanf("%d", &a);
    scanf("%d", &b);
    if (a >= g->n || b >= g->n)
        printf("Input out of bounds!\n");
    else
    {
        int dist = distance(g, a, b);
        printf("\nDistance between %s and %s = %d\n", g->station_names[a], g->station_names[b], dist);
    }

    printf("\nQuestion 7\n");
    if (!find_impossible_pairs(g))
    {
        int capital = railway_capital(g);
        printf("Railway capital = %s\n", g->station_names[capital]);
    }
    else
        printf("Not evaluated as graph is disconnected\n");

    printf("\nQuestion 8\n");
    for (int i = 0; i < g->n; i++)
    {
        bool maharaja = maharaja_express(g, i);
        printf("Maharaja Express tour starting from %s = %s\n", g->station_names[i], (maharaja ? "POSSIBLE" : "IMPOSSIBLE"));
    }
    printf("\n");
    return 0;
}