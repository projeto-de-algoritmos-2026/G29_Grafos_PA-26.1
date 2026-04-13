#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

class PlanejadorDeTarefas {
private:
    map<string, vector<string>> adj;
    map<string, int> grauEntrada;
    vector<string> todasAsTarefas;
    
    int custoEmpiricoTotal = 0;
    vector<string> logCustos;

    void registrarCusto(string operacao, string detalhes, string complexidade, int passos) {
        custoEmpiricoTotal += passos;
        string entradaLog = "[Op: " + operacao + " (" + detalhes + ")] | Teorico: " + complexidade + " | Passos CPU: " + to_string(passos);
        logCustos.push_back(entradaLog);
    }

    bool existeCaminho(string origem, string destino) {
        int passos = 0; 
        if (origem == destino) return true;
        
        queue<string> fila;
        map<string, bool> visitado;
        fila.push(origem);
        visitado[origem] = true;
        
        while (!fila.empty()) {
            string atual = fila.front();
            fila.pop();
            passos++; 
            
            for (const string& vizinho : adj[atual]) {
                passos++; 
                if (vizinho == destino) {
                    registrarCusto("Busca de Ciclo", origem + " -> " + destino, "O(V + E)", passos);
                    return true;
                }
                if (!visitado[vizinho]) {
                    visitado[vizinho] = true;
                    fila.push(vizinho);
                }
            }
        }
        registrarCusto("Verificacao de Rota", origem + " -> " + destino, "O(V + E)", passos);
        return false;
    }

    void registrarTarefa(string nome) {
        if (find(todasAsTarefas.begin(), todasAsTarefas.end(), nome) == todasAsTarefas.end()) {
            todasAsTarefas.push_back(nome);
            if (grauEntrada.find(nome) == grauEntrada.end()) grauEntrada[nome] = 0;
            registrarCusto("Registro de Vertice", nome, "O(1)", 1);
        }
    }

public:
    void adicionarTarefaInterativa() {
        string nomeTarefa, pre;
        char temPre, mais;
        cout << "\nNome da tarefa: "; getline(cin, nomeTarefa);
        registrarTarefa(nomeTarefa);
        
        cout << "Possui pre-requisitos? (s/n): "; cin >> temPre; cin.ignore();
        
        if (temPre == 's' || temPre == 'S') {
            do {
                cout << "Nome do pre-requisito: "; getline(cin, pre);
                registrarTarefa(pre);
                
                if (existeCaminho(nomeTarefa, pre)) {
                    cout << "[!] ERRO: '" << pre << "' causaria um ciclo infinito. Operacao cancelada." << endl;
                } else {
                    adj[pre].push_back(nomeTarefa); 
                    grauEntrada[nomeTarefa]++;
                    registrarCusto("Insercao de Aresta", pre + " -> " + nomeTarefa, "O(1)", 1);
                    cout << "[Ok] Dependencia registrada." << endl;
                }
                cout << "Adicionar outro pre-requisito para '" << nomeTarefa << "'? (s/n): "; 
                cin >> mais; cin.ignore();
            } while (mais == 's' || mais == 'S');
        }
    }

    void gerenciarPreRequisitos() {
        string nome, pre;
        int subOpt;
        cout << "\nTarefa a modificar: "; getline(cin, nome);
        if (find(todasAsTarefas.begin(), todasAsTarefas.end(), nome) == todasAsTarefas.end()) {
            cout << "[!] Tarefa nao encontrada." << endl; return;
        }

        cout << "1. Adicionar pre-requisito\n2. Remover pre-requisito\nEscolha: ";
        cin >> subOpt; cin.ignore();

        if (subOpt == 1) {
            cout << "Novo pre-requisito: "; getline(cin, pre);
            registrarTarefa(pre);
            if (existeCaminho(nome, pre)) cout << "[!] ERRO: Criaria um ciclo!" << endl;
            else { 
                adj[pre].push_back(nome); grauEntrada[nome]++; 
                registrarCusto("Modificacao Grafo (Add)", pre + " -> " + nome, "O(1)", 1);
                cout << "[Ok] Adicionado." << endl; 
            }
        } else if (subOpt == 2) {
            cout << "Pre-requisito a remover: "; getline(cin, pre);
            auto it = find(adj[pre].begin(), adj[pre].end(), nome);
            if (it != adj[pre].end()) {
                adj[pre].erase(it); grauEntrada[nome]--; 
                registrarCusto("Modificacao Grafo (Rem)", pre + " -> " + nome, "O(E)", 1);
                cout << "[Ok] Removido." << endl;
            } else cout << "[!] Relacao nao encontrada." << endl;
        }
    }

    void renomearTarefa() {
        string antiga, nova;
        cout << "\nNome atual: "; getline(cin, antiga);
        auto it = find(todasAsTarefas.begin(), todasAsTarefas.end(), antiga);
        if (it == todasAsTarefas.end()) { cout << "[!] Nao encontrada." << endl; return; }
        
        cout << "Novo nome: "; getline(cin, nova);
        *it = nova;
        adj[nova] = adj[antiga]; adj.erase(antiga);
        grauEntrada[nova] = grauEntrada[antiga]; grauEntrada.erase(antiga);
        
        int passos = 0;
        for (auto &par : adj) {
            for (string &v : par.second) {
                passos++;
                if (v == antiga) v = nova;
            }
        }
        registrarCusto("Renomeacao de Tarefa", antiga + " para " + nova, "O(V + E)", passos);
        cout << "[Ok] Nome atualizado." << endl;
    }

    void visualizarMapa() {
        if (todasAsTarefas.empty()) { cout << "\n[!] Vazio." << endl; return; }
        cout << "\n" << setfill('=') << setw(60) << "" << endl;
        cout << left << setw(30) << "TAREFA" << "DEPENDENCIAS (Pre-requisitos)" << endl;
        cout << setfill('-') << setw(60) << "" << setfill(' ') << endl;
        for (const string& t : todasAsTarefas) {
            cout << left << setw(30) << t;
            bool tem = false;
            for (auto const& par : adj) for (const string& d : par.second) 
                if (d == t) { cout << par.first << "  "; tem = true; }
            if (!tem) cout << "(Nenhum)";
            cout << endl;
        }
        cout << setfill('=') << setw(60) << "" << setfill(' ') << endl;
    }

    void gerarFluxograma() {
        int passosKahn = 0;
        map<string, int> grausTemp = grauEntrada;
        queue<string> fila;
        vector<vector<string>> niveis; 
        
        for (const string& t : todasAsTarefas) {
            passosKahn++;
            if (grausTemp[t] == 0) fila.push(t);
        }

        if (fila.empty() && !todasAsTarefas.empty()) { 
            cout << "\n[!] ERRO: Ciclo detectado! Impossivel gerar fluxo." << endl; 
            return; 
        }

        while (!fila.empty()) {
            int tam = fila.size();
            vector<string> nivelAtual;
            for (int i = 0; i < tam; i++) {
                string atual = fila.front(); fila.pop();
                nivelAtual.push_back(atual);
                passosKahn++;
                for (const string& v : adj[atual]) {
                    grausTemp[v]--; passosKahn++;
                    if (grausTemp[v] == 0) fila.push(v);
                }
            }
            niveis.push_back(nivelAtual);
        }

        registrarCusto("Geracao de Fluxograma", "Algoritmo de Kahn", "O(V + E)", passosKahn);

        cout << "\n\n============= ARVORE DE EXECUCAO ==================" << endl;
        for (size_t i = 0; i < niveis.size(); i++) {
            cout << "[ ETAPA " << i + 1 << " ]" << endl;
            
            for (const string& t : niveis[i]) {
                cout << "  |-- [" << t << "]";
                
                vector<string> dependencias;
                for (auto const& par : adj) {
                    for (const string& d : par.second) {
                        if (d == t) dependencias.push_back(par.first);
                    }
                }
                
                if (!dependencias.empty()) {
                    cout << "  <-- (Desbloqueado por: ";
                    for (size_t j = 0; j < dependencias.size(); j++) {
                        cout << dependencias[j] << (j < dependencias.size() - 1 ? ", " : "");
                    }
                    cout << ")";
                } else {
                    cout << "  <-- (Independente / Pronto agora)";
                }
                cout << endl;
            }
            
            if (i < niveis.size() - 1) {
                cout << "  |" << endl;
                cout << "  V" << endl;
            }
        }
        cout << "===================================================" << endl;
    }


    void salvar() {
        ofstream f("plano.txt");
        for(const string& t : todasAsTarefas) f << "T;" << t << endl;
        for(auto const& p : adj) for(const string& v : p.second) f << "D;" << p.first << ";" << v << endl;
        f.close(); cout << "[Ok] Salvo em plano.txt" << endl;
        registrarCusto("Escrita em Disco", "Backup txt", "O(V + E)", todasAsTarefas.size());
    }

    void carregar() {
        ifstream f("plano.txt");
        if (!f.is_open()) { cout << "[!] Arquivo nao encontrado." << endl; return; }
        adj.clear(); grauEntrada.clear(); todasAsTarefas.clear();
        string l;
        int passos = 0;
        while (getline(f, l)) {
            passos++;
            if (l.substr(0, 2) == "T;") registrarTarefa(l.substr(2));
            else if (l.substr(0, 2) == "D;") {
                size_t p = l.find(";", 2);
                string pre = l.substr(2, p - 2), post = l.substr(p + 1);
                adj[pre].push_back(post); grauEntrada[post]++;
            }
        }
        f.close(); cout << "[Ok] Projeto carregado." << endl;
        registrarCusto("Leitura de Disco", "Load txt", "O(V + E)", passos);
    }
};

int main() {
    PlanejadorDeTarefas projeto;
    int opt;
    do {
        cout << "\n--- GERENCIADOR DE PROJETOS ---" << endl;
        cout << "1. Cadastrar Tarefa / Dependencias" << endl;
        cout << "2. Gerenciar Pre-requisitos (Editar)" << endl;
        cout << "3. Renomear Tarefa" << endl;
        cout << "4. Ver Mapa de Relacoes (Tabela)" << endl;
        cout << "5. Gerar Fluxograma (Visual)" << endl; 
        cout << "6. Salvar Projeto (.txt)" << endl;
        cout << "7. Carregar Projeto (.txt)" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha: ";
        cin >> opt; cin.ignore();
        
        switch(opt) {
            case 1: projeto.adicionarTarefaInterativa(); break;
            case 2: projeto.gerenciarPreRequisitos(); break;
            case 3: projeto.renomearTarefa(); break;
            case 4: projeto.visualizarMapa(); break;
            case 5: projeto.gerarFluxograma(); break;
            case 6: projeto.salvar(); break;
            case 7: projeto.carregar(); break;
            case 0: cout << "Encerrando..." << endl; break;
            default: cout << "Opcao invalida!" << endl; break;
        }
    } while (opt != 0);
    return 0;
}