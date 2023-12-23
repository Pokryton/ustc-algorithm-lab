package main

import (
	"fmt"
	"testing"
	"unicode/utf8"
)

type TestCase struct {
	a, b string
	want string
}

func TestEmpty(t *testing.T) {
	tests := []TestCase{
		{"abc", "", ""},
		{"", "abc", ""},
		{"", "", ""},
	}

	runTests(t, tests)
}

func TestSample(t *testing.T) {
	tests := []TestCase{
		{"abcde", "ace", "ace"},
		{"abc", "def", ""},
	}

	runTests(t, tests)
}

func TestSample2(t *testing.T) {
	tests := []TestCase{{
		"abcdefghigkllmnopqrssstuvwxyz",
		"aabcdefghigklmnopopqrsttuvwxyz",
		"abcdefghigklmnopqrstuvwxyz",
	}}

	runTests(t, tests)
}

func TestOther(t *testing.T) {
	tests := []TestCase{
		{"hallo", "hello", "hllo"},
		{"abc1234", "abij3", "ab3"},
	}

	runTests(t, tests)
}

func (tt TestCase) run(t *testing.T) {
	count := utf8.RuneCountInString(tt.want)

	if lcs := Lcs(tt.a, tt.b); lcs != tt.want {
		t.Errorf("Lcs(%#v, %#v) = %#v; want = %#v", tt.a, tt.b, lcs, tt.want)
	}

	if l := lcsRuneCount(tt.a, tt.b); l != count {
		t.Errorf("lcsRuneCount(%#v, %#v) = %d; want = %d", tt.a, tt.b, l, count)
	}

	if l := LcsRuneCount(tt.a, tt.b); l != count {
		t.Errorf("LcsRuneCount(%#v, %#v) = %d; want = %d", tt.a, tt.b, l, count)
	}
}

func runTests(t *testing.T, tests []TestCase) {
	for _, tt := range tests {
		testname := fmt.Sprintf("a=%#v,b=%#v", tt.a, tt.b)
		t.Run(testname, tt.run)
	}
}
