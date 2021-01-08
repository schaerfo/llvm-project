// RUN: %check_clang_tidy %s bugprone-enum-cast %t

enum E1{A1, B1, C1};
enum E2{A2, B2};

void f() {
  int a;
  E1 e1;
  E2 e2;

  static_cast<E1>(3);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an int to an enum [bugprone-enum-cast]
  E1(3);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an int to an enum [bugprone-enum-cast]
  (E1)3;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an int to an enum [bugprone-enum-cast]

  static_cast<E1>(a);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an int to an enum [bugprone-enum-cast]
  (void) E1(a);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: You are casting an int to an enum [bugprone-enum-cast]
  (E1)a;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an int to an enum [bugprone-enum-cast]

  static_cast<E1>(A1);
  (void) E1(A1);
  (E1)A1;

  static_cast<E1>(e1);
  (void) E1(e1);
  (E1)e1;

  static_cast<E1>(A2);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an enum to a different enum [bugprone-enum-cast]
  (void) E1(A2);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: You are casting an enum to a different enum [bugprone-enum-cast]
  (E1)A2;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an enum to a different enum [bugprone-enum-cast]

  static_cast<E1>(e2);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an enum to a different enum [bugprone-enum-cast]
  (void) E1(e2);
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: You are casting an enum to a different enum [bugprone-enum-cast]
  (E1)e2;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: You are casting an enum to a different enum [bugprone-enum-cast]
}
