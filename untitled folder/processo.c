#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

int carregar_processos(const char *filename, Processo processos[], int *quantidade) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    char linha[256];
    fgets(linha, sizeof(linha), file);
    *quantidade = 0;

    while (fgets(linha, sizeof(linha), file)) {
        Processo p;
        char *token = strtok(linha, ",");
        p.id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(p.numero, token);

        token = strtok(NULL, ",");
        strncpy(p.data_ajuizamento, token, 19);
        p.data_ajuizamento[19] = '\0';

        token = strtok(NULL, ",{}");
        p.id_classe = atoi(token);

        p.qtd_assuntos = 0;
        token = strtok(NULL, "{};");
        while (token && p.qtd_assuntos < MAX_ASSUNTOS) {
            p.id_assunto[p.qtd_assuntos++] = atoi(token);
            token = strtok(NULL, "{};");
        }

        token = strtok(NULL, ",\n");
        p.ano_eleicao = atoi(token);

        processos[(*quantidade)++] = p;
    }

    fclose(file);
    return 1;
}

void salvar_csv(const char *filename, Processo processos[], int quantidade) {
    FILE *file = fopen(filename, "w");
    fprintf(file, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < quantidade; i++) {
        fprintf(file, "%d,%s,%s,%d,{", processos[i].id, processos[i].numero,
                processos[i].data_ajuizamento, processos[i].id_classe);
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            fprintf(file, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].qtd_assuntos - 1) fprintf(file, ";");
        }
        fprintf(file, "},%d\n", processos[i].ano_eleicao);
    }
    fclose(file);
}

int comparar_data(const char *a, const char *b) {
    return strcmp(a, b);
}

void ordenar_por_id(Processo processos[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (processos[j].id > processos[j + 1].id) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

void ordenar_por_data(Processo processos[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (comparar_data(processos[j].data_ajuizamento, processos[j + 1].data_ajuizamento) < 0) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

int contar_por_classe(Processo processos[], int quantidade, int id_classe) {
    int count = 0;
    for (int i = 0; i < quantidade; i++) {
        if (processos[i].id_classe == id_classe)
            count++;
    }
    return count;
}

int contar_assuntos_unicos(Processo processos[], int quantidade) {
    int unicos[10000] = {0};
    int total = 0;
    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int id = processos[i].id_assunto[j];
            if (!unicos[id]) {
                unicos[id] = 1;
                total++;
            }
        }
    }
    return total;
}

void listar_multiplos_assuntos(Processo processos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("ID %d com %d assuntos.\n", processos[i].id, processos[i].qtd_assuntos);
        }
    }
}

int dias_em_tramitacao(Processo processo) {
    struct tm data_ini = {0};
    strptime(processo.data_ajuizamento, "%Y-%m-%d %H:%M:%S", &data_ini);
    time_t inicio = mktime(&data_ini);
    time_t agora = time(NULL);
    double segundos = difftime(agora, inicio);
    return (int)(segundos / (60 * 60 * 24));
}