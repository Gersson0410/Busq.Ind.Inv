def hash_djb2(word):
    hash_value = 5381  # Inicialización del valor hash con un número primo
    for c in word:
        hash_value = ((hash_value << 5) + hash_value) + ord(c)  # Aplicación de la función hash DJB2
    return hash_value


def main():
    # Entrada del mapper en el formato (doc_id, documento)
    mapper_output = [
        (1, "Hello to all"),
        (2, "Hello World"),
        (3, "Love to all"),
        (4, "Love World")
    ]

    # Diccionario para la salida intermedia del mapper
    intermediate_output = dict()

    # Mapper
    for pair in mapper_output:
        doc_id = pair[0]  # Obtener el doc_id del par
        document = pair[1]  # Obtener el documento del par

        word = ''  # Inicializar palabra actual

        for c in document:
            if c.isspace():  # Si el carácter es un espacio en blanco
                if word != '':  # Y la palabra actual no está vacía
                    word_lower = word.lower()  # Convertir a minúsculas
                    if word_lower not in intermediate_output:
                        intermediate_output[word_lower] = set()
                    intermediate_output[word_lower].add(doc_id)  # Insertar el doc_id
                    word = ''  # Limpiar la palabra actual
            else:
                word += c.lower()  # Agregar el carácter en minúscula

        if word != '':  # Si queda una palabra al final del documento
            word_lower = word.lower()
            if word_lower not in intermediate_output:
                intermediate_output[word_lower] = set()
            intermediate_output[word_lower].add(doc_id)

    # Reducer
    table_size = len(intermediate_output)  # Calcular el tamaño de la tabla hash
    lookup_table = []  # Crear la tabla de búsqueda como una lista

    for i in range(table_size):
        lookup_table.append(('', set()))  # Inicializar con tuplas vacías

    for pair in intermediate_output.items():
        word = pair[0]  # Obtener la palabra
        doc_ids = pair[1]  # Obtener el conjunto de doc_ids

        position = hash_djb2(word) % table_size  # Calcular la posición hash
        lookup_table[position] = (word, doc_ids)  # Almacenar en la tabla hash

    # Output
    for pair in lookup_table:
        word = pair[0]
        doc_ids = pair[1]
        if word != '':
            print("(", end='')
            print(word, end=', ')
            for doc_id in sorted(doc_ids):
                print(doc_id, end=' ')
            print(")")

if __name__ == "__main__":
    main()