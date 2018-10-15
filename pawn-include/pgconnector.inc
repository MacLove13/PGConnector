//Status de conexão
#define PG_CONNECTION_OK 					0
#define PG_CONNECTION_BAD 					1
#define PG_CONNECTION_STARTED 					2
#define PG_CONNECTION_MADE 					3
#define PG_CONNECTION_AWAITING_RESPONSE 			4
#define PG_CONNECTION_AUTH_OK 					5
#define PG_CONNECTION_SETENV 					6
#define PG_CONNECTION_SSL_STARTUP 				7
#define PG_CONNECTION_NEEDED 					8

//Status de respostas
#define PGRES_EMPTY_QUERY 					0
#define PGRES_COMMAND_OK 					1
#define PGRES_TUPLES_OK 					2
#define PGRES_COPY_OUT 						3
#define PGRES_COPY_IN 						4
#define PGRES_BAD_RESPONSE 					5
#define PGRES_NONFATAL_ERROR 					6
#define PGRES_FATAL_ERROR 					7
#define PGRES_COPY_BOTH 					8
#define PGRES_SINGLE_TUPLE 					9

//Erros internos da plugin
#define PGCON_CONEXAO_INEXISTENTE				1000
#define PGRES_RESULTADO_INEXISTENTE				1001
#define PGRES_CONVERSAO_INVALIDA				1003
#define PGRES_CONVERSAO_OUTOFRANGE				1004
#define PGCON_CHARSET_INVALIDO					1005
#define PGCON_CONEXAO_ERROINDEFINIDO				1006

//Nulo
#define PG_NULL							1

//Nativas responsáveis pela manipulação das conexões
native PG_conectar(host[], porta[], usuario[], senha[], database[]);
native PG_encerrarConexao(idConexao);
native PG_status(idConexao);
native PG_ultimoErro(idConexao, strErro[], sizeofStrErro);
native PG_setClientEncode(idConexao, encode[] = "WIN1252");
native PG_escapeString(idConexao, strOrigem[], strDestino[], sizeofStrDestino);

//Nativas responsáveis pela manipulação de respostas
native PG_query(idConexao, query[]);
native PG_assyncQuery(idConexao, query[], callbackFunction[]);
native PG_statusResposta(idResposta);
native PG_erroResposta(idResposta, strErro[], sizeofStrErro);
native PG_liberarResposta(idResposta);
native PG_quantidadeColunas(idResultado);
native PG_quantidadeLinhas(idResultado);
native PG_recuperarInt(idResultado, linha, coluna);
native Float:PG_recuperarFloat(idResultado, linha, coluna);
native PG_recuperarStr(idResultado, linha, coluna, strValor[], sizeofStrValor);
native PG_verificarNulo(idResultado, linha, coluna);
native PG_escapeString(idConexao, strOrigem[], strDestino[], sizeofStrDestino); 

//Forwards
forward OnPostgreSQLError(errorCode, erro[]);