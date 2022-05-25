#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string.h>
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

// renvoie un node parent des deux nodes en paramètre
Node *node_from_nodes(Node *my_first_node, Node *my_second_node) {
  return new Node{' ', my_first_node->weight + my_second_node->weight,
                  my_first_node, my_second_node};
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

  // on renvoie le dernier noeud, la tête de notre arbre
  return create_node_from_two_smallest(my_nodes);
  ;
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
// utile pour la transformée de Burrows-Wheeler
bool vector_char_alphabetic_comparison(vector<char> x, vector<char> y) {


  auto x_it = x.begin();
  auto x_end = x.end();

  auto y_it = y.begin();
  auto y_end = y.end();
  for (; x_it != x_end; x_it++, y_it++) {
    if (*x_it < *y_it) {
      return true;
    } else if (*x_it > *y_it) {
      return false;
    }
  }
  return false;
}

// range le vecteur dans l'ordre alphabetique
void sorting_in_alphabetical_order(vector<string> &my_list_of_strings) {
  sort(my_list_of_strings.begin(), my_list_of_strings.end(),
       [](const auto &x, const auto &y) { return x < y; });
}

void sorting_in_alphabetical_order_deque(deque<string> &my_list_of_strings) {
  sort(my_list_of_strings.begin(), my_list_of_strings.end(),
       [](const auto &x, const auto &y) { return x < y; });
}


bool compare(const std::string &first, const std::string &second) {
  int i = first.length() - 1;
  string save_first;
  string save_second;
  while (i >= 0) {
    save_first.push_back(first[i]);
    save_second.push_back(second[i]);

    if (save_first < save_second) {
      return true;
    } else if (save_first > save_second) {
      return false;
    }
    --i;
  }
  return false;
}

void sorting_in_alphabetical_order_from_end_v2(vector<string> &my_list_of_strings) {
  sort(my_list_of_strings.begin(), my_list_of_strings.end(), compare);
}

void sorting_in_alphabetical_order_from_end(
    vector<string> &my_list_of_strings) {
  sort(my_list_of_strings.begin(), my_list_of_strings.end(),
       [](const auto &x, const auto &y) {
         int i = 0;
         auto x_it = x.end() - 1;
         auto y_it = y.end() - 1;

         while (*(x_it) == *(y_it)) {
           x_it--;
           y_it--;
         }
         if (*(x_it) > *(y_it)) {
           return true;
         } else {
           return false;
         }
       });
}

// transforme notre string en un vecteur contenant tout les rotations possible,
// et retourne l'index de la string originale
int all_possible_rotation_of_string(string my_string_to_rotate,
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
  int i = 0;
  for (auto s : my_list_of_strings) {
    if (s == my_original_string) {
      return i;
    }
    i += 1;
  }
  return i;
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

void sorting_char_vector_in_alphabetical_order(
    vector<deque<char>> &my_list_of_strings) {
  sort(my_list_of_strings.begin(), my_list_of_strings.end(),
       [](const auto &x, const auto &y) {
         auto x_it = x.begin();
         auto x_end = x.end();

         auto y_it = y.begin();
         auto y_end = y.end();
         for (; x_it != x_end; x_it++, y_it++) {
           if ((*x_it) > (*y_it)) {
             return true;
           }
         }
         return false;
       });
}

// reconstruit la string originale
string recontruct_original_string(string &my_rotated_string,
                                  vector<string> &reconstruct_string,
                                  int index_of_original_string) {
  const clock_t begin_time = clock();

  int string_length = my_rotated_string.size();

  // vector<string> reconstruct_string_beg;
  // reconstruct_string_beg.resize(string_length);

  reconstruct_string.resize(string_length);
  // on ajoute toute les lettre rotate à notre
  int index = 0;
  for (char my_char : my_rotated_string) {
    // string tmp(1, my_char);
    // reconstruct_string[index].insert(0, tmp);
    reconstruct_string[index].push_back(my_char);

    // string tmp(1, my_char);
    // reconstruct_string_beg[index].insert(0, tmp);

    index++;
  }
  cout << "finish" << endl;

  // sorting_in_alphabetical_order(reconstruct_string_beg);
  // sorting_in_alphabetical_order(reconstruct_string);
  sorting_in_alphabetical_order_from_end_v2(reconstruct_string);
  cout << "sort finish, taille string : " << string_length << endl;

  int i;
  for (int x = 0; x < string_length - 1; x++) {
    i = 0;
    for (char my_char : my_rotated_string) {
      // string tmp(1, my_char);
      // reconstruct_string_beg[i].insert(0, tmp);

      // string tmp(1, my_char);
      // reconstruct_string[i].insert(0, tmp);
      reconstruct_string[i].push_back(my_char);
      i += 1;
    }

    // sorting_in_alphabetical_order(reconstruct_string_beg);
    sorting_in_alphabetical_order_from_end_v2(reconstruct_string);
    // cout << "_____________" << endl;
    // for (auto element : reconstruct_string) {
    //   cout << "[" << element << "]" << endl;
    // }
  }

  std::cout << "temps écoulé : "
            << float(clock() - begin_time) / CLOCKS_PER_SEC;

  string the_text_file = text_file_to_string("my_text.txt");
  for (auto my_string : reconstruct_string) {
    if (the_text_file == my_string) {
      cout << endl << "réussi : " << endl;
    }
  }
  auto beginning = reconstruct_string.begin();
  auto ending = reconstruct_string.end() - 1;
  string str_to_print;
  cout << "taille : " << reconstruct_string.size() << endl;
  // for (; ending != beginning; ending--) {
  //   str_to_print += *ending;
  // }
  // cout << str_to_print << endl;
  // cout << "variante : " << endl;
  // for(string a_string : reconstruct_string){
  //   cout <<  a_string<< endl;
  // }

  reverse(reconstruct_string[index_of_original_string].begin(),
          reconstruct_string[index_of_original_string].end());
  return reconstruct_string[index_of_original_string];
  // return float(clock() - begin_time) / CLOCKS_PER_SEC;
}


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
  int8_t my_byte = '\0';
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

// on prend un int8_t, et pour chacun de ses bits, on en met un dans
// list_of_bits
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
  int lap_remaining_before_coding_size =
      1; // tour restant avant la taille du codage
  char actual_char;
  int size_of_codage;
  list<int8_t> codage_of_char;
  // pour chaque bit de mon dico en binaire
  for (int8_t element : my_dict_in_binary) {
    if (lap_remaining_before_coding_size ==
        1) { // le bit est au char, on enregistre le char
      actual_char = (char)element;
    } else if (lap_remaining_before_coding_size ==
               0) {            // on est à la taille du codage d'un char
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
      if (lap_remaining_before_coding_size ==
          2) { // si on a ajouter le dernier élément de codage_of_char
        add_a_codage_to_dict(my_dict, actual_char, size_of_codage,
                             codage_of_char);
      }
    }
    lap_remaining_before_coding_size--; // on décrémente le nombre restant
  }
}

map<deque<bool>, char> retrieve_dict(vector<int8_t> &my_dict_in_binary,
                                     vector<int8_t> &buffer, int dico_len) {
  my_dict_in_binary.reserve(dico_len);
  for (int i = 3; i < dico_len + 3; i++) { // + 3 car on compte par les premiers
                                           // bits, dédié à des autres choses
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
string retrieve_content_from_compressed_file(string file_name) {
  ifstream input(file_name, std::ios::binary);
  // on récupère le contenu du fichier compressé
  vector<int8_t> buffer(std::istreambuf_iterator<char>(input), {});

  // la taille du dico, comme ça on sait où il se commence et fini
  int dico_len = buffer[1] * 128 + buffer[2];

  // on récupère notre dictionnaire
  vector<int8_t> my_dict_in_binary;
  map<deque<bool>, char> my_dict =
      retrieve_dict(my_dict_in_binary, buffer, dico_len);

  // on récupère l'index du début de l'écriture du texte
  int text_begin = dico_len + 3; // les trois premiers bits dédié à des choses

  return retrieve_text(buffer, text_begin, my_dict);
}

void decompression(string file_to_decompress, string output_file) {
  std::ofstream out(output_file);
  out << retrieve_content_from_compressed_file(file_to_decompress);
  out.close();
}

void compression(string to_compress, string output_file) {
  string the_text_file = text_file_to_string(to_compress);

  /*vector<string> my_strings;
  int index = all_possible_rotation_of_string(the_text_file, my_strings);

  string string_from_rotation =
      get_the_string_to_write_in_compressed_file(my_strings);

  vector<string> reconstruct_string;
  string is_init = recontruct_original_string(string_from_rotation,
  reconstruct_string, index); cout << "is init ? : " << is_init << endl;*/

  vector<pair<char, int>> occurences_of_each_char =
      string_to_vector_of_occurrences(the_text_file);

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
  write_compressed_file(output_file, the_text_file, my_coding_list);

  // cout << "affichage : " <<
  // retrieve_content_from_compressed_file("my_compressed_text.bz2") << endl;
  cout << "Compression completed!" << endl;
}

void say_help_message() {
  cout << "Compression program. Usage : " << endl
       << "./a.out [-option] [input] [output (optional)]" << endl
       << endl;
  cout << "   Options : " << endl
       << "   -c : compress" << endl
       << "   -d : decompress" << endl
       << "   -h or --help : help (no input neither output)" << endl;
}

bool is_file_exist(string filename) {
  ifstream f(filename.c_str());
  // https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
  return f.good();
}
bool is_correct_extension_file(string file_name_to_verify, string extension) {
  return (file_name_to_verify.size() < 5 ||
          file_name_to_verify.substr(file_name_to_verify.size() - 4) !=
              ("." + extension));
}

int is_correct_file(string file_to_compress, string extension) {

  if (!is_file_exist(file_to_compress)) { // on vérifie si le fichier existe
    cout << "Error : This file doesn't exist..." << endl;
    return -1;
  }
  if (is_correct_extension_file(
          file_to_compress,
          extension)) { // on vérifie si c'est un fichier texte
    cout << "Error : It's not a .txt file..." << endl;
    return -1;
  }
  return 0;
}

bool is_output_file_can_be_written(string output_file) {
  string response;
  if (is_file_exist(output_file)) {
    cout << "The file " << output_file
         << " already exists, do you want overwrite it ? y/n : ";
    while (response != "y" && response != "n") {
      cin >> response;
      if (response != "y" && response != "n") {
        cout << "Please write y or n : " << endl;
      }
    }
  } else {
    return true;
  }
  if (response == "y") {
    return true;
  }
  return false; // -> "n"
}

int main(int argc, char *argv[]) {
  if (argc == 1) { // si aucun argument
    say_help_message();
  } else if (argc == 2 && (!strcmp(argv[1], "-h") ||
                           !strcmp(argv[1], "--help"))) { // si aide
    say_help_message();
  } else if ((argc == 4 || argc == 3) &&
             !strcmp(argv[1], "-c")) { // si compression

    // on vérifie si le fichier existe, et qu'il est un .txt
    string file_to_compress(argv[2]);
    if (is_correct_file(file_to_compress, "txt") == -1) {
      return EXIT_FAILURE;
    }

    // on vérifie si le fichier output existe déjà, si oui on vérifie si l'utilisateur veut overwrite
    string output_file;
    if (argc == 4) {
      output_file += argv[3]; // si un nom de fichier output est précisé
    } else {
      output_file += argv[2]; // sinon il prend celui du fichier à compresser
      // on enlève l'extension .txt
      for (int i = 0; i < 4; i++) {
        output_file.pop_back();
      }
    }
    output_file += ".bz2"; // on ajoute l'extension

    if (!is_output_file_can_be_written(
            output_file)) { // si il existe déjà, et qu'on veut pas écrire par
                            // dessus
      return 0;
    }

    // compression
    cout << "Compression in progress..." << endl;
    compression(file_to_compress, output_file);
    cout << "Compression finished! The output file is : " << output_file
         << endl;
  } else if ((argc == 4 || argc == 3) && !strcmp(argv[1], "-d")) {

    // on vérifie si le fichier à décompresser est correct (existe, et .bz2)
    string file_to_decompress(argv[2]);
    if (is_correct_file(file_to_decompress, "bz2") == -1) {
      return EXIT_FAILURE;
    }

    // on créer ajoute nom de l'output file, s'il n'est pas précisé, c'est le même que le fichier à décompresser
    string output_file;
    if (argc == 4) {
      output_file += argv[3];
    } else {
      output_file += argv[2];
      // on enlève l'extension .bz2
      for (int i = 0; i < 4; i ++){
        output_file.pop_back();
      }
    }
    output_file += ".txt";

    if (!is_output_file_can_be_written(
            output_file)) {
      // si il existe déjà, et qu'on veut pas écrire par
                            // dessus, on arrête le programme
      return 0;
    }

    // décompression
    cout << "Decompression in progress..." << endl;
    decompression(file_to_decompress, output_file);
    cout << "Decompression finished! The output file is : " << output_file
         << endl;
  } else {
    cout << "Error : bad usage of arguments... " << endl;
    return EXIT_FAILURE;
  }
  return 0;
}

