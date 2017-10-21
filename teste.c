#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {

	grafo g = le_grafo(stdin);
	if ( !g )
		return 1;

	int d = diametro(g);
	if (d == infinito)
		printf("diâmetro = oo\n");
	else
		printf("diâmetro = %d\n", d);

	char *s = "A";
	vertice v = vertice_nome(s, g); 
	if (v != NULL) {
		printf("%ssimplicial\n", simplicial(v, g) ? "" : "não ");
	}
	printf("nome: %s\n", nome_grafo(g));
	printf("%d vértices\n", numero_vertices(g));
	printf("%d arestas\n", numero_arestas(g));
	printf("%sdirecionado\n", direcionado(g) ? "" : "não \n");
	printf("%sbipartido\n", bipartido(g) ? "" : "não \n");


	escreve_grafo(stdout, g);

	return ! destroi_grafo(g);
}
