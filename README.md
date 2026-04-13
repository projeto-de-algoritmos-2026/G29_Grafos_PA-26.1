# G29_Grafos_PA-26.1

Número da lista: 26
Conteúdo da disciplina: Ordenação Topológica

## Alunos 
| Nome | Matrícula |
|------|-----------|
| | |
| Vilmar José Fagundes | 231026590 |

## Sobre
Esse projeto implementa um Gerenciador de Projetos e Tarefas baseado em Teoria dos Grafos, onde cada tarefa é um vértice e cada dependência (pré-requisito) é uma aresta direcionada.

A lógica do sistema está na função de gerar o fluxograma, que utiliza o Algoritmo de Kahn para realizar uma ordenação topológica. Garantindo rigorosamente que uma tarefa só apareça na linha do tempo após todos os seus pré-requisitos terem sido satisfeitos. O Algoritmo de Kahn resolve esse quebra-cabeça calculando o "grau de entrada" de cada atividade, que nada mais é do que o número de dependências que a bloqueiam. As tarefas que não possuem pré-requisitos, ou seja, com grau zero, são inseridas em uma fila de prontidão.

Conforme o sistema processa essa fila, ele retira as tarefas prontas e simula a conclusão delas, reduzindo o grau de entrada de todas as tarefas subsequentes que dependiam delas. Quando uma dessas tarefas dependentes finalmente tem seu grau reduzido a zero, ela é considerada desbloqueada e entra na fila para a próxima etapa. Esse ciclo continua até que o projeto inteiro seja organizado em níveis de execução simultânea. Caso o algoritmo termine de processar a fila, mas ainda existam tarefas pendentes no sistema, ele detecta automaticamente que há um ciclo lógico insolúvel no projeto, como duas tarefas que dependem uma da outra, e alerta o usuário. Para prevenir que esses ciclos sequer sejam formados durante a fase de planejamento, o programa também utiliza algoritmos de busca preventivos a cada nova dependência cadastrada, garantindo a integridade e a viabilidade do cronograma gerado.

## Screenshots
