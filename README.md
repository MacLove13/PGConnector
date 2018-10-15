# PGConnector
Utilizando este plugin você será capaz de manipular o SGBD PostgreSQL e desenvolver aplicações utilizando este SGBD. 
Segue abaixo a documentação da mesma:

### Instalação
Windows:

1. Primeiramente é necessário copiar a plugin PGConnector.dll para a pasta "plugins" do seu servidor, caso ela não exista, crie a mesma.

2. Após isso abra o arquivo server.cfg e edite a linha "plugins" adicionando a DLL PGConnector.dll na mesma. Caso a linha não exista crie a mesma no final do arquivo e adicione a DLL.

3. Copie a DLL libpq.dll e cole a mesma na mesma pasta onde encontra-se o executavel "samp-server.exe". Essa DLL é nativa do PostgreSQL e contém as funções necessárias para a plugin PGConnector.dll funcione corretamente.

4. Adicione a include "PGConnector.inc" na sua pasta de includes do Pawno.

5. Após estes passos a plugin já deve iniciar e funcionar corretamente. Caso ocorra algum erro, tente executar o "samp-server.exe" como administrador. Caso o erro persista envie um e-mail para mim em hugo.busnardo@gmail.com.

Linux:

1. Copie o arquivo "PGConnector.so" para a pasta plugins do seu servidor SA-MP.

2. Após isso abra o arquivo server.cfg e edite a linha "plugins" adicionando a DLL PGConnector.so na mesma. Caso a linha não exista crie a mesma no final do arquivo e adicione a plugin "PGConnector.so".

3. Adicione a include "PGConnector.inc" na sua pasta de includes do Pawno.

4. Após estes passos a plugin já deve iniciar e funcionar corretamente. Caso ocorra algum erro, tente executar o "samp-server.exe" como administrador. Caso o erro persista envie um e-mail para mim em hugo.busnardo@gmail.com.

### Documentação
A Plugin em sua versão atual conta com os comandos básicos para manipulação do SGBD PostgreSQL.

## Forwards
- *forward OnPostgreSQLError(errorCode, erro[]);*

Essa forward é chamada sempre que houver um erro. O parâmetro "errorCode" conterá o código do erro que ocorreu e o parâmetro "erro[]" conterá a descrição detalhada do erro.

## Nativas

- *native PG_conectar(host[], porta[], usuario[], senha[], database[]);*
Inicia a conexão com o PostgreSQL. 

Exemplo de uso:

`new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "", "serversamp");`

Em caso de sucesso a função retorna um inteiro referenciando o ID da conexão. Em caso de falha será retornado o inteiro -1.

- *native PG_encerrarConexao(idConexao);*

Encerra uma conexão aberta com o PostgreSQL
Exemplo de uso:
```
    new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "123456", "serversamp");
    PG_encerrarConexao(conexao);
```
Em caso de sucesso a função irá retornar o inteiro 1. Caso seja passado um ID de conexão inexistente será retornado o inteiro 0.

- *native PG_status(idConexao);*

Retorna o código do status da conexão passada pelo parâmetro "idConexao".

Exemplo de uso:
```
  new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "", "serversamp");
  if(PG_status(conexao) == PG_CONNECTION_OK){
    PG_encerrarConexao(conexao);
  } else {
    new erro[70];
    PG_ultimoErro(conexao, erro, sizeof(erro));
    printf("Erro ao conectar-se: %s", erro);
  }
```
A função retornará o código do status da conexão (inteiro). Caso a conexão não exista a função retornara o status `PGCON_CONEXAO_INEXISTENTE` (inteiro 1000).Caso a conexão tenha sido realizada com sucesso a função retornará `PG_CONNECTION_OK`, em caso de erro será retornado `PG_CONNECTION_BAD`. Para recuperar mais informações sobre o erro, utilize `PG_ultimoErro`.

- *native PG_ultimoErro(idConexao, strErro[], sizeofStrErro);*

Seta o valor da string "strErro[]" com a mensagem do último erro ocorrido na conexão passada por "idConexao".

Exemplo de uso:

```
  new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "", "serversamp");
  if(PG_status(conexao) == PG_CONNECTION_OK){
    PG_encerrarConexao(conexao);
  } else {
    new erro[70];
    PG_ultimoErro(conexao, erro, sizeof(erro));
    printf("Erro ao conectar-se: %s", erro);
  }
```
Em caso de sucesso a função retornará o inteiro 1. Em caso de falha será retornado o inteiro -1 e será chamada a forward OnPostgreSQLError (caso definida).

- *native PG_setClientEncode(idConexao[, encode[] = "WIN1252"]);*
Seta o encode da conexão "idConexao" para o encode passado por "encode[]". Caso "encode[]" não tenha sido definido o valor padrão será "WIN1252".

Exemplo de uso:

```
  new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "", "serversamp");
  if(PG_status(conexao) == PG_CONNECTION_OK){
    PG_setClientEncode(conexao, "UTF8");
  }
```
Caso a função seja executada com sucesso o inteiro 0 será retornado. Em caso de falha (possívelmente um charset inválido), será retornado o inteiro -1 e a callback OnPostgreSQLError será chamada.

- *native PG_query(idConexao, query[]);*

Executa um comando SQL passado por "query[]" utilizando a conexão passada por "idConexao".

Exemplo de uso:

```
  new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "", "serversamp");
  if(PG_status(conexao) != PG_CONNECTION_OK){
    new erro[100];
    PG_ultimoErro(conexao, erro, sizeof(erro)); 
    printf("Erro ao estabelecer conexão: %s", erro);
    return 0;
  }
  new resultado = PG_query(conexao, "SELECT usuario.id, usuario.nomeusuario FROM usuario");
  PG_encerrarConexao(conexao);
  printf("Quantidade de linhas: %d", PG_quantidadeLinhas(resultado));
  PG_liberarResultado(resultado);
  return 1;
```
Em caso de sucesso a função retornara um inteiro referenciando o ID da resposta. Em caso de erro o inteiro -1 será retornado e a forward OnPostgreSQLError será chamada.

- *native PG_assyncQuery(idConexao, query[], callbackFunction[]);*

Executa um comando SQL passado por "query[]" utilizando a conexao "idConexao". A função é executada de forma assíncrona, ou seja, o servidor não irá travar e aguardar a resposta da query. A query será enviada para o SGBD processa-la e a execução do servidor continuará normalmente sem a resposta do mesmo. Ao finalizar o processamento da query a forward passada em "callbackFunction[]" será chamada. Ao ser chamada a callback deverá receber 2 parâmetros, o primeiro será o ID da conexão e o segundo o ID da resposta.

Exemplo de uso:

```
forward exibirResposta(conexao, resultado);
public exibirResposta(conexao, resposta) {
  PG_encerrarConexao(conexao);
  if(PG_statusResposta(resposta) != PGRES_TUPLES_OK){
    new erro[100];
    PG_erroResposta(resposta, erro, sizeof(erro));
    printf("Erro ao realizar consulta: %s", erro);
    return 0;
  }
  new qtdLinhas = PG_quantidadeLinhas(resultado);
  for(new i = 0; i < qtdLinhas; ++i){
    new nome[24];
    PG_recuperarStr(resultado, i, 1, nome, sizeof(nome));
    printf("ID: %d\nNome: %s\n\n", PG_recuperarInt(resultado, i, 0), nome);
  }
  PG_liberarResposta(resposta);
  return 1;
}

public OnGameModeInit(){
  new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "", "serversamp");
  if(PG_status(conexao) != PG_CONNECTION_OK){
    new erro[100];
    PG_ultimoErro(conexao, erro, sizeof(erro)); 
    printf("Erro ao estabelecer conexão: %s", erro);
    return 0;
  }
  
  PG_assyncQuery(conexao, "SELECT usuario.id, usuario.nome FROM usuario", "exibirResposta");
  print("Eu executei e não precisei esperar pela resposta da query!!!!");
  return 1;
}
```
Em caso de sucesso a função retornará o inteiro 1. Em caso de erro a função executará a forward OnPostgreSQLError.

- *native PG_statusResposta(idResposta);*

A função retorna o código do status da resposta passada por "idResposta".

Exemplo de uso pode ser encontrado no exemplo anterior em "PG_assyncQuery".

Caso a função seja executada com sucesso o código do Status da resposta será retornado (uma lista com todos os status possivelmente retornados por ser encontrado [aqui](https://www.postgresql.org/docs/9.6/static/libpq-exec.html) na categoria da função "PQresultStatus"). Caso o ID do resultado passado seja inválido a função retornara `PGRES_RESULTADO_INEXISTENTE`.

- *native PG_erroResposta(idResposta, strErro[], sizeofStrErro);*

A função seta o valor de "strErro[]" para o erro retornado do SGBD para a resposta "idResposta".

O exemplo de uso desta função pode ser encontrado no exemplo anterior em "PG_assyncQuery".

Caso a função encontre um erro na resposta será retornado o inteiro 1. Caso contrario o inteiro 0 será retornado e a string "strErro[]" receberá um valor nulo.

- *native PG_liberarResposta(idResposta);*

Tem como objetivo limpar os dados recebidos em uma resposta da memória. É fortemente recomendavel que essa função seja executada sempre que não for mais utilizar os dados de uma resposta.

O exemplo de uso desta função pode ser encontrado no exemplo anterior em "PG_assyncQuery".

Em caso de sucesso a função retornará o inteiro 1. Em caso de falha a função retornará `PGRES_RESULTADO_INEXISTENTE`.

- *native PG_quantidadeColunas(idResultado);*

Essa função retorna a quantidade de colunas retornadas em um comando SQL.

```
  new conexao = PG_conectar("127.0.0.1", "5432", "postgres", "", "sampserver");
  if(PG_status(conexao) == PG_CONNECTION_OK){
    new resultado = PG_query(conexao, "SELECT usuario.id, usuario.nomeusuario, usuario.nome");
    PG_encerrarConexao(conexao);
    printf("Quantidade de Linhas: %s\nQuantidade de Colunas: ", PG_quantidadeLinhas(resultado), PG_quantidadeColunas(resultado));
    PG_liberarResultado(resultado);
  }
```

Em caso de sucesso a função retornara um inteiro informando a quantidade de colunas. Caso o "idResultado" seja inválido o inteiro -1 será retornado.

- *native PG_quantidadeLinhas(idResultado);*

Essa função retorna a quantidade de linhas retornadas em um comando SQL.

O exemplo de uso dessa função pode ser encontrado no exemplo anterior em "PG_quantidadeColunas".

Em caso de sucesso a função retornará um inteiro contendo a quantidade de linhas no resultado. Caso o "idResultado" seja inválido o inteiro -1 será retornado.

- *native PG_recuperarInt(idResultado, linha, coluna);*
Esta função extrai o valor de uma celula retornada em idResultado. A especificação da célula é passada em "linha" e "coluna". Essa função deve ser utilizada apenas para recuperar valores do tipo numérico irracionais (ou seja, sem ponto flutuante).

O exemplo de uso dessa função pode ser encontrado no exemplo anterior "PG_assyncQuery".

Caso a função seja executada corretamente um inteiro representando o valor da linha "linha" e da coluna "coluna" será retornado. Em caso de erro a função retornará o inteiro -1 e executara a forward OnPostgreSQLError (se definida).

- *native Float:PG_recuperarFloat(idResultado, linha, coluna);*

Esta função extrai o valor de uma célula retornada em "idResultado". A especificação da célula é passada em "linha" e "coluna". Essa função deverá ser utilizada para recuperar apenas valores do tipo numérico real (ou seja, qualquer numero, não podendo ser um valor do tipo string).

O exemplo de uso dessa função pode ser encontrado no exemplo anterior em "PG_assyncQuery".

Caso a função seja executada corretamente um valor do tipo Float será retornado. Em caso de falha o valor do tipo Float -1.0 será retornado e a forward OnPostgreSQLError será executada (se definida).

- *native PG_recuperarStr(idResultado, linha, coluna, strValor[], sizeofStrValor);*
Está função extrai o valor de uma célula retornada em "idResultado". A especificação da célula é passada em "linha" e "coluna". O valor de "strValor[]" será setado para o valor contido na célula. Essa função pode ser utilizada para recuperar qualquer tipo de valor do resultado.

O exemplo de uso dessa função pode ser encontrado no exemplo anterior "PG_assyncQuery".

Em caso de sucesso uma string será retornada para a variável "strValor[]". Caso o valor na célula seja um valor do tipo NULL uma string vazia será retornada para "strValor[]".

