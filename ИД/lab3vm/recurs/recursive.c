int prim()
{
	int s;
	if (match("(")) {
		if (add()==0) return 0;
		if (!match(")")) { cerror("mismatch ')'"); return 0; }
	}
	else if (number(&s)) {
		printf("%d ", s);
	} else { cerror("expression syntax"); return 0; }
	return 1;
}

int mult()
{
	if (prim()==0) return 0;
	while (1) {
		if (match("*")) {
			if (prim()==0) return 0;
			printf("* ");
		} else if (match("/")) {
			if (prim()==0) return 0;
			printf("/ ");
		} else break;
	}
	return 1;
}

int add()
{
	if (mult()==0) return 0;
	while (1) {
    		if (match("+")) {
	    		if (mult()==0) return 0;
			printf("+ ");
    		} else if (match("-")) {
	    		if (mult()==0) return 0;
			printf("- ");
		} else break;
	}
	return 1;
}

