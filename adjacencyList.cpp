#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <queue>
#include<set>
#include<math.h>
#include<list>
#include <unordered_map>
#include <unordered_set>
using namespace std;
#include <vector>

class Movie {


public:

    struct Actor {
    public:
        string name;
        string imdb;
        set<Movie*> movies;
        unordered_map<string, set<Movie*>> actorMap;
        set<string> adjacent;
        vector<string> adjacentV;
        Actor(string name, string imdb) {
            this->name = name;
            this->imdb = imdb;
        }
        void printActor() {
            cout << "\tName: " << name << " IMDB: " << imdb << endl;
            cout << "\tMovies: " << endl;
            for (Movie* i : movies)
                i->printMovie();
        }
        bool operator <(const Movie::Actor &rhs) const {
            return this->movies.size() < rhs.movies.size();
        }

    };
    string IMDB;
    string title;
    string year;
    string genre;
    set<Actor*> actors;
    Movie (string imdb, string title, string year, string gen) {
        IMDB = imdb;
        this->title = title;
        this->year = year;
        this->genre = gen;
    }
    void printMovie() {
        cout << "\t\t\"" << title << "\" (" << year << ") " << endl;
    }
};
/*
class Actor {
public:
    string name;
    string imdb;
    set<Movie*> movies;
    unordered_map<string, set<Movie*>> actorMap;
    set<string> adjacent;
    vector<string> adjacentV;
    Actor(string name, string imdb) {
        this->name = name;
        this->imdb = imdb;
    }
    void printActor() {
        cout << "\tName: " << name << " IMDB: " << imdb << endl;
        cout << "\tMovies: " << endl;
        for (Movie* i : movies)
            i->printMovie();
    }
};
 */

void ReadMovieFile(unordered_map<string, Movie*> *movieMap, string &genreFind) {
    //Obtaining movie data
    string line;
    cout << endl << "...Processing..." << endl;
    ifstream myFile("IMDb movies.csv");
    getline(myFile, line);
    while(getline(myFile, line)) {
        string IMDB, title, null, year, genre, temp;
        stringstream currentStream(line);
        getline(currentStream, IMDB, ',');
        getline(currentStream, title, ',');
        for (int i = 0; i < title.length(); i++) {
            if (title.at(i) == '"') {
                getline(currentStream, temp, '"');
                title = title.substr(1, title.length()) + temp;
                break;
            }
        }
        getline(currentStream, null, ',');
        if (null == "") {
            getline(currentStream, null, ',');
            for (int i = 0; i < null.length(); i++) {
                if (null.at(i) == '"') {
                    getline(currentStream, temp, '"');
                    null = null.substr(1, null.length()) + temp;
                    getline(currentStream, null, ',');
                    break;
                }
            }
        }
        getline(currentStream, year, ',');
        getline(currentStream, null, ',');
        getline(currentStream, genre, ',');
        for (int i = 0; i < genre.length(); i++) {
            if (genre.at(i) == '"') {
                getline(currentStream, temp, '"');
                genre = genre.substr(1, genre.length()) + "," + temp;
                getline(currentStream, temp, ',');
                break;
            }
        }
        string currGenre = "";
        for (int i = 0; i < genre.length(); i++) {
            if (isalpha(genre.at(i)))
                currGenre = currGenre + genre.at(i);
            if (genre.at(i) == ',' || genre.at(i) == '-') {
                if (currGenre == genreFind) {
                    if (movieMap->find(IMDB) == movieMap->end())
                        (*movieMap)[IMDB] = new Movie(IMDB, title, year, genre);
                }
                currGenre = "";
            }

            if (currGenre == genreFind) {
                if (movieMap->find(IMDB) == movieMap->end())
                    (*movieMap)[IMDB] = new Movie(IMDB, title, year, genre);
            }
        }

    }
}

string Genre() {
    vector<string> genres = {"", "Romance", "Crime", "Drama", "Fantasy", "Mystery", "Family", "Sci", "Comedy", "Biography", "History", "Adventure", "War", "Horror", "Action", "Western"};
    string genreFind;
    cout << "Genre options: (please enter the number of your choice)" << endl;
    cout << "1. Romance" << "\t9. Biography" << endl << "2. Crime" << "\t10. History" << endl;
    cout << "3. Drama" << "\t11. Adventure" << endl << "4. Fantasy" << "\t12. War" << endl;
    cout << "5. Mystery" << "\t13. Horror" << endl << "6. Family" << "\t14. Action" << endl;
    cout << "7. Sci-Fi" << "\t15. Western" << endl << "8. Comedy";
    int genreChoice;
    cout << endl << "Your choice: ";
    cin >> genreChoice;
    genreFind = genres[genreChoice];
    return genreFind;
}

void ReadNameFile(unordered_map<string, Movie*> *movieMap, unordered_map<string, Movie::Actor*> *actorMap, unordered_map<string, vector<Movie::Actor *>> *actorNamesforDijkstras) {
    //Obtaining name data
    string line;
    ifstream nameFile("IMDb names.csv");
    getline(nameFile, line);
    while (getline(nameFile, line)) {
        if (line.substr(0, 2) != "nm")
            continue;
        string imdb, name, null;
        stringstream currentStream(line);
        getline(currentStream, imdb, ',');
        getline(currentStream, name, ',');

        /*
        if (actorMap->find(imdb) == actorMap->end()) {
            (*actorMap)[imdb] = new Actor(name, imdb);
        }
         */

        if (actorMap->find(imdb) == actorMap->end()) {
            Movie::Actor *tempActor = new Movie::Actor(name, imdb);
            (*actorMap)[imdb] = tempActor;
            vector<Movie::Actor *> tempVec;
            tempVec.push_back(tempActor);
            (*actorNamesforDijkstras)[name] = tempVec;

            if (actorNamesforDijkstras->find(name) == actorNamesforDijkstras->end()) {
                (*actorNamesforDijkstras)[name] = vector<Movie::Actor *>{tempActor};
            } else {
                (*actorNamesforDijkstras)[name].push_back(tempActor);
            }


        }

    }
    cout << endl << "...Loading Actors..." << endl;
    //Obtaining title data
    ifstream otherFile("IMDb title_principals.csv");
    getline(otherFile, line);
    while (getline(otherFile, line)) {
        string movie, null, name, prof;
        stringstream currentStream(line);
        getline(currentStream, movie, ',');
        getline(currentStream, null, ',');
        getline(currentStream, name, ',');
        getline(currentStream, prof, ',');

        // If the movie is not found, then don't bother putting into the map
        if (movieMap->find(movie) == movieMap->end()) {
            continue;
        }
        else if (actorMap->find(name) != actorMap->end()) {
            if (prof.substr(0, 3) != "act")
                actorMap->erase(name);
            else {
                unordered_map<string, Movie::Actor*>::iterator actorIt = actorMap->find(name);
                unordered_map<string, Movie*>::iterator movieIt = movieMap->find(movie);

                actorIt->second->movies.insert(movieIt->second);
                movieIt->second->actors.insert(actorIt->second);
            }
        }
    }

    set<string> toDelete;

    // This first for loop find any actors that have no movies
    for (auto iter = actorMap->begin(); iter != actorMap->end(); iter++) {
        if (iter->second->movies.size() == 0)
            toDelete.insert(iter->first);
    }

    // This for loop deletes the actors that have no movies
    for (auto iter = toDelete.begin(); iter != toDelete.end(); iter++) {
        (*actorMap)[*iter]->imdb = "ERASED";

        actorMap->erase(*iter);
    }

    for (auto iter = actorMap->begin(); iter != actorMap->end(); iter++) {
        if (iter->second->movies.size() != 0) {
            for (Movie* movie : iter->second->movies) {
                if (movieMap->find(movie->IMDB) != movieMap->end())
                    movie->actors.insert(iter->second);

            }
        }
    }
    cout << "...Processing..." << endl;
    //for each actor, there is a map of actors that are connected to that actor and the movies they share
    for (auto iter = actorMap->begin(); iter != actorMap->end(); iter++) {
        if (iter->second->movies.size() != 0) {
            for (Movie *movie : iter->second->movies) {
                if (movieMap->find(movie->IMDB) != movieMap->end()) {
                    for (Movie::Actor* actor : movie->actors) {
                        iter->second->actorMap[actor->imdb].insert(movie);
                    }
                }
            }
        }
    }
};

class graphAdjacencyList {

public:
    unordered_map<Movie::Actor *, deque<pair<Movie::Actor *, vector<Movie *>>>> adjList;

    graphAdjacencyList(unordered_map<Movie::Actor *, deque<pair<Movie::Actor *, vector<Movie *>>>> &adjList) {

        this->adjList = adjList;

    }

    static unordered_set<Movie::Actor *> bfs(graphAdjacencyList &graph, Movie::Actor *&actor1, Movie::Actor *&actor2) {
        unordered_set<Movie::Actor *> visited;
        queue<Movie::Actor *> actorQueue;
        bool found = false;

        //Push in the first actor into the queue
        actorQueue.push(actor1);

        while (!actorQueue.empty()) {

            Movie::Actor *temp = actorQueue.front();
            actorQueue.pop();


            for (auto it = graph.adjList.find(temp)->second.begin();
                 it != graph.adjList.find(temp)->second.end(); it++) {

                // If the actor we are looking for is that which we are at
                if (it->first == actor2) {
                    visited.insert(it->first);
                    actorQueue.push(it->first);
                    found = true;
                }
                    // If the actor does not yet exist in our visited, then create a new entry for visited
                else if (visited.find(it->first) == visited.end()) {
                    visited.insert(it->first);
                    actorQueue.push(it->first);
                } else {

                }

            }

        }

        if (found == true) {
            cout << "There is a link between the two actors!" << endl;
            return visited;
        } else {
            visited.clear();
            return visited;
        }
    };

    static void djikstras(Movie::Actor *&actor1, Movie::Actor *&actor2, graphAdjacencyList &graph,
                          unordered_set<Movie::Actor *> &pathSet) {

        unordered_map<Movie::Actor *, pair<Movie::Actor *, int>> djikstraMap;
        unordered_set<Movie::Actor *> computed;

        cout << "PathSet length is: " << pathSet.size() << endl;
        // Take the actor1 element in our pathSet, and pushinto our djikstraMap with Actor* set to the current, and int of -1
        djikstraMap[actor1] = pair<Movie::Actor *, int>(actor1, -1);
        computed.insert(actor1);
        pathSet.erase(actor1);

        // Let's first set our map, initializing all the Actor's to 0 and the int to -1
        Movie::Actor *temp = new Movie::Actor("", "");
        for (auto it = pathSet.begin(); it != pathSet.end(); it++) {

            djikstraMap[*it] = pair<Movie::Actor *, int>(temp, -1);

        }

        // Let's reassign the edges of actor1
        pair<Movie::Actor *, vector<Movie *>> nextSmallest = graph.adjList.find(actor1)->second.front();
        for (auto it = graph.adjList.find(actor1)->second.begin();
             it != graph.adjList.find(actor1)->second.end(); it++) {

            if (it->second.size() < nextSmallest.second.size()) {

                nextSmallest = *it;

            }
            djikstraMap[it->first] = pair<Movie::Actor *, int>(actor1, it->second.size());

        }

        while (!pathSet.empty()) {

            cout << "PathSet length is: " << pathSet.size() << endl;
            pair<Movie::Actor *, vector<Movie *>>* loopSmallest = &nextSmallest;

            for (auto it = graph.adjList.find(loopSmallest->first)->second.begin();
                 it != graph.adjList.find(loopSmallest->first)->second.end(); it++) {


                // The element has already been computed, so check if the current length is less than 0, and replace
                if (djikstraMap[it->first].second < 0) {

                    djikstraMap[it->first] = pair<Movie::Actor *, int>(loopSmallest->first, it->second.size());

                    // Check to see if it could be the smallest
                    if (it->second.size() < nextSmallest.second.size()) {

                        nextSmallest = *it;

                    }

                }
                else {

                    // We want to find the actor we are looking for an see if that actor's current best path is better than
                    // the proposed path.

                    // If the current actor's closest node is less than the proposed node, do nothing
                    if (djikstraMap.find(it->first)->second.second <= it->second.size()) {


                    }

                        // Else, update the proposed node with the current node.
                    else {

                        djikstraMap[it->first] = pair<Movie::Actor *, int>(loopSmallest->first, it->second.size());

                    }

                    // Check to see if it could be the smallest
                    if (it->second.size() < nextSmallest.second.size()) {

                        nextSmallest = *it;

                    }

                }

                computed.insert(it->first);
                pathSet.erase(it->first);

                cout << "ERASED: Path Set Length: " << pathSet.size() << endl;

            }

        }

    };

    static Movie::Actor* findOneOfMany(graphAdjacencyList &graph, set<Movie::Actor *> dijkstraSet) {

        Movie::Actor* temp;

        cout << "Here are the top names in the category" << endl;

        int nameCount = 1;
        int input = 0;

        for (auto it = dijkstraSet.begin(); it != next(dijkstraSet.begin(), 10); it++) {
            cout << nameCount <<  ": " << (*it)->name << " has " << (*it)->movies.size() << " movies" << endl;
            nameCount++;
        }

        cout << "Input integer pertaining to actor: " << endl;
        cin >> input;
        while (input < 0 || input > 10)  {

            nameCount = 1;

            for (auto it = dijkstraSet.begin(); it != (next(dijkstraSet.begin(), 10)); it++) {

                cout << "Name: " << (*it)->name << " has " << (*it)->movies.size() << " movies" << endl;
                nameCount++;
            }

            cout << "Input integer pertaining to actor: " << endl;
            cin >> input;

        }

        return (*(next(dijkstraSet.begin(), input-1)));

    }

};


class Graph {
public:
    list<pair<pair<string, string>, set<Movie *>>> edges;
    unordered_map<string, Movie::Actor *> actorNames;

public:
    unordered_map<string, int> vMapper;
    int currV = 0;
    int vertices;

    Graph(int vertices, unordered_map<string, Movie::Actor *> *input) {
        this->actorNames = *input;
        this->vertices = vertices;
    }

    set<string> bfs(string src) {
        set<string> result;
        set<string> visited;
        queue<string> q;
        visited.insert(src);
        q.push(src);

        while (!q.empty()) {
            string u = q.front();
            result.insert(u);
            q.pop();
            set<string> adj = getAdjacent(u);
            for (auto v : adj) {
                if (visited.count(v) == 0) {
                    visited.insert(v);
                    q.push(v);
                }
            }
        }
        return result;
    }

    unordered_map<int, int> dijkstra(string src) {
        set<string> S; //contains vertices for which we have computed shortest distance
        set<string> VS; //contains vertices that still need to be processed;

        set<string> InBFS = bfs(src);
        unordered_map<string, int> vMapper = vMapper;
        unordered_map<int, int> distance;
        unordered_map<int, string> pred;

        for (auto iter = InBFS.begin(); iter != InBFS.end(); iter++) {
            VS.insert(*iter);
            distance[vMapper[*iter]] = 200;
            pred[vMapper[*iter]] = "-1";
        }
        distance[vMapper[src]] = 0;
        while (!VS.empty()) {
            int minDistance = 200;

            string toBeComputed;

            for (auto iter = VS.begin(); iter != VS.end(); iter++) {
                if (distance[vMapper[*iter]] < minDistance) {
                    toBeComputed = *iter;
                    minDistance = distance[vMapper[*iter]];
                }
            }

            S.insert(toBeComputed);
            VS.erase(toBeComputed);

            set<string> adjacent = getAdjacent(toBeComputed);

            if (adjacent.size() != 0) {
                for (auto iter = adjacent.begin(); iter != adjacent.end(); iter++) {
                    int weight = getWeight(toBeComputed, *iter);
                    if (distance[vMapper[*iter]] > (distance[vMapper[toBeComputed]] + weight)) {
                        distance[vMapper[*iter]] = distance[vMapper[toBeComputed]] + weight;
                        pred[vMapper[*iter]] = toBeComputed;
                    }

                }
            }
        }
        return distance;
    }

    void insertEdge(string from, string to, set<Movie *> movies) {
        pair<string, string> pair2(from, to);
        pair<pair<string, string>, set<Movie *>> pair1;
        pair1.first = pair2;
        pair1.second = movies;
        edges.push_back(pair1);
        actorNames[from]->adjacent.insert(to);
        actorNames[to]->adjacent.insert(from);
        if (vMapper.find(from) == vMapper.end()) {
            vMapper[from] = currV;
            currV++;
        }
        if (vMapper.find(to) == vMapper.end()) {
            vMapper[to] = currV;
            currV++;
        }
    }

    int getWeight(string from, string to) {
        return actorNames[from]->actorMap[to].size();
    }

    set<string> &getAdjacent(string vertex) {
        if (actorNames.find(vertex) != actorNames.end()) {
            //cout << actorMap[vertex]->adjacent.size();
            return actorNames[vertex]->adjacent;
        }
        set<string> x;
        return x;
    }

    void printGraph() {
        for (auto iter = edges.begin(); iter != edges.end(); iter++) {
            cout << "From: " << iter->first.first << " To: " << iter->first.second << " #: " << iter->second.size()
                 << endl;
            cout << "Weight: " << getWeight(iter->first.first, iter->first.second) << " ";
            for (auto it : getAdjacent(iter->first.first))
                cout << it << " ";
            for (Movie *x : iter->second) {
                cout << x->IMDB << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    string line;
    unordered_map<string, Movie *> movieMap;
    unordered_map<string, Movie::Actor *> actorNames;
    unordered_map<string, vector<Movie::Actor *>> actorNamesforDijkstras;
    cout << "Welcome to our COP3530 Project featuring a graph of actors we love and admire!" << endl;
    cout << endl;

    int input = -1;
    while (input < 1 || input > 2) {

        cout << "Main Menu:" << endl;
        cout << "\t Please select one of the following options: " << endl;
        cout << "\t 1. Load Data" << endl;
        cout << "\t 2. Exit " << endl;
        cin >> input;

        if (input == 2) {
            cout << "Program exited successfully!" << endl;
            exit(0);
        }

    }
    input = -1;
    while (input < 1 || input > 2) {

        cout << "Please select one of the following implementations: " << endl;
        cout << "\t 1. Adjacency List" << endl;
        cout << "\t 2. Edge List" << endl;
        cin >> input;

    }
    //First process movie file
    string genreFind = Genre();
    ReadMovieFile(&movieMap, genreFind);
    if (genreFind == "Sci") { genreFind = "Sci-Fi"; }
    cout << "...Loading Files..." << endl << endl << "Number of movies loaded in " << genreFind << " genre: "
         << movieMap.size() << endl;
    //then process names
    ReadNameFile(&movieMap, &actorNames, &actorNamesforDijkstras);
    cout << "Number of actors in " << genreFind << " movies: " << actorNames.size() << endl;

    if (input == 1) {
        //go the route of adjacency list
        {

            unordered_map<Movie::Actor *, deque<pair<Movie::Actor *, vector<Movie *>>>> adjList;
            graphAdjacencyList graph(adjList);

            // Contains all the actors in the specified genre's films
            set<Movie::Actor *>  dijkstraSet{};

            // This section of our code creates our adjList

            // For each movie in our myMap
            for (auto it = movieMap.begin(); it != movieMap.end(); it++) {

                int actorCount = 0;

                // For each actor in a specific movie's vector
                for (auto it2 = it->second->actors.begin(); it2 != it->second->actors.end(); it2++) {

                    // Let's pushback that specific actor into our set.
                    dijkstraSet.insert(*it2);

                    // If that actor doesn't exist yet in our adjList, then create an empty list of <string, vector<Movie*>>
                    if (graph.adjList.find(*it2) == graph.adjList.end()) {
                        deque<pair<Movie::Actor *, vector<Movie *>>> tempList;
                        graph.adjList[*it2] = tempList;
                    }

                    for (auto it4 = it->second->actors.begin(); it4 != it->second->actors.end(); it4++) {

                        // If the actor in the movie equals that of the actor we are currently working with, then
                        // continue
                        if (*it2 == *it4) {
                            continue;
                        }
                            // Else, check to see if it4 exists in adjList[it2]
                            // If it does, then pushback the movie on the vector
                            // If it doesn't, then create a new pair with the string of it4, and a vector containing
                            // the movie
                        else {

                            for (auto it5 = graph.adjList[*it2].begin(); it5 != graph.adjList[*it2].end(); it5++) {

                                // If the actor we want to link is already linked to current actor, then pushback
                                // the movie
                                if (*it4 == it5->first) {
                                    it5->second.push_back(it->second);
                                    break;
                                }
                            }

                            // If the actor we want to link is not already linked to current actor, then create a new
                            // pair with the string of the actor and the movie
                            pair<Movie::Actor *, vector<Movie *>> tempPair;
                            vector<Movie *> tempVec;
                            tempVec.push_back(it->second);
                            tempPair.first = (*it4);
                            tempPair.second = tempVec;
                            graph.adjList[*it2].push_back(tempPair);

                        }

                    }

                    actorCount++;

                }

                //cout << it->second->title << ": has " << actorCount << " actors" << endl;

            }

            if (dijkstraSet.size() != graph.adjList.size()) {
                cout << "Set size and AdjList are not the same size" << endl;
                exit(0);
            }

            int ALinput = -1;

            while (ALinput < 0 || ALinput > 3) {

                cout << "Please select the following algorithm" << endl;
                cout << "0. Exit" << endl;
                cout << "1. Two Actors" << endl;
                //cout << "2. Breadth First Search" << endl;
                //cout << "3. Depth First Search" << endl;
                cin >> ALinput;

            }

            if (ALinput == 0) {
                cout << "Program Exited Successfully!" << endl;
                exit(0);
            }

            if (ALinput == 1) {



                //if (actorNames.find(name1) == actorNames.end() || actorNames.find(name2) == actorNames.end()) {
                //   cout << "Actor names not found in actorNames Map" << endl;
                //    exit(0);
                //  }

                // Might have to replace actorNamesforDijkstras with dijkstraSet
                // dijkstaSet = unordered_set<Movie::Actor *> dijkstraSet{}; Contains all the actors in the specified movies genre
                // actorNamesforDijkstras = unordered_map<string, vector<Movie::Actor *>>


                Movie::Actor* Actor1 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);
                Movie::Actor* Actor2 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);

                while (Actor1 == Actor2) {
                    cout << "Same names! Please select two different names" << endl;
                    Actor1 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);
                    Actor2 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);

                }

                cout << "Your two actors are: " << Actor1->name << " and " << Actor2->name << endl;

                unordered_set<Movie::Actor *> pathSet = graphAdjacencyList::bfs(graph, Actor1, Actor2);

                while (pathSet.empty()) {

                    Actor1 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);
                    Actor2 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);

                    while (Actor1 == Actor2) {
                        cout << "Same names! Please select two different names" << endl;
                        Actor1 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);
                        Actor2 = graphAdjacencyList::findOneOfMany(graph, dijkstraSet);

                    }

                    cout << "Your two actors are: " << Actor1->name << " and " << Actor2->name << endl;

                    pathSet = graphAdjacencyList::bfs(graph,Actor1,Actor2);

                }


            }
        }
    }
    if (input == 2) {
        Graph myTentativeGraph(actorNames.size(), &actorNames);
        cout << endl << "Available Actors: ";
        int count = 0;
        vector<string> actorsAvail(actorNames.size() + 1);
        for (auto iter = actorNames.begin(); iter != actorNames.end(); iter++) {
            count++;
            actorsAvail[count] = iter->second->name;
            if (count % 4 == 0)
                cout << endl;
            cout << count << ". ";

            if (count == actorNames.size())
                cout << iter->second->name << endl;
            else
                cout << iter->second->name << ", ";
            for (auto i = iter->second->actorMap.begin(); i != iter->second->actorMap.end(); i++) {
                string from = iter->first;
                string to = i->first;
                if (from != to)
                    myTentativeGraph.insertEdge(from, to, i->second);
            }
        }
        cout << myTentativeGraph.edges.size();
        cout << endl;
        int first, second;
        string actor1 = "";
        string actor2 = "";
        cout << "Please enter the option numbers of two actors you choose from the above list: " << endl;
        cout << "Actor One: ";
        cin >> first;
        cout << "Actor Two: ";
        cin >> second;
        if (first > 0 && first < (actorsAvail.size() + 1)) {
            actor1 = actorsAvail[first];
        }
        if (second > 0 && second < (actorsAvail.size() + 1)) {
            actor2 = actorsAvail[second];
        }
        if (actor1 == "" || actor2 == "") {
            cout << "Invalid input";
        }
        cout << endl;
        cout << "Data on actors selected: " << endl;
        string IMDB1;
        string IMDB2;
        for (auto iter = actorNames.begin(); iter != actorNames.end(); iter++) {
            if (iter->second->name == actor1) {
                iter->second->printActor();
                IMDB1 = iter->first;
            }
            if (iter->second->name == actor2) {
                iter->second->printActor();
                IMDB2 = iter->first;
            }
        }
        unordered_map<int, int> distance = myTentativeGraph.dijkstra(IMDB1);
        unordered_map<string, int> vMap = myTentativeGraph.vMapper;
        set<string> BFS = myTentativeGraph.bfs(IMDB1);
        if (BFS.find(IMDB2) != BFS.end())
            cout << "connection";
        if (distance[vMap[IMDB2]] != 200 && distance[vMap[IMDB2]] != 0) {
            cout << "These two actors are connected";
            cout << "They share " << distance[vMap[IMDB2]];
            if (distance[vMap[IMDB2]] == 1)
                cout << "They share 1 movie" << endl;
            else
                cout << "They share " << distance[vMap[IMDB2]] << " movies" << endl;
        } else {
            cout << "These actors are not connected";
        }
    }


    return 0;
}

