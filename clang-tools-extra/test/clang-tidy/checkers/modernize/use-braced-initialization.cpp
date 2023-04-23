// RUN: %check_clang_tidy %s modernize-use-braced-initialization %t

// FIXME: Add something that triggers the check here.
//void f();
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'f' is insufficiently awesome [modernize-use-braced-initialization]

// FIXME: Verify the applied fix.
//   * Make the CHECK patterns specific enough and try to make verified lines
//     unique to avoid incorrect matches.
//   * Use {{}} for regular expressions.
// CHECK-FIXES: {{^}}void awesome_f();{{$}}

// FIXME: Add something that doesn't trigger the check here.
//void awesome_f2();

int getInt();

struct Base {
  Base() = default;
  Base(float);
  Base(double);
};

struct Default {};

struct Derived: Base {
  Derived(float f) : Base(f) {} // flag
  Derived(double d) : Base{d} {} // pass
};

struct M {
    M(float f): b_(f) {} // flag
    M(double d): b_{d} {} // pass
    M(): b_() {} // flag
    M(int): b_{} {} // pass

    Base b_;
    Default d_;
};

struct S {
    S() = default;
    S(int i): i_(i) {} // flag
    S(long l): l_{l} {} // pass

    int i_ = getInt(); // flag
    int j_ = {}; // indecisive
    int k_ = {43}; // indecisive
    long l_{0l}; // pass
    int m_;  // indecisive
    int n_{}; // indecisive
};

void f(){
    S a(0); // flag
    S b = 0; // pass
    S c{0}; // pass
    S d; // pass
    S e{}; // pass
}

void g() {
    int a(0); // flag
    int b = 0; // pass
    int c{0}; // pass
    int d; // pass
}

void take(S s);

void h(){
  take(S()); // flag
  take(S{}); // pass
}
