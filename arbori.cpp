#include<fstream>
#include<string>

using namespace std;

ifstream fin("exptree.in");
ofstream fout("exptree.out");


//coada care pastreaza un numar sau un operator (pentru crearea notatiei postfixate)

struct treenod {
	char kind;
	char op;
	int nmb;
	treenod* left, * right;
};

struct nod {
	treenod* val;
	nod* next;
};

nod* tree = NULL;
void pushnmb(int nr) {
	nod* r = new nod;
	treenod* q = new treenod;
	q->kind = 'N';
	q->op = '?';
	q->nmb = nr;
	q->left = NULL;
	q->right = NULL;

	r->val = q;
	r->next = tree;
	tree = r;
}


treenod* treepop() {
	nod* r = tree;
	tree = tree->next;
	treenod* q = r->val;
	delete r;
	return q;
}


void pushop(char e) {
	nod* r = new nod;
	treenod* q = new treenod;
	q->kind = 'O';
	q->op = e;
	q->nmb = 0;
	q->right = treepop();
	q->left = treepop();
	r->val = q;
	r->next = tree;
	tree = r;
}


//stiva pentru operanzi
class stiva {
private:
	int top;
	char op[100];
public:
	void settop(int k) {
		top = k;
	}
	bool empty() {
		return top == 0;
	}
	char TOP() {
		return op[top];
	}
	void push(char e) {
		top++;
		op[top] = e;
	}
	void pop() {
		top--;
	}
};
stiva stiv;
// verifica daca e cifra un caracter dat 
bool isdgt(char e) {
	return e >= '0' && e <= '9';
}

//verifica daca e operator un caracter dat
bool isoprt(char e) {
	return e == '+' || e == '-' || e == '*' || e == '/';
}

//returneaza prioritatea operatiilor
int prior(char e) {
	if (e == '*' || e == '/') return 2;
	if (e == '-' || e == '+') return 1;
	return 0;
}


void postordine(treenod*& arb) {
	if (arb != NULL) {
		postordine(arb->left);
		postordine(arb->right);
		if (arb->kind == 'O') {
			fout << arb->op << ' ';
		}
		else fout << arb->nmb << ' ';
	}
}

void preordine(treenod*& arb) {
	if (arb != NULL) {
		if (arb->kind == 'O') {
			fout << arb->op << ' ';
		}
		else fout << arb->nmb << ' ';
		preordine(arb->left);
		preordine(arb->right);
	}
}


int evalueaza(char e, int a, int b) {
	switch (e) {
	case '*': return a * b;
	case '/': return a / b;
	case '+': return a + b;
	case '-': return a - b;
	}
}

int calculeaza(treenod*& arb) {
	if (arb->kind == 'N') return arb->nmb;
	else {
		return evalueaza(arb->op, calculeaza(arb->left), calculeaza(arb->right));
	}
}

int main() {
	char in[104];
	stiv.settop(0);
	fin.get(in, 100);  //citesc expresia infix

	int n = strlen(in);
	for (char* i = in; i < in + n; i++) {
		if (isdgt(*i)) {
			int nr = 0;
			while (i < in + n && isdgt(*i)) {
				nr *= 10;
				nr += (*i) - '0';
				i++;
			}
			pushnmb(nr);
			i--;
		}
		else if (isoprt(*i)) {
			if (*i == '-') {
				char* f = i;
				while (f > in && (*(f - 1) == ' ' || *(f - 1) == '\t')) f--;
				if (f == in || *(f - 1) == '(') pushnmb(0);
			}
			if (stiv.empty()) {
				stiv.push(*i);
			}
			else {
				while (!stiv.empty() && prior(*i) <= prior(stiv.TOP())) {
					pushop(stiv.TOP());
					stiv.pop();
				}
				stiv.push(*i);
			}

		}
		else if (*i == '(') {
			stiv.push('(');
		}
		else if (*i == ')') {
			while (stiv.TOP() != '(') {
				pushop(stiv.TOP());
				stiv.pop();
			}
			stiv.pop();
		}

	}
	while (!stiv.empty()) {
		pushop(stiv.TOP());
		stiv.pop();
	}

	treenod* arbore = tree->val;
	postordine(arbore);
	fout << '\n';
	preordine(arbore);
	fout << '\n';
	fout << calculeaza(arbore);
	return 0;
}