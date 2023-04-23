// RUN: %check_clang_tidy %s modernize-use-braced-initialization %t

int getInt();

struct Base {
  Base() = default;
  Base(float);
  Base(double);
};

struct Default {};

struct Derived: Base {
  Derived(float f) : Base(f) {} // flag
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: Use braced initialization in object construction [modernize-use-braced-initialization]
  Derived(double d) : Base{d} {} // pass
};

struct M {
    M(float f): b_(f) {} // flag
    // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: Use braced initialization in object construction [modernize-use-braced-initialization]
    M(double d): b_{d} {} // pass
    M(): b_() {} // flag
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: Use braced initialization in object construction [modernize-use-braced-initialization]
    M(int): b_{} {} // pass

    Base b_;
    Default d_;
};

struct S {
    S() = default;
    S(int i): i_(i) {} // flag
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: Use braced initialization in constructor initializer [modernize-use-braced-initialization]
    S(long l): l_{l} {} // pass

    int i_ = getInt(); // flag
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: Use braced initialization in field declaration [modernize-use-braced-initialization]
    int j_ = {}; // indecisive
    int k_ = {43}; // indecisive
    long l_{0l}; // pass
    int m_;  // indecisive
    int n_{}; // indecisive
};

void f(){
    S a(0); // flag
    // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: Use braced initialization in variable initialization [modernize-use-braced-initialization]
    S b = 0; // pass
    S c{0}; // pass
    S d; // pass
    S e{}; // pass
}

void g() {
    int a(0); // flag
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: Use braced initialization in variable initialization [modernize-use-braced-initialization]
    int b = 0; // pass
    int c{0}; // pass
    int d; // pass
}

void take(S s);

void h(){
  take(S()); // flag
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: Use braced initialization in object construction [modernize-use-braced-initialization]
  take(S{}); // pass
}
