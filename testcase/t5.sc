// & pass by reference, * pointer 
main(){
	a = 99;
	puti(a);
	p = &a;
	fA(p);
	puti(*p);
}

fA(b){
	*b = 10;
}