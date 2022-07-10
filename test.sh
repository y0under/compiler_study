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

assert 27 a=20+5-4\;b=2*3\;a+b\;
assert 27 'a=20+5-4;b=2*3;a+b;'
assert 5 "2+3;"
assert 7 "a = (20 + 1) / 3;"
assert 7 "val = (20 + 1) / 3;"
assert 5 "return 5;"
assert 5 "if (1) 5;"
assert 6 "if (1) 6; else 1;"
assert 1 "if (0) 0; else 1;"

echo OK
