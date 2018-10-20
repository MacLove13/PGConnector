//Connection status
#define PG_CONNECTION_OK 					0
#define PG_CONNECTION_BAD 					1
#define PG_CONNECTION_STARTED 					2
#define PG_CONNECTION_MADE 					3
#define PG_CONNECTION_AWAITING_RESPONSE 			4
#define PG_CONNECTION_AUTH_OK 					5
#define PG_CONNECTION_SETENV 					6
#define PG_CONNECTION_SSL_STARTUP 				7
#define PG_CONNECTION_NEEDED 					8

//Response Status
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

//Internal Errors Plugins
#define PGCON_INVALID_CONNECTION				1000
#define PGRES_INVALIDR_RESULT					1001
#define PGRES_INVALID_CONVERTION				1003
#define PGRES_CONVERTION_OUTOFRANGE				1004
#define PGCON_INVALID_CHARSET					1005
#define PGCON_CONNECTION_INDEFINEDERROR				1006

//NULL
#define PG_NULL							1
								
//Natives responsible for handling connections
native PG_conectar(host[], port[], user[], password[], database[]);
native PG_encerrarConexao(connectionId);
native PG_status(connectionId);
native PG_ultimoErro(connectionId, errorStr[], sizeofErrorStr);
native PG_setClientEncode(connectionId, encode[] = "WIN1252");
native PG_escapeString(connectionId, sourceStr[], destinyStr[], sizeofDestinyStr);

//Natives responsible for handling responses
native PG_query(connectionId, query[]);
native PG_assyncQuery(connectionId, query[], callbackFunction[]);
native PG_statusResposta(responseId);
native PG_erroResposta(responseId, strError[], sizeofStrError);
native PG_liberarResposta(responseId);
native PG_quantidadeColunas(responseId);
native PG_quantidadeLinhas(responseId);
native PG_recuperarInt(responseId, row, column);
native Float:PG_recuperarFloat(responseId, row, column);
native PG_recuperarStr(responseId, row, column, strValue[], sizeofStrValue);
native PG_verificarNulo(responseId, row, column);

//English natives
#define PG_connect(%0,%1,%2,%3,%4)	PG_conectar(%0,%1,%2,%3,%4)
#define PG_close(%0) 			PG_encerrarConexao(%0)
#define PG_lastError(%0,%1,%2)		PG_ultimoErro(%0,%1,%2)
#define PG_responseStatus(%0) 		PG_statusResposta(%0) 
#define PG_responseError(%0,%1,%2)  	PG_erroResposta(%0,%1,%2) 
#define PG_freeResponse(%0) 		PG_liberarResposta(%0)
#define PG_numRows(%0) 			PG_quantidadeLinhas(%0)
#define PG_numFields(%0) 		PG_quantidadeColunas(%0)
#define PG_getInt(%0,%1,%2) 		PG_recuperarInt(%0,%1,%2)
#define PG_getFloat(%0,%1,%2) 		PG_recuperarFloat(%0,%1,%2)
#define PG_getStr(%0,%1,%2,%3,%4) 	PG_recuperarStr(%0,%1,%2,%3,%4)
#define PG_isNull(%0,%1,%2) 		PG_verificarNulo(%0,%1,%2)


//Forwards
forward OnPostgreSQLError(errorCode, error[]);