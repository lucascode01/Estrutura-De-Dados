#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_PROCESSOS 1000
#define MAX_ASSUNTOS 10

typedef struct {
    int id;
    char numero[50];
    char data_ajuizamento[20];
    int id_classe;
    int id_assunto[MAX_ASSUNTOS];
    int qtd_assuntos;
    int ano_eleicao;
} Processo;

int carregar_processos(const char *filename, Processo processos[], int *quantidade);
void salvar_csv(const char *filename, Processo processos[], int quantidade);
void ordenar_por_id(Processo processos[], int quantidade);
void ordenar_por_data(Processo processos[], int quantidade);
int contar_por_classe(Processo processos[], int quantidade, int id_classe);
int contar_assuntos_unicos(Processo processos[], int quantidade);
void listar_multiplos_assuntos(Processo processos[], int quantidade);
int dias_em_tramitacao(Processo processo);

#endif