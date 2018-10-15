#pragma once
#ifndef NATIVAS_H
#define NATIVAS_H

//native PG_conectar(host[], porta[], usuario[], senha[], database[]);
static cell AMX_NATIVE_CALL PG_conectar(AMX* amx, cell* params)
{
	string host, porta, usuario, senha, database;
	host = recuperarString(amx, params[1]);
	porta = recuperarString(amx, params[2]);
	usuario = recuperarString(amx, params[3]);
	senha = recuperarString(amx, params[4]);
	database = recuperarString(amx, params[5]);

	ConexaoPG *conexao = new ConexaoPG(host, usuario, senha, database, porta);
	if (conexao->status() != CONNECTION_OK) {
		chamarCallbackErro(amx, conexao->status(), conexao->ultimoErro());
	}
	return conexoes->adicionarConexao(conexao);
}

//native PG_status(idConexao);
cell AMX_NATIVE_CALL PG_status(AMX* amx, cell* params) {
	ConexaoPG *conexao = conexoes->recuperarConexao(params[1]);
	if (conexao == NULL) {
		//chamarCallbackErro(amx, PGCON_CONEXAO_INEXISTENTE, PGCON_CONEXAO_INEXISTENTE_MSG);
		return PGCON_CONEXAO_INEXISTENTE;
	}

	return conexao->status();
}

//native PG_ultimoErro(idConexao, strErro[], sizeofStrErro);
cell AMX_NATIVE_CALL PG_ultimoErro(AMX* amx, cell* params) {
	ConexaoPG *conexao = conexoes->recuperarConexao(params[1]);
	if (conexao == NULL) {
		chamarCallbackErro(amx, PGCON_CONEXAO_INEXISTENTE, PGCON_CONEXAO_INEXISTENTE_MSG);
		return -1;
	}
	setString(amx, params[2], params[3], conexao->ultimoErro());
	return 1;
}

//native PG_query(idConexao, query[]);
cell AMX_NATIVE_CALL PG_query(AMX* amx, cell* params) {
	ConexaoPG *conexao = conexoes->recuperarConexao(params[1]);
	if (conexao == NULL) {
		chamarCallbackErro(amx, PGCON_CONEXAO_INEXISTENTE, PGCON_CONEXAO_INEXISTENTE_MSG);
		return -1;
	}
	ResultadoPG *resultado = conexao->query(recuperarString(amx, params[2]));
	return resultados->adicionarResultado(resultado, params[1]);
}

//native PG_erroResposta(idResposta, strErro[], sizeofStrErro);
cell AMX_NATIVE_CALL PG_erroResposta(AMX* amx, cell* params) {
	ResultadoPG *resultado = resultados->recuperarResultado(params[1]);
	if (resultado->status() != PGRES_TUPLES_OK) {
		setString(amx, params[2], params[3], resultado->erro());
		return 1;
	}
	setString(amx, params[2], params[3], "");
	return 0;
}

//native PG_statusResposta(idResposta);
cell AMX_NATIVE_CALL PG_statusResposta(AMX *amx, cell* params) {
	ResultadoPG *resultado = resultados->recuperarResultado(params[1]);
	if (resultado == NULL) {
		//chamarCallbackErro(amx, PGRES_RESULTADO_INEXISTENTE, PGRES_RESULTADO_INEXISTENTE_MSG);
		return PGRES_RESULTADO_INEXISTENTE;
	}
	return resultado->status();
}

//native PG_quantidadeLinhas(idResultado);
cell AMX_NATIVE_CALL PG_quantidadeLinhas(AMX* amx, cell* params) {
	if (resultados->recuperarResultado(params[1]) == NULL) {
		return -1;
	}
	return resultados->recuperarResultado(params[1])->quantidadeLinhas;
}

//native PG_quantidadeColunas(idResultado);
cell AMX_NATIVE_CALL PG_quantidadeColunas(AMX *amx, cell *params) {
	if (resultados->recuperarResultado(params[1]) == NULL) {
		return -1;
	}
	return resultados->recuperarResultado(params[1])->quantidadeColunas;
}

//native PG_verificarNulo(idResultado, linha, coluna);
cell AMX_NATIVE_CALL PG_verificarNulo(AMX *amx, cell *params) {
	if (resultados->recuperarResultado(params[1]) == NULL) {
		return PGRES_RESULTADO_INEXISTENTE;
	}
	return resultados->recuperarResultado(params[1])->verificarNulo(params[2], params[3]);
}

//native PG_recuperarStr(idResultado, linha, coluna, strValor[], sizeofStrValor);
cell AMX_NATIVE_CALL PG_recuperarStr(AMX *amx, cell *params) {
	setString(amx, params[4], params[5], resultados->recuperarResultado(params[1])->recuperarDado(params[2], params[3]));
	return NULL;
}

//native Float:PG_recuperarFloat(idResultado, linha, coluna);
cell AMX_NATIVE_CALL PG_recuperarFloat(AMX *amx, cell *params) {
	ResultadoPG *resultado = resultados->recuperarResultado(params[1]);
	string valor = resultado->recuperarDado(params[2], params[3]);
	if (valor == "") {
		logprintf("testando");
		return NULL;
	}

	float pontoFlutuante;
	try {
		pontoFlutuante = stof(valor);
	}
	catch (invalid_argument ex) {
		chamarCallbackErro(amx, PGRES_CONVERSAO_INVALIDA, ex.what());
		return '\0';
	}
	catch (out_of_range ex) {
		chamarCallbackErro(amx, PGRES_CONVERSAO_OUTOFRANGE, ex.what());
		return '\0';
	}

	return amx_ftoc(pontoFlutuante);
}

//native PG_recuperarInt(idResultado, linha, coluna);
cell AMX_NATIVE_CALL PG_recuperarInt(AMX *amx, cell *params) {
	ResultadoPG *resultado = resultados->recuperarResultado(params[1]);
	string valor = resultado->recuperarDado(params[2], params[3]);
	int inteiro;

	try {
		inteiro = stoi(valor.c_str(), NULL, 10);
	}
	catch (invalid_argument ex) {
		chamarCallbackErro(amx, PGRES_CONVERSAO_INVALIDA, ex.what());
		return -1;
	}
	catch (out_of_range ex) {
		chamarCallbackErro(amx, PGRES_CONVERSAO_OUTOFRANGE, ex.what());
		return -1;
	}

	return inteiro;
}

//native PG_clientEncode(idConexao, str[] = "WIN1252");
cell AMX_NATIVE_CALL PG_setClientEncode(AMX *amx, cell *params) {
	if (conexoes->recuperarConexao(params[1])->setClientEncoding(recuperarString(amx, params[2])) != 0) {
		chamarCallbackErro(amx, PGCON_CHARSET_INVALIDO, PGCON_CHARSET_INVALIDO_MSG);
		return -1;
	}
	return 0;
}

//native PG_encerrarConexao(idConexao);
cell AMX_NATIVE_CALL PG_encerrarConexao(AMX *amx, cell *params) {
	if (conexoes->recuperarConexao(params[1]) == NULL) {
		return PGCON_CONEXAO_INEXISTENTE;
	}
	conexoes->recuperarConexao(params[1])->encerrarConexao();
	return 1;

}

//native PG_liberarResposta(idResposta);
cell AMX_NATIVE_CALL PG_liberarResposta(AMX *amx, cell *params) {
	if (resultados->recuperarResultado(params[1]) == NULL) {
		return PGRES_RESULTADO_INEXISTENTE;
	}
	resultados->removerResultado(params[1]);
	return 1;
}

//native PG_escapeString(idConexao, strOrigem[], strDestino[], sizeofStrDestino);
cell AMX_NATIVE_CALL PG_escapeString(AMX *amx, cell *params) {
	ConexaoPG *conexao = conexoes->recuperarConexao(params[1]);
	if (conexao == NULL) {
		return PGCON_CONEXAO_INEXISTENTE;
	}
	setString(amx, params[3], params[4], conexao->escapeString(recuperarString(amx, params[2])));
	return 1;
}

//native PG_assyncQuery(idConexao, comando[], callbackFunction[]);
cell AMX_NATIVE_CALL PG_assyncQuery(AMX *amx, cell *params) {
	thread(assyncQuery, amx, params[1], recuperarString(amx, params[2]), recuperarString(amx, params[3])).detach();
	return 1;
}

#endif // !NATIVAS_H
