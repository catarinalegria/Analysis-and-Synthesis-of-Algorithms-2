#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <vector>
#include <queue>
#include <cstring>
#include <list>
#include <utility>
#include <sys/time.h>
#include <iomanip>

class Edge{
    public:
        int id=0;
        int from,to;
        int residual=0;
        int capacity;

        Edge(int from, int to, int capacity, int residual){
            this->from = from;
            this->to = to;
            this->capacity = capacity;
            this->residual = residual;
    
        }
};


class Graph{
    public:
        int n_avenues, n_streets;
        int max_flow = 0;
        int sink,source;
        int index = 0;
        
        //used in bfs
        int token = 1;
        std::vector<int> visited;

        std::vector<std::vector<int> > graph;
        std::vector<Edge> edges;
    
        Graph(int n_avenues, int n_streets){
            this->sink = 2*(n_avenues*n_streets)+1;
            this->source=0;

            this->n_avenues = n_avenues;
            this->n_streets = n_streets;

            graph.resize(2*(n_avenues*n_streets)+2);

            for(int i=0; i<2*(n_avenues*n_streets)+1;i++){
                std::vector<int> edges;
                graph.push_back(edges);
            }
            
            //para cada vertice, cria um arco entre vin e vout de capacidade 1
            //vin fica com o id do vertice
            //vout fica com id = id + n_avenues*n_streets

            for(int i=1; i<=n_avenues*n_streets;i++)
                addEdge(i,i+n_avenues*n_streets);

        }

        void addPath(int from, int to){
            //como as ruas têm dois sentidos, teremos dois arcos por rua:
                //1-(from out,to in)
                //2-(to out, from in)

            //1-edge(from out, to in)
                //edge de from out para to in
                int id1 =index++;
                int id2=index++;

                Edge e1(from + n_avenues*n_streets ,to,1,id2);
                e1.id=id1;
                edges.push_back(e1);

                //edge residual 
                Edge e2(to,from + n_avenues*n_streets,0,id1);
                e2.id=id2;
                edges.push_back(e2);

                graph[from + n_avenues*n_streets].push_back(e1.id);
                graph[to].push_back(e2.id);

            //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

            //2-edge(to out,from in)
                //edge de to out para from in
                int id3 =index++;
                int id4=index++;

                Edge e3(to+ n_avenues*n_streets ,from,1,id4);
                e3.id=id3;
                edges.push_back(e3);

                //edge residual
                Edge e4(from ,to+ n_avenues*n_streets,0,id3);
                e4.id=id4;
                edges.push_back(e4);

                graph[to + n_avenues*n_streets].push_back(e3.id);
                graph[from].push_back(e4.id);

        }

        void addEdge(int from, int to){
            int id1 =index++;
            int id2=index++;


            Edge e1(from,to,1,id2);
            e1.id=id1;
            edges.push_back(e1);

            Edge e2(to,from,0,id1);
            e2.id=id2;
            edges.push_back(e2);

            graph[from].push_back(e1.id);
            graph[to].push_back(e2.id); 

        }

        void initialize(){

            for(int i = 1; i <= n_avenues; i++) 
                for(int j = 1; j < n_streets; j++)
                    addPath(j+(i-1)*n_streets , j+(i-1)*n_streets+1);

             
            for(int i = 1; i < n_avenues; i++) 
                for(int j = 1; j <= n_streets; j++)
                   addPath(j+(i-1)*n_streets, j+(i-1)*n_streets+n_streets);
        }


        void addSupers(int n_supers){
            int avenue, street;

            for(int i = 0; i < n_supers; i++){
                scanf("%d %d", &avenue, &street);
                //vertice de super out para t 
                addEdge(street+(avenue-1)*n_streets + (n_avenues*n_streets),sink);

            }

        }

        void addCitizens(int n_citizens){
            int avenue, street;

            for(int i = 0; i < n_citizens; i++){
                scanf("%d %d", &avenue, &street);

                //vertice de s para citizen in
                addEdge(source,street+(avenue-1)*n_streets);
            }

        }


        bool dfs(int node) {
  
            // At sink node, return augmented path flow.
            if (node == sink) return true;

            visited[node] = token;

            for (unsigned long int i = 0; i<graph[node].size(); i++) 

                if (visited[edges[graph[node][i]].to] != token && edges[graph[node][i]].capacity > 0) {

                    if (dfs(edges[graph[node][i]].to)) {

                            edges[graph[node][i]].capacity = 0;
                            edges[edges[graph[node][i]].residual].capacity = 1;

                            return true;

                        }
                    }
        
        return false;
    }

        void fordFulkerson() { 

           visited.resize(2*n_avenues*n_streets+2); 

           for(int i=0; i<2*n_avenues*n_streets+2; i++)
                visited[i] = -1;

            while (dfs(0)) {
                    token++;
                    max_flow ++;
                }    
        } 

        void printNumber(){
            printf("%d\n", max_flow);
        }


};

int main() {
    int n_avenues, n_streets;
    int n_supers, n_citizens;
    struct timeval start, end; 

    gettimeofday(&start, NULL); 

    scanf("%d %d", &n_avenues, &n_streets);

    Graph g(n_avenues, n_streets);

    g.initialize();

    scanf("%d %d", &n_supers, &n_citizens);

    g.addSupers(n_supers);
    g.addCitizens(n_citizens);


    g.fordFulkerson();

    g.printNumber();

    gettimeofday(&end, NULL);

    double time_taken; 
  
    time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    time_taken = (time_taken + (end.tv_usec -  
                              start.tv_usec)) * 1e-6; 
  
    std::cout << "Time taken by program is : " << std::fixed 
         << time_taken << std::setprecision(6); 
    std::cout << " sec" << std::endl;

    return 0;
}
