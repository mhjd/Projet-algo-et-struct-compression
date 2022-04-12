#include <iostream>
#include <list>
#include <deque>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <tuple>

using namespace std;

class Node {
public:
  char character;
  int weight;
  Node* node_left;
  Node* node_right;
  Node(char p_character, int p_weight, Node* p_node_left= NULL, Node* p_node_right = NULL){
    character = p_character;
    weight = p_weight;
    node_left = p_node_left;
    node_right = p_node_right;
  }
  void increaseWeight(){
    weight++;
  }
  char getMyChar(){
    return character;
  }

};


// upper_bond : renvoie le premier élément qui va être supérieur à une valeur

// emplace back, node, Bon flemme de faire des pointeurs, je compte m'occuper des opti plus tard
Node* returnNode(char my_char){
  Node* to_return = new Node{my_char, 0};
  return to_return;
}

// permet de voir si un char est présent dans la liste d'occurence
bool isIn(char char_to_search, map<char, int>& my_list){
  auto search = my_list.find(char_to_search);
  if (search == my_list.end()){
    return false;
  }
  return true;
}

// prend un file en paramètre, et renvoie une liste de tuples avec les occurences
// Fonctionne !
vector<pair<char, int>> file_to_vector(string fileName){
  ifstream readFile(fileName);

  map<char, int> my_chars_occurences;
  cout << endl;
  char c;

  //tuple<char> search;
  while (readFile.get(c)){
    auto search = my_chars_occurences.find(c);
    if (search == my_chars_occurences.end()){
      my_chars_occurences.emplace(std::make_pair(c, 1));
    } else {
      my_chars_occurences[c]++;
    }
  }

    vector<pair<char, int>> vec;
    copy(my_chars_occurences.begin(),
            my_chars_occurences.end(),
          back_inserter<std::vector<pair<char, int>>>(vec));

    sort(vec.begin(), vec.end(),
            [](const pair<char, int> &l, const pair<char, int> &r)
            {
                    return l.second < r.second;
            });

  return vec;
}


Node* node_from_pair(pair<char, int> my_pair){
  return new Node{std::get<0>(my_pair), std::get<1>(my_pair)};
}
Node* node_from_nodes(Node* my_first_node, Node* my_second_node){
  return new Node{' ', my_first_node -> weight + my_second_node -> weight, my_first_node, my_second_node};
}


void print_list(list<Node*>& my_list){
    //for(auto it = my_list.begin(); it != my_list.end(); ++it){
    //  cout << (*it) -> weight << ", ";
    //}
    //cout << endl;
}

// vérifier s'il y a plus de deux éléments avant d'appeler
void insert_with_sorting(list<Node*>& my_nodes, Node*& my_new_node){

  //cout << "valeur du poids de my_new_node : " << my_new_node -> weight << endl << "---->>>";
  bool insert = false;
  for(auto it = my_nodes.begin(); it != my_nodes.end(); ++it){
    //cout << "la valeur du poids de l'itérateur: " << ((*it) -> weight) << endl;
    if(my_new_node ->weight < ((*it) -> weight) && !(insert)){
      //cout << "insertion" << endl;
      my_nodes.insert(it, my_new_node);
      insert = true;
      //it = my_nodes.end();
    }
  }
cout << "liste après insertion : " << endl;

  for(auto it = my_nodes.begin(); it != my_nodes.end(); ++it){
    cout << (*it) -> weight << ", ";
  }
  cout << endl;
}


void parcours_profondeur(Node* my_node){
  deque<Node*> ma_file;
  ma_file.push_back(my_node);
  Node* head;
  while(!(ma_file.empty())){
    head = ma_file.front();
    ma_file.pop_front();
    if (head != NULL){
    cout << head -> weight << endl;
    ma_file.push_back(head -> node_right);// right= droite, on descend, donc 1
    ma_file.push_back(head -> node_left); // left = gauche, on descend, donc 1
    }
  }
  cout << endl;
  cout << "fin du parcours en profondeur" << endl;
}


void free_tree(Node* my_node){
  if (my_node == NULL){
    return;
  }
  if (my_node -> node_left != NULL){
    free_tree(my_node -> node_left);
  }

  if (my_node -> node_right != NULL){
    free_tree(my_node -> node_right);
  }
  delete[] my_node;
}

Node* vector_pair_to_tree(vector<pair<char, int>> my_occcurences){
  // méthode :
  // On prend une liste de noeuds, on lui ajoute tout nos noeud dans l'ordre croissant d'occurence, on prend les deux plus petits éléments (s'il y en a au moins deux) qui sont au début, on en fait un nouveau noeud, on enlève ces deux plus petits noeuds, et on ajoute le nouveau noeud au bon emplacement de sorte à ce que ça reste dans l'ordre croissant
  // on fait ça jusqu'à qu'il nous reste qu'un seul noeud, que l'on renvoie

  list<Node*> my_nodes;
  for (auto my_occurence : my_occcurences){
    my_nodes.push_back(node_from_pair(my_occurence));
  }

/* tout fonctionne bien dans le test :
  cout << "test " << endl;
  for (auto i : my_nodes){
    cout << "letter : " << i -> character << ", occurences : " << i -> weight << endl;
  }
  cout << "fin du test " << endl;
  */

    Node* my_smallest_element;
    Node* my_second_smallest_element;
    Node* my_new_node;
  while(my_nodes.size() > 1 ){
  cout <<"liste avant insertion : " << endl;
  for(auto it = my_nodes.begin(); it != my_nodes.end(); ++it){
    cout << (*it) -> weight << ", ";
  }
  cout << endl;

    my_smallest_element = my_nodes.front();
    my_nodes.pop_front();
    my_second_smallest_element = my_nodes.front();
    my_nodes.pop_front();



    //cout << "my nodes before size " << my_nodes.size() << endl;
    //print_list(my_nodes);
    my_new_node = node_from_nodes(my_smallest_element, my_second_smallest_element);
    insert_with_sorting(my_nodes, my_new_node);
    //cout << "my nodes after size " << my_nodes.size() << endl;
    //print_list(my_nodes);
    // faudrait afficher la liste de wight à chaque tour, et voir
  }

  cout <<"my new node " << my_new_node -> weight << endl;
  for(auto it = my_nodes.begin(); it != my_nodes.end(); ++it){
    cout << (*it) -> weight << ", ";
  }
  cout << endl;


  return my_new_node;
}

void free_tree(); // libère un arbre
//Node* tuples_to_tree(map<char, int> my_tuples){
//
//}



// première étape :
// lire l
// On créer une liste de noeud pour chaque caractère, qu'on utile lors de la lecture du fichier
// Ensuite on fait la boucle qui transforme la file de noeud en arbre
// Ensuite, on verra

int main(void){
  /*string ma_chaine{"la chaine"};
  char mon_char  = ma_chaine.front();
  cout << "mon char est : " << mon_char << endl;*/
  //string my_string{"My string"};

  string my_text{"my_text.txt"};
  auto my_test = file_to_vector(my_text);

  for (const auto &p : my_test) {
    std::cout << p.first << " => " << p.second << '\n';
  }

  // ma liste d'occurence fonctionne, il me faut maitenant transformer ça en arbre
  Node* chacal = node_from_pair(std::make_pair('c', 1));

  cout << "mon caractère : " << chacal -> character << endl;
  delete chacal;
  cout << "mon caractère : " << chacal << endl;
auto result = vector_pair_to_tree(my_test);
parcours_profondeur(result);
 free_tree(result);
// parcours en profondeur fonctionne, l'arbre est bien construit
// maintenant, je dois transformer mon arbre en


  /*
  std::map<int,char> example = {{1,'a'},{2,'b'}};
  auto search = example.find(8);
  cout << endl;
  if (search == example.end()){
    cout << "noice" << endl;
  }
*/
   // std::list<std::string>  listOfStrs = { "First", "Sec", "Third", "Fourth", "Fifth", "Sixth" };
   // auto it = listOfStrs.begin();
//advance(it, 1);
   // cout << "second element : " << *it << endl;
  return 0;
}
