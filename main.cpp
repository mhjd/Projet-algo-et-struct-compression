#include <algorithm>
#include <cstdint>
#include <deque>
#include <fstream>
#include <iostream>
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

// prend un file en paramètre, et renvoie une liste de tuples avec les
// occurences Fonctionne !
void file_to_string(string file_name, string &my_string) {
  ifstream read_file(file_name);

  char c;

  // tuple<char> search;
  while (read_file.get(c)) {
    my_string.push_back(c);
  }
  read_file.close();
}
vector<pair<char, int>> string_to_vector(string &my_string) {
  map<char, int> my_chars_occurences;
  cout << endl;
  char c;

  // tuple<char> search;
  for (auto c : my_string) {
    auto search = my_chars_occurences.find(c);
    if (search == my_chars_occurences.end()) {
      my_chars_occurences.emplace(std::make_pair(c, 1));
    } else {
      my_chars_occurences[c]++;
    }
  }

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

// vérifier s'il y a plus de deux éléments avant d'appeler
void insert_with_sorting(list<Node *> &my_nodes, Node *&my_new_node) {

  // cout << "valeur du poids de my_new_node : " << my_new_node -> weight <<
  // endl << "---->>>";
  bool insert = false;
  for (auto it = my_nodes.begin(); it != my_nodes.end(); ++it) {
    // cout << "la valeur du poids de l'itérateur: " << ((*it) -> weight) <<
    // endl;
    if (my_new_node->weight < (*it)->weight && !insert) {
      my_nodes.insert(it, my_new_node);
      insert = true;
      // it = my_nodes.end();
    }
  }
  if (!insert) { // si c'est pas inséré, c'est que tout les éléments sont plus
                 // petits que lui, donc on ajoute à la fin
    my_nodes.push_back(my_new_node);
  }
  //cout << "liste après insertion : " << endl;

  // for (auto it = my_nodes.begin(); it != my_nodes.end(); ++it) {
  //   cout << (*it)->weight << ", ";
  // }
  // cout << endl;
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

Node *vector_pair_to_tree(vector<pair<char, int>> my_occcurences) {
  // méthode :
  // On prend une liste de noeuds, on lui ajoute tout nos noeud dans l'ordre
  // croissant d'occurence, on prend les deux plus petits éléments (s'il y en a
  // au moins deux) qui sont au début, on en fait un nouveau noeud, on enlève
  // ces deux plus petits noeuds, et on ajoute le nouveau noeud au bon
  // emplacement de sorte à ce que ça reste dans l'ordre croissant on fait ça
  // jusqu'à qu'il nous reste qu'un seul noeud, que l'on renvoie

  list<Node *> my_nodes;
  for (auto my_occurence : my_occcurences) {
    my_nodes.push_back(
        new Node{std::get<0>(my_occurence), std::get<1>(my_occurence)});
  }

  /*
  cout << "tout fonctionne bien dans le test : " << endl;
  for (auto i : my_nodes){
  cout << "letter : " << i -> character << ", occurences : " << i -> weight
  << endl;
  }
  cout << "fin du test " << endl;
  */

  while (my_nodes.size() > 2) {
    //cout << "liste avant insertion : " << endl;
    for (auto it = my_nodes.begin(); it != my_nodes.end(); ++it) {
      cout << (*it)->weight << ", ";
    }
    cout << endl;

    Node *my_smallest_element = my_nodes.front();
    my_nodes.pop_front();
    Node *my_second_smallest_element = my_nodes.front();
    my_nodes.pop_front();

    // cout << "my nodes before size " << my_nodes.size() << endl;
    // print_list(my_nodes);
    //     my_new_node = node_from_nodes(my_smallest_element,
    //     my_second_smallest_element);
    Node *my_new_node = new Node{
        ' ', my_smallest_element->weight + my_second_smallest_element->weight,
        my_smallest_element, my_second_smallest_element};
    // cout << "my inserted new node : " << my_new_node->weight << endl;
    insert_with_sorting(my_nodes, my_new_node);
    // cout << "-------------" << endl;
    if (my_nodes.size() == 2) {
    }
    // cout << "my nodes after size " << my_nodes.size() << endl;
    // print_list(my_nodes);
    //  faudrait afficher la liste de wight à chaque tour, et voir
  }

  Node *my_smallest_element = my_nodes.front();
  my_nodes.pop_front();
  Node *my_second_smallest_element = my_nodes.front();
  my_nodes.pop_front();

  Node *my_node_to_return = new Node{
      ' ', my_smallest_element->weight + my_second_smallest_element->weight,
      my_smallest_element, my_second_smallest_element};
  // cout << "my new node " << my_node_to_return->weight << endl;
  //  parcours_profondeur(my_node_to_return);
  // je m'arrête ici
  /*for(auto it = my_nodes.begin(); it != my_nodes.end(); ++it){
    cout << it -> weight << ", ";
    }
    cout << endl;*/

  return my_node_to_return;
}

void tree_to_binary(map<deque<bool>, char> &coding_list, deque<bool> my_coding,
                    Node *my_tree) {

  if (my_tree->node_right == NULL && my_tree->node_right == NULL) {
    // ajouter au codage
    coding_list.emplace(make_pair(my_coding, my_tree->character));
  }
  // à chaque appel, on dupplique le vecteur booléen, et on fait l'appel
  // récursif :
  auto my_duplicate_coding = my_coding;

  // - y en a un on lui appendera false (donc 0), + appel récursif sur la gauche
  my_coding.push_back(false);
  if (my_tree->node_left != NULL) {
    tree_to_binary(coding_list, my_coding, my_tree->node_left);
  }
  // - un on lui appendera true(donc 1), + appel récursif sur la droite
  my_duplicate_coding.push_back(true);
  if (my_tree->node_right != NULL) {
    tree_to_binary(coding_list, my_duplicate_coding, my_tree->node_right);
  }

  // on fait ça jusqu'à atteindre le bout de l'arbre, càd quand les deux noeud
  // enfant == false quand c'est le cas, on append le vecteur de booléen +
  // caractère (tuple vector<bool> char), dans l'argument correspondant à la
  // liste de codage listes des arguments : vector<pair<char, vector<bool>>>
  // &coding_list vecto<bool>& my_coding, momnçarbre
}
// Node* tuples_to_tree(map<char, int> my_tuples){
//
// }

void sorting_alphabet(vector<string> &my_list_of_strings) {
  sort(my_list_of_strings.begin(), my_list_of_strings.end(),
       [](const auto &x, const auto &y) { return x < y; });
}

int all_possible_rotation(string my_string_to_rotate,
                          vector<string> &my_list_of_strings) {

  string my_original_string = my_string_to_rotate;
  my_list_of_strings.push_back(my_string_to_rotate);
  for (int i = 0; i < (int)my_string_to_rotate.size() - 1; i++) {
    rotate(my_string_to_rotate.begin(), my_string_to_rotate.begin() + 1,
           my_string_to_rotate.end());
    // cout << my_string_to_rotate << endl;
    my_list_of_strings.push_back(my_string_to_rotate);
  }
  sorting_alphabet(my_list_of_strings);

  int i = 0;
  for (auto s : my_list_of_strings) {
    if (s == my_original_string) {
      return i;
    }
    i += 1;
  }
  return i;
}

string coding_text(vector<string> &my_list_of_string_rotated) {
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
  sorting_alphabet(reconstruct_string);

  int i;
  for (int x = 0; x < string_length - 1; x++) {
    i = 0;
    for (char my_char : my_rotated_string) {
      string tmp(1, my_char);
      reconstruct_string[i].insert(0, tmp);
      i += 1;
    }
    sorting_alphabet(reconstruct_string);
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

void add_bits_in_list(char my_char_to_add,
                      map<deque<bool>, char> &my_coding_list,
                      list<int> &my_list_of_bits) {
  //cout << "lettre à chercher : " << my_char_to_add << endl;
  deque<bool> save_list_bit;
  bool find = false;
  for (auto it = my_coding_list.begin(); it != my_coding_list.end() && !find;
       ++it) {
    if (it->second == my_char_to_add) {
      save_list_bit = it->first;
      find = true;
    }
  }
  for (bool i : save_list_bit) {
    if (i) {
      my_list_of_bits.push_back(1);
    } else {
      my_list_of_bits.push_back(0);
    }
  }
}

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
// bon maintenant on sait qu'on peut écrire un binaire avec seulement un tableau
// de int8_t
// faut donc que je transforme ma liste en tableau de int8_t, que je peux
// ensuite récupérer avec fread
// et après, faut retransformer ce tableau de int8_t en liste d'int
// et après passer à la partie détermination de quel lettre c'est à partir du
// dico
void list_to_array(list<int> &my_list_of_bits, int8_t *my_tab) {
  // i - 1 < ... car on commence à 1, donc faut compenser
  // on commencer à 1, car l'indice 0 contient  le nb de bit ajouter
  cout << "taille chacal : " << ((my_list_of_bits.size() / 8)) << endl;
  auto number_of_repetition = (my_list_of_bits.size() / 8) ; // +1?
  for (int i = 1; (i - 1) < number_of_repetition; i++) {
    cout << i - 1 << " < " << ((my_list_of_bits.size() / 8) + 1) << endl;
    my_tab[i] = create_chunk_bit(my_list_of_bits);
    cout << "my tab " << (int) my_tab[i] << endl;
    cout << "my list of bit size  : " << my_list_of_bits.size() << endl << endl;
    // bizarre bizarre, le dernier élément de array ne passe pas
  }

  //my_tab[i] = create_chunk_bit(my_list_of_bits);
}

// permet d'ajouter les bit nécessaire pour avoir un multiple de 8
// renvoie le nombre de bit ajouter 
int8_t adding_bit_necessary(list<int> &my_list_of_bits){
  int nb_bit_to_add = 0;
  if (my_list_of_bits.size() < 8) {
    nb_bit_to_add = 8 - my_list_of_bits.size();
  } else if (my_list_of_bits.size() % 8 != 0) {
    nb_bit_to_add = 8 - my_list_of_bits.size() % 8;
  }
  int8_t bit_added = nb_bit_to_add;
  for (; nb_bit_to_add > 0; nb_bit_to_add--) {
    my_list_of_bits.push_back(0);
  }
  return bit_added;
}

/*vector<int8_t> dict_in_binary(map<deque<bool>, char> &my_coding_list) {
  vector<int8_t> my_dictionnary_in_binary;
  int8_t size_codage_in_bit;
  for(auto element : my_coding_list){
    // on ajoute notre caractère à notre liste
    my_dictionnary_in_binary.push_back((int8_t) element.second);
   // on récupère la taille du codage binaire, et on l'ajoute à notre liste de nombre binaire
    size_codage_in_bit = element.first.size();
    my_dictionnary_in_binary.push_back(size_codage_in_bit);
    // maintenant, on créer notre liste de bit nécessaire pour mettre le codage en binaire
    // il faut que ça soit un multiple de 8 donc faut ajouter des 0 si c'est pas le cas
    list<int> my_list_of_bits;
    for (auto my_bit : element.first){
      if (my_bit){
        my_list_of_bits.push_back(1);
      } else {
        my_list_of_bits.push_back(0);
      }
    }
    adding_bit_necessary(my_list_of_bits);
    // 
  }
  }*/

void write_compressed_file(string file_name, string &original_text,
                           map<deque<bool>, char> &my_coding_list, int8_t * tab_poubelle) {

  // il faut que je fasse en sorte d'écrire le dictionnaire, et normalement
  // après c'est bon pour écrire le dico, on écrit d'abord le char, puis l'on écrit le nombre de bit nécessaier au codage binaire, puis on écrit le codage binaire 


  // on écrit tout ça dans un vector int8_t pour ensuite récupérer la taille de bit allouer
  // on écrit ça dan  ma list of bits, et on pense à ajouter un argument pour add_bits_in_list pour qu'il commence à partir de la fin du array


  // bit nécessaire pour le codage,

  // créer une liste de bit (list int)
  list<int> my_list_of_bits;
  //cout << "taille de list of bits " << my_list_of_bits.size() << endl;
  for (char my_char : original_text) {
    add_bits_in_list(my_char, my_coding_list, my_list_of_bits);
    // cout << "taille de list of bits " << my_list_of_bits.size() << endl;
  }
  // ensuite compter le nombre de bit nécessaire pour que ça soit un multiple
  // de 8. pour ce faire : 8 - nb_bit_total % 8, quand nb_bit_total % 8 != 0, si
  // c'est = 0, alors pas besoin, si le nb de bit total est < 8 alors on fait
  // direct 8 - nb_bit_total
  int8_t bit_added = adding_bit_necessary(my_list_of_bits); // on ajoutera ça au début de notre fichier
                                    // pour ne pas lire les derniers bit

  //cout << "taille de ma liste de bits : " << my_list_of_bits.size() << endl;
  // (my_list_of_bits.size() / 8) -> nombre de byte nécessaire
  // + 1 : premier élément, celui donnant le nb de bit ajouter
  auto array_size = (my_list_of_bits.size() / 8) + 1;
  //cout << "array size vaut  :" << array_size << endl;

  int8_t array[array_size];

  //cout << "array sizen"
  array[0] = bit_added;
  list_to_array(my_list_of_bits, array);
  // sûrement un problème au niveau de array
  // en l'occurence au niveau de list_to_array

  // c'est la méthode la plus simple que j'ai trouver pour écrire un tab dans un
  // fichier
  FILE *pFile;
  pFile = fopen(file_name.c_str(), "wb");
  fwrite(array, 1, array_size, pFile);
  fclose(pFile);

  //cout << "écriture :  " << endl;
  for (int i = 0; i < array_size; i++) {
    //tab_poubelle[i] = array[i];
    cout << "avant : " << (int) array[i] << endl;
  }

  // for (int i : my_list_of_bits) {
  //   cout << i;
  // }
  // cout << endl;

  // le premier byte est = au nb de bit à 0 en plus à la fin ? ou alors osef car
  // on a une détermination automatique ? Vas y osef pour l'instant nan ? ou
  // pas, car 000 peut être = à un truc préexistant dans notre liste de codage
}

// maintenant qu'on a notre read_file, on le transforme en liste de bit, on
// enlève les bit inutile à la fin, on détermine les caractère à partir du dico
// (avec find), et ce jusqu'à la fin de la liste de bit.
// enfin, on transforme notre chaine de caractère grâce au délire de baus

void chunk_to_list_bit(list<int> &list_of_bits, int8_t chunk_of_bits) {

  int bit;

  cout << "mon byte : " <<  (int) chunk_of_bits << endl;
  for (int n = 0; n < 8; n++) {
    bit = (chunk_of_bits >> (8 - 1 - n)) & 1;
    //    cout << "mon bit : " <<  bit << endl;
    list_of_bits.push_back(bit);
  }
}

char find_char(map<deque<bool>, char> &my_coding_list,
               list<int> &list_of_bits) {
  bool added = false;
  // liste de bit d'un codage d'un caractère, on lui met des bit de notre liste
  // de bit jusqu'à avoir trouvé un codage de caractère
  deque<bool> list_of_bit_of_coding;
  while (1) {
    // on ajoute un élément à notre codage
    if (list_of_bits.front() == 1) {
      list_of_bit_of_coding.push_back(true);
    } else {
      list_of_bit_of_coding.push_back(false);
    }
    // cout << "ajout de " << list_of_bits.front() << endl;
    //  on la retire de notre liste de bit tout coller
    list_of_bits.pop_front();

    // on essaye de trouvé si notre codage existe
    if (my_coding_list.count(list_of_bit_of_coding) >
        0) { // on compte le nombre de clé qui valent list_of_bit_of_coding
      // cout << "lettre : " << my_coding_list[list_of_bit_of_coding] << endl;
      return my_coding_list[list_of_bit_of_coding];
    }
  }
}

void read_file(string file_name, map<deque<bool>, char> &my_coding_list, int8_t* my_array_writed) {
  std::ifstream input(file_name, std::ios::binary);
  std::vector<int8_t> buffer(std::istreambuf_iterator<char>(input), {});
  //cout << "testage d'égalité " << endl;
  for(int i = 0; i < buffer.size(); i++){
    //cout << "is equal : " << (buffer[i] == my_array_writed[i]) << endl;
  }

  // cout << "lecture : " << endl;
  // for (auto element : buffer){
  //   cout << element << endl;
  // }
  int bit_added = buffer[0]; // bit ajouter à la fin, qu'on devra enlever à
                             // notre liste de bit

  list<int> list_of_bits;
  for (int i = 1; i < buffer.size(); i++) {
    chunk_to_list_bit(list_of_bits, buffer[i]);
  }

  //cout << "seg fault ? :  " << list_of_bits.size() << endl;
  //cout << "number of bit added : " << bit_added << endl;
  for (; bit_added > 0; bit_added--) {
    list_of_bits.pop_back();
  }
  string my_text_recompose;
  // c'est tout a fait normal que la taille de la liste de bits soit supérieur à la taille de la string
  // car un caractère peut faire plusieurs bits, en l'occurence ici, a et b font deux bits, donc ça fait 18 au lieu de 16
  // maintenant, il est où le problème

  // kutta l'état entre ici et là bas devrait être le même, or c'est pas le
  // cas
  // cout << "printage de bits" << endl;
  // for (auto i : list_of_bits){
  //   cout << i << endl;
  // }


  // cout << "seg fault ? :  " << list_of_bits.size() << endl;
  char my_char_to_add;
  while (list_of_bits.size() > 0) {
    my_char_to_add = find_char(my_coding_list, list_of_bits);
    // cout << "sef fault char : "<< my_char_to_add << ", taille bits : " <<
    //  list_of_bits.size() << endl;
    my_text_recompose += my_char_to_add;
  }
  cout << "affichage : " << my_text_recompose << endl;
}
int main(void) {

  //  vector<string> my_strings;
  //  int index_of_original_string = all_possible_rotation("textuel",
  //  my_strings); string my_coding_from_rotation = coding_text(my_strings);
  //
  //  cout << "ma chaîne codée : " << my_coding_from_rotation << endl;
  //
  //  vector<string> reconstruct_string;
  //  string reconstruct =
  //  recontruct_original_string(my_coding_from_rotation,reconstruct_string,
  //  index_of_original_string); cout << "ma chaîne recontruite : " <<
  //  reconstruct << endl;

  // on transforme le fichier my_text.txt en vecteur
  string the_text_file;
  file_to_string("my_text.txt", the_text_file);
  the_text_file = "chocolat cacao café !";
  cout << "the text : " << the_text_file
       << ", la taille : " << the_text_file.size() << endl;
  // the_text_file = "Wikipedia";
  vector<string> my_strings;
  int index_of_original_string =
      all_possible_rotation(the_text_file, my_strings);
  // cout << "affichage de tout les string : " << endl;
  // print_string_vector(my_strings);
  string my_coding_from_rotation = coding_text(my_strings);
  //cout << "my index : " << index_of_original_string << endl;
  auto my_test = string_to_vector(my_coding_from_rotation);
  // je devrais plutôt faire : file to text, text to vector, et enregistrer le
  // texte quelque part car il faut retranscrire le texte après
  //

  // on affiche les occurences de chaque caractères
  /*for (const auto &p : my_test) {
    std::cout << p.first << " => " << p.second << '\n';
    }*/

  // On transforme notre vecteur d'occurence en arbre
  auto my_tree = vector_pair_to_tree(my_test);
  // cout << "parcours en profondeur  : " << endl;
  // parcours_profondeur(my_tree);

  // on transforme notre arbre d'occurence en une liste de codage pour chaque
  // lettre
  map<deque<bool>, char> my_coding_list;
  deque<bool> my_coding;
  tree_to_binary(my_coding_list, my_coding, my_tree);

  // on libère la mémoire de notre arbre, il nous sert plus
  free_tree(my_tree);

  // on affiche les caractères et leur codage
  // for (auto x : my_coding_list) {
  //   cout << "my char : " << x.second << endl;
  //   cout << "my list of bool : " << endl;
  //   for (auto y : x.first) {
  //     cout << y << ", ";
  //   }
  //   cout << endl;
  // }

  int8_t array[4];
  write_compressed_file("my_compressed_text.bz2", the_text_file,
                        my_coding_list, array);
  read_file("my_compressed_text.bz2", my_coding_list, array);
  // transcrire le message dans un fichier binaire, en f
  // parcours en profondeur fonctionne, l'arbre est bien construit
  // maintenant, je dois transformer mon arbre en liste de codage, réussi !
  // passons à la transcription dans un fichier ? C'est plutôt dur ça
  // En vrai, go nettoyer le code
  return 0;
}
