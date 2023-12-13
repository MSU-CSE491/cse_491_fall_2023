#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <faiss/IndexFlat.h>
#include <memory>

// Written by Milan Mihailovic with the assistance of ChatGPT

std::vector<std::vector<float>> load_vectors(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<float>> vectors;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::vector<float> vec;
        float value;
        while (ss >> value) {
            vec.push_back(value);
            if (ss.peek() == ',') ss.ignore();
        }
        vectors.push_back(vec);
    }
    return vectors;
}

// Function to load sentences from the file into a vector
std::vector<std::string> load_sentences(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> sentences;
    while (getline(file, line)) {
        sentences.push_back(line);
    }
    return sentences;
}

int main() {
    std::vector<std::vector<float>> vectors = load_vectors("/mnt/c/Users/mihai/Desktop/CSE 491/blogpost/src/vectors.csv");
    int d = 300; // 300 dimensions
    int nb = vectors.size(); // number of vectors: 200

    // Converting vectors to a single flat array
    std::unique_ptr<float[]> data(new float[nb * d]);
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < d; j++) {
            data[i * d + j] = vectors[i][j];
        }
    }

    // Creates a FAISS index for L2 (Euclidean) distance
    faiss::IndexFlatL2 index(d);

    // Adds vectors to the index
    index.add(nb, data.get());

    float query_vector[d];

    int some_index = 9;  // the index in the sentences.txt that will be searched against
    for (int i = 0; i < d; i++) {
        query_vector[i] = vectors[some_index][i];
    }

    int k = 10; // number of nearest neighbors you want
    std::unique_ptr<float[]> distances(new float[k]);
    std::unique_ptr<int64_t[]> indices(new int64_t[k]);

    index.search(1, query_vector, k, distances.get(), indices.get());

    std::vector<std::string> sentences = load_sentences("/mnt/c/Users/mihai/Desktop/CSE 491/blogpost/src/sentences.txt");

    for (int i = 0; i < 10; i++) {  // Top 10 results
        std::cout << "Index: " << indices[i] << " Distance: " << distances[i]
                  << " Sentence: " << sentences[indices[i]] << std::endl;
    }

}