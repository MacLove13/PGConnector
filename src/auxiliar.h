#pragma once
#ifndef AUXILIAR_H
#define AUXILIAR_H

string recuperarString(AMX* &amx, cell parametro) {
	string retorno = "";
	int tamanho;
	cell *addr = NULL;
	amx_GetAddr(amx, parametro, &addr);
	amx_StrLen(addr, &tamanho);
	if (tamanho != 0) {
		tamanho++;
		char *texto = new char[tamanho];
		amx_GetString(texto, addr, 0, tamanho);
		retorno = texto;
		delete texto;
	}
	return retorno;
}

bool setString(AMX* &amx, cell &param, unsigned int tamanhoParam, string str) {
	cell* addr = NULL;
	amx_GetAddr(amx, param, &addr);
	amx_SetString(addr, str.c_str(), 0, 0, tamanhoParam);
	return true;
}

void chamarCallbackErro(AMX *&amx, int codErro, string erro) {
	int idx;
	if (!amx_FindPublic(amx, "OnPostgreSQLError", &idx)) {
		cell ret, addr;
		amx_PushString(amx, &addr, NULL, erro.c_str(), NULL, NULL);
		amx_Push(amx, codErro);
		amx_Exec(amx, &ret, idx);
		amx_Release(amx, addr);
	}
}

void enviarParametroStr(AMX *&amx, string str) {
	cell addr;
	amx_PushString(amx, &addr, NULL, str.c_str(), NULL, NULL);
}

void enviarParametroInt(AMX *amx, int parametro) {
	amx_Push(amx, parametro);
}

void assyncQuery(AMX *amx, int idConexao, string comando, string callback) {
	ResultadoPG *resultado = conexoes->recuperarConexao(idConexao)->assyncQuery(comando);
	if (resultado == NULL) {
		if (conexoes->recuperarConexao(idConexao)->status() != CONNECTION_OK) {
			chamarCallbackErro(amx, conexoes->recuperarConexao(idConexao)->status(), conexoes->recuperarConexao(idConexao)->ultimoErro());
			return;
		}
		chamarCallbackErro(amx, PGCON_CONEXAO_ERROINDEFINIDO, conexoes->recuperarConexao(idConexao)->ultimoErro());
		return;
	}
	int idResultado = resultados->adicionarResultado(resultado, idConexao);

	if (resultado->status() != PGRES_TUPLES_OK && resultado->status() != PGRES_COMMAND_OK) {
		chamarCallbackErro(amx, resultado->status(), resultado->erro());
		return;
	}

	int idx;
	if (!amx_FindPublic(amx, callback.c_str(), &idx)) {
		cell retorno;
		enviarParametroInt(amx, idResultado);
		enviarParametroInt(amx, idConexao);
		amx_Exec(amx, &retorno, idx);
	}
	else {
		resultados->removerResultado(idResultado);
	}
}

#endif 