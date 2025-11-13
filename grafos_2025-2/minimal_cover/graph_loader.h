#ifndef GRAPH_LOADER_H
#define GRAPH_LOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <deque>

//Basically everything is this code is AI made.

// --- Data Structures ---

struct Edge {
    int source;
    int target;
};

struct InternalEdge {
    int target;
    const std::string* label_ptr;
};

// Using a struct for the graph keeps everything organized
struct Graph {
    std::vector<std::vector<InternalEdge>> adj_list;
    std::deque<std::string> road_name_db; 
    std::vector<int> node_degrees;
};

// --- Helper Functions ---

inline int getIntFromLine(const std::string& line) {
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
        try {
            return std::stoi(word);
        } catch (...) {
            continue;
        }
    }
    return -1;
}

// Extracts text found between the first and last quotation marks
inline std::string getStringFromLine(const std::string& line) {
    size_t first_quote = line.find('\"');
    size_t last_quote = line.rfind('\"');

    if (first_quote != std::string::npos && last_quote != std::string::npos && last_quote > first_quote) {
        return line.substr(first_quote + 1, last_quote - first_quote - 1);
    }
    return "";
}

// --- Main Loading Function ---

inline Graph loadGraphData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return {};
    }

    std::string line;
    int max_node_id = -1;
    bool in_node_block = false;

    // --- Pass 1: Find max node ID ---
    while (std::getline(file, line)) {
        if (line.find("node [") != std::string::npos) in_node_block = true;
        else if (line.find("]") != std::string::npos) in_node_block = false;
        
        if (in_node_block && line.find("id ") != std::string::npos) {
            int id = getIntFromLine(line);
            if (id > max_node_id) max_node_id = id;
        }
    }

    int num_nodes = max_node_id + 1;
    if (num_nodes <= 0) return {};

    Graph g;
    g.adj_list.resize(num_nodes);
    g.node_degrees.assign(num_nodes, 0);

    // --- Pass 1.5: Count Edges ---
    file.clear();
    file.seekg(0, std::ios::beg);
    
    int edge_count = 0;
    while (std::getline(file, line)) {
        if (line.find("edge [") != std::string::npos) {
            edge_count++;
        }
    }

    // --- Pass 2: Build Edges with Labels (Names) ---
    file.clear();
    file.seekg(0, std::ios::beg);

    bool in_edge_block = false;
    int current_source = -1;
    int current_target = -1;
    std::string current_name = "";

    while (std::getline(file, line)) {
        if (line.find("edge [") != std::string::npos) {
            in_edge_block = true;
            current_source = -1;
            current_target = -1;
            current_name = "Unknown"; 
            continue;
        }

        if (line.find("]") != std::string::npos && in_edge_block) {
            in_edge_block = false;
            
            if (current_source != -1 && current_target != -1) {
                g.road_name_db.push_back(current_name);
                
                std::string* name_ptr = &g.road_name_db.back();

                g.adj_list[current_source].push_back({current_target, name_ptr});
                g.adj_list[current_target].push_back({current_source, name_ptr});

                g.node_degrees[current_source]++;
                g.node_degrees[current_target]++;
            }
            continue;
        }

        if (in_edge_block) {
            if (line.find("source ") != std::string::npos) {
                current_source = getIntFromLine(line);
            } else if (line.find("target ") != std::string::npos) {
                current_target = getIntFromLine(line);
            } else if (line.find("name") != std::string::npos) { 
                std::string temp_name = getStringFromLine(line);
                if (!temp_name.empty()) {
                    current_name = temp_name;
                }
            }
        }
    }
    
    file.close();
    return g;
}

#endif // GRAPH_LOADER_H