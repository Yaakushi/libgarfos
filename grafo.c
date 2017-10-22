#include <stdio.h>
#include <graphviz/cgraph.h>
#include "grafo.h"

//------------------------------------------------------------------------------
// o valor que representa "infinito"

const int infinito = 0x3f3f3f3f;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo pode ser direcionado ou não
// 
// o grafo tem um nome, que é uma "string"

struct grafo
{
	char *nome;
	int numvert; // Numero de vertices
	vertice *vertices;
	int numarestas; // Numero de arestas (detalhar motivo da escolha no readme)
	int *matadj; // Matriz de adjacencia
	int isdirec; // É direcionado?
};

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados que representa um vértice do grafo
// 
// cada vértice tem um nome que é uma "string"

struct vertice 
{
	int id;
	char *nome;
	int cor;
	int vecid;
};

static int numgraphs = 0; // Numero de grafos carregados
static grafo *graphs = NULL; // Array de grafos carregados

static int get_vertice_index(grafo g, Agnode_t *other_node)
{
	for(int i = 0; i < g->numvert; i++)
		if(g->vertices[i]->id == (int) AGID(other_node)) 
			return i;

	return -1;
}

//------------------------------------------------------------------------------
// devolve o nome do grafo g

char *nome_grafo(grafo g)
{
	if(!g) return NULL;
	return (g->nome != NULL) ? g->nome : "";
}
//------------------------------------------------------------------------------
// devolve um grafo de nome s,
//         ou 
//         NULL, caso não exista tal grafo

grafo grafo_nome(char *s)
{
	if(!s) return NULL; // s == NULL?

	// Varre o vetor de grafos buscando alguem com o mesmo nome.
	for(int i = 0; i < numgraphs; i++)
		if(strcmp(graphs[i]->nome, s) == 0)
			return graphs[i];

	return NULL;
}
//------------------------------------------------------------------------------
// devolve 1, se g é direcionado,
//         ou 
//         0, caso contrário

int direcionado(grafo g)
{
	if(!g) return -1;
	return g->isdirec;
}
//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g

unsigned int numero_vertices(grafo g)
{
	if(!g) return -1;
	return g->numvert;
}
//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g

unsigned int numero_arestas(grafo g)
{
	if(!g) return -1;
	return g->numarestas;
}
//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g)
{
	// TODO
	return 0;
}
//------------------------------------------------------------------------------
// lê um grafo no formato dot de input
// 
// devolve o grafo lido,
//         ou 
//         NULL, em caso de erro 

grafo le_grafo(FILE *input)
{
	grafo newGrafo = malloc(sizeof(struct grafo));
	if(!newGrafo) return NULL;

	// Abre o novo grafo.
	Agraph_t *g = agread(input, 0);

	int numnodes = agnnodes(g);
	// Preenche os campos que a lib "dá de graça".
	newGrafo->nome = agnameof(g);
	newGrafo->numvert = numnodes;
	newGrafo->numarestas = agnedges(g);
	newGrafo->isdirec = agisdirected(g);

	// Aloca a matriz de adjacência e a array de vértices
	newGrafo->matadj = calloc(numnodes * numnodes, sizeof(int));
	if(!newGrafo->matadj)
		return NULL;

	// Idem para a matriz de vértices
	newGrafo->vertices = malloc(sizeof(vertice) * numnodes);
	
	int nid = 0;
	for(Agnode_t *n = agfstnode(g); n; n = agnxtnode(g, n))
	{
		vertice v = malloc(sizeof(struct vertice));
		if(!v) return NULL;

		v->id = AGID(n);
		v->nome = agnameof(n);
		newGrafo->vertices[nid++] = v;
	}

	nid = 0;
	for(Agnode_t *n = agfstnode(g); n; n = agnxtnode(g, n))
	{
		for(Agedge_t *e = agfstout(g, n); e; e = agnxtout(g, e))
		{
			Agnode_t *head = aghead(e);
			int hid = get_vertice_index(newGrafo, head);
			newGrafo->matadj[numnodes * nid + hid] = 1;
			if(!newGrafo->isdirec)
				newGrafo->matadj[numnodes * hid  + nid] = 1;
		}
		nid++;
	}

	for(int i = 0; i < newGrafo->numvert; i++)
	{
		newGrafo->vertices[i]->vecid = i;
	}

	// TODO: Remover.
	//for(int i = 0; i < numnodes; i++) {
	//	for(int j = 0; j < numnodes; j++) {
	//		printf("%d ", newGrafo->matadj[numnodes*i+j]);
	//	}
	//	printf("\n");
	//}

	return newGrafo;
}

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g)
{
	int *indices = malloc(g->numvert * sizeof(int));
	int i, j;
	int k = 0;
	if (output == NULL) {
		return NULL;
	}
	if (g->isdirec) {
		fprintf(output, "digraph %s {\n", g->nome);
		for (i = 0; i < g->numvert; i++) {
			for (j = 0; j < g->numvert; j++) {
				if (g->matadj[g->numvert * i + j] > 0) {
					indices[k++] = j;
				}
			}
			if (k > 1) {
				fprintf(output, "\t%s -> {", g->vertices[i]->nome);
				for (j = 0; j < k - 1; j++) {
					fprintf(output, "%s ", g->vertices[indices[j]]->nome);
				}
				fprintf(output, "%s};\n", g->vertices[indices[j]]->nome);
			} else if (k == 1){
				fprintf(output, "\t%s -> %s\n;", g->vertices[i]->nome, g->vertices[indices[0]]->nome);
			}
			k = 0;
		}
		printf("}\n");
	} else {
		fprintf(output, "graph %s {\n", g->nome);
		for (i = 0; i < g->numvert - 1; i++) {
			for (j = i + 1; j < g->numvert; j++) {
				if (g->matadj[g->numvert * i + j] > 0) {
					indices[k++] = j;
				}
			}
			if (k > 1) {
				fprintf(output, "\t%s -- {", g->vertices[i]->nome);
				for (j = 0; j < k - 1; j++) {
					fprintf(output, "%s ", g->vertices[indices[j]]->nome);
				}
				fprintf(output, "%s};\n", g->vertices[indices[j]]->nome);
			} else if (k == 1){
				fprintf(output, "\t%s -- %s;\n", g->vertices[i]->nome, g->vertices[indices[0]]->nome);
			}
			k = 0;
		}
		printf("}\n");
	}
	free(indices);
	return g;
}
//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v)
{
	if(!v) return NULL;
	return (v->nome != NULL) ? v->nome : "";
}
//------------------------------------------------------------------------------
// devolve um vertice de nome s no grafo g,
//         ou 
//         NULL caso não exista em g um vertice de nome s

vertice vertice_nome(char *s, grafo g)
{
	for (int i = 0; i < g->numvert; i++)
	{
		if (strcmp(g->vertices[i]->nome, s) == 0)
		{
			return g->vertices[i];
		}
	}
	return NULL;
}
//------------------------------------------------------------------------------
// devolve o grau do vértice v no grafo g
// 
// se g é direcionado, e 
//                       direcao =  1, o grau devolvido é o de saída
//                       direcao = -1, o grai devolvido é o de entrada
//                
// caso contrário o valor de direcao é ignorado.                  

unsigned int grau(vertice v, int direcao, grafo g)
{

  return 0;
}
//------------------------------------------------------------------------------
// devolve o "primeiro" vizinho de v em g,
//         ou
//         NULL se v é vértice isolado em g
//
// se g é direcionado, e 
//                       direcao =  1, o vizinho devolvido é de saída
//                       direcao = -1, o vizinho devolvido é de entrada
//                
// caso contrário o valor de direcao é ignorado.                  

vertice primeiro_vizinho(vertice v, int direcao, grafo g)
{
	// Procura id vertice no grafo.
	int vertid;
	for(vertid = 0; vertid < g->numvert; vertid++)
	{
		if(v == g->vertices[vertid]) break;
	}
	if(vertid == g->numvert)
	{
		return NULL; // vertice nao encontrado no grafo, nao deveria acontecer!
	}

	int incx = 0, incy = 0, x = 0, y = 0;
	if(direcao == 1) 
	{
		// Vizinho de saida (percorre linha)
		incx = 1;
		y = vertid;
	}
	else
	{
		// Vizinho de entrada (percorre coluna)
		incy = 1;
		x = vertid;
	}

	while(x < g->numvert && y < g->numvert)
	{
		// Varre a matriz de adjacencia em busca do primeiro vizinho.
		if(g->matadj[g->numvert * y + x] != 0) 
			return g->vertices[(direcao == 1 ? x : y)];

		// Apenas um desses valores será != 0.
		x += incx;
		y += incy;
	}
	return NULL;
}
//------------------------------------------------------------------------------
// devolve o "próximo" vizinho de v em g após u,
//         ou
//         NULL se u é o "último" vizinho de v em g
//
// se g é direcionado, e 
//                       direcao =  1, o vizinho devolvido é de saída
//                       direcao = -1, o vizinho devolvido é de entrada
//                
// caso contrário o valor de direcao é ignorado.                  

vertice proximo_vizinho(vertice u, vertice v, int direcao, grafo g)
{
	// Busca o ID dos vertices relevantes no grafo.
	int vid = g->numvert, uid = g->numvert;
	for(int i = 0; i < g->numvert; i++)
	{
		if(u = g->vertices[i]) uid = i;
		if(v = g->vertices[i]) vid = i;
	}
	// Algum dos vertices nao foi encontrado no grafo? Retorna.
	if(vid == g->numvert || uid == g->numvert) return NULL; 

	int incx = 0, incy = 0, x, y;
	if(direcao == 1)
	{
		// Vizinho de saida (percorre linha)
		incx = 1;
		x = uid + 1;
		y = vid;
	}
	else
	{
		incy = 1;
		x = vid;
		y = uid + 1;
	}

	while(x < g->numvert && y < g->numvert)
	{
		if(g->matadj[g->numvert * y + x] != 0)
			return g->vertices[(direcao == 1 ? x : y)];

		x += incx;
		y += incy;
	}
	return NULL;
}
//------------------------------------------------------------------------------
// devolve 1, se v é um vértice simplicial em g, 
//         ou
//         0, caso contrário
//
// um vértice é simplicial no grafo se sua vizinhança é uma clique

int simplicial(vertice v, grafo g)
{
	// aloca vetor do tam do nº de nodos
	int *vizin = malloc(sizeof(int) * g->numvert);
	int clique = 1;
	int numvi = 0;
	int i, j;
	for (i = 0; i < g->numvert; i++) { // busca vizinhos de g
		if (g->matadj[g->numvert * v->vecid + i] != 0) {
			vizin[numvi++] = i;
		}
	} // encontra numvi vizinhos em vizin[0] a vizin[numvi]
	for (i = 0; i < numvi && clique != 0; i++) { // para cada vizin
		for (j = 0; j != i && j < numvi; j++) { // para todo outro vizin
			clique = clique * g->matadj[g->numvert * vizin[i] + vizin[j]];
		}
	}
	free(vizin);
	if (clique == 0) {
		return 0;
	} else {
		return 1;
	}
}

void descolorir(grafo g)
{
	for(int i = 0; i < g->numvert; i++)
		g->vertices[i]->cor = -1;
}

//------------------------------------------------------------------------------
// devolve 1 se foi possivel pintar a arvore filha de v com cores alternadas de
//            forma que dois vizinhos não tenham a mesma cor
//         ou
//         2 caso contrário.
int pintaGrafoBipartido(grafo g, vertice v, int cor)
{
	v->cor = cor;
	vertice pv = primeiro_vizinho(v, 1, g);
	if(pv)
	{
		do {
			if(pv->cor == -1)
				if(!pintaGrafoBipartido(g, pv, cor^1)) return 0;
			if(pv->cor == cor) return 0;

		} while(pv = proximo_vizinho(pv, v, 1, g));
	}
	return 1;
}

//------------------------------------------------------------------------------
// devolve 1, se g é um grafo bipartido, 
//         ou
//         0, caso contrário

int bipartido( grafo g)
{
	
	return 0;
}


//------------------------------------------------------------------------------
// devolve em c um caminho mínimo de u a v no grafo não direcionado g, 
//              de forma que
//
//                  c[0]=u, ..., c[n]=v, onde n é o tamanho do caminho
//
//              ou
// 
//              NULL se não existe tal caminho, 
//
// em qualquer caso, devolve a distância de u a v em g

int caminho_minimo(vertice *c, vertice u, vertice v, grafo g)
{

  return 0;
}
//------------------------------------------------------------------------------
// devolve o diâmetro do grafo g

int diametro(grafo g)
{
    int max = 0;
    int ijk;
	int tam = g->numvert * g->numvert;
    int *matFloyd = malloc(tam * sizeof(int));
    int linha = 0;
    for (int k = 0; k < tam; k++) {
        // Quando o valor na matriz original é 0 && não está na diag principal
        matFloyd[k] = ((g->matadj[k] == 0) && (k % g->numvert != linha) ? infinito : g->matadj[k]); 
        if ((k % g->numvert) == g->numvert - 1) {
            linha++; // Quando k mod g->numvert == linha, g->matadj[k] está na diagonal principal
        }
    }
    for (int k = 0; k < g->numvert; k++) {
        for (int i = 0; i < g->numvert; i++) {
            for (int j = 0; j < g->numvert; j++) {
                ijk = matFloyd[g->numvert * i + k] + matFloyd[g->numvert * k + j];
                if (ijk < matFloyd[g->numvert * i + j]) {
                    matFloyd[g->numvert * i + j] = ijk;
					// imprime_matriz(matFloyd, tam, g->numvert);
                }
				if (k == g->numvert -1) { 
                	max = (matFloyd[g->numvert * i + j] > max ? matFloyd[g->numvert * i + j] : max);
				}
            }
        }
    }
	free(matFloyd);
    return max;
}
//------------------------------------------------------------------------------

void imprime_matriz(int *mat, int tam, int numvert)
{
	int linha = 0;
	for (int i = 0; i < tam; i++) {
		if (mat[i] == infinito) {
			printf("oo ");
		} else {
			printf("%d ", mat[i]);
		}
		if (i % numvert == numvert - 1) {
			printf("\n");
			linha++;
		}
	}
}



