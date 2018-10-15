#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

#define PGCON_CONEXAO_INEXISTENTE		1000
#define PGRES_RESULTADO_INEXISTENTE		1001
#define PGRES_CONVERSAO_INVALIDA		1003
#define PGRES_CONVERSAO_OUTOFRANGE		1004
#define PGCON_CHARSET_INVALIDO			1005
#define PGCON_CONEXAO_ERROINDEFINIDO	1006
#define PGCON_CONEXAO_INEXISTENTE_MSG	"PGConnector: ID de Conexão inexistente"
#define PGRES_RESULTADO_INEXISTENTE_MSG	"PGConnector: ID de resultado inexistente"
#define PGRES_CONVERSAO_INVALIDA_MSG	"PGConnector: Conversão inválida"
#define PGRES_CONVERSAO_OUTOFRANGE_MSG	"PGConnector: Tentativa de conversão de valor fora da Range permitida"
#define PGCON_CHARSET_INVALIDO_MSG		"PGConnector: Charset inválido"

#include <libpq-fe.h>
#include "ConexaoPG.h"
#include "Conexoes.h"
#include "Resultados.h"

Conexoes *conexoes;
Resultados *resultados;
#define PLUGIN_VERSION "1.0"
typedef void(*logprintf_t)(const char* format, ...);

logprintf_t logprintf;
void **ppPluginData;
extern void *pAMXFunctions;

#include <thread>
#include "auxiliar.h"
#include "nativas.h"
#include <string>
#include <unistd.h>


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf("\nInicializando PGConnector...\n");
	conexoes = new Conexoes();
	resultados = new Resultados();
	
	logprintf("===============================");
	logprintf("PGConnector por Mandrack_FreeZe");
	logprintf("Versão: %s", PLUGIN_VERSION);
	logprintf("Versão da libpq: %d", PQlibVersion());
	logprintf("===============================\n");
	logprintf("PGConnector inicializada com sucesso.\n");
	return true;
	
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("PGConnector foi descarregada com sucesso.");
}


AMX_NATIVE_INFO nativas[] =
{
	{ "PG_conectar",			PG_conectar },
	{ "PG_query",				PG_query },
	{ "PG_assyncQuery",			PG_assyncQuery},
	{ "PG_ultimoErro",			PG_ultimoErro},
	{ "PG_status",				PG_status},
	{ "PG_statusResposta",		PG_statusResposta},
	{ "PG_erroResposta",		PG_erroResposta},
	{ "PG_quantidadeLinhas",	PG_quantidadeLinhas},
	{ "PG_quantidadeColunas",	PG_quantidadeColunas},
	{ "PG_recuperarInt",		PG_recuperarInt},
	{ "PG_recuperarStr",		PG_recuperarStr},
	{ "PG_recuperarFloat",		PG_recuperarFloat},
	{ "PG_setClientEncode",		PG_setClientEncode},
	{ "PG_encerrarConexao",		PG_encerrarConexao},
	{ "PG_liberarResposta",		PG_liberarResposta},
	{ "PG_verificarNulo",		PG_verificarNulo},
	{ "PG_escapeString",		PG_escapeString},
	{ 0,						0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	return amx_Register(amx, nativas, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	return AMX_ERR_NONE;
}

