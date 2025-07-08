#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

unsigned long hash_djb2(const std::string& word) {
    unsigned long hash_value = 5381; // Inicialización del valor hash con un número primo
    for (char c : word) {
        hash_value = ((hash_value << 5) + hash_value) + c; // Aplicación de la función hash DJB2
    }
    return hash_value;
}

struct InvertedIndex {
    std::set<int> doc_ids;
};

int main() {
    std::vector<std::pair<int, std::string>> mapper_output = {
        {1, "Hello to all"},
        {2, "Hello World"},
        {3, "Love to all"},
        {4, "Love World"}
    }; // Entrada del mapper en el formato (doc_id, documento)

    std::unordered_map<std::string, std::set<int>> intermediate_output;

    // Mapper
    for (const auto& pair : mapper_output) {
        int doc_id = pair.first; // Obtener el doc_id del par
        std::string document = pair.second; // Obtener el documento del par
        std::string word;
        for (char c : document) {
            if (isspace(c)) { // Si el carácter es un espacio en blanco
                if (!word.empty()) { // Y la palabra actual no está vacía
                    intermediate_output[word].insert(doc_id); // Insertar la palabra y el doc_id en la salida intermedia
                    word.clear(); // Limpiar la palabra actual
                }
            } else {
                word += tolower(c); // Si el carácter no es un espacio, agregar el carácter en minúscula a la palabra actual
            }
        }
        if (!word.empty()) { // Si queda una palabra al final del documento
            intermediate_output[word].insert(doc_id); // Insertar la última palabra y el doc_id en la salida intermedia
        }
    }

    // Reducer
    const unsigned long table_size = intermediate_output.size(); // Calcular el tamaño de la tabla hash como el número de palabras únicas
    std::vector<std::pair<std::string, std::set<int>>> lookup_table(table_size); // Crear la tabla hash con el tamaño calculado

    for (const auto& pair : intermediate_output) {
        const std::string& word = pair.first; // Obtener la palabra del par
        const std::set<int>& doc_ids = pair.second; // Obtener el conjunto de doc_ids del par
        unsigned long position = hash_djb2(word) % table_size; // Calcular la posición hash de la palabra
        lookup_table[position] = std::make_pair(word, doc_ids); // Almacenar la palabra y el conjunto de doc_ids en la posición hash
    }

    // Output
    for (const auto& pair : lookup_table) {
        if (!pair.first.empty()) {
            std::cout << "(" << pair.first << ", ";
            for (int doc_id : pair.second) {
                std::cout << doc_id << " ";
            }
            std::cout << ")" << std::endl;
        }
    }

    return 0;
}