#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "graph_loader.h"

// All this code is human made.

// This function sets has_dependent[v] = true for all v that are in 'vertices'.
void makeHaveDependecies(const std::vector<InternalEdge>& vertices, std::vector<bool>& has_dependent) {
    for(const auto& v : vertices) has_dependent[v.target] = true;
}

void removeRedundantCameras(Graph& my_graph, std::vector<std::pair<int, int>> vertex_order,
                            std::vector<bool>& has_camera, std::vector<bool>& has_dependent){

    //Iterate through all vertices, removing cameras from those of lower degree.
    for(const auto pair : vertex_order) {
        int v = pair.second;
        //If there is a edge depending on this vertice, or he doesn't have a camera, can't remove a camera.
        if(has_dependent[v] || !has_camera[v]) continue;
        has_camera[v] = false;
        makeHaveDependecies(my_graph.adj_list[v], has_dependent);

    }
}

void outputResults(const Graph& my_graph, const std::vector<bool>& has_camera){
    int count = 0;
    for(size_t i = 0; i < has_camera.size(); i++) {
        if(has_camera[i]) {
            count++;
            std::cout << "Vertex " << i << ": ";
            for(size_t j = 0; j < my_graph.adj_list[i].size(); j++) {
                std::cout << *(my_graph.adj_list[i][j].label_ptr) 
                          << (j + 1 >= my_graph.adj_list[i].size() ? "\n" : ", ");
            }
        }
    }
    std::cout << "\nNumber of cameras: " << count << '\n';
}

bool validateMinimalCover(const Graph& my_graph, const std::vector<bool>& has_camera){
        bool is_valid = true;
        for(size_t u = 0; u < my_graph.adj_list.size(); u++) {
            bool all_have_camera = true;
            for(const auto& v : my_graph.adj_list[u]) {
                if(!has_camera[v.target]) {
                    all_have_camera = false;
                    break;
                }
            }
            
            if(has_camera[u]) {
                if (all_have_camera) {
                    std::cout << "This set is not a minimal cover because of a camera in " << u << '\n';
                    is_valid = false;
                }
            } else {
                if (!all_have_camera) {
                    std::cout << "Not all edges are covered in " << u << '\n';
                    is_valid = false;
                }
            }
        }

        return is_valid;
}

int main() {
    std::string filename = "sjdr.gml"; 
   
    // Load the graph using the function from the header
    Graph my_graph = loadGraphData(filename);

    if (my_graph.adj_list.empty()) {
        std::cout << "Graph could not be loaded." << std::endl;
        return 1;
    }

    // --- Logic Start ---

    //Create a decrescent vector based in vertex degree
    std::vector<std::pair<int, int>> vertex_order(my_graph.node_degrees.size());
    for(int i = 0; i < my_graph.node_degrees.size(); i++){
        vertex_order[i] = {my_graph.node_degrees[i], i};
    }
    std::sort(vertex_order.begin(), vertex_order.end());

    std::vector<bool> has_camera(my_graph.adj_list.size(), true);
    std::vector<bool> has_dependent(my_graph.adj_list.size(), false);

    removeRedundantCameras(my_graph, vertex_order, has_camera, has_dependent);

    // Validation
    std::cout << "\nChecking if minimal cover property holds...\n";
    bool is_minimal_cover = validateMinimalCover(my_graph, has_camera);

    if(is_minimal_cover) {
        std::cout << "The current set is a minimal cover.\n\n";
    }else{
        std::cout << "The current set is not a minimal cover.\n\n";
        exit(1);

    }

    outputResults(my_graph, has_camera);
        
    return 0;
}