#pragma once
#ifndef CONEXAOPG_H
#define CONEXAOPG_H

#include "ResultadoPG.h"
using namespace std;

class ConexaoPG {
private:
	PGconn *conexao;
	bool conectado;

public:

	~ConexaoPG() {
		if (this->conectado) {
			this->encerrarConexao();
		}
	}

	ConexaoPG(string host, string usuario, string senha, string database, string porta = "5432") {
		this->conectado = false;
		string connectionString;
		if (senha.size() != 0) {
			connectionString = "host=" + host + " user=" + usuario + " password=" + senha + " dbname=" + database + " port=" + porta;
		} else {
			connectionString = "host=" + host + " user=" + usuario + " dbname=" + database + " port=" + porta;
		}

		this->conexao = PQconnectdb(connectionString.c_str());
		this->conectado = (this->status() != CONNECTION_BAD);
	}

	string ultimoErro() {
		return PQerrorMessage(this->conexao);
	}


	short int setClientEncoding(string clientEncode = "WIN1252") {
		if (conexaoIniciada()) {
			return PQsetClientEncoding(this->conexao, clientEncode.c_str());
		}
		return -1;
	}

	int status() {
		return PQstatus(this->conexao);
	}

	ResultadoPG* query(string comando) {

		PGresult *consulta = PQexec(this->conexao, comando.c_str());
		ResultadoPG *resultado = new ResultadoPG(consulta);
		return resultado;
	}

	ResultadoPG* assyncQuery(string comando) {
		if (this->status() == CONNECTION_OK) {
			if (PQsendQuery(this->conexao, comando.c_str())) {
				PGresult *aux;
				ResultadoPG *resultado = NULL;
				while (aux = PQgetResult(this->conexao)) {
					switch (PQresultStatus(aux)) {
						case PGRES_TUPLES_OK:
						case PGRES_BAD_RESPONSE:
						case PGRES_FATAL_ERROR:
						case PGRES_COMMAND_OK:
							resultado = new ResultadoPG(aux);
					}	
				}
				return resultado;
			}
		}
		return NULL;
	}

	string escapeString(string str) {
		return PQescapeLiteral(this->conexao, str.c_str(), str.size());
	}

	void encerrarConexao() {
		if (!conectado) {
			//throw new exception("Não existem conexões iniciadas");
		}
		
		this->conectado = false;
		PQfinish(this->conexao);
	}

	bool conexaoIniciada() {
		if (this->conectado && PQstatus(this->conexao) != CONNECTION_OK) {
			this->conectado = false;
			return false;
		}
		return this->conectado;
	}
};

#endif // !ConexaoPG_H
