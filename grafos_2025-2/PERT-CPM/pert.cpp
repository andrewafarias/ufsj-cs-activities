#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>

using namespace std;
using ll = long long;


class PertCpm{
   private:
      struct Edge{
         int v;
         int w;
      };
      
      struct Vertex{
         ll min_time;
         ll max_time;
         int indegree;
         int outdegree;
         
         vector<Edge> foward;
         vector<Edge> backward;
      };
      
      struct Graph{
         vector<Vertex> vertex;
         int max_vertex;
         int edges_amt;
      };
   
      const int INF = numeric_limits<int>::max();
      
      Graph graph_;
   
      bool calculations_done_ = false;


   public:
      PertCpm(){};
      PertCpm(int max_vertex_index, int edges_amt){
         graph_.max_vertex = max_vertex_index;
         graph_.vertex.resize(graph_.max_vertex+1, {0, INF, 0, 0});

         fill_graph(max_vertex_index, edges_amt);
      }

      int get_max_vertex(){
         return graph_.max_vertex;
      }

      int get_edges_amt(){
         return graph_.edges_amt;
      }
      
      void fill_graph(int max_vertex_index, int edges_to_receive){
         if(calculations_done_){
            throw logic_error("It is not possible to modify the graph after PERT/CPM calculation is done."
                              "Please reset the object or create a new one.");
         }

         if(max_vertex_index >= graph_.max_vertex){
            graph_.max_vertex = max_vertex_index;
            graph_.vertex.resize(graph_.max_vertex+1);
         }

         for(int i = 0; i < edges_to_receive; i++){
            int v, u, w;
            cin >> u >> v >> w;
            graph_.vertex[u].foward.push_back({v, w});
            graph_.vertex[v].backward.push_back({u, w});
            graph_.vertex[u].outdegree++;
            graph_.vertex[v].indegree++;

            graph_.edges_amt++;
         }
      }

      void process_pert_cpm(){
         if(graph_.vertex.size() == 0){
            throw logic_error("It is not possible process PERT/CPM calculations without a graph");
         } else if(calculations_done_){
            return;
         }

         queue<int> finished_events;
         ll max_min_time = 0;
         
         for(int i = 1; i <= graph_.max_vertex; i++)
            if(graph_.vertex[i].indegree == 0)
               finished_events.push(i);
         
         while(finished_events.size()){
            int u = finished_events.front(); finished_events.pop();
         
            for(auto& e: graph_.vertex[u].foward){
               int v = e.v, time = e.w;
         
               graph_.vertex[v].min_time = max(graph_.vertex[v].min_time, graph_.vertex[u].min_time + time);
               max_min_time = max(max_min_time, graph_.vertex[v].min_time);
               graph_.vertex[v].indegree--;
               if(graph_.vertex[v].indegree <= 0){
                  finished_events.push(v);
               }
            }
         }
         
         queue<int> processed_vertexs;
         for(int i = 1; i <= graph_.max_vertex; i++){
            if(graph_.vertex[i].outdegree == 0){
               graph_.vertex[i].max_time = max_min_time;
               processed_vertexs.push(i);
            }
         }
         
         while(processed_vertexs.size()){
            int u = processed_vertexs.front(); processed_vertexs.pop();
         
            for(auto e: graph_.vertex[u].backward){
               int v = e.v, time = e.w;
               graph_.vertex[v].max_time = min(graph_.vertex[v].max_time, graph_.vertex[u].max_time - time);
         
               graph_.vertex[v].outdegree--;
               if(graph_.vertex[v].outdegree <= 0){
                  processed_vertexs.push(v);
               }
            }
         }

         calculations_done_ = true;
      }
      
      void show_results(){
         if(!calculations_done_){
            throw logic_error("PERT/CPM calculations have not been done so results can be shown.");
         }

         cout << "============== Estado dos vértices:\n";
         for(int i = 1; i < graph_.vertex.size(); i++){
            cout << "Vértice: " << i << ", tempo mínimo: " << graph_.vertex[i].min_time << ", folga: " << graph_.vertex[i].max_time - graph_.vertex[i].min_time << '\n';
         }
         
         cout << "\n============== Caminho crítico:\n";
         for(int i = 1; i <= graph_.vertex.size(); i++){
            if(graph_.vertex[i].min_time == graph_.vertex[i].max_time){
               cout << "Vértice: " << i << ", tempo mínimo: " << graph_.vertex[i].min_time << ", folga: " << graph_.vertex[i].max_time - graph_.vertex[i].min_time << '\n';
            }
         }
      }

         
};


int main(){

   int n, m;
   cin >> n >> m;

   PertCpm p(n, m);
   p.process_pert_cpm();
   p.show_results();

   return 0;
}