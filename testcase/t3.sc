// global array in argv
fA(){
	array aa[10] = 10;
	fB(aa,@gg);
}
fB(bb,ff){
	puti(bb[4]);
	puts(ff[4]);
}

array gg[10] = "gg";
fA();
