#include <algorithm>
#include <cstdint>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Node {
public:
  char character;
  int weight;
  Node *node_left;
  Node *node_right;
  Node(char p_character, int p_weight, Node *p_node_left = NULL,
       Node *p_node_right = NULL) {
    character = p_character;
    weight = p_weight;
    node_left = p_node_left;
    node_right = p_node_right;
  }
  void increaseWeight() { weight++; }
  char getMyChar() { return character; }
};

// upper_bond : renvoie le premier élément qui va être supérieur à une valeur

// emplace back, node, Bon flemme de faire des pointeurs, je compte m'occuper
// des opti plus tard
Node *returnNode(char my_char) {
  Node *to_return = new Node{my_char, 0};
  return to_return;
}

// permet de voir si un char est présent dans la liste d'occurence
bool isIn(char char_to_search, map<char, int> &my_list) {
  auto search = my_list.find(char_to_search);
  if (search == my_list.end()) {
    return false;
  }
  return true;
}

// prend le nom d'un fichier en paramètre, et renvoie un string avec tout le
// contenu du fichier
string text_file_to_string(string file_name) {
  ifstream read_file(file_name);
  char c;

  string my_string;
  while (read_file.get(c)) {
    my_string.push_back(c);
  }
  read_file.close();

  return my_string;
}

// transforme notre string à écrire dans le fichier compressé  en un map
// contenant les occurences de chaque caractère, et le range les caractères dans
// l'ordre croissant de nombre de caractères
vector<pair<char, int>> string_to_vector_of_occurrences(string &my_string) {

  map<char, int> my_chars_occurences;
  char c;

  // pour chaque caractère de notre string
  for (auto c : my_string) {
    auto search = my_chars_occurences.find(c);
    // si le char n'est pas déjà présent dans la liste d'occurence, on l'ajoute
    if (search == my_chars_occurences.end()) {
      my_chars_occurences.emplace(std::make_pair(c, 1));
    } else { // sinon on incrémente la variable d'occurence du char
      my_chars_occurences[c]++;
    }
  }

  // on copie notre map dans un vec, que l'on range dans l'ordre croissant de
  // nombre d'occurences
  vector<pair<char, int>> vec;
  copy(my_chars_occurences.begin(), my_chars_occurences.end(),
       back_inserter<std::vector<pair<char, int>>>(vec));

  sort(vec.begin(), vec.end(),
       [](const pair<char, int> &l, const pair<char, int> &r) {
         return l.second < r.second;
       });
  return vec;
}

/*Node& node_from_pair(pair<char, int> my_pair){

  return new Node{std::get<0>(my_pair), std::get<1>(my_pair)};
  }*/
Node *node_from_nodes(Node *my_first_node, Node *my_second_node) {
  return new Node{' ', my_first_node->weight + my_second_node->weight,
                  my_first_node, my_second_node};
}

void print_list(list<Node *> &my_list) {
  // for(auto it = my_list.begin(); it != my_list.end(); ++it){
  //   cout << (*it) -> weight << ", ";
  // }
  // cout << endl;
}

// insère le node en paramètre dans la liste de nodes tout en gardant l'ordre
// croissant
void insert_with_sorting(list<Node *> &my_nodes, Node *&my_new_node) {

  bool insert = false;
  for (auto it = my_nodes.begin(); it != my_nodes.end(); ++it) {
    // on insère le nouveau noeud
    if (my_new_node->weight < (*it)->weight && !insert) {
      my_nodes.insert(it, my_new_node);
      insert = true;
    }
  }
  if (!insert) { // si c'est pas inséré, c'est que tout les éléments sont plus
                 // petits que lui, donc on ajoute à la fin
    my_nodes.push_back(my_new_node);
  }
}

void parcours_profondeur(Node *my_node) {
  deque<Node *> ma_file;
  ma_file.push_back(my_node);
  Node *head;
  deque<string> droite_gauche;
  droite_gauche.push_back("tete");
  while (!(ma_file.empty())) {
    head = ma_file.front();
    ma_file.pop_front();
    if (head != NULL) {
      cout << head->weight << droite_gauche.front() << head->character << endl;
      droite_gauche.pop_front();
      ma_file.push_back(head->node_right); // right= droite, on monte, donc 1
      droite_gauche.push_back("droite");
      ma_file.push_back(head->node_left); // left = gauche, on descend, donc 0
      droite_gauche.push_back("gauche");
    }
  }
  // while(!(ma_file.empty())){
  //   head = ma_file.front();
  //   ma_file.pop_front();
  //   if(head != NULL){
  //     if(head -> character == ' ') {

  //       ma_file.push_back(head -> node_right);// right= droite, on monte,
  //       ma_file.push_back(head -> node_left); // left = gauche, on
  //       //     descend, donc 0
  //     }
  //   } else {
  //     cout << "mon caractère : " << head -> character << endl;
  //     ma_file.push_back(head->node_right); // right= droite, on monte,
  //     ma_file.push_back(head->node_left);  // left = gauche, on
  //   }
  // }
  cout << endl;
  cout << "fin du parcours en profondeur" << endl;
}

void free_tree(Node *my_node) {
  if (my_node == NULL) {
    return;
  }
  if (my_node->node_left != NULL) {
    free_tree(my_node->node_left);
  }

  if (my_node->node_right != NULL) {
    free_tree(my_node->node_right);
  }
  delete my_node;
}

// prend les deux plus petits éléments, en créer un nouveau noeud parent à ceux
// ci, retire les deux plus petits éléments, retuourne le nouveau neoud
Node *create_node_from_two_smallest(list<Node *> &my_nodes) {

  // vu que c'est dans l'ordre croissant d'occurence, les deux premiers éléments
  // sont ceux avec le plus petit nb d'occurence
  Node *my_smallest_element = my_nodes.front();
  my_nodes.pop_front();
  Node *my_second_smallest_element = my_nodes.front();
  my_nodes.pop_front();

  // on en fait un noeud, qui possède les deux plus petits noeud en noeud enfant
  return new Node{
      ' ', my_smallest_element->weight + my_second_smallest_element->weight,
      my_smallest_element, my_second_smallest_element};
}

// on transforme notre vecteur d'occurence en arbre
Node *vector_of_occurences_to_tree(vector<pair<char, int>> my_occcurences) {

  // on transforme chaque char associé à son occurence en node, qu'on met dans
  // une liste de Node
  list<Node *> my_nodes;
  for (auto my_occurence : my_occcurences) {
    my_nodes.push_back(
        new Node{std::get<0>(my_occurence), std::get<1>(my_occurence)});
  }

  // tant qu'il reste plus de deux noeud en jeux
  while (my_nodes.size() > 2) {

    Node *my_new_node = create_node_from_two_smallest(my_nodes);

    // on insère le nouveau noeud dans la liste de node
    insert_with_sorting(my_nodes, my_new_node);
  }

  // on créer le dernier noeud

  Node *my_node_to_return = create_node_from_two_smallest(my_nodes);

  return my_node_to_return;
}

// on transforme notre arbre en codage de binaire
void tree_to_dict_of_binary_codage(map<deque<bool>, char> &coding_list,
                                   deque<bool> my_coding, Node *my_tree) {

  if (my_tree->node_right == NULL && my_tree->node_right == NULL) {
    // ajouter au codage, car on est arrivé au bout
    coding_list.emplace(make_pair(my_coding, my_tree->character));
  }
  // à chaque appel, on dupplique le vecteur booléen, et on fait l'appel
  // récursif :
  auto my_duplicate_coding = my_coding;

  // - y en a un on lui appendera false (donc 0), + appel récursif sur la gauche
  my_coding.push_back(false);
  if (my_tree->node_left != NULL) {
    tree_to_dict_of_binary_codage(coding_list, my_coding, my_tree->node_left);
  }
  // - un on lui appendera true(donc 1), + appel récursif sur la droite
  my_duplicate_coding.push_back(true);
  if (my_tree->node_right != NULL) {
    tree_to_dict_of_binary_codage(coding_list, my_duplicate_coding,
                                  my_tree->node_right);
  }
}

// range le vecteur dans l'ordre alphabetique
void sorting_in_alphabetical_order(vector<string> &my_list_of_strings) {
  sort(my_list_of_strings.begin(), my_list_of_strings.end(),
       [](const auto &x, const auto &y) { return x < y; });
}

// transforme notre string en un vecteur contenant tout les rotations possible
void all_possible_rotation_of_string(string my_string_to_rotate,
                                     vector<string> &my_list_of_strings) {

  my_list_of_strings.reserve(my_string_to_rotate.size());

  string my_original_string = my_string_to_rotate;
  my_list_of_strings.push_back(my_string_to_rotate);

  // ajout de toutes les rotations
  for (int i = 0; i < (int)my_string_to_rotate.size() - 1; i++) {
    rotate(my_string_to_rotate.begin(), my_string_to_rotate.begin() + 1,
           my_string_to_rotate.end());
    my_list_of_strings.push_back(my_string_to_rotate);
  }

  sorting_in_alphabetical_order(my_list_of_strings);
}

// le string à écrire dans le fichier compressé, il est plus facilement
// compressable avec la Transformée de Burrows-Wheeler car ça augmente le taux
// de lettres identiques côte à côte
string get_the_string_to_write_in_compressed_file(
    vector<string> &my_list_of_string_rotated) {
  string the_return;
  for (auto i : my_list_of_string_rotated) {
    the_return.push_back(i.back());
  }
  return the_return;
}
void print_string_vector(vector<string> &a) {
  for (auto i : a) {
    cout << i << endl;
  }
}

string recontruct_original_string(string &my_rotated_string,
                                  vector<string> &reconstruct_string,
                                  int index_of_original_string) {
  int string_length = my_rotated_string.size();

  for (char my_char : my_rotated_string) {
    string tmp(1, my_char);
    reconstruct_string.push_back(tmp);
  }
  sorting_in_alphabetical_order(reconstruct_string);

  int i;
  for (int x = 0; x < string_length - 1; x++) {
    i = 0;
    for (char my_char : my_rotated_string) {
      string tmp(1, my_char);
      reconstruct_string[i].insert(0, tmp);
      i += 1;
    }
    sorting_in_alphabetical_order(reconstruct_string);
    cout << "_____________" << endl;
    for (auto element : reconstruct_string) {
      cout << "[" << element << "]" << endl;
    }
  }

  return reconstruct_string[index_of_original_string];
}

// première étape :
// lire l
// On créer une liste de noeud pour chaque caractère, qu'on utile lors de la
// lecture du fichier Ensuite on fait la boucle qui transforme la file de noeud
// en arbre Ensuite, on verra

// prend un char, regarde son équivalent en binaire, l'ajoute à la liste de bits
void add_char_in_list_of_bits(char my_char_to_add,
                              map<deque<bool>, char> &my_coding_list,
                              list<int> &my_list_of_bits) {

  // on cherche le char dans le dictionnaire, et récupérons son équivalent
  // binaire
  deque<bool> save_list_bit;
  bool find = false;
  for (auto it = my_coding_list.begin(); it != my_coding_list.end() && !find;
       ++it) {
    if (it->second == my_char_to_add) {
      save_list_bit = it->first;
      find = true;
    }
  }
  // on écrit en binaire le caractère

  for (bool i : save_list_bit) {
    if (i) {
      my_list_of_bits.push_back(1);
    } else {
      my_list_of_bits.push_back(0);
    }
  }
}

// prend une liste d'int de 0 et des 1, lui retire 8 élément pour en faire un
// octet, et renvoie cet octet
int8_t create_chunk_bit(list<int> &my_list_of_bits) {
  int8_t my_byte;
  for (int i = 0; i < 8; i++) {
    // 8 - 1 - i, pour que ça prenne le bit le plus à gauche
    // c'est pas très utile en vrai, mais ça m'aide à me représenter
    my_byte ^=
        (-my_list_of_bits.front() ^ my_byte) &
        (1UL
         << (8 - 1 -
             i)); // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
    my_list_of_bits.pop_front();
  }
  return my_byte;
}

// écrit la list de bits de text dans l'array
void write_list_of_bits_of_text_in_array(list<int> &my_list_of_bits,
                                         int8_t *my_tab, int the_beginning) {
  auto number_of_bytes = (my_list_of_bits.size() / 8);
  for (int i = the_beginning; (i - the_beginning) < number_of_bytes; i++) {
    my_tab[i] = create_chunk_bit(my_list_of_bits);
  }
}

// écrit le vector contenant notre dictionnaire sous forme de int de 8 bits,
// dans notre array qui contiendra tout le contenu du fichier à écrire
void write_dict_in_array(vector<int8_t> &my_dict, int8_t *my_array) {
  auto number_of_bytes = my_dict.size(); // +1?
  // 3 car les 3 premiers octets sont dédié à autre chose
  for (int i = 3; (i - 3) < number_of_bytes; i++) {
    my_array[i] = my_dict[i - 3]; // i-3 car on commence à l'indice 0
  }
}

// permet d'ajouter les bit nécessaire pour avoir un multiple de 8
// renvoie le nombre de bit ajouter
int8_t adding_bit_necessary(list<int> &my_list_of_bits) {
  int nb_bit_to_add = 0;
  // si le nombre de bits est inférieur à 8 (très petit fichier)j
  if (my_list_of_bits.size() < 8) {
    nb_bit_to_add = 8 - my_list_of_bits.size();
  }
  // si le nombre de bits n'est pas un multiple de 8 (donc pas uniquement des
  // octets)
  else if (my_list_of_bits.size() % 8 != 0) {
    nb_bit_to_add = 8 - my_list_of_bits.size() % 8;
  }

  int8_t bit_added = nb_bit_to_add;
  // on ajoute les bits à ajouter
  for (; nb_bit_to_add > 0; nb_bit_to_add--) {
    my_list_of_bits.push_back(0);
  }
  return bit_added;
}

// écrit le dictionnaire sous format binaire dans un vecteur d'élément 8 bits,
// en vu de l'écrire dans le fichier compressé final
vector<int8_t> dict_in_binary_form(map<deque<bool>, char> &my_coding_list) {
  // on écrit le dico sous cette forme
  // char - taille du codage du char - codage en lui même
  vector<int8_t> my_dictionnary_in_binary;
  int8_t size_codage_in_bit;
  // pour chaque élément du dico
  for (auto element : my_coding_list) {
    // on ajoute notre caractère à notre liste
    my_dictionnary_in_binary.push_back((int8_t)element.second);
    // on récupère la taille du codage binaire, et on l'ajoute à notre liste de
    // nombre binaire
    size_codage_in_bit = element.first.size();
    my_dictionnary_in_binary.push_back(size_codage_in_bit);
    // on créer notre liste de bit nécessaire pour mettre le codage en binaire
    list<int> my_list_of_bits;
    for (bool my_bit : element.first) {
      if (my_bit) {
        my_list_of_bits.push_back(1);
      } else {
        my_list_of_bits.push_back(0);
      }
    }
    // ajout des bits nécessaire si c'est pas un multiple de 8
    adding_bit_necessary(my_list_of_bits);
    // maintenant qu'on a notre liste de bit, il faut la mettre en format
    // int8_t, et l'ajouter à notre dictionnaire binaire
    auto number_of_bytes = my_list_of_bits.size() / 8;
    for (int i = 0; i < number_of_bytes; i++) {
      my_dictionnary_in_binary.push_back(create_chunk_bit(my_list_of_bits));
    }
  }
  return my_dictionnary_in_binary;
}

// prend un text, le converti en liste de bit, ajoute les bits nécessaire, et
// renvoie le nombre de bit ajouter
int8_t text_to_list_of_bits(list<int> &text_to_write_in_list_of_bits,
                            string &original_text,
                            map<deque<bool>, char> &my_coding_list) {

  for (char my_char : original_text) {
    add_char_in_list_of_bits(my_char, my_coding_list,
                             text_to_write_in_list_of_bits);
  }
  // on ajoute les bits à ajouter, et enregistrons le nb de bits ajouter, pour
  // l'écrire au début du fichier et enlever ces bits inutiles lors de la
  // décompression
  int8_t bit_added = adding_bit_necessary(text_to_write_in_list_of_bits);
  return bit_added;
}

void add_info_bits_in_array(int8_t *array, int bit_added_in_text,
                            int my_dict_in_binary_size) {

  array[0] = bit_added_in_text; // le nombre de bit ajouter à la fin du texte
  // on enregistre la taille sur deux bit: 128 * array[1] + array[2]
  if (my_dict_in_binary_size < 128) {
    array[1] = 0;
    array[2] = my_dict_in_binary_size;
  } else {
    array[1] = my_dict_in_binary_size / 128;
    array[2] = my_dict_in_binary_size % 128;
  }
}

int index_end_of_dict(int8_t *array) {
  // index à partir duquel on peut écrire, càd après le dico
  // l'indice est de 3, car on commence à écrire le dico partir de 3, + la
  // taille du dico car on veut écrire après le dico
  return 3 + array[1] * 128 + array[2];
}

void write_in_file(string file_name, int8_t *array, int array_size) {

  // c'est la méthode la plus simple que j'ai trouver pour écrire un tab dans un
  // fichier
  FILE *pFile;
  pFile = fopen(file_name.c_str(), "wb");
  fwrite(array, 1, array_size, pFile);
  fclose(pFile);
}

void write_array(int8_t *array, int bit_added,
                 vector<int8_t> &my_dict_in_binary,
                 list<int> &text_to_write_in_list_of_bits) {

  // on ajoute les bits d'info dans l'array
  add_info_bits_in_array(array, bit_added, my_dict_in_binary.size());

  // on écrit le dict dans l'array
  write_dict_in_array(my_dict_in_binary, array);

  // on écrit le text dans l'array
  write_list_of_bits_of_text_in_array(text_to_write_in_list_of_bits, array,
                                      index_end_of_dict(array));
}

// écrit le fichier compressé à partir du texte et du dictionnaire de traduction
// de char à codage binaire
void write_compressed_file(string file_name, string &original_text,
                           map<deque<bool>, char> &my_coding_list) {
  // créer une liste de bit (list int), qui contient le texte à écrire
  list<int> text_to_write_in_list_of_bits;
  int8_t bit_added = text_to_list_of_bits(text_to_write_in_list_of_bits,
                                          original_text, my_coding_list);

  // conversion du dict en binaire
  vector<int8_t> my_dict_in_binary = dict_in_binary_form(my_coding_list);

  // array size = dict size + text_size + bits d'infos
  int array_size = (text_to_write_in_list_of_bits.size() / 8) + 1;
  array_size += my_dict_in_binary.size() + 2;

  int8_t array[array_size]; // l'array qu'on va écrire dans le fichier

  // on écrit les bits d'infos, le dico et le texte
  write_array(array, bit_added, my_dict_in_binary,
              text_to_write_in_list_of_bits);

  // on écrit l'array dans le file
  write_in_file(file_name, array, array_size);
}

// maintenant qu'on a notre read_file, on le transforme en liste de bit, on
// enlève les bit inutile à la fin, on détermine les caractère à partir du dico
// (avec find), et ce jusqu'à la fin de la liste de bit.
// enfin, on transforme notre chaine de caractère grâce au délire de baus

void chunk_to_list_bit(list<int> &list_of_bits, int8_t chunk_of_bits) {

  int bit;
  for (int n = 0; n < 8; n++) {
    bit = (chunk_of_bits >> (8 - 1 - n)) & 1;
    list_of_bits.push_back(bit);
  }
}

// cherche le char dans le dictionnaire
char find_char(map<deque<bool>, char> &my_coding_list,
               list<int> &list_of_bits_of_text) {
  bool added = false;
  deque<bool>
      list_of_bit_of_coding; // on lui met des bit de notre list de bit jusqu'à
                             // avoir trouvé un codage de caractère
  while (1) {
    // on ajoute un bit à notre potentiel codage
    if (list_of_bits_of_text.front() == 1) {
      list_of_bit_of_coding.push_back(true);
    } else {
      list_of_bit_of_coding.push_back(false);
    }
    // on le retire de la liste de bit
    list_of_bits_of_text.pop_front();

    // on essaye de trouvé si notre codage existe
    // je sais plus pourquoi j'ai utiliser ça et pas find
    if (my_coding_list.count(list_of_bit_of_coding) > 0) {
      return my_coding_list[list_of_bit_of_coding];
    }
  }
}
// prend un list<int8_t>, contenant le codage d'un char, ainsi que la taille du
// char et le char en lui même, traduit le vector en list<int>, l'épure de ses
// bit en trop à partir de sa taille, le transforme en deque<bool>, et l'ajoute
// au map dico passer en paramètre
void add_a_codage_to_dict(map<deque<bool>, char> &my_dict_to_fill, char my_char,
                          int size_of_codage, list<int8_t> &codage_of_a_char) {
  list<int> my_list_of_bits;
  for (auto my_chunk_of_bit : codage_of_a_char) {
    chunk_to_list_bit(my_list_of_bits, my_chunk_of_bit);
  }

  codage_of_a_char.clear(); // On vide notre codage, on l'a lu, donc plus besoin

  // on enlève les bit en trop
  int extra_bit = 0; // bit en trop, à enlever
  // dans le cas où c'est un multiple de 8, y a pas de bit à enlever et on est
  // bon
  if (size_of_codage < 8) {
    extra_bit = 8 - size_of_codage;
  } else if (size_of_codage % 8 !=
             0) { // si la taille du codage n'est pas un multiple de 8
    extra_bit = 8 - (size_of_codage % 8);
  }

  for (; extra_bit > 0; extra_bit--) {
    my_list_of_bits.pop_back();
  }

  // transformation de notre liste de bit en deque<bool>
  deque<bool> my_codage_in_bool;
  for (auto my_bit : my_list_of_bits) {
    if (my_bit == 1) {
      my_codage_in_bool.push_back(true);
    } else {
      my_codage_in_bool.push_back(false);
    }
  }
  my_dict_to_fill[my_codage_in_bool] = my_char;
}

// transforme un dictionnaire en binaire, en un vrai dictionnaire
void dict_in_binary_to_real_dict(vector<int8_t> &my_dict_in_binary,
                                 map<deque<bool>, char> &my_dict) {
  int lap_remaining_before_coding_size = 1; // tour restant avant la taille du codage
  char actual_char;
  int size_of_codage;
  list<int8_t> codage_of_char;
  // pour chaque bit de mon dico en binaire
  for (int8_t element : my_dict_in_binary) {
    if (lap_remaining_before_coding_size == 1) { // le bit est au char, on enregistre le char
      actual_char = (char)element;
    } else if (lap_remaining_before_coding_size == 0) {            // on est à la taille du codage d'un char
      if ((int)element <= 8) { // si la taille du codage est inférieur à 1 byte
        // il reste 3 bits avant le prochain codage:
        lap_remaining_before_coding_size = 1 + 1 + 1;
      } else { // si la taille >= taille d'un byte
        lap_remaining_before_coding_size =
            (int)element /
            8; // on ajotue un tour restant pour chaque byte de codage
        lap_remaining_before_coding_size += 3;
      }
      size_of_codage = (int)element;

    } else { // si on est au codage en lui même
      codage_of_char.push_back(element);
      if (lap_remaining_before_coding_size == 2) { // si on a ajouter le dernier élément de codage_of_char
        add_a_codage_to_dict(my_dict, actual_char, size_of_codage,
                             codage_of_char);
      }
    }
    lap_remaining_before_coding_size--; // on décrémente le nombre restant
  }
}

map<deque<bool>, char> retrieve_dict(vector<int8_t> &my_dict_in_binary, vector<int8_t> &buffer, int dico_len) {
  my_dict_in_binary.reserve(dico_len);
  for (int i = 3; i < dico_len + 3;
       i++) { // + 3 car on compte par les premiers bits, dédié à des autres
              // choses
    my_dict_in_binary.push_back(buffer[i]);
  }

  // on converti notre dico binaire en dico réel, sous forme de map
  map<deque<bool>, char> my_dict;
  dict_in_binary_to_real_dict(my_dict_in_binary, my_dict);

  return my_dict;
}

string retrieve_text(vector<int8_t> &buffer, int text_begin,
                            map<deque<bool>, char> my_dict) {

  // on récupère le texte sous forme binaire en liste de bits
  list<int> list_of_bits_of_text;
  for (int i = text_begin; i < buffer.size(); i++) {
    chunk_to_list_bit(list_of_bits_of_text, buffer[i]);
  }

  int bit_added = buffer[0]; // bit ajouter à la fin, qu'on devra enlever à
                             // notre liste de bit

  // on enlève les bits ajouter à la fin
  for (; bit_added > 0; bit_added--) {
    list_of_bits_of_text.pop_back();
  }

  string my_text_recompose;
  char my_char_to_add;
  while (list_of_bits_of_text.size() > 0) {
    // retirer un caractère
    my_char_to_add = find_char(my_dict, list_of_bits_of_text);
    // l'ajouter à notre texte
    my_text_recompose += my_char_to_add;
  }
  return my_text_recompose;
}
void read_file(string file_name) {
  ifstream input(file_name, std::ios::binary);
  // on récupère le contenu du fichier compressé
  vector<int8_t> buffer(std::istreambuf_iterator<char>(input), {});

  // la taille du dico, comme ça on sait où il se commence et fini
  int dico_len = buffer[1] * 128 + buffer[2];

  // on récupère notre dictionnaire 
  vector<int8_t> my_dict_in_binary;
  map<deque<bool>, char> my_dict = retrieve_dict(my_dict_in_binary, buffer,
                                              dico_len);

  // on récupère l'index du début de l'écriture du texte
  int text_begin = dico_len + 3; // les trois premiers bits dédié à des choses

  string my_text_recompose = retrieve_text(buffer, text_begin, my_dict);
  cout << "affichage : " << my_text_recompose << endl;
}

//"my_text.txt"
void compression(string to_compress) {
  string the_text_file = text_file_to_string(to_compress);

  vector<string> my_strings;
  all_possible_rotation_of_string(the_text_file, my_strings);

  string string_from_rotation =
      get_the_string_to_write_in_compressed_file(my_strings);

  vector<pair<char, int>> occurences_of_each_char =
      string_to_vector_of_occurrences(string_from_rotation);

  // On transforme notre vecteur d'occurence en arbre
  Node *my_tree = vector_of_occurences_to_tree(occurences_of_each_char);

  // on transforme notre arbre d'occurence en une liste de codage pour chaque
  // char
  map<deque<bool>, char> my_coding_list;
  deque<bool> my_coding;
  tree_to_dict_of_binary_codage(my_coding_list, my_coding, my_tree);

  // on libère la mémoire de notre arbre, il nous sert plus
  free_tree(my_tree);

  // ecrire le fichier
  write_compressed_file("my_compressed_text.bz2", the_text_file,
                        my_coding_list);

  read_file("my_compressed_text.bz2");
}

int main(void) {
  compression("my_text.txt");
  return 0;
}
// {

//   //  vector<string> my_strings;
//   //  int index_of_original_string =
//   all_possible_rotation_of_string("textuel",
//   //  my_strings); string my_coding_from_rotation =
//   get_the_string_to_write_in_compressed_file(my_strings);
//   //
//   //  cout << "ma chaîne codée : " << my_coding_from_rotation << endl;
//   //
//   //  vector<string> reconstruct_string;
//   //  string reconstruct =
//   //  recontruct_original_string(my_coding_from_rotation,reconstruct_string,
//   //  index_of_original_string); cout << "ma chaîne recontruite : " <<
//   //  reconstruct << endl;

//   // on transforme le fichier my_text.txt en vecteur
//   string the_text_file;
//   text_file_to_string("my_text.txt", the_text_file);
//   // the_text_file =
//   //
//   "azertyuiopqsdfghaeaaajjhahezazeifeaoziuaoasoduiqpoqcdqdaacbbq,ncbqghcvhqdghc;q:!;,;;nhjqgdyquguayaireaas7894561230+$*ù$&é(-aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffggggggggggggggggggggggggggggggggggggggggggggggggggghhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhkjjkeè_çà)ù%£µ%/§.?";
//   cout << "the text : " << the_text_file
//        << ", la taille : " << the_text_file.size() << endl;
//   // the_text_file = "Wikipedia";
//   vector<string> my_strings;
//   int index_of_original_string =
//       all_possible_rotation_of_string(the_text_file, my_strings);
//   // cout << "affichage de tout les string : " << endl;
//   // print_string_vector(my_strings);
//   string my_coding_from_rotation =
//   get_the_string_to_write_in_compressed_file(my_strings);
//   // cout << "my index : " << index_of_original_string << endl;
//   auto my_test = string_to_vector_of_occurrences(my_coding_from_rotation);
//   // je devrais plutôt faire : file to text, text to vector, et enregistrer
//   le
//   // texte quelque part car il faut retranscrire le texte après
//   //

//   // on affiche les occurences de chaque caractères
//   /*for (const auto &p : my_test) {
//     std::cout << p.first << " => " << p.second << '\n';
//     }*/

//   // On transforme notre vecteur d'occurence en arbre
//   auto my_tree = vector_of_occurences_to_tree(my_test);
//   // cout << "parcours en profondeur  : " << endl;
//   // parcours_profondeur(my_tree);

//   // on transforme notre arbre d'occurence en une liste de codage pour chaque
//   // lettre
//   map<deque<bool>, char> my_coding_list;
//   deque<bool> my_coding;
//   tree_to_dict_of_binary_codage(my_coding_list, my_coding, my_tree);

//   // on libère la mémoire de notre arbre, il nous sert plus
//   free_tree(my_tree);

//   // on affiche les caractères et leur codage
//   // for (auto x : my_coding_list) {
//   //   cout << "my char : " << x.second << endl;
//   //   cout << "my list of bool : " << endl;
//   //   for (auto y : x.first) {
//   //     cout << y << ", ";
//   //   }
//   //   cout << endl;
//   // }

//   cout << "------------" << endl << "dico normal : " << endl;

//   for (auto x : my_coding_list) {
//     cout << "my char : " << x.second << endl;
//     // cout << "my list of bool : " << endl;
//     // for (auto y : x.first) {
//     //   cout << y << ", ";
//     // }
//     // cout << endl;
//   }

//   cout << "------------" << endl << "dico en binaire : " << endl;

//   vector<int8_t> my_vec = dict_in_binary_form(my_coding_list);

//   int tour_restant_pour_taille_codage = 1; // c'est en bit char actual_char;
//   char actual_char;
//   int size_of_codage;
//   list<int8_t> codage_of_char;

//   map<deque<bool>, char> my_dict_to_fill;
//   for (int8_t element : my_vec) {

//     //    cout << "my char : " << element << endl;

//     if (tour_restant_pour_taille_codage ==
//         1) { // dans le cas où on est au bit du char
//       // cout << "my char : " << element << endl;
//       actual_char = (char)element;
//     } else if (tour_restant_pour_taille_codage ==
//                0) { // si on est ) la taille du codage d'un char
//       // cout << "taille : " << (int) element << endl;
//       //  j'ai mus inférieur ou égal, car si c'est égal bah un byte suffit
//       if ((int)element <= 8) { // si la taille du codage est inférieur à 1
//       byte
//         tour_restant_pour_taille_codage =
//             1 + 1 + 1; // un byte pour le codage, puis un pour le caractère
//                        // suivant, et un je sais plus mais ça marche pas
//                        sinon

//         cout << "tour restant : " << tour_restant_pour_taille_codage << endl;
//       } else { // si la taille est d'au moins la taille d'yn byte
//         // cout << "parencacahuere" << endl;
//         tour_restant_pour_taille_codage =
//             (int)element / 8; // on ajoute un tour pour chaque byte de codage
//         // ça serait pas plutôt / 8 du coup  ?

//         cout << "tour restant : " << tour_restant_pour_taille_codage << endl;
//         tour_restant_pour_taille_codage +=
//             3; // on ajoute un byte pour le char suivant, et les deux autres
//             jsp
//                // mais sinon ça marche pas
//         cout << "tour restant : " << tour_restant_pour_taille_codage << endl;
//       }

//       size_of_codage = (int)element;
//       //  cout << "tour restant : " << tour_restant_pour_taille_codage <<
//       endl;
//     } else { // si on est au codage en lui même
//       codage_of_char.push_back(element);
//       //    cout << (int) element << endl;
//       if (tour_restant_pour_taille_codage ==
//           2) { // si on a ajouter le dernier élément de codage_of_char
//         // appel de la fonction add_to_dict

//         cout << "my char : " << actual_char << endl;
//         cout << "size of codage : " << size_of_codage << endl;
//         add_a_codage_to_dict(my_dict_to_fill, actual_char, size_of_codage,
//                              codage_of_char);
//       }
//     }
//     tour_restant_pour_taille_codage--;
//   }
//   cout << "noice : " << endl;
//   for (auto element : my_dict_to_fill) {
//     if (element.second == ';') {
//       cout << "my char : " << element.second << endl;
//       cout << "my list of bool : " << endl;
//       for (auto my_bit : element.first) {
//         cout << my_bit << ", ";
//       }
//       cout << endl;
//     }
//   }

//   // bon mon dico marche, mais faut que je puisse obtenir la traduction de
//   // codages binaires depuis les bytes Pour ce faire, j'ai besoin de
//   transformer
//   // tout les bytes dédié au codage en liste de bit. On peut savoir ça en
//   // arrondissant la taille du codage en bit au multiple supérieur de 8 (on
//   // enregistre ça dans taille_totale), puis en divisant par 8.

//   // ensuite, on enlève les bit en trop à la fin, en faisant taille_totale -
//   // taille du codage et on transforme ce qui reste, une liste d'int de 0 et
//   1,
//   // en un deque<bool>, on enregistre ça dans un map qui associe ce codage au
//   // caractère correspondant
//   write_compressed_file("my_compressed_text.bz2", the_text_file,
//                         my_coding_list);

//   read_file("my_compressed_text.bz2");
//   // transcrire le message dans un fichier binaire, en f
//   // parcours en profondeur fonctionne, l'arbre est bien construit
//   // maintenant, je dois transformer mon arbre en liste de codage, réussi !
//   // passons à la transcription dans un fichier ? C'est plutôt dur ça
//   // En vrai, go nettoyer le code
//   return 0;
// }
