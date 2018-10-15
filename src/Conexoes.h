#pragma once
#ifndef CONEXOES_H
#define CONEXOES_H
#define MAX_OPEN_CONNECTIONS 50
#include "ConexaoPG.h"
#include "Resultados.h"
struct Conexoes {
	int id[MAX_OPEN_CONNECTIONS];
	ConexaoPG *conexaoVinculada[MAX_OPEN_CONNECTIONS];

	Conexoes() {
		for (int i = 0; i < MAX_OPEN_CONNECTIONS; ++i) {
			this->id[i] = -1;
			this->conexaoVinculada[i] = NULL;
		}
	}

	int adicionarConexao(ConexaoPG *&conexao) {
		for (int i = 0; i < MAX_OPEN_CONNECTIONS; ++i) {
			if (this->id[i] == -1) {
				this->id[i] = i;
				this->conexaoVinculada[i] = conexao;
				return i;
			}
		}
		return -1;
	}

	bool removerConexao(int id) {
		if (id >= 0 && id < MAX_OPEN_CONNECTIONS) {
			this->id[id] = -1;
			if (this->conexaoVinculada[id] != NULL && this->conexaoVinculada[id]->conexaoIniciada()) {
				this->conexaoVinculada[id]->encerrarConexao();
			}
			delete this->conexaoVinculada[id];
			this->conexaoVinculada[id] = NULL;
			return true;
		}
		return false;
	}

	bool chegouNoLimite() {
		for (int i = 0; i < MAX_OPEN_CONNECTIONS; ++i) {
			if (this->id[i] == -1) {
				return false;
			}
		}
		return true;
	}

	ConexaoPG* recuperarConexao(int id) {
		if (id >= 0 && id <= MAX_OPEN_CONNECTIONS) {
			return this->conexaoVinculada[id];
		}
		return NULL;
	}
};

#endif // !CONEXOES_H


