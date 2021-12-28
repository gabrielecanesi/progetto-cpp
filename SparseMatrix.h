#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

/**
 *
 * @file SparseMatrix.h
 * @author Gabriele Canesi - Matricola 851637
 */


/**
 *
 * @brief Matrice sparsa. Questa classe permette di memorizzare i dati in una matrice, in posizioni scelte dall'utente.
 * La dimensione della matrice è scelta a runtime.
 * */


/**
 * @tparam T Il tipo di dato da memorizzare all'interno della matrice
 */

template<typename T>
class SparseMatrix {
private:
    struct Node;
public:

    typedef unsigned long size_type;

    /**
     * @brief Struttura che contiene i dati relativi a un elemento: riga, colonna e valore effettivo.\n
     * Viene ritornata dal const_iterator.
     */
    struct element{
        /**
         * @brief Riga in cui si trova il dato
         */
        size_type m_i;

        /**
         * @brief Colonna in cui si trova il dato
         */
        size_type m_j;

        /**
         * @brief Il dato effettivo
         */
        T data;

        /**
         * @brief Costruttore di default
         *
         * @post m_i == 0
         * @post m_j == 0
         */
        element() : m_i(0), m_j(0) {}

        /**
         *
         * @param i Valore della riga
         * @param j Valore della colonna
         * @param data il valore memorizzato
         *
         *
         * @post m_i = i
         * @post m_j = j
         * @post data = data
         */
        element(size_type i, size_type j, const T& data){
            m_i = i;
            m_j = j;
            this->data = data;
        }


        /**
         * @brief costruttore di copia
         * @param other l'elemento da copiare
         */
        element(const element& other) : m_i(0), m_j(0) {
            data = other.data;
            m_i = other.m_i;
            m_j = other.m_j;
        }

        /**
         * @brief Distruttore
         */
        ~element(){}


        /**
         * @brief Operatore di assegnamento
         * @param other l'elemento da assegnare
         * @return se stesso
         */
        element& operator=(const element& other){
            if (this != &other){
                element temp = other;
                data = temp.data;
                m_i = other.m_i;
                m_j = other.m_j;
            }
            return *this;
        }
    };
    /**
     * @typedef size_type
     * @brief Un wrapper per mascherare il vero tipo di dati che rappresenta la dimensione della matrice
     */


    /**
     * @brief Costruttore di default. Istanzia una matrice vuota e ha come dato di default il risultato del costruttore
     * di default del tipo T.
     *
     * @post m_width == 0
     * @post m_height == 0
     * @post m_data == nullptr
     */
    SparseMatrix() : m_width(0), m_height(0), m_data(nullptr) {}


    /**
     * @brief Costruttore che prende in input la dimensione della matrice e il valore di default
     * @param n numero di righe
     * @param m numero di colonne
     * @param default_value valore di default
     */
    SparseMatrix(size_type n, size_type m, const T& default_value) : m_data(nullptr), m_width(m), m_height(n), m_default(default_value){}

    /**
     * @brief Copy constructor
     * @param other Reference all'oggetto da copiare
     */


    //TODO: gestisci gli errori di memoria

    /**
     * @brief costruttore di copia
     * @param other l'oggetto da copiare
     * @post m_width == other.m_width
     * @post m_height == other.m_height
     * @post m_default == other.m_default
     */
    SparseMatrix(const SparseMatrix& other) : m_height(other.m_height), m_width(other.m_width), m_data(nullptr), m_default(other.m_default){
        Node* temp = other.m_data;

        // Dal momento che set chiamerà una new, devo gestire eventuali errori di memoria.
        try{
            while (temp != nullptr){
                set(temp->data.m_i, temp->data.m_j, temp->data.data);
                temp = temp->next;
            }
        }catch(...){
            distruggi_matrice();
            throw;
        }

    }

    /**
     * @brief Distruttore
     */
    ~SparseMatrix() {
        distruggi_matrice();
    }

    /**
     * @brief Operatore di assegnamento
     * @param other Reference all'oggetto da assegnare
     * @return Reference all'oggetto assegnato
     */
    SparseMatrix& operator=(const SparseMatrix& other) {
        if (this != other){
            SparseMatrix temp(other);
            //TODO: controllare swap
            m_data = temp.m_data;
            m_height = temp.m_height;
            m_width = temp.m_width;
            m_default = temp.m_default;
        }
        return *this;
    }

    /**
     * @brief Aggiunge un valore alla matrice ad una posizione precisa
     * @param i indice della riga
     * @param j indice della colonna
     * @param data
     */
    void set(size_type i, size_type j, const T& data){
        if(i > m_height || j > m_width || i < 0 || j < 0){
            return;
        }
        Node *found = get_node(i, j);
        if(found == nullptr){
            Node *new_node = new Node(i, j, data);
            new_node->next = m_data;
            m_data = new_node;
        }
        else {
            found->data.data = data;
        }
    }

    /**
     * @brief operatore per ottenere il valore alla posizione specificata
     * @param i indice della riga
     * @param j indice della colonna
     * @return il valore alla posizione specificata se esiste, il valore di default altrimenti
     */

    T& operator()(size_type i, size_type j) {
        if (i >= m_height || j >= m_width || i < 0 || j < 0){
            return m_default;
        }

        Node *found = get_node(i, j);
        if(found == nullptr){
            return m_default;
        }
        return found->data.data;
    }


    /**
     * @brief classe che implementa un const forward iterator per gli alementi della classe SparseMatrix. Ritorna oggetti di tipo element
     */
    class const_iterator {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef element                         value_type;
        typedef ptrdiff_t                 difference_type;
        typedef const element*                  pointer;
        typedef const element&                  reference;


        const_iterator() : ptr(nullptr) {}

        const_iterator(const const_iterator &other) {
            ptr = other.ptr;
        }

        const_iterator& operator=(const const_iterator &other) {
            if(this != &other){
                ptr = other.ptr;
            }
            return *this;
        }

        ~const_iterator() {}

        // Ritorna il dato riferito dall'iteratore (dereferenziamento)
        reference operator*() const {
            return ptr->data;
        }

        // Ritorna il puntatore al dato riferito dall'iteratore
        pointer operator->() const {
            return &(ptr->data);
        }

        // Operatore di iterazione post-incremento
        const_iterator operator++(int) {
            const_iterator temp = *this;
            temp.ptr = temp.ptr->next;
            return temp;
        }

        // Operatore di iterazione pre-incremento
        const_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        // Uguaglianza
        bool operator==(const const_iterator &other) const {
            return ptr == other.ptr;
        }

        // Diversita'
        bool operator!=(const const_iterator &other) const {
            return ptr != other.ptr;
        }

        // Solo se serve anche iterator aggiungere le precedenti definizioni

    private:
        //Dati membro

        const Node *ptr;
        // La classe container deve essere messa friend dell'iteratore per poter
        // usare il costruttore di inizializzazione.
        friend class SparseMatrix;

        // Costruttore privato di inizializzazione usato dalla classe container
        // tipicamente nei metodi begin e end
        explicit const_iterator(const Node* ptr) : ptr(ptr) {}

        // !!! Eventuali altri metodi privati

    }; // classe const_iterator


    /**
     * @return l'iteratore costante che punta al primo elemento disponibile
     */
    const_iterator begin() const {
        return const_iterator(m_data);
    }


    /**
     *
     * @return l'iteratore che rappresenta l'elemento dopo la fine della matrice.
     */
    const_iterator end() const {
        return const_iterator(nullptr);
    }

private:

    // Struttura che implementa il nodo della lista.
    struct Node{
        element data;
        Node *next;

        // Costruttore di default
        Node() : next(nullptr){}

        // Costruttore di copia
        Node(const Node& other){
            data = other.data;
            next = other.next;
        }

        // Costruttore che prende in input gli indici e il dato da inserire.
        Node(size_type i, size_type j, const T& data) : data(element(i, j, data)), next(nullptr){}

        // Distruttore. è vuoto perchè la distruzione viene gestita dalla classe SparseMatrix ed è visibile solo ad essa.
        ~Node(){}

        // Operatore di assegnamento.
        Node& operator=(const Node& other){
            if (this != &other){
                Node temp = other;
                data = other.data;
                next = temp.next;
            }
            return *this;
        }
    };


    // La matrice vera e propria
    Node *m_data;

    // Gli attributi che memorizzano larghezza e altezza della matrice
    size_type m_width;
    size_type m_height;

    // Il valore di default nel caso venga richiesta una cella vuota
    T m_default;

    // Funzione di appoggio che cerca il puntatore a un elemento con indici (i, j) se esiste, nullptr altrimenti
    Node* get_node(size_type i, size_type j){
        Node* temp = m_data;
        while (temp != nullptr && temp->data.m_i != i && temp->data.m_j != j){
            temp = temp->next;
        }
        return temp;
    }

    void distruggi_matrice(){
        Node* it = m_data;
        while (it != nullptr){
            Node* temp = it;
            it = it->next;
            delete temp;
        }

        m_height = 0;
        m_width = 0;
        m_data = nullptr;
    }

};


/**
 *
 * @tparam T il tipo di dato della matrice
 * @tparam Pred il tipo del funtore
 * @param M la matrice da visitare
 * @param P il predicato da testare
 * @return il numero di elementi inseriti nella matrice che soddisfano P
 */
template<typename T, typename Pred>
long evaluate(const SparseMatrix<T>& M, Pred P){
    long result = 0;
    typename SparseMatrix<T>::const_iterator begin, end;
    for(begin = M.begin(), end = M.end(); begin != end; ++begin){
        if(P(begin->data)){
            ++result;
        }
    }
    return result;
}


#endif // SPARSE_MATRIX_H
