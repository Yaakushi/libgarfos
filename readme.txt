Trabalho I de CI065 (2017/2)

Alunos:
GRR20148763 - Guilherme Bittencourt
GRR20143061 - Kaio Augusto de Camargo

* Por que utilizamos matriz de adjacência ao invés de uma lista de adjacência?

Escolhemos a matriz de adjacência pois, por mais que tivessemos preferência pela
lista de adjacência, acabamos concluindo que implementar uma lista encadeada em
apenas um arquivo .c poderia não ser exatamente a melhor opção. Não nos
beneficiamos muitas vezes no código do tempo constante para checar se uma
aresta em específico existe, ao passo que achamos que funções como a
"proximo_vizinho" poderiam ficar bem mais simples (e até mais eficientes) com
uma lista de adjacência. Apesar disso, outras funções foram facilitadas pelo uso 
da matriz de adjacência (por exemplo, diâmetro), por isso resolvemos manter a
matriz de adjacência, mas poderemos trocar caso haja mais pontos positivos na
lista de adjacência nos próximos trabalhos.

* Detalhes adicionais para levar em conta na correção:

- Na função "proximo_vizinho", escolhemos, por conveniência, pegar o próximo
vizinho do grafo de acordo com a matriz de adjacência, cujos vértices são
"ordenados" de acordo com a ordem que apareceram no arquivo DOT original.

- Vimos algumas otimizações que pareciam interessantes para alguma das funções
desse trabalho (simplicial e diâmetro, por exemplo), mas escolhemos não implementá-las
pois não sabiamos com quais tipos de grafo nossa biblioteca iria trabalhar no
futuro (com peso, direcionado, não direcionado, denso, esparso, etc).

* Bugs conhecidos:
Nenhum.
