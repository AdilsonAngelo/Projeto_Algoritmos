#include <iostream>
#include <fstream>
#include "Dicionario.h"
#include <string>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>

/**	PROJETO IF672 - 2016.1
 *	CENTRO DE INFORMATICA - UFPE
 *	ALGORITMOS E ESTRUTURA DE DADOS
 *	ALUNO: ADILSON ANGELO DA SILVA JUNIOR
 *	LOGIN: aasj2
 *	MATRICULA: 057.126.414-02
 *	PROFESSOR: RENATO VIMIEIRO
 *	
 *	Este programa foi compilado e executado nas ultimas versoes do Ubuntu-64bit
 *	(gcc 5.2.1) e Kali Linux-64bit (gcc 5.3.1), ambos da seguinte forma:
 *	g++ main.cpp -std=c++11 -o projeto
 */

using namespace std;

/** Funcao clear_up:
 *	- recebe uma string (token) como parametro
 *	- limpa o token deixando na string apenas os valores de char
 *	dentro do range de letras minusculas da tabela ASCII
 *	e transformando as maiusculas em minusculas
 *	- retorna a string "limpa"
 */

string clear_up(string token)
{
	string res = "";
	for(int i = 0; i < token.size(); i++)
	{
		if(token[i] >= 97 && token[i] <= 122) res += token[i];
		else if(token[i] >= 65 && token[i] <= 90) res += (token[i]+32);
	}
	return res;
}

/**	Funcao split_on_separators:
 *	- recebe uma string (linha) como parametro
 *	- para cada char da linha concatena numa string temporaria
 *	ate o proximo [SPACE] (valor 32 na tabela ASCII) para formar um token
 *	- entao adiciona o token a uma lista de strings
 *	- eh feita uma iteracao para chamar a funcao clear_up em cada
 *	token da lista
 *	- remove todos as strings vazias da lista
 *	- retorna a lista
 */

list<string> split_on_separators(string linha)
{
	list<string> res;
	res.clear();
	string temp = "";

	for(int i = 0; i < linha.size(); i++)
	{
		if(linha[i] != 32) temp += linha[i];
		else
		{
			res.push_back(temp);
			temp.clear();
		}
	}

	list<string>::iterator i;
	for(i = res.begin(); i != res.end(); ++i) *i = clear_up(*i);

	res.remove("");

	return res;
}

int main()
{
	rusage ram;
	long ram_final, ram_inicial;
	getrusage(RUSAGE_SELF, &ram);
	ram_inicial = ram.ru_maxrss;
    /**	Variaveis "clock_t" para contabilizar o tempo global e de cada etapa
	*/

	clock_t global_1, global_2, leitura_1, leitura_2, predicao_1, predicao_2;

	global_1 = clock();

	double erro_global = 0;

	Dicionario D;

	ifstream train, test;

	train.open("trainSet.txt");

	int score;

	string linha;

	leitura_1 = clock();
	while(getline(train, linha))
	{
        /**	Para todas as linhas do arquivo de entrada
		 *	armazena o score do comentario na variavel (int)score
		 *	e executa a funcao split_on_separators para armazenar todas
		 *	as palavras do comentario numa variavel (lista<string>) tokens;
         */

		score = linha.front() - 48;

		list<string> tokens = split_on_separators(linha);

		/**	Abaixo alguns tokens foram removidos por
		 *	nao serem exatamente palavras, o que atrapalhava
		 *	o resultado final de menor score
		 */

		tokens.remove("g");
		tokens.remove("di");
		tokens.remove("dw");
		tokens.remove("co");
		tokens.remove("aaa");
//		tokens.remove("won");
/**		tokens.remove("woe");
*		tokens.remove("ong");
*		tokens.remove("orc");
*/
        /** Eh feita uma iteracao na lista tokens de palavras para
         *	inseri-las no dicionario com seus respectivos scores
         */

		list<string>::iterator i;
		for(i = tokens.begin(); i != tokens.end(); ++i)
			D.inserir(*i, score);
	}
	leitura_2 = clock();
	train.close();

	predicao_1 = clock();
	test.open("testSet.txt");

	int score2;

	double inferencia = 0;

	string linha2;

	while(getline(test, linha2))
	{
		/**	Eh feito o mesmo procedimento inicial para pegar as linhas
		 *	e scores de cada comentario do arquivo de teste
		 */

		score2 = linha2.front() - 48;

		list<string> tokens = split_on_separators(linha2);

		double scores[tokens.size()];

        /**	Mas agora eh armazenado num vetor de doubles
         *	os scores de cada palavra (score medio de acordo com arquivo de treinamento)
         *	dos novos comentarios de teste com a funcao Dicionario::buscar_score()
         *	que retorna o score medio da palavra
         */

		list<string>::iterator i;
		int j = 0;
		for(i = tokens.begin(); i != tokens.end(); ++i)
		{
			scores[j] = D.buscar_score(*i);
			if(scores[j] < 0)
				scores[j] = 2;
			j++;
		}

        /** A inferencia do comentario é feita pela media dos scores medios de
         *	cada palavra. O erro é a diferenca entre o score real e a inferencia.
         *	Eh entao somado ao erro global o quadrado do erro
         */

		for(j = 0; j < tokens.size(); j++)
			inferencia += scores[j];

		if(tokens.size() > 0) inferencia = inferencia / tokens.size();

		double erro;

		erro = (inferencia - score2);

		if(score2 >= 0 && score2 <=4) erro_global += pow(erro, 2);

		inferencia = 0;
	}
	test.close();
	predicao_2 = clock();

    /** Sao exibidos os resultados finais:
     */

	D.maiorFrequencia();

	D.maiorScore();

	D.menorScore();

	global_2 = clock();

	getrusage(RUSAGE_SELF, &ram);
	ram_final = ram.ru_maxrss;

	cout << "\nERRO GLOBAL (soma dos quadrados dos erros): " << setprecision(2) << fixed << erro_global << endl;

	float tempo_global (((float)global_2 - (float)global_1)/ CLOCKS_PER_SEC);
	float tempo_leitura (((float)leitura_2 - (float)leitura_1)/ CLOCKS_PER_SEC);
	float tempo_predicao (((float)predicao_2 - (float)predicao_1)/ CLOCKS_PER_SEC);

	cout << "\nUSO DE RAM: " << (ram_final - ram_inicial) << " Kb" << endl;
    cout << "\nTEMPO GLOBAL: " << tempo_global << " segundos" << endl;
    cout << "\nTEMPO DE LEITURA: " << tempo_leitura << " segundos" << endl;
    cout << "\nTEMPO DE PREDICAO: " << tempo_predicao << " segundos" << endl;
    return 0;
}

