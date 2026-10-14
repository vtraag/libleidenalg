#ifndef GRAPHHELPER_INCLUDED
#define GRAPHHELPER_INCLUDED

#include <igraph/igraph.h>

#include "libleidenalg_export.h"

#include <vector>
#include <set>
#include <exception>
#include <deque>

#include <cstdbool>

//#ifdef DEBUG
#include <iostream>
  using std::cerr;
  using std::endl;
//#endif

class MutableVertexPartition;

using std::vector;
using std::pair;
using std::set;
using std::deque;
using std::make_pair;

vector<size_t> range(size_t n);

bool orderCSize(const size_t* A, const size_t* B);

double KL(double q, double p);
double KLL(double q, double p);

template <class T> T sum(vector<T> vec)
{
  T sum_of_elems = T();
  for (T x : vec)
      sum_of_elems += x;
  return sum_of_elems;
};

// Lightweight read-only view on a contiguous range of indices (neighbours or
// incident edges of a node), so that no copy is needed to iterate over them.
class IndexRange
{
  public:
    IndexRange() : _begin(NULL), _end(NULL) { }
    IndexRange(const size_t* b, const size_t* e) : _begin(b), _end(e) { }
    inline const size_t* begin() const { return _begin; }
    inline const size_t* end() const { return _end; }
    inline size_t size() const { return _end - _begin; }
    inline size_t operator[](size_t i) const { return _begin[i]; }
  private:
    const size_t* _begin;
    const size_t* _end;
};

class Exception : public std::exception
{
  public:
    Exception(const char* str)
    {
      this->str = str;
    }

    virtual const char* what() const throw()
    {
      return this->str;
    }

  private:
    const char* str;

};

inline size_t get_random_int(size_t from, size_t to, igraph_rng_t* rng)
{
  return igraph_rng_get_integer(rng, from, to);
};

void shuffle(vector<size_t>& v, igraph_rng_t* rng);

class LIBLEIDENALG_EXPORT Graph
{
  public:
    Graph(igraph_t* graph,
      vector<double> const& edge_weights,
      vector<double> const& node_sizes,
      vector<double> const& node_self_weights, int correct_self_loops);
    Graph(igraph_t* graph,
      vector<double> const& edge_weights,
      vector<double> const& node_sizes,
      vector<double> const& node_self_weights);
    Graph(igraph_t* graph,
      vector<double> const& edge_weights,
      vector<double> const& node_sizes, int correct_self_loops);
    Graph(igraph_t* graph,
      vector<double> const& edge_weights,
      vector<double> const& node_sizes);
    Graph(igraph_t* graph, int correct_self_loops);
    Graph(igraph_t* graph);
    Graph();
    ~Graph();

    static Graph* GraphFromEdgeWeights(igraph_t* graph, vector<double> const& edge_weights, int correct_self_loops);
    static Graph* GraphFromEdgeWeights(igraph_t* graph, vector<double> const& edge_weights);
    static Graph* GraphFromNodeSizes(igraph_t* graph, vector<double> const& node_sizes, int correct_self_loops);
    static Graph* GraphFromNodeSizes(igraph_t* graph, vector<double> const& node_sizes);

    int has_self_loops();
    double possible_edges();
    double possible_edges(double n);

    Graph* collapse_graph(MutableVertexPartition* partition);

    // Neighbours and incident edges of v, in the same order as returned by
    // igraph_incident(..., IGRAPH_LOOPS_TWICE). Stored in a compressed (CSR)
    // structure that is built once per mode, on first use.
    inline IndexRange get_neighbour_edges(size_t v, igraph_neimode_t mode)
    {
      size_t m = this->mode_index(mode);
      if (!this->_csr_built[m])
        this->build_csr(m);
      const size_t* base = this->_csr_edges[m].data();
      return IndexRange(base + this->_csr_offsets[m][v], base + this->_csr_offsets[m][v + 1]);
    }
    inline IndexRange get_neighbours(size_t v, igraph_neimode_t mode)
    {
      size_t m = this->mode_index(mode);
      if (!this->_csr_built[m])
        this->build_csr(m);
      const size_t* base = this->_csr_neighs[m].data();
      return IndexRange(base + this->_csr_offsets[m][v], base + this->_csr_offsets[m][v + 1]);
    }
    size_t get_random_neighbour(size_t v, igraph_neimode_t mode, igraph_rng_t* rng);

    inline size_t get_random_node(igraph_rng_t* rng)
    {
      return get_random_int(0, this->vcount() - 1, rng);
    };

    inline const igraph_t* get_igraph() { return this->_graph; };

    inline size_t vcount() { return igraph_vcount(this->_graph); };
    inline size_t ecount() { return igraph_ecount(this->_graph); };
    inline double total_weight() { return this->_total_weight; };
    inline double total_size() { return this->_total_size; };
    inline int is_directed() { return this->_is_directed; };
    inline double density() { return this->_density; };
    inline int correct_self_loops() { return this->_correct_self_loops; };
    inline int is_weighted() { return this->_is_weighted; };
    // False if every edge has weight exactly zero (e.g. a layer that only
    // carries node sizes in a multiplex optimisation). Edge loops can then be
    // skipped, since they cannot change any weight-based administration.
    inline bool has_nonzero_weights() { return this->_has_nonzero_weights; };

    inline double edge_weight(size_t e)
    {
      #ifdef DEBUG
      if (e > this->_edge_weights.size())
        throw Exception("Edges outside of range of edge weights.");
      #endif
      return this->_edge_weights[e];
    };

    inline void edge(size_t eid, size_t &from, size_t &to) {
      from = IGRAPH_FROM(this->get_igraph(), eid);
      to = IGRAPH_TO(this->get_igraph(), eid);
    }

    inline vector<size_t> edge(size_t e)
    {
      vector<size_t> edge(2);
      this->edge(e, edge[0], edge[1]);
      return edge;
    }

    inline double node_size(size_t v)
    { return this->_node_sizes[v]; };

    inline double node_self_weight(size_t v)
    { return this->_node_self_weights[v]; };

    inline size_t degree(size_t v, igraph_neimode_t mode)
    {
      if (mode == IGRAPH_IN || !this->is_directed())
        return this->_degree_in[v];
      else if (mode == IGRAPH_OUT)
        return this->_degree_out[v];
      else if (mode == IGRAPH_ALL)
        return this->_degree_all[v];
      else
        throw Exception("Incorrect mode specified.");
    };

    inline double strength(size_t v, igraph_neimode_t mode)
    {
      if (mode == IGRAPH_IN || !this->is_directed())
        return this->_strength_in[v];
      else if (mode == IGRAPH_OUT)
        return this->_strength_out[v];
      else
        throw Exception("Incorrect mode specified.");
    };

  protected:

    int _remove_graph;

    const igraph_t* _graph;

    void init_inclist_adjlist();
    /* Compressed adjacency (CSR) per mode: 0 = OUT, 1 = IN, 2 = ALL. For
     * undirected graphs only ALL is used. Built lazily per mode, but for all
     * nodes at once, so that iterating over neighbours never copies.
     */
    inline size_t mode_index(igraph_neimode_t mode)
    {
      if (!this->_is_directed)
        return 2; // igraph ignores mode for undirected graphs
      switch (mode)
      {
        case IGRAPH_OUT: return 0;
        case IGRAPH_IN:  return 1;
        case IGRAPH_ALL: return 2;
        default: throw Exception("Invalid mode for getting neighbours.");
      }
    }
    void build_csr(size_t m);
    bool _csr_built[3];
    vector<size_t> _csr_offsets[3];
    vector<size_t> _csr_neighs[3];
    vector<size_t> _csr_edges[3];

    // Utility variables to easily access the strength of each node
    vector<double> _strength_in;
    vector<double> _strength_out;

    vector<size_t> _degree_in;
    vector<size_t> _degree_out;
    vector<size_t> _degree_all;

    vector<double> _edge_weights; // Used for the weight of the edges.
    vector<double> _node_sizes; // Used for the size of the nodes.
    vector<double> _node_self_weights; // Used for the self weight of the nodes.


    double _total_weight;
    double _total_size;
    int _is_weighted;
    bool _has_nonzero_weights;
    bool _is_directed;

    int _correct_self_loops;
    double _density;

    void init_admin();
    void set_defaults();
    void set_default_edge_weight();
    void set_default_node_size();
    void set_self_weights();

};

// We need this ugly way to include the MutableVertexPartition
// to overcome a circular linkage problem.
#include "MutableVertexPartition.h"

#endif // GRAPHHELPER_INCLUDED

