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

assert 0 "main(){return 0;}"
assert 2 "main(){return 2;}"
assert 5 "main(){a = 2; b = 3; return a + b;}"
assert 3 "main(a, b){return 3;}"
assert 5 "main(a, b){a=2;b=3; return a+b;}"
assert 3 "main(a, b){return 3;}"
assert 7 "main(){return hoge();} hoge(){return 7;}"
assert 4 "main(){return hoge(4);} hoge(a){return a;}"
assert 3 "main(){return huga(1, 2);} huga(a, b){return a + b;}"
assert 6 "main(){return hoge(1, 2, 3);} hoge(a, b, c){return a + b + c;}"
assert 4 "main(){return hoge(1, 2, 3);} hoge(a, b, c){return a + c;}"
assert 1 "main(){return hoge(1, 2, 3);} hoge(a, b, c){return a;}"
assert 55 "main(){return sum(10);} sum(n){if(n<0) return 0; return n + sum(n-1);}"
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
