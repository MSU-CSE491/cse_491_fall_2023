#include <bitset>
#include <cassert>
#include <format>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace cowboys {

    constexpr char HEADER_SEP = ',';
    constexpr char GENOTYPE_SEP = ';';
    constexpr char HEADER_END = ':';

    namespace base64 {
        static std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        static std::string encode(size_t ull) {
            std::string result = "";
            while (ull > 0) {
                result = chars[ull % 64] + result;
                ull /= 64;
            }
            return result;
        }

        static size_t decode(std::string num_base64) {
            size_t result = 0;
            for (size_t i = 0; i < num_base64.size(); ++i) {
                const char ch = num_base64[i];
                const size_t coeff = std::pow(64, num_base64.size() - i - 1);
                result += chars.find(ch) * coeff;
            }
            return result;
        }

        static std::string encode(std::bitset<sizeof(size_t) * 8> bits) {
            std::string result = "";
            for (size_t i = 0; i < bits.size(); i += 6) {
                size_t num = 0;
                for (size_t j = 0; j < 6; j++) {
                    num += bits[i + j] * std::pow(2, 5 - j);
                }
                result += chars[num];
            }
            return result;
        }
    } // namespace base64

    struct CGPNodeGene {
        /// The connections per node. '1' means connected, '0' means not connected.
        std::vector<char> input_connections;

        /// The index of the function the node uses.
        size_t function_idx;
    };

    class CGPGenotype {
      protected:
        /// Number of inputs to the graph.
        size_t num_inputs;
        /// Number of outputs from the graph.
        size_t num_outputs;
        /// Number of middle layers in the graph.
        size_t num_layers;
        /// Number of nodes per middle layer.
        size_t num_nodes_per_layer;
        /// Number of layers backward that a node can connect to.
        size_t layers_back;

        /// The node configurations.
        std::vector<CGPNodeGene> nodes;

        /// The random number generator.
        std::mt19937 rng;

      private:
        /// @brief Decodes the header of the genotype.
        void DecodeHeader(const std::string &header) {
            // Parse header and save to member variables
            std::vector<size_t> header_parts;
            size_t start_pos = 0;
            size_t comma_pos = header.find(HEADER_SEP, start_pos);
            while (comma_pos != std::string::npos) {
                header_parts.push_back(std::stoull(header.substr(start_pos, comma_pos - start_pos)));
                start_pos = comma_pos + 1;
                comma_pos = header.find(HEADER_SEP, start_pos);
            }
            header_parts.push_back(std::stoull(header.substr(start_pos)));

            if (header_parts.size() != 5)
                throw std::runtime_error(std::format("Invalid genotype: Header should have 5 parameters but found {}.",
                                                     header_parts.size()));

            num_inputs = header_parts.at(0);
            num_outputs = header_parts.at(1);
            num_layers = header_parts.at(2);
            num_nodes_per_layer = header_parts.at(3);
            layers_back = header_parts.at(4);
        }

        /// @brief Decodes the genotype string. Header variables should be initialized before calling this.
        void DecodeGenotype(const std::string &genotype) {}

      public:
        CGPGenotype(size_t num_inputs, size_t num_outputs, size_t num_layers, size_t num_nodes_per_layer,
                    size_t layers_back)
            : num_inputs(num_inputs), num_outputs(num_outputs), num_layers(num_layers),
              num_nodes_per_layer(num_nodes_per_layer), layers_back(layers_back) {
            InitGenotype();
        }

        /// @brief Constructs a genotype from an encoding string.
        CGPGenotype(const std::string &encoded_genotype) {
            // Separate header and genotype
            size_t newline_pos = encoded_genotype.find(GENOTYPE_SEP);
            if (newline_pos == std::string::npos)
                throw std::runtime_error("Invalid genotype: No newline character found.");
            std::string header = encoded_genotype.substr(0, newline_pos);
            std::string genotype = encoded_genotype.substr(newline_pos + 1);

            // Parse header and save to member variables
            DecodeHeader(header);

            // Initialize the genotype
            InitGenotype();

            // Decode genotype
            DecodeGenotype(genotype);

            // Check if the number of functional nodes is correct
            assert(nodes.size() == num_layers * num_nodes_per_layer + num_outputs);
        }
        ~CGPGenotype() = default;

        /// @brief Returns the const iterator to the beginning of the node configurations.
        /// @return The const iterator to the beginning of the node configurations.
        std::vector<CGPNodeGene>::const_iterator begin() const { return nodes.begin(); }

        /// @brief Returns the const iterator to the end of the node configurations.
        /// @return The const iterator to the end of the node configurations.
        std::vector<CGPNodeGene>::const_iterator end() const { return nodes.end(); }

        /// @brief Returns the number of possible connections in the graph.
        /// @return The number of possible connections in the graph.
        size_t GetNumConnections() const {
            size_t num_connections = 0;
            for (const CGPNodeGene &node : nodes) {
                num_connections += node.input_connections.size();
            }
            return num_connections;
        }

        /// @brief Initializes an empty genotype with the cartesian graph parameters.
        void InitGenotype() {
            // Create header
            std::string header = std::format("{}{}{}{}{}{}{}{}{}", num_inputs, HEADER_SEP, num_outputs, HEADER_SEP,
                                             num_layers, HEADER_SEP, num_nodes_per_layer, HEADER_SEP, layers_back);
            // Clear node configurations
            nodes.clear();

            // Input nodes won't have any inputs and no function, so they are skipped

            // Start at 1 to account for the input layer
            for (size_t i = 1; i < num_layers + 1; i++) {
                for (size_t j = 0; j < num_nodes_per_layer; j++) {
                    // Count up possible input connections from each layer backwards
                    size_t valid_layers_back = std::min(layers_back, i);
                    size_t num_input_connections = valid_layers_back * num_nodes_per_layer;
                    if (i <= layers_back) {
                        num_input_connections -= num_nodes_per_layer;
                        num_input_connections += num_inputs;
                    }
                    // Create empty connections
                    std::vector<char> input_connections(num_input_connections, '0');
                    // Add the node configuration. Default function index is 0.
                    nodes.push_back({input_connections, 0});
                }
            }

            // Output layer can have a different number of nodes, do the same as above
            size_t output_layer_idx = num_layers + 1;
            for (size_t i = 0; i < num_outputs; i++) {
                size_t valid_layers_back = std::min(layers_back, output_layer_idx);
                size_t num_input_connections = valid_layers_back * num_nodes_per_layer;
                if (output_layer_idx <= layers_back) {
                    num_input_connections -= num_nodes_per_layer;
                    num_input_connections += num_inputs;
                }
                std::vector<char> input_connections(num_input_connections, '0');
                nodes.push_back({input_connections, 0});
            }
        }

        /// @brief Sets the seed of the random number generator.
        void SetSeed(size_t seed) { rng.seed(seed); }

        /// @brief Mutates the genotype by flipping the connections with a given probability between 0 and 1.
        /// @param mutation_rate The probability of flipping a connection.
        void MutateConnections(double mutation_rate) {
            assert(mutation_rate >= 0.0 && mutation_rate <= 1.0);
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            for (CGPNodeGene &node : nodes) {
                for (char &connection : node.input_connections) {
                    if (dist(rng) < mutation_rate) {
                        // Flip the connection
                        connection = connection == '0' ? '1' : '0';
                    }
                }
            }
        }
        
        /// @brief Mutates the genotype by changing the function of each node with a given probability between 0 and 1.
        /// @param mutation_rate The probability of changing the function of a node.
        /// @param num_functions The number of functions available to the nodes.
        void MutateFunctions(double mutation_rate, size_t num_functions) {
            assert(mutation_rate >= 0.0 && mutation_rate <= 1.0);
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            std::uniform_int_distribution<size_t> dist_func(0, num_functions - 1);
            for (CGPNodeGene &node : nodes) {
                if (dist(rng) < mutation_rate) {
                    // Mutate the function
                    node.function_idx = dist_func(rng);
                }
            }
        }
    };
} // namespace cowboys