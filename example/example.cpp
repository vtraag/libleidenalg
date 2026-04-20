#include <igraph.h>
#include <GraphHelper.h>
#include <Optimiser.h>
#include <CPMVertexPartition.h>
#include <stdio.h>

using std::cout;
using std::endl;

int main()
{
    igraph_t g;
    igraph_famous(&g, "Zachary");

    Graph graph(&g);

    CPMVertexPartition part(&graph,
                            0.05 /* resolution */ );

    Optimiser o;

    o.optimise_partition(&part);

    cout << "Node\tCommunity" << endl;
    for (int i = 0; i < graph.vcount(); i++)
        cout << i << "\t" << part.membership(i) << endl;

    igraph_destroy(&g);
}