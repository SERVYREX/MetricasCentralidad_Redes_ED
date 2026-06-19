#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
//Usamos templates para generalizar nuestra implementacion

//Estructura arista
template <typename TipoVertice, typename TipoPeso>
struct Arista{
  TipoVertice origen;
  TipoVertice destino;
  TipoPeso peso;
};


  
template <typename TipoVertice, typename TipoPeso = int>
class Grafo{
public:
  virtual ~Grafo() = default;

  // Comprobacion booleana para saber si el grafo es dirigido o no
  virtual bool esDirigido() const = 0;

  // Metodos de actualizacion, agregar y eliminar vertices
  virtual void addVertice(const TipoVertice& v) = 0;
  virtual void removeVertice(const TipoVertice& v) = 0;

  // Metodos de actualizacion, agregar y eliminar aristas
  virtual void addArista(const TipoVertice& v, const TipoVertice& u, TipoPeso peso) = 0;
  virtual void removeArista(const Arista<TipoVertice,TipoPeso>& e) = 0;
  virtual void removeAristaWV(const TipoVertice& v, const TipoVertice& u) = 0;

  // Metodos de reemplazo
  virtual void replaceVertice(const TipoVertice& v, const TipoVertice& w) = 0;
  virtual void replaceArista(const Arista<TipoVertice, TipoPeso>& e, const TipoPeso& x) = 0;

  // Observadores y getters basicos
  virtual bool isEmpty() const = 0;
  virtual int getNumVertices() const = 0;
  virtual int getNumAristas() const = 0;
  virtual bool sonAdyacentes(const TipoVertice& v, const TipoVertice& u) const = 0;
  virtual std::vector<TipoVertice> getVecinos(const TipoVertice& v) const = 0;

  //Iteradores
  virtual std::vector<Arista<TipoVertice, TipoPeso>> aristaIncidentes(const TipoVertice& v) const = 0;
  virtual std::vector<TipoVertice> getVertices() const = 0;
  virtual std::vector<Arista<TipoVertice, TipoPeso>> getAristas() const = 0;
  //Para grafos no dirigidos
  virtual int getGrado(const TipoVertice& v) const = 0;

  //Para grafos dirigidos
  virtual int getInGrado(const TipoVertice& v) const = 0;
  virtual int getOutGrado(const TipoVertice& v) const = 0;
};

// Implementacion basada en Listas de adyacencia

template<typename TipoVertice, typename TipoPeso = int>

class GrafoAdList : public Grafo<TipoVertice , TipoPeso>{
  //Variable para controlar el tipo de grafo y creacion de lista de adyacencia mediante un mapa
private:
  bool dirigido;
  int numAristas = 0;
  std::map<TipoVertice, std::vector<Arista<TipoVertice,TipoPeso>>> listaAdyacencia;

public:
  GrafoAdList(bool esDirigido = false){
    this->dirigido = esDirigido;
  }
  bool esDirigido() const override {
    return dirigido;
  }

  void addVertice(const TipoVertice& v) override {
    //Comprobamos que el vertice a insertar no este en el grafo, en caso de no estar, lo insertamos
    if(listaAdyacencia.find(v) == listaAdyacencia.end()) {
      listaAdyacencia.insert({v , std::vector<Arista<TipoVertice, TipoPeso>>()
	});
    }
  }

  void removeVertice(const TipoVertice& v) override {
    //Si el vertice no se encuentra en el grafo, no hacemos nada
    auto it = listaAdyacencia.find(v);
    
    if(it == listaAdyacencia.end()) return;

    //Elimina las aristas que apunten a v desde otros vertices
    for (auto& par: listaAdyacencia){
      if (par.first == v) continue;
      auto& aristas = par.second;

      //Recorremos la lista de aristas del vertice actual hasta encontrar las que tengan como destino v
      for(auto itr = aristas.begin(); itr != aristas.end();){
	if(itr-> destino == v){
	  itr = aristas.erase(itr);
	  numAristas--;
	}
	else {
	  ++itr;
	}
      }
    }
    //Si el grafo es dirigido tambien hay que desconectar las aristas salientes
    if(dirigido){
      numAristas -= it->second.size();
    }
    listaAdyacencia.erase(it);
  }

  //Dos formas de eliminar aristas segun el contexto
  void removeArista(const Arista<TipoVertice, TipoPeso>& e) override {
    removeAristaWV(e.origen, e.destino);
  }

  void removeAristaWV(const TipoVertice& v, const TipoVertice& u) override {
    //Comprobamos que el vertice origen exista
    auto it = listaAdyacencia.find(v);
    
    if(it == listaAdyacencia.end()) return;
    
    auto& vec = it->second;
    // Buscamos y eliminamos la arista (v,u)
    for(auto itr = vec.begin(); itr != vec.end(); ++itr){
      if(itr-> destino == u) {
	vec.erase(itr);

	//En grafos no dirigidos debemos eliminar la representacion inversa

	if(!dirigido) {
	  auto& vec2 = listaAdyacencia[u];

	  for(auto itr2 = vec2.begin(); itr2!= vec2.end(); ++itr2){
	    if(itr2->destino == v) {
	      vec2.erase(itr2);
	      break;
	    }
	  }
	}
	numAristas--;
	return;
      }
    }
  }
  
  void addArista(const TipoVertice& v, const TipoVertice& u, TipoPeso peso) override {
    //Comprobamos que los vertices esten en el grafo, en caso contrario los agregamos
    addVertice(v);
    addVertice(u);
    //Agregamos la representacion de arista a la lista de adyacencia
    listaAdyacencia[v].push_back({v,u,peso});
    //En caso de no ser dirigido, creamos la arista pero de forma contraria
    if(!dirigido){
      listaAdyacencia[u].push_back({u,v,peso});
    }
    numAristas++;
  }

  //Considerar que no es posible reemplazar un vertice por otro ya existente
  void replaceVertice(const TipoVertice& v, const TipoVertice& w) override {
    //Comprobamos que el vertice v exista y que w no este en el grafo
    if(listaAdyacencia.find(v) == listaAdyacencia.end()) return;
    if(listaAdyacencia.find(w) != listaAdyacencia.end()) return;
    
    //Copiamos la lista de adyacencia de v hacia w
    listaAdyacencia[w] = listaAdyacencia[v];
    for(auto& arista : listaAdyacencia[w]){
      arista.origen = w;
    }

    //Actualizamos todas las aristas que tenian a v como detino para que ahora apunten a w
    for(auto& par : listaAdyacencia){
      for(auto& arista : par.second){
	if(arista.destino == v)
	  arista.destino = w;
      }
    }
    //Eliminamos el vertice antiguo
    listaAdyacencia.erase(v);
  }

  void replaceArista(const Arista<TipoVertice, TipoPeso>& e, const TipoPeso& x) override {

    //Comprobamos que el vertice de la arista a reemplazar exista
    auto it = listaAdyacencia.find(e.origen);
    if(it == listaAdyacencia.end()) return;


    //Encontramos el vertice de destino para cambiar el peso de la arista
    for(auto& arista : it->second) {
      if(arista.destino == e.destino) {
	arista.peso = x;
	
	//En caso de ser no dirigido hacemos lo mismo en direccion contraria
	if(!dirigido) {
	  for(auto& a : listaAdyacencia[e.destino]) {
	    if(a.destino == e.origen) {
	      a.peso = x;
	      break;
	    }
	  }
	}
	return;
      }
    }
  }
  
  bool isEmpty() const override {
    return listaAdyacencia.empty();
  }

  bool sonAdyacentes(const TipoVertice& v, const TipoVertice& u) const override {
    //En caso de no existir algun vertice, asumimos que no son adyacentes
    auto it = listaAdyacencia.find(v);
    auto it2 = listaAdyacencia.find(u);
    if(it == listaAdyacencia.end() || it2 == listaAdyacencia.end()) return false;

    //Si hay una arista que termina en u devolvemos true
    for(const auto& arista : it->second) {
      if(arista.destino == u) return true;
    }
    return false;
  }

  std::vector<TipoVertice> getVecinos(const TipoVertice& v) const override {
    std::vector<TipoVertice> vecinos;
    //Comprobamos que el vertice exista
    auto it = listaAdyacencia.find(v);
    if(it == listaAdyacencia.end()) return vecinos;

    //Recorremos sus aristas y lleanmos el vector con todas las aristas que tengan v como destino
    for(const auto& arista : it-> second) {
      vecinos.push_back(arista.destino);
    }
    return vecinos;
  }

  std::vector<Arista<TipoVertice, TipoPeso>> aristaIncidentes(const TipoVertice& v) const override {
    //Encontramos v, si no existe devolvemos una lista vacia, en caso contrario devolvemos su lista de aristas
    auto it = listaAdyacencia.find(v);
    if(it == listaAdyacencia.end()) return {};
    return it->second;
  }

  std::vector<TipoVertice> getVertices() const override {
    std::vector<TipoVertice> vertices;

    //Recorremos la lista y extraemos la clave de cada entrada del mapa, es decir el vertice, luego lo agregamos a vertices
    for(const auto& par : listaAdyacencia) {
      vertices.push_back(par.first);
    }
    return vertices;
  }
  
  std::vector<Arista<TipoVertice, TipoPeso>> getAristas() const override {
    std::vector<Arista<TipoVertice, TipoPeso>> aristas;

    //Recorremos la lista y extraemos las aristas por cada uno de los vertices
    for(const auto& par : listaAdyacencia){
      for(const auto& arista : par.second) {
	aristas.push_back(arista);
      }
    }
    return aristas;
  }

  //Comprobamos que el vertice exista y devolvemos el tamaño de la lista asociada a v
  int getGrado(const TipoVertice& v) const override {
    auto it = listaAdyacencia.find(v);

    if(it == listaAdyacencia.end()) return 0;

    return it->second.size();
  }

  int getOutGrado(const TipoVertice& v) const override {
    return getGrado(v);
  }

  //Comprobamos que la cantidad de aristas que tienen como destino v
  int getInGrado(const TipoVertice& v) const override {
    if(listaAdyacencia.find(v) == listaAdyacencia.end()) return 0;
    
    int grado = 0;

    for(const auto& par : listaAdyacencia){
      for(const auto& arista : par.second) {
	if(arista.destino == v)
	  grado++;
      }
    }
    return grado;
  }
  
  								         
  int getNumVertices() const override {
    return listaAdyacencia.size();
  }
  
  int getNumAristas() const override {
    return numAristas;
  }
};
  

 
 
  
  
