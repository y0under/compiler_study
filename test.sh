#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./bin/main "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

#assert 1 "int main(){int a; a = 5; int *b = &b; return *b - 4;}"
assert 7 "int main(){int a = 0; int b = 7; return b;}"
assert 0 "int main(){return 0;}"
assert 2 "int main(){return 2;}"
assert 5 "int main(){int a = 2; int b = 3; return a + b;}"
assert 3 "int main(int a, int b){return 3;}"
assert 5 "int main(int a, int b){a = 2; b = 3; return a+b;}"
assert 3 "int main(int a, int b){return 3;}"
assert 7 "int main(){return hoge();} int hoge(){return 7;}"
assert 4 "int main(){return hoge(4);} int hoge(int a){return a;}"
assert 3 "int main(){return huga(1, 2);} int huga(int a, int b){return a + b;}"
assert 6 "int main(){return hoge(1, 2, 3);} int hoge(int a, int b, int c){return a + b + c;}"
assert 4 "int main(){return hoge(1, 2, 3);} int hoge(int a, int b, int c){return a + c;}"
assert 1 "int main(){int a; int b; return hoge(1, 2, 3);} int hoge(int a, int b, int c){return a;}"
assert 55 "int main(){return sum(10);} int sum(int n){if(n<0) return 0; return n + sum(n-1);}"
assert 3 "int main(){int x = 3; int y = 5; int z = &y + 8; return *z;}"
#assert 3 "hoge(){return 3;} main(){return hoge();}"
#assert 27 a=20+5-4\;b=2*3\;a+b\;
#assert 27 'a=20+5-4;b=2*3;a+b;'
#assert 5 "2+3;"
#assert 7 "a = (20 + 1) / 3;"
#assert 7 "val = (20 + 1) / 3;"
#assert 5 "return 5;"
#assert 5 "if (1) 5;"
#assert 6 "if (1) 6; else 1;"
#assert 1 "if (0) 0; else 1;"
#assert 0 "a=5;while(a > 0) a=a-1; return a;"
#assert 0 "for(i = 5; 0 < i; i = i - 1); return i;"
#assert 3 "for(i = 5; 0 < i; i = i - 1) { if (i == 3) return i;}"
#assert 5 "for(i = 5; 0 < i; i = i - 1) { if (i == 3) {i = i + 2; return i;}}"
#assert 0 "func(a, b); return 0;" # fail
#assert 0 "func(a, b){a+b;}; return 0;"
#assert 7 "func(){a = 5; b = 2; return a+b;};"

echo OK
