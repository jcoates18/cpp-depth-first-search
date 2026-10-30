/*
 * Project 14 - Depth First Search
 *
 * Name: Jourdyn Coates
 * Date: August 5, 2026
 */
#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

class GraphNode
{
private:
    int pointNum;
    int topNum;
    int dist;
    int path;
    bool visited;

    vector<GraphNode*> adjacentNodes;

public:
    GraphNode()
    {
        pointNum = 0;
        topNum = 0;
        dist = -1;
        path = -1;
        visited = false;
    }

    GraphNode(int point)
    {
        pointNum = point;
        topNum = 0;
        dist = -1;
        path = -1;
        visited = false;
    }

    void setPointNum(int point)
    {
        pointNum = point;
    }

    int getPointNum() const
    {
        return pointNum;
    }

    void setTopNum(int number)
    {
        topNum = number;
    }

    int getTopNum() const
    {
        return topNum;
    }

    void setDist(int distance)
    {
        dist = distance;
    }

    int getDist() const
    {
        return dist;
    }

    void setPath(int previousPoint)
    {
        path = previousPoint;
    }

    int getPath() const
    {
        return path;
    }

    void setVisited(bool value)
    {
        visited = value;
    }

    bool getVisited() const
    {
        return visited;
    }

    void addAdjacentNode(GraphNode* node)
    {
        adjacentNodes.push_back(node);
    }

    const vector<GraphNode*>& getAdjacentNodes() const
    {
        return adjacentNodes;
    }
};

class Graph
{
private:
    vector<GraphNode> graphNodes;

    void dfsVisit(GraphNode* currentNode)
    {
        currentNode->setVisited(true);

        cout << "Visited point: "
            << currentNode->getPointNum()
            << endl;

        const vector<GraphNode*>& adjacent =
            currentNode->getAdjacentNodes();

        for (GraphNode* nextNode : adjacent)
        {
            if (!nextNode->getVisited())
            {
                dfsVisit(nextNode);
            }
        }
    }

public:

    Graph(const vector<vector<int>>& connectivityMatrix)
    {
        int numberOfNodes =
            static_cast<int>(connectivityMatrix.size());

        graphNodes.resize(numberOfNodes);

        for (int i = 0; i < numberOfNodes; i++)
        {
            graphNodes[i].setPointNum(i + 1);
        }

        for (int row = 0; row < numberOfNodes; row++)
        {
            for (int column = 0;
                column < numberOfNodes;
                column++)
            {
                if (connectivityMatrix[row][column] == 1)
                {
                    graphNodes[row].addAdjacentNode(
                        &graphNodes[column]);
                }
            }
        }
    }

    int getNumberOfNodes() const
    {
        return static_cast<int>(graphNodes.size());
    }

    void displayGraph() const
    {
        cout << "\nGraph Adjacency List\n";
        cout << "--------------------\n";

        for (const GraphNode& node : graphNodes)
        {
            cout << "Point "
                << node.getPointNum()
                << " connects to: ";

            const vector<GraphNode*>& adjacent =
                node.getAdjacentNodes();

            if (adjacent.empty())
            {
                cout << "none";
            }
            else
            {
                for (const GraphNode* connectedNode : adjacent)
                {
                    cout << connectedNode->getPointNum()
                        << " ";
                }
            }

            cout << endl;
        }
    }

    void resetVisited()
    {
        for (GraphNode& node : graphNodes)
        {
            node.setVisited(false);
        }
    }

    void dfs(int startingPoint)
    {
        if (startingPoint < 1 ||
            startingPoint > getNumberOfNodes())
        {
            cout << "Invalid starting point." << endl;
            return;
        }

        resetVisited();

        cout << "\nDepth First Search beginning at point "
            << startingPoint
            << ":\n";

        dfsVisit(&graphNodes[startingPoint - 1]);
    }

    void unweighted(int startingPoint)
    {
        if (startingPoint < 1 ||
            startingPoint > getNumberOfNodes())
        {
            cout << "Invalid starting point." << endl;
            return;
        }

        for (GraphNode& node : graphNodes)
        {
            node.setDist(-1);
            node.setPath(-1);
        }

        queue<GraphNode*> nodeQueue;

        GraphNode* startingNode =
            &graphNodes[startingPoint - 1];

        startingNode->setDist(0);
        nodeQueue.push(startingNode);

        while (!nodeQueue.empty())
        {
            GraphNode* currentNode = nodeQueue.front();
            nodeQueue.pop();

            for (GraphNode* adjacentNode :
                currentNode->getAdjacentNodes())
            {
                if (adjacentNode->getDist() == -1)
                {
                    adjacentNode->setDist(
                        currentNode->getDist() + 1);

                    adjacentNode->setPath(
                        currentNode->getPointNum());

                    nodeQueue.push(adjacentNode);
                }
            }
        }

        cout << "\nUnweighted Shortest Paths From Point "
            << startingPoint
            << "\n";

        cout << "----------------------------------------\n";
        cout << "Point\tDistance\tPrevious Point\n";

        for (const GraphNode& node : graphNodes)
        {
            cout << node.getPointNum() << "\t";

            if (node.getDist() == -1)
            {
                cout << "Not reachable\t";
            }
            else
            {
                cout << node.getDist() << "\t\t";
            }

            if (node.getPath() == -1)
            {
                cout << "None";
            }
            else
            {
                cout << node.getPath();
            }

            cout << endl;
        }
    }

    void topologicalSort()
    {
        int numberOfNodes = getNumberOfNodes();

        vector<int> indegree(numberOfNodes, 0);
        queue<GraphNode*> nodeQueue;

        for (const GraphNode& node : graphNodes)
        {
            for (const GraphNode* adjacentNode :
                node.getAdjacentNodes())
            {
                int adjacentIndex =
                    adjacentNode->getPointNum() - 1;

                indegree[adjacentIndex]++;
            }
        }

        for (int i = 0; i < numberOfNodes; i++)
        {
            if (indegree[i] == 0)
            {
                nodeQueue.push(&graphNodes[i]);
            }
        }

        int topologicalNumber = 0;

        while (!nodeQueue.empty())
        {
            GraphNode* currentNode = nodeQueue.front();
            nodeQueue.pop();

            topologicalNumber++;

            currentNode->setTopNum(topologicalNumber);

            for (GraphNode* adjacentNode :
                currentNode->getAdjacentNodes())
            {
                int adjacentIndex =
                    adjacentNode->getPointNum() - 1;

                indegree[adjacentIndex]--;

                if (indegree[adjacentIndex] == 0)
                {
                    nodeQueue.push(adjacentNode);
                }
            }
        }

        if (topologicalNumber != numberOfNodes)
        {
            cout << "\nThe graph contains a cycle.\n";
            cout << "A topological sort cannot be completed.\n";
            return;
        }

        cout << "\nTopological Sort\n";
        cout << "----------------\n";
        cout << "Point\tTopological Number\n";

        for (const GraphNode& node : graphNodes)
        {
            cout << node.getPointNum()
                << "\t"
                << node.getTopNum()
                << endl;
        }
    }
};

int getPositiveInteger(const string& prompt)
{
    int value;

    while (true)
    {
        cout << prompt;

        if (cin >> value && value > 0)
        {
            return value;
        }

        cout << "Invalid input. Enter a positive whole number.\n";

        cin.clear();
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n');
    }
}

int getBinaryValue(int row, int column)
{
    int value;

    while (true)
    {
        cout << "Enter connectivity from point "
            << row
            << " to point "
            << column
            << " (1 = edge, 0 = no edge): ";

        if (cin >> value &&
            (value == 0 || value == 1))
        {
            return value;
        }

        cout << "Invalid input. Enter only 0 or 1.\n";

        cin.clear();
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n');
    }
}

int getStartingPoint(int numberOfNodes)
{
    int startingPoint;

    while (true)
    {
        cout << "Enter the starting point (1-"
            << numberOfNodes
            << "): ";

        if (cin >> startingPoint &&
            startingPoint >= 1 &&
            startingPoint <= numberOfNodes)
        {
            return startingPoint;
        }

        cout << "Invalid starting point.\n";

        cin.clear();
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n');
    }
}

int main()
{
    cout << "Graph Application\n";
    cout << "=================\n\n";

    int numberOfNodes =
        getPositiveInteger("Enter the number of nodes: ");

    vector<vector<int>> connectivityMatrix(
        numberOfNodes,
        vector<int>(numberOfNodes));

    cout << "\nEnter the connectivity matrix information.\n";
    cout << "Enter 1 if an edge exists or 0 if no edge exists.\n";
    cout << "Edges are directed.\n\n";

    for (int row = 0; row < numberOfNodes; row++)
    {
        for (int column = 0;
            column < numberOfNodes;
            column++)
        {
            connectivityMatrix[row][column] =
                getBinaryValue(row + 1, column + 1);
        }
    }

    Graph graph(connectivityMatrix);

    graph.displayGraph();

    int menuChoice = 0;

    do
    {
        cout << "\nGraph Menu\n";
        cout << "----------\n";
        cout << "1. Display graph\n";
        cout << "2. Perform topological sort\n";
        cout << "3. Perform unweighted shortest path\n";
        cout << "4. Perform depth first search\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> menuChoice))
        {
            cout << "Invalid input. Enter a number from 1 to 5.\n";

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n');

            continue;
        }

        switch (menuChoice)
        {
        case 1:
            graph.displayGraph();
            break;

        case 2:
            graph.topologicalSort();
            break;

        case 3:
        {
            int startingPoint =
                getStartingPoint(numberOfNodes);

            graph.unweighted(startingPoint);
            break;
        }

        case 4:
        {
            int startingPoint =
                getStartingPoint(numberOfNodes);

            graph.dfs(startingPoint);
            break;
        }

        case 5:
            cout << "Program ended." << endl;
            break;

        default:
            cout << "Invalid choice. Enter a number from 1 to 5.\n";
        }

    } while (menuChoice != 5);

    return 0;
}