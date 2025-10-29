#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <cassert>


class SinkHoleGraph{
   const long long INF = std::numeric_limits<long long>::max();
   private:

   struct Edge{int to; long long capacity; int reverse_edge_pos; };
   
   std::vector<std::vector<Edge>> adj_;
   std::vector<int> visited_;
   
   int n_;
   long long max_flow_ = 0;
   
   /*The source of the flow.*/
   int s_ = -1;
   
   /* The hole of the graph.*/
   int t_ = -1;
   
   /** Updates n and resizes graph if needed if vertex_number is greater than n
    * @param vertex_number The vertex that is being inserted in the graph.
    * @return true if there was an update
    */
   bool adjust_graph_size(int vertex_number){
      bool updated = false;
      if(vertex_number > n_){
         n_ = vertex_number;
         updated = true;
      }
      if(vertex_number >= adj_.size() || vertex_number >= visited_.size()){
         adj_.resize(std::max(adj_.size()*2, 1UL*vertex_number));
         visited_.resize(adj_.size());
         updated = true;
      }
      return updated;
   }

   
   
   /** Recursive function that saturate the first edge with capacity, and updates the reverse_edges.
    * @
    * @return how much flow actually went to sink
    */
   int saturate_a_path(int s, long long receiving_flow){
      if(s == t_){
         max_flow_ += receiving_flow;
         return receiving_flow;
      }
      visited_[s] = true;
      
      for(auto& e: adj_[s]){
         auto&[v, capacity, reverse_edge_pos] = e;
         
         if(visited_[v] || capacity == 0) continue;

         int sending_flow = std::min(capacity, receiving_flow);
         int flowed_to_sink = saturate_a_path(v, sending_flow);
         
         if(flowed_to_sink == 0) continue;
         
         capacity -= flowed_to_sink;
         assert(capacity >= 0);
         adj_[v][reverse_edge_pos].capacity += flowed_to_sink;
         
         visited_[s] = false;
         return flowed_to_sink;
      }
      
      visited_[s] = false;
      return 0;
   }
   public:   
   
   SinkHoleGraph(){
      adj_.resize(100);
      visited_.resize(100, false);
   }
   
   void insert_edge(int u, int v, int capacity){
      int maxn = std::max(u, v);
      adjust_graph_size(maxn);
      adj_[u].push_back((SinkHoleGraph::Edge){v, capacity, (int)adj_[v].size()});
      adj_[v].push_back((SinkHoleGraph::Edge){u, 0, (int)adj_[u].size()-1});
   }

   void set_source(int v){s_ = v;}
   void set_sink(int v){t_ = v;}

   int get_source(){return s_;}
   int get_sink(){return t_;}

   long long calculate_max_flow(){
      if(s_ == -1 || t_ == -1 || s_ >= adj_.size() || t_ >= adj_.size()){
         throw std::runtime_error("Defina 'source' e 'sink' antes de calcular a capacidade máxima.");
      }

      while(saturate_a_path(s_, INF));

      return max_flow_;
   }
};

int main(){
   SinkHoleGraph graph;
   int inp;
   
   // std::cout << "Digite o vértice fonte: ";
   std::cin >> inp;
   graph.set_source(inp);
   
   // std::cout << "Digite o vértice ralo: ";
   std::cin >> inp;
   graph.set_sink(inp);
   
   int u, v, c;
   while(std::cin >> u >> v >> c){
      graph.insert_edge(u, v, c);
   }

   std::cout << "Fluxo máximo: " << graph.calculate_max_flow() << '\n';

   return 0;
}