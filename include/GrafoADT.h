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
    if(listaAdyacencia.find(v) == listaAdyacencia.end()) return;

    //Elimina las aristas que apunten a v desde otros vertices

    for (auto& par: listaAdyacencia){
      if (par.first == v) continue;
      auto& aristas = par.second;

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
      numAristas -= listaAdyacencia[v].size();
    }
    listaAdyacencia.erase(v);
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

  int getNumVertices() const override {
    return listaAdyacencia.size();
  }
  
  int getNumAristas() const override {
    return numAristas;
  }
  
 
  

 
 
  
  
