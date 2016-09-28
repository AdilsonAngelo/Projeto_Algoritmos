#ifndef DICIONARIO_H
#define DICIONARIO_H

#include <iostream>
#include <iomanip>
#include <list>
#include <string>

/** PROJETO IF672 - 2016.1
 *	CENTRO DE INFORMATICA - UFPE
 *	ALGORITMOS E ESTRUTURA DE DADOS
 *	ALUNO: ADILSON ANGELO DA SILVA JUNIOR
 *	LOGIN: aasj2
 *	MATRICULA: 057.126.414-02
 *	PROFESSOR: RENATO VIMIEIRO
 */

using namespace std;

/** A IMPLEMENTACAO DO DICIONARIO FOI FEITA ATRAVES DE UMA TRIE
 *	CADA NO' DA TRIE CONTEM:
 *	- O item (char) QUEM REPRESENTA A LETRA DE UMA PALAVRA
 *	- UM double score, QUE ARMAZENA O ESCORE TOTAL DA PALAVRA
 *	- UM int frequencia, QUE ARMAZENA A QUANTIDADE DE VEZES QUE
 *	A PALAVRA FOI INSERIDA NA TRIE
 *	- UM PONTEIRO PARA O NO' PAI
 *	- E UMA LISTA DE PONTEIROS PARA OS NO'S FILHOS
 *	OBS.: OS VALORES DE ESCORE E FREQUENCIA SO SAO ARMAZENADOS
 *	NO NO' FINAL DE UMA PALAVRA, TODOS OS OUTROS NO'S TEM ESSES
 *	VALORES COMO "NULL" NO MOMENTO DE SUA INSERCAO
 *
 *	A CLASSE DICIONARIO TEM COMO ATRIBUTOS PRIVADOS:
 *	- UM NO' raiz
 *
 *	FUNCOES DA TRIE (Dicionario):
 *
 *	- CONSTRUTOR (Dicionario()):
 *		Inicializa o dicionario com o no' raiz com todos seus atributos "NULL"
 *		e a lista de filhos vazia;
 *
 *	- Nodo* buscarProx(lista<Nodo*> *lista, char c):
 *		Funcao privada que sera utilizada pelas outras funcoes. Recebe como parametro
 *		um ponteiro para uma lista de filhos de um no' qualquer e um char. Retorna um
 *		ponteiro para o no' filho se o char eh um dos filhos da lista e NULL caso contrario;
 *
 *	- int buscar_frequencia(string palavra):
 *		Recebe uma palavra como parametro e faz um loop para cada letra da palavra na TRIE
 *		ate chegar na ultima e retornar sua frequencia. Caso a palavra nao esteja na TRIE
 *		retorna -1;
 *
 *	- double buscar_score(string palavra):
 *		Mesma logica de buscar_frequencia(), retorna escore medio caso palavra se encontre
 *		na TRIE e -1 caso contrario;
 *
 *	- void inserir(string palavra, double escore):
 *		Recebe a string com a palavra a ser inserida e o double escore. Checa se
 *		os no's correspondentes as letras da palavra se encontram na TRIE, e entao
 *		se nao tiver no' para a letra, cria-se um novo no, armazena a letra, a frequencia
 *		e o score sao setados para NULL e o ponteiro para o pai eh atribuido a letra anterior.
 *		Caso seja a ultima letra da palavra que esteja sendo adicionada a TRIE, a frequencia eh
 *		setada para 1 (pois é uma palavra nova) e o score para o parametro recebido; Caso ja haja
 *		o no' correspondente a palavra na TRIE e for a ultima letra da palavra, a frequencia eh
 *		incrementada e o score é somado para um score total. Nesse caso o score medio eh dado
 *		pelo score total (que eh o valor armazenado no no') e a frequencia;
 *
 *	- bool remover(string palavra):
 *		Nao remove os no's da TRIE, apenas seta os valores de frequencia e score para NULL;
 *
 *	- void maiorFrequencia():
 *		Baseado no algoritmo de busca em largura: Eh criada uma fila na qual eh adicionada a raiz.
 *		Para cada no' filho do primeiro elemento da fila eh adicionado tambem a fila e eh verificado
 *		quem tem maior frequencia e armazenado num ponteiro; Eh entao impresso na tela a palavra e seus
 *		respectivos dados
 *
 *	- void menorScore() & void maiorScore():
 *		Mesma logica de maiorFrequencia();
 *
 */

struct Nodo
{
	char item;
	int frequencia;
	double score;
	Nodo *pai;
	list<Nodo*> filhos;
};

class Dicionario
{
	public:
		Dicionario()
		{
			this->raiz = {NULL, NULL, NULL, NULL,};
			this->raiz.filhos.clear();
		}
		~Dicionario() {}

		int buscar_frequencia(string palavra)
		{
			Nodo *aux = &this->raiz;
			for(int i = 0; i < palavra.size(); i++)
			{
				char c = palavra[i];
				aux = buscarProx(&aux->filhos, c);

				if(aux == NULL) break;

				if(i == palavra.size() - 1 && aux->frequencia != NULL) return aux->frequencia;
			}
			return -1;
		}

		double buscar_score(string palavra)
		{
			Nodo *aux = &this->raiz;
			for(int i = 0; i < palavra.size(); i++)
			{
				char c = palavra[i];
				aux = buscarProx(&aux->filhos, c);

				if(aux == NULL) break;

				if(i == palavra.size() - 1 && aux->frequencia != NULL)	return (aux->score)/(aux->frequencia);
			}
			return -1;
		}

		void inserir(string palavra, double escore)
		{
			Nodo* atual = &this->raiz;
			Nodo *prox;
			for(int i = 0; i < palavra.size(); i++)
			{
				char c = palavra[i];

				prox = buscarProx(&atual->filhos, c);

				if(prox == NULL)
				{
					prox = new Nodo();
					prox->item = palavra[i];
					prox->frequencia = prox->score = NULL;

					prox->pai = atual;

					if(i == palavra.size()-1)
					{
						prox->frequencia = 1;
						prox->score = escore;
					}

					atual->filhos.push_back(prox);
				}
				else
				{
					if(i == palavra.size() - 1)
					{
						prox->score += escore;
						prox->frequencia++;
					}
				}
				atual = prox;
			}

		}

		bool remover(string palavra)
		{
			Nodo *aux = &this->raiz;
			for(int i = 0; i < palavra.size(); i++)
			{
				char c = palavra[i];
				aux = buscarProx(&aux->filhos, c);

				if(aux == NULL) break;

				if(i == palavra.size() - 1)
				{
					aux->score = NULL;
					aux->frequencia = NULL;
					return true;
				}
			}
			return false;
		}

		void maiorFrequencia()
		{
			string palavra = "";
			Nodo *res;

			int FREQ_MAX = -1;

			list<Nodo*> vertices;

			vertices.push_back(&this->raiz);

			list<Nodo*>::iterator i;
			for(i = vertices.begin(); !vertices.empty(); ++i)
			{
				Nodo *I = vertices.front();
				vertices.pop_front();

				list<Nodo*>::iterator j;
				for(j = I->filhos.begin(); j != I->filhos.end(); ++j)
				{
					Nodo *J = (*j);
					if(J->frequencia != NULL && J->frequencia > FREQ_MAX)
					{
						res = J;
						FREQ_MAX = J->frequencia;
					}
					vertices.push_back(J);
				}
				i = vertices.begin();
			}


			Nodo *aux;
			aux = res;
			while(aux->item != NULL)
			{
				palavra = aux->item + palavra;
				aux = aux->pai;
			}

			cout << "PALAVRA DE MAIOR FREQUENCIA: " << palavra << "		||		Score: " << setprecision(2) << fixed << res->score/res->frequencia << "	||	Frequencia: " << res->frequencia << endl;
		}

		void menorScore()
		{
			string palavra = "";
			Nodo *res;

			double SCORE_MIN = 999999999999999;

			list<Nodo*> vertices;

			vertices.push_back(&this->raiz);

			list<Nodo*>::iterator i;
			for(i = vertices.begin(); !vertices.empty(); ++i)
			{
				Nodo *I = vertices.front();
				vertices.pop_front();

				list<Nodo*>::iterator j;
				for(j = I->filhos.begin(); j != I->filhos.end(); ++j)
				{
					Nodo *J = (*j);
					if(J->score != NULL && (J->score/J->frequencia) < SCORE_MIN)
					{
						res = J;
						SCORE_MIN = (J->score/J->frequencia);
					}
					vertices.push_back(J);
				}
				i = vertices.begin();
			}


			Nodo *aux;
			aux = res;
			while(aux->item != NULL)
			{
				palavra = aux->item + palavra;
				aux = aux->pai;
			}

			cout << "PALAVRA DE MENOR SCORE MEDIO: " << palavra << "		||		Score: " << setprecision(2) << fixed << SCORE_MIN << "	||	Frequencia: " << res->frequencia <<endl;
		}

		void maiorScore()
		{
			string palavra = "";
			Nodo *res;

			double SCORE_MAX = -1;

			list<Nodo*> vertices;

			vertices.push_back(&this->raiz);

			list<Nodo*>::iterator i;
			for(i = vertices.begin(); !vertices.empty(); ++i)
			{
				Nodo *I = vertices.front();
				vertices.pop_front();

				list<Nodo*>::iterator j;
				for(j = I->filhos.begin(); j != I->filhos.end(); ++j)
				{
					Nodo *J = (*j);
					if(J->score != NULL && (J->score/J->frequencia) > SCORE_MAX)
					{
						res = J;
						SCORE_MAX = (J->score/J->frequencia);
					}
					vertices.push_back(J);
				}
				i = vertices.begin();
			}


			Nodo *aux;
			aux = res;
			while(aux->item != NULL)
			{
				palavra = aux->item + palavra;
				aux = aux->pai;
			}

			cout << "PALAVRA DE MAIOR SCORE MEDIO: " << palavra << "		||		Score: " << setprecision(2) << SCORE_MAX << "	||	Frequencia: " << res->frequencia << endl;
		}

	protected:

	private:
		Nodo raiz;

		Nodo* buscarProx(list<Nodo*> *lista, char c)
		{
			list<Nodo*>::iterator i;
			for(i = lista->begin(); i != lista->end(); ++i)
			{
				Nodo *aux = (*i);
				if(c == aux->item) return aux;
			}
			return NULL;
		}
};

#endif // DICIONARIO_H

