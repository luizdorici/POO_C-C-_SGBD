1.Explicações sobre as funções em geral encontram-se nos headers...

2.Alguns Headers nao foram separados em .h e .cc para evitar erros menos importantes no momento...

3.Pasta system sera o database inicial onde ficara a tabela com o registro dos usuarios e permições para acesso e uso do database
a autenticaçao do login sera feito com base nessa tabela caso existam varios usuarios...

4.Classe SGBD e Classe User ainda nao foram implementadas, apenas existem para futura criaçao de uma conexao do banco de dados em rede...

5.Sobre o calculo do byteoffset, foi feito da forma como dito no trabalho somando apenas 1 byte para int e double
apesar de que na realidade esses valores nao dariam o offset real para busca no arquivo de registro entao pode-se
futuramente calcular a diferença e descobrir o real offset de cada registro(o calculo foi feito dessa forma apenas para seguir o PDF)...
