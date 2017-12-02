#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {
	float distancia;
	grafo g = le_grafo(stdin);
	if ( !g )
		return 1;

	int d = diametro(g);
	if (d == infinito)
		printf("diâmetro = oo\n");
	else
		printf("diâmetro = %d\n", d);

	char *s = "a";
	vertice v = vertice_nome(s, g); 
	char *b = "d";
	vertice v2 = vertice_nome(b, g);

	vertice *c = malloc(100 * sizeof(vertice));

	int dist = caminho_minimo(c, &distancia, v, v2, g);
	printf("DISTANCIA DE RUN PARA RUNSWAP: %d.\n", dist);
	
	if (v != NULL) {
		//printf("%ssimplicial\n", simplicial(v, g) ? "" : "não ");
	}
	printf("nome: %s\n", nome_grafo(g));
	printf("%d vértices\n", numero_vertices(g));
	printf("%d arestas\n", numero_arestas(g));
	printf("%sdirecionado\n", direcionado(g) ? "" : "não ");
	printf("%sbipartido\n", bipartido(g) ? "" : "não ");
	printf("%sponderado\n", ponderado(g) ? "" : "não ");


	escreve_grafo(stdout, g);

	return ! destroi_grafo(g);
}
