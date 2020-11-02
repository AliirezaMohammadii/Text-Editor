#include <iostream>
#include <string>

using namespace std;


struct Node;
struct Node_child;
void minHeap_insertKey(string);
void maxHeap_insertKey(string);
string current_text = "";
string current_copied_text = "";
void statistics(string);
void insert(string);
void command(string);
int num_of_deleted_words = 0;
int num_of_deleted_lines = 0;
void search(string);
void check_input(string&);
bool cursor_printed = false;
int length_of_deleted_string;
void printPreorder(Node*);
void show_text();
void search(string);
void newLine(int, string&, int);
void sort();
void swap(int&, int&);
void make_new_pieces(Node*, int, int);
Node* find_parent(Node*);
void delete_line();
void edit_data1(int, int);
void delete_from_cursor_to_end();
void edit_data2(int);
void copy_line();
void change_state(string);
void move_cursor(string);
void put_cursor_on_end_of_line();
void put_cursor_on_first_of_line();
void cursor_shift_to_left();
void cursor_shift_to_right();
void maxHeap_deleteKey(string);
void minHeap_deleteKey(string);
void maxHeap_decreaseKey(int, string);
void minHeap_decreaseKey(int, string);
string maxHeap_extractMax();
string minHeap_extractMin();
void MaxHeapify(int);
void MinHeapify(int);
void maxHeap_insertKey(string);
void minHeap_insertKey(string);
int right(int);
int left(int);
int parent(int);
string mxaHeap_getMin();
string minHeap_getMin();
void swap(string*, string*);
void initialize();
void initial_text(string);
Node* newNode(int, int, string);
void print_C(string, int, int, int, int);
void print_D(string, int, int);
void read_input();
int deleted_indexes[1000][2];
int d_ctr = 0;


struct piece_table {

    string original = "Stanford University, officially Leland Stanford Junior University, "
    "is a private research university in Stanford, California.\n Stanford is known for its "
    "academic achievements, wealth, close proximity to Silicon Valley, and selectivity; \n"
    "it ranks as one of the world's top universities.";

    string add = "";

    Node* root;

} pt;

struct Node_child {

    Node_child* next;
    Node* child;

};

struct Node
{ 

    int start;
    int length;
    string source;
 
    Node_child* childs;
};

Node* newNode(int start, int length, string source)
{ 
    Node* temp = new Node();

    temp->start = start;
    temp->length = length;
    temp->source = source;
    temp->childs = NULL;

    return temp;
}

struct text_editor_data {

    int cursor_place = 0;
    int num_of_chars = 0;
    int num_of_words = 0;
    int num_of_lines = 0;

    char state;

    int num_of_pieces = 0;

    int* indexes_of_first_of_lines = new int[1000];


} dt;

int main() {

    initialize();
    read_input();
}

void read_input() {

    string str;

    while (getline(cin , str)) {

        cout << "CURRENT STATE : ";
        if (dt.state == 'c' || str == "c")
            cout << "COMMAND";
        else if (dt.state == 's' || str == "v")
            cout << "STATISTICS";
        else if (dt.state == 'i' || str == "i")
            cout << "INSERT";
        cout << endl;

        switch(dt.state) 
        {
            case 'c' : command(str); break;

            case 'i' : insert(str); break;
            
            case 's' : statistics(str); break;

        }

        show_text();

    }

}

void initialize() {

    initial_text(pt.original);

    pt.root = newNode(0, dt.num_of_chars, "original");

    current_text = pt.original;

    // cursor place is end of text, at first.
    dt.cursor_place = dt.num_of_chars;

    dt.state = 'c';

}

void initial_text(string str) {

    dt.num_of_chars = str.size();
    dt.indexes_of_first_of_lines[dt.num_of_lines++] = 0;

    int j = 0;
    for (int i = 0 ; i < dt.num_of_chars; i++) {
        if (str[i] == '\n') {

            // 2 ta kar ro ba ham kardm tu in khatte payin.
            dt.indexes_of_first_of_lines[dt.num_of_lines++] = i;
        }
        else if(str[i] == ' ') {

            minHeap_insertKey(str.substr(j,i-j));
            maxHeap_insertKey(str.substr(j,i-j));
            j = i+1;

            dt.num_of_words++;
        }
    }

    dt.num_of_words++;

}


struct MinHeap {

    string* harr = new string[10000];
    int capacity = 10000;
    int heap_size = 0;

} Min_h; 

struct MaxHeap {

    string* harr = new string[10000];
    int capacity = 10000;
    int heap_size = 0;

} Max_h; 

string minHeap_getMin() { 
    return Min_h.harr[0]; 
}

string mxaHeap_getMin() { 
    return Max_h.harr[0]; 
}

int parent(int i) { 
    return (i-1)/2; 
}

// to get index of left child of node at index i 
int left(int i) { return (2*i + 1); } 

// to get index of right child of node at index i 
int right(int i) { return (2*i + 2); } 
  

void minHeap_insertKey(string str) 
{
  
    // First insert the new key at the end 
    Min_h.heap_size++; 
    int i = Min_h.heap_size - 1; 
    Min_h.harr[i] = str; 
  
    // Fix the min heap property if it is violated 
    while (i != 0 && Min_h.harr[parent(i)].size() > Min_h.harr[i].size()) 
    { 
       swap(&Min_h.harr[i], &Min_h.harr[parent(i)]); 
       i = parent(i); 
    } 
    
}

void maxHeap_insertKey(string str) 
{
  
    // First insert the new key at the end 
    Max_h.heap_size++; 
    int i = Max_h.heap_size - 1; 
    Max_h.harr[i] = str; 
  
    // Fix the min heap property if it is violated 
    while (i != 0 && Max_h.harr[parent(i)].size() < Max_h.harr[i].size()) 
    { 
       swap(&Max_h.harr[i], &Max_h.harr[parent(i)]); 
       i = parent(i); 
    } 
    
}

void MinHeapify(int i) 
{ 
    int l = left(i); 
    int r = right(i); 
    int smallest = i; 
    if (l < Min_h.heap_size && Min_h.harr[l].size() < Min_h.harr[i].size()) 
        smallest = l; 
    if (r < Min_h.heap_size && Min_h.harr[r].size() < Min_h.harr[smallest].size()) 
        smallest = r; 
    if (smallest != i) 
    { 
        swap(&Min_h.harr[i], &Min_h.harr[smallest]); 
        MinHeapify(smallest); 
    } 
}

void MaxHeapify(int i) 
{ 
    int l = left(i); 
    int r = right(i); 
    int largest = i; 
    if (l < Max_h.heap_size && Max_h.harr[l].size() > Max_h.harr[i].size()) 
        largest = l; 
    if (r < Max_h.heap_size && Max_h.harr[r].size() > Max_h.harr[largest].size()) 
        largest = r; 
    if (largest != i) 
    { 
        swap(&Max_h.harr[i], &Max_h.harr[largest]); 
        MaxHeapify(largest); 
    } 
}

string minHeap_extractMin() 
{ 

    if (Min_h.heap_size == 1) 
    { 
        Min_h.heap_size--; 
        return Min_h.harr[0]; 
    } 
  
    // Store the minimum value, and remove it from heap 
    string root = Min_h.harr[0];
    Min_h.harr[0] = Min_h.harr[Min_h.heap_size-1]; 
    Min_h.heap_size--; 
    MinHeapify(0);
  
    return root; 
}

string maxHeap_extractMax() 
{ 

    if (Max_h.heap_size == 1) 
    { 
        Max_h.heap_size--; 
        return Max_h.harr[0]; 
    } 
  
    // Store the minimum value, and remove it from heap 
    string root = Max_h.harr[0]; 
    Max_h.harr[0] = Max_h.harr[Max_h.heap_size-1]; 
    Max_h.heap_size--; 
    MaxHeapify(0);
  
    return root; 
}

void minHeap_decreaseKey(int i, string new_val) 
{ 
    Min_h.harr[i] = new_val; 
    while (i != 0 && Min_h.harr[parent(i)] > Min_h.harr[i]) 
    { 
       swap(&Min_h.harr[i], &Min_h.harr[parent(i)]); 
       i = parent(i); 
    } 
}

void maxHeap_decreaseKey(int i, string new_val) 
{ 
    Max_h.harr[i] = new_val; 
    while (i != 0 && Max_h.harr[parent(i)] > Max_h.harr[i]) 
    { 
       swap(&Max_h.harr[i], &Max_h.harr[parent(i)]);
       i = parent(i); 
    } 
} 

void minHeap_deleteKey(string str) 
{ 

    for(int i = 0; i < sizeof(Min_h.harr); i++) {
        if (Min_h.harr[i] == str) {
            minHeap_decreaseKey(i, "");
            minHeap_extractMin();
            break;
        }
    }

}

void maxHeap_deleteKey(string str) 
{ 

    for(int i = 0; i < sizeof(Max_h.harr) ; i++) {
        if (Max_h.harr[i] == str) {
            maxHeap_decreaseKey(i, "ooooooooooooooooooooooooooooooooooo");
            maxHeap_extractMax();
            break;
        }
    }

} 

void swap(string *x, string *y) 
{ 
    string temp = *x; 
    *x = *y; 
    *y = temp; 
}







void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    int L[n1], R[n2]; 
  
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l+(r-l)/2; 
  
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 









void cursor_shift_to_right() {
    if (dt.cursor_place < dt.num_of_chars) 
        dt.cursor_place++;
    
    for (int i = 0 ; i < d_ctr ; i++) {
        if (dt.cursor_place == deleted_indexes[i][0] + 1) {
            dt.cursor_place += deleted_indexes[i][1];
            break;
        }
    }
}

void cursor_shift_to_left() {
    if (dt.cursor_place > 0) 
        dt.cursor_place--;

    for (int i = dt.cursor_place ; i >= 0 ; i--) {
        if (dt.cursor_place == deleted_indexes[i][0] + deleted_indexes[i][1]) {
            dt.cursor_place -= deleted_indexes[i][1];
            break;
        }
    }
}

void put_cursor_on_first_of_line() {

    for (int i = 0 ; i < dt.num_of_lines ; i++) {

        if (dt.num_of_lines == 1)
            dt.cursor_place = 0;

        else if (dt.cursor_place <= dt.indexes_of_first_of_lines[i] ) {
            dt.cursor_place = dt.indexes_of_first_of_lines[i-1] + 1;
            return;
        }

        else if (i == dt.num_of_lines - 1)
            dt.cursor_place = dt.indexes_of_first_of_lines[i]+1;
    }

}

void put_cursor_on_end_of_line() {

    for (int i = 0 ; i < dt.num_of_lines ; i++) {

        if (dt.cursor_place < dt.indexes_of_first_of_lines[i] ) {
            dt.cursor_place = dt.indexes_of_first_of_lines[i];
            return;
        }

        else if (i == dt.num_of_lines - 1)
            dt.cursor_place = dt.num_of_chars;
    }

}


void move_cursor(string str) {

    if (str == "0")
        put_cursor_on_first_of_line();

    else if (str == "$")
        put_cursor_on_end_of_line();

    else if (str == ">")
        cursor_shift_to_right();

    else if (str == "<")
        cursor_shift_to_left();

    if (str == ":0")
        dt.cursor_place = 0;

    else if (str == ":$")
        dt.cursor_place = dt.num_of_chars;

}

void change_state(string str) {

    if (str == "i")
        dt.state = 'i';
    
    else if(str == "v") {
        statistics(str);
        dt.state = 's';
    }

    else if(str == "q")
        dt.state = 'c';

}


void copy_line() {

    int f,r;

    for (int i = 0 ; i < dt.num_of_lines; i++) {

        if (dt.cursor_place <= dt.indexes_of_first_of_lines[i]) {
            r = dt.indexes_of_first_of_lines[i] - 1;
            f = dt.indexes_of_first_of_lines[i-1];
            current_copied_text = current_text.substr(f,r-f+1);
            break;
        }

        else if (i == dt.num_of_lines - 1) {
            f = dt.indexes_of_first_of_lines[i];
            r = dt.num_of_chars - 1;
            current_copied_text = current_text.substr(f,r-f+1);
        }
    }

}


void paste() {
    insert(current_copied_text);
}


void edit_data2(int r) {

    string str = current_text.substr(dt.cursor_place, r - 
    dt.cursor_place + 1);

    int j = 0;
    for (int i = 0 ; i < str.size(); i++) {
        if(str[i] == ' ') {

            minHeap_deleteKey(str.substr(j,i-j));
            maxHeap_deleteKey(str.substr(j,i-j));
            j = i+1;

            dt.num_of_words--;
        }
    }

    dt.num_of_words--;

}

void delete_from_cursor_to_end() {

    int r;
    for (int i = 0 ; i < dt.num_of_lines; i++) {

        if ((dt.cursor_place <= dt.indexes_of_first_of_lines[i]) ||
        (i == dt.num_of_lines - 1)) {

            if ((dt.cursor_place <= dt.indexes_of_first_of_lines[i])) {
                r = dt.indexes_of_first_of_lines[i] - 1;
            }

            else {
                r = dt.num_of_chars - 1;
            }

            edit_data2(r);

            int length = r - dt.cursor_place + 1;
            dt.num_of_chars -= length;

            deleted_indexes[d_ctr][0] = dt.cursor_place;
            deleted_indexes[d_ctr][1] = length;

            string spaces = "";
            for (int i = 0 ; i < length; i++)
                spaces += " ";
            insert(spaces);
            break;
        }
    }

}

void edit_data1(int f, int r) {

    string str = current_text.substr(f, r-f+1);

    int j = 0;
    for (int i = 0 ; i < str.size(); i++) {
        if(str[i] == ' ') {

            minHeap_deleteKey(str.substr(j,i-j));
            maxHeap_deleteKey(str.substr(j,i-j));
            j = i+1;

            num_of_deleted_words++;
        }
    }
}

void delete_line() {


    num_of_deleted_lines++;

    int f,r;


    for (int i = 0 ; i < dt.num_of_lines; i++) {

        if ((dt.cursor_place <= dt.indexes_of_first_of_lines[i])
        || i == dt.num_of_lines - 1) {
            if ((dt.cursor_place <= dt.indexes_of_first_of_lines[i])) {
                r = dt.indexes_of_first_of_lines[i] - 1;
                f = dt.indexes_of_first_of_lines[i-1];
            }
            else {
                f = dt.indexes_of_first_of_lines[i];
                r = dt.num_of_chars - 1;
            }

            edit_data1(f, r);

            int length = r - f + 1;
            dt.cursor_place = f;
            dt.num_of_chars -= length;

            deleted_indexes[d_ctr][0] = f;
            deleted_indexes[d_ctr][1] = length;


            string spaces = "";
            for (int i = 0 ; i < length; i++)
                spaces += " ";
            insert(spaces);
            break;
        }
    }

}

void cursor_on_next_word() {

    size_t found_index = current_text.find(" ", dt.cursor_place);
    if (found_index != string::npos) dt.cursor_place = found_index + 1; 

}

void cursor_on_previous_word() {

    int j = 0;
    for (int i = dt.cursor_place ; i >= 0 ; i-- ) {
        if (current_text[i] == ' ') {
            j = i-1;
            break;
        }
    }

    for (int i = j ; i >= 0 ; i-- ) {
        if (current_text[i] == ' ') {
            dt.cursor_place = i+1;
            break;
        }
    }

}

void cursor_on_next_word_multi(int num) {


    size_t found_index;

    while (num-- && found_index != string::npos) {
        found_index = current_text.find(" ", dt.cursor_place);
        if (found_index != string::npos) dt.cursor_place = found_index + 1;
    }

}

void cursor_on_previous_word_multi(int num) {
    while (num--) {
        cursor_on_previous_word();
    }
}


void command(string str) {

    if (str[0] == '/')
        search(str.substr(1));

    else if (str == "i" || str == "v")
        change_state(str);

    else if (str == "0" || str == "$" || str == ">" || str == "<"
    || str == ":0" || str == ":$" || str == ":w" || str == ":b")
        move_cursor(str);

    else if (str.size() == 2 && str[0] == ':') {
        int number_of_line = str[1] - 48;
        // minus one -> necause arrays start from zero.
        if (number_of_line == 1)
            dt.cursor_place = 0;
        else 
            dt.cursor_place = dt.indexes_of_first_of_lines[number_of_line - 1]+1;
    }

    else if (str == "Y")
        copy_line();

    else if (str == "p") {
        paste();
    }


    else if (str == "D")
        delete_from_cursor_to_end();

    else if (str == "w")
        cursor_on_next_word();

    else if (str == "b")
        cursor_on_previous_word();

    else if (str[1] == 'w') {
        int num = str[0] - 48;
        cursor_on_next_word_multi(num);
    }

    else if (str[1] == 'b') {
        int num = str[0] - 48;
        cursor_on_previous_word_multi(num);
    }

    else if (str == "dd")
        delete_line();
    
    else if (str == "yy")
        copy_line();

    else
        cout << "unKnownInput!" << endl;


    

}

int num = 0, previous_num=0;
// finds parent and makes a right child.
Node* find_parent(Node* parent) {
    
    Node* the_parent = NULL;

    if (parent == NULL)
        return NULL;
    
    int l1 , l2;
    int start1 = parent->start , start2 ;

    num += parent->length;

    if (num >= dt.cursor_place) {
            
        
        if (num != dt.cursor_place) {
            
            l1 = dt.cursor_place - previous_num;
            parent->start = start1;
            parent->length = l1;

            start2 = l1 + parent->start;
            l2 = num - dt.cursor_place;
            Node* ch = newNode(start2, l2, parent->source);

            ch->childs = NULL;
            Node_child* nch = new Node_child();
            nch->next = parent->childs;
            parent->childs = nch;
            nch->child = ch;
        }
        
        num = 0;
        previous_num = 0;
        return parent;
        
    }

    previous_num = num;

    Node_child* currentNode_childs = parent->childs;

    while (currentNode_childs != NULL) {
        the_parent = find_parent(currentNode_childs->child);
        if (the_parent != NULL)
            break;
        currentNode_childs = currentNode_childs->next;
    }
    
    return the_parent;

}


void make_new_pieces(Node* newPiece, int start, int length) {

    Node* parent = find_parent(pt.root);
    
    Node_child* nch = new Node_child();
    newPiece->childs = NULL;
    nch->next = parent->childs;
    parent->childs = nch;
    nch->child = newPiece;

}


void insert(string str) {

    if (str == "q" || str == "v")
        change_state(str);


    else if (str == "0" || str == "$" || str == ">" || str == "<"
    || str == ":0" || str == ":$" || str == ":w" || str == ":b") {
        move_cursor(str);
    }

    else if (str.size() == 2 && str[0] == ':') {
        int number_of_line = str[1];
        // minus one -> necause arrays start from zero.
        dt.cursor_place = dt.indexes_of_first_of_lines[number_of_line - 1];
    }

    else {

        int start, length;
        string source;

        Node* newPiece = NULL;


        if (str[0] == ' ' && str[1] == ' ') {
            start = 0;
            length = str.size();
            source = "deleted";
        }

        else {

            check_input(str);

            start = pt.add.size();
            length = str.size();
            pt.add += str;
            source = "add";
            dt.num_of_chars += length;

            for(int i = 0; i < dt.num_of_lines; i++)
                if (dt.indexes_of_first_of_lines[i] >= dt.cursor_place)
                    dt.indexes_of_first_of_lines[i] += length;

        }

        newPiece = newNode(start, length, source);
        make_new_pieces(newPiece, start, length);

        if (source == "add")
            dt.cursor_place += length;

    }

}

void statistics(string str) {

    if (str == "q" || str == "i")
        change_state(str);

    else {
        cout << endl << endl;
        cout << "number of words = " << dt.num_of_words
        - num_of_deleted_words << endl;
        cout << "number of lines = " << dt.num_of_lines
        - num_of_deleted_lines << endl;

        cout << "top 10 longest words :" << endl;

        string* max_words = new string[10];
        string* min_words = new string[10];

        int size = (dt.num_of_words < 10) ? dt.num_of_words : 10;

        for (int i = 0 ; i < size-1; i++) {
            string max_str = maxHeap_extractMax();
            cout << i+1 << " : " <<  max_str << " ";
            max_words[i] = max_str;
        }
        for (int i = 0 ; i < size-1; i++)
            maxHeap_insertKey(max_words[i]);
        
        cout << endl;

        cout <<"top 10 smallest words :" << endl;

        for (int i = 0 ; i < size-1; i++) {
            string min_str = minHeap_extractMin();
            cout << i+1 << " : " <<  min_str << " ";
            min_words[i] = min_str;
        }
        for (int i = 0 ; i < size-1; i++)
            minHeap_insertKey(min_words[i]);

        cout << endl;
    }

}

void newLine(int i, string& str, int size) {

    string temp2 = "";

    for (int j = 0 ; j < str.size() ; j++)
        temp2 += str[j];

    str = str.substr(0,i);
    str += "\n";
    str += ( (size - i > 2) ? temp2.substr(i+2) : "" );

    dt.indexes_of_first_of_lines[dt.num_of_lines++] = dt.cursor_place + i + 1;
    mergeSort(dt.indexes_of_first_of_lines, 0, dt.num_of_lines - 1);
}

// void sort() {

//     int size = sizeof(dt.indexes_of_first_of_lines);

// 	for (int i = 0 ; i < size ; i++)
// 	{
// 		for (int j = i + 1 ; j < size ; j++)
// 			if (dt.indexes_of_first_of_lines[j] < 
//             dt.indexes_of_first_of_lines[i]) 
//             swap(dt.indexes_of_first_of_lines[j] ,
//             dt.indexes_of_first_of_lines[i]) ;
//     }

// }

// void swap(int& i, int& j) {

//     int temp = dt.indexes_of_first_of_lines[i];
//     dt.indexes_of_first_of_lines[i] = dt.indexes_of_first_of_lines[j];
//     dt.indexes_of_first_of_lines[j] = temp;

// }


void search(string str) {

    size_t found_index = current_text.find(str);
    int ctr = 0;
    bool found = false;

    while ( found_index != string::npos) {
        found = true;
        ctr++;
        for (int i = 0 ; i < dt.num_of_lines; i++) {
            if (found_index <= dt.indexes_of_first_of_lines[i]) {
                cout << " line : " << i << " | ";
                break;
            }
            else if (i == dt.num_of_lines - 1)
                cout << " line : " << i+1 << " | ";
        }
        found_index = current_text.find(str, found_index+1);
    }

    cout << endl;

    if (found)
        cout << "number of found strings = " << ctr << endl;
    
    else
        cout << "Not Found!" << endl;

    
}   

void check_input(string& str) {

    bool only_one_word = true;

    int size = str.size();

    int j = 0;
    for(int i = 0 ; i < size ; i++) {

        if (size - i >= 1 && str[i] == '\\' && str[i+1] == 'n') {
            newLine(i, str, size);
            continue;
        }

        else if (str[i] == ' ') {
            
            only_one_word = false;
            minHeap_insertKey(str.substr(j,i-j));
            maxHeap_insertKey(str.substr(j,i-j));
            j = i+1;

            dt.num_of_words++;
        }
    }

    if (only_one_word) {
        minHeap_insertKey(str);
        maxHeap_insertKey(str);
    }


}

// ctr bara ine ke bedunam be curser residam ya na.
int ctr = 0, previous_counter=0;

void printPreorder(Node* root) 
{ 
    if (root == NULL)
        return; 
        
    if (root->source != "deleted")
        ctr += root->length;

    if (root->source == "deleted")
        length_of_deleted_string = root->length;


    else if (root->source == "original") {


        if (length_of_deleted_string != 0) {


            if (root->length <= length_of_deleted_string) {
                length_of_deleted_string -= root->length;
            }
            else {
                
                string str = pt.original;
                int start = root->start + length_of_deleted_string;
                int length = root->length - length_of_deleted_string;

                // print with contains deleted string.
                print_D(str, start, length);
            }
        }

        else {


            current_text += pt.original.substr(root->start , root->length);


            if (dt.cursor_place > 0 && ctr >= dt.cursor_place &&
            !cursor_printed) {
                string str = pt.original;
                int s1 = root->start;
                int l1 = dt.cursor_place - previous_counter;
                int s2 = dt.cursor_place - previous_counter + root->start;
                int l2 = ctr - dt.cursor_place;

                // print with contains cursor.
                print_C(str, s1, l1, s2, l2);
            }
            else
                cout << pt.original.substr(root->start , root->length);
        }
    }
    else {

        if (length_of_deleted_string != 0) {

            if (root->length <= length_of_deleted_string) {
                length_of_deleted_string -= root->length;
            }
            else {

                string str = pt.add;
                int start = root->start + length_of_deleted_string;
                int length = root->length - length_of_deleted_string;

                print_D(str, start, length);
            }
        }

        else {
        
            current_text += pt.add.substr(root->start , root->length);

            if (dt.cursor_place > 0 && ctr >= dt.cursor_place && !cursor_printed) {

                string str = pt.add;
                int s1 = root->start;
                int l1 = dt.cursor_place - previous_counter;
                int s2 = dt.cursor_place - 
                    previous_counter + root->start;
                int l2 = ctr - dt.cursor_place;

                print_C(str, s1, l1, s2, l2);
            }

            else cout << pt.add.substr(root->start , root->length);
        }
    }
    
    previous_counter = ctr;
        
    Node_child* currentNode_childs = root->childs;

    while (currentNode_childs != NULL) {
        printPreorder(currentNode_childs->child);
        currentNode_childs = currentNode_childs->next;
    }

} 

void print_D(string str, int start, int length) {

        current_text += str.substr(start ,length);

        cout << str.substr(start ,length);

        length_of_deleted_string = 0;

} 

void print_C(string str, int s1, int l1, int s2, int l2) {

    cursor_printed = true;
    cout << str.substr(s1 , l1);
    cout << "\'";
    if (ctr - dt.cursor_place > 0) cout << str.substr(s2 , l2);

}

void show_text() {

    current_text = "";

    cout << "*********************************************" << endl << endl;
    cout << "CURRENT TEXT : " << endl << endl << "\"";

    if (dt.cursor_place == 0)
            cout << "\'";

    printPreorder(pt.root);

    cout << "\"";

    cout << endl << endl;
    cout << "*********************************************" << endl << endl;

    ctr=0;
    previous_counter=0;
    cursor_printed = false;

}