#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// o valor que representa "infinito"

const int infinito;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo pode ser direcionado ou nÃ£o
// 
// o grafo tem um nome, que Ã© uma "string"

typedef struct grafo *grafo;

//------------------------------------------------------------------------------
// devolve o nome do grafo g

char *nome_grafo(grafo g);

//------------------------------------------------------------------------------
// devolve um grafo de nome s,
//         ou 
//         NULL, caso nÃ£o exista tal grafo

grafo grafo_nome(char *s);

//------------------------------------------------------------------------------
// devolve 1, se g Ã© direcionado,
//         ou 
//         0, caso contrÃ¡rio

int direcionado(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g Ã© ponderado,
//         ou 
//         0, caso contrÃ¡rio
//
// o grafo g é ponderado se todas as suas arestas/arcos tem um
// atributo de nome "weight" cujo valor é um número.

int ponderado(grafo g);
//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g

unsigned int numero_vertices(grafo g);

//------------------------------------------------------------------------------
// devolve o nÃºmero de arestas/arcos do grafo g

unsigned int numero_arestas(grafo g);

//------------------------------------------------------------------------------
// desaloca toda a memÃ³ria usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrÃ¡rio

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// lÃª um grafo no formato dot de input
// 
// devolve o grafo lido,
//         ou 
//         NULL, em caso de erro 

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados que representa um vÃ©rtice do grafo
// 
// cada vÃ©rtice tem um nome que Ã© uma "string"

typedef struct vertice *vertice;

//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v);

//------------------------------------------------------------------------------
// devolve um vertice de nome s no grafo g,
//         ou 
//         NULL caso nÃ£o exista em g um vertice de nome s

vertice vertice_nome(char *s, grafo g);

//------------------------------------------------------------------------------
// devolve o grau do vÃ©rtice v no grafo g
// 
// se g Ã© direcionado, e 
//                       direcao =  1, o grau devolvido Ã© o de saÃ­da
//                       direcao = -1, o grai devolvido Ã© o de entrada
//                
// caso contrÃ¡rio o valor de direcao Ã© ignorado.                  

unsigned int grau(vertice v, int direcao, grafo g);

//------------------------------------------------------------------------------
// devolve o "primeiro" vizinho de v em g,
//         ou
//         NULL se v Ã© vÃ©rtice isolado em g
//
// se g Ã© direcionado, e 
//                       direcao =  1, o vizinho devolvido Ã© de saÃ­da
//                       direcao = -1, o vizinho devolvido Ã© de entrada
//                
// caso contrÃ¡rio o valor de direcao Ã© ignorado.                  

vertice primeiro_vizinho(vertice v, int direcao, grafo g);

//------------------------------------------------------------------------------
// devolve o "prÃ³ximo" vizinho de v em g apÃ³s u,
//         ou
//         NULL se u Ã© o "Ãºltimo" vizinho de v em g
//
// se g Ã© direcionado, e 
//                       direcao =  1, o vizinho devolvido Ã© de saÃ­da
//                       direcao = -1, o vizinho devolvido Ã© de entrada
//                
// caso contrÃ¡rio o valor de direcao Ã© ignorado.                  

vertice proximo_vizinho(vertice u, vertice v, int direcao, grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g Ã© um grafo bipartido, 
//         ou
//         0, caso contrÃ¡rio

int bipartido( grafo g);

//------------------------------------------------------------------------------
// devolve em c um caminho (direcionado) mínimo de u a v no grafo
//              (direcionado e/ou ponderado) g, de forma que
//
//
//
//              ou
// 
//              NULL se nãoo existe tal caminho, 
//
// em qualquer caso, devolve a distância de u a v em distancia e
// devolve o tamanho do caminho como valor da função

int caminho_minimo(vertice *c, float *distancia, vertice u, vertice v, grafo g);

//------------------------------------------------------------------------------
// devolve 1, se v Ã© um vÃ©rtice simplicial em g, 
//         ou
//         0, caso contrÃ¡rio
//
// um vÃ©rtice Ã© simplicial no grafo se sua vizinhanÃ§a Ã© uma clique

int simplicial(vertice v, grafo g);

//------------------------------------------------------------------------------
// devolve o diÃ¢metro do grafo (direcionado e/ou ponderado) g

int diametro(grafo g);

//------------------------------------------------------------------------------
// devolve o número de componentes fortes de g,
//         e um vetor de grafos com os componentes fortes de g em componentes

int componentes_fortes(grafo g, grafo *componentes);

//------------------------------------------------------------------------------
// devolve um vetor de numero_vertices(g) vertices com uma ordenação
//         topológica de g
//         ou 
//         NULL, caso g seja cíclico

vertice *ordenacao_topologica(grafo g);
//------------------------------------------------------------------------------
#endif
