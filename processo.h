#ifndef PROCESSO_H
#define PROCESSO_H
#include <time.h>

#define MAX_PROCESSOS 1000

typedef struct {
    int id;
    int id_classe;
    int id_assunto;
    char data_ajuizamento[30];
    char assuntos_extra[256];
} Processo;

// Funções principais
int carregar_processos(const char *filename, Processo processos[], int *quantidade);
void salvar_csv(const char *filename, Processo processos[], int quantidade);

// Ordenações
void ordenar_por_id(Processo processos[], int quantidade);
void ordenar_por_data(Processo processos[], int quantidade);

// Contagens e análises
int contar_por_classe(Processo processos[], int quantidade, int id_classe);
int contar_assuntos_unicos(Processo processos[], int quantidade);
void listar_multiplos_assuntos(Processo processos[], int quantidade);
int dias_em_tramitacao(Processo processo);

// Função utilitária substituta do strptime
int parse_datetime(const char *datetime_str, struct tm *tm);

#endif
