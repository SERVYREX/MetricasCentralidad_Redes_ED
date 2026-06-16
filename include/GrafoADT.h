#include <vector>

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

 

 
  

 
 
  
  
