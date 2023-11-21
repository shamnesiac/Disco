/**
 * Group ID -
 * Member 1 Name -
 * Member 1 BITS ID -
 * Member 2 Name -
 * Member 2 BITS ID -
 * Member 3 Name -
 * Member 3 BITS ID -
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
            {
                noOfConnections++;
            }
        }
        if (noOfConnections >= 4)
        {
            noOfJunctions++;
        }
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
    if (SAME_STATION == true)
    {

        for (int i = 0; i < g->n; i++)
        {
            int deg = 0;
            for (int j = 0; j < g->n; j++)
            {

                if (g->adj[i][j])
                {
                    deg++;

                    if ((deg % 2) != 0)
                    {
                        return false;
                    }
                }
            }
        }
    }
    else if (SAME_STATION == false)
    {

        int oddDegree = 0;
        for (int i = 0; i < g->n; i++)
        {
            int deg = 0;
            for (int j = 0; j < g->n; j++)
            {
                if (g->adj[i][j] == 1)
                {
                    deg++;
                }
            }
            if (deg % 2 != 0)
            {
                oddDegree++;
            }
        }

        if (oddDegree != 2)
        {
            return false;
        }
        else
        {
            return true;
        }
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
    for (int i = 0; i < g->n; i++)
    {

        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j] == 1)
            {
                closure[i][j] = 1;
            }
            else
                closure[i][j] = 0;
        }
    }
    for (int k = 0; k < g->n; k++)
    {
        for (int i = 0; i < g->n; i++)
        {
            for (int j = 0; j < g->n; j++)
            {
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
            }
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
                noOfcities++;
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
            testsum = testsum + test[i][j];
    }
    printf("%d\n", testsum);
    int essential = 0;

    for (int a = 0; a < g->n; a++)
    {
        for (int b = (g->n - 1); b > a; b--)
        {
            int **tempadj;
            int **tempclosure;
            tempadj = (int **)malloc(g->n * sizeof(int *));
            for (int i = 0; i < g->n; i++)
            {
                tempadj[i] = calloc(g->n, sizeof(int));
            }
            tempclosure = (int **)malloc(g->n * sizeof(int *));
            for (int i = 0; i < g->n; i++)
            {
                tempclosure[i] = calloc(g->n, sizeof(int));
            }
            for (int i = 0; i < g->n; i++)
            {
                for (int j = 0; j < g->n; j++)
                {
                    if (((i == a) && (j == b)) || ((i == b) && (j == a)))
                        continue;
                    else
                        tempadj[i][j] = g->adj[i][j];
                }
            }
            for (int i = 0; i < g->n; i++)
            {

                for (int j = 0; j < g->n; j++)
                {
                    if (tempadj[i][j] == 1)
                    {
                        tempclosure[i][j] = 1;
                    }
                    else
                        tempclosure[i][j] = 0;
                }
            }
            for (int k = 0; k < g->n; k++)
            {
                for (int i = 0; i < g->n; i++)
                {
                    for (int j = 0; j < g->n; j++)
                    {
                        tempclosure[i][j] = tempclosure[i][j] || (tempclosure[i][k] && tempclosure[k][j]);
                    }
                }
            }
            int tempsum = 0;
            for (int i = 0; i < g->n; i++)
            {
                for (int j = 0; j < g->n; j++)
                    tempsum = tempsum + tempclosure[i][j];
            }
            printf("%d\n", tempsum);
            if (tempsum != testsum)
                essential++;
        }
    }
    printf("%d\n", essential);
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

    for (int i = 0; i < g->n; i++)
        upgrades[i] = -1;
    upgrades[0] = 1;
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j] == 1)
            {
                if (upgrades[j] == -1)
                {
                    if (upgrades[i] == 0)
                        upgrades[j] = 1;
                    else if (upgrades[i] == 1)
                        upgrades[j] = 0;
                }
                else if (upgrades[i] == upgrades[j])
                {
                    for (int k = 0; k < g->n; k++)
                        upgrades[k] = -1;
                    return upgrades;
                }
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
    if (city_x == city_y)
        return city_x;
    int MAX = 100;
    bool visited[MAX];
    for (int i = 0; i < g->n; i++)
    {
        visited[i] = false;
    }
    int queue[MAX];
    int front = 0, rear = 0;
    visited[city_x] = true;
    queue[rear++] = city_x;
    int distance = 1;
    int temp = city_x;
    int gen = 0, size = 1;
    while (front != rear)
    {
        temp = queue[front++];
        size--;
        for (int i = 0; i < g->n; i++)
        {
            if (g->adj[temp][i] && (i == city_y))
                return distance;
            if (g->adj[temp][i] && !visited[i])
            {
                visited[i] = true;
                queue[rear++] = i;
                gen++;
            }
        }
        if (size == 0)
        {
            size = gen;
            gen = 0;
            distance++;
        }
    }
    return -1;
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
 */
int railway_capital(Graph *g)
{
    int min = __INT_MAX__;
    int capital = -1;
    for (int i = 0; i < g->n; i++)
    {
        int sum = 0;
        for (int j = 0; j < g->n; j++)
            sum = sum + distance(g, i, j);
        if (sum < min)
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
    if (g->adj[previous_city][current_city] == 1)
    {
        if (visited[current_city] == 1)
        {
            if (current_city == source)
                return true;
            else
                return false;
        }
        visited[current_city]++;
        for (int j = 0; j < g->n; j++)
        {
            if (j != current_city && j != previous_city && maharaja_express_tour(g, source, j, current_city, visited))
                return true;
        }
    }

    return false;
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
    for (int i = 0; i < g->n; i++)
    {
        if (i != source && maharaja_express_tour(g, source, i, source, visited))
            return true;
    }
    return false;
}

int main()
{
    char input_file_path[100] = "testcase_3.txt"; // Can be modified
    Graph *g = create_graph(input_file_path);     // Do not modify

    // Code goes here
    printf("Question 1\n");
    int noOfJunctions = find_junctions(g);
    printf("nNumber of junctions = %d\n", noOfJunctions);

    printf("\nQuestion 2\n");
    if (sheldons_tour(g, true))
        printf("Sheldon's tour (ending in same city as start) = possible\n");
    else
        printf("Sheldon's tour (ending in same city as start) = impossible\n");

    if (sheldons_tour(g, false))
        printf("Sheldon's tour (ending in different city) = possible\n");
    else
        printf("Sheldon's tour (ending in different city) = impossible\n");

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
    int *upgrade = upgrade_railway_stations(g);
    printf("Railway Station Upgrades :\n");
    for (int i = 0; i > g->n; i++)
    {
        if (upgrade[i] == -1)
        {
            printf("NOT POSSIBLE\n");
            break;
        }
        else if (upgrade[i] == 1)
            printf("%s = Restaurant\n", g->station_names[i]);
        else
            printf("%s = Maintenance depot\n", g->station_names[i]);
    }

    printf("\nQuestion 6\n");
    printf("Type in the indexes of the cities you want to calculate the distance between\n");
    int a, b;
    scanf("%d", &a);
    scanf("%d", &b);
    int dist = distance(g, a, b);
    printf("\nDistance between %s and %s = %d\n", g->station_names[a], g->station_names[b], dist);

    printf("\nQuestion 7\n");
    int capital = railway_capital(g);
    printf("Railway capital = %s\n", g->station_names[capital]);

    printf("\nQuestion 8\n");
    for (int i = 0; i < g->n; i++)
    {
        bool maharaja = maharaja_express(g, i);
        printf("Maharaja Express tour starting from %s = %s\n", g->station_names[i], (maharaja ? "POSSIBLE" : "IMPOSSIBLE"));
    }
    return 0;
}