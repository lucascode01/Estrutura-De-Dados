#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_CLASSES 10
#define MAX_ASSUNTOS 10

typedef struct {
    long id;
    char numero[25];
    char data_ajuizamento[25];
    int id_classe[MAX_CLASSES];
    int num_classes;
    int id_assunto[MAX_ASSUNTOS];
    int num_assuntos;
    int ano_eleicao;


} Processo;




void ler_csv(const char *nome_arquivo, Processo processos[], int *n);
void ordenar_por_id(Processo processos[], int n);
void ordenar_por_data(Processo processos[], int n);
int contar_por_classe(Processo processos[], int n, int classe);
int contar_assuntos_diferentes(Processo processos[], int n);
void listar_varios_assuntos(Processo processos[], int n);
int dias_em_tramitacao(const char *data_str);



#endif