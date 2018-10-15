#pragma once
#ifndef  RESULTADOS_H
#define RESULTADOS_H
#include "Conexoes.h"
#include "ResultadoPG.h"

#define MAX_STORED_RESULTS (MAX_OPEN_CONNECTIONS*10)
class Resultados {
private:
	int id[MAX_STORED_RESULTS];
	int idConexao[MAX_STORED_RESULTS];
	ResultadoPG *resultados[MAX_STORED_RESULTS];

public:
	Resultados() {
		for (int i = 0; i < MAX_STORED_RESULTS; ++i) {
			this->id[i] = -1;
			this->idConexao[i] = -1;
			this->resultados[i] = NULL;
		}
	}
	
	bool chegouNoLimite() {
		for (int i = 0; i < MAX_STORED_RESULTS; ++i) {
			if (this->id[i] == -1) {
				return false;
			}
		}
		return true;
	}

	int adicionarResultado(ResultadoPG *&resultado, int idConexao) {
		for (int i = 0; i < MAX_STORED_RESULTS; ++i) {
			if (this->id[i] == -1) {
				this->id[i] = i;
				this->idConexao[i] = idConexao;
				this->resultados[i] = resultado;
				return i;
			}
		}
		return -1;
	}

	void removerResultado(int idResultado) {
		if (idResultado >= 0 && idResultado < MAX_STORED_RESULTS) {
			this->id[idResultado] = -1;
			this->idConexao[idResultado] = -1;
			this->resultados[idResultado]->apagar();
			delete this->resultados[idResultado];
			this->resultados[idResultado] = NULL;
		}
	}

	ResultadoPG* recuperarResultado(int idResultado) {
		if (idResultado >= 0 && idResultado < MAX_STORED_RESULTS) {
			return this->resultados[idResultado];
		}
		return NULL;
	}
};
#endif // ! RESULTADOS_H
