#pragma once
#ifndef RSULTADOPG_H
#define RESULTADOPG_H

#include <string>
#include <libpq-fe.h>

using namespace std;
class ResultadoPG {
public:
	PGresult *dados;
	int quantidadeColunas;
	int quantidadeLinhas;

	ResultadoPG(PGresult *&resultado) {

		this->dados = resultado;
		this->quantidadeLinhas = PQntuples(resultado);
		this->quantidadeColunas = PQnfields(resultado);
	}

	string erro() {
		return PQresultErrorMessage(this->dados);
	}

	int status() {
		return PQresultStatus(this->dados);
	}

	void apagar() {
		PQclear(this->dados);
	}

	bool verificarNulo(int linha, int coluna) {
		return PQgetisnull(this->dados, linha, coluna);
	}

	string recuperarDado(int linha, int coluna) {
		return PQgetvalue(this->dados, linha, coluna);
	}

	string recuperarColuna(int coluna) {
		return PQfname(this->dados, coluna);
	}
};

#endif // !RSULTADOPG_H

